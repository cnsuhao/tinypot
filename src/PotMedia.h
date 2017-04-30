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
    PotStreamVideo* _streamVideo = nullptr;
    PotStreamAudio* _streamAudio = nullptr;
    PotStreamSubtitle* _streamSubtitle = nullptr;
    int _extAudioFrame = 1; //�����ѹһ֡��Ƶ����ʱ��Ƶ�ߴ�ܴ󣬿��ܵ�����Ƶ�������
private:
    int _count = 0;
    int _totalTime = 0;
    int _lastdts = 0;
    int _timebase = 0;
    bool _seeking = false;
public:
    PotStreamVideo* getVideo() { return _streamVideo; };
    PotStreamAudio* getAudio() { return _streamAudio; };
    int decodeFrame();
    int openFile(const std::string& filename);
    int getAudioTime();
    int getVideoTime();
    int seekTime(int time, int direct = 1, int reset = 0);
    int seekPos(double pos, int direct = 1, int reset = 0);
    int showVideoFrame(int time);
    int getTotalTime() { return _totalTime; }
    int getTime();
    void destroy();
    bool isMedia();
    void setPause(bool pause);
};

