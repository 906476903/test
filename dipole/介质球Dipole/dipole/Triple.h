#ifndef TRIPLE_H_INCLUDED
#define TRIPLE_H_INCLUDED

class Triple
{
public:
    Triple(int x, int y, int z);
    Triple() {};
    bool operator < (const Triple &b) const;

    int kx, ky, kz;
    int s;
    double expPiPiSAnAn;
};

#endif // TRIPLE_H_INCLUDED
