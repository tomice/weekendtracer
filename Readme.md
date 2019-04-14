# Weekend Raytracer

## About

This repo is mostly following along with the famous Ray Tracer in One Weekend  
by Peter Shirley. I am slowly refactoring it, adding/removing from it, and  
just having fun with it as I learn more about ray tracing in general.

This particular example will output a PPM file which can be read by any  
multitude of PPM readers out there. For OS X, Peter originally recommended  
ToyViewer. I used OpenSeelt on Windows 10. Gimp is always an option for just  
about any platform, too.

## Build

This should be able to compile on just about anything that has a C++ compiler  
written after like 1998 or something. No special libraries are needed for this.  
However, it will not compile cleanly at the moment on Linux/BSD-based systems.  
I'll eventually go and clean the errors later. It's mostly rval/lval stuff and  
redefinitions of things that exist on the target system already.

## License

Peter Shirley released the original Ray Tracing series under the Creative  
Commons license. Given how closely this code resembles the original work  
(I don't take credit for this code; I'm simply using his tutorial as a learning  
process), I'm using the same license.

## Authors

Original code written by Peter Shirley  
Reimplemented and modified by Tom Ice