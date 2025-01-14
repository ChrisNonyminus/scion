/* Run the C pre-processor over this file with one of the following defined
 * ELF - elf object files,
 * OUT - a.out object files,
 * BSDI - BSDI style a.out object files
 * SOL - Solaris style elf
 */

#define TYPE(a,b)       .type   a,b
#define SIZE(a,b)       .size   a,b

#if defined(OUT) || (defined(BSDI) && !defined(ELF))
#define RC4 _RC4

#endif

#ifdef OUT
#define OK	1
#define ALIGN	4
#if defined(__CYGWIN__) || defined(__DJGPP__)
#undef SIZE
#undef TYPE
#define SIZE(a,b)
#define TYPE(a,b)
#endif /* __CYGWIN || __DJGPP */
#endif

#if defined(BSDI) && !defined(ELF)
#define OK              1
#define ALIGN           4
#undef SIZE
#undef TYPE
#define SIZE(a,b)
#define TYPE(a,b)
#endif

#if defined(ELF) || defined(SOL)
#define OK              1
#define ALIGN           16
#endif

#ifndef OK
You need to define one of
ELF - elf systems - linux-elf, NetBSD and DG-UX
OUT - a.out systems - linux-a.out and FreeBSD
SOL - solaris systems, which are elf with strange comment lines
BSDI - a.out with a very primative version of as.
#endif

/* Let the Assembler begin :-) */
	/* Don't even think of reading this code */
	/* It was automatically generated by rc4-586.pl */
	/* Which is a perl program used to generate the x86 assember for */
	/* any of elf, a.out, BSDI, Win32, gaswin (for GNU as on Win32) or Solaris */
	/* eric <eay@cryptsoft.com> */

	.file	"rc4-586.s"
	.version	"01.01"
gcc2_compiled.:
.text
	.align ALIGN
.globl RC4
	TYPE(RC4,@function)
RC4:

	pushl	%ebp
	pushl	%ebx
	movl	12(%esp),	%ebp
	movl	16(%esp),	%ebx
	pushl	%esi
	pushl	%edi
	movl	(%ebp),		%ecx
	movl	4(%ebp),	%edx
	movl	28(%esp),	%esi
	incl	%ecx
	subl	$12,		%esp
	addl	$8,		%ebp
	andl	$255,		%ecx
	leal	-8(%ebx,%esi),	%ebx
	movl	44(%esp),	%edi
	movl	%ebx,		8(%esp)
	movl	(%ebp,%ecx,4),	%eax
	cmpl	%esi,		%ebx
	jl	.L000end
.L001start:
	addl	$8,		%esi
	/* Round 0 */
	addl	%eax,		%edx
	andl	$255,		%edx
	incl	%ecx
	movl	(%ebp,%edx,4),	%ebx
	movl	%ebx,		-4(%ebp,%ecx,4)
	addl	%eax,		%ebx
	andl	$255,		%ecx
	andl	$255,		%ebx
	movl	%eax,		(%ebp,%edx,4)
	nop
	movl	(%ebp,%ebx,4),	%ebx
	movl	(%ebp,%ecx,4),	%eax
	movb	%bl,		(%esp)
	/* Round 1 */
	addl	%eax,		%edx
	andl	$255,		%edx
	incl	%ecx
	movl	(%ebp,%edx,4),	%ebx
	movl	%ebx,		-4(%ebp,%ecx,4)
	addl	%eax,		%ebx
	andl	$255,		%ecx
	andl	$255,		%ebx
	movl	%eax,		(%ebp,%edx,4)
	nop
	movl	(%ebp,%ebx,4),	%ebx
	movl	(%ebp,%ecx,4),	%eax
	movb	%bl,		1(%esp)
	/* Round 2 */
	addl	%eax,		%edx
	andl	$255,		%edx
	incl	%ecx
	movl	(%ebp,%edx,4),	%ebx
	movl	%ebx,		-4(%ebp,%ecx,4)
	addl	%eax,		%ebx
	andl	$255,		%ecx
	andl	$255,		%ebx
	movl	%eax,		(%ebp,%edx,4)
	nop
	movl	(%ebp,%ebx,4),	%ebx
	movl	(%ebp,%ecx,4),	%eax
	movb	%bl,		2(%esp)
	/* Round 3 */
	addl	%eax,		%edx
	andl	$255,		%edx
	incl	%ecx
	movl	(%ebp,%edx,4),	%ebx
	movl	%ebx,		-4(%ebp,%ecx,4)
	addl	%eax,		%ebx
	andl	$255,		%ecx
	andl	$255,		%ebx
	movl	%eax,		(%ebp,%edx,4)
	nop
	movl	(%ebp,%ebx,4),	%ebx
	movl	(%ebp,%ecx,4),	%eax
	movb	%bl,		3(%esp)
	/* Round 4 */
	addl	%eax,		%edx
	andl	$255,		%edx
	incl	%ecx
	movl	(%ebp,%edx,4),	%ebx
	movl	%ebx,		-4(%ebp,%ecx,4)
	addl	%eax,		%ebx
	andl	$255,		%ecx
	andl	$255,		%ebx
	movl	%eax,		(%ebp,%edx,4)
	nop
	movl	(%ebp,%ebx,4),	%ebx
	movl	(%ebp,%ecx,4),	%eax
	movb	%bl,		4(%esp)
	/* Round 5 */
	addl	%eax,		%edx
	andl	$255,		%edx
	incl	%ecx
	movl	(%ebp,%edx,4),	%ebx
	movl	%ebx,		-4(%ebp,%ecx,4)
	addl	%eax,		%ebx
	andl	$255,		%ecx
	andl	$255,		%ebx
	movl	%eax,		(%ebp,%edx,4)
	nop
	movl	(%ebp,%ebx,4),	%ebx
	movl	(%ebp,%ecx,4),	%eax
	movb	%bl,		5(%esp)
	/* Round 6 */
	addl	%eax,		%edx
	andl	$255,		%edx
	incl	%ecx
	movl	(%ebp,%edx,4),	%ebx
	movl	%ebx,		-4(%ebp,%ecx,4)
	addl	%eax,		%ebx
	andl	$255,		%ecx
	andl	$255,		%ebx
	movl	%eax,		(%ebp,%edx,4)
	nop
	movl	(%ebp,%ebx,4),	%ebx
	movl	(%ebp,%ecx,4),	%eax
	movb	%bl,		6(%esp)
	/* Round 7 */
	addl	%eax,		%edx
	andl	$255,		%edx
	incl	%ecx
	movl	(%ebp,%edx,4),	%ebx
	movl	%ebx,		-4(%ebp,%ecx,4)
	addl	%eax,		%ebx
	andl	$255,		%ecx
	andl	$255,		%ebx
	movl	%eax,		(%ebp,%edx,4)
	nop
	movl	(%ebp,%ebx,4),	%ebx
	addl	$8,		%edi
	movb	%bl,		7(%esp)
	/* apply the cipher text */
	movl	(%esp),		%eax
	movl	-8(%esi),	%ebx
	xorl	%ebx,		%eax
	movl	-4(%esi),	%ebx
	movl	%eax,		-8(%edi)
	movl	4(%esp),	%eax
	xorl	%ebx,		%eax
	movl	8(%esp),	%ebx
	movl	%eax,		-4(%edi)
	movl	(%ebp,%ecx,4),	%eax
	cmpl	%ebx,		%esi
	jle	.L001start
.L000end:
	/* Round 0 */
	addl	$8,		%ebx
	incl	%esi
	cmpl	%esi,		%ebx
	jl	.L002finished
	movl	%ebx,		8(%esp)
	addl	%eax,		%edx
	andl	$255,		%edx
	incl	%ecx
	movl	(%ebp,%edx,4),	%ebx
	movl	%ebx,		-4(%ebp,%ecx,4)
	addl	%eax,		%ebx
	andl	$255,		%ecx
	andl	$255,		%ebx
	movl	%eax,		(%ebp,%edx,4)
	nop
	movl	(%ebp,%ebx,4),	%ebx
	movl	(%ebp,%ecx,4),	%eax
	movb	-1(%esi),	%bh
	xorb	%bh,		%bl
	movb	%bl,		(%edi)
	/* Round 1 */
	movl	8(%esp),	%ebx
	cmpl	%esi,		%ebx
	jle	.L002finished
	incl	%esi
	addl	%eax,		%edx
	andl	$255,		%edx
	incl	%ecx
	movl	(%ebp,%edx,4),	%ebx
	movl	%ebx,		-4(%ebp,%ecx,4)
	addl	%eax,		%ebx
	andl	$255,		%ecx
	andl	$255,		%ebx
	movl	%eax,		(%ebp,%edx,4)
	nop
	movl	(%ebp,%ebx,4),	%ebx
	movl	(%ebp,%ecx,4),	%eax
	movb	-1(%esi),	%bh
	xorb	%bh,		%bl
	movb	%bl,		1(%edi)
	/* Round 2 */
	movl	8(%esp),	%ebx
	cmpl	%esi,		%ebx
	jle	.L002finished
	incl	%esi
	addl	%eax,		%edx
	andl	$255,		%edx
	incl	%ecx
	movl	(%ebp,%edx,4),	%ebx
	movl	%ebx,		-4(%ebp,%ecx,4)
	addl	%eax,		%ebx
	andl	$255,		%ecx
	andl	$255,		%ebx
	movl	%eax,		(%ebp,%edx,4)
	nop
	movl	(%ebp,%ebx,4),	%ebx
	movl	(%ebp,%ecx,4),	%eax
	movb	-1(%esi),	%bh
	xorb	%bh,		%bl
	movb	%bl,		2(%edi)
	/* Round 3 */
	movl	8(%esp),	%ebx
	cmpl	%esi,		%ebx
	jle	.L002finished
	incl	%esi
	addl	%eax,		%edx
	andl	$255,		%edx
	incl	%ecx
	movl	(%ebp,%edx,4),	%ebx
	movl	%ebx,		-4(%ebp,%ecx,4)
	addl	%eax,		%ebx
	andl	$255,		%ecx
	andl	$255,		%ebx
	movl	%eax,		(%ebp,%edx,4)
	nop
	movl	(%ebp,%ebx,4),	%ebx
	movl	(%ebp,%ecx,4),	%eax
	movb	-1(%esi),	%bh
	xorb	%bh,		%bl
	movb	%bl,		3(%edi)
	/* Round 4 */
	movl	8(%esp),	%ebx
	cmpl	%esi,		%ebx
	jle	.L002finished
	incl	%esi
	addl	%eax,		%edx
	andl	$255,		%edx
	incl	%ecx
	movl	(%ebp,%edx,4),	%ebx
	movl	%ebx,		-4(%ebp,%ecx,4)
	addl	%eax,		%ebx
	andl	$255,		%ecx
	andl	$255,		%ebx
	movl	%eax,		(%ebp,%edx,4)
	nop
	movl	(%ebp,%ebx,4),	%ebx
	movl	(%ebp,%ecx,4),	%eax
	movb	-1(%esi),	%bh
	xorb	%bh,		%bl
	movb	%bl,		4(%edi)
	/* Round 5 */
	movl	8(%esp),	%ebx
	cmpl	%esi,		%ebx
	jle	.L002finished
	incl	%esi
	addl	%eax,		%edx
	andl	$255,		%edx
	incl	%ecx
	movl	(%ebp,%edx,4),	%ebx
	movl	%ebx,		-4(%ebp,%ecx,4)
	addl	%eax,		%ebx
	andl	$255,		%ecx
	andl	$255,		%ebx
	movl	%eax,		(%ebp,%edx,4)
	nop
	movl	(%ebp,%ebx,4),	%ebx
	movl	(%ebp,%ecx,4),	%eax
	movb	-1(%esi),	%bh
	xorb	%bh,		%bl
	movb	%bl,		5(%edi)
	/* Round 6 */
	movl	8(%esp),	%ebx
	cmpl	%esi,		%ebx
	jle	.L002finished
	incl	%esi
	addl	%eax,		%edx
	andl	$255,		%edx
	incl	%ecx
	movl	(%ebp,%edx,4),	%ebx
	movl	%ebx,		-4(%ebp,%ecx,4)
	addl	%eax,		%ebx
	andl	$255,		%ecx
	andl	$255,		%ebx
	movl	%eax,		(%ebp,%edx,4)
	nop
	movl	(%ebp,%ebx,4),	%ebx
	movb	-1(%esi),	%bh
	xorb	%bh,		%bl
	movb	%bl,		6(%edi)
.L002finished:
	decl	%ecx
	addl	$12,		%esp
	movl	%edx,		-4(%ebp)
	movb	%cl,		-8(%ebp)
	popl	%edi
	popl	%esi
	popl	%ebx
	popl	%ebp
	ret
.RC4_end:
	SIZE(RC4,.RC4_end-RC4)
.ident	"RC4"
