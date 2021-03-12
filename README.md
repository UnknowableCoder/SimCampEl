# SimCampEl
A fully relativistic 3D electromagnetic simulator, with graphical representation!

# Requirements
The code requires a C++ compiler. Though no "modern functions" are used in the code, at least C++11 compatibility seems like a safe requirement.

The graphical representation is written using the [Irrlicht Engine](http://irrlicht.sourceforge.net/).

The code also requires the [fparser library](http://warp.povusers.org/FunctionParser/) for parsing user-defined electric and magnetic fields.

# Compiling

Provided that the above requirements are met and the compiler knows where to find the relevant libraries, simply compile `SimCampEl.cpp`.

(The authors apologize for the lack of good practices in including `.cpp` files directly in the code, though at the time of the writing their C++ knowledge was still maturing.)


# Acknowledgements

This work builds on the functionalities provided by the Irrlicht library, which itself is based in part on the work of the Independent JPEG Group, the zlib and the libPng.
