#ifndef MONTE_CARLO_H_INCLUDED
#define MONTE_CARLO_H_INCLUDED
#include "sys.h"

double single_energy(int i,sys &s);
void system_mc_moveonce(sys &s);

double single_T_energy(int i,sys &s);
void system_mcT_moveonce(sys &s);

#endif // MONTE_CARLO_H_INCLUDED
