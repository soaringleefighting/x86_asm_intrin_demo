%include "x86inc.asm"

SECTION .data
data_8:	times 8 dw 8


SECTION .text

INIT_XMM ssse3
ALIGN 16

;====================================================================
;static void ff_pred_dc(unsigned char *_src, const unsigned char *_top,
;                       const unsigned char *_left,
;                       int stride, int log2_size)
;====================================================================

cglobal pred_dc_8x8_x86, 5, 7, 5
	mov r4, r3
	shl r4, 1
	add r4, r3
	;mov  r4, 8

	; 获取left和top的像素值
	movq    m1, [r1]	; t0 t1 t2 t3 t4 t5 t6 t7 byte
	movq    m2, [r2]	; l0 l1 l2 l3 l4 l5 l6 l7 byte

	pxor	  m0, m0		
	punpcklbw m1, m0	; t0 t1 t2 t3 t4 t5 t6 t7 word
	punpcklbw m2, m0	; l0 l1 l2 l3 l4 l5 l6 l7 word

	paddw   m1, m2		; t0+l0 t1+l1 t2+l2 t3+l3 t4+l4 t5+l5 t6+l6 t7+l7 word
	phaddw  m1, m1		; t0+l0+t1+l1 t2+l2+t3+l3 t4+l4+t5+l5 t6+l6+t7+l7 t0+l0+t1+l1 t2+l2+t3+l3 t4+l4+t5+l5 t6+l6+t7+l7 word
	phaddw  m1, m1		; t0+l0+t1+l1+t2+l2+t3+l3 t4+l4+t5+l5+t6+l6+t7+l7 t0+l0+t1+l1+t2+l2+t3+l3 t4+l4+t5+l5+t6+l6+t7+l7 
						; t0+l0+t1+l1+t2+l2+t3+l3 t4+l4+t5+l5+t6+l6+t7+l7 t0+l0+t1+l1+t2+l2+t3+l3 t4+l4+t5+l5+t6+l6+t7+l7 word
	phaddw  m1, m1		; t0+l0+t1+l1+t2+l2+t3+l3+t4+l4+t5+l5+t6+l6+t7+l7 t0+l0+t1+l1+t2+l2+t3+l3+t4+l4+t5+l5+t6+l6+t7+l7 .... word

	movdqu  m4, [data_8]
	paddw   m1, m4
	
	; dc >>= log2_size + 1;
	psraw   m1, 4
	movdqa  m4, m1

	; 存dc值
	packuswb m1, m1

loop_8:
	;movq [r0], m1
	;lea  r0, [r0+r3]
	;dec r4
	;jg  loop_8

	movq [r0		 ], m1
	movq [r0 +     r3],	m1
	movq [r0 +   2*r3],	m1
	movq [r0 +     r4],	m1
	add  r0, r3
	add  r0, r4
	movq [r0		 ], m1
	movq [r0 +     r3],	m1
	movq [r0 +   2*r3],	m1
	movq [r0 +     r4],	m1

loop_end:
	REP_RET