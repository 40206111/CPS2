#define __CL_ENABLE_EXCEPTIONS

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <CL/cl.hpp>

#include "..\..\jpeg_cl\jpeg_comp\jpeg_comp\kernel.h"

typedef unsigned char uint8;

using namespace std;
using namespace cl;

namespace Kernel
{
	void RGB_to_YCC(uint8 *pDst, const uint8 *pSrc, int num_pixels)
	{
		try
		{
			// Get the platforms
			vector<Platform> platforms;
			Platform::get(&platforms);

			vector<Device> devices;
			platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);

			Context context(devices);

			CommandQueue queue(context, devices[0]);

			std::size_t DATA_SIZE = sizeof(uint8) * num_pixels;

			Buffer bufpDst(context, CL_MEM_READ_ONLY, DATA_SIZE);
			Buffer bufpSrc(context, CL_MEM_READ_ONLY, DATA_SIZE);

			queue.enqueueWriteBuffer(bufpSrc, CL_TRUE, 0, DATA_SIZE, pSrc);

			ifstream file("kernel.cl");
			string code(istreambuf_iterator<char>(file), (istreambuf_iterator<char>()));

			Program::Sources source(1, make_pair(code.c_str(), code.length() + 1));
			Program program(context, source);

			program.build(devices);

			cl::Kernel theKernel(program, "RGB_to_YCC");

			theKernel.setArg(0, bufpDst);
			theKernel.setArg(1, bufpSrc);
		
			NDRange(num_pixels);
			NDRange local(256);
			queue.enqueueNDRangeKernel(theKernel, NullRange, NDRange(1024), local);

			queue.enqueueReadBuffer(bufpSrc, CL_TRUE, 0, DATA_SIZE, pDst);
		}
		catch (Error error)
		{
			cout << error.what() << "(" << error.err() << ")" << endl;
		}
	}
}