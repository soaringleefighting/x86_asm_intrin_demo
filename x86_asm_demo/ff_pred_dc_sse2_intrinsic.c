//#include <xmmintrin.h>  /* SSE */
#include <emmintrin.h>	  /* SSE2 */
#include <pmmintrin.h>   /* SSE3 */
#include <tmmintrin.h>   /* SSSE3 */
//#include <smmintrin.h>  /* SSE4.1 */
//#include <immintrin.h>  /* AVX */

void x264_ff_pred_dc_8x8_sse2_intrinsic(unsigned char *_src, const unsigned char *_top,
										const unsigned char *_left,
										int stride, int log2_size)
{
	__m128i m0, m1, m2, m3;

	int		      stride3 = stride * 3;
	unsigned char *src3   = _src + stride3;

	m1 = _mm_loadl_epi64((__m128i* const) _top);
	m2 = _mm_loadl_epi64((__m128i* const) _left);
	m0 = _mm_setzero_si128();

	m1 = _mm_unpacklo_epi8(m1, m0);
	m2 = _mm_unpacklo_epi8(m2, m0);

	m1 = _mm_add_epi16(m1, m2);
	m1 = _mm_hadd_epi16(m1, m1);
	m1 = _mm_hadd_epi16(m1, m1);
	m1 = _mm_hadd_epi16(m1, m1);
	
	m3 = _mm_set1_epi16(8);
	m1 = _mm_add_epi16(m1, m3);

	/* dc >>= log2size + 1; */
	m1 = _mm_srai_epi16(m1, 4);

	/* dc */
	m1 = _mm_packus_epi16(m1, m1);

	_mm_storel_epi64((__m128i*) (_src			), m1);
	_mm_storel_epi64((__m128i*) (_src +   stride), m1);
	_mm_storel_epi64((__m128i*) (_src + 2*stride), m1);
	_mm_storel_epi64((__m128i*) (src3			), m1);
	_mm_storel_epi64((__m128i*) (src3 +   stride), m1);	
	_mm_storel_epi64((__m128i*) (src3 + 2*stride), m1);	
	_mm_storel_epi64((__m128i*) (src3 + 3*stride), m1);
	_mm_storel_epi64((__m128i*) (src3 + 4*stride), m1);
}
