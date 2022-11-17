mkdir -p build_server
cd build_server
source /opt/poky/2.1.3/environment-setup-aarch64-poky-linux
echo $ARCH
cmake -v -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE ../server
