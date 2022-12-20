#ifndef IMAGE_H
#define IMAGE_H
#include <vector>

class Image
{
public:

private:
	std::vector<uint8_t> data;
	int width = 0;
	int height = 0;
	int format;
};

#endif