export ARCH=arm
export SUBARCH=arm
export CROSS_COMPILE=/usr/bin/arm-linux-gnueabi-
export TARGET=out
rm -rf out
rm build.log
./build_kernel.sh
