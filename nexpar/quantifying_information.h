#ifndef quantifying_information_h
#define quantifying_information_h

// Degeneracy profile
void degeneracy_profile(unsigned int *partition, unsigned int *dg_profile,
                        unsigned int partition_size);

// Resolution
// Without knowing the degeneracy profile
float resolution(unsigned int *partition, unsigned int partition_size);

// Knowing the degeneracy profile
float resolution_degeneracy(unsigned int *dg_profile, unsigned int dg_profile_size);
                            

// Relevance
// Without knowing the degeneracy profile
float relevance(unsigned int *partition, unsigned int partition_size);

// Knowing the degeneracy profile
float relevance_degeneracy(unsigned int *dg_profile, unsigned int dg_profile_size);
                          

#endif // quantifying_information_h
