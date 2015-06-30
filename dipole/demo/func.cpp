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

double potential_xyz(double x,double y,double z,sys &s)
{
    double potential=0;
    if(s.mode==1||s.mode==2)
    {
        for(int n=0; n<s.ttnum; n++)
        {
            double dx=x-s.part_ob[n].x;
            double dy=y-s.part_ob[n].y;
            double dz=z-s.part_ob[n].z;
            double r=sqrt(dx*dx+dy*dy+dz*dz);
            potential+=dz/pow(r,3);
        }
    }

    if(s.mode==3||s.mode==4)
    {
        double a=an*an/s.boundary/s.boundary;
        double sqrtAdivPi = sqrt(a / pi), sqrtA = an/s.boundary;
        double b3=pow(s.boundary,3) , twopib=2*pi/s.boundary;
        for(int n=0; n<s.ttnum; n++)
        {
            double dx_=x-s.part_ob[n].x;
            double dy_=y-s.part_ob[n].y;
            double dz_=z-s.part_ob[n].z;
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
                potential+=4*kz*pi*sin(twopib*(kx*dx+ky*dy+kz*dz))*
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
                        double earr = exp(-a*r*r), erfcSqrtAmR = erfc(sqrtA*r);
                        double r2 = r*r , r3=r2*r ;
                        potential+=2*sqrtAdivPi*dz*earr/r2+dz*erfcSqrtAmR/r3;
                    }
                }
            }
        }
    }
    return potential;
}

void system_potential_yz(double x_,sys &s)
{
    double BdivM=s.boundary/(double)map_n;
    for(int j=0; j<map_n; j++)
    {
        for(int k=0; k<map_n; k++)
        {
            s.potential_map_yz[j][k]=0;
            s.potential_map_yz[j][k]=potential_xyz(x_,j*BdivM,k*BdivM,s);
        }
    }
}

void system_potential_xz(double y_,sys &s)
{
    double BdivM=s.boundary/(double)map_n;
    for(int i=0; i<map_n; i++)
    {
        for(int k=0; k<map_n; k++)
        {
            s.potential_map_xz[i][k]=0;
            s.potential_map_xz[i][k]=potential_xyz(i*BdivM,y_,k*BdivM,s);
        }
    }
}

void system_potential_xy(double z_,sys &s)
{
    double BdivM=s.boundary/(double)map_n;
    for(int i=0; i<map_n; i++)
    {
        for(int j=0; j<map_n; j++)
        {
            s.potential_map_xy[i][j]=0;
            s.potential_map_yz[i][j]=potential_xyz(i*BdivM,j*BdivM,z_,s);
        }
    }
}

void system_number_yz(sys &s)
{
    double map_dis=s.boundary/(2*map_n);

    for(int j=0; j<map_n; j++)
    {
        for(int k=0; k<map_n; k++)
        {
            s.number_map_yz[j][k]=0;
            for(int n=0; n<s.ttnum; n++)
            {
                double dy=j*s.boundary/map_n-s.part_ob[n].y;
                double dz=k*s.boundary/map_n-s.part_ob[n].z;
                if((dy>0?dy:-dy)<map_dis&&(dz>0?dz:-dz)<map_dis)
                {
                    s.number_map_yz[j][k]+=1;
                }
            }
        }
    }
}

void system_number_xz(sys &s)
{
    double map_dis=s.boundary/(2*map_n);

    for(int i=0; i<map_n; i++)
    {
        for(int k=0; k<map_n; k++)
        {
            s.number_map_xz[i][k]=0;
            for(int n=0; n<s.ttnum; n++)
            {
                double dx=i*s.boundary/map_n-s.part_ob[n].x;
                double dz=k*s.boundary/map_n-s.part_ob[n].z;
                if((dx>0?dx:-dx)<map_dis&&(dz>0?dz:-dz)<map_dis)
                {
                    s.number_map_xz[i][k]+=1;
                }
            }
        }
    }
}

void system_number_xy(sys &s)
{
    double map_dis=s.boundary/(2*map_n);

    for(int i=0; i<map_n; i++)
    {
        for(int j=0; j<map_n; j++)
        {
            s.number_map_xy[i][j]=0;
            for(int n=0; n<s.ttnum; n++)
            {
                double dx=i*s.boundary/map_n-s.part_ob[n].x;
                double dy=j*s.boundary/map_n-s.part_ob[n].y;
                if((dx>0?dx:-dx)<map_dis&&(dy>0?dy:-dy)<map_dis)
                {
                    s.number_map_xy[i][j]+=1;
                }
            }
        }
    }
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

void print_info(sys &s)
{
    for(int i=1; i<100; i++) std::cout<<"\b";
    if(s.cal_total%10==0) system_energy(s);
    if(s.mode==1||s.mode==3)
        std::cout<<"步数："<<s.cal_total<<"    总能量："<<s.Ek+s.Ep<<"    动能："<<s.Ek<<"    势能："<<s.Ep<<"          ";
    if(s.mode==2||s.mode==4)
        std::cout<<"步数："<<s.cal_total<<"    总能量（势能）："<<s.Ep<<"          ";
    s.cal_total++;
}

void menu_temp(sys &s,Main_display &m)
{
    cout<<"欢迎来到Dipole Demo版"<<endl;
    cout<<endl;
    ifstream fin("save//save.sav");
    if(fin)
    {
        char select_n;
        cout<<"是否继续上次的计算？（y/n）";
        cin>>select_n;
        if(select_n=='y')
        {
            load_present(s,m);
            system("cls");
            goto start;
        }
        system("cls");
    }

mode_select:
    ;

    cout<<"请输入数字选择相应的模式"<<endl;
    cout<<"1、分子动力学方法（刚性边界）"<<endl;
    cout<<"2、蒙特卡洛方法（刚性边界）"<<endl;
    cout<<"3、分子动力学方法（周期边界）"<<endl;
    cout<<"4、蒙特卡洛方法（周期边界）"<<endl;
    cout<<"5、退出"<<endl;
    cin>>mode;
    s.mode=mode;
    system("cls");

const_select:
    ;
    char temp;
    if(mode==1||mode==3)
    {
        cout<<"请输入相应的数字改变你需要改变的参数"<<endl;
        cout<<"1、粒子半径（现在的值为"<<part_r<<"）"<<endl;
        cout<<"2、粒子介电常数（现在的值为"<<ep<<"）"<<endl;
        cout<<"3、液体介电常数（现在的值为"<<ef<<"）"<<endl;
        cout<<"4、电场强度（现在的值为"<<E<<"）"<<endl;
        cout<<"5、粒子质量密度（现在的值为"<<rou<<"）"<<endl;
        cout<<"6、总粒子数（现在的值为"<<ttnum<<"）"<<endl;
        cout<<"7、容器边长（现在的值为"<<boundary<<"）"<<endl;
        cout<<"8、阻尼系数（现在的值为"<<f<<"）"<<endl;
        cout<<"9、时间步长（现在的值为"<<dt<<"）"<<endl;
        cout<<"0、开始"<<endl;
        cout<<"b、重新选择模式"<<endl;
        cin>>temp;
        system("cls");

        if(temp=='1')
        {
            cout<<"输入粒子半径"<<endl;
            cin>>part_r;
            s.part_r=part_r;
            system("cls");
            goto const_select;
        }

        else if(temp=='2')
        {
            cout<<"输入粒子介电常数"<<endl;
            cin>>ep;
            s.ep=ep;
            s.part_p=pow(part_r,3.)*E*ef*(ep-ef)/(ep+2*ef);
            system("cls");
            goto const_select;
        }

        else if(temp=='3')
        {
            cout<<"输入液体介电常数"<<endl;
            cin>>ef;
            s.ef=ef;
            s.part_p=pow(part_r,3.)*E*ef*(ep-ef)/(ep+2*ef);
            system("cls");
            goto const_select;
        }

        else if(temp=='4')
        {
            cout<<"输入电场强度"<<endl;
            cin>>E;
            s.E=E;
            s.part_p=pow(part_r,3.)*E*ef*(ep-ef)/(ep+2*ef);
            system("cls");
            goto const_select;
        }

        else if(temp=='5')
        {
            cout<<"输入粒子质量密度"<<endl;
            cin>>rou;
            s.rou=rou;
            s.part_m=4*pi/3.*rou*pow(part_r,3.);
            system("cls");
            goto const_select;
        }

        else if(temp=='6')
        {
            cout<<"输入总粒子数（注意，模式1或2能流畅模拟约300个粒子，3或4只有约60个）"<<endl;
            cin>>ttnum;
            s.ttnum=ttnum;
            system("cls");
            goto const_select;
        }

        else if(temp=='7')
        {
            cout<<"输入容器边长（推荐"<<(int)(pow(ttnum,1./3.)+1)*5.*part_r<<"，至少大于"<<(int)(pow(ttnum,1./3.)+1)*2.5*part_r<<"）"<<endl;
            cin>>boundary;
            s.boundary=boundary;
            system("cls");
            goto const_select;
        }

        else if(temp=='8')
        {
            cout<<"输入阻尼系数"<<endl;
            cin>>f;
            s.f=f;
            system("cls");
            goto const_select;
        }

        else if(temp=='9')
        {
            cout<<"输入时间步长"<<endl;
            cin>>dt;
            s.dt=dt;
            system("cls");
            goto const_select;
        }

        else if(temp=='0')
        {
            system("cls");
            goto start;
        }

        else if(temp=='b')
        {
            system("cls");
            goto mode_select;
        }
    }

    if(mode==2||mode==4)
    {
        cout<<"请输入相应的数字改变你需要改变的参数"<<endl;
        cout<<"1、粒子半径（现在的值为"<<part_r<<"）"<<endl;
        cout<<"2、粒子介电常数（现在的值为"<<ep<<"）"<<endl;
        cout<<"3、液体介电常数（现在的值为"<<ef<<"）"<<endl;
        cout<<"4、电场强度（现在的值为"<<E<<"）"<<endl;
        cout<<"5、总粒子数（现在的值为"<<ttnum<<"）"<<endl;
        cout<<"6、容器边长（现在的值为"<<boundary<<"）"<<endl;
        cout<<"0、开始"<<endl;
        cout<<"b、重新选择模式"<<endl;
        cin>>temp;
        system("cls");

        if(temp=='1')
        {
            cout<<"输入粒子半径"<<endl;
            cin>>part_r;
            s.part_r=part_r;
            system("cls");
            goto const_select;
        }

        else if(temp=='2')
        {
            cout<<"输入粒子介电常数"<<endl;
            cin>>ep;
            s.ep=ep;
            s.part_p=pow(part_r,3.)*E*ef*(ep-ef)/(ep+2*ef);
            system("cls");
            goto const_select;
        }

        else if(temp=='3')
        {
            cout<<"输入液体介电常数"<<endl;
            cin>>ef;
            s.ef=ef;
            s.part_p=pow(part_r,3.)*E*ef*(ep-ef)/(ep+2*ef);
            system("cls");
            goto const_select;
        }

        else if(temp=='4')
        {
            cout<<"输入电场强度"<<endl;
            cin>>E;
            s.E=E;
            s.part_p=pow(part_r,3.)*E*ef*(ep-ef)/(ep+2*ef);
            system("cls");
            goto const_select;
        }

        else if(temp=='5')
        {
            cout<<"输入总粒子数（注意，模式1或2能流畅模拟约300个粒子，3或4只有约60个）"<<endl;
            cin>>ttnum;
            s.ttnum=ttnum;
            system("cls");
            goto const_select;
        }

        else if(temp=='6')
        {
            cout<<"输入容器边长（推荐"<<(int)(pow(ttnum,1./3.)+1)*5.*part_r<<"，至少大于"<<(int)(pow(ttnum,1./3.)+1)*2.5*part_r<<"）"<<endl;
            cin>>boundary;
            s.boundary=boundary;
            system("cls");
            goto const_select;
        }

        else if(temp=='0')
        {
            system("cls");
            goto start;
        }

        else if(temp=='b')
        {
            system("cls");
            goto mode_select;
        }
    }

    if(mode==5)
    {
        exit(0);
    }

start:
    ;
}
