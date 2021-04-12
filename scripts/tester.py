import sys
import subprocess
import os


def txt2dict(path):
    dct = {}
    with open(path) as f:
        for l in f.readlines():
            key, val = l.strip().split(" ")
            dct[key] = val
    return dct


def run_one_time(cfg_path, exe_path, res1_path, res2_path):
    if os.path.exists(res1_path):
        os.remove(res1_path)
    if os.path.exists(res2_path):
        os.remove(res2_path)
    result = subprocess.run([exe_path, cfg_path], stdout=subprocess.PIPE).stdout
    total, read, index = map(lambda x: float(x.split()[1]), result.decode("utf-8").strip().split("\n"))
    dct1 = txt2dict(res1_path)
    dct2 = txt2dict(res2_path)

    return dct1, dct2, total, read, index


def check_n_times(n, cfg_path, exe_path):
    with open(cfg_path) as f:
        lines = f.readlines()
        res1_path = lines[1].strip()
        res2_path = lines[2].strip()

    mn_total = float("inf")
    mn_read = float("inf")
    mn_indexing = float("inf")

    res1 = res2 = None
    check = True

    for i in range(n):
        dct1, dct2, total, read, index = run_one_time(cfg_path, exe_path, res1_path, res2_path)
        mn_total = min(mn_total, total)
        mn_read = min(mn_read, read)
        mn_indexing = min(mn_indexing, index)

        if i > 0 and (dct1 != res1 or dct2 != res2):
            check = False
        res1 = dct1
        res2 = dct2

    return mn_total, mn_read, mn_indexing, check


if __name__ == "__main__":
    if len(sys.argv) != 4:
        raise ValueError("Invalid number of arguments")
    exe_path = sys.argv[1]
    cfg_path = sys.argv[2]
    n = int(sys.argv[3])
    total, read, index, check = check_n_times(n, cfg_path, exe_path)
    print(f"Minimal total time: {total}")
    print(f"Minimal read time: {read}")
    print(f"Minimal indexing time: {index}")
    if check:
        print("All returned values are the same")
    else:
        print("Some two returned dictionaries are different")
