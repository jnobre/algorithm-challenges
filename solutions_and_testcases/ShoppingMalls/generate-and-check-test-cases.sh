#!/bin/bash

while [ $# -ge 1 ] ; do
    case $1 in
        --from-scratch) rm -f TestCases/*.out ;;
        --create-test-cases) rm -f TestCases/[2-9].in ;;
    esac
    shift
done

make

for (( n=1; n <= 6; n++ ))
do
    echo -e "\n TEST $n:"

     input="TestCases/${n}.in"
    output="TestCases/${n}.out"

    if [ ! -f ${input} ] ; then
        java -cp classes TestCaseGenerator >${input}
    fi

    if [ ! -f ${output} ] ; then
        echo ""
        echo java -cp classes JonSolution2
        time java -cp classes JonSolution2 <${input} >${output}
    fi

    echo ""
    echo java -cp classes JonSolution
    time java -cp classes JonSolution <${input} >kk.$$
    diff kk.$$ ${output} >/dev/null 2>&1
    if [ $? -ne 0 ] ; then
        echo "'diff' fails. Using the special checker."
        Solutions/JonSolution.exe ${input} kk.$$
        if [ $? -ne 0 ] ; then
            echo ""
            echo " Wrong output for test case ${n}  "
            echo ""
        fi
    fi

    echo ""
    echo Solutions/JonSolution.exe
     time Solutions/JonSolution.exe <${input} >kk.$$
     diff kk.$$ ${output} >/dev/null 2>&1
     if [ $? -ne 0 ] ; then
         echo "'diff' fails. Using the special checker."
         Solutions/JonSolution.exe ${input} kk.$$
         if [ $? -ne 0 ] ; then
             echo ""
             echo " Wrong output for test case ${n}  "
             echo ""
         fi
     fi

    echo ""
    echo Solutions/solution_paco.exe
    time Solutions/solution_paco.exe <${input} >kk.$$
    diff kk.$$ ${output} >/dev/null 2>&1
    if [ $? -ne 0 ] ; then
        echo "'diff' fails. Using the special checker."
        Solutions/JonSolution.exe ${input} kk.$$
        if [ $? -ne 0 ] ; then
            echo ""
            echo " Wrong output for test case ${n}  "
            echo ""
        fi
    fi

done

rm -f kk.$$

make clean
