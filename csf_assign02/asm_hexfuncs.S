/* Assembly language implementation of hexdump functions */

	.section .rodata

	/* TODO: add read-only data such as strings here */

	.section .text

	/* TODO: add functions here */

	.globl hex_to_printable
hex_to_printable:
	cmpb $32,%dil 
	jb .Lnot_printable
	cmpb $126,%dil
	ja .Lnot_printable 
	movb %dil,%al 
	ret 

.Lnot_printable:
	movb $46,%al 
	ret 

	.globl hex_format_byte_as_hex
hex_format_byte_as_hex:
	ret

	.globl hex_format_offset
hex_format_offset:
	ret


/* vim:ft=gas:
 */
