#!/usr/bin/env bash

file="rn_mle_adventitia_1000_w_angle.txt"


while read -r line; do
  #Reading each line
  echo "====================================="
  echo "running $line"
  echo "====================================="
  ../../optimize postprocess_run_all_w_angle.i layer=adventitia fileName=sto_w_angle_adventitia sample=0 $line </dev/null

done < $file
