tar rem:1234
set confirm off
add-symbol-file build/kernel8.elf 0x60000000
set $pc=_start

hb *0x600000c0
c
si
file build/kernel8.elf
disp /i $pc
