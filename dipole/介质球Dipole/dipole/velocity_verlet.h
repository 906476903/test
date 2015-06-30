#ifndef VELOCITY_VERLET_H_INCLUDED
#define VELOCITY_VERLET_H_INCLUDED
#include "sys.h"

void system_acal(sys &s);
void system_acal_judge(sys &s);
void system_vv_moveonce(sys &s);

void system_T_acal(sys &s);
void system_T_acal_judge(sys &s);
void system_vvT_moveonce(sys &s);

#endif // SYSTEM_MOVE_H_INCLUDED
