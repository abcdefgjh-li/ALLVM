	.file	"irobf-opaque-predicate-bytes.ll"
	.text
	.globl	opaque_bytes                    // -- Begin function opaque_bytes
	.p2align	2
	.type	opaque_bytes,@function
opaque_bytes:                           // @opaque_bytes
	.cfi_startproc
// %bb.0:
	mov	x9, xzr
	cbnz	x9, .LBB0_1
	b	.LBB0_2
.LBB0_1:
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	232
	.byte	3
	.p2align	2, 0x0
	b	.LBB0_2
.LBB0_2:
	add	w0, w0, #1
	ret
.Lfunc_end0:
	.size	opaque_bytes, .Lfunc_end0-opaque_bytes
	.cfi_endproc
                                        // -- End function
	.section	".note.GNU-stack","",@progbits
