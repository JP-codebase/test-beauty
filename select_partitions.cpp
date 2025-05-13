#include <fstream>
#include <iostream>
#include <string>

#include "./headers/precision.h"


std::string real_t_without_trailing_zeros(real_t f);

const char RED[] = "\033[31m";
const char RESET_STYLE[] = "\033[m";

int main(int argc, char* argv[]) {

    unsigned int partition_size;
    real_t res_min, res_max;


    /* ------------------------ Input Check --------------------------------- */

    // Command line arguments
    {
        if (argc != 4) {
            std::cerr << RED << "Error: Expected 3 arguments: " << RESET_STYLE
                      << "size resolution_min resolution_max. " << RED
                      << "Got : " << RESET_STYLE << (argc - 1) << std::endl;
            return 1;
        }

        try {
            partition_size = std::stoul(argv[1]);
            res_min = std::stod(argv[2]);
            res_max = std::stod(argv[3]);

            if (res_min < 0 || res_max < 0 || res_min > res_max)
                throw std::out_of_range("Min and Max must be ≥0 and Min ≤ Max");
        } catch (const std::invalid_argument&) {
            std::cerr << RED << "Error: Arguments must be numeric.\n"
                      << RESET_STYLE;
            return 1;
        } catch (const std::out_of_range& e) {
            std::cerr << RED << "Error: " << e.what() << "\n" << RESET_STYLE;
            return 1;
        }
    }


    std::string path { "./partitions/" };

    /* ------------------------ Open Input Files --------------------------- */

    std::string filename = "partitions_of_" + std::to_string(partition_size);

    // Input from txt file
    std::ifstream input_file_partitions_txt(path + filename + ".txt");

    if (!input_file_partitions_txt) {
        std::cerr << RED << "Error: Could not open " << path + filename
                  << ".txt"
                  << " for reading." << RESET_STYLE << std::endl;
        return 1;
    }

    // Input from binary file
    std::ifstream input_file_resrel_bin(path + filename + "_resrel.bin",
                                        std::ios::binary);

    if (!input_file_resrel_bin) {
        std::cerr << RED << "Error: Could not open file " << path + filename
                  << "_resrel.bin"
                  << " for reading." << RESET_STYLE << std::endl;
        return 1;
    }

    std::ifstream input_file_colors_bin(path + filename + "_colors.bin",
                                        std::ios::binary);

    if (!input_file_colors_bin) {
        std::cerr << RED << "Error: Could not open file " << path + filename
                  << "_colors.bin"
                  << " for reading." << RESET_STYLE << std::endl;
        return 1;
    }


    /* ------------------------ Open Output Files --------------------------- */

    // Output to txt file
    std::ofstream output_file_partitions_txt(
        path + "[" + real_t_without_trailing_zeros(res_min) + "," +
        real_t_without_trailing_zeros(res_max) + "]_" + filename + ".txt");


    if (!output_file_partitions_txt) {
        std::cerr << RED << "Error: Could not open "
                  << (path + "[" + real_t_without_trailing_zeros(res_min) +
                      "," + real_t_without_trailing_zeros(res_max) + "]_" +
                      filename + ".txt")
                  << " for writing." << RESET_STYLE << std::endl;
        return 1;
    }

    // Output to binary file
    std::ofstream output_file_resrel_bin(
        path + "[" + real_t_without_trailing_zeros(res_min) + "," +
            real_t_without_trailing_zeros(res_max) + "]_" + filename +
            "_resrel.bin",
        std::ios::binary);

    if (!output_file_resrel_bin) {
        std::cerr << RED << "Error: Could not open file "
                  << (path + "[" + real_t_without_trailing_zeros(res_min) +
                      "," + real_t_without_trailing_zeros(res_max) + "]_" +
                      filename + "_resrel.bin")
                  << " for writing." << RESET_STYLE << std::endl;
        return 1;
    }


    std::ofstream output_file_colors_bin(
        path + "[" + real_t_without_trailing_zeros(res_min) + "," +
            real_t_without_trailing_zeros(res_max) + "]_" + filename +
            "_colors.bin",
        std::ios::binary);

    if (!output_file_colors_bin) {
        std::cerr << RED << "Error: Could not open file "
                  << (path + "[" + real_t_without_trailing_zeros(res_min) +
                      "," + real_t_without_trailing_zeros(res_max) + "]_" +
                      filename + "_colors.bin")
                  << " for writing." << RESET_STYLE << std::endl;
        return 1;
    }


    /* ----------------------- Reading from File --------------------------- */


    real_t res { -1 }, rel { -1 };
    const unsigned int size_real_t { sizeof(res) };

    unsigned int n_colors { 0 };
    const unsigned int size_u_int { sizeof(n_colors) };

    std::string partition(2 * partition_size, '\0');

    std::string buffer {};
    unsigned int buffer_limit { 1024 };


    while (true) {

        input_file_resrel_bin.read(reinterpret_cast<char*>(&res), size_real_t);

        input_file_resrel_bin.read(reinterpret_cast<char*>(&rel), size_real_t);

        input_file_colors_bin.read(reinterpret_cast<char*>(&n_colors),
                                   size_u_int);

        getline(input_file_partitions_txt, partition);

        if (input_file_resrel_bin.eof()) {
            break;
        }

        if ((res_min <= res) && (res_max >= res)) {
            output_file_resrel_bin.write(reinterpret_cast<const char*>(&res),
                                         size_real_t);
            output_file_resrel_bin.write(reinterpret_cast<const char*>(&rel),
                                         size_real_t);
            output_file_colors_bin.write(
                reinterpret_cast<const char*>(&n_colors), size_u_int);

            buffer += partition + "\n";

            if (buffer.size() >= buffer_limit) {
                output_file_partitions_txt << buffer;
                buffer.clear();
            }
        }
    }

    if (!buffer.empty()) {
        output_file_partitions_txt << buffer;
        buffer.clear();
    }

    input_file_partitions_txt.close();
    input_file_resrel_bin.close();
    input_file_colors_bin.close();

    output_file_partitions_txt.close();
    output_file_resrel_bin.close();
    output_file_colors_bin.close();

    return 0;
}


/* ---------------------- Function Implementations ------------------------- */

std::string real_t_without_trailing_zeros(real_t f) {

    std::string real_t_str { std::to_string(f) };

    for (int i = real_t_str.size() - 1; i >= 0; i--) {

        if (real_t_str[i] == '0' || real_t_str[i] == '.') {
            real_t_str.erase(i);

        } else if ((real_t_str[i] == '1') || (real_t_str[i] == '2') ||
                   (real_t_str[i] == '3') || (real_t_str[i] == '4') ||
                   (real_t_str[i] == '5') || (real_t_str[i] == '6') ||
                   (real_t_str[i] == '7') || (real_t_str[i] == '8') ||
                   (real_t_str[i] == '9'))
            break;
    }

    return real_t_str;
}
