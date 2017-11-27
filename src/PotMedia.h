#pragma once

#include "PotBase.h"
#include "PotStream.h"
#include "PotStreamAudio.h"
#include "PotStreamVideo.h"
#include "PotStreamSubtitle.h"

//#include "BigPotSubtitle.h"


class PotMedia : public PotBase
{
public:
    PotMedia();
    virtual ~PotMedia();
private:
    PotStreamVideo* stream_video_ = nullptr;
    PotStreamAudio* stream_audio_ = nullptr;
    PotStreamSubtitle* stream_subtitle_ = nullptr;
    int ext_audio_frame_ = 1; //�����ѹһ֡��Ƶ����ʱ��Ƶ�ߴ�ܴ󣬿��ܵ�����Ƶ�������
private:
    int count_ = 0;
    int total_time_ = 0;
    int lastdts_ = 0;
    int timebase_ = 0;
    bool seeking_ = false;
public:
    PotStreamVideo* getVideo() { return stream_video_; };
    PotStreamAudio* getAudio() { return stream_audio_; };
    int decodeFrame();
    int openFile(const std::string& filename);
    int getAudioTime();
    int getVideoTime();
    int seekTime(int time, int direct = 1, int reset = 0);
    int seekPos(double pos, int direct = 1, int reset = 0);
    int showVideoFrame(int time);
    int getTotalTime() { return total_time_; }
    int getTime();
    void destroy();
    bool isMedia();
    void setPause(bool pause);
};

