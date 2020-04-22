~/arm64/host/bin/qemu-system-aarch64 \
-M virt \
-cpu cortex-a53 \
-smp 1 \
-m 4096 \
-nographic -serial mon:stdio \
-kernel build/kernel8.elf \
-s -S \

#-device loader,file=build/kernel8.bin,addr=0x40000000
