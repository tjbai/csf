/* Assembly language implementation of hexdump functions */

	.section .rodata

	/* TODO: add read-only data such as strings here */

	.section .text


/* 
Convert a byte value (in the range 0-255) to a printable character
value.  If byteval is already a printable character, it is returned
unmodified.  If byteval is not a printable character, then the
ASCII code for '.' should be returned.
*/
	.globl hex_to_printable
hex_to_printable:
	cmpb $32,%dil  			/* compare character (stored in dil) to 32 */
	jb .Lnot_printable		/* if character less than 32, it is not printable */
	cmpb $126,%dil			/* compare character to 126 */
	ja .Lnot_printable 		/* if character greater than 126, it is not printable */
	movb %dil,%al 			/* if 32 <= character <= 126, then return character */
	ret 

.Lnot_printable:
	movb $46,%al 			/* if character is not printable, then return '.' == 46 */
	ret 


/*
Format a byte value (in the range 0-255) as string consisting
of two hex digits.  The string is stored in sbuf.
*/
	.globl hex_format_byte_as_hex
hex_format_byte_as_hex:
	movb %dil,%r10b 	    /* copy byteval (passed by dil) into r10 */
	movq $1,%r11    		/* initialize r11 with 1 */
	movb $0,2(%rsi)

.Lmain:
	andb $0xF,%r10b 		/* bitmask with 0xF for least-significant 4 digits */
	cmpb $10,%r10b			/* compare value with 10 */
	jb .Ldigit 				/* if value is less than 10, then we insert a digit */
	jmp .Lletter			/* otherwise, we insert a letter */

.Ldigit:
	addb $48,%r10b			/* add 48 to r10, equivalent to offsetting by '0' */
	movb %r10b,(%rsi,%r11)  /* copy r10 into input buffer (passed by rsi) at index r11 */
	jmp .Lcheck				/* jump to check section */

.Lletter:				
	addb $87,%r10b			/* add 87 to r10, equivalent to offsetting by 'a' */
	movb %r10b,(%rsi,%r11)  /* copy r10 into input buffer (passed by rsi) at index r11 */
	jmp .Lcheck				/* jump to check section */

.Lcheck:			
	cmpq $0,%r11 			/* compare r11 to 0 */
	jg .Lshift				/* if r11 is greater than 0, then continue */
	ret						/* otherwise, we have parsed the entire byte */

.Lshift:
	decq %r11 				/* decrement r11 */
	movb %dil,%r10b			/* copy byteval into r10 again, resetting value */
	shrb $4,%r10b			/* right-shift r10 by 4 places */
	jmp .Lmain 				/* repeat from main */



	.globl hex_format_offset
hex_format_offset:
	ret


/* vim:ft=gas:
 */
