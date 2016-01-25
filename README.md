cavr
====

A fancy VR library.

The cavr demo is in examples/app2. When you have setup up cavr in the build file make sure to run the example in build/examples/app2.

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
    sudo apt-get install -y libgflags-dev libgoogle-glog-dev protobuf-compiler libprotobuf-dev swig libzmq-dev liblua5.1-dev cmake
    
    # now to get vrpn setup
    git submodule init
    git submodule update
    
    # now to build and make vrpn
    cd vrpn
    mkdir build
    cd build
    cmake ..
    make -j 8
    
    # now install vrpn (make sure you have sudo rights)
    sudo make install
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

    # now lets install cavr -- run this if you have rights to
    make install # this will install in your /usr/local/lib and /usr/local/include

    # or you could install for all users
    # sudo make install -- up to you 

    ```

3.  **Lets run some cavr examples**

    ** Configuring cavr to run some examples. ** 
    Now that cavr is installed we should be able to run some examples.
    To run these examples you are going to need to a few things depending on how cavr was built and/or installed.
    
    If you went the make install route where you install cavr into /usr/local/lib, then you will need to set some environemt     variables to run cavr. You will need to set the CAVR_PATH, CAVR_PLUGIN_PATH, and possibly CAVR_ETC_PATH. The CAVR_PATH      and CAVR_ETC_PATH are where all the configuration and schema files are stored. The CAVR_PLUGIN_PATH is where CAVR's         plugins are stored. 
    
    Here is a example bash script that sets this path environment variables:
    ```bash
    CAVR_PATH=path to cavr (exp. ~/Desktop/cavr/build/cavr)
    CAVR_PLUGIN_PATH=path to cavr plugins (exp. ~/Desktop/cavr/build/lib/plugins 
    CAVR_ETC_PATH=path to cavr's schemas and configs (exp. ~/Desktop/cavr/etc)
    export CAVR_PATH
    export CAVR_PLUGIN_PATH
    export CAVR_ETC_PATH
    ```
    
    This will set your paths for a current terminal session. If you want the variable changes to be set for when you are        logging in, then add these few lines of bash to the bottom of your .bashrc at ~/.bashrc.
    
    ** Running the code **
    
    There is one example provide with cavr that can be used as a starting point.
    
    Example two or app2 in the examples directory has directions on how to run it. 
    
    Please see the examples/app directory for more information on how to run the examples.
    
# Using VRPN with CAVR

This section will introduce how you incorporate VRPN devices into CAVR.

To be filled soon.

# Adding more plugins into CAVR

To be filled soon. 
    
    
