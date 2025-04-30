#!/bin/bash

bold="\u001b[1m"
yellow_text="\u001b[33m"
reset_style="\e[m"

echo -e "${bold}${yellow_text}Script for generating coloured with same resolution, energy and number of colours, but different relevance."

echo -e ${reset_style} 

echo -e "${bold}Grid size.${reset_style}"
echo "Insert the width: "
read width
echo "Insert the height: "
read height


