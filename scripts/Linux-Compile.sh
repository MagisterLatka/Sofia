pushd $(dirname "$0")/../ > /dev/null
NB_CORES=$(grep -c '^processor' /proc/cpuinfo)
make -j$((NB_CORES+1)) -l${NB_CORES} config=debug
make -j$((NB_CORES+1)) -l${NB_CORES} config=release
popd > /dev/null