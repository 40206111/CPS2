#define __CL_ENABLE_EXCEPTIONS

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <CL/cl.hpp>

#include "kernel.h"

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

			//Get Devices
			vector<Device> devices;
			platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);

			//set context
			Context context(devices);

			//Create a command queue
			CommandQueue queue(context, devices[0]);

			//set data size
			std::size_t DATA_SIZE = sizeof(uint8) * num_pixels;

			//create buffers
			Buffer bufpDst(context, CL_MEM_READ_ONLY, DATA_SIZE);
			Buffer bufpSrc(context, CL_MEM_READ_ONLY, DATA_SIZE);

			//Copy data to GPU
			queue.enqueueWriteBuffer(bufpSrc, CL_TRUE, 0, DATA_SIZE, pSrc);

			//Read in Kernel source
			ifstream file("kernel.cl");
			string code(istreambuf_iterator<char>(file), (istreambuf_iterator<char>()));

			//Create Program
			Program::Sources source(1, make_pair(code.c_str(), code.length() + 1));
			Program program(context, source);

			//build program for devices
			program.build(devices);

			//Create the kernel
			cl::Kernel theKernel(program, "RGB_to_YCC");
			
			//Set kernel arguments
			theKernel.setArg(0, bufpDst);
			theKernel.setArg(1, bufpSrc);
		
			//Execute Kernel
			NDRange(num_pixels);
			NDRange local(256);
			queue.enqueueNDRangeKernel(theKernel, NullRange, NDRange(1024), local);
			
			//Copy results back
			queue.enqueueReadBuffer(bufpSrc, CL_TRUE, 0, DATA_SIZE, pDst);
		}
		catch (Error error)
		{
			cout << error.what() << "(" << error.err() << ")" << endl;
		}
	}
}