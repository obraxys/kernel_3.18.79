/usr/arm-linux-androideabi-4.9/bin/arm-linux-androideabi-gcc -E -Wp,-MD,m502.dtb.d.pre.tmp -nostdinc -I./arch/arm/boot/dts -I./arch/arm/boot/dts/include -I./include/ -I./drivers/of/testcase-data -undef -D__DTS__ -x assembler-with-cpp -o m502.dtb.dts.tmp m502.dts
./dtc -O dtb -o m502.dtb -b 0 -i ./arch/arm/boot/dts/  -d m502.dtb.d.dtc.tmp m502.dtb.dts.tmp
cat m502.dtb.d.pre.tmp m502.dtb.d.dtc.tmp > m502.dtb.d
rm m502.dtb.d
rm m502.dtb.d.dtc.tmp
rm m502.dtb.d.pre.tmp
rm m502.dtb.dts.tmp
