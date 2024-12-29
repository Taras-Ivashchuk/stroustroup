This project has been built on Ubuntu 24.04.  Code has been formatted using clang-format program
with style LLVM.

### Pre-requisites:
 * Make sure cmake is installed. If not use `sudo apt install cmake `.
 * You have to have fltk installed system-wide. Use following instructions to install it. Install
    [dependencies](https://fltk-rs.github.io/fltk-book/Setup.html). Clone the repo [fltk](https://github.com/fltk/fltk).
    Go to cloned repo. Then issue commands:
     * cmake -B build 
     * cmake --build build
     * sudo cmake --install build

### Building
 * Change the project name in the CMakeLists.txt to your project's name, that
 coressponds to the name of the main source cpp file.
 * Go to build directory.
 * Command cmake ..
 * Command make
 * Run the program from the build directory
    ./testdemo 
