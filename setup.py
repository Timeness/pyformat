from setuptools import setup, find_packages, Extension
from pybind11.setup_helpers import Pybind11Extension, build_ext
from pybind11 import get_cmake_dir
import pybind11

ext_modules = [
    Pybind11Extension(
        "pyformat._formatter",
        ["src/formatter.cpp"],
        include_dirs=[pybind11.get_include(), "src/"],
        language='c++'
    ),
]

setup(
    name="pyformat",
    version="1.0.0",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    packages=find_packages(),
    zip_safe=False,
    python_requires=">=3.7",
)
