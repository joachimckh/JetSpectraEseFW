#!/bin/bash
# fRun="325059" # R=0.2 #small
# fRun="326346" # R=0.2 #full
fRun="325147" # R=0.4
rval="0.4"

data_size="small"

centmin="30"
centmax="50"


qmin="0"
qmax="100"
echo "Running data_proc with run=${fRun}, q2min=${qmin}, q2max=${qmax}, centmin=${centmin}, centmax=${centmax}, jet Radius=${rval}"
./build/Tasks/data-proc "${fRun}" "${qmin}" "${qmax}" "${centmin}" "${centmax}" "${rval}" "${data_size}"

qmin="0"
qmax="30"
echo "Running data_proc with run=${fRun}, q2min=${qmin}, q2max=${qmax}, centmin=${centmin}, centmax=${centmax}, jet Radius=${rval}"
./build/Tasks/data-proc "${fRun}" "${qmin}" "${qmax}" "${centmin}" "${centmax}" "${rval}" "${data_size}"

qmin="70"
qmax="100"
echo "Running data_proc with run=${fRun}, q2min=${qmin}, q2max=${qmax}, centmin=${centmin}, centmax=${centmax}, jet Radius=${rval}"
./build/Tasks/data-proc "${fRun}" "${qmin}" "${qmax}" "${centmin}" "${centmax}" "${rval}" "${data_size}"
