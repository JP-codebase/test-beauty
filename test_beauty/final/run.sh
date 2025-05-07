#!/bin/bash

reset_style="\e[m"

bold="\u001b[1m"

green_text="\e[32m"
yellow_text="\e[33m"
red_text="\e[31m"

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



./generate_partitions.out ${size}
