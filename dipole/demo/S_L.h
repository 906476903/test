#ifndef S_L_H_INCLUDED
#define S_L_H_INCLUDED
#include "sys.h"
#include "main_display.h"

void whether_save(sys &s);
void whether_load(sys &s,Main_display &m);
void save_present(sys &s);//保存这一时刻
void load_present(sys &s,Main_display &m);//载入保存的时刻继续计算

#endif // S_L_H_INCLUDED
