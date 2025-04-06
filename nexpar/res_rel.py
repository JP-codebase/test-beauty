import matplotlib.pyplot as plt
import struct
import sys

def read_resrel_bin(filename, percentage=100):
    res_vals, rel_vals = [], []
    record_size = 8
    i = 0

    with open(filename, "rb") as file:
        while True:
            if i < percentage:
                record = file.read(record_size)
                if len(record) < record_size:
                    break
                res, rel = struct.unpack("ff", record)
                res_vals.append(res)
                rel_vals.append(rel)
                i = i + 1
            elif i == 100:
                record = file.read(record_size)
                if len(record) < record_size:
                    break
                res, rel = struct.unpack("ff", record)
                res_vals.append(res)
                rel_vals.append(rel)
                i = 0
            else:
                record = file.read(record_size)
                if len(record) < record_size:
                    break
                i = i + 1

    return res_vals, rel_vals

arguments = sys.argv[1:]

if len(arguments) == 0:
    n = 64
    res_vals, rel_vals = read_resrel_bin ("./partitions/partitions_of_" + str(n) + "_resrel.bin")
elif len(arguments) == 1:
    n = int(arguments[0])
    res_vals, rel_vals = read_resrel_bin ("./partitions/partitions_of_" + str(n) + "_resrel.bin")
elif len(arguments) == 3:
    n = int(arguments[0])
    res_left_bound = float(arguments[1])
    res_right_bound = float(arguments[2])
    # filename = "partitions/[" + str(res_left_bound) + "," + str(res_right_bound) + "]_partitions_of_" + str(n) + "_resrel.bin"
    # filename = "partitions/[ f'{res_left_bound:g}, f'{res_right_bound:g}]_partitions_of_" + str(n) + "_resrel.bin"
    filename = "partitions/[" + f'{res_left_bound:g}' + "," + f'{res_right_bound:g}' + "]_partitions_of_" + str(n) + "_resrel.bin"
    res_vals, rel_vals = read_resrel_bin (filename)
else:
    print("Error: Too many arguments")
    sys.exit(1)


# Print plot
plt.scatter(res_vals, rel_vals, color='blue', alpha=0.7, marker=".", s=2)
plt.xlabel('Resolution')
plt.ylabel('Relevance')
plt.title('n = ' + str(n) + "  N. Partitions = " + str(len(res_vals)))
plt.grid(True)
plt.show()
