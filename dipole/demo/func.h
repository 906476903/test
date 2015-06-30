#ifndef FUNC_H_INCLUDED
#define FUNC_H_INCLUDED
#include "sys.h"
#include "main_display.h"

void system_moveonce(sys &s);

double potential_xyz(double x,double y,double z,sys &s);
void system_potential_yz(double map_x,sys &s);
void system_potential_xz(double map_y,sys &s);
void system_potential_xy(double map_z,sys &s);//电势分布图（给定某一个面）

void system_number_yz(sys &s);
void system_number_xz(sys &s);
void system_number_xy(sys &s);//粒子数分布图（投影至某一个面）

void system_energy(sys &s);//总能量

void print_info(sys &s);

void menu_temp(sys &s,Main_display &m);

#endif // FUNC_H_INCLUDED
