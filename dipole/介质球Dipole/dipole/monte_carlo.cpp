#include "monte_carlo.h"
#include "const.h"
#include <cmath>
#include <cstdlib>

double single_energy(int i,sys &s)//第i个粒子与其它粒子的相互作用能
{
    double Eps=0;

    for(int j=0; j<s.ttnum; j++)
    {
        if(j!=i)
        {
            double dx=s.part_ob[i].x-s.part_ob[j].x;
            double dy=s.part_ob[i].y-s.part_ob[j].y;
            double dz=s.part_ob[i].z-s.part_ob[j].z;
            double r=sqrt(dx*dx+dy*dy+dz*dz);
            Eps+=(1.-3.*dz*dz/r/r)/(r*r*r);
        }
    }
    return Eps*s.part_p*s.part_p/s.ef;
}

void system_mc_moveonce(sys &s)
{
    double dE=0;//前后能量差值
    double n=0;//一个0到1之间的随机数

    for(int i=0; i<s.ttnum; i++)
    {
        double Ep0=single_energy(i,s);
        double xi0=s.part_ob[i].x;
        double yi0=s.part_ob[i].y;
        double zi0=s.part_ob[i].z;//一些初始值
        int xn=xi0/(s.part_r*2)+0.01;
        int yn=yi0/(s.part_r*2)+0.01;
        int zn=zi0/(s.part_r*2)+0.01;//由于存储数据问题，向上+0.01保证取整不出错
        int dn=s.dis_mcmap;
        int dxn=rand()%(2*dn+1)-dn-1;
        int dyn=rand()%(2*dn+1)-dn-1;
        int dzn=rand()%(2*dn+1)-dn-1;//x，y、z方向的随机位移格子数

        if(xn+dxn<s.boundary_mcmap-1&&xn+dxn>=1&&
                yn+dyn<s.boundary_mcmap-1&&yn+dyn>=1&&
                zn+dzn<s.boundary_mcmap-1&&zn+dzn>=1&&
                s.num_map[xn+dxn][yn+dyn][zn+dzn]==0)
        {
            s.part_ob[i].x+=dxn*2*s.part_r;
            s.part_ob[i].y+=dyn*2*s.part_r;
            s.part_ob[i].z+=dzn*2*s.part_r;
            s.num_map[xn+dxn][yn+dyn][zn+dzn]=1;
            s.num_map[xn][yn][zn]=0;
            dE=single_energy(i,s)-Ep0;//能量差值
            n=double(rand())/double(RAND_MAX);//0到1的随机数
            if(exp(-b*dE)<n)//按min{Exp[-b*dE],1}的概率接受这个位形
            {
                s.part_ob[i].x=xi0;
                s.part_ob[i].y=yi0;
                s.part_ob[i].z=zi0;
                s.num_map[xn+dxn][yn+dyn][zn+dzn]=0;
                s.num_map[xn][yn][zn]=1;
            }
        }
    }
}

void system_mcT_moveonce(sys &s)
{
    double dE=0;//前后能量差值
    double n=0;//一个0到1之间的随机数

    for(int i=0; i<s.ttnum; i++)
    {
        double Ep0=single_T_energy(i,s);
        double xi0=s.part_ob[i].x;
        double yi0=s.part_ob[i].y;
        double zi0=s.part_ob[i].z;//一些初始值
        int xn=xi0/(s.part_r*2)+0.01;
        int yn=yi0/(s.part_r*2)+0.01;
        int zn=zi0/(s.part_r*2)+0.01;//由于存储数据问题，向上+0.01保证取整不出错
        int dn=s.dis_mcmap;
        int xn_=xn+rand()%(2*dn+1)-dn-1;
        int yn_=yn+rand()%(2*dn+1)-dn-1;
        int zn_=zn+rand()%(2*dn+1)-dn-1;//x，y、z方向移动后的格子位置

        if(xn_>s.boundary_mcmap-1)xn_-=s.boundary_mcmap;
        if(xn_<0)xn_+=s.boundary_mcmap;
        if(yn_>s.boundary_mcmap-1)yn_-=s.boundary_mcmap;
        if(yn_<0)yn_+=s.boundary_mcmap;
        if(zn_>s.boundary_mcmap-1)zn_-=s.boundary_mcmap;
        if(zn_<0)zn_+=s.boundary_mcmap;

        if(s.num_map[xn_][yn_][zn_]==0)
        {
            s.part_ob[i].x=xn_*2*s.part_r;
            s.part_ob[i].y=yn_*2*s.part_r;
            s.part_ob[i].z=zn_*2*s.part_r;
            s.num_map[xn_][yn_][zn_]=1;
            s.num_map[xn][yn][zn]=0;
            dE=single_T_energy(i,s)-Ep0;
            n=double(rand())/double(RAND_MAX);//0到1的随机数
            if(exp(-b*dE)<n)//按min{Exp[-b*dE],1}的概率接受这个位形
            {
                s.part_ob[i].x=xi0;
                s.part_ob[i].y=yi0;
                s.part_ob[i].z=zi0;
                s.num_map[xn_][yn_][zn_]=0;
                s.num_map[xn][yn][zn]=1;
            }
        }
    }
}

double single_T_energy(int i,sys &s)
{
    double Eps=0;
    double a=an*an/s.boundary/s.boundary;
    double sqrtAdivPi = sqrt(a / pi), sqrtA = an/s.boundary;
    double b3=pow(s.boundary,3) , twopib=2*pi/s.boundary;

    for(int j=0; j<s.ttnum; j++)
    {
        if(j!=i)
        {
            double dx_=s.part_ob[i].x-s.part_ob[j].x;
            double dy_=s.part_ob[i].y-s.part_ob[j].y;
            double dz_=s.part_ob[i].z-s.part_ob[j].z;
            double dx=dx_;
            double dy=dy_;
            double dz=dz_;
            double r=sqrt(dx*dx+dy*dy+dz*dz);
            for(int pp = 0; pp < TotTri; ++pp)
            {
                if(s.k3[pp].s > kn * kn) break;
                if(abs(s.k3[pp].kx) + abs(s.k3[pp].ky) + abs(s.k3[pp].kz) == 0) continue;
                int kx = s.k3[pp].kx, ky = s.k3[pp].ky, kz = s.k3[pp].kz;
                double k2=s.k3[pp].s;
                Eps+=4*kz*kz*pi*cos(twopib*(kx*dx+ky*dy+kz*dz))*
                     s.k3[pp].expPiPiSAnAn/(b3*k2);
            }

            for(int ix=-1; ix<=1; ix++)
            {
                for(int iy=-1; iy<=1; iy++)
                {
                    for(int iz=-1; iz<=1; iz++)
                    {
                        dx=dx_+ix*s.boundary;
                        dy=dy_+iy*s.boundary;
                        dz=dz_+iz*s.boundary;
                        r=sqrt(dx*dx+dy*dy+dz*dz);
                        if(r>s.boundary) continue;
                        double arr = a*r*r, earr = exp(-arr), erfcSqrtAmR = erfc(sqrtA*r);
                        double dz2 = dz*dz, r2 = r*r , r3=r2*r , r5=r2*r3;
                        Eps+=(erfcSqrtAmR/r3+2*sqrtAdivPi*earr/r2)
                             -dz2*(3*erfcSqrtAmR/r5+2*sqrtAdivPi*(2*a+3/r2)*earr/r2);
                    }
                }
            }
        }
    }
    return Eps*s.part_p*s.part_p/s.ef;
}
