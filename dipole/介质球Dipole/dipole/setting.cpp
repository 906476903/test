#include "setting.h"
#include "texture.h"
#include "global_variables.h"
#include "sys.h"
#include "const.h"
#include <cmath>
#include <stdio.h>
#include <iostream>
#include "Menu_display.h"
#include <stdlib.h>
#include <GL/glut.h>
using namespace std;

setting::setting()
{
    mode=1;
    para[0]=125.0;
    para[1]=0.5;
    para[2]=2.0;
    para[3]=12.5;
    para[4]=0.5;
    setting_mouse_on[0]=setting_mouse_on[1]=false;
}

void setting::setting_startbutton(float x,float y,float t)
{
   glPushMatrix();
   if(setting_mouse_on[1])  glBindTexture(GL_TEXTURE_2D, theTexture[47]);
   else                     glBindTexture(GL_TEXTURE_2D, theTexture[28]);
        glBegin(GL_POLYGON);
          glTexCoord2d(76./512.,23./512.); glVertex3f(x+(76.-36.)/427.*t,y,-1.0f);
          glTexCoord2d(36./512.,83./512.); glVertex3f(x,y+screen_ratio*t*(83.-23.)/427.,-1.0f);
          glTexCoord2d(36./512.,441./512.); glVertex3f(x,y+(441.-23.)/427.*t*screen_ratio,-1.0f);
          glTexCoord2d(102./512.,493./512.); glVertex3f(x+(102.-36.)/427.*t,y+(493.-23.)/427.*t*screen_ratio,-1.0f);
          glTexCoord2d(397./512.,493./512.); glVertex3f(x+(397.-36.)/427.*t,y+(493.-23.)/427.*t*screen_ratio,-1.0f);
          glTexCoord2d(463./512.,441./512.);   glVertex3f(x+(463.-36.)/427.*t,y+(441.-23.)/427.*t*screen_ratio,-1.0f);
          glTexCoord2d(463./512.,83./512.);    glVertex3f(x+(463.-36.)/427.*t,y+(83.-23.)/427.*t*screen_ratio,-1.0f);
          glTexCoord2d(423./512.,23./512.);    glVertex3f(x+(423.-36.)/427.*t,y,-1.0f);
        glEnd();
        glPopMatrix();
}

void setting::setting_backbutton(float x,float y,float t)//返回按钮
{
       glPushMatrix();
           if(setting_mouse_on[0])  glBindTexture(GL_TEXTURE_2D, theTexture[48]);
           else                     glBindTexture(GL_TEXTURE_2D, theTexture[20]);
        glBegin(GL_POLYGON);
          glTexCoord2d(76./512.,23./512.); glVertex3f(x+(76.-36.)/427.*t,y,-1.0f);
          glTexCoord2d(36./512.,83./512.); glVertex3f(x,y+screen_ratio*t*(83.-23.)/427.,-1.0f);
          glTexCoord2d(36./512.,441./512.); glVertex3f(x,y+(441.-23.)/427.*t*screen_ratio,-1.0f);
          glTexCoord2d(102./512.,493./512.); glVertex3f(x+(102.-36.)/427.*t,y+(493.-23.)/427.*t*screen_ratio,-1.0f);
          glTexCoord2d(397./512.,493./512.); glVertex3f(x+(397.-36.)/427.*t,y+(493.-23.)/427.*t*screen_ratio,-1.0f);
          glTexCoord2d(463./512.,441./512.);   glVertex3f(x+(463.-36.)/427.*t,y+(441.-23.)/427.*t*screen_ratio,-1.0f);
          glTexCoord2d(463./512.,83./512.);    glVertex3f(x+(463.-36.)/427.*t,y+(83.-23.)/427.*t*screen_ratio,-1.0f);
          glTexCoord2d(423./512.,23./512.);    glVertex3f(x+(423.-36.)/427.*t,y,-1.0f);
        glEnd();
        glPopMatrix();
}


void setting::setting_init()
{
    glClearColor(0.0,0.0,0.0,0.0);
   glShadeModel(GL_SMOOTH);

   glColor3f(1.0,1.0,1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-10.0,10.0,-10.0,10.0,0.0,10.0);
}

void setting::setting_button_position()
{
    for(int i=0;i<6;i++)
    {
       stibutton[i].x=((para[i]-minimum[i])/leng[i]*8.25-5.0);
       stibutton[i].y=4.5-2.0*i;
    }
}

void setting::setting_reshape(int w,int h)
{
     glViewport(0,0,(GLsizei)w,(GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10.0,10.0,-10.0,10.0,-10.0,10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0,0.0,8.0,0.0,0.0,0.0,0.0,1.0,0.0);//参数可以调的
    glutPostRedisplay();
}

void setting::setting_display()
{
   glViewport(0,0,width,height);
   glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
   setting_backbutton(-10.0,-10.0,2.0);
   setting_startbutton(10.0-2.0,-10.0,2.0);
   //按钮
     for(int i=12;i<=17;i++){
   if(i<=16||mode==1||mode==3)
   {
       glBindTexture(GL_TEXTURE_2D, theTexture[i]);
        glBegin(GL_QUADS);
          glTexCoord2f(0.0f, 0.0f); glVertex3f(-7.0,5.85-(i-12)*2.0,0.0f);
          glTexCoord2f(0.0f, 1.0f); glVertex3f(-7.0,5.85+2.0*screen_ratio-(i-12)*2.0, 0.0f);
          glTexCoord2f(1.0f, 1.0f); glVertex3f(-7.0+2.,5.85+2.0*screen_ratio-(i-12)*2.0, 0.0f);
          glTexCoord2f(1.0f, 0.0f); glVertex3f(-7.0+2.,5.85-(i-12)*2.0, 0.0f);
        glEnd();
   }
     }
     for(int i=0;i<4;i++){
     glBindTexture(GL_TEXTURE_2D, theTexture[B[i]]);
        glBegin(GL_QUADS);
          glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0+i,6.5, 0.0f);
          glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0+i,6.5+1.0*screen_ratio, 0.0f);
          glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0+i+1.,6.5+1.0*screen_ratio, 0.0f);
          glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0+i+1.,6.5, 0.0f);
        glEnd();
     }
    for(int i=0;i<=4;i++){
    if(i<=3||mode==1||mode==3)
    {
     glBindTexture(GL_TEXTURE_2D, theTexture[21]);
        glBegin(GL_QUADS);
          glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0,stibutton[i].y, -0.5f);
          glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0,stibutton[i].y+1.0*screen_ratio, -0.5f);
          glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0+10.,stibutton[i].y+1.0*screen_ratio, -0.5f);
          glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0+10.,stibutton[i].y,-0.5f);
        glEnd();
     glBindTexture(GL_TEXTURE_2D, theTexture[22]);
        glBegin(GL_QUADS);
          glTexCoord2f(0.0f, 0.0f); glVertex3f(stibutton[i].x,stibutton[i].y, 0.0f);
          glTexCoord2f(0.0f, 1.0f); glVertex3f(stibutton[i].x,stibutton[i].y+1.0*screen_ratio, 0.0f);
           glTexCoord2f(1.0f, 1.0f); glVertex3f(stibutton[i].x+1.,stibutton[i].y+1.0*screen_ratio, 0.0f);
          glTexCoord2f(1.0f, 0.0f); glVertex3f(stibutton[i].x+1.,stibutton[i].y, 0.0f);
        glEnd();
     para[i]=(stibutton[i].x+5.)/8.25*leng[i]+minimum[i];
     if(i==0)para[i]=ceil(para[i]);
        if(i!=0) sprintf(settingnum,"%f",para[i]);
        else sprintf(settingnum,"%3.0f",para[i]);
        glprint(5.0,4.8-i*2.0,settingnum);
    }

    }
   glBindTexture(GL_TEXTURE_2D, theTexture[23]);//标题
   glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-4.0,8., 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-4.0,8.+1.4*screen_ratio, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-4.0+8.,8.+1.4*screen_ratio, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-4.0+8.,8., 0.0f);
   glEnd();
   glFlush();
   glutSwapBuffers();
}

void setting::setting_mouse(int button,int state,int x,int y,sys&s)
{
    extern Menu_display menuplay;
    extern bool if_change;
     float X=(float)x/width*20.0-10.0;
    float Y=10-(float)y/height*20.0;
    if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN
       &&setting_mouse_on[0])
    {
        MODE=SMENU;
        if_change=true;
        glutPostRedisplay();
    }
    if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN
       &&setting_mouse_on[1])
    {
        MODE=SSHOW;
        setting_para_to_sys(s);
        s.system_start();
        if_change=true;
        glutPostRedisplay();
    }
    if(button==GLUT_LEFT_BUTTON&&state==GLUT_UP
       &&X>-5.0&&X<(-5.0+1.0)&&Y>6.5&&Y<(6.5+1.0*screen_ratio))
    {
        B[0]=5,B[1]=6,B[2]=8,B[3]=10;
        mode=1;
        glutPostRedisplay();
    }
    if(button==GLUT_LEFT_BUTTON&&state==GLUT_UP
       &&X>-4.0&&X<(-4.0+1.0)&&Y>6.5&&Y<(6.5+1.0*screen_ratio))
    {
        B[0]=4,B[1]=7,B[2]=8,B[3]=10;
        mode=2;
        glutPostRedisplay();
    }
    if(button==GLUT_LEFT_BUTTON&&state==GLUT_UP
       &&X>-3.0&&X<(-3.0+1.0)&&Y>6.5&&Y<(6.5+1.0*screen_ratio))
    {
        B[0]=4,B[1]=6,B[2]=9,B[3]=10;
        mode=3;
        glutPostRedisplay();
    }
    if(button==GLUT_LEFT_BUTTON&&state==GLUT_UP
       &&X>-2.0&&X<(-2.0+1.0)&&Y>6.5&&Y<(6.5+1.0*screen_ratio))
    {
        B[0]=4,B[1]=6,B[2]=8,B[3]=11;
        mode=4;
        glutPostRedisplay();
    }
    for(int i=0;i<=4;i++){
        if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&
       X>stibutton[i].x&&X<(stibutton[i].x+1.)&&Y>stibutton[i].y&&Y<(stibutton[i].y+1.*screen_ratio))
    {
        l_button_down=true;
        setting_button_down[i]=true;
        oldpt={x,y};
    }
    }
  if(button==GLUT_LEFT_BUTTON&&state==GLUT_UP)
   {
      for(int k=0;k<=5;k++)
      {
        setting_button_down[k]=false;
        l_button_down=false;
      }
    }
}


void setting::setting_mousemove(int x,int y)
{
     float X=(float)x/width*20.0-10.0;
    float Y=10-(float)y/height*20.0;
    if(l_button_down)//左键按下时才能拖动
    {
        for(int i=0;i<=4;i++)
    {
        if(setting_button_down[i])//左键按下时才能拖动
        {
            stibutton[i].x+=X-(float)oldpt.x/width*20.0+10.0;
            oldpt.x=x;
            oldpt.y=y;
            para[i]=((stibutton[i].x+5.0)/8.25*leng[i]+minimum[i]);
        }
        if(stibutton[i].x>3.25) para[i]=leng[i]+minimum[i];
        if(stibutton[i].x<-5.0) para[i]=minimum[i];
        }
    }
    setting_button_position();
    glutPostRedisplay();
}

void setting::setting_idle()
{
    setting_display();
}

void setting::setting_para_to_sys(sys &s)
{
    s.mode=mode;
    s.ttnum=para[0];
    s.part_r=para[1];
    s.E=para[2];
    s.boundary=para[3];
    s.f=para[4];

    s.part_p=pow(part_r,3.)*s.E*ef*(ep-ef)/(ep+2*ef);
    s.part_m=4*pi/3.*s.rou*pow(s.part_r,3.);
    s.whether_start=0;
    s.dt=0.01*s.part_r/s.E;
}

void setting::setting_key(char c,int x,int y,sys &s)
{
    extern bool if_change;
    switch(c)
    {
    case 27:
        MODE=SMENU;
        if_change=true;
        break;
    default:break;
    }
}

void setting::setting_passivemotion(int x,int y)
{
    float X=(float)x/width*20.0-10.0;
    float Y=10-(float)y/height*20.0;
    if(X>-10.0&&X<(-10.0+2.5/screen_ratio)&&Y>-10.0&&Y<(-10.0+2.5*470./427.))
        setting_mouse_on[0]=true;
    else setting_mouse_on[0]=false;
    if(X>10.0-2.5/screen_ratio&&X<10.0&&Y>-10.0&&Y<-10.0+2.5*470./427.)
        setting_mouse_on[1]=true;
    else setting_mouse_on[1]=false;
}

