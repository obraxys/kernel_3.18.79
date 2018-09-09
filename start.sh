export ARCH=arm
export ARCH_MTK_PLATFORM=mt6580
export SUBARCH=arm
#export CROSS_COMPILE=/usr/arm-eabi-4.8/bin/arm-eabi-
export CROSS_COMPILE=/usr/arm-linux-androideabi-4.9/bin/arm-linux-androideabi-
#export CROSS_COMPILE=/home/vas/android/alps_8.1/prebuilts/gcc/linux-x86/x86/x86_64-linux-android-4.9/bin/x86_64-linux-android-
export TARGET=out
rm -rf out
rm build.log
make O=$TARGET ARCH=$ARCH CROSS_COMPILE=$CROSS_COMPILE m502_defconfig
./build_kernel.sh


