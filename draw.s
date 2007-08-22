	.file	1 "draw.c"
	.section .mdebug.eabi32
	.section .gcc_compiled_long32
	.previous
	.text
	.align	2
	.globl	boxfill
	.ent	boxfill
boxfill:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	subu	$7,$7,$5
	subu	$6,$6,$4
	lui	$2,%hi(screen_pitch)
	lui	$3,%hi(screen_address)
	addiu	$7,$7,1
	move	$10,$4
	addiu	$6,$6,1
	lw	$9,%lo(screen_pitch)($2)
	beq	$7,$0,$L13
	lw	$4,%lo(screen_address)($3)

	mult	$5,$9
	sll	$12,$9,1
	move	$11,$0
	mflo	$2
	addu	$2,$10,$2
	sll	$2,$2,1
	addu	$2,$2,$4
$L4:
	beql	$6,$0,$L12
	addiu	$11,$11,1

	andi	$10,$8,0xffff
	move	$9,$2
	move	$3,$0
$L5:
	addiu	$3,$3,1
	sh	$10,0($9)
	bne	$6,$3,$L5
	addiu	$9,$9,2

	addiu	$11,$11,1
$L12:
	bne	$7,$11,$L4
	addu	$2,$2,$12

$L13:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	boxfill
	.size	boxfill, .-boxfill
	.align	2
	.globl	boxfill_alpha
	.ent	boxfill_alpha
boxfill_alpha:
	.frame	$sp,8,$31		# vars= 0, regs= 2/0, args= 0, gp= 0
	.mask	0x00030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-8
	subu	$7,$7,$5
	sw	$17,4($sp)
	sw	$16,0($sp)
	subu	$6,$6,$4
	lui	$2,%hi(screen_pitch)
	lui	$3,%hi(screen_address)
	addiu	$16,$7,1
	move	$10,$4
	ext	$15,$8,10,5
	addiu	$13,$6,1
	andi	$14,$8,0x1f
	move	$17,$9
	lw	$6,%lo(screen_pitch)($2)
	lw	$4,%lo(screen_address)($3)
	beq	$16,$0,$L22
	ext	$8,$8,5,5

	mult	$5,$6
	sll	$9,$6,1
	move	$7,$0
	mflo	$2
	addu	$2,$10,$2
	sll	$2,$2,1
	addu	$25,$2,$4
$L17:
	beql	$13,$0,$L24
	addiu	$7,$7,1

	move	$10,$25
	move	$12,$0
	lui	$24,%hi(alpha_blend)
	sll	$11,$17,5
$L18:
	lhu	$2,0($10)
	addu	$6,$11,$15
	sll	$6,$6,5
	ext	$5,$2,10,5
	addu	$3,$11,$8
	addu	$4,$11,$14
	addu	$6,$6,$5
	sll	$3,$3,5
	andi	$5,$2,0x1f
	sll	$4,$4,5
	ext	$2,$2,5,5
	addu	$3,$3,$2
	addu	$4,$4,$5
	sll	$6,$6,2
	addiu	$5,$24,%lo(alpha_blend)
	addu	$6,$6,$5
	sll	$3,$3,2
	sll	$4,$4,2
	addu	$4,$4,$5
	lw	$2,0($6)
	addu	$3,$3,$5
	lw	$5,0($3)
	lw	$6,0($4)
	sll	$2,$2,10
	sll	$5,$5,5
	or	$2,$2,$6
	or	$5,$5,$2
	addiu	$12,$12,1
	sh	$5,0($10)
	bne	$13,$12,$L18
	addiu	$10,$10,2

	addiu	$7,$7,1
$L24:
	bne	$16,$7,$L17
	addu	$25,$25,$9

$L22:
	lw	$17,4($sp)
	lw	$16,0($sp)
	j	$31
	addiu	$sp,$sp,8

	.set	macro
	.set	reorder
	.end	boxfill_alpha
	.size	boxfill_alpha, .-boxfill_alpha
	.align	2
	.globl	init_progress
	.ent	init_progress
init_progress:
	.frame	$sp,8,$31		# vars= 0, regs= 1/0, args= 0, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-8
	lui	$2,%hi(progress_total)
	sw	$31,0($sp)
	sw	$4,%lo(progress_total)($2)
	lui	$4,%hi(progress_message)
	addiu	$4,$4,%lo(progress_message)
	lui	$2,%hi(progress_current)
	jal	strcpy
	sw	$0,%lo(progress_current)($2)

	lui	$3,%hi(screen_pitch)
	lw	$4,%lo(screen_pitch)($3)
	li	$2,140			# 0x8c
	lui	$3,%hi(screen_address)
	mult	$4,$2
	lw	$5,%lo(screen_address)($3)
	sll	$4,$4,1
	addiu	$4,$4,2
	move	$6,$0
	mflo	$2
	addiu	$2,$2,89
	sll	$2,$2,1
	addu	$3,$2,$5
$L26:
	addiu	$6,$6,1
	li	$2,303			# 0x12f
	sh	$0,0($3)
	bne	$6,$2,$L26
	addu	$3,$3,$4

	move	$3,$0
$L29:
	addiu	$3,$3,1
	li	$2,13			# 0xd
	bne	$3,$2,$L29
	lw	$31,0($sp)

	j	$31
	addiu	$sp,$sp,8

	.set	macro
	.set	reorder
	.end	init_progress
	.size	init_progress, .-init_progress
	.align	2
	.globl	show_progress
	.ent	show_progress
show_progress:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lui	$2,%hi(screen_pitch)
	lw	$6,%lo(screen_pitch)($2)
	li	$3,140			# 0x8c
	lui	$2,%hi(screen_address)
	mult	$6,$3
	lw	$10,%lo(screen_address)($2)
	sll	$9,$6,1
	addiu	$5,$9,2
	move	$4,$0
	mflo	$3
	addiu	$3,$3,89
	sll	$3,$3,1
	addu	$3,$3,$10
$L36:
	addiu	$4,$4,1
	li	$2,303			# 0x12f
	sh	$0,0($3)
	bne	$4,$2,$L36
	addu	$3,$3,$5

	move	$3,$0
$L45:
	addiu	$3,$3,1
	li	$2,13			# 0xd
	bne	$3,$2,$L45
	lui	$2,%hi(progress_current)

	lw	$2,%lo(progress_current)($2)
	beq	$2,$0,$L57
	li	$4,983040			# 0xf0000

	li	$4,100			# 0x64
	mult	$2,$4
	lui	$2,%hi(progress_total)
	lw	$3,%lo(progress_total)($2)
	li	$2,141			# 0x8d
	move	$7,$0
	move	$8,$0
	mflo	$4
	bne	$3,$0,1f
	div	$0,$4,$3
	break	7
1:
	li	$3,3			# 0x3
	mflo	$4
	mult	$6,$2
	mflo	$2
	addiu	$2,$2,90
	sll	$2,$2,1
	mult	$4,$3
	addu	$5,$10,$2
	mflo	$4
$L56:
	sll	$2,$7,1
	sltu	$3,$7,$4
	addu	$6,$2,$5
	bne	$3,$0,$L41
	addiu	$7,$7,1

	addiu	$8,$8,1
	sltu	$2,$8,11
	addu	$5,$5,$9
	bne	$2,$0,$L56
	move	$7,$0

	li	$4,983040			# 0xf0000
$L57:
	j	sceKernelDelayThread
	ori	$4,$4,0x4240

$L41:
	li	$2,15855
	j	$L56
	sh	$2,0($6)

	.set	macro
	.set	reorder
	.end	show_progress
	.size	show_progress, .-show_progress
	.align	2
	.globl	update_progress
	.ent	update_progress
update_progress:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lui	$2,%hi(screen_pitch)
	lw	$5,%lo(screen_pitch)($2)
	li	$2,141			# 0x8d
	lui	$6,%hi(progress_current)
	mult	$5,$2
	lui	$3,%hi(screen_address)
	lw	$4,%lo(progress_current)($6)
	lui	$2,%hi(progress_total)
	lw	$10,%lo(screen_address)($3)
	lw	$12,%lo(progress_total)($2)
	sll	$9,$5,1
	addiu	$4,$4,1
	addiu	$8,$9,2
	move	$7,$0
	sw	$4,%lo(progress_current)($6)
	mflo	$11
	addiu	$2,$11,89
	sll	$2,$2,1
	addu	$3,$2,$10
$L59:
	li	$2,31
	sh	$2,0($3)
	addiu	$7,$7,1
	li	$2,303			# 0x12f
	bne	$7,$2,$L59
	addu	$3,$3,$8

	move	$3,$0
$L67:
	addiu	$3,$3,1
	li	$2,11			# 0xb
	bne	$3,$2,$L67
	addiu	$2,$11,90

	li	$3,100			# 0x64
	mult	$4,$3
	sll	$2,$2,1
	addu	$5,$10,$2
	li	$2,3			# 0x3
	move	$6,$0
	move	$8,$0
	mflo	$3
	bne	$12,$0,1f
	div	$0,$3,$12
	break	7
1:
	mflo	$3
	mult	$3,$2
	mflo	$4
$L78:
	sll	$2,$6,1
	sltu	$3,$6,$4
	addu	$7,$2,$5
	bne	$3,$0,$L62
	addiu	$6,$6,1

	addiu	$8,$8,1
	sltu	$2,$8,11
	addu	$5,$5,$9
	bne	$2,$0,$L78
	move	$6,$0

	j	$31
	nop

$L62:
	li	$2,15855
	j	$L78
	sh	$2,0($7)

	.set	macro
	.set	reorder
	.end	update_progress
	.size	update_progress, .-update_progress
	.local	progress_total
	.comm	progress_total,4,4
	.local	progress_current
	.comm	progress_current,4,4
	.local	progress_message
	.comm	progress_message,128,4

	.comm	productStruct,16,4

	.comm	dbg_file,4,4

	.comm	msg,768,4

	.comm	msg_data,16384,4
	.ident	"GCC: (GNU) 4.1.0 (PSPDEV 20060507)"
