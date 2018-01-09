import zipfile
import os
import time
import sys

def compress(src, dst):
        origin_src = src
        archive = zipfile.ZipFile(dst, 'w', compression=zipfile.ZIP_DEFLATED)
        if os.path.isfile(src):
            archive.write(src, )
        elif os.path.isdir(src):
            dir_retrieve(archive, origin_src, src)
        else:
            sys.exit(1)
        archive.close()

def dir_retrieve(archive, origin_src, inner_dir): 
    for element in os.listdir(inner_dir):
        element_path = os.path.join(inner_dir, element)
        if os.path.isfile(element_path):
            archive.write(element_path, element_path[len(origin_src):])
        elif os.path.isdir(element_path):
            dir_retrieve(archive, origin_src, element_path)
        else:
            sys.exit(1)

def main(argv):
    if len(argv) != 2:
        print("Usage : compress.py [src] [dst]")
        sys.exit(1)

    compress(argv[0], argv[1])

if __name__ == "__main__":
    main(sys.argv[1:])