#include <arm_neon.h>

void x264_ff_pred_dc_8x8_neon_intrinsic(unsigned char *_src, const unsigned char *_top,
										const unsigned char *_left,
										int stride, int log2_size)
{
	uint8x8_t top, left, dc;
    uint16x8_t  dst;

	int		      stride3 = stride * 3;
	unsigned char *src3   = _src + stride3;

	top  = vld1_u8(_top);
	left = vld1_u8(_left);

	dst = vaddl_u8(top, left);
	dst = vpaddq_s16(dst, dst);
	dst = vpaddq_s16(dst, dst);
	dst = vpaddq_s16(dst, dst);

	/* dc >>= log2size + 1; */
	dc = vqrshrun_n_s16(dst, 4);

	vst1_u8(_src		   , dc);
	vst1_u8(_src +   stride, dc);
	vst1_u8(_src + 2*stride, dc);
	vst1_u8(src3		   , dc);
	vst1_u8(src3 +   stride, dc);	
	vst1_u8(src3 + 2*stride, dc);	
	vst1_u8(src3 + 3*stride, dc);
	vst1_u8(src3 + 4*stride, dc);
}
