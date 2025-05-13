#!/bin/bash

# --------------- Output Formatting -------------

RESET_STYLE="\033[m"

BOLD="\u001b[1m"

GREEN_TEXT="\033[32m"
YELLOW_TEXT="\033[33m"
RED_TEXT="\033[31m"


# --------------- Starting ----------------------

mkdir -p ./partitions
mkdir -p ./images

echo -e "${BOLD}${YELLOW_TEXT}Script for generating coloured with same resolution, energy and number of colours, but different relevance.${RESET_STYLE} "


# --------------- Grid Size ----------------------

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


# --------------- Generate partitions -------------

echo
echo "--------------------------------------------------"
echo

./generate_partitions.out ${size}


if [ $? != 0 ]; then
  echo -e "${BOLD}${RED_TEXT}Execution interruped.${RESET_STYLE}"
  exit 1
fi


# ---------------- Res_Rel Plot ----------------------

echo 
echo -e "${BOLD}${GREEN_TEXT}Plotting Resolution-Relevance.${RESET_STYLE}"
echo -e "Wait. A plot will be shown."

python res_rel_plot.py ${size} 2> >(grep -v '^MESA-INTEL:' >&2) &
# python res_rel_plot.py ${size} ${res_min} ${res_max} 

sleep 0.8


# ---------------------- Select partitions -------------------------

echo
echo "--------------------------------------------------"
echo

echo -e "${BOLD}${YELLOW_TEXT}Insert the resolution range.${RESET_STYLE} "

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


./select_partitions.out ${size} ${res_min} ${res_max}


if [ $? != 0 ]; then
  echo -e "${BOLD}${RED_TEXT}Execution interruped.${RESET_STYLE}"
  exit 1
fi

# ---------------- Res_Rel Plot : Selected by Resolution ----------------------

echo
echo "--------------------------------------------------"
echo

echo -e "${BOLD}${GREEN_TEXT}Plotting Resolution-Relevance.${RESET_STYLE}"
echo "Wait. A plot will be shown"

python res_rel_plot.py ${size} ${res_min} ${res_max} 2> >(grep -v '^MESA-INTEL:' >&2) &
# python res_rel_plot.py ${size} ${res_min} ${res_max} 

sleep 0.8


# ---------------- Selecting Partition by Number of Colors ----------------------

echo
echo "--------------------------------------------------"

echo

echo -e "${BOLD}Colors.${RESET_STYLE} "

# Get number of colors
while true; do
    read -p "Select the number of colors: " n_colors

    # Check if input is an integer
    if [[ "$n_colors" =~ ^-?[0-9]+$ ]]; then
      if (( n_colors > 0 )); then
        break
      else 
        echo -e "${RED_TEXT}Error: Please enter a positive integer.${RESET_STYLE}"
      fi
    else
      echo -e "${RED_TEXT}Error: The input is not an integer.${RESET_STYLE}"
    fi
done

# Get number of partitions
while true; do
    read -p "Select the number of partitions: " n_partitions

    # Check if input is an integer
    if [[ "$n_partitions" =~ ^-?[0-9]+$ ]]; then
      if (( n_partitions > 0 )); then
        break
      else 
        echo -e "${RED_TEXT}Error: Please enter a positive integer.${RESET_STYLE}"
      fi
    else
      echo -e "${RED_TEXT}Error: The input is not an integer.${RESET_STYLE}"
    fi
done

./select_partitions_colors.out ${size} ${res_min} ${res_max} ${n_colors} ${n_partitions}

if [ $? != 0 ]; then
  echo -e "${BOLD}${RED_TEXT}Execution interruped.${RESET_STYLE}"
  exit 1
fi

# ---------------- Selecting Partition by Number of Colors ----------------------

echo
echo "--------------------------------------------------"

echo

echo -e "${BOLD}Generate colored grids.${RESET_STYLE} "

./generate_colored_grids.out ${width} ${height} ${res_min} ${res_max} ${n_colors} 

if [ $? != 0 ]; then
  echo -e "${BOLD}${RED_TEXT}Execution interruped.${RESET_STYLE}"
  exit 1
fi


# ---------------- Display Colored Grids ----------------------

echo 
echo -e "${BOLD}${GREEN_TEXT}Creating the colored grids images.${RESET_STYLE}"
echo -e "Wait. A plot will be shown."

python display_colored_grids.py ${width} ${height} ${res_min} ${res_max} ${n_colors} ${n_partitions} 
# python display_colored_grids.py ${width} ${height} ${res_min} ${res_max} ${n_colors} ${n_partitions} 2> >(grep -v '^MESA-INTEL:' >&2) &
# python res_rel_plot.py ${size} ${res_min} ${res_max} 

sleep 0.8

# ---------------- Res_Rel Plot : Selected by Number of Colors ----------------------

# echo 
# echo -e "${BOLD}${GREEN_TEXT}Plotting Resolution-Relevance.${RESET_STYLE}"
# echo -e "Wait. A plot will be shown."
#
# python res_rel_colors.py ${size} ${res_min} ${res_max} ${n_colors} 2> >(grep -v '^MESA-INTEL:' >&2) &
# # python res_rel_plot.py ${size} ${res_min} ${res_max} 
#
# sleep 0.8
