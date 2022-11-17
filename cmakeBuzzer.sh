mkdir -p build_buzzer
cd build_buzzer
source /opt/poky/2.1.3/environment-setup-aarch64-poky-linux
cmake -v -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE ../buzzer
make
