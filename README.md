# Bomberman

**Game Description**

This project simulates the popular game known as Bomberman by using an 8x8 LED Matrix on the Arduino platform. The purpose of the game is to destroy as many objects as possible by placing bombs close to them. The game becomes harder with each level by making the bombs explode faster, increasing the frequency of object movements, and reducing the time to destroy everything. The map with walls and objects is randomly generated every time, whether it is a new level or if the game has restarted, and the player is spawned randomly as well.

**Instructions for Playing**

Firstly, when the game is powered on, a welcome message appears on the LCD screen. After this message, the game menu is displayed alongside possible options, the first one being "Start Game". The other main menu options are: Start Game, HighScore, Settings, Choose Player, About, and How to Play. 

When booting the program for the first time, there are no created players, nor saved high scores; it is completely plain. To be able to play the game by selecting the first menu option, "Start Game," a player must be chosen. If the user tries to play the game without having a user chosen, an LCD message stating that a player must be chosen to play will appear, and the user will be taken back to the main menu.

There are two options for a user if they have not selected a player: 
1. Navigate through the main menu to the "Choose a Player" section, where existing players can be selected by scrolling with the joystick, if players exist.
2. If there are no created players, the user can create a new one, and it will become the current player. To create a player, navigate to Settings > SetName, enter the name character by character by choosing it from the alphabet by moving the joystick horizontally, and confirm the letter at a position with a short press of the joystick button. To modify the name, the joystick can be moved vertically up or down to navigate through the name positions. After obtaining the desired name, hold the joystick for about 4-5 seconds, and a message confirming player creation will be displayed on the LCD.

Now that we have explained the basic requirements to start the game, let's get more into the details about the rest of the menu items and functionalities.

<details>
  <summary>While Playing</summary>
 <br> 
  In the playing mode, the LCD screen always displays the following information: Current level, Remaining lives, Time left, Score, and the Player name. On the LED Matrix, the player is indicated by the fastest blinking position, the objects by the cells that blink very slowly, and the indestructible walls by non-blinking cells. The player can move anywhere he desires with the help of the joystick, except the occupied cells, if there is a possible path.

  To place a bomb at the current position, press the joystick button. If a bomb has been placed, a sound can be heard, and the cell blinking rapidly can be observed. The player must be careful because only one bomb at a time per cell is allowed, and also, if they are in the range of the bomb (1 cell up/down/left/right), they will lose a life.

  The game ends in the following situations: The time has expired, or the remaining lives are 0. In both of these cases, a message on the LCD informs the player about their score and level. If the score becomes one of the top three high scores, the player is informed about their current position in the high scores table. The same messages are displayed when the user advances to a new level after destroying all existing destructible objects, and the game difficulty increases slightly. To exit the game and return to the main menu, press the button on the Arduino breadboard.
</details>

<details>
  <summary>Navigating through the Menu</summary>
  <br> 

  Entering any option from the menu is made by pressing the joystick button on the desired one. To exit from an option without saving any changes in the main menu options, press the button on the breadboard.

  We already covered the functionality of Start Game and Choose Player options. It is to be noted that arrows are displayed on the left and right side of the LCD to indicate the directions where we can scroll to choose an item.

  The Highscore option displays the top 3 scores saved in the memory alongside the names of the players who achieved them. 

  The About and How to Play options display information about the game developer and basic instructions for new users to be able to play the Bomberman game. The user can scroll through messages displayed using the joystick.

  If the Settings option is selected, there are multiple sub-menu items: 
  - **Set Name**: Create a new user or change the name of an existing one.
  - **Set LCD Brightness**: Adjust the LCD brightness using the joystick and confirm the value with the breadboard button.
  - **Set Matrix Brightness**: Choose the LED matrix brightness. Two LEDs will be turned on the matrix with the real-time brightness value. Use the breadboard button to confirm and exit.
  - **Sounds**: Enable or disable sounds when navigating through the menu. Hold the joystick button for a few seconds to toggle and use the breadboard button to save and exit.
  - **Delete All Players**: Erase all players and high scores from memory.
  - **Reset Highscores**: Delete all high scores of existing players.
  - **Back**: Return to the main menu options.
</details>
