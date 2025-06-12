#include <iostream>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libavdevice/avdevice.h>
#include <libavutil/avutil.h>
#include <libpostproc/postprocess.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
}

int main()
{
    const std::string url = "/root/learn/ffmpeg/audioFile/input.avi"; // 视频地址
    AVFormatContext *avformatcontext;                                 // 最外层 context
    AVCodecContext *avcodeccontext;                                   // 视频或音频流的信息
    const AVCodec *codec;                                             // AVCodec 解编码器
                                                                      // 变量初始化
    AVPacket *packet;
    AVFrame *frame;
    avformat_open_input(&avformatcontext, url.c_str(), nullptr, nullptr); // 打开视频
    avformat_find_stream_info(avformatcontext, nullptr);                  // 读取流信息
    // 找到音频 streams下表
    int videoIndex = -1;
    for (int i = 0; i < avformatcontext->nb_streams; i++)
    {
        if (avformatcontext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {

            videoIndex = i;
        }
    }

    const AVCodec *decoder = avcodec_find_decoder(avformatcontext->streams[videoIndex]->codecpar->codec_id); // 找到解码器

    avcodeccontext = avcodec_alloc_context3(decoder); // 创建 AVCodecContext，并从 codecpar 拷贝参数
    codec = avcodeccontext->codec;
    avcodec_open2(avcodeccontext, codec, nullptr);
    packet = av_packet_alloc();
    frame = av_frame_alloc();
    // 视频宽高
    int width = avcodeccontext->width;
    int height = avcodeccontext->height;
}