#!/bin/bash
#set -x

declare -i score=0
declare -i increment=1

test_command(){
   # $1: test file name
   echo "Testing command: " $1 
   sh     < $1 > bash_output 2>/dev/null
   ./msh  < $1 > msh_output  2>/dev/null
   if diff -i -w -q --ignore-all-space bash_output msh_output > /dev/null; then
    echo " OK"
    echo -n "1 " >> excel
    score=$(( score + increment ))  
   else
    echo " Error"
    echo -n "0 " >> excel
    echo " === Test === "
         cat  $1
    echo " === ./msh Output === "
    cat  msh_output
    echo " === Expected Output === "     
    cat  bash_output
   fi
   rm -f bash_output msh_output res.txt
}

test_background(){
   # $1: test file name
   sh     < $1 > bash_output 2>/dev/null
   ./msh  < $1 > msh_output  2>/dev/null
   tail -n 1 bash_output > bash_b
   tail -n 1 msh_output > msh_m
   if diff -i -w -q --ignore-all-space bash_b msh_m > /dev/null; then
    echo " OK"
    echo -n "1 " >> excel
    score=$(( score + increment ))
   else
    echo " Error"
    echo -n "0 " >> excel
    echo " === Test === "
         cat  $1
    echo " === ./msh Output === "
    cat  msh_m
    echo " === Expected Output === "     
    cat  bash_b
   fi
   rm -f bash_output msh_output res.txt bash_b msh_m
}

test_redirection(){
   # $1: test file name
   sh     < $2
   ./msh  < $1 2>/dev/null
   if diff -i -w -q --ignore-all-space bash_output msh_output > /dev/null; then
    echo " OK"
    echo -n "1 " >> excel
    score=$(( score + increment ))
   else
    echo " Error"
    echo -n "0 " >> excel
    echo " === Test === "
         cat  $1
    echo " === ./msh Output === "
    cat  msh_output
    echo " === Expected Output === "     
    cat  bash_output
   fi
   rm -f bash_output msh_output res.txt
}

test_error_redirection(){
   # $1: test file name
   sh     < $2 >/dev/null 2> bash_output
   ./msh  < $1 >/dev/null 2>/dev/null
   if diff -i -w -q --ignore-all-space bash_output msh_output > /dev/null; then
    echo " OK"
    echo -n "1 " >> excel
    score=$(( score + increment ))
   else
    echo " Error"
    echo -n "0 " >> excel
    echo " === Test === "
         cat  $1
    echo " === ./msh Output === "
    cat  msh_output
    echo " === Expected Output === "     
    cat  bash_output
   fi
   rm -f bash_output msh_output res.txt
}

test_error_redirection_e(){
   # $1: test file name
   sh     < $2 >/dev/null 2> bash_output
   ./msh  < $1 >/dev/null 2> msh_output 
   tail -n 1 msh_output > msh_output2
   if [ -s ${msh_output2} ]
   then 
         echo " OK"
         echo -n "1 " >> excel
    score=$(( score + increment ))
   else
         echo " Error"
         echo -n "0 " >> excel
         echo " === Test === "
         cat  $1
         echo " === ./msh Output === "
         cat  msh_output2
         echo " === Expected Output === "
         cat  bash_output
   fi
   rm -f bash_output msh_output res.txt msh_output2
}

test2(){
   
  ./msh  < $1 > msh_output  2>/dev/null

   if diff -i -w -q --ignore-all-space $2 msh_output > /dev/null; then
    echo " OK"
    echo -n "1 " >> excel
    score=$(( score + increment ))
   else
    echo " Error"
    echo -n "0 " >> excel
    echo " === Test === "
         cat  $1
    echo " === ./msh Output === "
    cat  msh_output
    echo " === Expected Output === "     
    cat  $2
   fi
}

test2_error(){
   
  ./msh  <$1 2> msh_output  >/dev/null
  cat msh_output | head -n 4 > msh2
  mv msh2 msh_output 
   if diff -i -w -q --ignore-all-space $2 msh_output; then
    echo " OK"
    echo -n "1 " >> excel
    score=$(( score + increment ))
   else
    echo " Error"
    echo -n "0 " >> excel
    echo " === Test === "
         cat  $1
    echo " === ./msh Output === "
    cat msh_output
    echo " === Expected Output === "     
    cat  $2
   fi   
}

test3(){
   
  ./msh  < $1 2> msh_output  >/dev/null

   if diff -i -w -q --ignore-all-space $2 msh_output > /dev/null; then
    if diff -i -w -q $3 $4 > /dev/null; then
      echo " OK"
      echo -n "1 " >> excel
      score=$(( score + increment ))
    else
      echo " Error"
      echo -n "0 " >> excel
      echo " === Test === "
      cat  $1
      echo "Files are different"
    fi
   else
    echo " Error"
    echo -n "0 " >> excel
    echo " === Test === "
         cat  $1
    echo " === ./msh Output === "
    cat  msh_output
    echo " === Expected Output === "     
    cat  $2
   fi
}

test_time_error(){
   
  ./msh < $1 > msh_output 2>/dev/null

  if diff -i -w -q --ignore-all-space $2 msh_output > /dev/null; then
   echo " OK"
   echo -n "1 " >> excel
   score=$(( score + increment ))
  else
   echo " Error"
   echo -n "0 " >> excel
   echo " === Test === "
   cat  $1
   echo "Files are different"
 fi
}

test_calc(){
   ENVIR=`cat var`
   if [ $ENVIR = "Acc=13" ]; then
      echo " OK"
      echo -n "1 " >> excel
      score=$(( score + increment ))
   else
      echo " Error"
      echo -n "0 " >> excel
      echo " === Test === "
      echo "Environment variable usage"
      echo " === ./msh Output === "
      echo  $ENVIR
      echo " === Expected Output === "      
      echo "Acc=13"
   fi
}

test2_calc(){
   
  ./msh  < $1 2> msh_output1  >/dev/null
  ./msh  < $2 2> msh_output2  >/dev/null
  var1=`diff -i -w -q $3 msh_output1 > /dev/null`
  var2=`diff -i -w -q $3 msh_output2 > /dev/null`
  var3=`diff -i -w -q $4 msh_output1 > /dev/null`
  var4=`diff -i -w -q $4 msh_output2 > /dev/null` 
   if [ ${var1} || ${var2}  || ${var3} || ${var4} ]; then
    echo " OK"
    echo -n "1 " >> excel
    score=$(( score + increment ))
   else
    echo " Error"
    echo -n "0 " >> excel
    echo " === Test === "
         cat  $1 $2
    echo " === ./msh Output === "
    cat  msh_output1 msh_output2
    echo " === Expected Output === "     
    cat  $3 $4
   fi
}

test2_time(){

  ./msh  < $1 2> msh_output >/dev/null

  (sed -e 's/MSH>>//g' ./msh_output | grep -Eq  ^[0-9]{2}:[0-9]{2}:[0-9]{2}$) && echo "1" > output2

   if [ -s output2 ]; then
         echo " OK"
         echo -n "1 " >> excel
    score=$(( score + increment ))
   else
         echo " Error"
         echo -n "0 " >> excel
         echo " === Test === "
         cat  $1
         echo " === ./msh Output === "
         cat  msh_output
         echo " === Expected Output === "
         echo "HH:MM:SS Format"
   fi
}

test_pwd_error(){
   # $1: test file name
   ./msh  < $1 2> msh_output  >/dev/null
   tail -n 2 msh_output | head -n1 | tee msh_output_2 >/dev/null
   if diff -i -w -q --ignore-all-space $2 msh_output_2 > /dev/null; then
    echo " OK"
    echo -n "1 " >> excel
    score=$(( score + increment ))
   else
    echo " Error"
    echo -n "0 " >> excel
    echo " === Test === "
         cat  $1
    echo " === ./msh Output === "
    cat  msh_output_2
    echo " === Expected Output === "     
    cat  $2
   fi
   rm -f bash_output msh_output res.txt msh_output_2
}


clear

echo "*** TESTING THE MINISHELL ***"

 
TDIR="/tmp/ssoo/p2_testdir"

mkdir -p $TDIR

cp $1 $TDIR
cp Makefile $TDIR
cp *.so $TDIR

cd $TDIR

unzip $1

cp authors.txt ../authors.txt

echo "File : $1"

#cd $TDIR


if [ ! -f authors.txt ]; then
   echo "Error: authors.txt file not found"
   cd ..
   rm -r $TDIR
   exit
fi
if [ ! -f msh.c ]; then
   echo "Error: msh.c file not found"
   cd ..
   rm -r $TDIR
   exit
fi


make clean 2> /dev/null > /dev/null
make       2> /dev/null > /dev/null


echo "Compiling"
if [ ! -f msh ]; then
   echo "Error: Compilation failed"
   cd ..
#  rm -r $TDIR
   exit
else
   echo "OK"
fi



#Test preparation
cat > foo.txt << EOF 
123432
67890
12345
46789
90a
10a
EOF

#test

#simple
echo "wc -l  foo.txt"                  > test1
echo "wc -l  < foo.txt"                > test2
echo "cat foo.txt > msh_output"        > test3
echo "cat foo.txt > bash_output"       > test3.res
echo "cat non_existent.txt !> res.txt"     > test4
cat non_existent.txt 2> res.txt            > test4.res

#1 pipe
echo "cat foo.txt | grep a"                     >test5
echo "grep 1      | grep a < foo.txt"           >test6
echo "10a"                                      >test6.res
echo "cat foo.txt | grep a > msh_output"        >test7
echo "cat foo.txt | grep a > bash_output"       >test7.res
echo "cat non_existent.txt | grep a !> msh_output"  >test7error
echo "cat non_existent.txt | grep a"                >test7error.res

#2 pipes
echo "cat foo.txt | grep a | grep 1"                    >test8
echo "grep 1      | grep a | wc -l < foo.txt"           >test9
echo "1"                                                >test9.res
echo "cat foo.txt | grep a | wc -l > msh_output"        >test10
echo "cat foo.txt | grep a | wc -l > bash_output"       >test10.res
echo "cat non_existent.txt | grep a | wc -l !> msh_output"  >test10Error
echo "cat non_existent.txt | grep a | wc -l"                >test10Error.res

#N pipes
echo "cat foo.txt | grep 1 | grep 2 | grep 3 | grep 4"  >test11

#Background
echo "sleep 1 &"                > test21
echo "ps | grep sleep | wc -l"  >> test21
echo "sleep 1"                  > test21
echo "sleep 1 &"                > test21.res
echo "ps | grep sleep | wc -l"  >> test21.res

#mycalc
echo "mycalc 3 add -8"                >calc1
#echo "MSH>> mycalc 3 add -8"         >calc1.res
echo "MSH>>[OK] 3 + -8 = -5; Acc -5"  >>calc1.res
echo "MSH>>"                          >>calc1.res
echo "mycalc 3 add -8"                >calc2
echo "mycalc 5 add 13"                >>calc2
echo "env | grep Acc > var"           >>calc2

#echo "MSH>> mycalc 3 add -8"         >calc2.res
echo "MSH>>[OK] 3 + -8 = -5; Acc -5"  >>calc2.res
#echo "MSH>> mycalc 5 add 13"         >>calc2.res
echo "MSH>>[OK] 5 + 13 = 18; Acc 13"  >>calc2.res
echo "MSH>>MSH>>"                     >>calc2.res

echo "mycalc 3 mul 8"                >calc3
#echo "MSH>> mycalc 10 mod 7"        > calc3.res
echo "MSH>>[OK] 3 * 8 = 24"          >>calc3.res
echo "MSH>>"                         >>calc3.res

echo "mycalc 10 div 7"                >calc4
#echo "MSH>> mycalc 10 mod 7"         > calc4.res
echo "MSH>>[OK] 10 / 7 = 1; Remainder 3"  >>calc4.res
echo "MSH>>"                          >>calc4.res

echo "mycalc 10 mod 7"                 >calc5
echo "[ERROR] The command structure is mycalc <operand_1> <add/mul/div> <operand_2>"  >calc5.res

echo "mycalc mycalc 8 mas"            >calc6
echo "[ERROR] The command structure is mycalc <operand_1> <add/mul/div> <operand_2>"  >calc6.res

#mytime
echo "mytime" >time1



#Basic
#single command
echo -n "Test1: 1 command -> "
test_command test1
echo -n "Test2: 1 command + input redirection -> "
test_command test2
echo -n "Test3: 1 command + output redirection -> "
test_redirection test3 test3.res   
echo -n "Test4: 1 command + error redirection -> "
test2 test4 test4.res


echo -n "Test5: 2 commands -> "
test_command test5
echo -n "Test6: 2 commands + input redirection -> "
test2 test6 test6.res
echo -n "Test7: 2 commands + output redirection -> "
test_redirection test7 test7.res
echo -n "Test8: 2 commands + error redirection -> "
test_error_redirection_e test7error test7error.res


echo -n "Test9: 3 commands -> "
test_command test8
echo -n "Test10: 3 commands + input redirection -> "
test2 test9 test9.res
echo -n "Test11: 3 commands + output redirection -> "
test_redirection test10 test10.res
echo -n "Test12: 3 commands + error redirection -> "
test_error_redirection_e test10Error test10Error.res


echo -n "Test13: N commands -> "
test_command test11

#internal commands
echo -n "Test14: Calculator 1 -> "
test2_error calc1 calc1.res  
echo -n "Test15: Calculator 2 -> "
test2_error calc2 calc2.res
echo -n "Test16: Calculator 3 -> "
test2_error calc3 calc3.res 
echo -n "Test17: Calculator 4 -> "
test2_error calc4 calc4.res  
echo -n "Test18: Calculator 5 -> "
test2 calc5 calc5.res
echo -n "Test19: Calculator 6 -> "
test2 calc6 calc6.res
echo -n "Test20: Calculator \$Acc -> "
test_calc

echo -n "Test21: time 1 -> "
test2_time time1


#background
echo -n "Test22: Background -> "
test_command test21 test21.res

echo "Test summary: -> "
final_score=$((echo scale=2 ; echo $score*10 / 21) | bc )
echo "Score: $final_score" 
echo $final_score > ../score.txt
#cat excel

rm -fr foo.txt test* excel tmp/msh.c msh_output time* pwd1 pwd2*

#exit directory
cd ..
rm -r $TDIR