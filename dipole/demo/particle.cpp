#include "particle.h"

particle::particle(int num_,double x_,double y_,double z_,double vx_,double vy_,double vz_)
{
    num=num_;
    x=x_;
    y=y_;
    z=z_;
    vx=vx_;
    vy=vy_;
    vz=vz_;
    ax=ay=az=0;
};

