#!/bin/bash

echo "Script for generating and selecting all partitions of a positive integer 'n'."

mkdir ./partitions

echo "Generating all partitions"
echo "Insert 'n' : "
read n

g++ nexpar.cpp quantifying_information.cpp nexpar_functions.cpp -o nexpar.out
./nexpar.out $n

echo
echo "--------------------------------------------------"
echo

echo "Resoultion/Relevance plot"
python res_rel.py $n

echo
echo "--------------------------------------------------"
echo

echo "Selecting all partitions in a given resolution interval"
echo "Resolution min value"
read res_min
echo "Resolution max value"
read res_max

g++ select_partitions.cpp quantifying_information.cpp nexpar_functions.cpp -o select_partitions.out
./select_partitions.out $n $res_min $res_max

echo
echo "--------------------------------------------------"
echo

python res_rel.py $n $res_min $res_max
