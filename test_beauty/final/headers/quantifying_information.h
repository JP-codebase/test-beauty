#ifndef quantifying_information_h

#include "precision.h"

#define quantifying_information_h

// Degeneracy profile
void degeneracy_profile(unsigned int *partition, unsigned int *dg_profile,
                        unsigned int partition_size);

// Resolution
// Without knowing the degeneracy profile
real_t resolution(unsigned int *partition, unsigned int partition_size);

// Knowing the degeneracy profile
real_t resolution_degeneracy(unsigned int *dg_profile, unsigned int dg_profile_size);
                            

// Relevance
// Without knowing the degeneracy profile
real_t relevance(unsigned int *partition, unsigned int partition_size);

// Knowing the degeneracy profile
real_t relevance_degeneracy(unsigned int *dg_profile, unsigned int dg_profile_size);
                          
// Energy of a lattice with boundary conditions
real_t energy_lattice(unsigned int *lattice, unsigned int lattice_shape[2],
                     char boundary_conditions = 'o');

#endif // quantifying_information_h
