#include "sys.h"
#include "const.h"
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <algorithm>

using namespace std;

sys::sys(double part_r_,double ep_,double ef_,double E_,double rou_,int ttnum_,double f_,double dt_,double boundary_,int mode_)
{
    srand(time(NULL));
    part_r=part_r_;
    ep=ep_;
    ef=ef_;
    E=E_;
    part_p=pow(part_r,3.)*E*ef*(ep-ef)/(ep+2*ef);
    rou=rou_;
    part_m=4*pi/3.*rou*pow(part_r,3.);
    ttnum=ttnum_;
    f=f_;
    dt=dt_;
    boundary=boundary_;
    mode=mode_;
    whether_start=0;
}

void sys::system_start()
{
    if(whether_start==0)
    {
        cal_total=0;
        Ek=0;Ep=0;
        N=int(pow(ttnum,1./3.)+2); //初始化时每一行上的粒子数
        dis=boundary/(double)N;

        if(mode==2||mode==4)
        {
            part_ob=new particle[ttnum];
            dis_mcmap=dis/part_r/2.+0.5;
            dis=dis_mcmap*2*part_r;
            boundary_mcmap=dis_mcmap*N;
            boundary=boundary_mcmap*2*part_r;
            num_map=new int**[boundary_mcmap]();
            for(int i=0; i<boundary_mcmap; i++)
            {
                num_map[i]=new int*[boundary_mcmap]();
            }
            for(int i=0; i<boundary_mcmap; i++)
            {
                for(int j=0; j<boundary_mcmap; j++)
                {
                    num_map[i][j]=new int[boundary_mcmap]();
                }
            }

            int i=0;
            for(int iz=1; iz<N&&i<ttnum; iz++)
            {
                for(int iy=1; iy<N&&i<ttnum; iy++)
                {
                    for(int ix=1; ix<N&&i<ttnum; ix++)
                    {
                        part_ob[i]=particle(i,ix*dis,iy*dis,iz*dis,0,0,0);
                        num_map[ix*dis_mcmap][iy*dis_mcmap][iz*dis_mcmap]=1;
                        i++;
                    }
                }
            }
        }

        if(mode==1||mode==3)
        {
            double theta,phi;
            double v=1.;
            double vx0=0,vy0=0,vz0=0;
            int i=0;
            part_ob=new particle[ttnum];                         //枚举时计数用
            for(int iz=1; iz<N&&i<ttnum; iz++)
            {
                for(int iy=1; iy<N&&i<ttnum; iy++)
                {
                    for(int ix=1; ix<N&&i<ttnum; ix++)
                    {
                        theta=double(rand())/double(RAND_MAX)*2*pi;
                        phi=double(rand())/double(RAND_MAX)*2*pi;
                        part_ob[i]=particle(i,ix*dis,iy*dis,iz*dis,v*cos(theta)*cos(phi),v*cos(theta)*sin(phi),v*sin(theta));
                        i++;
                        vx0+=v*cos(theta)*cos(phi);
                        vy0+=v*cos(theta)*sin(phi);
                        vz0+=v*sin(theta);
                    }
                }
            }
            for(int i=0;i<ttnum;i++)
            {
                part_ob[i].vx-=vx0/(double)ttnum;
                part_ob[i].vy-=vy0/(double)ttnum;
                part_ob[i].vz-=vz0/(double)ttnum;
            }
        }
        whether_start=1;
    }
}

void sys::system_init()
{
    if(mode==3||mode==4)
    {
        k3=new Triple[TotTri];
        int i = 0;
        for(int kx = -kn; kx <= kn; kx++)
            for(int ky = -kn; ky <= kn; ky++)
                for(int kz = -kn; kz <= kn; kz++)
                    k3[i++] = Triple(kx, ky, kz);
        sort(k3, k3 + i);
    }

    potential_map_yz=new double*[map_n]();
    for(int i=0; i<map_n; i++)
    {
        potential_map_yz[i]=new double[map_n]();
    }

    potential_map_xz=new double*[map_n]();
    for(int i=0; i<map_n; i++)
    {
        potential_map_xz[i]=new double[map_n]();
    }

    potential_map_xy=new double*[map_n]();
    for(int i=0; i<map_n; i++)
    {
        potential_map_xy[i]=new double[map_n]();
    }  //电势图初始化，初值为0

    number_map_yz=new int*[map_n]();
    for(int i=0; i<map_n; i++)
    {
        number_map_yz[i]=new int[map_n]();
    }

    number_map_xz=new int*[map_n]();
    for(int i=0; i<map_n; i++)
    {
        number_map_xz[i]=new int[map_n]();
    }

    number_map_xy=new int*[map_n]();
    for(int i=0; i<map_n; i++)
    {
        number_map_xy[i]=new int[map_n]();
    }  //粒子分布图初始化，初值为0
}

sys::~sys()
{
    if(whether_start)
    {
        delete []part_ob;
        delete []k3;

        for(int i=0; i<map_n; i++)
        {
            delete[] potential_map_xy[i];
            delete[] potential_map_yz[i];
            delete[] potential_map_xz[i];
            delete[] number_map_xy[i];
            delete[] number_map_yz[i];
            delete[] number_map_xz[i];
        }

        delete[] potential_map_xy;
        delete[] potential_map_yz;
        delete[] potential_map_xz;
        delete[] number_map_xy;
        delete[] number_map_yz;
        delete[] number_map_xz;

        if(mode==2||mode==4)
        {
            for(int i=0; i<boundary_mcmap; i++)
            {
                for(int j=0; j<boundary_mcmap; j++)
                {
                    delete[] num_map[i][j];
                }
            }

            for(int i=0; i<boundary_mcmap; i++)
            {
                delete[] num_map[i];
            }

            delete[] num_map;
        }
    }
}
