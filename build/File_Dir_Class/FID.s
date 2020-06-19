	.file	"FID_class.cpp"
	.section	.rodata
	.type	_ZStL19piecewise_construct, @object
	.size	_ZStL19piecewise_construct, 1
_ZStL19piecewise_construct:
	.zero	1
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
.LC0:
	.string	"."
.LC1:
	.string	"Can not get memory - %d"
	.text
	.align 2
	.globl	_ZN3FID9FID_classC2Ev
	.type	_ZN3FID9FID_classC2Ev, @function
_ZN3FID9FID_classC2Ev:
.LFB1200:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	$0, 40(%rax)
	movq	-8(%rbp), %rax
	movq	$0, 48(%rax)
	movq	-8(%rbp), %rax
	movl	$0, 56(%rax)
	movq	-8(%rbp), %rax
	movl	$0, 72(%rax)
	movq	-8(%rbp), %rax
	movb	$1, 76(%rax)
	movl	$4, %esi
	movl	$.LC0, %edi
	call	pathconf
	movq	-8(%rbp), %rdx
	movq	%rax, 16(%rdx)
	movl	$3, %esi
	movl	$.LC0, %edi
	call	pathconf
	movq	-8(%rbp), %rdx
	movq	%rax, 24(%rdx)
	movl	$4, %edi
	call	sysconf
	movq	-8(%rbp), %rdx
	movq	%rax, 32(%rdx)
	movq	-8(%rbp), %rax
	movq	16(%rax), %rax
	movq	%rax, %rdi
	call	_Znam
	movq	-8(%rbp), %rdx
	movq	%rax, (%rdx)
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	jne	.L2
	movq	-8(%rbp), %rax
	movb	$0, 76(%rax)
	movl	$1, %edx
	movl	$.LC1, %esi
	movl	$11, %edi
	movl	$0, %eax
	call	syslog
.L2:
	movq	-8(%rbp), %rax
	movq	24(%rax), %rax
	movq	%rax, %rdi
	call	_Znam
	movq	-8(%rbp), %rdx
	movq	%rax, 8(%rdx)
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	testq	%rax, %rax
	jne	.L3
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	je	.L4
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	_ZdaPv
.L4:
	movq	-8(%rbp), %rax
	movb	$0, 76(%rax)
	movl	$2, %edx
	movl	$.LC1, %esi
	movl	$11, %edi
	movl	$0, %eax
	call	syslog
.L3:
	movq	-8(%rbp), %rax
	movq	32(%rax), %rax
	movabsq	$2287828610704211968, %rdx
	cmpq	%rdx, %rax
	ja	.L5
	salq	$2, %rax
	jmp	.L6
.L5:
	movq	$-1, %rax
.L6:
	movq	%rax, %rdi
	call	_Znam
	movq	-8(%rbp), %rdx
	movq	%rax, 64(%rdx)
	movq	-8(%rbp), %rax
	movq	64(%rax), %rax
	testq	%rax, %rax
	jne	.L1
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	je	.L8
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	_ZdaPv
.L8:
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	testq	%rax, %rax
	je	.L9
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, %rdi
	call	_ZdaPv
.L9:
	movq	-8(%rbp), %rax
	movb	$0, 76(%rax)
	movl	$3, %edx
	movl	$.LC1, %esi
	movl	$11, %edi
	movl	$0, %eax
	call	syslog
.L1:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1200:
	.size	_ZN3FID9FID_classC2Ev, .-_ZN3FID9FID_classC2Ev
	.globl	_ZN3FID9FID_classC1Ev
	.set	_ZN3FID9FID_classC1Ev,_ZN3FID9FID_classC2Ev
	.section	.rodata
.LC2:
	.string	"Deleting Work_Home\n"
.LC3:
	.string	"Deleting Root_Home\n"
.LC4:
	.string	"Deleting FDES set\n"
.LC5:
	.string	"Deleting Dir\n"
.LC6:
	.string	"Call _CLOSE_DIR_\n"
.LC7:
	.string	"DIR * = "
.LC8:
	.string	"Deleting List\n"
	.align 8
.LC9:
	.string	"FID_class had been deleted! - %ld"
.LC10:
	.string	"FID_class did nothing!"
	.text
	.align 2
	.globl	_ZN3FID9FID_classD2Ev
	.type	_ZN3FID9FID_classD2Ev, @function
_ZN3FID9FID_classD2Ev:
.LFB1203:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA1203
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$40, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -40(%rbp)
	movq	-40(%rbp), %rax
	movzbl	76(%rax), %eax
	testb	%al, %al
	je	.L11
	movl	$.LC2, %esi
	movl	$_ZSt4cout, %edi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movq	-40(%rbp), %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	je	.L12
	movq	-40(%rbp), %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	je	.L12
	movq	-40(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	_ZdaPv
.L12:
	movl	$.LC3, %esi
	movl	$_ZSt4cout, %edi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	testq	%rax, %rax
	je	.L13
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	testq	%rax, %rax
	je	.L13
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, %rdi
	call	_ZdaPv
.L13:
	movl	$.LC4, %esi
	movl	$_ZSt4cout, %edi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movq	-40(%rbp), %rax
	movq	64(%rax), %rax
	testq	%rax, %rax
	je	.L14
	movl	$0, -20(%rbp)
	jmp	.L15
.L16:
	movq	-40(%rbp), %rax
	movq	64(%rax), %rax
	movl	-20(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rdx, %rax
	movl	(%rax), %edx
	movq	-40(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	_ZN3FID9FID_class7_CLOSE_Ei
	addl	$1, -20(%rbp)
.L15:
	movq	-40(%rbp), %rax
	movl	72(%rax), %eax
	cmpl	-20(%rbp), %eax
	jg	.L16
	movq	-40(%rbp), %rax
	movq	64(%rax), %rax
	testq	%rax, %rax
	je	.L17
	movq	-40(%rbp), %rax
	movq	64(%rax), %rax
	movq	%rax, %rdi
	call	_ZdaPv
.L17:
.L14:
	movl	$.LC5, %esi
	movl	$_ZSt4cout, %edi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movq	-40(%rbp), %rax
	movq	40(%rax), %rax
	testq	%rax, %rax
	je	.L18
	movl	$.LC6, %esi
	movl	$_ZSt4cout, %edi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movq	-40(%rbp), %rax
	movq	40(%rax), %rbx
	movl	$.LC7, %esi
	movl	$_ZSt4cout, %edi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	_ZNSolsEPKv
	movl	$_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %esi
	movq	%rax, %rdi
	call	_ZNSolsEPFRSoS_E
	movq	-40(%rbp), %rax
	movq	40(%rax), %rax
	movq	%rax, %rdi
	call	closedir
.L18:
	movl	$.LC8, %esi
	movl	$_ZSt4cout, %edi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movq	-40(%rbp), %rax
	movq	48(%rax), %rax
	testq	%rax, %rax
	je	.L19
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	_ZN3FID9FID_class12_CLEAR_LIST_Ev
.L19:
	call	getpid
	movl	%eax, %edx
	movl	$.LC9, %esi
	movl	$13, %edi
	movl	$0, %eax
	call	syslog
	jmp	.L10
.L11:
	movl	$.LC10, %esi
	movl	$13, %edi
	movl	$0, %eax
	call	syslog
.L10:
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1203:
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA1203:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE1203-.LLSDACSB1203
.LLSDACSB1203:
.LLSDACSE1203:
	.text
	.size	_ZN3FID9FID_classD2Ev, .-_ZN3FID9FID_classD2Ev
	.globl	_ZN3FID9FID_classD1Ev
	.set	_ZN3FID9FID_classD1Ev,_ZN3FID9FID_classD2Ev
	.align 2
	.globl	_ZN3FID9FID_class6_OPEN_EPKciz
	.type	_ZN3FID9FID_class6_OPEN_EPKciz, @function
_ZN3FID9FID_class6_OPEN_EPKciz:
.LFB1205:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$240, %rsp
	movq	%rdi, -216(%rbp)
	movq	%rsi, -224(%rbp)
	movq	%rcx, -152(%rbp)
	movq	%r8, -144(%rbp)
	movq	%r9, -136(%rbp)
	testb	%al, %al
	je	.L22
	movaps	%xmm0, -128(%rbp)
	movaps	%xmm1, -112(%rbp)
	movaps	%xmm2, -96(%rbp)
	movaps	%xmm3, -80(%rbp)
	movaps	%xmm4, -64(%rbp)
	movaps	%xmm5, -48(%rbp)
	movaps	%xmm6, -32(%rbp)
	movaps	%xmm7, -16(%rbp)
.L22:
	movl	%edx, -228(%rbp)
	movl	$-1, -180(%rbp)
	orl	$256, -228(%rbp)
	movl	-228(%rbp), %eax
	andl	$64, %eax
	testl	%eax, %eax
	je	.L23
	movl	$24, -208(%rbp)
	movl	$48, -204(%rbp)
	leaq	16(%rbp), %rax
	movq	%rax, -200(%rbp)
	leaq	-176(%rbp), %rax
	movq	%rax, -192(%rbp)
	movl	-208(%rbp), %eax
	cmpl	$48, %eax
	jnb	.L24
	movq	-192(%rbp), %rdx
	movl	-208(%rbp), %eax
	movl	%eax, %eax
	addq	%rdx, %rax
	movl	-208(%rbp), %edx
	addl	$8, %edx
	movl	%edx, -208(%rbp)
	jmp	.L25
.L24:
	movq	-200(%rbp), %rdx
	movq	%rdx, %rax
	addq	$8, %rdx
	movq	%rdx, -200(%rbp)
.L25:
	movl	(%rax), %eax
	movl	%eax, -184(%rbp)
	movl	-184(%rbp), %edx
	movl	-228(%rbp), %ecx
	movq	-224(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	open
	movl	%eax, -180(%rbp)
	cmpl	$0, -180(%rbp)
	jns	.L26
	movl	$-1, %eax
	jmp	.L21
.L26:
	movq	-216(%rbp), %rax
	movq	64(%rax), %rsi
	movq	-216(%rbp), %rax
	movl	72(%rax), %eax
	leal	1(%rax), %ecx
	movq	-216(%rbp), %rdx
	movl	%ecx, 72(%rdx)
	cltq
	salq	$2, %rax
	leaq	(%rsi,%rax), %rdx
	movl	-180(%rbp), %eax
	movl	%eax, (%rdx)
	jmp	.L28
.L23:
	movl	-228(%rbp), %edx
	movq	-224(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	open
	movl	%eax, -180(%rbp)
	cmpl	$0, -180(%rbp)
	jns	.L29
	movl	$-1, %eax
	jmp	.L21
.L29:
	movq	-216(%rbp), %rax
	movq	64(%rax), %rsi
	movq	-216(%rbp), %rax
	movl	72(%rax), %eax
	leal	1(%rax), %ecx
	movq	-216(%rbp), %rdx
	movl	%ecx, 72(%rdx)
	cltq
	salq	$2, %rax
	leaq	(%rsi,%rax), %rdx
	movl	-180(%rbp), %eax
	movl	%eax, (%rdx)
.L28:
.L21:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1205:
	.size	_ZN3FID9FID_class6_OPEN_EPKciz, .-_ZN3FID9FID_class6_OPEN_EPKciz
	.section	.text._ZN3FID9FID_class7_CLOSE_Ei,"axG",@progbits,_ZN3FID9FID_class7_CLOSE_Ei,comdat
	.align 2
	.weak	_ZN3FID9FID_class7_CLOSE_Ei
	.type	_ZN3FID9FID_class7_CLOSE_Ei, @function
_ZN3FID9FID_class7_CLOSE_Ei:
.LFB1206:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movl	-12(%rbp), %eax
	movl	%eax, %edi
	call	close
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1206:
	.size	_ZN3FID9FID_class7_CLOSE_Ei, .-_ZN3FID9FID_class7_CLOSE_Ei
	.text
	.align 2
	.globl	_ZN3FID9FID_class7_LSTAT_EPKc
	.type	_ZN3FID9FID_class7_LSTAT_EPKc, @function
_ZN3FID9FID_class7_LSTAT_EPKc:
.LFB1212:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-8(%rbp), %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	lstat
	shrl	$31, %eax
	testb	%al, %al
	je	.L32
	movq	-8(%rbp), %rax
	movq	$0, 48(%rax)
	jmp	.L33
.L32:
	movq	-8(%rbp), %rax
	movl	24(%rax), %eax
	andl	$61440, %eax
	cmpl	$40960, %eax
	jne	.L33
	movq	-8(%rbp), %rax
	movq	$0, 48(%rax)
.L33:
	nop
	movq	-8(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1212:
	.size	_ZN3FID9FID_class7_LSTAT_EPKc, .-_ZN3FID9FID_class7_LSTAT_EPKc
	.align 2
	.globl	_ZN3FID9FID_class11_LOCK_FILE_EiiPK5flock
	.type	_ZN3FID9FID_class11_LOCK_FILE_EiiPK5flock, @function
_ZN3FID9FID_class11_LOCK_FILE_EiiPK5flock:
.LFB1216:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movl	%edx, -16(%rbp)
	movq	%rcx, -24(%rbp)
	movl	-16(%rbp), %eax
	cmpl	$6, %eax
	je	.L37
	cmpl	$7, %eax
	je	.L38
	cmpl	$5, %eax
	jne	.L41
	movq	-24(%rbp), %rdx
	movl	-12(%rbp), %eax
	movl	$5, %esi
	movl	%eax, %edi
	movl	$0, %eax
	call	fcntl
	jmp	.L40
.L37:
	movq	-24(%rbp), %rdx
	movl	-12(%rbp), %eax
	movl	$6, %esi
	movl	%eax, %edi
	movl	$0, %eax
	call	fcntl
	jmp	.L40
.L38:
	movq	-24(%rbp), %rdx
	movl	-12(%rbp), %eax
	movl	$7, %esi
	movl	%eax, %edi
	movl	$0, %eax
	call	fcntl
	jmp	.L40
.L41:
	movl	$-1, %eax
.L40:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1216:
	.size	_ZN3FID9FID_class11_LOCK_FILE_EiiPK5flock, .-_ZN3FID9FID_class11_LOCK_FILE_EiiPK5flock
	.align 2
	.globl	_ZN3FID9FID_class10_OPEN_DIR_EPKc
	.type	_ZN3FID9FID_class10_OPEN_DIR_EPKc, @function
_ZN3FID9FID_class10_OPEN_DIR_EPKc:
.LFB1217:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	opendir
	movq	-8(%rbp), %rdx
	movq	%rax, 40(%rdx)
	movq	-8(%rbp), %rax
	movq	40(%rax), %rax
	testq	%rax, %rax
	setne	%al
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1217:
	.size	_ZN3FID9FID_class10_OPEN_DIR_EPKc, .-_ZN3FID9FID_class10_OPEN_DIR_EPKc
	.section	.text._ZN3FID9FID_class10_READ_DIR_Ev,"axG",@progbits,_ZN3FID9FID_class10_READ_DIR_Ev,comdat
	.align 2
	.weak	_ZN3FID9FID_class10_READ_DIR_Ev
	.type	_ZN3FID9FID_class10_READ_DIR_Ev, @function
_ZN3FID9FID_class10_READ_DIR_Ev:
.LFB1218:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	40(%rax), %rax
	movq	%rax, %rdi
	call	readdir
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1218:
	.size	_ZN3FID9FID_class10_READ_DIR_Ev, .-_ZN3FID9FID_class10_READ_DIR_Ev
	.section	.text._ZN3FID9FID_class11_CLOSE_DIR_Ev,"axG",@progbits,_ZN3FID9FID_class11_CLOSE_DIR_Ev,comdat
	.align 2
	.weak	_ZN3FID9FID_class11_CLOSE_DIR_Ev
	.type	_ZN3FID9FID_class11_CLOSE_DIR_Ev, @function
_ZN3FID9FID_class11_CLOSE_DIR_Ev:
.LFB1220:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	40(%rax), %rax
	movq	%rax, %rdi
	call	closedir
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1220:
	.size	_ZN3FID9FID_class11_CLOSE_DIR_Ev, .-_ZN3FID9FID_class11_CLOSE_DIR_Ev
	.section	.rodata
.LC11:
	.string	"Created list,length is %d"
	.text
	.align 2
	.globl	_ZN3FID9FID_class7_CHDIR_EPKc
	.type	_ZN3FID9FID_class7_CHDIR_EPKc, @function
_ZN3FID9FID_class7_CHDIR_EPKc:
.LFB1221:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	chdir
	shrl	$31, %eax
	testb	%al, %al
	je	.L48
	movl	$0, %eax
	jmp	.L49
.L48:
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	strlen
	movq	%rax, -8(%rbp)
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	-8(%rbp), %rdx
	movq	-32(%rbp), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	_Copy_String_
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	_ZN3FID9FID_class12_CLEAR_LIST_Ev
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	_ZN3FID9FID_class15_MAKE_DIR_LIST_Ev
	movq	-24(%rbp), %rdx
	movl	%eax, 56(%rdx)
	movq	-24(%rbp), %rax
	movl	56(%rax), %eax
	cmpl	$-1, %eax
	jne	.L50
	movl	$0, %eax
	jmp	.L49
.L50:
	movq	-24(%rbp), %rax
	movl	56(%rax), %eax
	movl	%eax, %edx
	movl	$.LC11, %esi
	movl	$14, %edi
	movl	$0, %eax
	call	syslog
	movl	$1, %eax
.L49:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1221:
	.size	_ZN3FID9FID_class7_CHDIR_EPKc, .-_ZN3FID9FID_class7_CHDIR_EPKc
	.align 2
	.globl	_ZN3FID9FID_class8_GETCWD_EPcm
	.type	_ZN3FID9FID_class8_GETCWD_EPcm, @function
_ZN3FID9FID_class8_GETCWD_EPcm:
.LFB1222:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	-24(%rbp), %rax
	movq	16(%rax), %rax
	cmpq	-40(%rbp), %rax
	ja	.L52
	movq	-24(%rbp), %rax
	movq	16(%rax), %rax
	jmp	.L53
.L52:
	movq	-40(%rbp), %rax
.L53:
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	movq	(%rax), %rcx
	movq	-32(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	_Copy_String_
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1222:
	.size	_ZN3FID9FID_class8_GETCWD_EPcm, .-_ZN3FID9FID_class8_GETCWD_EPcm
	.align 2
	.globl	_ZN3FID9FID_class15_MAKE_DIR_LIST_Ev
	.type	_ZN3FID9FID_class15_MAKE_DIR_LIST_Ev, @function
_ZN3FID9FID_class15_MAKE_DIR_LIST_Ev:
.LFB1223:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	40(%rax), %rax
	testq	%rax, %rax
	jne	.L55
	movq	-40(%rbp), %rax
	movl	$.LC0, %esi
	movq	%rax, %rdi
	call	_ZN3FID9FID_class10_OPEN_DIR_EPKc
	xorl	$1, %eax
	testb	%al, %al
	je	.L55
	movl	$-1, %eax
	jmp	.L56
.L55:
	movq	$0, -24(%rbp)
	movl	$16, %edi
	call	_Znwm
	movq	-40(%rbp), %rdx
	movq	%rax, 48(%rdx)
	movq	-40(%rbp), %rax
	movq	48(%rax), %rax
	testq	%rax, %rax
	je	.L57
	movq	-40(%rbp), %rax
	movq	48(%rax), %rax
	movq	$0, 8(%rax)
	movq	-40(%rbp), %rax
	movq	48(%rax), %rax
	movq	$0, (%rax)
	movl	$0, -4(%rbp)
	movq	$0, -32(%rbp)
	movq	-40(%rbp), %rax
	movq	48(%rax), %rax
	movq	%rax, -16(%rbp)
	jmp	.L59
.L57:
	movl	$-1, %eax
	jmp	.L56
.L65:
	movq	-24(%rbp), %rax
	movzbl	19(%rax), %eax
	cmpb	$46, %al
	jne	.L60
	movq	-24(%rbp), %rax
	movzbl	20(%rax), %eax
	cmpb	$46, %al
	je	.L61
	movq	-24(%rbp), %rax
	movzbl	20(%rax), %eax
	testb	%al, %al
	jne	.L60
.L61:
	jmp	.L59
.L60:
	movq	-24(%rbp), %rax
	addq	$19, %rax
	movq	%rax, %rdi
	call	strlen
	movq	%rax, -32(%rbp)
	movq	-40(%rbp), %rax
	movq	24(%rax), %rax
	movq	%rax, %rdi
	call	_Znam
	movq	-16(%rbp), %rdx
	movq	%rax, (%rdx)
	movq	-16(%rbp), %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	je	.L62
	movq	-24(%rbp), %rax
	leaq	19(%rax), %rcx
	movq	-16(%rbp), %rax
	movq	(%rax), %rax
	movq	-32(%rbp), %rdx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	_Copy_String_
	addl	$1, -4(%rbp)
	movl	$16, %edi
	call	_Znwm
	movq	-16(%rbp), %rdx
	movq	%rax, 8(%rdx)
	movq	-16(%rbp), %rax
	movq	8(%rax), %rax
	testq	%rax, %rax
	je	.L64
	jmp	.L66
.L62:
	movl	-4(%rbp), %eax
	jmp	.L56
.L66:
	movq	-16(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	movq	$0, 8(%rax)
	movq	-16(%rbp), %rax
	movq	$0, (%rax)
	jmp	.L59
.L64:
	movl	-4(%rbp), %eax
	jmp	.L56
.L59:
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	_ZN3FID9FID_class10_READ_DIR_Ev
	movq	%rax, -24(%rbp)
	cmpq	$0, -24(%rbp)
	setne	%al
	testb	%al, %al
	jne	.L65
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	_ZN3FID9FID_class11_CLOSE_DIR_Ev
	movl	-4(%rbp), %eax
.L56:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1223:
	.size	_ZN3FID9FID_class15_MAKE_DIR_LIST_Ev, .-_ZN3FID9FID_class15_MAKE_DIR_LIST_Ev
	.local	_ZGVZN3FID9FID_class10_TRAVERSE_EPcE8Next_Pos
	.comm	_ZGVZN3FID9FID_class10_TRAVERSE_EPcE8Next_Pos,8,8
	.align 2
	.globl	_ZN3FID9FID_class10_TRAVERSE_EPc
	.type	_ZN3FID9FID_class10_TRAVERSE_EPc, @function
_ZN3FID9FID_class10_TRAVERSE_EPc:
.LFB1224:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	$_ZGVZN3FID9FID_class10_TRAVERSE_EPcE8Next_Pos, %eax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L68
	movl	$_ZGVZN3FID9FID_class10_TRAVERSE_EPcE8Next_Pos, %edi
	call	__cxa_guard_acquire
	testl	%eax, %eax
	setne	%al
	testb	%al, %al
	je	.L68
	movq	-24(%rbp), %rax
	movq	48(%rax), %rax
	movq	%rax, _ZZN3FID9FID_class10_TRAVERSE_EPcE8Next_Pos(%rip)
	movl	$_ZGVZN3FID9FID_class10_TRAVERSE_EPcE8Next_Pos, %edi
	call	__cxa_guard_release
.L68:
	movq	_ZZN3FID9FID_class10_TRAVERSE_EPcE8Next_Pos(%rip), %rax
	testq	%rax, %rax
	je	.L69
	movzwl	_ZZN3FID9FID_class10_TRAVERSE_EPcE8Node_Num(%rip), %eax
	movswl	%ax, %edx
	movq	-24(%rbp), %rax
	movl	56(%rax), %eax
	cmpl	%eax, %edx
	jge	.L69
	movq	_ZZN3FID9FID_class10_TRAVERSE_EPcE8Next_Pos(%rip), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	strlen
	movq	%rax, -8(%rbp)
	movq	_ZZN3FID9FID_class10_TRAVERSE_EPcE8Next_Pos(%rip), %rax
	movq	(%rax), %rcx
	movq	-8(%rbp), %rdx
	movq	-32(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	_Copy_String_
	movq	_ZZN3FID9FID_class10_TRAVERSE_EPcE8Next_Pos(%rip), %rax
	movq	8(%rax), %rax
	movq	%rax, _ZZN3FID9FID_class10_TRAVERSE_EPcE8Next_Pos(%rip)
	movzwl	_ZZN3FID9FID_class10_TRAVERSE_EPcE8Node_Num(%rip), %eax
	addl	$1, %eax
	movw	%ax, _ZZN3FID9FID_class10_TRAVERSE_EPcE8Node_Num(%rip)
	jmp	.L67
.L69:
	movq	-24(%rbp), %rax
	movq	48(%rax), %rax
	movq	%rax, _ZZN3FID9FID_class10_TRAVERSE_EPcE8Next_Pos(%rip)
	movw	$0, _ZZN3FID9FID_class10_TRAVERSE_EPcE8Node_Num(%rip)
	movq	-32(%rbp), %rax
	movb	$0, (%rax)
.L67:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1224:
	.size	_ZN3FID9FID_class10_TRAVERSE_EPc, .-_ZN3FID9FID_class10_TRAVERSE_EPc
	.section	.rodata
.LC12:
	.string	"# Processing list clear.\n"
	.section	.text._ZN3FID9FID_class12_CLEAR_LIST_Ev,"axG",@progbits,_ZN3FID9FID_class12_CLEAR_LIST_Ev,comdat
	.align 2
	.weak	_ZN3FID9FID_class12_CLEAR_LIST_Ev
	.type	_ZN3FID9FID_class12_CLEAR_LIST_Ev, @function
_ZN3FID9FID_class12_CLEAR_LIST_Ev:
.LFB1225:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movl	$.LC12, %esi
	movl	$_ZSt4cout, %edi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movq	-8(%rbp), %rax
	movq	48(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	_ZN3FID9FID_class12_CLEAR_LIST_EPNS_13DIR_Item_ListE
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1225:
	.size	_ZN3FID9FID_class12_CLEAR_LIST_Ev, .-_ZN3FID9FID_class12_CLEAR_LIST_Ev
	.section	.rodata
.LC13:
	.string	"# NODE is "
	.text
	.align 2
	.globl	_ZN3FID9FID_class12_CLEAR_LIST_EPNS_13DIR_Item_ListE
	.type	_ZN3FID9FID_class12_CLEAR_LIST_EPNS_13DIR_Item_ListE, @function
_ZN3FID9FID_class12_CLEAR_LIST_EPNS_13DIR_Item_ListE:
.LFB1226:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movl	$.LC13, %esi
	movl	$_ZSt4cout, %edi
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movq	-16(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	_ZNSolsEPKv
	movl	$_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %esi
	movq	%rax, %rdi
	call	_ZNSolsEPFRSoS_E
	cmpq	$0, -16(%rbp)
	je	.L72
	movq	-16(%rbp), %rax
	movq	8(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	_ZN3FID9FID_class12_CLEAR_LIST_EPNS_13DIR_Item_ListE
	movq	-16(%rbp), %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	je	.L74
	movq	-16(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	_ZdaPv
.L74:
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	_ZdlPv
.L72:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1226:
	.size	_ZN3FID9FID_class12_CLEAR_LIST_EPNS_13DIR_Item_ListE, .-_ZN3FID9FID_class12_CLEAR_LIST_EPNS_13DIR_Item_ListE
	.align 2
	.globl	_ZN3FID9FID_class10_WORKHOME_EPcm
	.type	_ZN3FID9FID_class10_WORKHOME_EPcm, @function
_ZN3FID9FID_class10_WORKHOME_EPcm:
.LFB1227:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, -8(%rbp)
	cmpq	$0, -32(%rbp)
	je	.L76
	movq	-24(%rbp), %rax
	movq	(%rax), %rcx
	movq	-40(%rbp), %rdx
	movq	-32(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	_Copy_String_
	movq	-40(%rbp), %rax
	leaq	-1(%rax), %rdx
	movq	-32(%rbp), %rax
	addq	%rdx, %rax
	movb	$0, (%rax)
	movl	$1, %eax
	jmp	.L77
.L76:
	movl	$-1, %eax
.L77:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1227:
	.size	_ZN3FID9FID_class10_WORKHOME_EPcm, .-_ZN3FID9FID_class10_WORKHOME_EPcm
	.type	_Z41__static_initialization_and_destruction_0ii, @function
_Z41__static_initialization_and_destruction_0ii:
.LFB1383:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	cmpl	$1, -4(%rbp)
	jne	.L78
	cmpl	$65535, -8(%rbp)
	jne	.L78
	movl	$_ZStL8__ioinit, %edi
	call	_ZNSt8ios_base4InitC1Ev
	movl	$__dso_handle, %edx
	movl	$_ZStL8__ioinit, %esi
	movl	$_ZNSt8ios_base4InitD1Ev, %edi
	call	__cxa_atexit
.L78:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1383:
	.size	_Z41__static_initialization_and_destruction_0ii, .-_Z41__static_initialization_and_destruction_0ii
	.type	_GLOBAL__sub_I__ZN3FID9FID_classC2Ev, @function
_GLOBAL__sub_I__ZN3FID9FID_classC2Ev:
.LFB1384:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$65535, %esi
	movl	$1, %edi
	call	_Z41__static_initialization_and_destruction_0ii
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1384:
	.size	_GLOBAL__sub_I__ZN3FID9FID_classC2Ev, .-_GLOBAL__sub_I__ZN3FID9FID_classC2Ev
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I__ZN3FID9FID_classC2Ev
	.local	_ZZN3FID9FID_class10_TRAVERSE_EPcE8Next_Pos
	.comm	_ZZN3FID9FID_class10_TRAVERSE_EPcE8Next_Pos,8,8
	.local	_ZZN3FID9FID_class10_TRAVERSE_EPcE8Node_Num
	.comm	_ZZN3FID9FID_class10_TRAVERSE_EPcE8Node_Num,2,2
	.hidden	__dso_handle
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-39)"
	.section	.note.GNU-stack,"",@progbits
