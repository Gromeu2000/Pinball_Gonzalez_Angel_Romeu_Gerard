# Pokemon Pinball

This is a simple recreation of 1999's Pokemon Pinball for the Game Boy Colour. It was made from scratch in C++ using Visual Studio and with the Box2D physics library. This game was made by Gerard Romeu and Ángel González, two 2nd year  students of the Bachelor Degree in Video Game Design and Development at UPC CITM/TTC, Barcelona .



# Creators' repositories 

- **Ángel González:**  https://github.com/BarcinoLechiguino
- **Gerard Romeu:** https://github.com/Gromeu2000



# Original game

**We got all our references for the game from here:**

​								 		https://kbhgames.com/game/pokemon-pinball



# Differences

- FX are different from the original game.
- The "Catch a Pokemon!" mechanic has not been implemented.
- Instead of having view of half the pinball board, the whole board can be seen and the camera is fixed
- There is now a Diglett Plunger / Kicker, which will send the balls in the board.
- Score can be now seen at all times between the kickers. High score and previous score are shown top right of the screen.
- Implemented a new combo score (Explained below)



# Combo

If you hit all 3 Voltorb bouncers and both kickers you get and extra ball that has its own sprite as well as 1000 extra points.

Every time one of the aforementioned elements are hit, they will light up. Once they are all lit then the extra ball enters the board and all elements are turned off.



# Controls 

- **LEFT Arrow** --> Left flipper

- **RIGHT Arrow** --> Right flipper

- **DOWN Arrow** --> Kicker / Plunger

- **R** --> Restart game



# Debug Keys

- **F1 Key:** Debug Mode
- **While in Debug Mode:**
  - See all the shapes that conform the board as well as the ball.
  - If a moving element of the board is left-clicked, a mouse joint will be generated.
  - 1 Key --> Create a ball wherever the mouse is at.
  - 2 Key --> Creates an extra ball wherever the mouse is at.



# Work distribution

* **Ángel González:**
* **Gerard Romeu:** 



# Project's repository

https://github.com/Gromeu2000/Pinball_Gonzalez_Angel_Romeu_Gerard



# License

MIT License

Copyright (c) [2019] [Gerard Romeu, Ángel González]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.