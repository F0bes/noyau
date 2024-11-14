MEMORY
{
	vec (rx) : org = 0x80000000, len = 4k
	ram (wx) : org = 0x80001000, len = 1M
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

	__kern_stack_size = 4k;

	.stack ALIGN(128) : {
		PROVIDE(__kern_stack = .);
		. = . + __kern_stack_size;
		PROVIDE(__kern_stack_top = .);
	} > ram

	.heap ALIGN(128) : {
		PROVIDE(__heap_start = .);
		. = . + 256k;
		PROVIDE(__heap_end = .);
	} > ram

	/DISCARD/ : {
		/*(.debug*)
		*(.mdebug*)
		*(.gnu.attributes)
		*(.pdr)
		*(.comment)
		*(.MIPS.abiflags)
		*(.reginfo)
		*(.rel.dyn) */
	}
}
