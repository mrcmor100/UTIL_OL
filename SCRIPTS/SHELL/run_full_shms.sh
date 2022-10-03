#! /bin/bash

# Which spectrometer are we analyzing.
spec=${0##*_}
spec=${spec%%.sh}
SPEC=$(echo "$spec" | tr '[:lower:]' '[:upper:]')

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
scalar_script="SCRIPT/${SPEC}/SCALARS/replay_${spec}_scalar.C"
production_script="SCRIPTS/${SPEC}/PRODUCTION/replay_production_${spec}.C"
counter_script="./UTIL_XEM/el_counter.C"

# Which commands to run.
runProd="./hcana -l -b -q \"${production_script}(${runNum}, ${numEvents})\""
runScal="./hcana -l -b -q \"${scalar_script}(${runNum}, ${numEvents})\""

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
  eval ${runScal}

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
  echo ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:="
  echo ""
  echo "Running electron counter for analyzed ${SPEC} run ${runNum}:"
  echo " -> SCRIPT:  ${counter_script}"
  echo " -> RUN:     ${runNum}"
  echo " -> NEVENTS: ${numEvents}"
  echo ""
  echo ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:="

  sleep 2
  ./hcana -b -l << EOF
  .L ${counter}
  run_el_counter_${spec}("./ROOTfiles/${SPEC}/${spec}_replay_production_${runNum}_${numEvents}.root");
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
