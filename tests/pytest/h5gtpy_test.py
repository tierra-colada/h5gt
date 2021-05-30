import sys

sys.path.append('.')

import pybind11_stl_test.cp39-win_amd64

if len(sys.argv) > 1:
    blah = sys.argv[1]
else:
    raise Exception("You have to pass dir containing h5gtpy module")

sys.path.append(sys.argv[1])

import h5gtpy as h5gt



# h5gtpy_dir = sys.argv[1]
# if h5gtpy_dir

# from pathlib import Path
#
# for path in Path('src').rglob('*.c'):
#     print(path.name)
#
# import sys
#
# sys.path.append('.')
#
# import h5gtpy