#!/bin/bash

default="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23_PbPb_pass4_goldenRuns/jet/366086/AnalysisResults.root"
sub1=""

vtxz9andLead6="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_small/jet/366324/AnalysisResults.root"
subLead=""
subVtx="_VtxZ9"

./build/src/Systematics/sysEvent "${default}" "${sub1}" "${vtxz9andLead6}" "${subLead}" "${vtxz9andLead6}" "${subVtx}" 