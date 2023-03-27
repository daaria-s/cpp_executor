## Student project 

_This is a program that reads commands from input and responds to them. Each command is a separate line: the program reads them and parses them._


Parts of program:

* **Quote.** 
Uses sherlock-holmes and cat-jeoffry to display an excerpt from Sherlock Holmes and Jubilate Agno, respectively. Preserves all whitespace characters exactly, including newlines. Uses the quit command to terminate the interpreter successfully. This part ensures that the input data is correct.

* **Careful reading.** The input data is now an arbitrary sequence of bytes with codes from 1 to 127 inclusive. Each command are be placed on a separate line, whitespace characters can be added at the beginning or at the end. Invalid commands should result in the message Invalid command: '<command>'.
     
* **Walker.** 
Emulates a walker on an infinite square grid: initially you are in cell (0, 0) and look up. Commands allow you to move forward, backward, turn 90 degrees left and right, and show the explored map using ASCII graphics. Recording an arbitrary sequence of commands should also be supported so that it can then be simply executed.

* **Walls.** 
Adding two new commands: add and remove a wall in front of the player. If the player tries to go through the wall, the message Bump! is displayed.
