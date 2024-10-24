#Check Point
# Battle City Tank Game

## Overview

Battle City is a 2D tank battle game inspired by classic arcade games. Players control a tank that can move in various directions, fire bullets.

## Features

- Player-controlled tank with rotation and firing mechanics
- Collision detection for tanks and bullets
- Simple graphical interface using SDL2 for rendering

## Requirements

- C compiler (e.g., GCC)
- SDL2 library
- SDL2_image library for loading PNG images

## Setup

### Installation

1. **Clone the repository:**

   ```bash
   git clone https://github.com/TimurSatarov/Tank-Commander.git
   cd Tank-Commander

2. **Install SDL2 and SDL2_image:**
-
    - On Ubuntu/Debian:
    ```bash
    sudo apt-get install libsdl2-dev libsdl2-image-dev
-
    - On Archlinux:
    ```bash
    sudo pacman -S libsdl2-dev libsdl2-image-dev
-
    - On Windows
    ```txt
    Dowload the sdl develoment lib from libsdl.org
#
3. **Build the project**
    ```bash
    make
### Usage

1. **Run**
    ```bash
    ./battleCity
2. **Controle**
    ###### Arrow Keys: Move the tank (Up, Down, Left, Right)
    ###### A / D: Rotate the tank's gun
    ###### Space: Fire bullets

