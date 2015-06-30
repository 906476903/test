#ifndef MENU_DISPLAY_H_INCLUDED
#define MENU_DISPLAY_H_INCLUDED
#include "main_display.h"

class Menu_display
{
  public:
    Menu_display();
    ~Menu_display(){};

    float menu_dt;//目录页的时间步长
    float button_r;//球形按钮的半径
    float screenboundary;//屏幕长度
    float logoZ;
    bool menu_mouse_on[4];//判断鼠标是不是在某个选项上面
    bool menu_pause;
    int texture_total;

    void menubuttonmove();
    void menubuttonshow();

    struct moving_button
    {
      double x,y;
      double vx,vy;
    }movbutton[4];

    void menu_display_init();
    void menu_display_init_button();
    void menu_display_play();
    void menu_display_idle();
    void menu_display_mouse(int button,int state,int x,int y,sys&s);
    void menu_display_key(char c,int x,int y);
    void menu_display_reshape(int w,int h);
    void menu_display_usefog();
    void menu_passive_motion(int x,int y);
};

#endif // MENU_DISPLAY_H_INCLUDED
