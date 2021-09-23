#!/usr/bin/env bash

# file="rn_mle_adventitia_5000.txt"
#
#
# while read -r line; do
#   #Reading each line
#   echo "====================================="
#   echo "running $line"
#   echo "====================================="
#   ../../optimize postprocess_run_all.i layer=adventitia fileName=stochastic_adventitia sample=12 angle_1=55.3 angle_2=145.3 $line </dev/null
#
# done < $file

### with angle randomization
#!/usr/bin/env bash

file="rn_mle_intima_1000_w_angle.txt"


while read -r line; do
  #Reading each line
  echo "====================================="
  echo "running $line"
  echo "====================================="
  ../../optimize postprocess_run_all_w_angle.i layer=intima fileName=sto_w_angle_intima sample=0 $line </dev/null

done < $file

# file="guess.txt"
#
#
# while read -r line; do
#   #Reading each line
#   echo "====================================="
#   echo "running $line"
#   echo "====================================="
#   ../../optimize tissue.i sample=10 angle_1=53.4 angle_2=143.4 layer=adventitia $line </dev/null
#
# done < $file
