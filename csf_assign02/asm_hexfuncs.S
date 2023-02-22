/* Assembly language implementation of hexdump functions 
 * CSF Assignment 2
 * TJ Bai
 * tbai4@jhu.edu
 * Halle Cho
 * hcho64@jhu.edu
*/

	.section .rodata

	/* TODO: add read-only data such as strings here */

	.section .text

	.globl hex_read
hex_read:
	xorq %rax,%rax
	movq %rdi,%rsi
	movq $0,%rdi
	movq $16,%rdx
	syscall
	ret

	.globl hex_write_string
hex_write_string:
	movq %rdi,%rsi
	movq $1,%rdi
	movq $1,%rax
	xorq %rdx,%rdx

.Lcompare:
	movb (%rsi,%rdx),%r11b
	cmpb $0,%r11b
	je .Lwrite
	incq %rdx
	jmp .Lcompare

.Lwrite:	
	syscall
	ret

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

/*
Format an unsigned value as an offset string consisting of exactly 8
hex digits.  The formatted offset is stored in sbuf, which must
have enough room for a string of length 8.
*/
	.globl hex_format_offset
hex_format_offset:
	movl %edi,%r10d 	    	/* copy offset (passed by edi) into r10 */
	movq $7,%r11    		/* initialize r11 with 7 */
	movb $4,%cl			/* initialize rax with 4 */
	movb $0,8(%rsi) 

.Lmain_offset:
	andl $0xF,%r10d 			/* bitmask with 0xF for least-significant 4 digits */
	cmpl $10,%r10d			/* compare value with 10 */
	jb .Ldigit_offset 		/* if value is less than 10, then we insert a digit */
	jmp .Lletter_offset		/* otherwise, we insesrt a letter */

.Ldigit_offset:
	addl $48,%r10d			/* add 48 to r10, equivalent to offsetting by '0' */
	movb %r10b,(%rsi,%r11)   /* copy r10 into input buffer (passed by rsi) at index r11 */
	jmp .Lcheck_offset				/* jump to check section */

.Lletter_offset:				
	addl $87,%r10d			/* add 87 to r10, equivalent to offsetting by 'a' */
	movb %r10b,(%rsi,%r11) /* copy r10 into input buffer (passed by rsi) at index r11 */
	jmp .Lcheck_offset				/* jump to check section */

.Lcheck_offset:			
	cmpq $0,%r11 			/* compare r11 to 0 */
	jg .Lshift_offset		/* if r11 is greater than 0, then continue */
	ret						/* otherwise, we have parsed the entire byte */

.Lshift_offset:
	decl %r11d 				/* decrement r11 */
	movl %edi,%r10d			/* copy byteval into r10 again, resetting value */
	shr %cl,%r10d			/* right-shift r12 */
	addb $4,%cl
	jmp .Lmain_offset 		/* repeat from main */
	ret


/* vim:ft=gas:
 */
