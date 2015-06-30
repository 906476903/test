#ifndef SYS_H_INCLUDED
#define SYS_H_INCLUDED
#include "particle.h"
#include "Triple.h"

class sys
{
public:
    sys(double part_r,double ep,double ef,double E,double rou,int ttnum,double f,double dt,double boundary,int mode);
    void system_start(); //将整个系统初始化，若继续上次中断时的运算则跳过此函数
    void system_init();  //预处理
    ~sys();

    int ttnum,N,mode; //粒子总数，初始化时每一排的粒子，模式
    double part_r,ep,ef,E,rou,f,dt,boundary;
    //粒子半径，颗粒介电常数，液体介电常数，场强，质量密度，阻尼系数，时间步长，边长
    double part_p,part_m,dis; //电偶极矩，质量，初始化时粒子间距

    int dis_mcmap,boundary_mcmap; //网格上的初始距离，网格上的边长（MC算法网格化后引入的参量）

    bool whether_start; //判断是否初始化。若继续上次中断时的运算则认为已初始化，从而跳过start

    int cal_total; //已经计算的步数
    double Ek,Ep; //动能，势能

    particle *part_ob; //粒子指针
    Triple *k3; //预处理用

    int ***num_map; //MC网格指针

    double **potential_map_yz;
    double **potential_map_xz;
    double **potential_map_xy;//电势图指针（数组）

    int **number_map_yz;
    int **number_map_xz;
    int **number_map_xy;//粒子数图指针（数组）
};

#endif // SYS_H_INCLUDED
