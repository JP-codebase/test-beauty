import struct
import sys


def read_resrel_bin(filename, real_t):

    # filename             :   name of the file to open
    # real_t               :   'f' float or 'd' double

    match real_t:
        case "f":
            record_type = "ff"
            record_size = 8
        case "d":
            record_type = "dd"
            record_size = 16
        case _:
            print(
                "Invalid record type in read_resrel_bin function. Default type float."
            )
            record_type = "ff"
            record_size = 8

    res_vals, rel_vals = [], []

    # Store data if counter < threshold and discard them if threshold < counter < reset_counter

    i = 0

    with open(filename, "rb") as file:
        while True:
            record = file.read(record_size)
            if len(record) < record_size:
                break
            res, rel = struct.unpack(record_type, record)
            res_vals.append(res)
            rel_vals.append(rel)
            i = i + 1

    return res_vals, rel_vals


def print_resres_txt(filename, res_vals, rel_vals):
    with open(filename, "w") as file:
        for i in range(0, len(res_vals)):
            print(res_vals[i], ' ', rel_vals[i], file=file)


def main():
    arguments = sys.argv[1:]
    filename = ""

    if len(arguments) != 1:
        print(f"Error : Expected 1 arguments. Got {len(arguments)}")
        print("Arguments : filename")
        return 1
    else:
        filename = arguments[0]

    res_vals, rel_vals = read_resrel_bin(filename, "d")
    
    print_resres_txt(filename.replace(".bin", ".txt"), res_vals, rel_vals)

if __name__ == "__main__":
    main()

