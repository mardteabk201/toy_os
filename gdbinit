tar rem:1234
set confirm off
add-symbol-file build/kernel8.elf 0x60000000
set $pc=_start
disp /i $pc
