
# mcRun="346927"
# dataset="LHC24g3_medium"

mcRun="349100"
dataset="LHC24f3b"

#<run number> <R> <mc run number> <data size> <mc size>" 
echo "Running eff_proc"
./build/src/Tasks/eff-proc "${mcRun}" "${dataset}"
