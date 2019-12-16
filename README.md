## kpl

kpl is intended as my own personal C++ library where I can keep and maintain a reusable layer of the code that I use in projects.

Kpl is given as a core library, and a number of submodules. Each submodule is built into a self-contained library file so that you only need to link against what you need in your own project.

By default stl types are used, however you can use Qt types instead by setting the KPL_USE_QT variable in CMakeLists.txt. kpl uses type generic headers and setting this option causes the build system to link against qt versions of a submodule (Where available). This is convienent if working on a Qt project as it reduces the amount of unnessecary copying between application and library boundaries. 

How to build

```shell
    git clone https://github.com/kdchambers/kpl
    cd kpl
    mkdir build && cd build
    cmake ..
    make
```

Alternatively, build.sh is a simple helper script that builds the project as shown above.

```shell
    git clone https://github.com/kdchambers/kpl
    cd kpl
    ./build.sh
```

Building the project as shown above will create the following files

Library files

* kpl/lib/libkpl.so
* kpl/lib/libkpl_static.a
* kpl/lib/libkpl-reflection_static.a

Binary files 

* kpl/bin/tests
* kpl/bin/benchmarks

Disclaimers:

* kpl is only built / tested on Linux, although source code is cross platform

