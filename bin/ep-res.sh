#!/bin/bash
fRun="325059"
A="FT0A"
B="TPCpos"
C="TPCneg"

echo "Running event-plate-res with run=${fRun} and A=${A} B=${B} C=${C}"
./build/JAnalysis/event-plate-res "${fRun}" "${A}" "${B}" "${C}"
