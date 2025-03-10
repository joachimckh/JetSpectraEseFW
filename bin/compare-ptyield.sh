#!/bin/bash
path1="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23_PbPb_pass4_goldenRuns/jet/346921/AnalysisResults.root" #golden runs
name1="13/02/25 Hyperloop: PbPb Golden Runs"

# path2="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_full/jet/359615/AnalysisResults.root" # pass4 full (rho_event)
# name2="28/02/25 Hyperloop: PbPb_pass4_full rho(event)"

# path3="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_small/jet/361920/AnalysisResults.root" # pass4 small rho(phi)
# name3="01/03/25 Hyperloop: PbPb_pass4_small rho(phi)" 

# path6="/Users/joachimcarlokristianhansen/jet_analysis/localr/jetSpectraESE/AnalysisResults_wRhoLocalSmall.root"
# name6="local run rho(phi) small"

# path7="/Users/joachimcarlokristianhansen/jet_analysis/localr/jetSpectraESE/AnalysisResults_wRhoEventSmall.root"
# name7="local run rho(event) small"

# pathT1="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/tests/AnalysisResults_tag20250221-0919-1.root"
# nameT1="tag20250221-0919-1"

# pathT2="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/tests/AnalysisResults_tag20250301-0000-1_rhoEvent.root"
# nameT2="tag20250301-0000-1_rhoEvent"

# pathT3="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/tests/AnalysisResults_tag20250212-0000-1_sameasGR.root"
# nameT3="tag20250212-0000-1"

# pathT4="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_small/jet/363145/AnalysisResults.root"
# nameT4="HL small rho(event)"

# pathL1="/Users/joachimcarlokristianhansen/jet_analysis/localr/jetSpectraESE/AnalysisResults.root"
# nameL1="local rho(phi)"

# pathT5="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_small/jet/365493/AnalysisResults.root"
# nameT5="HL small rho(phi)"

# pathT5="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_full/jet/366003/AnalysisResults.root"
# nameT5="HL full rho(phi)"

pathT5="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23_PbPb_pass4_goldenRuns/jet/366086/AnalysisResults.root"
nameT5="HL golden runs rho(phi)"


./build/src/Tasks/compare-pt-yield "${path1}" "${name1}" "${pathT5}" "${nameT5}"

