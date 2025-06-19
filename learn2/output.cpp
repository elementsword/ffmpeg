#include <iostream>
#include <fstream>
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
    std::ofstream outfile("output.txt");                               // 输出文件
    std::ofstream outH264("output.h264", std::ios::binary);            // 输出文件
    std::ofstream outYUV("output.yuv", std::ios::binary);              // 新增 YUV 文件
    const std::string url = "/root/learn/ffmpeg/audioFile/Titanic.ts"; // 视频地址
    AVFormatContext *avformatcontext = nullptr;                        // 最外层 context
    AVCodecContext *avcodeccontext = nullptr;                          // 视频或音频流的信息
    const AVCodec *codec = nullptr;                                    // AVCodec 解编码器
                                                                       // 变量初始化
    AVPacket *packet = nullptr;
    AVFrame *frame = nullptr;
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

    avcodeccontext = avcodec_alloc_context3(decoder);                                              // 创建 AVCodecContext，
    avcodec_parameters_to_context(avcodeccontext, avformatcontext->streams[videoIndex]->codecpar); // 并从 codecpar 拷贝参数
    codec = avcodeccontext->codec;
    avcodec_open2(avcodeccontext, codec, nullptr);
    packet = av_packet_alloc();
    frame = av_frame_alloc();
    // 视频宽高
    int width = avcodeccontext->width;
    int height = avcodeccontext->height;
    outfile << avformatcontext->iformat->long_name << std::endl;
    outfile << avformatcontext->bit_rate << std::endl;
    outfile << avformatcontext->duration << std::endl;
    outfile << width << std::endl;
    outfile << height << std::endl;
    outfile << codec->name << std::endl;

    while (av_read_frame(avformatcontext, packet) >= 0)
    {
        if (packet->stream_index == videoIndex)
        {
            outH264.write((const char *)(packet->data), packet->size);
            if (avcodec_send_packet(avcodeccontext, packet) == 0)
            {
                while (avcodec_receive_frame(avcodeccontext, frame) == 0)
                {
                    // yuv文件格式 frame[0] y frame[1] u frame[2] v
                    //  linesize 包括填充数据 所以跳过的时候 要跳过linesize 而写入的数据 只写入width
                    // Y（亮度）：frame->data[0]，大小 width * height
                    // U（蓝色差）：frame->data[1]，大小 width/2 * height/2
                    // V（红色差）：frame->data[2]，大小 width/2 * height/2
                    // 写 Y
                    for (int i = 0; i < height; i++)
                        outYUV.write((char *)(frame->data[0] + i * frame->linesize[0]), width);

                    // 写 U
                    for (int i = 0; i < height / 2; i++)
                        outYUV.write((char *)(frame->data[1] + i * frame->linesize[1]), width / 2);

                    // 写 V
                    for (int i = 0; i < height / 2; i++)
                        outYUV.write((char *)(frame->data[2] + i * frame->linesize[2]), width / 2);
                }
            }
        }
        av_packet_unref(packet);
    }
    // 释放资源
    av_packet_free(&packet);
    av_frame_free(&frame);
    avcodec_free_context(&avcodeccontext);
    avformat_close_input(&avformatcontext);
    outfile.close();
    outH264.close();
    outYUV.close();
}