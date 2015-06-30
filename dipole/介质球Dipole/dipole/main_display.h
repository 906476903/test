#ifndef MAIN_DISPLAY_H_INCLUDED
#define MAIN_DISPLAY_H_INCLUDED
#include <GL/glut.h>
#include "sys.h"

class Main_display
{
public:
    Main_display();
   ~Main_display(){};

    struct Camera                  /*用于摄像机定位*/
    {
        GLfloat xeye;
        GLfloat yeye;
        GLfloat zeye;
    }camera;
    struct Polar                   /*球坐标*/
    {
        double r;                   /*距离r*/
        double alpha;               /*水平偏角α*/
        double fy;                  /*竖直偏角φ（单位均用角度）*/
    }polar;

    void SetCamera(GLfloat x,GLfloat y);
    void main_display_keyboard(unsigned char c,int x,int y,sys &s);
    void main_display_mouse(int button,int state,int x,int y);
    void main_display_mouse_move(int x,int y);
    void main_display_play(sys &s);
    void main_display_init();
    void main_display_idle(sys &s1);
    void main_display_reshape(int w,int h,sys &s);
    void main_display_passivemotion(int x,int y);

    void main_display_continue_button(int x,int y);
    void main_display_quit_button(int x,int y);//x，y是按钮中心坐标，以像素为单位
    void main_display_yn_button();//因为位置固定所以不需要参数
    void main_display_electricfield();//画电场
    void main_display_electric_button(int x,int y);//绘制电场按钮

    double drag_velocity;
    float texture_r;//按钮半径，以像素为单位
    bool main_display_pause;
    bool quit;
    float bigger;//按钮变大的倍数
    int M;
    bool play_mouse_on[3];//看pause，quit按钮是不是有鼠标在上面
    bool quit_mouse_on[2];//看quit界面下按钮上是否有鼠标
    bool show_electric;

    int pause_button_x;
    int pause_button_y;
    int leave_button_x;
    int leave_button_y;
};

#endif // MAIN_DISPLAY_H_INCLUDED
