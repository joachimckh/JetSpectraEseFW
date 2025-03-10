#!/bin/bash
fRun="366086" # R=0.2 LHC23_PbPb_pass4_goldenRuns fRhoPhi 0
# fRun="366003" # R=0.2 LHC23zzh_pass4_full # fRhoPhi 1
rval="0.2"
# subtask="_jetR04"
data_size="LHC23_PbPb_pass4_goldenRuns"


fRhoPhi="1"

centmin="30"
centmax="50"


qmin="0"
qmax="100"
echo "Running data_proc with run=${fRun}, q2min=${qmin}, q2max=${qmax}, centmin=${centmin}, centmax=${centmax}, jet Radius=${rval}"
./build/src/Tasks/data-proc "${fRun}" "${qmin}" "${qmax}" "${centmin}" "${centmax}" "${rval}" "${data_size}" "${subtask}" "${fRhoPhi}"

qmin="0"
qmax="30"
echo "Running data_proc with run=${fRun}, q2min=${qmin}, q2max=${qmax}, centmin=${centmin}, centmax=${centmax}, jet Radius=${rval}"
./build/src/Tasks/data-proc "${fRun}" "${qmin}" "${qmax}" "${centmin}" "${centmax}" "${rval}" "${data_size}" "${subtask}" "${fRhoPhi}"

qmin="70"
qmax="100"
echo "Running data_proc with run=${fRun}, q2min=${qmin}, q2max=${qmax}, centmin=${centmin}, centmax=${centmax}, jet Radius=${rval}"
./build/src/Tasks/data-proc "${fRun}" "${qmin}" "${qmax}" "${centmin}" "${centmax}" "${rval}" "${data_size}" "${subtask}" "${fRhoPhi}"
