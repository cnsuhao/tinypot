﻿#pragma once

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
#include "libavformat/avformat.h"
}

#include "PotBase.h"
#include "Engine.h"
#include <algorithm>
#include <map>
#include "Config.h"
#include <mutex>

enum PotMediaType
{
    BPMEDIA_TYPE_VIDEO = AVMEDIA_TYPE_VIDEO,
    BPMEDIA_TYPE_AUDIO = AVMEDIA_TYPE_AUDIO,
    BPMEDIA_TYPE_SUBTITLE = AVMEDIA_TYPE_SUBTITLE,
};

/*
Context - 未解码数据
Packet - 读取的一个未解的包
Frame - 解得的一帧数据
Content - 转换而得的可以直接显示或播放的数据，包含时间，信息（通常为总字节），和指向数据区的指针
*/

class PotStream : public PotBase
{
public:
    struct Content
    {
        int time;
        int64_t info;
        void* data;
    };

    PotStream();
    virtual ~PotStream();
protected:
    PotMediaType type_;
    AVFormatContext* formatCtx_ = nullptr;
    AVFrame* frame_ = nullptr;
    AVStream* stream_ = nullptr;
    AVCodecContext* codecCtx_ = nullptr;
    AVCodec* codec_ = nullptr;
    AVPacket packet_;
    bool needReadPacket_ = true;
    int stream_index_ = -1;
    int decodeSizeInPacket_ = 0;
    double time_per_frame_ = 0, time_base_packet_ = 0;
    int maxSize_ = 0;  //为0时仅预解一帧, 理论效果与=1相同, 但不使用map和附加缓冲区
    AVSubtitle* subtitle_;
    std::string filename_;
    std::mutex mutex_;

    int ticks_shown_ = -1;  //最近展示的ticks
    int time_dts_ = 0, time_pts_ = 0, time_shown_ = 0;  //解压时间，应展示时间，最近已经展示的时间
    int time_other_ = 0;
    int start_time_ = 0;
    int total_time_ = 0;

    bool pause_ = false;
    int pause_time_ = 0;
    bool key_frame_ = false;
    void* data_ = nullptr;  //无缓冲时的用户数据, 可能为纹理或音频缓冲区
    int data_length_ = 0;
    bool stopping = false;  //表示放弃继续解压这个流
    //int frame_number_;
private:

    std::map<int, Content> _map;
    bool _decoded = false, _skip = false, _ended = false, _seeking = false;
    int _seek_record = 0;  //上次seek的记录
    virtual int avcodec_decode_packet(AVCodecContext*, void*, int*, AVPacket*) { return 0; }
private:
    virtual Content convertFrameToContent(void* p = nullptr)
    {
        return{ 0, 0, nullptr };
    }

    int dropContent(int key = -1);
    void setMap(int key, Content f);
    virtual void freeContent(void* p) {};
    void clearMap();
    bool needDecode();
    virtual bool needDecode2() { return true; };
    int decodeNextPacketToFrame(bool decode = true);
protected:
    void setDecoded(bool b);
    bool haveDecoded();
    void dropAllDecoded();
    bool useMap();
    Content getCurrentContent();
public:
    int openFile(const std::string& filename);
    int tryDecodeFrame(bool reset = false);
    void dropDecoded();
    int getTotalTime();

    void setSkip(bool b) { _skip = b; }
    void resetTimeBegin() { ticks_shown_ = -1; }

    int seek(int time, int direct = 1, int reset = 0);
    void setFrameTime();
    int getTime();
    int setAnotherTime(int time);
    int skipFrame(int time);

    void getSize(int& w, int& h);
    int getWidth() { return exist() ? codecCtx_->width : 0; }
    int getHeight() { return exist() ? codecCtx_->height : 0; }
    int getTimedts() { return time_dts_ > 0 ? time_dts_ : time_pts_; }
    int getTimeShown() { return time_shown_; }
    bool exist() { return this != nullptr && stream_index_ >= 0; }
    void resetTimeAxis(int time);
    bool isPause() { return pause_; }
    bool isKeyFrame() { return key_frame_; }
    virtual void setPause(bool pause);
    void resetDecoderState() { avcodec_flush_buffers(codecCtx_); }
    double getRotation();
    void getRatio(int& x, int& y);
    int getRatioX() { return exist() ? std::max(stream_->sample_aspect_ratio.num, 1) : 1; }
    int getRatioY() { return exist() ? std::max(stream_->sample_aspect_ratio.den, 1) : 1; }
    bool isStopping() { return stopping; }
};


