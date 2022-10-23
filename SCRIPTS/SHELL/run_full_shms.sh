#! /bin/bash

# Which spectrometer are we analyzing.
spec=${0##*_}
spec=${spec%%.sh}
SPEC=$(echo "$spec" | tr '[:lower:]' '[:upper:]')
if [ ${SPEC} == "SHMS" ]; then
  SPEC_LETTER="P"
fi
if [ ${SPEC} == "HMS" ]; then
  SPEC_LETTER="H"
fi

# Which run to analyze.
runNum=$1
if [ -z "$runNum" ]; then
  echo "I need the run number!!!"
  exit 0
fi

# How many events to analyze.
numEvents=$2
if [ -z "$numEvents" ]; then
    numEvents=-1
    echo "Number of events not specified, will replay all events from run ${runNum}."
fi

# Which scripts to run.
scalar_script="SCRIPTS/${SPEC}/SCALERS/replay_${spec}_scalers.C"
bcm_calib_script="CALIBRATION/bcm_current_map/run.C"
production_script="SCRIPTS/${SPEC}/PRODUCTION/replay_production_${spec}.C"
counter_script="./UTIL_XEM/el_counter.C"

# Which commands to run.
runProd="./hcana -l -b -q \"${production_script}(${runNum}, ${numEvents})\""
runScal="./hcana -l -b -q \"${scalar_script}(${runNum}, ${numEvents})\""

# File names
bcm_param_out_dir="./PARAM/${SPEC}/BCM/"
scalar_out_dir="./ROOTfiles/${SPEC}/SCALARS/"
scalar_out_file="${scalar_out_dir}${spec}_replay_scalars_${runNum}_${numEvents}.root"
prod_out_file="./ROOTfiles/${SPEC}/PRODUCTION/${spec}_replay_production_${runNum}_${numEvents}.root"

# Start analysis and monitoring plots.
{
  echo ""
  echo ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:="
  echo "" 
  date
  echo ""
  echo "Running scalar ${SPEC} replay on the run ${runNum}:"
  echo " -> SCRIPT:  ${scalar_script}"
  echo " -> RUN:     ${runNum}"
  echo " -> NEVENTS: ${numEvents}"
  echo " -> COMMAND: ${runScal}"
  echo ""
  echo ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:="

  sleep 2
  #eval ${runScal}
  sleep 2

  echo ""
  echo ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:="
  echo ""
  date
  echo ""
  echo "Running ${SPEC} BCM Calibration on the run ${runNum}:"
  echo " -> SCRIPT:  ${bcm_calib_script}"
  echo " -> RUN:     ${runNum}"
  echo " -> NEVENTS: ${numEvents}"
  echo ""
  echo ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:="

  sleep 2
  #./hcana -b- l << EOF
  #.x ${bcm_calib_script}("${scalar_out_file}", "${SPEC_LETTER}", ${runNum})
#EOF

#  mv bcmcurrent_${runNum}.param ${bcm_param_out_dir}

  echo ""
  echo ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:="
  echo "" 
  date
  echo ""
  echo "Running production ${SPEC} replay on the run ${runNum}:"
  echo " -> SCRIPT:  ${production_script}"
  echo " -> RUN:     ${runNum}"
  echo " -> NEVENTS: ${numEvents}"
  echo " -> COMMAND: ${runProd}"
  echo ""
  echo ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:="

  sleep 2
  eval ${runProd}

  echo "" 
  echo ""
  echo ""
  echo ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:="
  echo ""
  echo "Running electron counter for analyzed ${SPEC} run ${runNum}:"
  echo " -> SCRIPT:  ${counter_script}"
  echo " -> RUN:     ${runNum}"
  echo " -> NEVENTS: ${numEvents}"
  echo ""
  echo ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:="

  sleep 2
  ./hcana -l << EOF
  .L ${counter_script}
  run_el_counter_${spec}("${prod_out_file}");
EOF

  echo "" 
  echo ""
  echo ""
  echo "-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|"
  echo ""
  echo "So long and thanks for all the fish!"
  echo ""
  echo "-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|"
  echo "" 
  echo ""
  echo ""

}
