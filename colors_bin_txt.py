import struct
import sys

# def read_resrel_bin(filename, real_t):
#
#     # filename             :   name of the file to open
#     # real_t               :   'f' float or 'd' double
#
#     match real_t:
#         case "f":
#             record_type = "ff"
#             record_size = 8
#         case "d":
#             record_type = "dd"
#             record_size = 16
#         case _:
#             print(
#                 "Invalid record type in read_resrel_bin function. Default type float."
#             )
#             record_type = "ff"
#             record_size = 8
#
#     res_vals, rel_vals = [], []
#
#     # Store data if counter < threshold and discard them if threshold < counter < reset_counter
#
#     i = 0
#
#     with open(filename, "rb") as file:
#         while True:
#             record = file.read(record_size)
#             if len(record) < record_size:
#                 break
#             res, rel = struct.unpack(record_type, record)
#             res_vals.append(res)
#             rel_vals.append(rel)
#             i = i + 1
#
#     return res_vals, rel_vals



def read_colors_bin(filename, int_t):

    # filename      :   name of the file to open
    # int_t        :   'i' int or 'I' unsigned int

    record_size = 4

    match int_t:
        case "i":
            record_type = "i"
        case "I":
            record_type = "I"
        case _:
            print("Invalid record type in read_colors_bin function. Default type int.")
            record_type = "i"

    n_colors_vals = []

    i = 0

    with open(filename, "rb") as file:
        while True:
                record = file.read(record_size)
                if len(record) < record_size:
                    break
                n_colors = struct.unpack(record_type, record)
                n_colors_vals.append(n_colors)
                i = i + 1

    return n_colors_vals

def print_colors_txt(filename, n_colors_vals):
    with open(filename, "w") as file:
        for i in range(0, len(n_colors_vals)):
            print(n_colors_vals[i], file=file)

# def print_resres_colors_txt(filename, res_vals, rel_vals, n_colors_vals):
#     with open(filename, "w") as file:
#         for i in range(0, len(res_vals)):
#             print(n_colors_vals[i], res_vals[i], rel_vals[i], file=file)

def main():
    
    arguments = sys.argv[1:]
    filename = ""

    if len(arguments) != 1:
        print(f"Error : Expected 1 arguments. Got {len(arguments)}")
        print("Arguments : filename")
        return 1
    else:
        filename = arguments[0]

    # res_vals, rel_vals = read_resrel_bin(filename, "d")
    n_colors_vals = read_colors_bin(filename, "I")
    
    print_colors_txt(filename.replace(".bin", ".txt"), n_colors_vals)

if __name__ == "__main__":
    main()

