
STACKTOP = 0x50000000;

SECTIONS
{
	. = 0x40000000;
	.text : { *(.text) }
	.text.hell : { *(.text.hell) }
	.rodata : { *(.rodata) }
	.data : { *(.data) }
	. = ALIGN(0x8);
	bss_begin = .;
	.bss : { *(.bss*) } 
	bss_end = .;

	. = STACKTOP;
	stacktop = .;
}
