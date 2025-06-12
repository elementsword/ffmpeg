#include <iostream>

extern "C" {
#include <libavcodec/avcodec.h>
}

int main() {
    const char* config = avcodec_configuration();
    std::cout << "FFmpeg avcodec configuration:\n" << config << std::endl;
    return 0;
}
