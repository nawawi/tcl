
What's here
===========
  The official ZLIB1.DLL


Source
======
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
  zlib version 1.2.8
=======
  zlib version 1.2.11
>>>>>>> upstream/master
=======
  zlib version 1.2.11
>>>>>>> upstream/master
=======
  zlib version 1.2.11
>>>>>>> upstream/master
  available at http://www.gzip.org/zlib/


Specification and rationale
===========================
  See the accompanying DLL_FAQ.txt


Usage
=====
  See the accompanying USAGE.txt


Build info
==========
  Contributed by Jan Nijtmans.

  Compiler:
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
    i686-w64-mingw32-gcc (GCC) 4.5.3
  Library:
    mingw64-i686-runtime/headers: 3.0b_svn5747-1
=======
    i686-w64-mingw32-gcc (GCC) 5.4.0
  Library:
    mingw64-i686-runtime/headers: 5.0.0
>>>>>>> upstream/master
=======
    i686-w64-mingw32-gcc (GCC) 5.4.0
  Library:
    mingw64-i686-runtime/headers: 5.0.0
>>>>>>> upstream/master
=======
    i686-w64-mingw32-gcc (GCC) 5.4.0
  Library:
    mingw64-i686-runtime/headers: 5.0.0
>>>>>>> upstream/master
  Build commands:
    i686-w64-mingw32-gcc -c -DASMV contrib/asm686/match.S
    i686-w64-mingw32-gcc -c -DASMINF -I. -O3 contrib/inflate86/inffas86.c
    make -f win32/Makefile.gcc PREFIX=i686-w64-mingw32- LOC="-mms-bitfields -DASMV -DASMINF" OBJA="inffas86.o match.o"
   Finally, from VS commandline (VS2005 or higher):
    lib -machine:X86 -name:zlib1.dll -def:zlib.def -out:zdll.lib

Copyright notice
================
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
  Copyright (C) 1995-2010 Jean-loup Gailly and Mark Adler
=======
  Copyright (C) 1995-2017 Jean-loup Gailly and Mark Adler
>>>>>>> upstream/master
=======
  Copyright (C) 1995-2017 Jean-loup Gailly and Mark Adler
>>>>>>> upstream/master
=======
  Copyright (C) 1995-2017 Jean-loup Gailly and Mark Adler
>>>>>>> upstream/master

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Jean-loup Gailly        Mark Adler
  jloup@gzip.org          madler@alumni.caltech.edu

