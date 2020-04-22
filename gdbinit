tar rem:1234
set confirm off
add-symbol-file build/kernel8.elf 0x40000000
disp /i $pc
