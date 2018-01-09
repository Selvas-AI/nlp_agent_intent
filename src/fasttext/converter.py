import sys

from tqdm import tqdm

def main(argv):
    src_path = argv[0]
    dst_path = argv[1]

    with open(src_path, mode='r', encoding="utf-8") as file:
        lines = file.readlines()
        line = lines[0]

        count = len(lines)
        dim = len(line.strip().split()) - 1

    with open(dst_path, mode='w', encoding="utf-8") as output_file:
        output_file.write(str(count) + ' ' + str(dim) + '\n')
        with open(src_path, mode='r', encoding="utf-8") as input_file:
            for line in tqdm(input_file.readlines()):
                output_file.write(line.strip() + '\n')


if __name__ == "__main__":
    main(sys.argv[1:])