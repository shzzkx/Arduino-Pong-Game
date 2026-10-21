# Arduino-Pong-Game
A 2-player Pong game built on an Arduino board using an OLED display. Created in a team of 2 from September to October of 2026.

## Introduction and Objectives
This game was created for a university-level tutorial class, where we had to combine polling loops, interrupts, and timers to create a Pong game in 2 3-hour sessions. We used push buttons for user input - each player could press the button to toggle whether the paddle (always in motion) was moving up or down. Players scored points if the ball hit the other player's wall. Whichever player scored 5 points first would win, and then the game would end.

## Debugging
One of the biggest errors we faced was inconsistent pushbutton input. With help from our course instructors, we determined the cause of this issue to be button bouncing, and developed a debouncing function that reduced bouncing errors during gameplay. Another error we had to troubleshoot was the ball passing through paddles instead of colliding off of them. This took us a while to figure out, but eventually we determined - by slowing the frame rate and logging ball and paddle coordinates over the serial monitor - that high speeds let the ball skip past the paddle between frames. We resolved this error by adjusting our ball speed so that the ball and paddle coordinates matched up before the ball went out of frame.

## List of Materials
1x Arduino UNO
1x OLED Display
1x Breadboard
3x Pushbuttons
12x Wires
1x USB-C Cable

### Software
We programmed this game using C++ in the Arduino IDE.
