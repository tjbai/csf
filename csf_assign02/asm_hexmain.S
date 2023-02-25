/* Hexdump main function, assembly language version */

	.section .text

.equ ABUF_OFFSET, -40
.equ SBUF_OFFSET, -20

	.globl main
main:	
	pushq %r12 							/* represents total characters read */
	pushq %r13							/* use to index for loops */
	pushq %r14							/* use to store temporary values */
	movq $0, %r12

	pushq %rbp							/* align stack */
	movq %rsp, %rbp
	subq $48, %rsp
	
.Lwhile_loop_top:
	leaq ABUF_OFFSET(%rbp), %rdi	
	call hex_read
	cmpq $0, %rax
	je .Ldone

	/* Store number of characters just read */
	movq %rax, %r14

	/* Convert total characters read to hex, store in SBUF */
	movq %r12, %rdi 					
	leaq SBUF_OFFSET(%rbp), %rsi
	call hex_format_offset

	/* Print offset with proper formatting */
	movq $58, (SBUF_OFFSET + 8)(%rbp)
	movq $32, (SBUF_OFFSET + 9)(%rbp)
	movq $0, (SBUF_OFFSET + 10)(%rbp)
	leaq SBUF_OFFSET(%rbp), %rdi
	call hex_write_string

	movq $0, %r13
.Lfor_loop_top:
	cmpq %r14, %r13
	jge .Lprint_remaining_whitespace

	/* Convert value in ABUF and store in SBUF */
	movq ABUF_OFFSET(%rbp,%r13), %rdi
	leaq SBUF_OFFSET(%rbp), %rsi
	call hex_format_byte_as_hex

	/* Write formatted byte with proper formatting */
	movq $32, (SBUF_OFFSET + 2)(%rbp)
	movq $0, (SBUF_OFFSET + 3)(%rbp)
	leaq SBUF_OFFSET(%rbp), %rdi
	call hex_write_string
	
	/* Replace character in ABUF with printable */
	movq (ABUF_OFFSET)(%rbp,%r13), %rdi
	call hex_to_printable
	movq %rax, (ABUF_OFFSET)(%rbp,%r13)

	/* Increment r13 and repeat for loop */
	incq %r13
	jmp .Lfor_loop_top

	/* Print whitespace to fill rest of line */
.Lprint_remaining_whitespace:
	movq $32, (SBUF_OFFSET)(%rbp)
	movq $32, (SBUF_OFFSET + 1)(%rbp)
	movq $32, (SBUF_OFFSET + 2)(%rbp)
	movq $0, (SBUF_OFFSET + 3)(%rbp)
	leaq (SBUF_OFFSET)(%rbp), %rdi

.Lprint_remaining_whitespace_loop:
	cmpq $16, %r13
	jge .Lwhile_loop_bottom
	call hex_write_string
	incq %r13
	jmp .Lprint_remaining_whitespace_loop

.Lwhile_loop_bottom:
	/* Print one extra whitespace */
	movq $32, (SBUF_OFFSET)(%rbp)
	movq $0, (SBUF_OFFSET + 1)(%rbp)
	leaq SBUF_OFFSET(%rbp), %rdi
	call hex_write_string

	/* Print original text */
	movq $0, ABUF_OFFSET(%rbp,%r14)
	leaq ABUF_OFFSET(%rbp), %rdi
	call hex_write_string

	/* Print new line */
	movq $10, (SBUF_OFFSET)(%rbp)
	movq $0, (SBUF_OFFSET + 1)(%rbp)
	leaq SBUF_OFFSET(%rbp), %rdi
	call hex_write_string

	/* Increment offset and reset while loop */
	addq $16, %r12
	jmp .Lwhile_loop_top

.Ldone:
	addq $48, %rsp
	popq %rbp
	popq %r14
	popq %r13
	popq %r12
	movq $0, %rax
	ret

/* vim:ft=gas:
 */
