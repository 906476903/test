#include "func.h"
#include "const.h"
#include "velocity_verlet.h"
#include "monte_carlo.h"
#include "S_L.h"
#include "Triple.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>

using namespace std;

void system_moveonce(sys &s)
{
    if(s.mode==1) system_vv_moveonce(s);
    if(s.mode==2) system_mc_moveonce(s);
    if(s.mode==3) system_vvT_moveonce(s);
    if(s.mode==4) system_mcT_moveonce(s);
}

void system_energy(sys &s)
{
    s.Ek=0;
    s.Ep=0;
    if(s.mode==1)
    {
        for(int i=0; i<s.ttnum; i++)
        {
            s.Ek+=0.5*(pow(s.part_ob[i].vx,2)+pow(s.part_ob[i].vy,2)+pow(s.part_ob[i].vz,2));
        }
        s.Ek=s.Ek*s.part_m;

        for(int i=0; i<s.ttnum; i++)
        {
            for(int j=i+1; j<s.ttnum; j++)
            {
                double dx=s.part_ob[i].x-s.part_ob[j].x;
                double dy=s.part_ob[i].y-s.part_ob[j].y;
                double dz=s.part_ob[i].z-s.part_ob[j].z;
                double r=sqrt(dx*dx+dy*dy+dz*dz);
                s.Ep+=(1.-3.*pow(dz/r,2))/pow(r,3);
            }
        }
        s.Ep=s.Ep*pow(s.part_p,2)/s.ef;
    }

    if(s.mode==2)
    {
        for(int i=0; i<s.ttnum; i++)
        {
            for(int j=i+1; j<s.ttnum; j++)
            {
                double dx=s.part_ob[i].x-s.part_ob[j].x;
                double dy=s.part_ob[i].y-s.part_ob[j].y;
                double dz=s.part_ob[i].z-s.part_ob[j].z;
                double r=sqrt(dx*dx+dy*dy+dz*dz);
                s.Ep+=(1.-3.*pow(dz/r,2))/pow(r,3);
            }
        }
        s.Ep=s.Ep*pow(s.part_p,2)/s.ef;
    }

    if(s.mode==3)
    {
        double a=an*an/s.boundary/s.boundary;
        double sqrtAdivPi = sqrt(a / pi), sqrtA = an/s.boundary;
        double b3=pow(s.boundary,3) , twopib=2*pi/s.boundary;

        for(int i=0; i<s.ttnum; i++)
        {
            s.Ek+=0.5*(pow(s.part_ob[i].vx,2)+pow(s.part_ob[i].vy,2)+pow(s.part_ob[i].vz,2));
        }
        s.Ek=s.Ek*s.part_m;

        for(int i=0; i<s.ttnum; i++)
        {
            for(int j=i+1; j<s.ttnum; j++)
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
                    s.Ep+=4*kz*kz*pi*cos(twopib*(kx*dx+ky*dy+kz*dz))*
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
                            s.Ep+=(erfcSqrtAmR/r3+2*sqrtAdivPi*earr/r2)
                                  -dz2*(3*erfcSqrtAmR/r5+2*sqrtAdivPi*(2*a+3/r2)*earr/r2);
                        }
                    }
                }
            }
        }
        s.Ep=(s.Ep-2*pi/3*pow(a/pi,1.5)*s.ttnum)*pow(s.part_p,2)/s.ef;
    }

    if(s.mode==4)
    {
        double a=pow(an/s.boundary,2);
        double sqrtAdivPi = sqrt(a / pi), sqrtA = an/s.boundary;
        double b3=pow(s.boundary,3) , twopib=2*pi/s.boundary;

        for(int i=0; i<s.ttnum; i++)
        {
            for(int j=i+1; j<s.ttnum; j++)
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
                    s.Ep+=4*kz*kz*pi*cos(twopib*(kx*dx+ky*dy+kz*dz))*
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
                            s.Ep+=(erfcSqrtAmR/r3+2*sqrtAdivPi*earr/r2)
                                  -dz2*(3*erfcSqrtAmR/r5+2*sqrtAdivPi*(2*a+3/r2)*earr/r2);
                        }
                    }
                }
            }
        }
        s.Ep=(s.Ep-2*pi/3*pow(a/pi,1.5)*s.ttnum)*pow(s.part_p,2)/s.ef;
    }
}
