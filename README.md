# MSP GAMES 👾

![demo of the game snake on the MSP432](https://i.imgur.com/uURFLyd.jpg)

## What this is

This is my final project for the course _Embedded Software for the Internet of Things_, held in University of Trento by professor Kasım Sinan Yıldırım during academic year 2020/2021.  
Its goal is to showcase some of the funcitonalities offered by the microcontroller MSP432 by implementing some simple games whilst using the numerous sensors/peripherals available on the platform.

## What this system does

The user can play two games (one of which is snake) using the joystick to control direction. At any time, games can be paused by pressing the button and there is audible feedback on win/loss through the in-built buzzer. Finally, two color schemes are available (light and dark) and the platform can switch automatically between them based on measured ambient light.

## High level software architecture

<p align="center">
<img height="400em" src="https://i.imgur.com/YQALROQ.png" alt="Software description diagram">
<p>

The program is structured as a infinite loop, which starts after some hardware initialization.  
First, a menu screen is shown. Here the player can decide which game he will play and change the color theme settings. Available options are:

- **L** for light mode
- **D** for dark mode
- **A** for automatic mode (uses ambient light sensor to decide wether to show the light or dark version)

Once a game has been chosen, that game is rendered on the screen and the user can play it.
If the user presses the button while playing a game, a pause screen appears and the microcontroller is put in low power mode to save energy. Pressing the button again resumes the game.  
After the game has ended, a won/lost screen is shown, the buzzer plays one of two tunes according to the result and the controller is put, again, in low power mode. Pressing the button starts a new iteration of the loop, so that the player can continue with another match.

## Demo images (dark mode)

| Menu screen                                           | Game 1                                           | Snake                                                |
| ----------------------------------------------------- | ------------------------------------------------ | ---------------------------------------------------- |
| ![Menu screen image](https://i.imgur.com/trSFoyz.png) | ![Game 1 image](https://i.imgur.com/cXbSXhc.png) | ![Snake game image](https://i.imgur.com/GuHnLiy.png) |

| Pause screen                                           | Lost screen                                               | Choose maximum score screen                                           |
| ------------------------------------------------------ | --------------------------------------------------------- | --------------------------------------------------------------------- |
| ![pause screen image](https://i.imgur.com/RftvzYU.png) | ![won/lost screen image](https://i.imgur.com/sRJaipl.png) | ![choose maximum score screen image](https://i.imgur.com/S6f2Nm9.png) |

## Hardware platform

This project has been developed for the [MSP432 board](https://www.ti.com/product/MSP432P401R) and the [BoosterPack MKII](https://www.ti.com/tool/BOOSTXL-EDUMKII) by Texas Instruments.
Besides this, hardware-dependent code is isolated in its own folder so that porting to other platforms does not require rewrting the application logic.

## Tools used

[Code Composer Studio](https://www.ti.com/tool/CCSTUDIO) has been extensively used for developing, uploading the software to the platform, debugging and checking the power consumption with EnergyTrace.
[Uniflash](https://www.ti.com/tool/UNIFLASH) has also been really helpful in restoring the launchpad after a faulty USB cable had caused memory corruption.  
To communicate with the peripherals (LCD and buzzer in particular) I have also used libraries provided by Texas Instruments in the [MSP432 SDK](https://dev.ti.com/tirex/explore/node?compilers=ccs&devtools=MSP-EXP432P401R&node=ADoDWIZc5WTuUbxxkAgevQ__z-lQYNj__LATEST).
