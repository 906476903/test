#include "Main_display.h"
#include "const.h"
#include "func.h"
#include <stdio.h>
#include <cstring>
#include "sys.h"
#include "S_L.h"
#include "global_variables.h"
#include <cmath>
#include <GL/glut.h>
#include "texture.h"
#include <iostream>
using namespace std;

Main_display::Main_display()
{
    polar= {1.0,60.0f,75.0f};
    l_button_down=0;
    drag_velocity=0.5;
    main_display_pause=false;
    quit=false;
    texture_r=50;
    pause_button_x=700;
    pause_button_y=325;
    leave_button_x=700;
    leave_button_y=450;
    M=40;
    play_mouse_on[0]=play_mouse_on[1]=play_mouse_on[2]=false;
    quit_mouse_on[0]=quit_mouse_on[1]=false;
    bigger=1.3;
    show_electric=false;
}

void Main_display::SetCamera(GLfloat x,GLfloat y)
{
    GLfloat alpha,fy;                                    /*和它的名字一样，不过是单位是弧度*/
    if((polar.fy+y)>5.0f && (polar.fy+y)<175.0f)
    {
        /*定义竖直偏角只能在5°到175°之间*/
        polar.alpha += x*drag_velocity;                  /*根据鼠标移动的方向设置新的球坐标*/
        polar.fy += y*drag_velocity;

        if(polar.alpha>360.0f) polar.alpha-=360.0f;
        if(polar.alpha<0.0f) polar.alpha+=360.0f;         /*将水平偏角锁定在0°到360°之间*/

        alpha=polar.alpha*DEG_TO_RAD;
        fy=polar.fy*DEG_TO_RAD;                          /*角度转弧度*/
        camera.xeye = polar.r * sin(fy) * cos(alpha);       /*极坐标转直角坐标*/
        camera.zeye = polar.r * sin(fy) * sin(alpha);
        camera.yeye = polar.r * cos(fy);                  /*注意：竖直方向的是y轴*/
    }
}

void Main_display::main_display_keyboard(unsigned char c,int x,int y,sys &s)
{
    glMatrixMode(GL_MODELVIEW);
    switch(c)
    {
    case'a':
        polar.alpha-=10;
        break;
    case'd':
        polar.alpha+=10;
        break;
    case'w':
        polar.fy+=10;
        break;
    case's':
        polar.fy-=10;
        break;
    case'c':
        main_display_pause=false;
        break;
    case'p':
        main_display_pause=true;
        break;
    case 27:
        quit=true;
        break;
    default:
        break;
    }
   SetCamera(0.0,0.0);
    glutPostRedisplay();
}

void Main_display::main_display_mouse_move(int x,int y)
{
    if(l_button_down)                              /*如果鼠标没有按下则不改变摄像机位置*/
    {
        SetCamera(float(x-oldpt.x),float(oldpt.y-y));       /*根据增量重新设定摄像机位置*/

        oldpt.x=x;                                      /*将当前坐标置为旧坐标*/
        oldpt.y=y;
    }
}

void Main_display::main_display_mouse(int button,int state,int x,int y)
{
    extern sys s;
   if(quit)//如果此时处于询问是否保存界面
   {
     if(button==GLUT_LEFT&&state==GLUT_DOWN
        &&quit_mouse_on[0])//点击yes
     {
         save_present(s);
         MODE=SMENU;
         if_change=true;
         quit=false;
     }
     if(button==GLUT_LEFT&&state==GLUT_DOWN//点击no
        &&quit_mouse_on[1])
        {
            MODE=SMENU;
            if_change=true;
            quit=false;
        }
   }
   else//不处于保存界面，而处于显示界面
   {

      if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN&&x<=600)//在左部按下
    {
        l_button_down = true;                      /*当l_button_down为true时才可改变相机位置*/
        oldpt.x=x;                                /*记录旧坐标，以便移动发生时计算鼠标移动方向*/
        oldpt.y=y;
    }
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP&&x<=600)//在左部弹起
    {
        l_button_down = false;                    /*拿起时不能改变摄像机位置*/
    }
    if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN
       &&play_mouse_on[0])//按下continue、pause键
       {
           if(main_display_pause) main_display_pause=false;
           else main_display_pause=true;
       }
    if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN
       &&play_mouse_on[1])//按下exit键

       {
           quit=true;
           main_display_pause=false;
       }
   }
}

void Main_display::main_display_continue_button(int x,int y)
{
    float rx_unit=texture_r/100.;
    float ry_unit=texture_r/100.;//在右半部分把像素数据转化成坐标
    float X=(x-700)/100.;
    float Y=1.-y/300.;//在右半部分把像素坐标转化成坐标系坐标

    glEnable(GL_TEXTURE_2D);
    if(main_display_pause)//如果系统处于暂停状态，那么此时应该放上continue按钮
    {
        if(play_mouse_on[0])//如果按钮是逼格状态
        {
              glBindTexture(GL_TEXTURE_2D,theTexture[36]);
              glBegin(GL_POLYGON);
              for(int j=0;j<M;j++)
              {
                  if(play_mouse_on[0])
                  {
                     glTexCoord2f(0.5+0.475*cos(j*2*pi/M),0.5+0.475*sin(j*2*pi/M));
                     glVertex2f(X+rx_unit*bigger*cos(j*2*pi/M),Y+ry_unit*bigger/3*sin(j*2*pi/M));
                  }
              }
              glEnd();
        }
        else//如果按钮不是逼格状态
        {
            glBindTexture(GL_TEXTURE_2D,theTexture[31]);
            glBegin(GL_POLYGON);
            for(int j=0;j<M;j++)
            {

                glTexCoord2f(0.5+0.475*cos(j*2*pi/M),0.5+0.475*sin(j*2*pi/M));
                glVertex2f(X+rx_unit*cos(j*2*pi/M),Y+ry_unit/3*sin(j*2*pi/M));
            }
        glEnd();
        }
    }
    else
    {
      if(play_mouse_on[0])//如果按钮是逼格状态
        {
              glBindTexture(GL_TEXTURE_2D,theTexture[37]);
              glBegin(GL_POLYGON);
              for(int j=0;j<M;j++)
              {
                     glTexCoord2f(0.5+0.475*cos(j*2*pi/M),0.5+0.475*sin(j*2*pi/M));
                     glVertex2f(X+rx_unit*bigger*cos(j*2*pi/M),Y+ry_unit*bigger/3*sin(j*2*pi/M));
              }
              glEnd();
        }
        else//如果按钮不是逼格状态
        {
            glBindTexture(GL_TEXTURE_2D,theTexture[32]);
            glBegin(GL_POLYGON);
            for(int j=0;j<M;j++)
            {
                glTexCoord2f(0.5+0.475*cos(j*2*pi/M),0.5+0.475*sin(j*2*pi/M));
                glVertex2f(X+rx_unit*cos(j*2*pi/M),Y+ry_unit/3*sin(j*2*pi/M));
            }
        glEnd();
        }
    }
    glDisable(GL_TEXTURE_2D);
}

void Main_display::main_display_quit_button(int x,int y)
{
    float rx_unit=texture_r/100.;
    float ry_unit=texture_r/100.;//在右半部分把像素数据转化成坐标
    float X=(x-700)/100.;
    float Y=1.-y/300.;//在右半部分把像素坐标转化成坐标系坐标

    glEnable(GL_TEXTURE_2D);
        if(play_mouse_on[1])//如果按钮是逼格状态
        {
              glBindTexture(GL_TEXTURE_2D,theTexture[38]);
              glBegin(GL_POLYGON);
              for(int j=0;j<M;j++)
              {
                     glTexCoord2f(0.5+0.475*cos(j*2*pi/M),0.5+0.475*sin(j*2*pi/M));
                     glVertex2f(X+rx_unit*bigger*cos(j*2*pi/M),Y+ry_unit*bigger/3*sin(j*2*pi/M));
              }
              glEnd();
        }
        else//如果按钮不是逼格状态
        {
            glBindTexture(GL_TEXTURE_2D,theTexture[33]);
            glBegin(GL_POLYGON);
            for(int j=0;j<M;j++)
            {

                glTexCoord2f(0.5+0.475*cos(j*2*pi/M),0.5+0.475*sin(j*2*pi/M));
                glVertex2f(X+rx_unit*cos(j*2*pi/M),Y+ry_unit/3*sin(j*2*pi/M));
            }
        glEnd();
        }
    glDisable(GL_TEXTURE_2D);
}

void Main_display::main_display_yn_button()
{
    if(quit_mouse_on[0])
    {
       glBindTexture(GL_TEXTURE_2D,theTexture[45]);
        {
            glBegin(GL_POLYGON);
            for(int i=0;i<M;i++)
            {
                    glTexCoord2f(0.5+0.475*cos(2*i*pi/M),0.5+0.475*sin(2*i*pi/M));
                    glVertex3f(-0.715+bigger*37./400.*cos(2*i*pi/M),-0.58+bigger*37./300*sin(2*i*pi/M),-0.5f);
            }
            glEnd();
        }
    }
    else
    {
         glBindTexture(GL_TEXTURE_2D,theTexture[34]);
        {
            glBegin(GL_POLYGON);
            for(int i=0;i<M;i++)
            {
                    glTexCoord2f(0.5+0.48*cos(2*i*pi/M),0.5+0.48*sin(2*i*pi/M));
                     glVertex3f(-0.715+37./400.*cos(2*i*pi/M),-0.58+37./300*sin(2*i*pi/M),-0.5f);
            }
            glEnd();
        }
    }
    if(quit_mouse_on[1])
    {
        glBindTexture(GL_TEXTURE_2D,theTexture[46]);
        {
            glBegin(GL_POLYGON);
            for(int i=0;i<M;i++)
            {
                    glTexCoord2f(0.5+0.48*cos(2*i*pi/M),0.5+0.48*sin(2*i*pi/M));
                    glVertex3f(0.6325+bigger*37./400.*cos(2*i*pi/M),-0.58+bigger*37./300*sin(2*i*pi/M),-0.5f);
            }
            glEnd();
        }
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D,theTexture[35]);//n.bmp
        {
            glBegin(GL_POLYGON);
            for(int i=0;i<M;i++)
            {
                glTexCoord2f(0.5+0.48*cos(2*i*pi/M),0.5+0.48*sin(2*i*pi/M));
                glVertex3f(0.6325+37./400.*cos(2*i*pi/M),-0.58+37./300*sin(2*i*pi/M),-0.5f);
            }
            glEnd();
        }
    }
}

void Main_display::main_display_electric_button(int x,int y)
{
    glEnable(GL_TEXTURE_2D);
    float ra=620/289.;
    glBindTexture(GL_TEXTURE_2D,theTexture[15]);
    {
        float X=(x-700)/100.0;
        float Y=(600-y)/300.-1.;
        glBegin(GL_QUADS);
        glTexCoord2d(8/640.,164./640.); glVertex2f(X-0.6,Y-0.2/ra);
        glTexCoord2d(8/640.,453./640.); glVertex2f(X-0.6,Y+0.2/ra);
        glTexCoord2d(628./640.,453./640.); glVertex2f(X+0.6,Y+0.2/ra);
        glTexCoord2d(628./640.,164./640.); glVertex2f(X+0.6,Y-0.2/ra);
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
}

void Main_display::main_display_electricfield()
{
    if(!quit&&!main_display_pause)
    {
        extern sys s;
        int speed=150;
        glLineWidth(2.0);
        int t=s.cal_total%speed;
        int layer=40;
        int lines=20;
        float r0=0.2*boundary/layer;
        glBegin(GL_LINES);
        for(int i=0;i<layer;i++)
        {
            for(int j=0;j<lines*i;j++)
            {
                glColor3f(1.0,0.0,0.0);
                glVertex3f((i+0.2)*r0*cos(2*j*pi/lines/i),(i+0.2)*r0*sin(2*j*pi/lines/i),(1.0+-t*2.0/speed*2)*boundary+2.0*(float)i/layer*boundary);
                glColor3f(0.0,0.0,1.0);
                glVertex3f((i+0.2)*r0*cos(2*j*pi/lines/i),(i+0.2)*r0*sin(2*j*pi/lines/i),(0.70+-t*2.0/speed*2)*boundary+2.0*(float)i/layer*boundary);
            }
        }
        glEnd();
    }
    glLineWidth(1.0);
}

void Main_display::main_display_play(sys &s1)
{
     extern sys s;
     glClearColor(0.0,0.0,0.0,0.0);
     glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_ACCUM_BUFFER_BIT);
   if(quit)
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
        glViewport(0,0,width,height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1.0,1.0,-1.0,1.0,0.5,2.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0.0,0.0,0.0,0.0,0.0,-1.0,0.0,1.0,0.0);
        glBindTexture(GL_TEXTURE_2D,theTexture[30]);//贴背景
        {
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0,-1.0, -1.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0,1.0, -1.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0,1.0, -1.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0,-1.0, -1.0f);
            glEnd();
        }
        main_display_yn_button();

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_DEPTH_TEST);
    }
   else
   {
    //初始化
    glViewport(0,0,height,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-s1.boundary,s1.boundary,-s1.boundary,s1.boundary,-s1.boundary,s1.boundary);

    //设置光照，材料，视角等等
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_position[]= {1.0,1.0,1.0,0.0};
    GLfloat light_color[]= {1.0,1.0,1.0};
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light_color);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLightfv(GL_LIGHT0,GL_POSITION,light_position);
    gluLookAt(camera.xeye,camera.yeye,camera.zeye,0,0.0,0.0,0.0,1.0,0.0);
    GLfloat material_color[]= {0.0,0.4,1.0};
    GLfloat material_color_reflect[]= {1.0,1.0,1.0};
    glMaterialf(GL_FRONT,GL_SHININESS,120.0);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,material_color);

    //绘制粒子
    for(int i=0; i<s1.ttnum; i++)
    {
        glPushMatrix();
        if(s1.mode!=4)
        glTranslatef(s1.part_ob[i].x-s1.boundary/2.,s1.part_ob[i].y-s1.boundary/2.,s1.part_ob[i].z-s1.boundary/2.);
        if(s1.mode==4)
        glTranslatef(s1.part_ob[i].x-s1.boundary/2.+s1.part_r,s1.part_ob[i].y-s1.boundary/2.+s1.part_r
                     ,s1.part_ob[i].z-s1.boundary/2.+s1.part_r);
        glutSolidSphere(s1.part_r,20,20);
        glPopMatrix();
    }
    glDisable(GL_LIGHTING);

    //绘制线框和内部灰色填充物
    glColor3f(1.0,1.0,1.0);
    glutWireCube(s1.boundary);
    if(play_mouse_on[2])
    {
           show_electric=false;
           main_display_electricfield();
    }
    glColor4f(0.0,0.0,0.0,0.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);
    glColor4f(0.7,0.7,0.7,0.9);
    glDepthMask(GL_FALSE);
    glutSolidCube(s1.boundary);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glDisable(GL_LIGHT0);

    //绘制贴图部分
    glViewport(600,0,200,600);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glColor4f(1.0,1.0,1.0,1.0);
    glLineWidth(5.0f);
    glBegin(GL_LINES);
    glVertex2f(-1.0,1.0);
    glVertex2f(-1.0,-1.0);
    glEnd();
    glLineWidth(1.0f);

    main_display_electric_button(700,550);
    main_display_continue_button(pause_button_x,pause_button_y);
    main_display_quit_button(leave_button_x,leave_button_y);//绘制两个按钮

    //输出能量等信息
    if(s.mode==1||s.mode==3)
    {
        glprint(-0.9,0.92,"Energy:");
        sprintf(settingnum,"%.3f",s.Ek+s.Ep);glprint(-0.7,0.82,settingnum);
        glprint(-0.9,0.74,"Kinetic Energy:");
        sprintf(settingnum,"%.3f",s.Ek);glprint(-0.7,0.66,settingnum);
        glprint(-0.9,0.58,"Potential Energy:");
        sprintf(settingnum,"%.3f",s.Ep);glprint(-0.7,0.50,settingnum);
        glprint(-0.9,0.42,"Calculate Times:");
        sprintf(settingnum,"%d",s.cal_total); glprint(-0.7,0.34,settingnum);
        glprint(-0.9,0.26,"Total Time:");
        sprintf(settingnum,"%5.3f",0.6136*s.cal_total*s.dt);glprint(-0.7,0.18,settingnum);
        glprint(0.1,0.18,"ms");
    }
     if(s.mode==2||s.mode==4)
     {
        glprint(-0.9,0.90,"Potential Energy:");
        sprintf(settingnum,"%3f",s.Ep);glprint(-0.7,0.80,settingnum);
        glprint(-0.9,0.70,"Calculate Times:");
        sprintf(settingnum,"%d",s.cal_total); glprint(-0.7,0.60,settingnum);
     }

    glDisable(GL_TEXTURE_2D);
   }

   glFlush();
   glutSwapBuffers();
}

void Main_display::main_display_init()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    SetCamera(0.0,0.0);
    glDisable(GL_TEXTURE_2D);
}

void Main_display::main_display_reshape(int w,int h,sys& s)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glutPostRedisplay();
}

void Main_display::main_display_idle(sys &s)
{
    if(!main_display_pause&&!quit)
    {
        system_moveonce(s);
        s.cal_total++;
        if(s.cal_total%10==0) system_energy(s);
    }
}

void Main_display::main_display_passivemotion(int x,int y)
{
    if(quit)
    {
        if((x-114)*(x-114)+(y-473)*(y-473)<=37*37) quit_mouse_on[0]=true;
        else  if((x-114)*(x-114)+(y-473)*(y-473)>37*37*bigger*bigger) quit_mouse_on[0]=false;
        if((x-653)*(x-653)+(y-473)*(y-473)<=37*37) quit_mouse_on[1]=true;
        else if((x-653)*(x-653)+(y-473)*(y-473)>37*37*bigger*bigger) quit_mouse_on[1]=false;
    }
    else
    {
        if((x-pause_button_x)*(x-pause_button_x)+(y-pause_button_y)*(y-pause_button_y)<texture_r*texture_r)
            play_mouse_on[0]=true;
        else if((x-pause_button_x)*(x-pause_button_x)+(y-pause_button_y)*(y-pause_button_y)
                >texture_r*texture_r*bigger*bigger)
            play_mouse_on[0]=false;
        if((x-leave_button_x)*(x-leave_button_x)+(y-leave_button_y)*(y-leave_button_y)
           <texture_r*texture_r)
            play_mouse_on[1]=true;
        else if((x-leave_button_x)*(x-leave_button_x)+(y-leave_button_y)*(y-leave_button_y)
            >texture_r*texture_r*bigger*bigger) play_mouse_on[1]=false;
        if(x>640&&x<760&&y<595&&y>515) play_mouse_on[2]=true;
        else play_mouse_on[2]=false;
    }
}
