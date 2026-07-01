	.file	"call_ret_codegen.cpp"
	.text
	.globl	target_call                     // -- Begin function target_call
	.p2align	2
	.type	target_call,@function
target_call:                            // @target_call
	.cfi_startproc
// %bb.0:
	add	w0, w0, #7
	br	x30
.Lfunc_end0:
	.size	target_call, .Lfunc_end0-target_call
	.cfi_endproc
                                        // -- End function
	.globl	call_ret_probe                  // -- Begin function call_ret_probe
	.p2align	2
	.type	call_ret_probe,@function
call_ret_probe:                         // @call_ret_probe
	.cfi_startproc
// %bb.0:
	stp	x29, x30, [sp, #-16]!           // 16-byte Folded Spill
	.cfi_def_cfa_offset 16
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	adrp	x1, target_call
	add	x1, x1, :lo12:target_call
	blr	x1
	add	w0, w0, #1
	.cfi_def_cfa wsp, 16
	ldp	x29, x30, [sp], #16             // 16-byte Folded Reload
	.cfi_def_cfa_offset 0
	.cfi_restore w30
	.cfi_restore w29
	br	x30
.Lfunc_end1:
	.size	call_ret_probe, .Lfunc_end1-call_ret_probe
	.cfi_endproc
                                        // -- End function
	.ident	"\nA-protector\nBy abcdefgjh.\n\344\272\221\345\274\200\346\234\210\346\230\216\357\274\214\346\255\244\345\216\273\347\273\217\345\271\264\346\227\240\346\201\231\343\200\202\345\210\253\347\246\273\346\234\211\346\227\266\357\274\214\351\207\215\351\200\242\346\227\240\346\234\237\357\274\214\347\232\206\346\230\257\345\244\251\346\204\217\343\200\202\n"
	.section	".note.GNU-stack","",@progbits
