#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

#include "../headers/quantifying_information.h"

const float beta_1 = 1.0;
const char boundary_conditions { 'o' };

float stationary_distribution(unsigned int* lattice,
                              unsigned int lattice_shape[2],
                              const float beta = beta_1) {

    return std::exp(
        -1.0 * energy_lattice(lattice, lattice_shape, boundary_conditions) *
        beta);
}

void print_lattice_to_file_buffered(unsigned int* lattice,
                                    unsigned int lattice_size,
                                    std::ofstream& file, std::string& buffer,
                                    unsigned int buffer_limit = 1024) {
    std::ostringstream oss;

    for (unsigned int i { 0 }; i < lattice_size; i++) {
        oss << lattice[i] << ' ';
    }
    oss << '\n';

    buffer += oss.str();

    if (buffer.size() >= buffer_limit) {
        file << buffer;
        buffer.clear();
    }
}

void next_state(unsigned int* lattice, unsigned int lattice_shape[2],
                unsigned int lattice_size) {

    unsigned int cell1 { rand() % lattice_size };
    unsigned int cell2 { rand() % lattice_size };


    unsigned int* lattice2 { new unsigned int[lattice_size] };

    // Copy lattice in lattice2
    {
        for (unsigned int *index { lattice }, *index2 { lattice2 };
             index != (lattice + lattice_size);
             ++index, ++index2) {
            *index2 = *index;
        }
    }

    // Swapping cell1 e cell2 in lattice2
    unsigned int temp { lattice2[cell1] };
    lattice2[cell1] = lattice[cell2];
    lattice2[cell2] = temp;


    // std::cout << "Swapping : " << (cell1 + 1) << " \033[3"
    //           << (lattice[cell1] + 1) << "m"
    //           << "■" << ' ';
    // // std::cout << "\033[37m";   // White text
    // std::cout << "\033[30m";   // Black text
    // std::cout << " -> ";
    // std::cout << (cell2 + 2) << " \033[3" << (lattice[cell2] + 1) << "m"
    //           << "■" << ' ';
    // // std::cout << "\033[37m";   // White text
    // std::cout << "\033[30m";   // Black text
    // std::cout << std::endl;


    // Energy of the lattices
    float energy1 { energy_lattice(
        lattice, lattice_shape, boundary_conditions) };
    float energy2 { energy_lattice(
        lattice2, lattice_shape, boundary_conditions) };

    float probability1 { stationary_distribution(lattice, lattice_shape) };
    float probability2 { stationary_distribution(lattice2, lattice_shape) };

    float acceptance;
    (1 < (probability2 / probability1))
        ? acceptance = 1
        : acceptance = (probability2 / probability1);

    int rand_n { rand() };

    // std::cout << "Acceptance : " << acceptance << std::endl;
    // (rand3 < acceptance) ? std::cout << "Accepted" : std::cout <<
    // "Discarded"; std::cout << std::endl;

    if (rand_n < acceptance * RAND_MAX) {
        // std::cout << "Accepted" << std::endl;
        // std::cout << std::endl;

        lattice[cell1] = lattice[cell2];
        lattice[cell2] = temp;

    } else {
        // std::cout << "Discarded" << std::endl;
    }

    // for (int i = 0; i < lattice_shape[0]; i++) {
    //     for (int j = 0; j < lattice_shape[1]; j++) {
    //         std::cout << "\033[3" << (lattice[i * lattice_shape[1] + j] + 1)
    //                   << "m"
    //                   << "■" << ' ';
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << "\033[37m";   // White text
    // std::cout << "\033[30m";   // Black text

    // std::cout << "----------------------------------------------" <<
    // std::endl; std::cout << std::endl;

    delete[] lattice2;
}

int main() {
    // unsigned int n{9};
    // unsigned int shape[]{3, 3}; // shape = (number_of_rows,number_of_columns)
    // unsigned int *partition = new unsigned int[n]{4, 3, 1, 1, 0, 0, 0, 0, 0};

    unsigned int n { 30 };   // partition and lattice size
    unsigned int shape[] { 6,
                           5 };   // shape = (number_of_rows,number_of_columns)
    unsigned int* partition =
        new unsigned int[n] { 10, 5, 4, 3, 3, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0,
                              0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    /*unsigned int n{10};*/
    /*unsigned int shape[]{2, 5}; // shape = (number_of_rows,
     * number_of_columns)*/
    /*unsigned int *partition = new unsigned int[n]{4, 3, 1, 1, 1, 0, 0, 0, 0,
     * 0};*/

    // unsigned int* lattice { new unsigned int[n] };
    //
    // // TODO: random filling
    // // Filling the lattice
    // {
    //     unsigned int* lattice_index { lattice };
    //     for (int i = 0; i < n; i++) {
    //         for (int j = 0; j < partition[i]; j++) {
    //             *lattice_index = i;
    //             lattice_index++;
    //         }
    //     }
    //
    //     // lattice_index = nullptr;
    // }

    unsigned int* lattice =
        new unsigned int[n] { 0, 1, 0, 4, 4, 0, 0, 1, 1, 2, 2, 0, 0, 1, 0,
                              2, 2, 3, 6, 3, 4, 1, 0, 5, 0, 5, 6, 3, 0, 6 };

    // Printing the lattice in the terminal
    for (int i = 0; i < shape[0]; i++) {
        for (int j = 0; j < shape[1]; j++) {
            std::cout << "\033[3" << (lattice[i * shape[1] + j] + 1) << "m"
                      << "■" << ' ';
            /*std::cout << lattice[i * shape[1] + j] << ' ';*/
        }
        std::cout << std::endl;
    }
    // std::cout << "\033[37m";   // White text
    std::cout << "\033[30m";   // Black text

    float max_energy { energy_lattice(lattice, shape, boundary_conditions) };
    float min_energy { max_energy };

    std::cout << std::endl;
    std::cout << "Energy : " << max_energy << std::endl;

    std::string filename = "metropolis";
    std::ofstream out_file_lattice_txt(filename + ".txt");
    if (!out_file_lattice_txt) {
        std::cerr << "Error: Could not open " << filename << ".txt"
                  << " for writing." << std::endl;
        return 1;
    }

    std::string buffer;
    unsigned int buffer_limit = 1024;


    // Needed in the Metropolis algorithm implementation in next_state
    std::srand(time({}));

    int n_frames = 600;
    int iterations = 600*100;
    int print_once_every = iterations / (n_frames/2);
    int ten_percent = iterations / 10;
    float energy;

    std::cout << "Progress bar : ";
    for (int i = 0; i < iterations; i++) {

        next_state(lattice, shape, n);
        energy = energy_lattice(lattice, shape, boundary_conditions);

        if (energy > max_energy) {
            max_energy = energy;
        } else if (energy < min_energy) {
            min_energy = energy;
        }

        if ((i % print_once_every) == 0) {
            print_lattice_to_file_buffered(
                lattice, n, out_file_lattice_txt, buffer);
        }
        if ((i % ten_percent) == 0) {
            std::cout << "*" << std::flush;
        }
    }

    for (int i = 0; i < 300; i++) {

        next_state(lattice, shape, n);
        energy = energy_lattice(lattice, shape, boundary_conditions);

        if (energy > max_energy) {
            max_energy = energy;
        } else if (energy < min_energy) {
            min_energy = energy;
        }

        print_lattice_to_file_buffered(
            lattice, n, out_file_lattice_txt, buffer);
    }

    std::cout << std::endl;

    std::cout << "Max energy : " << max_energy << std::endl;
    std::cout << "Min energy : " << min_energy << std::endl;
    std::cout << "Current energy : " << energy << std::endl;

    // Empty the buffer
    if (!buffer.empty()) {
        out_file_lattice_txt << buffer;
        buffer.clear();
    }

    out_file_lattice_txt.close();
    delete[] partition;
    return 0;
}
