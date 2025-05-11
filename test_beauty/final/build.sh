
# --------------------- Format Terminal Output -----------------------
reset_style="\e[m"

bold="\u001b[1m"

green_text="\e[32m"
yellow_text="\e[33m"
red_text="\e[31m"


# --------------------- Compilers -----------------------------

compiler="g++"
# compiler="clang++"


# --------------------- Start Compiling -----------------------

echo -e "Compiling ..."

echo -e "generate_partition.cpp : \c"
${compiler} generate_partitions.cpp ./headers/quantifying_information.cpp ./headers/nexpar_functions.cpp -o generate_partitions.out -DREAL_IS_DOUBLE 

if [ $? -eq 0 ]; then
  echo -e "${bold}${green_text}Compilation complete.${reset_style}"
else
  echo -e "${bold}${red_text}Compilation failed."
fi

echo -e "select_partitions.cpp : \c"
${compiler} select_partitions.cpp ./headers/quantifying_information.cpp ./headers/nexpar_functions.cpp -o select_partitions.out -DREAL_IS_DOUBLE 

if [ $? -eq 0 ]; then
  echo -e "${bold}${green_text}Compilation complete.${reset_style}"
else
  echo -e "${bold}${red_text}Compilation failed."
fi

echo
