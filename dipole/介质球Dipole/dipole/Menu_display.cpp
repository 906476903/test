#include "Menu_display.h"
#include "texture.h"
#include "global_variables.h"
#include <ctime>
#include "const.h"
#include <GL\glut.h>
#include <cmath>
#include <iostream>
#include "S_L.h"
using namespace std;

Menu_display::Menu_display()
{
    menu_dt=0.1;
    button_r=1.7;
    screenboundary=10.0;
    logoZ=2.0;
    menu_pause=false;
    texture_total=48;
    for(int i=0;i<4;i++) menu_mouse_on[i]=false;
}

void Menu_display::menu_display_usefog()
{
  glEnable(GL_DEPTH_TEST);
    glEnable(GL_FOG);
    {
        static GLint fogMode=GL_LINEAR;
        GLfloat fogColor[4]={0.0,0.,0.,0.5};
        glFogi(GL_FOG_MODE,fogMode);
        glFogfv(GL_FOG_COLOR,fogColor);
        glFogf(GL_FOG_DENSITY,0.35);
        glHint(GL_FOG_HINT,GL_DONT_CARE);
        glFogf(GL_FOG_START,0.0);
        glFogf(GL_FOG_END,5.0);
    }
}

void Menu_display::menu_display_init()
{
    glColor3f(1.0,1.0,1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0,0.0,0.0,0.0,0.0,-1.0,0.0,1.0,0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10.0,10.0,-10.0,10.0,-1.0,5.0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    menu_pause=false;
    for(int i=0;i<4;i++) menu_mouse_on[i]=false;
}

void Menu_display::menu_display_init_button()
{
    srand(time(NULL));
    for(int i=0;i<4;i++){
    movbutton[i].x=5.0*(i-1.5);
    movbutton[i].y=-3.;
    movbutton[i].vx=double(rand())/double(RAND_MAX)/2.;
    movbutton[i].vy=double(rand())/double(RAND_MAX)/2.;
    }
}

void Menu_display::menubuttonmove()
{
    int i,j;
    for(i=0;i<4;i++){
        movbutton[i].x+=movbutton[i].vx*menu_dt;
        movbutton[i].y+=movbutton[i].vy*menu_dt;
    }
    for(i=0;i<4;i++){
        for(j=i+1;j<4;j++){
            double dx=movbutton[i].x-movbutton[j].x;
            double dy=movbutton[i].y-movbutton[j].y;
            double r=sqrt(dx*dx+dy*dy);
            if(r<2*button_r){
                double dvn=(movbutton[i].vx*dx+movbutton[i].vy*dy-
                            movbutton[j].vx*dx-movbutton[j].vy*dy)/r;
                movbutton[i].vx-=dvn*dx/r;
                movbutton[i].vy-=dvn*dy/r;
                movbutton[j].vx+=dvn*dx/r;
                movbutton[j].vy+=dvn*dy/r;
            }
        }
    }
    for(i=0;i<4;i++){
        if(movbutton[i].x-button_r<-screenboundary+0.5){movbutton[i].vx=-movbutton[i].vx;}
        if(movbutton[i].x+button_r>screenboundary-0.5){movbutton[i].vx=-movbutton[i].vx;}
        if(movbutton[i].y-button_r<-screenboundary){movbutton[i].vy=-movbutton[i].vy;}
        if(movbutton[i].y+button_r>screenboundary-8.0){movbutton[i].vy=-movbutton[i].vy;}
    }

}


void Menu_display::menubuttonshow()
{
    int M=30;
    for(int i=0;i<4;i++){
      if(menu_mouse_on[i])
      {
        glColor3f(1.0,1.0,1.0);
        glBindTexture(GL_TEXTURE_2D, theTexture[i]);
        glBegin(GL_POLYGON);
        for(int n=0;n<M;n++)
        {
         glTexCoord2f(0.5+0.5*cos(2*n*pi/M), 0.5+0.5*sin(2*n*pi/M));
         glVertex3f(movbutton[i].x+cos(2*n*pi/M)*button_r,movbutton[i].y+sin(2*n*pi/M)*button_r, -0.9f);
        }
        glEnd();
      }
      else
      {
          if(menu_pause)
          {
            glColor3f(0.1,0.1,0.1);
            glBindTexture(GL_TEXTURE_2D, theTexture[i+41]);
            glBegin(GL_POLYGON);
            for(int n=0;n<M;n++)
            {
                glTexCoord2f(0.5+0.5*cos(2*n*pi/M), 0.5+0.5*sin(2*n*pi/M));
                glVertex3f(movbutton[i].x+cos(2*n*pi/M)*button_r,movbutton[i].y+sin(2*n*pi/M)*button_r, -0.9f);
            }
            glEnd();
          }
          else
          {
              glColor3f(1.0,1.0,1.0);
            glBindTexture(GL_TEXTURE_2D, theTexture[i]);
            glBegin(GL_POLYGON);
            for(int n=0;n<M;n++)
            {
                glTexCoord2f(0.5+0.5*cos(2*n*pi/M), 0.5+0.5*sin(2*n*pi/M));
                glVertex3f(movbutton[i].x+cos(2*n*pi/M)*button_r,movbutton[i].y+sin(2*n*pi/M)*button_r, -0.9f);
            }
            glEnd();
          }
      }
    }

}

void Menu_display::menu_display_play()
{

   glShadeModel(GL_SMOOTH);
   if(logoZ<13){
   glClearColor(0.0,0.0,0.0,0.0);
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
         if(texture_total>=0) LoadGLTextures(texture_total);
      texture_total--;
   menu_display_usefog();
   glBindTexture(GL_TEXTURE_2D, theTexture[29]);
   glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0/screen_ratio,-10,logoZ);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0/screen_ratio,10,logoZ);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0/screen_ratio,10,logoZ);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0/screen_ratio,-10,logoZ);
   glEnd();
   }
   else{
   glClearColor(1.0,1.0,1.0,1.0);
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


   glViewport(100,0,600,600);
   glDisable(GL_FOG);
   menubuttonshow();

   glViewport(0,0,width,height);
   if(menu_pause) glColor3f(0.5,0.5,0.5);
   glBindTexture(GL_TEXTURE_2D, theTexture[24]);
   glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f,-10.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0f,10.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f,10.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f,-10.0f, -1.0f);
   glEnd();
   glColor3f(1.0,1.0,1.0);
   }
   glutSwapBuffers();
}

void Menu_display::menu_display_reshape(int w,int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10.0,10.0,-10.0,10.0,-10.0,10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0,0.0,8.0,0.0,0.0,0.0,0.0,1.0,0.0);//参数可以调的
    glutPostRedisplay();
}

void Menu_display::menu_display_idle()
{
  if(logoZ<13)
    {
      logoZ+=0.04;
      menu_display_play();
    }
    else
    {
      if(!menu_pause) menubuttonmove();
      menu_display_play();
    }
}

void Menu_display::menu_display_mouse(int button,int state,int x,int y,sys&s)
{
    extern int load_present(sys&s);
    extern bool if_change;
    extern setting settingplay;

    float X=(float)(x-100)/600.0*20.0-10.0;
    float Y=10-(float)y/height*20.0;
if(logoZ>13)
{
        if(button==GLUT_LEFT_BUTTON&&state==GLUT_UP
       &&((X-movbutton[0].x)*(X-movbutton[0].x)+(Y-movbutton[0].y)*(Y-movbutton[0].y))<button_r*button_r)
    {
        if(load_present(s)) ;
        else
        {
            settingplay.setting_para_to_sys(s);
            s.system_start();
        }
        MODE=SSHOW;
        glBindTexture(GL_TEXTURE_2D, theTexture[24]);
         if_change=true;
        glutPostRedisplay();
    }
     if(button==GLUT_LEFT_BUTTON&&state==GLUT_UP
      &&((X-movbutton[1].x)*(X-movbutton[1].x)+(Y-movbutton[1].y)*(Y-movbutton[1].y))<button_r*button_r)
    {
        MODE=SSETTING;
        if_change=true;
        glutPostRedisplay();
    }
     if(button==GLUT_LEFT_BUTTON&&state==GLUT_UP
       &&((X-movbutton[2].x)*(X-movbutton[2].x)+(Y-movbutton[2].y)*(Y-movbutton[2].y))<button_r*button_r)
    {
        MODE=SINTRO;
        glutPostRedisplay();
        if_change=true;
    }
     if(button==GLUT_LEFT_BUTTON&&state==GLUT_UP
       &&((X-movbutton[3].x)*(X-movbutton[3].x)+(Y-movbutton[3].y)*(Y-movbutton[3].y))<button_r*button_r)
    {
        exit(0);
    }
}
}

void Menu_display::menu_display_key(char c,int x,int y)
{
    switch(c)
    {
        case 27:exit(0);
        default:break;
    }
}

void Menu_display::menu_passive_motion(int x,int y)
{
  if(logoZ>13)
  {
    float X=(float)(x-100)/600.0*20.0-10.0;
    float Y=10-(float)y/height*20.0;
    if(menu_pause)
    {
        for(int i=0;i<4;i++)
        {
            if(((X-movbutton[i].x)*(X-movbutton[i].x)+(Y-movbutton[i].y)*(Y-movbutton[i].y))<button_r*button_r)
            {
                menu_mouse_on[i]=true;
                menu_pause=false;//只是为了说明此时应该暂停，在后面就改过来了
            }
            else menu_mouse_on[i]=false;
        }
        if(menu_pause) menu_pause=false;
        else menu_pause=true;
    }
    else
    {
        for(int i=0;i<4;i++)
        {
            if(((X-movbutton[i].x)*(X-movbutton[i].x)+(Y-movbutton[i].y)*(Y-movbutton[i].y))<button_r*button_r)
            {
                menu_mouse_on[i]=true;
                menu_pause=true;//只是为了说明此时应该暂停，在后面就改过来了
            }
            else menu_mouse_on[i]=false;
        }
    }
  }
}
