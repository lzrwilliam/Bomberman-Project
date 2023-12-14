# Bomberman

## Game Description

Welcome to the reimagined world of **Bomberman**, a classic favorite, now ingeniously adapted for the Arduino platform with a vivid 8x8 LED matrix display. This project is a delightful fusion of nostalgia and modern technology, inspired by the cherished gaming memories from my childhood. Bringing Bomberman to life on the Arduino platform presented a unique blend of challenges and learning experiences, combining electronics and programming in a creative and educational journey.

The essence of the game is to strategically place bombs to destroy as many objects as possible. Each new level increases the game's complexity by accelerating the bomb detonation time, enhancing the movement frequency of objects, and reducing the time available to clear the level. The game's environment, comprising randomly generated maps with walls and objects, adds an element of unpredictability, with the player's spawn point also varying randomly.

## Instructions for Playing

Upon powering on the game, a warm welcome message greets you on the LCD screen. Following this, the game menu presents several options, with "Start Game" being the first. The main menu options include: Start Game, HighScore, Settings, Choose Player, About, and How to Play.

For first-time players, the experience begins as a blank canvas. There are no pre-created players or saved high scores. To dive into the "Start Game" option, you must first select a player. If you attempt to start the game without selecting a player, a prompt will remind you to choose one, redirecting you back to the main menu.

If you haven't selected a player, you have two paths:
1. **Choose a Player**: Navigate to this menu option and scroll through existing players using the joystick (if any are available).
2. **Create a Player**: In the absence of existing players, create a new one under Settings > SetName. Enter the name character-by-character, selecting each letter from the alphabet using the joystick. A short press of the joystick button confirms each letter. For name adjustments, move the joystick vertically. Finalize your new player by holding down the joystick for about 4-5 seconds, upon which a confirmation message will appear on the LCD.

Let's dive more into the details of the gameplay and other menu functionalities.

<details>
  <summary><strong>While Playing</strong></summary>

  In gameplay mode, the LCD screen continuously displays key information: Current Level, Remaining Lives, Time Left, Score, and Player Name. The LED Matrix brings the game to life: the player is indicated by the fastest blinking light, objects by slower blinking cells, and indestructible walls by static cells. Navigate freely across the matrix with the joystick, avoiding occupied cells and finding your path.

  To deploy a bomb at your current location, simply press the joystick button. A distinct sound and rapid blinking on the cell indicate a bomb placement. Be cautious: only one bomb per cell is permitted, and being within the bomb's range (one cell in any direction) will cost you a life.

  The game concludes under two circumstances: when the time runs out or when no lives remain. In either case, a message will display your score and level on the LCD. If your score ranks within the top three high scores, you'll be notified of your position on the leaderboard. A similar message appears upon advancing to a new level after clearing all destructible objects, with each new level bringing a slight increase in difficulty. To exit the game and return to the main menu, press the button on the Arduino breadboard.
</details>

<details>
  <summary><strong>Navigating through the Menu</strong></summary>

  To explore menu options, press the joystick button on your desired selection. To exit without saving any changes in the main menu, simply press the breadboard button.

  We've already explored the functionalities of "Start Game" and "Choose Player." Note that arrows are displayed on the LCD's left and right sides to indicate scrollable directions.

  The "Highscore" option showcases the top 3 scores, complete with the players' names who achieved them. 

  The "About" and "How to Play" sections offer insights into the game's development and essential playing instructions. Use the joystick to navigate through these informative messages.

  Under the "Settings" option, you'll find several sub-menu items:
  - **Set Name**: Create a new user or alter the name of an existing one.
  - **Set LCD Brightness**: Adjust the LCD's brightness and confirm your setting with the breadboard button.
  - **Set Matrix Brightness**: Customize the LED matrix brightness. For a real-time reference, two LEDs on the matrix will reflect your chosen brightness level. Confirm and exit using the breadboard button.
  - **Sounds**: Toggle sound effects during menu navigation and gameplay. Hold the joystick button for a few seconds to toggle and use the breadboard button to save and exit.
  - **Delete All Players**: This option erases all players and high scores from memory.
  - **Reset Highscores**: Clears all high scores from existing players.
  - **Back**: Return to the primary menu options.

    **I noticed the following bug: if we want to exit or enter the settings menu (the second time), we cannot. Also, sometimes the SwPin of joystick presses himself!**.
</details>

<details>
  <summary><strong>Used and Required Components</strong></summary>

  To bring this game to life, the following components are essential:
  
  - **Arduino Uno**: The heart of the game, managing all the processing.
  - **A Joystick**: For player movement and menu navigation.
  - **8x8 LED Matrix**: The dynamic game map.
  - **MAX7219 Display Driver**: Controls the LED Matrix with ease.
  - **Capacitors**: Essential for stabilizing power supply and signal integrity.
  - **A Buzzer**: Adds auditory feedback during gameplay and menu navigation.
  - **LCD 16x2**: The interface for all in-game messages, settings adjustments, and gameplay interactions.
</details>

The functionality of the project can be seen in the following video:
[Project Video](https://www.youtube.com/watch?v=hVTK0uVyZXE)

Picture of the hardware setup:

![WhatsApp Image 2023-12-14 at 14 13 05](https://github.com/lzrwilliam/Bomberman-Project/assets/92024459/a2a346a5-1de7-4c02-b622-70731eb604c9)


