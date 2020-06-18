tar rem:1234
set confirm off
add-symbol-file build/kernel8.elf 0x60000000
set $pc=_start

hb *0x600000c0
#hb copy_process
#hb _schedule
#hb cpu_switch_to
#hb timer_tick
#hb *0xa8
#hb *0x2c

c
si
file build/kernel8.elf
disp /i $pc
disp /x current

define ttbr1
	i r TTBR1_EL1
end

define ttbr0
	i r TTBR0_EL1
end

define sp_el0
	i r SP_EL0
end

define spsr_el1
	i r SPSR_EL1
end

define elr_el1
	i r ELR_EL1
end

define esr_el1
	i r ESR_EL1
end
