# Install TML for Python {#tml_install}

### Get libTML Core library ###

TML for Python RAD requires the [libTML core library](https://github.com/tml21/libtml-c) to be installed. Get the source of the core library and follow the instructions of the [libTML-c installation guide](http://www.libtml.org/docs/libtml-c-html/tml_install.html).

### Get libTML for Python ###

Download the [libTML-python source package from GitHub](https://github.com/tml21/libtml-python) and extract it.

## Prerequisites ##

TML for Python uses *distutils* to build and install the libraries. Compiling the Python Extension to access the libTML core library requires a compiler. For installation on Linux use gcc and on Windows Visual Studio.

### Working with libTML-c binary packages ###

If you use the libTML-c binary packages you have to take care about the runtime environment.<br>
Please refer to the "[Install libTML-c binary package](http://libtml.org/docs/libtml-c-html/tml_bin_install.html)" documentation for details.

It may be necessary to configure the system using the right compiler and runtime environment.<br>
For example on FreeBSD:

	setenv CC gcc49
	setenv CXX g++49
	setenv LD_LIBRARY_PATH /usr/local/lib/gcc49

## Installation ##

Go to the root directory of the libTML-python source package and call the `setup.py` script with the `install` command.


    python ./setup.py install

The package is installed for the Python version used to call the script. If multiple Python versions are installed, repeat this step for each.

If the installation fails because of library dependencies while compiling the extension modules `_tml_ext` and `_sidex_ext`, modify the paths in `setup.py` to match the location of the libTML-c files. Modify `include_dirs` and `library_dirs`. 

After successful installation try to import the extensions. Make sure the libTML-c shared libraries or dlls are installed and can found while loading the extension modules. 

    import sidex
    import tml  

If the import doesn't raise an error, all libraries are in place and the module can be uses. Read the [Introduction to TML for Python](tml_intro.md) to learn what to do next.




