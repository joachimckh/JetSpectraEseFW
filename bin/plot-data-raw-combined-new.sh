#!/bin/bash

# path="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23_PbPb_pass4_goldenRuns/jet/366086/AnalysisResults.root" # R=0.2
path="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_small/jet/367114/AnalysisResults.root" # R=0.4
r="0.4"

./analysis/build/src/JAnalysis/plot-data-raw-combined-new "${path}" "${r}"
