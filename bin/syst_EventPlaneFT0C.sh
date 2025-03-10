#!/bin/bash

default="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_full/jet/366003/AnalysisResults.root"
sub1=""
# epres=""
sub2="_AFT0C"

# vtxz9andLead6="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_small/jet/366324/AnalysisResults.root"
# subLead=""
# subVtx="_VtxZ9"

./build/src/Systematics/sysEPFT0C "${default}" "${sub1}" "${default}" "${sub2}" 