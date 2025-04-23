#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "../headers/quantifying_information.h"

float stationary_distribution(unsigned int* lattice,
                              unsigned int lattice_shape[2], const float beta) {

    return std::exp(-energy_lattice(lattice, lattice_shape) * beta);
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

void next_state(unsigned int* lattice, unsigned int lattice_shape[2], unsigned int lattice_size) {

    // unsigned int lattice_size = lattice_shape[0] * lattice_shape[1];

    float rand1 { static_cast<float>(rand()) / static_cast<float>(RAND_MAX) };
    float rand2 { static_cast<float>(rand()) / static_cast<float>(RAND_MAX) };

    unsigned int cell1 { static_cast<unsigned int>(
        floor(rand1 * (lattice_size))) };
    unsigned int cell2 { static_cast<unsigned int>(
        floor(rand2 * (lattice_size))) };

    unsigned int* lattice2 { new unsigned int[lattice_size] };
    std::copy(lattice, (lattice + lattice_size), lattice2);

    unsigned int temp { lattice[cell1] };
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
    //
    float energy1 { energy_lattice(lattice, lattice_shape) };
    float energy2 { energy_lattice(lattice2, lattice_shape) };
    /*std::cout << "energy1 : " << energy1 << std::endl;*/
    /*std::cout << "energy2 : " << energy2 << std::endl;*/

    float probability1 { std::exp(-energy1) };
    float probability2 { std::exp(-energy2) };

    float acceptance;
    (1 < (probability2 / probability1))
        ? acceptance = 1
        : acceptance = (probability2 / probability1);

    float rand3 { static_cast<float>(rand()) / static_cast<float>(RAND_MAX) };

    /*std::cout << "rand 3: " << rand3 << std::endl;*/
    // std::cout << "Acceptance : " << acceptance << std::endl;
    // (rand3 < acceptance) ? std::cout << "Accepted" : std::cout <<
    // "Discarded"; std::cout << std::endl;

    if (rand3 < acceptance) {
        // std::cout << "Accepted" << std::endl;
        // std::cout << std::endl;
        std::copy(lattice2, (lattice2 + lattice_size), lattice);
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

    // std::cout << "----------------------------------------------" << std::endl;
    // std::cout << std::endl;


    delete[] lattice2;
}

int main() {
    // unsigned int n{9};
    // unsigned int shape[]{3, 3}; // shape = (number_of_rows,number_of_columns)
    // unsigned int *partition = new unsigned int[n]{4, 3, 1, 1, 0, 0, 0, 0, 0};

    unsigned int n { 30 }; // partition and lattice size
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

    unsigned int* lattice { new unsigned int[n] };

    // TODO: random filling
    // Filling the lattice
    {
        unsigned int* lattice_index { lattice };
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < partition[i]; j++) {
                *lattice_index = i;
                lattice_index++;
            }
        }
        // lattice_index = nullptr;
    }

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

    std::cout << std::endl;
    std::cout << "Energy : " << energy_lattice(lattice, shape, 'p')
              << std::endl;
    /*energy_lattice(lattice, shape, 'p');*/


    std::string filename = "metropolis";
    std::ofstream out_file_lattice_txt(filename + ".txt");
    if (!out_file_lattice_txt) {
        std::cerr << "Error: Could not open " << filename << ".txt"
                  << " for writing." << std::endl;
        return 1;
    }

    std::string buffer;
    unsigned int buffer_limit = 128;


    // Needed in the Metropolis algorithm implementation in next_state
    std::srand(time({}));

    for (int i = 0; i < 10000; i++) {
        next_state(lattice, shape, n);
        print_lattice_to_file_buffered(lattice, n, out_file_lattice_txt, buffer);
    }

    // Empty the buffer
    if (!buffer.empty()) {
        out_file_lattice_txt << buffer;
        buffer.clear();
    }

    out_file_lattice_txt.close();
    delete[] partition;
    return 0;
}
