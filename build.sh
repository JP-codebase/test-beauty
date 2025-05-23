
# --------------------- Format Terminal Output -----------------------
reset_style="\e[m"

bold="\u001b[1m"

green_text="\e[32m"
yellow_text="\e[33m"
red_text="\e[31m"


# --------------------- Compilers -----------------------------

compiler="g++"
optimization_options="-O2"
# compiler="clang++"


# --------------------- Start Compiling -----------------------

echo -e "Compiling ..."


echo -e "generate_partition.cpp : \c"
${compiler} generate_partitions.cpp ./headers/quantifying_information.cpp ./headers/nexpar_functions.cpp -o generate_partitions.out -DREAL_IS_DOUBLE ${optimization_options}

if [ $? -eq 0 ]; then
  echo -e "${bold}${green_text}Compilation complete.${reset_style}"
else
  echo -e "${bold}${red_text}Compilation failed."
fi


echo -e "select_partitions.cpp : \c"
${compiler} select_partitions.cpp  -o select_partitions.out -DREAL_IS_DOUBLE ${optimization_options}

if [ $? -eq 0 ]; then
  echo -e "${bold}${green_text}Compilation complete.${reset_style}"
else
  echo -e "${bold}${red_text}Compilation failed."
fi


echo -e "select_partitions_colors.cpp : \c"
${compiler} select_partitions_colors.cpp  -o select_partitions_colors.out -DREAL_IS_DOUBLE ${optimization_options}

if [ $? -eq 0 ]; then
  echo -e "${bold}${green_text}Compilation complete.${reset_style}"
else
  echo -e "${bold}${red_text}Compilation failed."
fi


echo -e "generate_colored_grids.cpp : \c"
${compiler} generate_colored_grids.cpp ./headers/quantifying_information.cpp  -o generate_colored_grids.out -DREAL_IS_DOUBLE  ${optimization_options}

if [ $? -eq 0 ]; then
  echo -e "${bold}${green_text}Compilation complete.${reset_style}"
else
  echo -e "${bold}${red_text}Compilation failed."
fi
