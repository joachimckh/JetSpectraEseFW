fRun="339293" # R=0.2,=0.4 (_jetR4) #full
data_size="LHC23zzh_pass4_full"

mcRun="346927" # Pb-Pb
mc_size="LHC24g3_medium"

bkgRun="346921"
bkg_size="LHC23_PbPb_pass4_goldenRuns"

# mcppRun="346903"
# mcpp_size="LHC24g4"

# mcppRun="348328"
# mcpp_size="LHC24f3b"
mcppRun="349100" # ingen occupancy cuts
mcpp_size="LHC24f3b"

rval="0.2"
bayesIterations="1"
svdIterations="2"

# <run number> <data size> <mc run number> <mc size> <bkg run> <bkg size> <mc pp run> <mc pp dataset> <R>
echo "Running unfold-proc"
./build/src/Tasks/unfold-proc "${fRun}" "${data_size}" "${mcRun}" "${mc_size}" "${bkgRun}" "${bkg_size}" "${mcppRun}" "${mcpp_size}" "${rval}" "${bayesIterations}" "${svdIterations}"
