*********************************************
*********************************************

Instructions To Run
---------------------------------------------
# Run 'make' which will compile the code and make an executable named sample2D
# To run the game, type './sample2D' in the terminal
# Instructions to play the game are provided in help.txt in the same folder
---------------------------------------------



Libraries Used
---------------------------------------------
# lGl
# lglfw
# ldl
---------------------------------------------

###########################################################
#		     About The Game 			  #
###########################################################

- The motive of the game is to collect as many falling blocks and destroying the
  dangerous blocks.

- The blocks spawn from the top at random positions every second. The blocks
  can be good blocks or dangerous blocks.

- Dangerous Blocks: The blocks coloured black are dangerous blocks.
  Good Blocks: The blocks coloured green or red are the good blocks.

- The game also has a lazer that can be used to shoot down the dangerous blocks,
  but a penalty will be imposed if the good blocks are destroyed.

- There are double-sided mirrors in the game that can reflect the lazer shot in
  their direction.
 
- You can only shoot until the battery is charged. After that you have to leave
  shooting button to let battery recharge.

- Walls in the game simply absorb the lazer.
 
- There are two buckets coloured red and green at the bottom of the screen which can be moved 
  horizontally to collect the respective coloured falling bricks. Penalty will be imposed
  if wrong blocks are collected in wrong buckets.
  
- The blocks not collected by any bucket or destroyed by lazer will simply fall.

- The game will get over if any black brick is collected by any of the two buckets.



############################################################
#		    Scoring System 			   #
############################################################

- Correct Block Collected  : +10
- Wrong Block Collected    : -10
- Dangerous Block Destroyed: +10
- Good Block Destoryed     : -5


############################################################
# 		     Instructions			                               #
############################################################

- Controlling the Cannon
  * Moving Vertically
    Keyboard:
    	+ Move Up  : a
	+ Move Down: d
    Mouse:

  * Rotating Cannon
    Keyboard:
    	+ Rotate Up  : s
	    + Rotate Down: f
    Mouse:
        + The cursor will determine the angle
                + Click on the cannon and move mouse up/down to move 
    cannon up/down.


  * Shooting
    Keyboard:
        + Hold up SpaceBar to shoot from the cannon.

- Controlling the Buckets
  * Move Left
    Keyboard:
    	+ Red Bucket  : Ctrl + left
	+ Green Bucket: Alt + left
    Mouse:
        + Red Bucket  : Click on the bucket and move mouse to left.
	+ Green Bucket: Click on the bucket and move mouse to left.

   * Move Right
     Keyboard:
        + Red Bucket  : Ctrl + right
	+ Green Bucket: Ctrl + right
     Mouse: 
        + Red Bucket  : Click on the bucket and move mouse to right.
	+ Green Bucket: Click on the bucket and move mouse to right.

- Miscellaneous
  * Zoom in
    Mouse:
        + Scroll Up
    Keyboard:
      + up
  
  * Zoom out
    Mouse:
        + Scroll Down
            Keyboard:
      + down


  * Pan Left/Right:
         + arrowKey to pan in respective direction.

  * Controlling Speed Of Game
    Keyboard:
        + Speed Up  : m
	+ Speed Down: n


************************END OF FILE***************************

