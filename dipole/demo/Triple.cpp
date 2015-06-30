#include "Triple.h"
#include "const.h"
#include <cmath>

Triple::Triple(int x, int y, int z)
{
    kx = x, ky = y, kz = z;
    s = kx * kx + ky * ky + kz * kz;
    expPiPiSAnAn = exp(-pi*pi*(s)/(an*an));
}

bool Triple::operator < (const Triple &b) const
{
    return s < b.s;
}
