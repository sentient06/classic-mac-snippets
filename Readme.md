# Classic Mac Snippets

This is just a collection of self-contained snippets compilable on Mac OS 9's CodeWarrior and possibly other compilers. I created this repo to keep some stuff in version control while I work on projects that need these.

The files are either in C or C++, with or without the Macintosh Toolkit. The idea is for these to be imported into C/C++ projects, so the C headers should have "extern C" directives to disable name mangling and make them compatible with C++.

I haven't really done any pure C using the Macintosh Toolkit, but if I do, I'll add a dir for that.

All code has been only tested on PowerPC instructions, on Mac OS 9.2.2. I recommend not using "bundles" (Application Package or Shared Library Package), as they can mess up some functionality. Use standalone executable binaries instead.

Git will remove the resource fork and extended attributes of the Classic Macintosh files, so if you want to restore these to open on CodeWarrior automatically, the type is `TEXT` and the creator is `CWIE`. (Use FileType or similar to change these attributes.)
