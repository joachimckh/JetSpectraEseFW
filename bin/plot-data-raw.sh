#!/bin/bash

# if [ "$#" -ne 1 ]; then
#   echo "Usage: $0 <rval>"
#   exit 1
# fi

# rval="$1"

path="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23_PbPb_pass4_goldenRuns/jet/366086/AnalysisResults.root"
# path="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_small/jet/367114/AnalysisResults.root" # R=0.4
r="0.2"

./build/src/JAnalysis/plot-data-raw "${path}" "${r}"
