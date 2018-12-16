const int YR = 19595, YG = 38470, YB = 7471, CB_R = -11059, CB_G = -21709, CB_B = 32768, CR_R = 32768, CR_G = -27439, CR_B = -5329;
//inline uchar clamp(__global int i) { if ((uchar)i > 255U) { if (i < 0){ i = 0;} else if (i > 255){ i = 255; } return (uchar)i; }
//unsigned char uint8;


__kernel void RGB_to_YCC(__global uchar *pDst, __global const uchar *pSrc)
{
	int idx = get_global_id(0);
	
	const int r = pSrc[0 + idx * 3], g = pSrc[1 + idx * 3], b = pSrc[2 + idx * 3];
	
	pDst[0 + idx * 3] = (uchar)((r * YR + g * YG + b * YB + 32768) >> 16);
	int i = 128 + ((r * CB_R + g * CB_G + b * CB_B + 32768) >> 16);
	
	if ((uchar)i > 255U)
	{
		if (i < 0)
		{
			i = 0;
		}
		else if (i > 255)
		{
			i = 255;
		}
	}
	pDst[1 + idx * 3] = (uchar)i;
	
	i = 128 + ((r * CR_R + g * CR_G + b * CR_B + 32768) >> 16);
	
	if ((uchar)i > 255U)
	{
		if (i < 0)
		{
			i = 0;
		}
		else if (i > 255)
		{
			i = 255;
		}
	}
	pDst[2 + idx * 3] = (uchar)i;
}