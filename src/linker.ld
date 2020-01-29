
STACKTOP = 0x51000000;

SECTIONS
{
	. = 0x40010000;
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
