#!/bin/bash

echo "Script for generating and selecting all partitions of a positive integer n.\n"

mkdir ./partitions

echo "Generating all partitions"
g++ nexpar.cpp quantifying_information.cpp nexpar_functions.cpp -o nexpar.out
./nexpar.out

echo "Resoultion/Relevance plot"
python res_rel.py

echo "Selecting all partitions in a given resolution interval"
g++ selected_partitions.cpp quantifying_information.cpp nexpar_functions.cpp -o selected_partitions.out
./selected_partitions.out

