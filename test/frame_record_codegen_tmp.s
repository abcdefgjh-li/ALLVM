	.file	"frame_record_codegen.cpp"
	.text
	.globl	frame_record_callee             // -- Begin function frame_record_callee
	.p2align	2
	.type	frame_record_callee,@function
frame_record_callee:                    // @frame_record_callee
	.cfi_startproc
// %bb.0:
	add	w0, w0, #7
	ret
.Lfunc_end0:
	.size	frame_record_callee, .Lfunc_end0-frame_record_callee
	.cfi_endproc
                                        // -- End function
	.globl	frame_record_probe              // -- Begin function frame_record_probe
	.p2align	2
	.type	frame_record_probe,@function
frame_record_probe:                     // @frame_record_probe
	.cfi_startproc
// %bb.0:
	str	x29, [sp, #-16]!                // 8-byte Folded Spill
	.cfi_def_cfa_offset 16
	add	x29, sp, #16
	str	x30, [sp, #8]                   // 8-byte Folded Spill
	sub	x29, x29, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	bl	frame_record_callee
	mov	w8, #90                         // =0x5a
	eor	w8, w0, w8
	sub	w0, w8, #16
	.cfi_def_cfa wsp, 16
	ldur	x29, [sp]                       // 8-byte Folded Reload
	ldr	x30, [sp, #8]                   // 8-byte Folded Reload
	add	sp, sp, #16
	.cfi_def_cfa_offset 0
	.cfi_restore w30
	.cfi_restore w29
	ret
.Lfunc_end1:
	.size	frame_record_probe, .Lfunc_end1-frame_record_probe
	.cfi_endproc
                                        // -- End function
	.ident	"\nA-protector\nBy abcdefgjh.\n\351\233\250\346\211\223\351\243\216\345\220\271\357\274\214\345\271\264\345\260\221\350\275\273\347\213\202\345\267\262\351\200\235\343\200\202\345\233\236\351\246\226\346\235\245\350\267\257\357\274\214\345\215\212\347\224\237\350\271\211\350\267\216\357\274\214\346\263\252\346\273\241\350\241\243\350\245\237\343\200\202\n"
	.section	".note.GNU-stack","",@progbits
