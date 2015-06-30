#include "Main_display.h"
#include "const.h"
#include "func.h"
#include "S_L.h"
#include <cmath>
#include <GL/glut.h>

Main_display::Main_display()
{
    polar= {1,60.0f,75.0f};
    oldpt= {-1,-1};
    l_button_down=0;
    drag_velocity=0.5;
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
    void SetCamera(GLfloat x, GLfloat y);
    glMatrixMode(GL_MODELVIEW);
    switch(c)
    {
    case'a':
        polar.alpha-=5;
        break;
    case'd':
        polar.alpha+=5;
        break;
    case'w':
        polar.fy+=5;
        break;
    case's':
        polar.fy-=5;
        break;
    case 27:
        whether_save(s);
        exit(0);
        break;
    default:
        break;
    }
    Main_display::SetCamera(0.0,0.0);
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
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        l_button_down = true;                      /*当l_button_down为true时才可改变相机位置*/
        oldpt.x=x;                                /*记录旧坐标，以便移动发生时计算鼠标移动方向*/
        oldpt.y=y;
    }
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        l_button_down = false;                    /*拿起时不能改变摄像机位置*/
    }
}

void Main_display::main_display_play(sys &s1)
{
    GLfloat light_position[]= {1.0,1.0,1.0,0.0};
    GLfloat light_color[]= {1.0,1.0,1.0};
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light_color);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLightfv(GL_LIGHT0,GL_POSITION,light_position);
    gluLookAt(camera.xeye,camera.yeye,camera.zeye,0,0.0,0.0,0.0,1.0,0.0);
    GLfloat material_color[]= {0.0,0.4,1.0};
    GLfloat material_color_reflect[]= {1.0,1.0,1.0};
    glMaterialf(GL_FRONT,GL_SHININESS,120.0);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,material_color);
    //glMaterialfv(GL_FRONT,GL_SPECULAR,material_color_reflect);

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
    glColor3f(1.0,1.0,1.0);
    glutWireCube(s1.boundary);
    glColor4f(0.0,0.0,0.0,0.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);
    glColor4f(0.7,0.7,0.7,0.9);
    glDepthMask(GL_FALSE);
    glutSolidCube(s1.boundary);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);

    glFlush();
    glutSwapBuffers();
}

void Main_display::main_display_init()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0,0.0,0.0,0.0);
}

void Main_display::main_display_reshape(int w,int h,sys& s)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-la/(double)lb*s.boundary,la/(double)lb*s.boundary,-s.boundary,s.boundary,-s.boundary,s.boundary);
    glutPostRedisplay();
}

void Main_display::main_display_idle(sys &s)
{
    system_moveonce(s);
    print_info(s);
}
