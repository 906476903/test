#include <iostream>
#include <windows.h>
#include "sys.h"
#include "func.h"
#include "main_display.h"
#include "Menu_display.h"
#include "const.h"
#include "S_L.h"
#include <ctime>
#include "global_variables.h"
#include "texture.h"
#include "setting.h"
#include "Intro_display.h"
#include <GL/glut.h>
#include <conio.h>
#include <stdlib.h>

using namespace std;

sys s=sys(part_r,ep,ef,E,rou,ttnum,f,dt,boundary,mode);
 Main_display m;
 Menu_display menuplay;
 setting settingplay=setting();
 Intro_Display intro=Intro_Display();
//模式1是动力学，2是随机算法（这两个是刚性边界），3是动力学，4是随机算法（这两个是周期边界）
//对周期边界的电势图估计做不出来了。。

//按现在的代码如果盒子里放不下那么多粒子是不会报错的，但是明显可以看出来有问题。。
HWND openglwnd;

void init()
{
    settingplay.setting_para_to_sys(s);
    settingplay.setting_button_position();
    s.system_start();
    menuplay.menu_display_init_button();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    LoadGLTextures(29);

}

void Reshape(int w,int h)
{

    switch(MODE)
   {
   case SMENU:
    menuplay.menu_display_reshape(w,h);
    break;
   case SSHOW:
     m.main_display_reshape(w,h,s);
    break;
   case SSETTING:
    settingplay.setting_reshape(w,h);
    break;
   case SINTRO:
    intro.Intro_Display_reshape(w,h);
    break;
   default:;
   }

}

void Display()
{
     switch(MODE)
   {
   case SMENU:
   menuplay.menu_display_play();
    break;
   case SSHOW:
   m.main_display_play(s);
    break;
   case SSETTING:
   settingplay.setting_display();
    break;
   case SINTRO:
    intro.Intro_Display_play();
    break;
   default:;
   }
}

void Idle()
{
    clock_t start_time=clock();
  if(MODE==SMENU&&menuplay.texture_total<0||MODE==SSETTING||MODE==SINTRO)
  {
      while(clock()-start_time<0.1/3.0*(double)CLOCKS_PER_SEC);
  }
  /*if(MODE==SMENU&&menuplay.texture_total>=0)
  {
      while(clock()-start_time<0.1/3.0*(double)CLOCKS_PER_SEC)
      {
      if(menuplay.texture_total>=0) LoadGLTextures(menuplay.texture_total);
      menuplay.texture_total--;
      }
  }*/

    switch(MODE)
   {
   case SMENU:
    menuplay.menu_display_idle();
    break;
   case SSHOW:
    m.main_display_idle(s);
    break;
   case SSETTING:
    settingplay.setting_idle();
    break;
   case SINTRO:
    intro.Intro_Display_idle();
    break;
   default:;
   }
   Display();
}

void Mouse(int button,int state,int x,int y)        /*当鼠标按下或拿起时会回调该函数*/
{
     switch(MODE)
   {
   case SMENU:
    menuplay.menu_display_mouse(button,state,x,y,s);
    break;
   case SSHOW:
   m.main_display_mouse(button,state,x,y);
    break;
   case SSETTING:
    settingplay.setting_mouse(button,state,x,y,s);
    break;
   case SINTRO:
    intro.Intro_Display_mouse(button,state,x,y);
    break;
   default:;
   }

  if(if_change)
  {
      switch(MODE)
   {
   case SMENU:
    menuplay.menu_display_init();
    menuplay.menu_display_init_button();
    break;
   case SSHOW:
   m.main_display_init();
    break;
   case SSETTING:
    settingplay.setting_init();
    break;
   case SINTRO:
   intro.Intro_Display_init();
    break;
   default:;
   }
   if_change=false;
  }
}

void OnMouseMove(int x,int y)          /*当鼠标移动时会回调该函数*/
{
     switch(MODE)
   {
   case SMENU:
    break;
   case SSHOW:
    m.main_display_mouse_move(x,y);
    break;
   case SSETTING:
    settingplay.setting_mousemove(x,y);
   // settingmousemotion(x,y);
    break;
   case SINTRO:
    intro.Intro_Display_mousemove(x,y);
   default:;
   }
}

void Keyboard(unsigned char c,int x,int y)
{
         switch(MODE)
   {
   case SMENU:
    menuplay.menu_display_key(c,x,y);
    break;
   case SSHOW:
    m.main_display_keyboard(c,x,y,s);
    break;
   case SSETTING:
    settingplay.setting_key(c,x,y,s);
    break;
   case SINTRO:
    intro.Intro_Display_key(c,x,y);
   default:;
   }

   if(if_change)
  {
      switch(MODE)
   {
   case SMENU:
    menuplay.menu_display_init();
    menuplay.menu_display_init_button();
    break;
   case SSHOW:
   m.main_display_init();
    break;
   case SSETTING:
    settingplay.setting_init();
    break;
   case SINTRO:
  intro.Intro_Display_init();
    break;
   default:;
   }
   if_change=false;
  }
}

void passivemotion(int x,int y)
{
    switch(MODE)
    {
    case SMENU:
    menuplay.menu_passive_motion(x,y);
        break;
    case SSHOW:
    m.main_display_passivemotion(x,y);
        break;
    case SSETTING:
    settingplay.setting_passivemotion(x,y);
    case SINTRO:
    intro.Intro_Display_passivemotion(x,y);
        break;
    }

}

/*LRESULT CALLBACK WndProc( HWND hWnd,     // 窗口的句柄
    UINT uMsg,     // 窗口的消息
    WPARAM wParam,     // 附加的消息内容
    LPARAM lParam)     // 附加的消息内容
{

}*/
int main(int argc,char*argv[])
{
    oldpt={-1,-1};
    /*char str[200];
      GetConsoleTitle(str,200);
      HWND hd=FindWindow(NULL,str);
      ShowWindow(hd,SW_HIDE);*/

    SetConsoleTitle("Dipole_info");
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowPosition(0.5*(GetSystemMetrics(SM_CXSCREEN)-width),0.5*(GetSystemMetrics(SM_CYSCREEN)-height));
    glutInitWindowSize(width-10,height-10);
    glutCreateWindow("Dipole");
    openglwnd=FindWindow(NULL,"Dipole");
    SetWindowLong(openglwnd,GWL_STYLE,WS_CAPTION | WS_SYSMENU |WS_MAXIMIZE|WS_MINIMIZEBOX|WS_BORDER|WS_OVERLAPPED);
    init();
    glutReshapeFunc(Reshape);
    glutDisplayFunc(Display);
    glutMouseFunc(Mouse);
    glutMotionFunc(OnMouseMove);
    glutPassiveMotionFunc(passivemotion);

    glutKeyboardFunc(Keyboard);
    glutIdleFunc(Idle);
    glutMainLoop();
    return 0;
}
