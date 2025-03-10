#!/bin/bash
bkgRun="366003"
bkg_size="LHC23zzh_pass4_full"


./build/src/Tasks/background "${bkgRun}" "${bkg_size}" "0" "100"
./build/src/Tasks/background "${bkgRun}" "${bkg_size}" "0" "30"
./build/src/Tasks/background "${bkgRun}" "${bkg_size}" "70" "100"

