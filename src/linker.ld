
STACKTOP = 0xffff000020000000;

SECTIONS
{
	. = 0xffff000000000000;
	.text : { *(.text) }
	. = ALIGN(0x00001000);
	user_begin = .;
	.text.user : { build/user* (.text) }
	.rodata.user : { build/user* (.rodata) }
	.data.user : { build/user* (.data) }
	.bss.user : { build/user* (.bss) }
	user_end = .;
	.text.hell : { *(.text.hell) }
	.rodata : { *(.rodata) }
	.data : { *(.data) }
	. = ALIGN(0x8);
	bss_begin = .;
	.bss : { *(.bss*) } 
	bss_end = .;
	. = ALIGN(0x00001000);
	pg_dir = .;
	.data.pgd : { . += (3 * (1 << 12)); }
	idmap_pg_dir = .;
	.data.pgd : { . += (3 * (1 << 12)); }

	. = STACKTOP;
	stacktop = .;
}
