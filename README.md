glint-ui
========

A graphical frontend for emulators with controller navigation. Developed both on and for the Raspbery Pi. Intended for use with **glint**-flavored projects (such as [glint-nes](https://github.com/normalocity/glint-nes)), but it can easily be used with other emulators.

I stand on the shoulders of giants (see CREDITS file). What contributions I have made are simply refinements to an already beautiful dream.

State of the Code
=================

Lately I've been making more and more ambitious changes to the code. **glint-ui** was originally called **glint-es** and at that time it was based on (in fact a near copy of) Aloshi's EmulationStation.

I'm in the middle of refactoring the entire code base, one piece at a time as I encounter them. Because of this you're going to see a few things you should be aware of:

* There are many code styles being simultaneously used throughout the code. This is a side effect of changing from one style to another, and I'm not really sure when the code base will be consistent. When wondering what the proper code style for the project is your best bet is to look at the most most recently changed code is and go with that style for future changes, while refactoring code that you run into that doesn't meet that style as you go along. I will eventually setting on a single style and work toward bring everything under that one style.
* There are components of the code that are left over from EmulationStation that are no longer necessary or even being used in many places. If you find one of these and you feel like removing it, by all means let me know and ask if you can jump in - I'm all for it.

Building
========

**On the Raspberry Pi:**

glint-es has a few dependencies, so kindly install them:
```
sudo apt-get install libsdl1.2-dev libboost-filesystem-dev libfreeimage-dev libfreetype6-dev
```

There are also a few libraries already on the RPi (located in /opt/vc/, like the Broadcom libraries, EGL, and GLES).

You can build glint-es by simply running `make` from the `glint-es` directory. You should get an executable (also called **glint-nes**) runs the frontend.

**On something else (desktop):**

glint-es can also be built on a "normal" Linux system. You'll need the same libraries listed above:
```
sudo apt-get install libsdl1.2-dev libboost-filesystem-dev libfreeimage-dev libfreetype6-dev libsdl-mixer1.2-dev
```

You'll also need OpenGL. I don't know the proper package name, but you'll need `/usr/include/GL/gl.h` and `libGL`. You probably already have it. You can build with `make -f Makefile.x86` (badly named Makefile, I know).

Configuring
===========

**~/.glint-es/es_systems.cfg:**
When first run, an example systems configuration file will be created at $HOME/.glint-es/es_systems.cfg. This example has some comments explaining how to write the configuration file, and an example RetroArch launch command. Keep in mind you can define more than one system! Just use all the variables again. Also, you can use multiple extensions - just separate them with a space, e.g.: ".nes .NES .bin".

If an SDL Joystick is detected at startup, and $HOME/.glint-es/es_input.cfg doesn't exist, an Input Configuration screen will appear instead of the game list.
This should be pretty self-explanatory. If you want to reconfigure, just delete $HOME/.glint-es/es_input.cfg.

Mappings will be applied to the first joystick with the same name as the joystick you configured.
An Xbox 360 controller with the xboxdrv driver was used for testing. POV hats are automatically mapped to directions (so if you're not using an analog stick, you'll need to skip mapping Up/Down/Left/Right by pressing a keyboard key).

**Keep in mind you'll have to set up your emulator separately from glint-es.**
If you're using RetroArch, a handy input config generation tool can be found in the tools/ subdirectory - you can use it with `retroarch-joyconfig -o ~/.retroarch.cfg` or something similar. You may need to tell RetroArch to load this config file with `-c ~/.retroarch.cfg` in your RetroArch launch commands.

glint-es will return once your system's command terminates (i.e. your emulator closes).


**Keyboard mappings:**

    Up - Scroll up
    Down - Scroll down
    Left - Last system (if it exists)
    Right - Next system (if it exists)
    Enter - Select
    Escape - Back out of a folder
    F1 - Open the restart/shutdown system menu
    F2 - Open the fast select dialog
    F4 - Close glint-es (should work as long as ES hasn't frozen)

Unfortunately, there is no built-in way to change keyboard mappings - if you need to, check out `src/InputManager.cpp`. There's a switch statement with a list of keys; it should be pretty simple to change them.

What, you want a config file so you can change it without recompiling? Pish posh - I'm working on it.

gamelist.xml
============

**glint-es** does away with the XML games list, preferring simply to store all ROMs in a known directory.

Themes
======

Being based wholly on EmulationStation, glint-es is themed in the same way, should you prefer to theme it with your custom look and feel.
