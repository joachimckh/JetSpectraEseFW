#!/bin/bash


# pathT4="/Users/joachimcarlokristianhansen/jet_analysis/localr/jetSpectraESE/AnalysisResults.root"
# nameT4="local run"

# pathT4="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_small/jet/365493/AnalysisResults.root"
# nameT4="HL small rho(phi)"

# pathT4="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_full/jet/366003/AnalysisResults.root"
# nameT4="HL full rho(phi)"

pathT4="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23_PbPb_pass4_goldenRuns/jet/366086/AnalysisResults.root"
nameT4="HL golden runs rho(phi)"

./analysis/build/src/Tasks/compare-pt-rhophi "${pathT4}" "${nameT4}"

