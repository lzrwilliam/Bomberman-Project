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
