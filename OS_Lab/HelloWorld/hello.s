	.file	"hello.c"
	.option nopic
	.attribute arch, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0_zifencei2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.section	.rodata
	.align	3
.LC0:
	.string	"Hello, World!\n"
	.text
	.align	1
	.globl	_start
	.type	_start, @function
_start:
	addi	sp,sp,-32
	sd	ra,24(sp)
	sd	s0,16(sp)
	addi	s0,sp,32
	lui	a5,%hi(.LC0)
	addi	a5,a5,%lo(.LC0)
	sd	a5,-32(s0)
	sw	zero,-20(s0)
	j	.L2
.L3:
	lw	a5,-20(s0)
	ld	a4,-32(s0)
	add	a4,a4,a5
	li	a5,268435456
	lbu	a4,0(a4)
	sb	a4,0(a5)
	lw	a5,-20(s0)
	addiw	a5,a5,1
	sw	a5,-20(s0)
.L2:
	lw	a5,-20(s0)
	ld	a4,-32(s0)
	add	a5,a4,a5
	lbu	a5,0(a5)
	bne	a5,zero,.L3
.L4:
	j	.L4
	.size	_start, .-_start
	.ident	"GCC: () 14.2.0"
	.section	.note.GNU-stack,"",@progbits
