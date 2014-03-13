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
