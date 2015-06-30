#include "global_variables.h"
#include "sys.h"
bool l_button_down=false;//用来表示鼠标左键当前的状态
cmode MODE=SMENU;//用来表示当前的场景
Point oldpt={-1,-1};//用来标记鼠标的老位置，以像素为单位
char* settingnum=new char[30] ;//用在setting——display中，用来对数字贴图
bool if_change=false;//标记场景是否改变，在glutmousefunc中决定是否重新初始化

