# Contributing
This will most likely be a solo project, but if you wish to help out with
the game's development, there are multiple ways of doing so! Everything
from reporting bugs to posting suggestions can be best done at the public
repositiory on [Github](https://github.com/ProarchwasTaken/tld_skirmish)

If you wish to make more substancial contributions like fixing bugs, and
making optimizations yourself. You are free to fork the project, and open
a pull request. If the changes aligns with the project's code conventions,
and fits what I have in mind for the game, I'll most likely accept it.

What rest of this section details the code conventions of the project, and
how to compile the project yourself.

## Dependencies
You're gonna need these installed if you want to compile the game's source
code.

* C++17 along with g++ and clang++ compiler. However if you're not 
  planning to build the game for release, you only need clang++ 
* CMake to generate the MakeFile. Anything above version 3.10 should be
  fine
* The Raylib 5 library.
* The toml11 library
* plog v1.1.10, which is a logging library.

## Setting up the Build System 
Once you have forked and cloned the project. The first step would be to
create a folder for CMake to use as the build directory. You should
name it something like "build", but if you coding for multiple platforms,
I advise creating multiple folders, but it's up to you on how you want to
do this.

First step is generating the makefile using CMake, you can do that by
using this command at the project's root directory.

`cmake -S . -B ./build -DBUILD_DEBUG=ON`

If that doesn't work, you may have to specify what type of Makefile would
be generated. For me, it's "MinGW Makefiles"

`-G "MinGW Makefiles"`

The last step should be compiling the project. Enter the project's build
directory and use the makefile to compile the project into a executable
the will be outputted into the game's root directory if things go as they
should.

Here a nice trick you can do. You could run this command to compile the
source code and run the outputted executable without leaving the root
directory. It's what I use all the time to quickly compile and test the
game. Although it may have to be edited slightly if you work on linux.

`make -C build\ && .\game.exe --debug`

## Coding Conventions
List of guidelines when it comes to programming for the project. More will
probably be added later, and some rules may be subject to change.

* If you're planning to allocate memory from the heap. Please do this
  through the use of smart pointers to reduce the chances of memory leaks.
  Although, using raw pointers that point to memory allocated from the 
  stack, is fair game. 

  Having a raw pointer point to heap allocated memory that's owned by a 
  smart pointer is fine as well.

* The project's naming pattern is as follows: snake_case for variables,
  MACRO_CASE for constants and preprocessor macros, camelCase for 
  functions/methods (Pretty much anything that's callable), and PascalCase
  for classes/namespaces.

* All lines of code must not be longer than 74 characters. The reason for
  this is my monitor only has enough width for 2 vertical splits of 74
  characters each. Because this is more of a personal convenience thing,
  this rule is not as strongly enforced as the other ones.

* The way you should write code for this project is about making it more 
  human and readable. Avoid writing too many comments. Docstrings should
  only be written in header files and should be used for explaining WHY 
  something is like this instead of only what's it's doing. Good
  documentation always helps.

  If you want to better understand what I'm talking about, you can watch
  this [video.](https://www.youtube.com/watch?v=Bf7vDBBOBUA&t) It's really
  enjoyable to watch, and it's one of my most favorite videos. You could
  also just, y'know... read the code itself to understand how I 
  programmed everything.

