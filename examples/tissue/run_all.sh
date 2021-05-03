#!/usr/bin/env bash

file="rn_mle_3000.txt"


while read -r line; do
  #Reading each line
  echo "====================================="
  echo "running $line"
  echo "====================================="
  ../../optimize postprocess.i $line </dev/null

done < $file
