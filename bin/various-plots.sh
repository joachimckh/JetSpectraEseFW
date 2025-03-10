#!/bin/bash
fRun="346921" # R=0.2
rval="0.2"

data_size="LHC23_PbPb_pass4_goldenRuns"

ptLow="30"
ptHigh="100"

./build/src/JAnalysis/various-plots "${fRun}" "${data_size}" "${rval}" "${ptLow}" "${ptHigh}"
