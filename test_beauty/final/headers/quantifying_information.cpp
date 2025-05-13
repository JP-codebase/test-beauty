#include <cmath>
#include <functional>
#include <unordered_map>

// #include "precision.h"

#include "quantifying_information.h"


/* ----------------- Degeneracy Profile ----------------------- */

void degeneracy_profile(unsigned int* partition, unsigned int* dg_profile,
                        unsigned int partition_size) {
    for (unsigned int i = 0; i < partition_size; i++) {
        dg_profile[i] = 0;
    }

    for (unsigned int i = 0; (i < partition_size) && (partition[i] != 0); i++) {
        dg_profile[partition[i] - 1]++;
    }
}

/* ----------------- Resolution ----------------------- */

// Without knowing the degeneracy profile
real_t resolution(unsigned int* partition, unsigned int partition_size) {
    real_t res { 0 };

    unsigned int* dg_profile { new unsigned int[partition_size] };
    degeneracy_profile(partition, dg_profile, partition_size);

    for (unsigned int i = 0; i < partition_size; i++) {
        if (dg_profile[i] == 0)
            continue;
        res -= static_cast<real_t>((i + 1) * dg_profile[i]) /
               static_cast<real_t>(partition_size) *
               std::log(static_cast<real_t>(i + 1) /
                        static_cast<real_t>(partition_size));
    }

    delete[] dg_profile;

    return res;
}

// Knowing the degeneracy profile
real_t resolution_degeneracy(unsigned int* dg_profile,
                             unsigned int dg_profile_size) {
    real_t res { 0 };

    for (unsigned int i = 0; i < dg_profile_size; i++) {
        if (dg_profile[i] == 0)
            continue;
        res -= static_cast<real_t>((i + 1) * dg_profile[i]) /
               static_cast<real_t>(dg_profile_size) *
               std::log(static_cast<real_t>(i + 1) /
                        static_cast<real_t>(dg_profile_size));
    }

    return res;
}


/* ----------------- Relevance ----------------------- */

// Without knowing the degeneracy profile
real_t relevance(unsigned int* partition, unsigned int partition_size) {
    real_t rel { 0 };

    unsigned int* dg_profile { new unsigned int[partition_size] };
    degeneracy_profile(partition, dg_profile, partition_size);

    for (unsigned int i = 0; i < partition_size; i++) {
        if (dg_profile[i] == 0)
            continue;
        rel -= (i + 1) * dg_profile[i] / static_cast<real_t>(partition_size) *
               std::log(static_cast<real_t>((i + 1) * dg_profile[i]) /
                        static_cast<real_t>(partition_size));
    }

    delete[] dg_profile;

    return rel;
}

// Knowing the degeneracy profile
real_t relevance_degeneracy(unsigned int* dg_profile,
                            unsigned int dg_profile_size) {
    real_t rel { 0 };

    for (unsigned int i = 0; i < dg_profile_size; i++) {
        if (dg_profile[i] == 0)
            continue;
        rel -= (i + 1) * dg_profile[i] / static_cast<real_t>(dg_profile_size) *
               std::log(static_cast<real_t>((i + 1) * dg_profile[i]) /
                        static_cast<real_t>(dg_profile_size));
    }

    return rel;
}


/* ----------------- Energy ----------------------- */

using number_of_bonds =
    std::function<unsigned int(int, int, unsigned int*, unsigned int, unsigned int)>;

// Open boundary conditions
number_of_bonds open_boundary = [](int i, int j, unsigned int* lattice,
                                   unsigned int width, unsigned int height) {
    unsigned int n_bonds { 0 };

    if (i > 0) {
        if (lattice[i * width + j] ==
            lattice[(i - 1) * width + j]) {
            n_bonds++;
        }
    }

    if (i < (height - 1)) {
        if (lattice[i * width + j] ==
            lattice[(i + 1) * width + j]) {
            n_bonds++;
        }
    }

    if (j > 0) {
        if (lattice[i * width + j] ==
            lattice[i * width + (j - 1)]) {
            n_bonds++;
        }
    }

    if (j < (width - 1)) {
        if (lattice[i * width + j] ==
            lattice[i * width + (j + 1)]) {
            n_bonds++;
        }
    }

    return n_bonds;
};

real_t energy_colored_grid(unsigned int* lattice, unsigned int width,
                           unsigned int height, char boundary_conditions) {

    // Add key-function pairs to use other boundary conditions
    std::unordered_map<char, number_of_bonds> boundary_functions = {
        { 'o', open_boundary },
    };

    auto pair = boundary_functions.find(boundary_conditions);
    number_of_bonds n_bonds = pair->second;

    real_t total_n_bonds { 0 };
    const real_t J { 1 };   // Coupling constant

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            total_n_bonds += n_bonds(i, j, lattice, width, height);
        }
    }

    
    return  - 1.0 * J * total_n_bonds / 2.0;
}

unsigned int number_of_colors_partition(unsigned int* partition,
                                        unsigned int partition_size) {

    unsigned int n_colors { 0 };

    for (; n_colors < partition_size; n_colors++) {
        if (partition[n_colors] == 0) {
            return n_colors;
        }
    }

    return n_colors;
}
