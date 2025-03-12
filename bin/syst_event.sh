#!/bin/bash

default="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23_PbPb_pass4_goldenRuns/jet/366086/AnalysisResults.root"
sub1=""

Lead6="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_small/jet/366324/AnalysisResults.root"
subLead=""

vtxz7="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_small/jet/367265/AnalysisResults.root"
subVtx=""

./build/src/Systematics/sysEvent "${default}" "${sub1}" "${Lead6}" "${subLead}" "${vtxz7}" "${subVtx}" 