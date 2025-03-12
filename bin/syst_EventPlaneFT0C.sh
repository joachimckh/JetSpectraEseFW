#!/bin/bash

default="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23_PbPb_pass4_goldenRuns/jet/366086/AnalysisResults.root"
FT0C="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_full/jet/366003/AnalysisResults.root"
sub1=""
sub2="_AFT0C"


./analysis/build/src/Systematics/sysEPFT0C "${default}" "${sub1}" "${FT0C}" "${sub2}" 