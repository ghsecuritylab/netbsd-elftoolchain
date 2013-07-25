#	Makefile.inc,v 1.3 2012/11/26 18:57:35 drochner Exp

SRCS+= \
	random.c \
	divrem_2.c \
	toom_interpolate_7pts.c \
	sbpi1_divappr_q.c \
	random2.c \
	mu_bdiv_q.c \
	toom32_mul.c \
	toom44_mul.c \
	toom8h_mul.c \
	toom2_sqr.c \
	zero.c \
	mod_1_4.c \
	gcdext.c \
	binvert.c \
	mu_div_q.c \
	invertappr.c \
	dump.c \
	mu_divappr_q.c \
	dcpi1_div_qr.c \
	redc_1_sec.c \
	toom6_sqr.c \
	divrem_1.c \
	mod_1.c \
	divexact.c \
	powlo.c \
	mul.c \
	set_str.c \
	toom42_mul.c \
	toom_interpolate_6pts.c \
	dcpi1_divappr_q.c \
	copyd.c \
	toom_eval_dgr3_pm2.c \
	mod_1_3.c \
	com.c \
	copyi.c \
	toom_couple_handling.c \
	add.c \
	toom_interpolate_12pts.c \
	perfsqr.c \
	toom53_mul.c \
	toom_eval_pm2exp.c \
	mu_div_qr.c \
	toom_interpolate_16pts.c \
	mod_34lsub1.c \
	bdiv_q.c \
	toom22_mul.c \
	toom4_sqr.c \
	div_q.c \
	jacbase.c \
	gcdext_subdiv_step.c \
	mullo_n.c \
	toom33_mul.c \
	toom_eval_dgr3_pm1.c \
	mod_1_2.c \
	sub_1.c \
	trialdiv.c \
	add_1.c \
	sqr_basecase.c \
	toom_interpolate_5pts.c \
	sbpi1_bdiv_q.c \
	pre_mod_1.c \
	hgcd.c \
	bdiv_dbm1c.c \
	sqrtrem.c \
	bdiv_q_1.c \
	gcdext_1.c \
	toom63_mul.c \
	mul_basecase.c \
	toom8_sqr.c \
	neg.c \
	gcdext_lehmer.c \
	divis.c \
	dcpi1_div_q.c \
	subcnd_n.c \
	toom_eval_pm2.c \
	hgcd2.c \
	toom62_mul.c \
	sbpi1_bdiv_qr.c \
	scan1.c \
	pre_divrem_1.c \
	perfpow.c \
	get_str.c \
	mullo_basecase.c \
	mulmod_bnm1.c \
	tdiv_qr.c \
	toom43_mul.c \
	mod_1_1.c \
	matrix22_mul.c \
	powm_sec.c \
	divrem.c \
	gcd_1.c \
	dcpi1_bdiv_qr.c \
	mul_n.c \
	redc_2.c \
	toom6h_mul.c \
	mul_fft.c \
	sqrmod_bnm1.c \
	powm.c \
	rootrem.c \
	mode1o.c \
	toom_interpolate_8pts.c \
	remove.c \
	lshiftc.c \
	dive_1.c \
	cmp.c \
	toom_eval_pm1.c \
	scan0.c \
	gcd_subdiv_step.c \
	sbpi1_div_qr.c \
	invert.c \
	sub.c \
	toom_eval_pm2rexp.c \
	dcpi1_bdiv_q.c \
	redc_1.c \
	sqr.c \
	nussbaumer_mul.c \
	mu_bdiv_qr.c \
	pow_1.c \
	gcd_lehmer.c \
	get_d.c \
	toom52_mul.c \
	sbpi1_div_q.c \
	diveby3.c \
	fib2_ui.c \
	bdiv_qr.c \
	toom3_sqr.c \
	gcd.c \
	redc_n.c \

C_SRCS_LIST= \
	xor_n.c		mpn/generic/logops_n.c \
	nand_n.c		mpn/generic/logops_n.c \
	nior_n.c		mpn/generic/logops_n.c \
	hamdist.c		mpn/generic/popham.c \
	popcount.c		mpn/generic/popham.c \
	and_n.c		mpn/generic/logops_n.c \
	ior_n.c		mpn/generic/logops_n.c \
	xnor_n.c		mpn/generic/logops_n.c \
	iorn_n.c		mpn/generic/logops_n.c \
	andn_n.c		mpn/generic/logops_n.c \

ASM_SRCS_LIST= \
	sqr_diagonal.asm		mpn/mips64/sqr_diagonal.asm \
	add_n.asm		mpn/mips64/add_n.asm \
	submul_1.asm		mpn/mips64/submul_1.asm \
	umul.asm		mpn/mips64/umul.asm \
	lshift.asm		mpn/mips64/lshift.asm \
	mul_1.asm		mpn/mips64/mul_1.asm \
	rshift.asm		mpn/mips64/rshift.asm \
	addmul_1.asm		mpn/mips64/addmul_1.asm \
	sub_n.asm		mpn/mips64/sub_n.asm \

gen-trialdivtab: gen-trialdivtab.c
	${HOST_CC} -o ${.OBJDIR}/gen-trialdivtab ${.ALLSRC} -lm

.if (defined(CPUFLAGS) && !empty(CPUFLAGS:M-mabi=64))
GMP_LIMB_BITS=64
.else
GMP_LIMB_BITS=32
.endif

trialdiv.c: trialdivtab.h
trialdivtab.h: gen-trialdivtab Makefile
	${.OBJDIR}/gen-trialdivtab $(GMP_LIMB_BITS) 8000 > ${.OBJDIR}/trialdivtab.h

CLEANFILES+=	trialdivtab.h

COPTS+=		-Wno-error