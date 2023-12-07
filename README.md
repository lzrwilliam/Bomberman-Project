# Bomberman

**Game description**
<br><br>
This project simulates the popular game known as Bomberman by using an 8x8 LED Matrix on Arduino platform. The purpose of the game is to destroy as much as possible objects by placing a bomb as close as possible to the objects. Playing the game becomes harder with each level that the player is advancing, by making the bombs the explode faster, the objects movements are more frequently and also the time to destroy everything is less than the precedent level. <br>
The map with walls and objects are randomly generated every time, no matter if it is a new level or if the game has started from beginning, and the player is spawned randomly as well. 
<br><br>

**Instructions for playing**
<br><br>
Firstly, when the game is powered on, a welcome message appears on the LCD screen. After this message has finished, the game menu is displayed alongside with the possible options, and the first one is "Start Game". The other main menu options are: - Start Game, - HighScore, - Settings, - Choose Player, - About and - How to Play.
When booting the program for the first time, there are no players created, no saved highscores, it is completly plain. In order to be able to play the game by selecting the menu first option, "Start Game" a player has to be chosen. If the user is trying to play the game without having a user chosen, on lcd a message saying that a player has to be chosen to play will apear and the user will be taken back to the main menu.
There are two options for a user if he has not selected a player: - Navigate through the main menu until the section "Choose a player", where existing players can be selected by scrolling with the joystick, if players exists. - If there are no created players, the user can create a new one and it will be the current player. To create a player, the user must navigate to Settings - SetName, enters the name char by char by choosing it from the alphabet by moving the joystick horizontally and confirms the letter on a position with a short press of the joystick button. For any modifications in the name, the joystick can be vertically moved up or down to navigate through the name postions. After the desired name is obtained, the joystick has to be hold for about 4-5 seconds and a message to confirm the player creation will be displayed on the LCD.
Now that we have explained the basic requirments to be able to start the game, let's get more in details about the rest of menu items and functionalities.


<details>
  <summary>While Playing</summary>
In the playing mode, on the LCD screen all the time there will be displayed the following informations: Current level, Remaining lives, time left, score and the player name. On the LED Matrix, the player is indicated by the fastest blinking position, the objects by the cells that blinks very slow and the walls that cannot be destroyed by not blinking cells. The player can move everywhere he deserves with the help of the joystick, except the occupied cells, if there is a possible path. <br> In order to place a bomb on the current position, the joystick button has to be pressed. If the bomb has been placed, a sound can be heard and the cell blinking too fast can be observed.The player has to be very carefull because only one bomb at a time per cell is allowed and also, if he is in the range of bomb (1 cell up/down/left/right), he will lose a life.
The game ends in the following situations: * The time has expired, * The remaining lives are 0. In both of this cases, a message on lcd informing the player about his score and level will be shown, and also, if the score becomes one of the three highscores, the player is informed about the current position in the highscores table. The same messages are displayed when the user advance to a new level when all existing destructible objects have been destroyed, and also the game difficulty increases a little bit. In order to exit the game and get back to the main menu, the player has to press the button on the Arduino breadbord.

</details>

<details>
<summary>Navigating through the menu</summary>
Entering in every option from the menu is made by pressing the joystick button on the desired one. To exit from an option, without saving any changes in the main menu options, the button on the breadbord can be pressed.
We already covered the functionality of Start Game and Choose player option. To be noticed, that in the left and right side of the LCD, arrows are displayed to indicate the directions where we can scroll to choose an item.
The Highscore option displays the top 3 scores saved in the memory alongside with the name of the players that achieved it. 
The about and how to play options will display informations about the game developer and basic instructions for new users in order to be able to play the bomberman game. The user can scroll through messages displayed using the joystick.
If the settings option is selected, there will be multiple sub-menu items : 
  - Set Name: from here the user can create a new user or change the name of the existing one.
  - Set LCD Brightness: the lcd brightness can be set here by using the joystick to chose the desired value and the breadbord button to exit and confirm the value.
  - Set Matrix Brightness: option to chose the brightness for the led matrix, as a better reference, two leds will be turned on on the matrix with the real-time value for brightness we choose. The button on the breadbord has the same purpose, to confirm and exit from the selection.
  - Sounds: wheather or not to have enabled sounds when navigating through the menu. To toggle the variable, the joystick button has to be hold for a few seconds, and to save the selection and exit, the button on the breadbord is to be used.
  - Delete all players: if selected, all players, alongside with all highscores saved, will be erased from the memory.
  - Reset Highscores: deletes all the highscores of the existing players>
_ Back: it returns from settings to the main menu options


  
</details>


