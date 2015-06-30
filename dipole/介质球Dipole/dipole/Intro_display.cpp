#include "Intro_display.h"
#include "texture.h"
#include "global_variables.h"
#include <GL/glut.h>
#include <iostream>

Intro_Display::Intro_Display()
{
   introY=1.0;
   for(int i=0;i<3;i++) intro_mouse_on[i]=false;
}

void Intro_Display::Intro_Display_backbutton(float x,float y,float t)//返回按钮
{
        glPushMatrix();
        if(intro_mouse_on[2]) glBindTexture(GL_TEXTURE_2D, theTexture[48]);
        else  glBindTexture(GL_TEXTURE_2D, theTexture[20]);
        glBegin(GL_QUADS);
          glTexCoord2f(0.0f, 0.0f); glVertex3f(x,y,-1.0f);
          glTexCoord2f(0.0f, 1.0f); glVertex3f(x,y+t,-1.0f);
          glTexCoord2f(1.0f, 1.0f); glVertex3f(x+t/screen_ratio,y+t,-1.0f);
          glTexCoord2f(1.0f, 0.0f); glVertex3f(x+t/screen_ratio,y,-1.0f);
        glEnd();
        glPopMatrix();
}

void Intro_Display::Intro_Display_play()
{
    glViewport(0,0,width,height);
    glShadeModel(GL_SMOOTH);
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glPushMatrix();
   Intro_Display_backbutton(9.1,-10.0,1.2);

   glBindTexture(GL_TEXTURE_2D, theTexture[25]);//reference图片
    glBegin(GL_QUADS);
         glTexCoord2f(0.0,introY-0.3); glVertex3f(-10.0,-10,-1.0);
         glTexCoord2f(0.0,introY); glVertex3f(-10.0,10.0,-1.0);
         glTexCoord2f(1.0,introY); glVertex3f(9.1,10.0,-1.0);
         glTexCoord2f(1.0,introY-0.3); glVertex3f(9.1,-10,-1.0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, theTexture[21]);//控制条
        glBegin(GL_QUADS);
          glTexCoord2f(0.0f, 0.0f); glVertex3f(10,-7.6,-1.0);
          glTexCoord2f(0.0f, 1.0f); glVertex3f(9.1,-7.6, -1.0);
          glTexCoord2f(1.0f, 1.0f); glVertex3f(9.1,10.0,-1.0f);
          glTexCoord2f(1.0f, 0.0f); glVertex3f(10,10.0,-1.0f);
        glEnd();

    if(intro_mouse_on[0]) glBindTexture(GL_TEXTURE_2D, theTexture[39]);//up
    else glBindTexture(GL_TEXTURE_2D, theTexture[26]);
        glBegin(GL_QUADS);
          glTexCoord2f(0.0f, 0.0f); glVertex3f(10,8.8,-0.5);
          glTexCoord2f(0.0f, 1.0f); glVertex3f(10,10.0, -0.5f);
          glTexCoord2f(1.0f, 1.0f); glVertex3f(9.1,10.0, -0.5f);
          glTexCoord2f(1.0f, 0.0f); glVertex3f(9.1,8.8,-0.5f);
        glEnd();

    if(intro_mouse_on[1]) glBindTexture(GL_TEXTURE_2D, theTexture[40]);//down
    else glBindTexture(GL_TEXTURE_2D, theTexture[27]);
        glBegin(GL_QUADS);
          glTexCoord2f(0.0f, 0.0f); glVertex3f(10,-8.8,-1.0);
          glTexCoord2f(0.0f, 1.0f); glVertex3f(10.0,-7.6, -1.0f);
          glTexCoord2f(1.0f, 1.0f); glVertex3f(9.1,-7.6, -1.0f);
          glTexCoord2f(1.0f, 0.0f); glVertex3f(9.1,-8.8,-1.0f);
        glEnd();

    glBindTexture(GL_TEXTURE_2D, theTexture[22]);//control bar
        glBegin(GL_QUADS);
          glTexCoord2f(0.0f, 0.0f); glVertex3f(10,(22*introY-14.2), -0.5f);
          glTexCoord2f(0.0f, 1.0f); glVertex3f(9.1,(22*introY-14.2), -0.5f);
          glTexCoord2f(1.0f, 1.0f); glVertex3f(9.1,(22*introY-13.2), -0.5f);
          glTexCoord2f(1.0f, 0.0f); glVertex3f(10,(22*introY-13.2), -0.5f);
        glEnd();
   glPopMatrix();
   glFlush();
   glutSwapBuffers();
}

void Intro_Display::Intro_Display_reshape(int w,int h)
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

void Intro_Display::Intro_Display_mouse(int button,int state,int x,int y)
{
    extern bool if_change;
    float X=(float)x/width*20.0-10.0;
    float Y=10-(float)y/height*20.0;
    if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN
       &&intro_mouse_on[2])
    {
        MODE=SMENU;
        if_change=true;
        glutPostRedisplay();
    }
    if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN
       &&intro_mouse_on[0])
    {
        introY+=0.02;
    }
     if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN
       &&intro_mouse_on[1])
    {
        introY-=0.02;
    }
    if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN
    &&X>8.8&&X<10&&Y>(22*introY-14.2)&&Y<(22*introY-13.2))
    {
        l_button_down=true;
        oldpt.y=y;
    }
    if(button==GLUT_LEFT_BUTTON&&state==GLUT_UP)
    {
        l_button_down=false;
    }
    if(introY>1.0)introY=1.0;
    if(introY<0.3)introY=0.3;
}

void Intro_Display::Intro_Display_mousemove(int x,int y)
{
     float X=(float)x/width*20.0-10.0;
    float Y=10-(float)y/height*20.0;
    if(l_button_down)
    {
        introY+=(Y-(10-(float)oldpt.y/height*20.0))/22.0;
        oldpt.y=y;
        glutPostRedisplay();
    }
    if(introY>1.0)introY=1.0;
    if(introY<0.3)introY=0.3;
    glutPostRedisplay();
}

void Intro_Display::Intro_Display_idle()
{
    Intro_Display_play();
}

void Intro_Display::Intro_Display_init()
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10.0,10.0,-10.0,10.0,-5.0,5.0);
}

void Intro_Display::Intro_Display_key(char c,int x,int y)
{
    extern bool if_change;
    switch (c)
    {
    case 27:
        MODE=SMENU;
        if_change=true;
        break;
    case 'w':
        introY+=0.03;
        break;
    case 's':
        introY-=0.03;
        break;
    default:break;
    }
    if(introY>1.0)introY=1.0;
    if(introY<0.3)introY=0.3;
}

void Intro_Display::Intro_Display_passivemotion(int x,int y)
{
    float X=(float)x/width*20.0-10.0;
    float Y=10-(float)y/height*20.0;
    if(X>8.8&&X<(8.8+1.2)&&Y>-10.0&&Y<(-10.0+1.2)) intro_mouse_on[2]=true;
    else intro_mouse_on[2]=false;
    if(X>8.8&&X<10&&Y>8.8&&Y<10) intro_mouse_on[0]=true;
    else intro_mouse_on[0]=false;
    if(X>8.8&&X<10&&Y>-8.8&&Y<-7.6) intro_mouse_on[1]=true;
    else intro_mouse_on[1]=false;
}
