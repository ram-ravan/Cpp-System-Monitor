# Linux-System-Monitor

This Linux System Monitor is an inspiration from htop, a popular system monitor. This project code is written in C++ programming language using its Object Oriented Programming features.

![System Monitor](images/monitor.png)

## ncurses
[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output.

Install ncurses within your Linux environment: `sudo apt install libncurses5-dev libncursesw5-dev`

## Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

## Instructions to clone and execute the code

1. Clone the project repository: `git clone https://github.com/ram-ravan/Linux-System-Monitor.git`

2. To compile the project, first, create a `build` directory and change to that directory:

            ```mkdir build && cd build ```
            
            From within the `build` directory, then run `cmake` and `make` as follows:

                ```cmake ..
                   make```

2. Build the project: `make build`

3. Run the resulting executable: `./build/monitor`
![Starting System Monitor](images/starting_monitor.png)