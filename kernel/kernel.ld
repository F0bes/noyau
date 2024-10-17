MEMORY
{
	vec (rx) : org = 0x80000000, len = 4k
	ram (wx) : org = 0x80001000, len = 128k
	heap (wx) : org = 0x80021000, len = 128k
	misc (wx) : org = 0x80041000, len = 1m
}

ENTRY(__start)

SECTIONS
{
	.text 0x80000000 : { *(.vec.tlb) } > vec
	.text 0x80000080 : { *(.vec.perfc) } > vec
	.text 0x80000100 : { *(.vec.debug) } > vec
	.text 0x80000180 : { *(.vec.common) } > vec
	.text 0x80000200 : { *(.vec.interrupt) } > vec

	.text  : {
		*(.text)
		*(.text.*)
	} > ram

	.data ALIGN(128) : {
		*(.data)
		*(.data.*)
		*(.rodata)
		*(.rodata.*)
	} > ram

	_gp = ALIGN(128);

	.bss ALIGN(128) : {
		PROVIDE(__bss_start = .);
		*(.bss)
		PROVIDE(__bss_end = .);
	} > ram

	.heap ALIGN(128) : {
		PROVIDE(__heap_start = .);
		. = . + 128k;
		PROVIDE(__heap_end = .);
	} > heap

	.misc ALIGN(128) : {
		PROVIDE(__misc_start = .);
		*(.misc)
		PROVIDE(__misc_end = .);
	} > misc

	PROVIDE(__kern_stack = ALIGN(128));

	/DISCARD/ : {
		*(.debug*)
		*(.mdebug*)
		*(.gnu.attributes)
		*(.pdr)
		*(.comment)
		*(.MIPS.abiflags)
		*(.reginfo)
		*(.rel.dyn) 
	}
}
