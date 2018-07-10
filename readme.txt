//////////////////////////////////////////////////////////////////////
// MarSTD version 2004 - (c)2004 - Marcel Smit                      //
//                                                                  //
// Marcel_Smit_101@hotmail.com                                      //
// marcel.athlon@hccnet.nl                                          //
//                                                                  //
// This code may not be used in a commercial product without my     //
// permission. If you redistribute it, this message must remain     //
// intact. If you use this code, some acknowledgement would be      //
// appreciated. ;-)                                                 //
//////////////////////////////////////////////////////////////////////

MarSTD v2004 README FILE
------------------------

[See the documentation for more information]

__________
Installing

  - Copy the directory marstd2/ to your include/ directory.
    The directory marstd2/ includes all headers and sources to use MarSTD.

  - You're done. :)

_____
Using

  A> #include <marstd2/marstd.h>
  B> #include <marstd2/marstd.cpp>

  A> Add this line to every source file that will be using MARSTD.
  B> Add this line exactly once in your entire project.

  The implementation of MarSTD is for a large part inside the inline header files. Some
  classes have code in the marstd.cpp file though. To use these classes, you will
  need to include marstd.cpp too. Be carefull to include this file only once!
  Like said before. some classes do not require you to include the marstd.cpp file.
  It is best not to rely on this, since it may change in the future. It is best to
  always include the .cpp file somehow.

  It may be wise to include the .cpp file inside one of your source files that is
  rarely compiled, or a seperate file for MarSTD only. Eg:

  -- begin marinc.cpp --

    #include <marstd2/marstd.cpp>

  -- end marinc.cpp --


Removing:

  Simply delete the directory include/marstd2/.


Updating:

  Copy the contents of an updated MarSTD librairy to include/marstd2/ and
  overwrite all files. Make sure to compile older projects using 'rebuild all'.


Compiling:

  Compiling the test program can be done using Dev-C++. Just open the .dev project
  file and hit F9 to run.

  See the readme.txt file in the examples/ directory for information on compiling the examples.
