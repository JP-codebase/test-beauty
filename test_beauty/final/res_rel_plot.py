import matplotlib.pyplot as plt

from numpy import block, floor, log10
import struct
import sys


def read_resrel_bin(filename, real_t, fraction):

    # filename      :   name of the file to open
    # real_t        :   'f' float or 'd' double
    # fraction      :   fraction of points to plot ( 0 < fraction <= 1 )

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

    threshold = fraction * 10 ** (-floor(log10(fraction)))
    reset_count = 1 * 10 ** (-floor(log10(fraction)))

    i = 0

    with open(filename, "rb") as file:
        while True:
            if i < threshold:
                record = file.read(record_size)
                if len(record) < record_size:
                    break
                res, rel = struct.unpack(record_type, record)
                # print ("res : ", res, "\t rel : ", rel)
                res_vals.append(res)
                rel_vals.append(rel)
                i = i + 1
            elif i == reset_count:
                record = file.read(record_size)
                if len(record) < record_size:
                    break
                res, rel = struct.unpack(record_type, record)
                res_vals.append(res)
                rel_vals.append(rel)
                i = 0
            else:
                record = file.read(record_size)
                if len(record) < record_size:
                    break
                i = i + 1

    return res_vals, rel_vals


fraction = 1
arguments = sys.argv[1:]

if len(arguments) == 0:

    n = int(input("Insert the size. (Width x Height) : \t"))
    res_vals, rel_vals = read_resrel_bin(
        "./partitions/partitions_of_" + str(n) + "_resrel.bin", "d", fraction
    )

elif len(arguments) == 1:

    n = int(arguments[0])
    res_vals, rel_vals = read_resrel_bin(
        "./partitions/partitions_of_" + str(n) + "_resrel.bin", "d", fraction
    )

elif len(arguments) == 3:

    n = int(arguments[0])
    res_left_bound = float(arguments[1])
    res_right_bound = float(arguments[2])
    # filename = "partitions/[" + str(res_left_bound) + "," + str(res_right_bound) + "]_partitions_of_" + str(n) + "_resrel.bin"
    # filename = "partitions/[ f'{res_left_bound:g}, f'{res_right_bound:g}]_partitions_of_" + str(n) + "_resrel.bin"
    filename = (
        "partitions/["
        + f"{res_left_bound:g}"
        + ","
        + f"{res_right_bound:g}"
        + "]_partitions_of_"
        + str(n)
        + "_resrel.bin"
    )
    res_vals, rel_vals = read_resrel_bin(filename, "d", fraction)

else:
    print("Error: Too many arguments")
    sys.exit(1)


# Print plot

plt.scatter(res_vals, rel_vals, color="blue", alpha=0.7, marker=".", s=2)

plt.title("n = " + str(n) + "  N. Partitions ~ " + str(int(len(res_vals) / fraction)))
plt.xlabel("Resolution")
plt.ylabel("Relevance")
plt.xticks(rotation=60, ha="right")

plt.grid(True)

plt.show()
# plt.savefig()

