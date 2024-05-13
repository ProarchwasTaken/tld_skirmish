# Synopsis
**True Human Tribulation: Skirmish** is a wave-based fighter where you
have to survive against multiple waves of enemies. The game features 
controls simple enough that you could play with just an SNES controller.
As of now, the game is available on Windows and Linux.


# Controls
This section details the game's controls. I trust that you could figure
out the rest.

## Keyboard

### Menu
* **Arrow Keys** - Menu Navigation
* **Z Key** - Confirm
* **X Key** - Cancel/Go Back

### Gameplay
* **Arrow Keys** - Movement
* **C Key** - Light Attack
* **V Key** - Heavy Attack
* **X Key** - Guard
* **Z Key** - Special/Sub-Weapon

## Gamepad
The controls detailed here are based on the SNES button layout, but I'm
sure it would work with other controllers.

### Menu
* **D-Pad** - Menu Navigation
* **A Button** - Confirm
* **B Button** - Cancel/Go Back

### Gameplay
* **D-Pad** - Movement
* **A Button** - Light Attack
* **B Button** - Heavy Attack
* **Y/R Button** - Guard
* **X/L Button** - Special/Sub-Weapon


# Credits and Contacts

## Tyler Dillard
Role: Programmer, Designer, Pixel Artist, Sound and Music Designer, pretty
much everything.

**Contact Email** - lorenarcdev@gmail.com

**Discord** - proarch. (With the period)

**Itch.io** - https://lorenarc.itch.io

**Gamejolt** - https://gamejolt.com/@LorenArc

**Twitter** - https://twitter.com/Tyler37961642

**Dev Channel** - https://www.youtube.com/channel/UCzR-5I3KE2YAV-Ilndl8Y9w


# Known Issues
This is the list of known issues about the game.

## Windows
As of now, there aren't any known issues on windows. Everything seems to
work the way it's supposed to, but that may change later down the line.

## Linux
Please note that I use the WSL to test the game on linux, so it could
possible that these issues aren't present in actual distros. If you're 
using an proper distro, I would really appreciate it if you tried the game
on linux, and let me know how it turns out. Like if there are more issues,
or maybe you found the solution to fix it!

* Controller support not working.

* Problems with resizing the window. Most notably with trying to change
  the height.

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
