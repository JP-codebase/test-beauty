import matplotlib.pyplot as plt
import struct


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
                res_vals.append(res)
                rel_vals.append(rel)
                i = 0
            else:
                record = file.read(record_size)
                if len(record) < record_size:
                    break
                i = i + 1

    return res_vals, rel_vals

n = 64
res_vals, rel_vals = read_resrel_bin ("./partitions/partitions_of_" + str(n) + "_resrel.bin")

# Print plot
plt.scatter(res_vals, rel_vals, color='blue', alpha=0.7, marker=".", linewidths=0.7)
plt.xlabel('Resolution')
plt.ylabel('Relevance')
plt.title('n = ' + str(n) + "  N. Partitions = " + str(len(res_vals)))
plt.grid(True)
plt.show()
