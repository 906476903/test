#include "S_L.h"
#include "const.h"
#include "particle.h"
#include <fstream>
#include <iostream>
#include <windows.h>

using namespace std;

void save_present(sys &s)
{
    ofstream fout;
    CreateDirectory(TEXT("save"),NULL);
    fout.open("save//save.sav",ios::binary);
    fout.write(reinterpret_cast<char *>(&s.part_r),sizeof(s.part_r));
    fout.write(reinterpret_cast<char *>(&s.ep),sizeof(s.ep));
    fout.write(reinterpret_cast<char *>(&s.ef),sizeof(s.ef));
    fout.write(reinterpret_cast<char *>(&s.E),sizeof(s.E));
    fout.write(reinterpret_cast<char *>(&s.part_p),sizeof(s.part_p));
    fout.write(reinterpret_cast<char *>(&s.rou),sizeof(s.rou));
    fout.write(reinterpret_cast<char *>(&s.part_m),sizeof(s.part_m));
    fout.write(reinterpret_cast<char *>(&s.ttnum),sizeof(s.ttnum));
    fout.write(reinterpret_cast<char *>(&s.f),sizeof(s.f));
    fout.write(reinterpret_cast<char *>(&s.dt),sizeof(s.dt));
    fout.write(reinterpret_cast<char *>(&s.boundary),sizeof(s.boundary));
    fout.write(reinterpret_cast<char *>(&s.mode),sizeof(s.mode));
    fout.write(reinterpret_cast<char *>(&s.cal_total),sizeof(s.cal_total));
    fout.write(reinterpret_cast<char *>(&s.Ek),sizeof(s.Ek));
    fout.write(reinterpret_cast<char *>(&s.Ep),sizeof(s.Ep));
    for(int i=0; i<s.ttnum; i++)
    {
        fout.write(reinterpret_cast<char *>(&s.part_ob[i].num),sizeof(s.part_ob[i].num));
        fout.write(reinterpret_cast<char *>(&s.part_ob[i].x),sizeof(s.part_ob[i].x));
        fout.write(reinterpret_cast<char *>(&s.part_ob[i].y),sizeof(s.part_ob[i].y));
        fout.write(reinterpret_cast<char *>(&s.part_ob[i].z),sizeof(s.part_ob[i].z));
        fout.write(reinterpret_cast<char *>(&s.part_ob[i].vx),sizeof(s.part_ob[i].vx));
        fout.write(reinterpret_cast<char *>(&s.part_ob[i].vy),sizeof(s.part_ob[i].vy));
        fout.write(reinterpret_cast<char *>(&s.part_ob[i].vz),sizeof(s.part_ob[i].vz));
    }

    if(s.mode==2||s.mode==4)
    {
        fout.write(reinterpret_cast<char *>(&s.boundary_mcmap),sizeof(s.boundary_mcmap));
        fout.write(reinterpret_cast<char *>(&s.dis),sizeof(s.dis));
        for(int ix=0; ix<s.boundary_mcmap; ix++)
        {
            for(int iy=0; iy<s.boundary_mcmap; iy++)
            {
                for(int iz=0; iz<s.boundary_mcmap; iz++)
                {
                    fout.write(reinterpret_cast<char *>(&s.num_map[ix][iy][iz]),sizeof(s.num_map[ix][iy][iz]));
                }
            }
        }
    }
    fout.close();
}

int load_present(sys &s)
{
    ifstream fin;
    fin.open("save//save.sav",ios::binary);
    if(!fin.is_open()) return 0;
    fin.read(reinterpret_cast<char *>(&s.part_r),sizeof(s.part_r));
    fin.read(reinterpret_cast<char *>(&s.ep),sizeof(s.ep));
    fin.read(reinterpret_cast<char *>(&s.ef),sizeof(s.ef));
    fin.read(reinterpret_cast<char *>(&s.E),sizeof(s.E));
    fin.read(reinterpret_cast<char *>(&s.part_p),sizeof(s.part_p));
    fin.read(reinterpret_cast<char *>(&s.rou),sizeof(s.rou));
    fin.read(reinterpret_cast<char *>(&s.part_m),sizeof(s.part_m));
    fin.read(reinterpret_cast<char *>(&s.ttnum),sizeof(s.ttnum));
    fin.read(reinterpret_cast<char *>(&s.f),sizeof(s.f));
    fin.read(reinterpret_cast<char *>(&s.dt),sizeof(s.dt));
    fin.read(reinterpret_cast<char *>(&s.boundary),sizeof(s.boundary));
    fin.read(reinterpret_cast<char *>(&s.mode),sizeof(s.mode));
    fin.read(reinterpret_cast<char *>(&s.cal_total),sizeof(s.cal_total));
    fin.read(reinterpret_cast<char *>(&s.Ek),sizeof(s.Ek));
    fin.read(reinterpret_cast<char *>(&s.Ep),sizeof(s.Ep));
    s.part_ob=new particle[s.ttnum];
    for(int i=0; i<s.ttnum; i++)
    {
        fin.read(reinterpret_cast<char *>(&s.part_ob[i].num),sizeof(s.part_ob[i].num));
        fin.read(reinterpret_cast<char *>(&s.part_ob[i].x),sizeof(s.part_ob[i].x));
        fin.read(reinterpret_cast<char *>(&s.part_ob[i].y),sizeof(s.part_ob[i].y));
        fin.read(reinterpret_cast<char *>(&s.part_ob[i].z),sizeof(s.part_ob[i].z));
        fin.read(reinterpret_cast<char *>(&s.part_ob[i].vx),sizeof(s.part_ob[i].vx));
        fin.read(reinterpret_cast<char *>(&s.part_ob[i].vy),sizeof(s.part_ob[i].vy));
        fin.read(reinterpret_cast<char *>(&s.part_ob[i].vz),sizeof(s.part_ob[i].vz));
    }
    if(s.mode==2||s.mode==4)
    {
        fin.read(reinterpret_cast<char *>(&s.boundary_mcmap),sizeof(s.boundary_mcmap));
        fin.read(reinterpret_cast<char *>(&s.dis),sizeof(s.dis));
        s.num_map=new int**[s.boundary_mcmap]();
        for(int i=0; i<s.boundary_mcmap; i++)
        {
            s.num_map[i]=new int*[s.boundary_mcmap]();
        }
        for(int i=0; i<s.boundary_mcmap; i++)
        {
            for(int j=0; j<s.boundary_mcmap; j++)
            {
                s.num_map[i][j]=new int[s.boundary_mcmap]();
            }
        }
        for(int ix=0; ix<s.boundary_mcmap; ix++)
        {
            for(int iy=0; iy<s.boundary_mcmap; iy++)
            {
                for(int iz=0; iz<s.boundary_mcmap; iz++)
                {
                    fin.read(reinterpret_cast<char *>(&s.num_map[ix][iy][iz]),sizeof(s.num_map[ix][iy][iz]));
                }
            }
        }
    }
    s.whether_start=1;
    fin.close();
    return 1;
}
