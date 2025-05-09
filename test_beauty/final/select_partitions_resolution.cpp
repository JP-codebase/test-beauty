#include <fstream>
#include <iostream>
#include <string>

#include "./headers/precision.h"

const char RED[] = "\033[31m";
const char RESET_STYLE[] = "\e[m";

int main(int argc, char* argv[]) {

    unsigned int partition_size;
    real_t res_min, res_max;

    /* ------------------------ Inupt Check --------------------------------- */

    // Command line arguments
    {
        switch (argc) {
            case 4:
                try {
                    try {
                        int value = std::stoi(argv[1]);

                        if (value < 0) {
                            throw std::out_of_range(
                                "Value must be a positive integer");
                        }

                        partition_size = std::stoul(argv[1]);

                    } catch (const std::invalid_argument& e) {
                        std::cerr << RED
                                  << "Error: Invalid argument. Input is "
                                     "not a valid integer.\n"
                                  << RESET_STYLE;
                        return 1;

                    } catch (const std::out_of_range& e) {
                        std::cerr << RED << "Error: " << e.what() << "\n"
                                  << RESET_STYLE;
                        return 1;
                    }

                    real_t value1 = std::stod(argv[2]);
                    real_t value2 = std::stod(argv[3]);

                    if (value1 < 0 || value2 < 0 || value1 > value2) {
                        throw std::out_of_range(
                            "Error: Min, Max > 0 and Min < Max.");
                    }

                    res_min = value1;
                    res_max = value2;

                } catch (const std::invalid_argument& e) {
                    std::cerr << RED
                              << "Error: Invalid argument. Input is "
                                 "not a valid integer.\n"
                              << RESET_STYLE;
                    return 1;
                } catch (const std::out_of_range& e) {
                    std::cerr << RED << "Error: " << e.what() << "\n"
                              << RESET_STYLE;
                    return 1;
                }

                break;
            default:
                std::cerr << RED << "Error: Incorrect number of arguments."
                          << RESET_STYLE << std::endl;
                return 1;
                break;
        }
    }


    /* ------------------------ Input from File ----------------------------- */

    std::string filename =
        "./partitions/partitions_of_" + std::to_string(partition_size);

    // Input to txt file
    std::ifstream input_file_partitions_txt(filename + ".txt");

    if (!input_file_partitions_txt) {
        std::cerr << RED << "Error: Could not open " << filename << ".txt"
                  << " for writing." << RESET_STYLE << std::endl;
        return 1;
    }

    // Input to binary file
    std::ifstream input_file_resrel_bin(filename + "_resrel.bin",
                                        std::ios::binary);

    if (!input_file_resrel_bin) {
        std::cerr << RED << "Error: Could not open file " << filename
                  << "_resrel.bin"
                  << " for writing." << RESET_STYLE << std::endl;
        return 1;
    }


    /* ----------------------- Reading from File --------------------------- */


    real_t res { -1 }, rel { -1 };
    // unsigned int* partition {new unsigned int[partition_size]};

    std::string partition(2*partition_size, '\0');

    while (true) {

        input_file_resrel_bin.read(reinterpret_cast<char*>(&res), sizeof(res));
        input_file_resrel_bin.read(reinterpret_cast<char*>(&rel), sizeof(rel));
        getline(input_file_partitions_txt, partition);

        if (input_file_resrel_bin.eof()) {
            break;
        }

        if ((res_min <= res) && (res_max >= res)) {
            std::cout << "Resolution : " << res << "\t Relevance : " << rel
                      << std::endl;
            for (auto letter : partition) {
                std::cout << letter;
            }
            std::cout << std::endl;
        }
    }


    input_file_partitions_txt.close();
    input_file_resrel_bin.close();

    return 0;
}
