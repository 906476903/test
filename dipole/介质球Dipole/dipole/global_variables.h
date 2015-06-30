#ifndef GLOBAL_VARIABLES_H_INCLUDED
#define GLOBAL_VARIABLES_H_INCLUDED
#include "main_display.h"
#include "Menu_display.h"
#include "sys.h"
#include "const.h"
#include "setting.h"

extern bool l_button_down;
enum cmode{SMENU,SSHOW,SSETTING,SINTRO};//控制流程
extern cmode MODE;

 struct Point                   /*用于记录鼠标位置*/
    {
        int x;
        int y;
    };
extern Point oldpt;

extern char* settingnum;

extern bool if_change;
#endif // GLOBAL_VARIABLES_H_INCLUDED
