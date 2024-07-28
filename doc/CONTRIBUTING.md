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


# Dependencies
You're gonna need these installed if you want to compile the game's source
code.

* C++17 along with g++ and clang++ compiler. However if you're not 
  planning to build the game for release, you only need clang++ 
* CMake to generate the MakeFile. Anything above version 3.10 should be
  fine
* The Raylib 5 library.
* The toml11 library
* plog v1.1.10, which is a logging library.


# Setting up the Build System 
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

`make -C build\ && .\skirmish.exe`

There may be times during the game's development where the game would not
be able to ran normal due to the starting scene not being implemented yet. 
To circumvent this, you can start the game into the debug scene using
the following command line argument.

`--debug_scene`

You could also use `-v` to enable verbose logging statements.


# Coding Conventions
List of guidelines when it comes to programming for the project. More will
probably be added later, and some rules may be subject to change.

* If you're planning to allocate memory from the heap. Please do this
  through the use of smart pointers to reduce the chances of memory leaks.
  Although, using raw pointers that point to memory allocated from the 
  stack, is fair game. 

  Having a raw pointer point to heap allocated memory that's owned by a 
  smart pointer is fine as well.

* The project's naming pattern is as follows: snake_case for variables,
  MACRO_CASE for constants, global variables, and preprocessor macros, 
  camelCase for functions/methods (Pretty much anything that's callable), 
  and PascalCasefor classes/namespaces.

* Try to avoid nesting if statements whenever possible. Something like 
  this could be avoided through the use of guard clauses, or separating
  one function multiple separate ones. If it's truly unavoidable, you may
  want to restructure your code, or consider using inline\short hand if
  statements.

* All lines of code must not be longer than 74 characters. The reason for
  this is my monitor only has enough width for 2 vertical splits of 74
  characters each. Because this is more of a personal convenience thing,
  this rule is not as strongly enforced as the other ones.

* Most if not all global variables must be declared and defined within
  namespaces to avoid name mangling. Only declare global variables when
  you absolutely have to, and only when you intend for that variable to
  be used all across the program.

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


# Contributing Art
There's a couple things you need to know if you wish contribute to the 
project in the form of art for stages, enemies, effects, etc. More will
be added as we go.

All art must adhere to the game's color palette. It's basically the NES 
color palette. You can view the palette in the "graphics" directory. The 
only exception is art made purely for the purpose of debugging and 
testing. (Something that not gonna be seen in normal play.) Depending on
what program you use, you may have to disable anti-aliasing or 
interpolation to avoid accidentally breaking this rule. 

What program you use is completely up to you. Personally, I use 
Asesprite for the pixel art, and paint.net/gimp for the more
complex stuff like the game's stages. Speaking of which, there are 
multiple templates you could use as a reference for how things are done.


# Commits Guidelines.
Obviously, commit messages should be detailed and helpful. Commit messages
should detail what you just changed, and why you did it. Non-descriptive
commit messages are generally not accepted.


# About the Linux version.
As of July 3rd of 2024, I will be putting most of my focus into making
the game for Windows. The Linux version simply has too many issues that I
just not comfortable releasing this version of the game at it's current
state. I'd rather not waste time bashing my head against something that's
essentially a black box with my current knowledge.

It's not like I completely abandoned it. I will still make sure the game
at least runs on Linux. However, that's just the bare minimum.

## Linux Issues
Please note that I use the WSL to test the game on linux, so it could
possible that these issues aren't present in actual distros. If you're 
using an proper distro, I would really appreciate it if you tried the game
on linux, and let me know how it turns out. Like if there are more issues,
or maybe you found the solution to fix it!

* Controller support not working.

* Problems with resizing the window. Most notably with trying to change
  the height.

* The Audio Device failing to initialize. Which causes the game to have
  no audio. This was the issue the caused me to stop maintaining the
  linux version once I discovered it.

* Borderless fullscreen is broken. Every time I try to go fullscreen, the
  game throws an warning along the lines of: "Wayland: The platform does 
  not provide the window position". The only reason I could assume this 
  warning could be thrown is due to the calling of "correctWindow()", 
  which is a method of the game class. 

  When the method is called, it first gets the current width and height of 
  the window. Which I assume is what caused the warning. The method is 
  supposed to be called whenever the window gets resized or enters
  borderless fullscreen. It seems like for some reason the game doesn't
  detect when the window is resized on linux.

  EDIT: After some more testing it seems the warning is because 
  ToggleBorderlessWindowed is called, not because of correctWindow().

Again, I would like to state that this could all be attributed to me using
WSL, and most of these issues are present on actual distros.
