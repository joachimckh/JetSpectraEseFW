#!/bin/bash
# fRun="326346" # full FT0A
# fRun="325738" # small FT0C
# fRun="331439" # full A: TPCpos, B: FT0C, C: TPCneg
# fRun="334606" # small A: FT0A, B: TPCpos, C: TPCneg
fRun="346921" #  A: FT0A, B: TPCpos, C: TPCneg | LHC23_PbPb_pass4_goldenRuns
A="FT0A"
B="TPCpos"
C="TPCneg"
rSize="LHC23_PbPb_pass4_goldenRuns"



echo "Running event-plate-res with run=${fRun} and A=${A} B=${B} C=${C}" size=${rSize}
./analysis/build/src/JAnalysis/event-plate-res "${fRun}" "${A}" "${B}" "${C}" "${rSize}"
