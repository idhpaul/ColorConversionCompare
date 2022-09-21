#include "libyuv.h"

#include <iostream>
#include <fstream>
#include <vector>


void check_cpu();

int main()
{
	const int src_width = 375;
	const int src_height = 500;
	const int dst_width = 375;
	const int dst_height = 500;

	// Create dst
	uint8_t* nv12 = new uint8_t[dst_width * dst_height + dst_width * dst_height >> 1];

	// Read jpg file
	std::ifstream infile("cat.jpg");
	std::ofstream outfile("cat.nv12");
	std::vector<uint8_t> buffer;


	//get length of file
	infile.seekg(0, infile.end);
	size_t length = infile.tellg();
	infile.seekg(0, infile.beg);

	//read file
	if (length > 0) {
		buffer.resize(length);
		infile.read((char*)(&buffer[0]), length);
	}

	auto ret = libyuv::MJPGToNV12(buffer.data(),
		length,
		nv12,
		dst_width,
		nv12 + dst_width * dst_height,
		dst_width,
		src_width,
		src_height,
		dst_width,
		dst_height);

	std::cout << ret << std::endl;

	// Save dst file
	outfile.write((const char*)(nv12), dst_width * dst_height + dst_width * dst_height >> 1);

	infile.close();
	outfile.close();

	// Delete dst
	delete[] nv12;
}

void check_cpu()
{
	std::cout << libyuv::InitCpuFlags() << std::endl;

	for (int i = 0x2; i <= 0x800000; i = i << 1)
	{
		std::cout << libyuv::TestCpuFlag(i) << std::endl;
	}
}

