#!/bin/bash

# --------------- Output Formatting -------------

reset_style="\e[m"

bold="\u001b[1m"

green_text="\e[32m"
yellow_text="\e[33m"
red_text="\e[31m"


# --------------- Starting ----------------------

echo -e "${bold}${yellow_text}Script for generating coloured with same resolution, energy and number of colours, but different relevance.${reset_style} "


echo 
echo -e "${bold}Grid size.${reset_style}"


while true; do
    read -p "Insert the width: " width

    # Check if input is an integer
    if [[ "$width" =~ ^-?[0-9]+$ ]]; then
      if (( width > 0 )); then
        break
      else 
        echo -e "${red_text}Error: Please enter a positive integer.${reset_style}"
      fi
    else
      echo -e "${red_text}Error: The input is not an integer.${reset_style}"
    fi
done

while true; do
    read -p "Insert the height: " height

    # Check if input is an integer
    if [[ "$height" =~ ^-?[0-9]+$ ]]; then
      if (( height > 0 )); then
        break
      else 
        echo -e "${red_text}Error: Please enter a positive integer.${reset_style}"
      fi
    else
      echo -e "${red_text}Error: The input is not an integer.${reset_style}"
    fi
done

let size=width*height
mkdir -p ./partitions


echo
echo "--------------------------------------------------"
echo

# --------------- Generate partitions -------------

./generate_partitions.out ${size}


if [ $? != 0 ]; then
  echo -e "${bold}${red_text}Execution interruped.${reset_style}"
  exit 1
fi


# --------------- Select partitions -------------

echo -e "${bold}${yellow_text}Insert the range of resolution.${reset_style} "

while true; do
    read -p "Insert the resolution minimum: " res_min

    # Check if input is a number
    if [[ "$res_min" =~ ^[+-]?([0-9]+(\.[0-9]+)?|\.[0-9]+)$ ]]; then
      if (( $(bc -l <<< "$res_min > 0") )); then
        break
      else 
        echo -e "${red_text}Error: Please enter a positive number.${reset_style}"
      fi
    else
      echo -e "${red_text}Error: The input is not a number.${reset_style}"
    fi
done

while true; do
    read -p "Insert the resolution maximum: " res_max

    # Check if input is a number
    if [[ "$res_max" =~ ^[+-]?([0-9]+(\.[0-9]+)?|\.[0-9]+)$ ]]; then
      if (( $(bc -l <<< "$res_max > 0") )); then
          if (( $(bc -l <<< "$res_max > $res_min") )); then
              break
          else
            echo -e "${red_text}Error: Max must be greater than min:${reset_style} ${res_min}"
          fi 
      else 
        echo -e "${red_text}Error: Please enter a positive number.${reset_style}"
      fi
    else
      echo -e "${red_text}Error: The input is not a number.${reset_style}"
    fi
done


./select_partitions_resolution.out ${size} ${res_min} ${res_max}


if [ $? != 0 ]; then
  echo -e "${bold}${red_text}Execution interruped.${reset_style}"
  exit 1
fi
