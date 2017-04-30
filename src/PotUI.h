#pragma once
#include "PotBase.h"

class PotUI : public PotBase
{
public:
    PotUI();
    virtual ~PotUI();
private:
    BP_Texture* _square, *_text;

    int _time, _totoalTime, _volume;
    int _win_w, _win_h;

    int x = 0, y = 0, w = 0, h = 0;
    uint8_t _alpha;
    std::string _fontname;
public:
    void init();
    void setTime(int t) { _time = t; };
    void setTotalTime(int t) { _totoalTime = t; };
    void drawBall();
    void drawText(const std::string& text);
    void drawUI(uint8_t alpha, int time, int totoalTime, int volume);
    void destory();
    std::string convertTimeToString(int time);
};

