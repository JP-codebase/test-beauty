#!/bin/bash

# --------------- Output Formatting -------------

RESET_STYLE="\033[m"

BOLD="\u001b[1m"

GREEN_TEXT="\033[32m"
YELLOW_TEXT="\033[33m"
RED_TEXT="\033[31m"


# --------------- Starting ----------------------

echo -e "${BOLD}${YELLOW_TEXT}Script for generating coloured with same resolution, energy and number of colours, but different relevance.${RESET_STYLE} "


echo 
echo -e "${BOLD}Grid size.${RESET_STYLE}"


# Get width
while true; do
    read -p "Insert the width: " width

    # Check if input is an integer
    if [[ "$width" =~ ^-?[0-9]+$ ]]; then
      if (( width > 0 )); then
        break
      else 
        echo -e "${RED_TEXT}Error: Please enter a positive integer.${RESET_STYLE}"
      fi
    else
      echo -e "${RED_TEXT}Error: The input is not an integer.${RESET_STYLE}"
    fi
done

# Get height
while true; do
    read -p "Insert the height: " height

    # Check if input is an integer
    if [[ "$height" =~ ^-?[0-9]+$ ]]; then
      if (( height > 0 )); then
        break
      else 
        echo -e "${RED_TEXT}Error: Please enter a positive integer.${RESET_STYLE}"
      fi
    else
      echo -e "${RED_TEXT}Error: The input is not an integer.${RESET_STYLE}"
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
  echo -e "${BOLD}${RED_TEXT}Execution interruped.${RESET_STYLE}"
  exit 1
fi


# ---------------- Res_Rel Plot ----------------------

echo -e "${BOLD}${GREEN_TEXT}Plotting Resolution-Relevance.${RESET_STYLE} "

python res_rel_plot.py ${size} 2> >(grep -v '^MESA-INTEL:' >&2) &
# python res_rel_plot.py ${size} ${res_min} ${res_max} 

echo
echo "--------------------------------------------------"
echo

# ---------------------- Select partitions -------------------------

echo -e "${BOLD}${YELLOW_TEXT}Insert the range of resolution.${RESET_STYLE} "

# Get resolution_min
while true; do
    read -p "Insert the resolution minimum: " res_min

    # Check if input is a number
    if [[ "$res_min" =~ ^[+-]?([0-9]+(\.[0-9]+)?|\.[0-9]+)$ ]]; then
      if (( $(bc -l <<< "$res_min >= 0") )); then
        break
      else 
        echo -e "${RED_TEXT}Error: Please enter a positive number.${RESET_STYLE}"
      fi
    else
      echo -e "${RED_TEXT}Error: The input is not a number.${RESET_STYLE}"
    fi
done

# Get resolution_max
while true; do
    read -p "Insert the resolution maximum: " res_max

    # Check if input is a number
    if [[ "$res_max" =~ ^[+-]?([0-9]+(\.[0-9]+)?|\.[0-9]+)$ ]]; then
      if (( $(bc -l <<< "$res_max > 0") )); then
          if (( $(bc -l <<< "$res_max >= $res_min") )); then
              break
          else
            echo -e "${RED_TEXT}Error: Max must be greater than min:${RESET_STYLE} ${res_min}"
          fi 
      else 
        echo -e "${RED_TEXT}Error: Please enter a positive number.${RESET_STYLE}"
      fi
    else
      echo -e "${RED_TEXT}Error: The input is not a number.${RESET_STYLE}"
    fi
done


./select_partitions_resolution.out ${size} ${res_min} ${res_max}


if [ $? != 0 ]; then
  echo -e "${BOLD}${RED_TEXT}Execution interruped.${RESET_STYLE}"
  exit 1
fi

# ---------------- Res_Rel Plot ----------------------

echo -e "${BOLD}${GREEN_TEXT}Plotting Resolution-Relevance.${RESET_STYLE} "

python res_rel_plot.py ${size} ${res_min} ${res_max} 2> >(grep -v '^MESA-INTEL:' >&2) &
# python res_rel_plot.py ${size} ${res_min} ${res_max} 

echo
echo "--------------------------------------------------"
echo
