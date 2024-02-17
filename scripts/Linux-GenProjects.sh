pushd $(dirname "$0")/../ > /dev/null
chmod +x vendor/premake/bin/premake5
./vendor/premake/bin/premake5 gmake2
popd > /dev/null