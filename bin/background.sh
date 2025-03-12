#!/bin/bash
bkgRun="366003"
bkg_size="LHC23zzh_pass4_full"


./analysis/build/src/Tasks/background "${bkgRun}" "${bkg_size}" "0" "100"
./analysis/build/src/Tasks/background "${bkgRun}" "${bkg_size}" "0" "30"
./analysis/build/src/Tasks/background "${bkgRun}" "${bkg_size}" "70" "100"

