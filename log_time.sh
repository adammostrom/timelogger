#!/bin/bash


d=$(date +%F)


read -p "start": s
read -p "stop:" e

duration= ${e#0}-${s#0}

echo "$d,$s,$e,$duration" >> logged_times.csv
