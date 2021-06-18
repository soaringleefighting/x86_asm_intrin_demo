void x264_ff_pred_dc_8x8(unsigned char *_src, const unsigned char *_top,
	const unsigned char *_left,
	int stride, int log2_size)
{
	int i = 0, j = 0;
	int size          = (1 << log2_size); // 8
	unsigned char *src        = (unsigned char *)_src;
	const unsigned char *top  = (const unsigned char *)_top;
	const unsigned char *left = (const unsigned char *)_left;
	int dc            = size;
	//pixel4 a;
	for (i = 0; i < size; i++)
		dc += left[i] + top[i];

	dc >>= log2_size + 1;

	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			src[j + i * stride] = dc;
		}
	}
}