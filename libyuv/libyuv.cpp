#define _CRT_SECURE_NO_WARNINGS
#include "libyuv.h"

#include <iostream>
#include <fstream>
#include <vector>

#define align_buffer_page_end(var, size)                                    \
    uint8_t* var##_mem =                                                    \
        reinterpret_cast<uint8_t*>(malloc(((size) + 4095 + 63) & ~4095));   \
    uint8_t* var = reinterpret_cast<uint8_t*>(                              \
        (intptr_t)(var##_mem + (((size) + 4095 + 63) & ~4095) - (size)) & ~63)

#define free_aligned_buffer_page_end(var)   \
    free(var##_mem);                        \
    var = 0

void check_cpu();

#define TEST_WIDTH_HEIGHT 1
#define TEST_FULL_BUFFER 0

int main()
{
	const int src_width = 375;
	const int src_height = 500;
	const int dst_width = 375;
	const int dst_height = 500;
#if test
	const int half_src_width = (src_width + 1) >> 1;
	const int half_src_height = (src_height + 1) >> 1;
#else
	// 해당 나누기를 했을 경우에 힙 오류가 남
	const int half_src_width = src_width >> 1;
	const int half_src_height = src_height>> 1;
#endif


	// Create dst
	uint8_t* nv12 = new uint8_t[src_width * src_height + half_src_width * half_src_height * 2];
	uint8_t* nv12_y = new uint8_t[src_width * src_height];
	uint8_t* nv12_uv = new uint8_t[half_src_width * half_src_height * 2];

	// Read jpg file
	std::ifstream infile("cat.jpg",std::ios_base::binary);
	std::ofstream outfile("cat.yuv");
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

	int32_t test_width, test_height;
	libyuv::MJPGSize(buffer.data(), length, &test_width, &test_height);

	auto ret = libyuv::MJPGToNV12(buffer.data(),
		length,
		nv12_y,
		src_width,
		nv12_uv,
		half_src_width * 2,
		src_width,
		src_height,
		dst_width,
		dst_height);
	if (0 == ret)
		std::cout << "change to nv12 success" << std::endl;
	else
		std::cout << "change to nv12 failed" << std::endl;


	// Save dst file
	outfile.write((const char*)(nv12_y), dst_width * dst_height);
	outfile.write((const char*)(nv12_uv), half_src_width * half_src_height * 2);

	infile.close();
	outfile.close();

	// Delete dst
	delete[] nv12;
	delete[] nv12_y;
	delete[] nv12_uv;

	std::cout << "end" << std::endl;

    /*int32_t width = 0, height = 0;

    FILE* dst;
    FILE* ori = fopen("cat.jpg", "rb+");
    fseek(ori, 0, SEEK_END);
    uint32_t data_size = ftell(ori);
    fseek(ori, 0, SEEK_SET);
    uint8_t* data = (uint8_t*)calloc(1, data_size);
    fread(data, data_size, 1, ori);
    fclose(ori);

    int32_t ret = libyuv::MJPGSize(data, data_size, &width, &height);
    int32_t half_width = (width + 1) / 2;
    int32_t half_height = (height + 1) / 2;

    align_buffer_page_end(dst_y, width * height);
    align_buffer_page_end(dst_uv, half_width * half_height * 2);

    ret = libyuv::MJPGToNV12(data, data_size, dst_y, width, dst_uv, half_width * 2, width, height, width, height);
    if (0 == ret) {
        printf("change to nv12 success\n");
    }
    else {
        printf("change to nv12 failed\n");
        goto errChange;
    }

    dst = fopen("test.yuv", "wb+");
    fwrite(dst_y, width * height, 1, dst);
    fwrite(dst_uv, half_width * half_height * 2, 1, dst);
    fclose(dst);

errChange:
    free(data);
    free_aligned_buffer_page_end(dst_y);
    free_aligned_buffer_page_end(dst_uv);
	*/
}

void check_cpu()
{
	std::cout << libyuv::InitCpuFlags() << std::endl;

	for (int i = 0x2; i <= 0x800000; i = i << 1)
	{
		std::cout << libyuv::TestCpuFlag(i) << std::endl;
	}
}

