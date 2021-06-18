bits 32

section .data
align 16

data_8    dw  8, 8, 8, 8, 8, 8, 8, 8

section .text
align 16

global  x264_ff_pred_dc_8x8_x86_sse2
x264_ff_pred_dc_8x8_x86_sse2:
	push ebp
	mov  ebp ,esp

	push  esi
	push  edi
	
	mov esi, [ebp+8]	; src
	mov edi, [ebp+12]	; top
	mov eax, [ebp+16]	; left
	mov edx, [ebp+20]	; stride
	mov ecx,  8			; size
	
	; 获取left和top的像素点
	movq xmm0, [edi]	; 获取top的8个像素点
	movq xmm1, [eax]	; 获取left的8个像素点
	
	pxor xmm2, xmm2		; xmm2清零
	punpcklbw xmm0, xmm2 ; 将8bit扩展为16bit
	punpcklbw xmm1, xmm2 ; 将8bit扩展为16bit
	
	; dc += left[i] + top[i];
	paddw xmm0, xmm1 	 ; top[i]+left[i] 按字相加
	
	phaddw xmm0, xmm0    ; 按字水平相加
	phaddw xmm0, xmm0    
	phaddw xmm0, xmm0	 ; xmm0中8个字存放top[i]+left[i]
 
    movdqa xmm4, [data_8] ; 8
	paddw  xmm0, xmm4
	
	; dc >>= log2_size + 1;
	psraw  xmm0, 4		 ; xmm0存放8个16位的dc值
	movdqa xmm4, xmm0	 ; 保存xmm0
	
	; 存dc值
	packuswb  xmm0, xmm0 ; 将16bit转换成8bit
	
loop_8:
	movq [esi], xmm0
	lea  esi, [esi+edx]
	dec ecx
	jg  loop_8
	
loop_end:
	pop  edi
	pop  esi
	mov  esp, ebp 		; 收回局部变量的空间
	pop  ebp
	ret	