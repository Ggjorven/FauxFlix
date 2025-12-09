#include <iostream>

#include <Faux/Core/Window.hpp>

extern "C" {
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libavutil/imgutils.h>
	#include <libswscale/swscale.h>
	#include <libswresample/swresample.h>
}

using namespace Faux;

int main(const int argc, const char* argv[])
{
	(void)argc; (void)argv;

	std::cout << "Hello, world!" << std::endl;

	std::cout << "FFmpeg version: " << av_version_info() << "\n";

	return 0;
}