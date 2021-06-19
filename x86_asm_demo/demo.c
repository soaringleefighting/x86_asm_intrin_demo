#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "os_time_sdk.h"

#define CLIP(x) (x>255? 255: x<0? 0:x)

void x264_ff_pred_dc_8x8(unsigned char *_src, const unsigned char *_top,
	const unsigned char *_left,
	int stride, int log2_size);

#if ARCH_X86
/* д��һ������x86inc.asm */
void x264_pred_dc_8x8_x86_ssse3(unsigned char *_src, const unsigned char *_top,
	const unsigned char *_left,
	int stride, int log2_size);

/* д������ԭʼassembly */
void x264_ff_pred_dc_8x8_x86_sse2(unsigned char *_src, const unsigned char *_top,
	const unsigned char *_left,
	int stride, int log2_size);

/* x86 intrinsic */
void x264_ff_pred_dc_8x8_sse2_intrinsic(unsigned char *_src, const unsigned char *_top,
	const unsigned char *_left,
	int stride, int log2_size);
#endif

#if ARCH_AARCH64
/* x86 intrinsic */
void x264_ff_pred_dc_8x8_neon_intrinsic(unsigned char *_src, const unsigned char *_top,
	const unsigned char *_left,
	int stride, int log2_size);
#endif


static void GenerateMatrix(unsigned char *src, unsigned char *top,unsigned char *left, unsigned char *dst, int stride, int height)
{
	int i = 0;
	srand((int)time(0)); // ����rand()��������������������

	for (i = 0; i < height*stride; i++)
	{
		src[i] = (unsigned char) CLIP(1.0*rand()/RAND_MAX * 255);
		dst[i] = src[i];
	}
	for (i = 0; i < height*stride; i++)
	{
		top[i] = (unsigned char) CLIP(1.0*rand()/RAND_MAX * 255);
	}
	for (i = 0; i < height*stride; i++)
	{
		left[i] = (unsigned char) CLIP(1.0*rand()/RAND_MAX * 255);
	}

}


int main(int argc, char* argv[])
{
	int ret = 0;
	int i = 0, j = 0;
	int times   = 500000;
	int NG_flag = 0;

	unsigned char src[255];
	unsigned char dst[255];
	unsigned char top[255];
	unsigned char left[255];
	os_timer pTimer = {0};
	double time_c = 0.0, time_opt = 0.0, time_opt_intrin = 0.0;

	GenerateMatrix(src, top, left, dst, 8, 8);

	os_sdk_inittimer(&pTimer);

	printf("src: \t\n");
	for (i = 0; i < 8 ; i++)
	{
		for (j = 0; j < 8; j++)
		{
			printf("%d\t", src[j+i*8]);
		}
		printf("\n");
	}

	printf("top: \t\n");
	for (i = 0; i < 8 ; i++)
	{
		for (j = 0; j < 8; j++)
		{
			printf("%d\t", top[j+i*8]);
		}
		printf("\n");
	}

	printf("left: \t\n");
	for (i = 0; i < 8 ; i++)
	{
		for (j = 0; j < 8; j++)
		{
			printf("%d\t", left[j+i*8]);
		}
		printf("\n");
	}

	printf("dst: \t\n");
	for (i = 0; i < 8 ; i++)
	{
		for (j = 0; j < 8; j++)
		{
			printf("%d\t", dst[j+i*8]);
		}
		printf("\n");
	}

	os_sdk_starttimer(&pTimer);
	for (i = 0; i < times; i++)
	{
		// Cʵ��
		x264_ff_pred_dc_8x8(src, top, left, 8, 3);
	}
	time_c = os_sdk_stoptimer(&pTimer);

	printf("src_pred_dc_C: \t\n");
	for (i = 0; i < 8 ; i++)
	{
		for (j = 0; j < 8; j++)
		{
			printf("%d\t", src[j+i*8]);
		}
		printf("\n");
	}

#if ARCH_X86
	os_sdk_starttimer(&pTimer);
	for (i = 0; i < times; i++)
	{
		// x86������Ż�
		x264_pred_dc_8x8_x86_ssse3(dst, top, left, 8, 3); // assembly����Ч������2.5�����ҡ�
		//x264_ff_pred_dc_8x8_x86_sse2(dst, top, left, 8, 3);
	}
	time_opt = os_sdk_stoptimer(&pTimer);


 	os_sdk_starttimer(&pTimer);
 	for (i = 0; i < times; i++)
 	{
 		// x86 intrinsic���
 		x264_ff_pred_dc_8x8_sse2_intrinsic(dst, top, left, 8, 3); // intrinsc����Ч������5�����ҡ�
 	}
 	time_opt_intrin = os_sdk_stoptimer(&pTimer);

	printf("src_pred_dc_X86ASM: \t\n");
	for (i = 0; i < 8 ; i++)
	{
		for (j = 0; j < 8; j++)
		{
			printf("%d\t", dst[j+i*8]);
		}
		printf("\n");
	}
#endif

#if ARCH_AARCH64
	os_sdk_starttimer(&pTimer);
	for (i = 0; i < times; i++)
	{
	
		x264_ff_pred_dc_8x8_neon_intrinsic(dst, top, left, 8, 3); 

	}
	time_opt_intrin = os_sdk_stoptimer(&pTimer);

	printf("arm neon intrinsic: \t\n");
	for (i = 0; i < 8 ; i++)
	{
		for (j = 0; j < 8; j++)
		{
			printf("%d\t", dst[j+i*8]);
		}
		printf("\n");
	}
#endif

	// ��C�����SSE�Ż���� ��ȷ��/һ���ԱȽ�
	for (i = 0; i < 8; i++) // 4
	{
		for (j = 0; j < 8; j++)
		{
			if(src[j+i*8] != dst[j+i*8])
			{
				NG_flag = 1;
				break;
			}
		}
	}

	if(NG_flag != 0)
	{
		printf("[demo] compare NG!!! \t");
	}
	else
	{
		printf("[demo] compare OK!!! \t");
	}


	printf("time_c: %f ms\t time_opt_asm: %f ms\t time_opt_intrinsic: %f ms\n", time_c, time_opt, time_opt_intrin);

#if WIN32
	system("pause");
#endif

	return 0;
}