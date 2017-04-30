#include "PotSubtitleAss.h"


PotSubtitleAss::PotSubtitleAss()
{
    init();
}


PotSubtitleAss::~PotSubtitleAss()
{
    destroyAllTex();
    closeSubtitle();
    destroy();
}

void PotSubtitleAss::init()
{
    _lib = ass_library_init();
    _ren = ass_renderer_init(_lib);
    ass_set_fonts(_ren, fontname_.c_str(), "Sans", 0, "", 0);
}

bool PotSubtitleAss::openSubtitle(const std::string& filename)
{
    //�����Ĳ�����char*,Ϊ�����⸴��һ��
    auto s = filename;
    //if (checkFileExt(filename))
    _track = ass_read_file(_lib, (char*)s.c_str(), NULL);
    haveSubtitle_ = (_track != nullptr);
    if (haveSubtitle_) { subfilename_ = filename; }
    return  haveSubtitle_;
}

void PotSubtitleAss::show(int time)
{
    int a;
    _img = ass_render_frame(_ren, _track, time, &a);
    //cout << engine_->getTicks() << endl;
    auto img = _img;
    if (a)
    {
        destroyAllTex();
        while (img)
        {
            auto t = engine_->transBitmapToTexture(img->bitmap, img->color, img->w, img->h, img->stride);
            engine_->renderCopy(t, img->dst_x, img->dst_y, img->w, img->h, 1);
            _tex_v.push_back(t);
            img = img->next;
        }
    }
    else
    {
        int i = 0;
        while (img && i < _tex_v.size())
        {
            engine_->renderCopy(_tex_v[i++], img->dst_x, img->dst_y, img->w, img->h, 1);
            img = img->next;
        }
    }
    //cout << engine_->getTicks() << endl;
}

void PotSubtitleAss::destroy()
{
    ass_renderer_done(_ren);
    ass_library_done(_lib);
}

void PotSubtitleAss::setFrameSize(int w, int h)
{
    //if (_track)
    ass_set_frame_size(_ren, w, h);
}

void PotSubtitleAss::closeSubtitle()
{
    if (_track)
    { ass_free_track(_track); }
    _track = nullptr;
}

void PotSubtitleAss::destroyAllTex()
{
    for (auto t : _tex_v)
    {
        engine_->destroyTexture(t);
    }
    _tex_v.clear();
}