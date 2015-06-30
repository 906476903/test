#include "velocity_verlet.h"
#include "const.h"
#include <cmath>
#include <cstdlib>

void system_acal(sys &s)
{
    for(int i=0; i<s.ttnum; i++)
    {
        s.part_ob[i].ax=-s.f*s.part_ob[i].vx/s.part_m;
        s.part_ob[i].ay=-s.f*s.part_ob[i].vy/s.part_m;
        s.part_ob[i].az=-s.f*s.part_ob[i].vz/s.part_m;
    }

    double p2 = pow(s.part_p, 2);
    for(int i=0; i<s.ttnum; i++)
    {
        for(int j=i+1; j<s.ttnum; j++)
        {
            double dx=s.part_ob[i].x-s.part_ob[j].x;
            double dy=s.part_ob[i].y-s.part_ob[j].y;
            double dz=s.part_ob[i].z-s.part_ob[j].z;
            double r=sqrt(dx*dx+dy*dy+dz*dz), d2 = pow(dz/r,2), r5 = pow(r,5);
            double a0=3.*p2*(1.-5.*d2)/(s.ef*r5*s.part_m);
            double azi=3.*p2*dz*(3.-5.*d2)/(s.ef*r5*s.part_m);
            s.part_ob[i].ax+=a0*dx;
            s.part_ob[i].ay+=a0*dy;
            s.part_ob[i].az+=azi;
            s.part_ob[j].ax-=a0*dx;
            s.part_ob[j].ay-=a0*dy;
            s.part_ob[j].az-=azi;
        }
    }
}

void system_acal_judge(sys &s)
{
    for(int i=0; i<s.ttnum; i++)
    {
        s.part_ob[i].ax=-s.f*s.part_ob[i].vx/s.part_m;
        s.part_ob[i].ay=-s.f*s.part_ob[i].vy/s.part_m;
        s.part_ob[i].az=-s.f*s.part_ob[i].vz/s.part_m;
    }
    double p2 = pow(s.part_p,2);
    for(int i=0; i<s.ttnum; i++)
    {
        if(s.part_ob[i].x<s.part_r)
        {
            s.part_ob[i].x=s.part_r;
            s.part_ob[i].vx=-s.part_ob[i].vx;
        }
        if(s.part_ob[i].x>s.boundary-s.part_r)
        {
            s.part_ob[i].x=s.boundary-s.part_r;
            s.part_ob[i].vx=-s.part_ob[i].vx;
        }
        if(s.part_ob[i].y<s.part_r)
        {
            s.part_ob[i].y=s.part_r;
            s.part_ob[i].vy=-s.part_ob[i].vy;
        }
        if(s.part_ob[i].y>s.boundary-s.part_r)
        {
            s.part_ob[i].y=s.boundary-s.part_r;
            s.part_ob[i].vy=-s.part_ob[i].vy;
        }
        if(s.part_ob[i].z<s.part_r)
        {
            s.part_ob[i].z=s.part_r;
            s.part_ob[i].vz=-s.part_ob[i].vz;
        }
        if(s.part_ob[i].z>s.boundary-s.part_r)
        {
            s.part_ob[i].z=s.boundary-s.part_r;
            s.part_ob[i].vz=-s.part_ob[i].vz;
        }

        for(int j=i+1; j<s.ttnum; j++)
        {
            double dx=s.part_ob[i].x-s.part_ob[j].x;
            double dy=s.part_ob[i].y-s.part_ob[j].y;
            double dz=s.part_ob[i].z-s.part_ob[j].z;
            double r=sqrt(dx*dx+dy*dy+dz*dz);
            double r_rat=(2*s.part_r/r-1);
            if(r_rat>0)
            {
                s.part_ob[i].x+=r_rat*dx/2.;
                s.part_ob[i].y+=r_rat*dy/2.;
                s.part_ob[i].z+=r_rat*dz/2.;
                s.part_ob[j].x-=r_rat*dx/2.;
                s.part_ob[j].y-=r_rat*dy/2.;
                s.part_ob[j].z-=r_rat*dz/2.;
                double dvn=(s.part_ob[i].vx*dx+s.part_ob[i].vy*dy+s.part_ob[i].vz*dz-
                            s.part_ob[j].vx*dx-s.part_ob[j].vy*dy-s.part_ob[j].vz*dz)/r;
                s.part_ob[i].vx-=dvn*dx/r;
                s.part_ob[i].vy-=dvn*dy/r;
                s.part_ob[i].vz-=dvn*dz/r;
                s.part_ob[j].vx+=dvn*dx/r;
                s.part_ob[j].vy+=dvn*dy/r;
                s.part_ob[j].vz+=dvn*dz/r;
            }

            r=sqrt(dx*dx+dy*dy+dz*dz);
            double a0=3.*p2*(1.-5.*pow(dz/r,2))/(pow(r,5)*s.ef*s.part_m);
            double azi=3.*p2*dz*(3.-5.*pow(dz/r,2))/(pow(r,5)*s.ef*s.part_m);
            s.part_ob[i].ax+=a0*dx;
            s.part_ob[i].ay+=a0*dy;
            s.part_ob[i].az+=azi;
            s.part_ob[j].ax-=a0*dx;
            s.part_ob[j].ay-=a0*dy;
            s.part_ob[j].az-=azi;
        }
    }
}

void system_vv_moveonce(sys &s)
{
    system_acal(s);

    for(int i=0; i<s.ttnum; i++)
    {
        s.part_ob[i].vx+=0.5*s.part_ob[i].ax*s.dt;
        s.part_ob[i].vy+=0.5*s.part_ob[i].ay*s.dt;
        s.part_ob[i].vz+=0.5*s.part_ob[i].az*s.dt;
        s.part_ob[i].x+=s.part_ob[i].vx*s.dt;
        s.part_ob[i].y+=s.part_ob[i].vy*s.dt;
        s.part_ob[i].z+=s.part_ob[i].vz*s.dt;
    }

    system_acal_judge(s);

    for(int i=0; i<s.ttnum; i++)
    {
        s.part_ob[i].vx+=0.5*s.part_ob[i].ax*s.dt;
        s.part_ob[i].vy+=0.5*s.part_ob[i].ay*s.dt;
        s.part_ob[i].vz+=0.5*s.part_ob[i].az*s.dt;
    }

    return;
}

void system_T_acal(sys &s)
{
    double axi,ayi,azi,a0;
    double a=an*an/s.boundary/s.boundary;
    double sqrtAdivPi = sqrt(a / pi), sqrtA = sqrt(a);
    double p2 = pow(s.part_p, 2), b4 = pow(s.boundary,4), twopib=2*pi/s.boundary;
    for(int i=0; i<s.ttnum; i++)
    {
        s.part_ob[i].ax=-s.f*s.part_ob[i].vx/s.part_m;
        s.part_ob[i].ay=-s.f*s.part_ob[i].vy/s.part_m;
        s.part_ob[i].az=-s.f*s.part_ob[i].vz/s.part_m;
    }
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
            double rp_2=dx*dx+dy*dy;
            for(int pp = 0; pp < TotTri; ++pp)
            {
                if(s.k3[pp].s > kn * kn) break;
                if(abs(s.k3[pp].kx) + abs(s.k3[pp].ky) + abs(s.k3[pp].kz) == 0||s.k3[pp].kz<0) continue;
                int kx = s.k3[pp].kx, ky = s.k3[pp].ky, kz = s.k3[pp].kz;
                double k2=s.k3[pp].s;
                a0=p2*8*kz*kz*pi*pi*sin(twopib*(kx*dx+ky*dy+kz*dz))*s.k3[pp].expPiPiSAnAn/(b4*k2*s.part_m*s.ef);
                if(kz==0)
                {
                    axi=kx*a0;
                    ayi=ky*a0;
                    azi=0;
                }
                else
                {
                    axi=2*kx*a0;
                    ayi=2*ky*a0;
                    azi=2*kz*a0;
                }

                s.part_ob[i].ax+=axi;
                s.part_ob[i].ay+=ayi;
                s.part_ob[i].az+=azi;
                s.part_ob[j].ax-=axi;
                s.part_ob[j].ay-=ayi;
                s.part_ob[j].az-=azi;
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
                        rp_2=dx*dx+dy*dy;
                        double arr = a*r*r, earr = exp(-arr), erfcSqrtAmR = erfc(sqrtA*r);
                        double dz2 = dz*dz, dz4 = dz2*dz*dz, dz6=dz4*dz*dz, r5 = r*r*r*r*r, r7 = r5*r*r;
                        a0=-p2*(earr*2*sqrtAdivPi*r*(4*a*a*dz6+8*a*dz4*(a*rp_2+1)
                                                     +2*dz2*(a*rp_2*(2*a*rp_2+3)+6)-rp_2*(2*a*rp_2+3))
                                -3*(r*r-5*dz2)*erfcSqrtAmR)/(r7*s.part_m*s.ef);
                        axi=a0*dx;
                        ayi=a0*dy;
                        azi=(a0+p2*(4*sqrtAdivPi*earr*r*(3+2*arr)+6*erfcSqrtAmR)/(s.ef*r5*s.part_m))*dz;

                        s.part_ob[i].ax+=axi;
                        s.part_ob[i].ay+=ayi;
                        s.part_ob[i].az+=azi;
                        s.part_ob[j].ax-=axi;
                        s.part_ob[j].ay-=ayi;
                        s.part_ob[j].az-=azi;
                    }
                }
            }
        }
    }
}

void system_T_acal_judge(sys &s)
{
    double axi,ayi,azi,a0;
    double a=an*an/s.boundary/s.boundary;
    double sqrtAdivPi = sqrt(a / pi), sqrtA = sqrt(a);
    double p2 = pow(s.part_p, 2), b4 = pow(s.boundary,4) , twopib=2*pi/s.boundary;;
    for(int i=0; i<s.ttnum; i++)
    {
        s.part_ob[i].ax=-s.f*s.part_ob[i].vx/s.part_m;
        s.part_ob[i].ay=-s.f*s.part_ob[i].vy/s.part_m;
        s.part_ob[i].az=-s.f*s.part_ob[i].vz/s.part_m;
    }

    for(int i=0; i<s.ttnum; i++)
    {
        if(s.part_ob[i].x<0)
        {
            s.part_ob[i].x+=s.boundary;
        }
        if(s.part_ob[i].x>s.boundary)
        {
            s.part_ob[i].x-=s.boundary;
        }
        if(s.part_ob[i].y<0)
        {
            s.part_ob[i].y+=s.boundary;
        }
        if(s.part_ob[i].y>s.boundary)
        {
            s.part_ob[i].y-=s.boundary;
        }
        if(s.part_ob[i].z<0)
        {
            s.part_ob[i].z+=s.boundary;
        }
        if(s.part_ob[i].z>s.boundary)
        {
            s.part_ob[i].z-=s.boundary;
        }
        for(int j=i+1; j<s.ttnum; j++)
        {
            double dx=s.part_ob[i].x-s.part_ob[j].x;
            double dy=s.part_ob[i].y-s.part_ob[j].y;
            double dz=s.part_ob[i].z-s.part_ob[j].z;
            if(dx>s.boundary/2.) dx-=s.boundary;
            if(dx<-s.boundary/2.) dx+=s.boundary;
            if(dy>s.boundary/2.) dy-=s.boundary;
            if(dy<-s.boundary/2.) dy+=s.boundary;
            if(dz>s.boundary/2.) dz-=s.boundary;
            if(dz<-s.boundary/2.) dz+=s.boundary;
            double r=sqrt(dx*dx+dy*dy+dz*dz);
            double r_rat=(2*s.part_r/r-1);
            if(r_rat>0)
            {
                s.part_ob[i].x+=r_rat*dx/2.;
                s.part_ob[i].y+=r_rat*dy/2.;
                s.part_ob[i].z+=r_rat*dz/2.;
                s.part_ob[j].x-=r_rat*dx/2.;
                s.part_ob[j].y-=r_rat*dy/2.;
                s.part_ob[j].z-=r_rat*dz/2.;
                double dvn=(s.part_ob[i].vx*dx+s.part_ob[i].vy*dy+s.part_ob[i].vz*dz-
                            s.part_ob[j].vx*dx-s.part_ob[j].vy*dy-s.part_ob[j].vz*dz)/r;
                s.part_ob[i].vx-=dvn*dx/r;
                s.part_ob[i].vy-=dvn*dy/r;
                s.part_ob[i].vz-=dvn*dz/r;
                s.part_ob[j].vx+=dvn*dx/r;
                s.part_ob[j].vy+=dvn*dy/r;
                s.part_ob[j].vz+=dvn*dz/r;
            }

            double dx_=s.part_ob[i].x-s.part_ob[j].x;
            double dy_=s.part_ob[i].y-s.part_ob[j].y;
            double dz_=s.part_ob[i].z-s.part_ob[j].z;
            dx=dx_;
            dy=dy_;
            dz=dz_;
            r=sqrt(dx*dx+dy*dy+dz*dz);
            double rp_2=dx*dx+dy*dy;

            for(int pp = 0; pp < TotTri; ++pp)
            {
                if(s.k3[pp].s > kn * kn) break;
                if(abs(s.k3[pp].kx) + abs(s.k3[pp].ky) + abs(s.k3[pp].kz) == 0||s.k3[pp].kz<0) continue;
                int kx = s.k3[pp].kx, ky = s.k3[pp].ky, kz = s.k3[pp].kz;
                double k2=s.k3[pp].s;
                a0=p2*8*kz*kz*pi*pi*sin(twopib*(kx*dx+ky*dy+kz*dz))*s.k3[pp].expPiPiSAnAn/(b4*k2*s.part_m*s.ef);
                if(kz==0)
                {
                    axi=kx*a0;
                    ayi=ky*a0;
                    azi=0;
                }
                else
                {
                    axi=2*kx*a0;
                    ayi=2*ky*a0;
                    azi=2*kz*a0;
                }

                s.part_ob[i].ax+=axi;
                s.part_ob[i].ay+=ayi;
                s.part_ob[i].az+=azi;
                s.part_ob[j].ax-=axi;
                s.part_ob[j].ay-=ayi;
                s.part_ob[j].az-=azi;
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
                        rp_2=dx*dx+dy*dy;
                        double arr = a*r*r, earr = exp(-arr), erfcSqrtAmR = erfc(sqrtA*r);
                        double dz2 = dz*dz, dz4 = dz2*dz*dz, dz6=dz4*dz*dz, r5 = r*r*r*r*r, r7=r*r*r5;
                        a0=-p2*(earr*2*sqrtAdivPi*r*(4*a*a*dz6+8*a*dz4*(a*rp_2+1)
                                                     +2*dz2*(a*rp_2*(2*a*rp_2+3)+6)-rp_2*(2*a*rp_2+3))
                                -3*(r*r-5*dz2)*erfcSqrtAmR)/(r7*s.part_m*s.ef);
                        axi=a0*dx;
                        ayi=a0*dy;
                        azi=(a0+p2*(4*sqrtAdivPi*earr*r*(3+2*arr)+6*erfcSqrtAmR)/(s.ef*r5*s.part_m))*dz;

                        s.part_ob[i].ax+=axi;
                        s.part_ob[i].ay+=ayi;
                        s.part_ob[i].az+=azi;
                        s.part_ob[j].ax-=axi;
                        s.part_ob[j].ay-=ayi;
                        s.part_ob[j].az-=azi;
                    }
                }
            }
        }
    }
}

void system_vvT_moveonce(sys &s)
{
    system_T_acal(s);

    for(int i=0; i<s.ttnum; i++)
    {
        s.part_ob[i].vx+=0.5*s.part_ob[i].ax*s.dt;
        s.part_ob[i].vy+=0.5*s.part_ob[i].ay*s.dt;
        s.part_ob[i].vz+=0.5*s.part_ob[i].az*s.dt;
        s.part_ob[i].x+=s.part_ob[i].vx*s.dt;
        s.part_ob[i].y+=s.part_ob[i].vy*s.dt;
        s.part_ob[i].z+=s.part_ob[i].vz*s.dt;
    }

    system_T_acal_judge(s);

    for(int i=0; i<s.ttnum; i++)
    {
        s.part_ob[i].vx+=0.5*s.part_ob[i].ax*s.dt;
        s.part_ob[i].vy+=0.5*s.part_ob[i].ay*s.dt;
        s.part_ob[i].vz+=0.5*s.part_ob[i].az*s.dt;
    }

    return;
}
