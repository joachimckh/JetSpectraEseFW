#!/bin/bash
fRun="334606" # small A: FT0A, B: TPCpos, C: TPCneg
A="FT0A"
B="TPCpos"
C="TPCneg"
rSize="small"


./build/src/JAnalysis/correction-steps "${fRun}" "${A}" "${B}" "${C}" "${rSize}"
