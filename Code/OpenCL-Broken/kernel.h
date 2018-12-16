#pragma once

typedef unsigned char uint8;

namespace Kernel {
	void  RGB_to_YCC(uint8 *pDst, const uint8 *pSrc, int num_pixels);
}
