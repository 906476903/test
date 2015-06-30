#ifndef PARTICLE_H_INCLUDED
#define PARTICLE_H_INCLUDED
class particle
{
public:
    particle(int num=0,double x=0,double y=0,double z=0,double vx=0,double vy=0,double vz=0);
    ~particle(){};

    int num;
    double x,y,z,vx,vy,vz,ax,ay,az;
};


#endif // PARTICLE_H_INCLUDED
