#ifndef INTRO_DISPLAY_H_INCLUDED
#define INTRO_DISPLAY_H_INCLUDED

class Intro_Display
{
public:
    Intro_Display();
    ~Intro_Display(){};

    void Intro_Display_play();
    void Intro_Display_reshape(int w,int h);
    void Intro_Display_idle();
    void Intro_Display_mouse(int button,int state,int x,int y);
    void Intro_Display_mousemove(int x,int y);
    void Intro_Display_init();
    void Intro_Display_key(char c,int x,int y);
    void Intro_Display_backbutton(float x,float y,float t);
    void Intro_Display_passivemotion(int x,int y);

    float introY;
    bool intro_mouse_on[3];//分别对应up，down，back三个键
};

#endif // INTRO_DISPLAY_H_INCLUDED
