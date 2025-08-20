#!/bin/bash


d=$(date +%F)

echo "Input hours as format: 0910 for 9.10."
echo "Add total amount of breaks, under an hour would be 0030 for 30 minutes.\n"
echo " "

read -p "start: " s
read -p "stop: " e
read -p "breaks: " b

# Interpret values as decimal and not octal
s=$((10#$s))
e=$((10#$e)) 

e=$((e - b))
duration=$((e - s))


echo "$d,$s,$e,$duration" >> logged_times.csv
 
