cavr
====

A fancy VR library.

#Core Dependencies

In addition to a compiler that supports C++11, cavr uses the following tools and headers to build. Depending on your OS, some of these tools can be installed through a package manager.

* [cmake](http://www.cmake.org/): Build environment
* [Lua5.1](http://www.lua.org/): Used for configuration. Requires headers and libraries to build.
* [google-glog](https://code.google.com/p/google-glog/): Used for logging. Requires headers and libraries to build.
* [gflags](https://code.google.com/p/gflags/): Used by google-glog as well as to simplify command-line flags. Requires headers and libraries to build.
* [swig](http://www.swig.org/): Used to create an interface for Lua configuration files.


#Installation
Cavr has currently only been compiled on Ubuntu 14.04 (Let us know if you get it to work on another OS). 
You will need to get the dependencies list above to compile cavr, but the instructions will go over how to install them in a automatic way with a few manual things to do.

1.  **Getting the dependencies:**

    Here are a few lines of bash to acquire the dependencies for Ubuntu 14.04:
    ```bash
    sudo apt-get install -y libgflags-dev libgoogle-glog-dev protobuf-compiler libprotobuf-dev swig libzmq-dev liblua5.1-dev     cmake
    ```
    You will need sudo preveileges to install these packages. 
    With these packages now installed you should be able to compile cavr.

2. **Compiling and Installing cavr**

    In order to build cavr you will need to use cmake to have it build a bunch of makefiles. Once cmake has made the files,     you will be able to compile cavr and install it. Here are a few lines of bash:

    ```bash
    # Change to cavr directory
    cd cavr

    # make a build directory -- This is the author's personal preference for cmake.
    mkdir build

    # Now lets change to the build directory and make some build files
    cd build
    cmake .. # making the build files

    # Time to compile cavr
    make -j 4 # If you want cavr to compile fast, I highly suggest a -j # to have make run parallel jobs.

    # now lets install cavr -- 
    make install # this will install in your /usr/local/lib and /usr/local/include

    # or you could install for all users
    # sudo make install -- up to you 

    ```

3.  **Lets run some cavr examples**

    Now that cavr is installed we should be able to run some examples.

    This section will be filled in soon.
