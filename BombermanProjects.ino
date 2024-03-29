#include "LedControl.h"
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <avr/pgmspace.h>




//----------------------------------- Messages stored in flash memory-----------------------

const char aboutMessage0[] PROGMEM = "The game you are playing is named Bomberman.";
const char aboutMessage1[] PROGMEM = "This game was developed by Lazar William Patrick,";
const char aboutMessage2[] PROGMEM = "student at University of Bucharest.";
const char aboutMessage3[] PROGMEM = "To see more of his projects,";
const char aboutMessage4[] PROGMEM = "search for github user: lzrwilliam.";
const char aboutMessage5[] PROGMEM = "Thank You!";

const char welcomeMessage[] PROGMEM = "Welcome. Choose a player or create a new one from settings in order to play.";

const char playMessage0[] PROGMEM = "This is bomberman, the game purpose is to achieve points by placing bombs and destroy the moving objects in your path.";
const char playMessage1[] PROGMEM = " Firstly, when you power up the game, you have to choose a user from existing one or create a new one.";
const char playMessage2[] PROGMEM = "If there is no memory to create new users, you can delete all existing users from Settings";
const char playMessage3[] PROGMEM = "After you created or selected a user, you can press on Start Game.";
const char playMessage4[] PROGMEM = "The game will begin, on LCD screen the player name is displayed, along side ";
const char playMessage5[] PROGMEM = " with the number of lives remained, time remained, current level and points. ";
const char playMessage6[] PROGMEM = " To gain points, you have to destroy the moving objects that blink slowly. ";
const char playMessage7[] PROGMEM = "The objects can be destroyed by placing a bomb by pressing the joystick button and when the bomb explodes";
const char playMessage8[] PROGMEM = " if the object is in it range of 2 columns/rows, a point will be added to your score.";
const char playMessage9[] PROGMEM = " Be very carefull, that if you are in range of the bomb, you will lose a life.";
const char playMessage10[] PROGMEM = "If you have 0 lifes remained, then the game will begin from scratch.";
const char playMessage11[] PROGMEM = "With every  level up, the bomb time will be lesser, the objects will move faster and the level time will be less.";
const char playMessage12[] PROGMEM = "Also, if the time expires, you lose and start from scratch.";
const char playMessage13[] PROGMEM = "At the end of the game, you will be informed on the LCD Screen of your level and points, and also if you beat any highscore of the top 3 ones.";
const char playMessage14[] PROGMEM = "If you want to exit the game, you can press the button on the breadbord.";
const char playMessage15[] PROGMEM = "In order to change settings, after you made the change, press the button on breadbord and the changes will be saved.";
const char playMessage16[] PROGMEM = "Also, if you want to exit a main menu selection or set name selection, without any change, press also the same button.";
const char playMessage17[] PROGMEM = "Not to forget about, there are arrows to left and right on the LCD screen, that indicates the directions possible to navigate in the menu.";
const char playMessage18[] PROGMEM = "Enjoy!";


const char highscoreMessagePart1[] PROGMEM = "Congrats, you are now on ";
const char noHighScoreMessage[] PROGMEM = "No highscores to be shown at the moment!";

const char playerUpdateMsg[] PROGMEM = "Player ";
const char isNowMsg[] PROGMEM = " is now ";
const char hasBeenCreatedMsg[] PROGMEM = " has been created";
const char noSpaceMsg[] PROGMEM = "No space to create new users!";
const char startGame[] PROGMEM = "Game has started";

const char choseMessage[] PROGMEM = "You have to choose a player in order to play!";
const char deleteMessage[] PROGMEM = "All players deleted!";
const char timeExpired[] PROGMEM = "Time has expired! Game over!";
const char resetHighScoreMessage[] PROGMEM = "Highscores have been reseted!";


const char* const aboutMessages[] PROGMEM = {
  aboutMessage0,
  aboutMessage1,
  aboutMessage2,
  aboutMessage3,
  aboutMessage4,
  aboutMessage5
};

const char* const playMessages[] PROGMEM = {
  playMessage0,
  playMessage1,
  playMessage2,
  playMessage3,
  playMessage4,
  playMessage5,
  playMessage6,
  playMessage7,
  playMessage8,
  playMessage9,
  playMessage10,
  playMessage11,
  playMessage12,
  playMessage13,
  playMessage14,
  playMessage15,
  playMessage16,
  playMessage17,
  playMessage18,
};
const int aboutMessagesCount = sizeof(aboutMessages) / sizeof(aboutMessages[0]);
const int playMessagesCount = sizeof(playMessages) / sizeof(playMessages[0]);

//-------------------------------------------------------------------------------------------//

// --------------------------------------------------- EEPROM Addresses ----------------------------------//


const int eepromBrightnessAddress = 0;
const int eepromMatrixBrightnessAddress = 1;
const int eepromSoundsAddress = 2;
const int eepromNumPlayersAddress = 3;
const int playersStartAddress = 5;
const int eepromLastPlayerIndex = 100;
const int eepromHighScoresAddress = 200;
//-----------------------------------------------------------------------------------------//

struct HighScore {
  char playerName[11];
  int score;
};

const int maxHighScores = 3;
HighScore highScores[maxHighScores];

struct Player {
  char fullName[11];
};

int numPlayers = 0;  //  number of currently total players
const int maxPlayers = 3;
const int playerSize = sizeof(Player);
Player players[maxPlayers];

int currentPlayerIndex = 0;
int selectedPlayerIndex = -1;
int currentPlayer = -1;




const int exitGameButtonPin = A5;
bool exitGameRequested = false;
bool backSelected = false;
bool soundsEnabled = true;

unsigned long lastDisplayChange = 0;
const unsigned long displayChangeInterval = 5000;  // used for alternating between showing the name or lives,scores on lcd while playing
bool displayingPlayerName = false;

const int settingsItems = 7;
int currentSettingsItem = -1;
String settingsMenu[settingsItems] = { "Set Name", "Set LCD Brightness", "Set Matrix Brightness", "Sounds", "Delete Players", "Reset Highscores", "Back" };

//-------------LCD, JOYSTICK AND MATRIX-----------------------------//

const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 13;
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);  // DIN, CLK, LOAD, No. DRIVER


const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
const int lcdBrightnessPin = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int xPin = A0;
const int yPin = A1;
const int swPin = 2;
const int pinBuzzer = 3;

byte matrixBrightness = 2;
byte xPos = 0;
byte yPos = 0;
byte xLastPos = 0;
byte yLastPos = 0;
const int minThreshold = 200;
const int maxThreshold = 600;

const byte moveInterval = 500;  // for the speed of moving through leds on matrix
const byte BlinkingPlayerPositionInterval = 600;
unsigned long lastBlink = 0;
bool isLedOn = true;
unsigned long long lastMoved = 0;

const byte matrixSize = 8;  // size of matrix
bool matrixChanged = true;  // track if the matrix display needs update
byte matrix[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};


//--------------------------------------------------//

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};
byte arrowLeft[8] = {
  0b00000,
  0b00010,
  0b00001,
  0b11111,
  0b00001,
  0b00010,
  0b00000,
  0b00000
};

byte arrowRight[8] = {
  0b00000,
  0b01000,
  0b10000,
  0b11111,
  0b10000,
  0b01000,
  0b00000,
  0b00000
};

byte settingsIcon[8] = {
  B00100100,
  B01111110,
  B11011011,
  B11111111,
  B11111111,
  B11011011,
  B01111110,
  B00100100
};

byte trophyIcon[8] = {
  B00111100,
  B01111110,
  B11111111,
  B11011011,
  B11000011,
  B11000011,
  B11111111,
  B01111110
};

byte aboutIcon[8] = {
  B00000000,
  B00011000,
  B00011000,
  B00000000,
  B00011000,
  B00011000,
  B00011000,
  B00011000
};

byte questionMark[8] = {
  B00011000,
  B00100100,
  B00100100,
  B00001000,
  B00010000,
  B00000000,
  B00010000,
  B00000000
};




// ------------------------- GAME LOGIC ---------------------------------------

struct Bomb {
  bool active;
  int x;
  int y;
  unsigned long timer;
};
const int maxBombs = 10;
Bomb bombs[maxBombs];

const byte EMPTY = 0;
const byte PLAYER = 1;
const byte INDESTRUCTIBLE_WALL = 2;
const byte DESTRUCTIBLE_WALL = 3;
const byte BOMB = 4;

uint8_t numarPeretiDistrusi = 0;
bool bombPlaced = false;
int bombX = 0;
int bombY = 0;
unsigned long bombTimer = 0;
const unsigned long initialBombDelay = 4000;
unsigned long bombDelay = initialBombDelay;
const unsigned long debounceDelay = 50;
unsigned long lastDebounceTime = 0;


int joystickButtonState = HIGH;
int lastJoystickButtonState = HIGH;

int level = 1;
int lives = 1;
unsigned long levelStartTime;
unsigned long levelTime = 300000;
unsigned long timeLeft = 0;

unsigned long lastWallMoveTime = 0;
const unsigned long initialWallMoveInterval = 2000;
unsigned long wallMoveInterval = initialWallMoveInterval;

bool gameStarted = false;
bool isResetting = false;

const int menuItems = 6;
int currentMenuItem = 0;
String menu[menuItems] = { "Start Game", "HighScore", "Settings", "Choose Player", "About", "How to Play" };

unsigned long lastScrollTime = 0;
const int scrollInterval = 300;
int messagePosition = 0;
bool messageCompleted = false;

// variables for input player name
String playerName = "";
const String alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
int alphabetIndex = 0;
const int maxNameLength = 10;

void loadEeprom() {

  int storedBrightness = EEPROM.read(eepromBrightnessAddress);
  if (storedBrightness == 255) {
    storedBrightness = 100;
  }
  setLCDBrightness(storedBrightness);

  matrixBrightness = EEPROM.read(eepromMatrixBrightnessAddress);
  if (matrixBrightness == 255) {
    matrixBrightness = 8;
  }
  lc.setIntensity(0, matrixBrightness);

  soundsEnabled = EEPROM.read(eepromSoundsAddress);
  if (soundsEnabled == 255) {
    soundsEnabled = true;
  }

  EEPROM.get(eepromNumPlayersAddress, numPlayers);
  if (numPlayers > maxPlayers || numPlayers < 0) {
    numPlayers = 0;
  }

  for (int i = 0; i < numPlayers; i++) {
    EEPROM.get(playersStartAddress + i * playerSize, players[i]);
  }

  // EEPROM.get(eepromLastPlayerIndex, currentPlayer);
  // EEPROM.get(eepromLastPlayerIndex, selectedPlayerIndex);
}

void setLCDBrightness(int brightness) {
  analogWrite(lcdBrightnessPin, brightness);
  EEPROM.update(eepromBrightnessAddress, brightness);
}

void turnLedsOff() {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      lc.setLed(0, row, col, false);
    }
  }
}

void playNavigationSound() {
  if (soundsEnabled) {
    tone(pinBuzzer, 1000, 100);
  }
}
void scrollLCDMessage(const char* message, int line, int* currentMessage = nullptr, unsigned long* lastJoystickTime = nullptr, int totalMessagescount = 0, bool isProgmem = true) {
  if (!message) return;
  lcd.clear();
  int displayLength = 16;
  int messageLength = isProgmem ? strlen_P(message) : strlen(message);
  int start = 0;
  unsigned long lastScrollTime = 0;
  const unsigned long scrollInterval = 300;

  while (start <= messageLength) {
    if (millis() - lastScrollTime > scrollInterval) {
      lcd.setCursor(0, line);
      for (int i = 0; i < displayLength; i++) {
        if (start + i < messageLength) {
          char ch = isProgmem ? pgm_read_byte(message + start + i) : message[start + i];
          lcd.write(ch);
        } else {
          lcd.write(' ');
        }
      }
      lastScrollTime = millis();
      start++;
    }
    if (currentMessage != nullptr) {  // we verify  if we have a multi line message or just single one

      if (millis() - *lastJoystickTime > 500) {  // joystick debounce
        int yRead = analogRead(yPin);
        if (yRead< minThreshold&& * currentMessage > 0) {
          (*currentMessage)--;
          *lastJoystickTime = millis();
          return;  //exit scroll to print a message behind
        } else if (yRead > maxThreshold && *currentMessage < totalMessagescount - 1) {
          (*currentMessage)++;
          *lastJoystickTime = millis();
          return;  // //exit scroll to print a message forward
        }
      }
    }

    if (digitalRead(exitGameButtonPin) == LOW) {
      return;
    }
  }
}

void checkExitGameButton() {
  static unsigned long lastDebounceTime = 0;
  const unsigned long debounceDelay = 50;
  int reading = digitalRead(exitGameButtonPin);
  if (reading == LOW) {
    exitGameRequested = true;
  }
}

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(swPin, INPUT_PULLUP);
  pinMode(exitGameButtonPin, INPUT_PULLUP);

  randomSeed(analogRead(0));
  lc.shutdown(0, false);
  lc.setIntensity(0, matrixBrightness);  // sets brightness (0-15)
  lc.clearDisplay(0);
  spawnPlayer();
  initializeWalls();
  BombsSetToFalse();
  levelStartTime = millis();
  lcd.createChar(0, heart);
  lcd.createChar(1, arrowLeft);
  lcd.createChar(2, arrowRight);
  loadHighScores();
  messageCompleted = false;

  loadEeprom();
  scrollLCDMessage(welcomeMessage, 0);
  displayMenu();
}

void manageGameState() {
  if (exitGameRequested) {
    if (gameStarted) {
      resetGame();
      turnLedsOff();
    }
    gameStarted = false;
    exitGameRequested = false;
    displayMenu();
  } else if (gameStarted) {
    if (currentPlayer != -1) {
      playGame();
    } else {
      gameStarted = false;
      displayMenu();
    }
  } else {

    navigateMenu();
  }
}
void loop() {
  checkExitGameButton();
  manageGameState();
}

void playGame() {
  handleBombLogic();
  updateMatrix();
  BlinkPlayerPos();
  displayGameInfo();

  if (millis() - lastMoved > moveInterval) {
    updatePositions();
    lastMoved = millis();
  }
  if (matrixChanged) {
    updateMatrix();
    matrixChanged = false;
  }

  if (areAllDestructibleWallsDestroyed()) {
    advanceToNextLevel();
  }

  unsigned long currentMillis = millis();

  if (currentMillis < levelStartTime) {  // for the case if millis has overflow and lesser than levelstartime
    levelStartTime = currentMillis;
  }
  timeLeft = (levelTime - (millis() - levelStartTime)) / 1000;

  if (timeLeft <= 0) {
    onTimeExpired();
  }

  if (millis() - lastWallMoveTime > wallMoveInterval) {
    moveDestructibleWalls();
    lastWallMoveTime = millis();
  }
}

void updateMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      bool bombHere = false;
      for (int i = 0; i < maxBombs; i++) {
        if (bombs[i].active && bombs[i].x == row && bombs[i].y == col) {
          bombHere = true;
          break;
        }
      }
      if (bombHere) {
        lc.setLed(0, row, col, (millis() / 50) % 2);  // bomb blink fast
      } else {
        switch (matrix[row][col]) {
          case INDESTRUCTIBLE_WALL:
            lc.setLed(0, row, col, true);  // indestructible objects no blink
            break;
          case DESTRUCTIBLE_WALL:
            lc.setLed(0, row, col, (millis() / 700) % 2);  // destructible objects little blink
            break;
          case PLAYER:
            lc.setLed(0, row, col, true);
            break;
          default:
            lc.setLed(0, row, col, false);  // empty cells off
            break;
        }
      }
    }
  }
}
void updatePositions() {
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);

  byte newXPos = xPos;
  byte newYPos = yPos;

  if (xValue < minThreshold && xPos > 0) {
    if (matrix[xPos - 1][yPos] == EMPTY) {
      newXPos = xPos - 1;
    }
  } else if (xValue > maxThreshold && xPos < matrixSize - 1) {
    if (matrix[xPos + 1][yPos] == EMPTY) {
      newXPos = xPos + 1;
    }
  }

  if (yValue < minThreshold && yPos < matrixSize - 1) {
    if (matrix[xPos][yPos + 1] == EMPTY) {
      newYPos = yPos + 1;
    }
  } else if (yValue > maxThreshold && yPos > 0) {
    if (matrix[xPos][yPos - 1] == EMPTY) {
      newYPos = yPos - 1;
    }
  }

  if (newXPos != xPos || newYPos != yPos) {
    matrix[xPos][yPos] = EMPTY;  // the cell that player left we turn it off
    xPos = newXPos;
    yPos = newYPos;
    matrix[xPos][yPos] = PLAYER;  // new player position
    matrixChanged = true;
  }
}
void BlinkPlayerPos() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastBlink > BlinkingPlayerPositionInterval) {
    lastBlink = currentMillis;
    isLedOn = !isLedOn;
    matrix[xPos][yPos] = isLedOn ? 1 : 0;
    matrixChanged = true;
  }
}
void initializeWalls() {
  //firstly we add indestructible objects
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      if (random(100) < 5) {
        matrix[row][col] = INDESTRUCTIBLE_WALL;
      } else {
        matrix[row][col] = EMPTY;
      }
    }
  }

  // secondly, we add destructible objects
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      if (matrix[row][col] == EMPTY && random(100) < 5) {
        if (isAccessible(row, col)) {
          matrix[row][col] = DESTRUCTIBLE_WALL;
        }
      }
    }
  }
}
void spawnPlayer() {
  do {
    xPos = random(0, matrixSize);
    yPos = random(0, matrixSize);
  } while (matrix[xPos][yPos] != EMPTY);  // make sure that random spawn position for player is empty
  matrix[xPos][yPos] = PLAYER;
  matrixChanged = true;
}
void placeBomb() {

  // check for existing bomb in current cell
  for (int i = 0; i < maxBombs; i++) {
    if (bombs[i].active && bombs[i].x == xPos && bombs[i].y == yPos) {
      return;
    }
  }
  for (int i = 0; i < maxBombs; i++) {
    if (!bombs[i].active) {
      bombs[i].active = true;
      bombs[i].x = xPos;
      bombs[i].y = yPos;
      bombs[i].timer = millis();
      matrix[xPos][yPos] = BOMB;
      tone(pinBuzzer, 300, 70);
      break;
    }
  }
}
void detonateBomb() {
  // bool peretiDistrusiInAceastaDetonare = false;
  for (int i = 0; i < maxBombs; i++) {
    if (bombs[i].active && millis() - bombs[i].timer > bombDelay) {
      if ((abs(bombs[i].x - xPos) <= 1 && bombs[i].y == yPos) || (abs(bombs[i].y - yPos) <= 1 && bombs[i].x == xPos) || (bombs[i].x == xPos && bombs[i].y == yPos)) {
        loseLife();
      }
      bombs[i].active = false;
      // destroy horizontally right
      for (int dx = 1; dx <= 2; dx++) {
        int colRight = bombs[i].y + dx;
        if (colRight < matrixSize) {
          if (matrix[bombs[i].x][colRight] == DESTRUCTIBLE_WALL) {
            matrix[bombs[i].x][colRight] = EMPTY;
            numarPeretiDistrusi++;
            // peretiDistrusiInAceastaDetonare = true;

          } else if (matrix[bombs[i].x][colRight] == INDESTRUCTIBLE_WALL) {
            break;  // we cannot destroy through indestructible objects
          }
        }
      }
      for (int dx = -1; dx >= -2; dx--) {
        int colLeft = bombs[i].y + dx;
        if (colLeft >= 0) {
          if (matrix[bombs[i].x][colLeft] == DESTRUCTIBLE_WALL) {
            matrix[bombs[i].x][colLeft] = EMPTY;
            numarPeretiDistrusi++;
            // peretiDistrusiInAceastaDetonare = true;
          } else if (matrix[bombs[i].x][colLeft] == INDESTRUCTIBLE_WALL) {
            break;
          }
        }
      }
      // destroy vertically up
      for (int dy = 1; dy <= 2; dy++) {
        int rowUp = bombs[i].x - dy;
        if (rowUp >= 0) {
          if (matrix[rowUp][bombs[i].y] == DESTRUCTIBLE_WALL) {
            matrix[rowUp][bombs[i].y] = EMPTY;
            numarPeretiDistrusi++;
            // peretiDistrusiInAceastaDetonare = true;
          } else if (matrix[rowUp][bombs[i].y] == INDESTRUCTIBLE_WALL) {
            break;
          }
        }
      }
      for (int dy = 1; dy <= 2; dy++) {
        int rowDown = bombs[i].x + dy;
        if (rowDown < matrixSize) {
          if (matrix[rowDown][bombs[i].y] == DESTRUCTIBLE_WALL) {
            matrix[rowDown][bombs[i].y] = EMPTY;
            numarPeretiDistrusi++;
            // peretiDistrusiInAceastaDetonare = true;
          } else if (matrix[rowDown][bombs[i].y] == INDESTRUCTIBLE_WALL) {
            break;
          }
        }
      }

      matrix[bombs[i].x][bombs[i].y] = EMPTY;
      matrixChanged = true;
    }
  }
}
void handleBombLogic() {
  int reading = digitalRead(swPin);

  if (reading != lastJoystickButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != joystickButtonState) {
      joystickButtonState = reading;
      if (joystickButtonState == LOW) {
        placeBomb();
      }
    }
  }
  lastJoystickButtonState = reading;
  detonateBomb();
}
void BombsSetToFalse() {
  for (int i = 0; i < maxBombs; i++) {
    bombs[i].active = false;
  }
}

bool isInBounds(int row, int col) {
  return row >= 0 && row < matrixSize && col >= 0 && col < matrixSize;
}

bool isFree(int row, int col) {
  return matrix[row][col] == EMPTY || matrix[row][col] == PLAYER;
}

// check if placing an indestructible object will block access
bool isAccessible(int row, int col) {
  if (!isInBounds(row, col) || !isFree(row, col)) {
    return false;
  }

  int dx[] = { -1, 1, 0, 0 };
  int dy[] = { 0, 0, -1, 1 };

  for (int i = 0; i < 4; i++) {
    int adjRow = row + dx[i];
    int adjCol = col + dy[i];

    // if adjancy cell is free access will not be blocked
    if (isInBounds(adjRow, adjCol) && isFree(adjRow, adjCol)) {
      return true;
    }
  }
  // if no way for player to reach the object, we will not block the access
  return false;
}

bool areAllDestructibleWallsDestroyed() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      if (matrix[row][col] == DESTRUCTIBLE_WALL) {
        return false;
      }
    }
  }
  return true;
}

void advanceToNextLevel() {
  level++;
  levelStartTime = millis();                   // reset time for next level
  levelTime = max(30, 300000 - (10 * level));  // level time decrease
  initializeWalls();
  spawnPlayer();
  bombDelay = max(1000, bombDelay - 250);               // by each level bomb explodes faster
  wallMoveInterval = max(500, wallMoveInterval - 250);  // walls moves faster with each level
  updateHighScores(numarPeretiDistrusi);
}
void onTimeExpired() {
  scrollLCDMessage(timeExpired, 1);
  resetGame();
}

void loseLife() {
  tone(pinBuzzer, 1000, 500);
  lives--;
  if (lives <= 0) {
    resetGame();
  }
}

void resetGame() {
  lcd.clear();

  char messageFragment[25];
  snprintf(messageFragment, sizeof(messageFragment), "Congrats for reaching");
  scrollLCDMessage(messageFragment, 1, nullptr, nullptr, 0, false);

  snprintf(messageFragment, sizeof(messageFragment), "level %d", level);
  scrollLCDMessage(messageFragment, 1, nullptr, nullptr, 0, false);

  snprintf(messageFragment, sizeof(messageFragment), "with score of %d", numarPeretiDistrusi);
  scrollLCDMessage(messageFragment, 1, nullptr, nullptr, 0, false);

  updateHighScores(numarPeretiDistrusi);
  level = 1;
  lives = 1;
  numarPeretiDistrusi = 0;
  bombDelay = initialBombDelay;
  wallMoveInterval = initialWallMoveInterval;
  timeLeft = 0;
  levelStartTime = millis();
  timeLeft = levelTime;
}
void moveDestructibleWalls() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      if (matrix[row][col] == DESTRUCTIBLE_WALL && random(2) == 0) {  // 50% chance to move the wall
        int dx[] = { -1, 1, 0, 0 };
        int dy[] = { 0, 0, -1, 1 };
        int direction = random(4);
        int newRow = row + dx[direction];
        int newCol = col + dy[direction];

        if (isInBounds(newRow, newCol) && matrix[newRow][newCol] == EMPTY) {
          matrix[newRow][newCol] = DESTRUCTIBLE_WALL;
          matrix[row][col] = EMPTY;
        }
      }
    }
  }
  matrixChanged = true;
}
void displayMenu() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("*Menu Game*");
  if (currentMenuItem > 0) {
    lcd.setCursor(0, 0);
    lcd.write(byte(1));  // left arrow
  }
  lcd.setCursor(0, 1);
  lcd.print("> " + menu[currentMenuItem]);

  if (currentMenuItem < menuItems - 1) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
  }
}
void navigateMenu() {
  static unsigned long lastNavTime = 0;
  const int navDelay = 200;
  int xRead = analogRead(xPin);
  int yRead = analogRead(yPin);
  unsigned long currentMillis = millis();

  if (currentMillis - lastNavTime > navDelay) {
    if (yRead < 300 && currentMenuItem < menuItems - 1) {
      currentMenuItem++;
      playNavigationSound();
      displayMenu();
      lastNavTime = currentMillis;
    } else if (yRead > 700 && currentMenuItem > 0) {
      playNavigationSound();
      currentMenuItem--;
      displayMenu();
      lastNavTime = currentMillis;
    }

    if (digitalRead(swPin) == LOW) {
      playNavigationSound();
      selectMenuItem();
      lastNavTime = currentMillis;
    }
  }
}

void selectMenuItem() {
  if (currentMenuItem == 0) {
    if (currentPlayer != -1) {
      levelStartTime = millis();  // solved the bug that time was already much elapsed
      lcd.clear();
      scrollLCDMessage(startGame, 0);
      gameStarted = true;
    } else {
      gameStarted = false;
      scrollLCDMessage(choseMessage, 0, nullptr, nullptr, 0, true);
      displayMenu();
    }
  } else if (currentMenuItem == 1) {
    displayOnMatrix(trophyIcon);
    displayHighScores();
  } else if (currentMenuItem == 2) {
    displayOnMatrix(settingsIcon);
    displaySettingsMenu();
    navigateSettingsMenu();

  } else if (currentMenuItem == 3) {
    showPlayers();
    navigatePlayerSelection();
  } else if (currentMenuItem == 4) {

    displayAboutMessage();
  } else if (currentMenuItem == 5) {

    displayHowToPlayMessage();
  }
}

void displaySettingsMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Settings");

  if (currentSettingsItem > 0) {
    lcd.setCursor(0, 1);
    lcd.write(byte(1));  // left arrow
  }

  lcd.setCursor(1, 1);
  lcd.print("> " + settingsMenu[currentSettingsItem]);
  if (currentSettingsItem < settingsItems - 1) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
  }
}

void navigateSettingsMenu() {
  backSelected = false;
  unsigned long lastNavTime = 0;
  const unsigned long navInterval = 200;

  while (!backSelected) {
    int yRead = analogRead(yPin);
    unsigned long currentMillis = millis();


    if (currentMillis - lastNavTime > navInterval) {
      // Navigate down
      if (yRead < 300 && currentSettingsItem < settingsItems - 1) {
        playNavigationSound();
        currentSettingsItem++;
        displaySettingsMenu();
        lastNavTime = currentMillis;
      }
      // Navigate up
      if (yRead > 700 && currentSettingsItem > 0) {
        playNavigationSound();
        currentSettingsItem--;
        displaySettingsMenu();
        lastNavTime = currentMillis;
      }
    }

    if (digitalRead(swPin) == LOW && currentMillis - lastNavTime > navInterval) {
      playNavigationSound();
      selectSettingsItem();
      lastNavTime = currentMillis;
    }

    if (backSelected) {
      break;
    }
  }


  displayMenu();
}

void selectSettingsItem() {
  switch (currentSettingsItem) {
    case 0:
      enterName();
      break;
    case 1:
      adjustLCDBrightness();
      break;
    case 2:
      adjustMatrixBrightness();
      break;
    case 3:
      manageSounds();
      break;
    case 4:
      deletePlayers();
      break;

    case 5:
      resetHighScores();
      scrollLCDMessage(resetHighScoreMessage, 1);
      break;
    case 6:
      backSelected = true;
      turnLedsOff();
      break;
  }
  displaySettingsMenu();
}

void enterName() {
  int currentLetterIndex = 0;
  unsigned long lastJoystickMove = 0;
  const unsigned long joystickMoveInterval = 200;
  bool isButtonPressed = false;
  const unsigned long debounceInterval = 50;
  unsigned long lastButtonPress = 0;
  unsigned long buttonPressStartTime = 0;
  String alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int alphabetIndex = 0;
  bool isEditing = false;

  playerName = "";
  const unsigned long refreshInterval = 150;
  unsigned long lastRefreshTime = 0;
  while (true) {
    int xRead = analogRead(xPin);
    int yRead = analogRead(yPin);
    int joystickButtonState = digitalRead(swPin);
    unsigned long currentMillis = millis();

    // navigate letter with debounce
    if (currentMillis - lastJoystickMove > joystickMoveInterval) {
      if (yRead < minThreshold && alphabetIndex > 0) {
        alphabetIndex--;
        isEditing = true;
        lastJoystickMove = currentMillis;
      } else if (yRead > maxThreshold && alphabetIndex < alphabet.length() - 1) {
        alphabetIndex++;
        isEditing = true;
        lastJoystickMove = currentMillis;
      }

      if (xRead < minThreshold && currentLetterIndex > 0) {
        currentLetterIndex--;
        isEditing = currentLetterIndex < playerName.length();
        lastJoystickMove = currentMillis;
      } else if (xRead > maxThreshold && currentLetterIndex < playerName.length()) {
        currentLetterIndex++;
        isEditing = currentLetterIndex < playerName.length();
        lastJoystickMove = currentMillis;
      }
    }
    if (joystickButtonState == LOW) {
      if (!isButtonPressed) {
        isButtonPressed = true;
        buttonPressStartTime = currentMillis;
        lastButtonPress = currentMillis;
      } else if (currentMillis - buttonPressStartTime > 4000) {
        // if joystick button pressed more than 4 seconds, select the player and exit the entername function
        updatePlayerName();
        break;
      }
    } else if (joystickButtonState == HIGH && isButtonPressed) {
      isButtonPressed = false;
      if (currentMillis - lastButtonPress > debounceInterval) {
        if (isEditing && alphabetIndex < alphabet.length() - 1) {
          // add or edit char
          if (currentLetterIndex < playerName.length()) {
            playerName.setCharAt(currentLetterIndex, alphabet.charAt(alphabetIndex));
          } else {
            playerName += alphabet.charAt(alphabetIndex);
          }
          currentLetterIndex++;
          isEditing = false;
        }
      }
    }
    if (playerName.length() >= maxNameLength) {
      updatePlayerName();
      break;
    }
    if (currentMillis - lastRefreshTime > refreshInterval) {  // refresh in order that the lcd screen not refresh too fast
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Name: " + playerName);
      if (isEditing) {
        lcd.setCursor(6 + currentLetterIndex, 0);
        lcd.print(alphabet.charAt(alphabetIndex));
      }
      lcd.setCursor(6 + currentLetterIndex, 0);
      lcd.cursor();
      lastRefreshTime = currentMillis;
    }

    if (digitalRead(exitGameButtonPin) == LOW) {
      return;
    }
  }
}

void updatePlayerName() {
  static char messageBuffer[50];
  if (selectedPlayerIndex != -1 && selectedPlayerIndex < numPlayers) {
    String oldName = String(players[selectedPlayerIndex].fullName);
    // update if a player is selected
    strncpy(players[selectedPlayerIndex].fullName, playerName.c_str(), sizeof(players[selectedPlayerIndex].fullName) - 1);
    players[selectedPlayerIndex].fullName[sizeof(players[selectedPlayerIndex].fullName) - 1] = '\0';
    EEPROM.put(playersStartAddress + selectedPlayerIndex * playerSize, players[selectedPlayerIndex]);


    snprintf(messageBuffer, sizeof(messageBuffer), "Player %s is now %s", oldName.c_str(), playerName.c_str());
    scrollLCDMessage(messageBuffer, 1, nullptr, nullptr, 0, false);

  } else if (numPlayers < maxPlayers) {
    // create new player
    strncpy(players[numPlayers].fullName, playerName.c_str(), sizeof(players[numPlayers].fullName) - 1);
    players[numPlayers].fullName[sizeof(players[numPlayers].fullName) - 1] = '\0';
    EEPROM.put(playersStartAddress + numPlayers * playerSize, players[numPlayers]);
    snprintf(messageBuffer, sizeof(messageBuffer), "Player %s has been created", playerName.c_str());
    scrollLCDMessage(messageBuffer, 1, nullptr, nullptr, 0, false);
    currentPlayer = numPlayers;
    selectedPlayerIndex = currentPlayer;

    EEPROM.put(eepromLastPlayerIndex, currentPlayer);
    numPlayers++;
    EEPROM.put(eepromNumPlayersAddress, numPlayers);
  } else {
    // no memory
    scrollLCDMessage(noSpaceMsg, 0);
  }
  playerName = "";  // resets name for future inputs
}

void showPlayers() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Choose player:");

  if (numPlayers == 0) {
    lcd.setCursor(0, 1);
    lcd.print("No player");
    return;
  }

  Player p;
  EEPROM.get(playersStartAddress + currentPlayerIndex * playerSize, p);
  lcd.setCursor(0, 1);
  lcd.print(p.fullName);
}

void navigatePlayerSelection() {
  static unsigned long lastNavTime = 0;
  unsigned long buttonPressStartTime = 0;
  bool isButtonPressed = false;
  const int navDelay = 1000;

  while (true) {
    int joystickButtonState = digitalRead(swPin);

    int yRead = analogRead(yPin);
    unsigned long currentMillis = millis();

    if (digitalRead(exitGameButtonPin) == LOW) {
      displayMenu();
      return;
    }
    // joystick debounce
    if (currentMillis - lastNavTime > navDelay) {
      if (yRead < minThreshold && currentPlayerIndex > 0) {
        currentPlayerIndex--;
        showPlayers();
        lastNavTime = currentMillis;
      } else if (yRead > maxThreshold && currentPlayerIndex < numPlayers - 1) {
        currentPlayerIndex++;
        showPlayers();
        lastNavTime = currentMillis;
      }
    }
    if (joystickButtonState == LOW && !isButtonPressed) {
      isButtonPressed = true;
      buttonPressStartTime = currentMillis;
    }
    if (joystickButtonState == HIGH && isButtonPressed) {
      if (currentMillis - buttonPressStartTime > 3000) {
        if (numPlayers > 0) {

          selectedPlayerIndex = currentPlayerIndex;
          currentPlayer = currentPlayerIndex;  // retain the current player

          String oldName = String(players[selectedPlayerIndex].fullName);


          EEPROM.put(eepromLastPlayerIndex, selectedPlayerIndex);  // save selected player in eeprom

          char selectedPlayerName[11];  // max name 10 char+string end
          strncpy(selectedPlayerName, players[selectedPlayerIndex].fullName, sizeof(selectedPlayerName) - 1);
          selectedPlayerName[sizeof(selectedPlayerName) - 1] = '\0';

          char messageBuffer[30];
          snprintf(messageBuffer, sizeof(messageBuffer), "Player %s chosen", selectedPlayerName);
          scrollLCDMessage(messageBuffer, 1, nullptr, nullptr, 0, false);
        }
        displayMenu();
        return;
      }
      isButtonPressed = false;
    }
  }
}

void displayGameInfo() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastDisplayChange > displayChangeInterval) {
    displayingPlayerName = !displayingPlayerName;
    lastDisplayChange = currentMillis;
  }
  lcd.clear();
  if (displayingPlayerName && currentPlayer != -1) {
    lcd.setCursor(0, 0);
    lcd.print("Player: ");
    lcd.print(players[currentPlayer].fullName);
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Lvl:" + String(level) + " Lives:");
    for (int i = 0; i < lives && i < 4; i++) {
      lcd.write(byte(0));
    }
  }

  // show score and time remained
  unsigned long timeRemaining = (levelStartTime + levelTime - millis()) / 1000;
  lcd.setCursor(0, 1);
  lcd.print("Scor:" + String(numarPeretiDistrusi) + " Timp:" + String(timeRemaining) + "s");
}
void adjustLCDBrightness() {
  int brightness = EEPROM.read(eepromBrightnessAddress);
  int yRead;
  unsigned long lastAdjustTime = 0;
  const int adjustInterval = 50;
  unsigned long buttonPressStartTime = 0;
  bool isButtonPressed = false;
  const unsigned long refreshInterval = 200;
  unsigned long lastRefreshTime = 0;

  while (true) {
    yRead = analogRead(yPin);
    int joystickButtonState = digitalRead(swPin);
    unsigned long currentMillis = millis();
    if (currentMillis - lastAdjustTime > adjustInterval) {
      if (yRead < 300) {
        brightness = max(0, brightness - 10);
      } else if (yRead > 700) {
        brightness = min(255, brightness + 10);
      }
      lastAdjustTime = currentMillis;
    }
    if (joystickButtonState == LOW && !isButtonPressed) {
      isButtonPressed = true;
      buttonPressStartTime = currentMillis;
    }

    if (joystickButtonState == HIGH && isButtonPressed) {
      if (currentMillis - buttonPressStartTime > 2000) {
        return;
      }
      isButtonPressed = false;
    }
    setLCDBrightness(brightness);
    if (currentMillis - lastRefreshTime > refreshInterval) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Brightness: ");
      lcd.setCursor(0, 1);
      lcd.print(brightness);
      lastRefreshTime = currentMillis;
    }

    if (digitalRead(exitGameButtonPin) == LOW) {

      return;
    }
  }
}

void adjustMatrixBrightness() {
  int yRead;
  unsigned long lastAdjustTime = 0;
  const int adjustInterval = 100;
  unsigned long buttonPressStartTime = 0;
  bool isButtonPressed = false;
  const unsigned long refreshInterval = 200;
  unsigned long lastRefreshTime = 0;
  while (true) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastRefreshTime > refreshInterval) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Brightness: ");
      lcd.setCursor(0, 1);
      lcd.print(matrixBrightness);
      lastRefreshTime = currentMillis;
    }
    yRead = analogRead(yPin);
    if (currentMillis - lastAdjustTime > adjustInterval) {
      if (yRead < 300) {
        matrixBrightness = max(0, matrixBrightness - 1);  // increase brightness
      } else if (yRead > 700) {
        matrixBrightness = min(15, matrixBrightness + 1);  // decrease
      }
      lc.setIntensity(0, matrixBrightness);
      lastAdjustTime = currentMillis;
    }
    // turn on two leds on matrix to see the changes made in order to adjust properly
    lc.setLed(0, 0, 0, true);
    lc.setLed(0, 7, 7, true);

    int joystickButtonState = digitalRead(swPin);
    if (joystickButtonState == LOW && !isButtonPressed) {
      isButtonPressed = true;
      buttonPressStartTime = currentMillis;
    }
    if (joystickButtonState == HIGH && isButtonPressed) {
      if (currentMillis - buttonPressStartTime > 4000) {
        EEPROM.update(eepromMatrixBrightnessAddress, matrixBrightness);
        lc.setLed(0, 0, 0, false);
        lc.setLed(0, 7, 7, false);
        return;
      }
      isButtonPressed = false;
    }
    if (digitalRead(exitGameButtonPin) == LOW) {
      turnLedsOff();
      return;
    }
  }
}

void manageSounds() {
  unsigned long buttonPressStartTime = 0;
  bool isButtonPressed = false;
  unsigned long lastRefreshTime = 0;
  const unsigned long refreshInterval = 200;
  const unsigned long buttonDelay = 500;
  while (true) {
    unsigned long currentMillis = millis();

    if (currentMillis - lastRefreshTime > refreshInterval) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sounds: ");
      lcd.setCursor(0, 1);
      lcd.print(soundsEnabled ? "On" : "Off");
      lastRefreshTime = currentMillis;
    }
    int joystickButtonState = digitalRead(swPin);
    if (joystickButtonState == LOW && !isButtonPressed) {
      isButtonPressed = true;
      buttonPressStartTime = currentMillis;
    }

    if (joystickButtonState == HIGH && isButtonPressed) {
      if (currentMillis - buttonPressStartTime > buttonDelay) {
        // if button has been short pressed , sounds var toggle
        soundsEnabled = !soundsEnabled;
        EEPROM.update(eepromSoundsAddress, soundsEnabled);
      }
      isButtonPressed = false;
    }
    if (digitalRead(exitGameButtonPin) == LOW) {
      return;
    }
  }
}

void deletePlayers() {
  for (int i = 0; i < maxPlayers; i++) {
    Player emptyPlayer;
    memset(&emptyPlayer, 0, sizeof(Player));
    EEPROM.put(playersStartAddress + i * playerSize, emptyPlayer);
  }
  numPlayers = 0;
  EEPROM.put(eepromNumPlayersAddress, numPlayers);
  selectedPlayerIndex = -1;
  currentPlayer = -1;
  EEPROM.put(eepromLastPlayerIndex, currentPlayer);
  resetHighScores();
  scrollLCDMessage(deleteMessage, 0);
}
void loadHighScores() {
  EEPROM.get(eepromHighScoresAddress, highScores);
}

void saveHighScores() {
  EEPROM.put(eepromHighScoresAddress, highScores);
}
void resetHighScores() {
  for (int i = 0; i < maxHighScores; i++) {
    highScores[i].score = 0;
    memset(highScores[i].playerName, 0, sizeof(highScores[i].playerName));
  }
  saveHighScores();
}

void updateHighScores(int newScore) {
  int existingIndex = -1;
  bool isNewHighScore = false;
  int newHighScorePosition = -1;

  // if player has already a highscore
  for (int i = 0; i < maxHighScores; i++) {
    if (strcmp(highScores[i].playerName, players[currentPlayer].fullName) == 0) {
      existingIndex = i;
      break;
    }
  }

  if (existingIndex != -1) {
    // Update score if the new score is higher
    if (newScore > highScores[existingIndex].score) {
      highScores[existingIndex].score = newScore;
      // Reorder high scores
      for (int i = existingIndex; i > 0; i--) {
        if (highScores[i].score > highScores[i - 1].score) {
          HighScore temp = highScores[i];
          highScores[i] = highScores[i - 1];
          highScores[i - 1] = temp;
          isNewHighScore = true;
          newHighScorePosition = i - 1;
        } else {
          break;
        }
      }
    }
  } else {
    // Add new score if the player is not in the list
    for (int i = 0; i < maxHighScores; i++) {
      if (newScore > highScores[i].score) {
        // Move other scores down
        for (int j = maxHighScores - 1; j > i; j--) {
          highScores[j] = highScores[j - 1];
        }
        // Add new score
        highScores[i].score = newScore;
        strncpy(highScores[i].playerName, players[currentPlayer].fullName, sizeof(highScores[i].playerName) - 1);
        highScores[i].playerName[sizeof(highScores[i].playerName) - 1] = '\0';
        isNewHighScore = true;
        newHighScorePosition = i;
        break;
      }
    }
  }
  saveHighScores();

  if (isNewHighScore) {
    char positionBuffer[33];
    snprintf(positionBuffer, sizeof(positionBuffer), "%d position", newHighScorePosition + 1);

    scrollLCDMessage(highscoreMessagePart1, 1, nullptr, nullptr, 0, true);

    scrollLCDMessage(positionBuffer, 1, nullptr, nullptr, 0, false);
  }
}

void displayHighScores() {
  unsigned long lastRefreshTime = millis();
  const unsigned long refreshInterval = 3000;
  int displayCycleIndex = 0;
  bool firstShow = true;

  while (true) {

    if (highScores[0].score == 0) {
      scrollLCDMessage(noHighScoreMessage, 0);
      displayMenu();
      return;
    }

    if (digitalRead(exitGameButtonPin) == LOW) {
      turnLedsOff();
      displayMenu();
      return;
    }

    // if we remove firstshow, when we enter highscore from main menu, it will display highscores after few seconds
    if (millis() - lastRefreshTime > refreshInterval || firstShow) {
      lcd.clear();
      firstShow = false;
      int firstDisplayIndex = displayCycleIndex % (maxHighScores - 1);
      int secondDisplayIndex = (firstDisplayIndex + 1) % maxHighScores;

      lcd.setCursor(0, 0);
      lcd.print(firstDisplayIndex + 1);
      lcd.print(": ");
      lcd.print(highScores[firstDisplayIndex].playerName);
      lcd.print(" - ");
      lcd.print(highScores[firstDisplayIndex].score);

      lcd.setCursor(0, 1);
      lcd.print(secondDisplayIndex + 1);
      lcd.print(": ");
      lcd.print(highScores[secondDisplayIndex].playerName);
      lcd.print(" - ");
      lcd.print(highScores[secondDisplayIndex].score);

      displayCycleIndex++;
      lastRefreshTime = millis();
    }
  }
}

void displayAboutMessage() {
  int currentMessage = 0;
  unsigned long lastJoystickTime = 0;
  displayOnMatrix(aboutIcon);
  while (true) {
    lcd.clear();
    const char* messagePtr = (const char*)pgm_read_word(&(aboutMessages[currentMessage]));
    scrollLCDMessage(messagePtr, 0, &currentMessage, &lastJoystickTime, aboutMessagesCount);

    if (digitalRead(exitGameButtonPin) == LOW) {
      turnLedsOff();
      displayMenu();
      break;
    }
  }
}

void displayHowToPlayMessage() {
  int currentMessage = 0;
  unsigned long lastJoystickTime = 0;
  displayOnMatrix(questionMark);

  while (true) {
    lcd.clear();
    // show message from progmem
    const char* messagePtr = (const char*)pgm_read_word(&(playMessages[currentMessage]));
    scrollLCDMessage(messagePtr, 0, &currentMessage, &lastJoystickTime, playMessagesCount);
    if (digitalRead(exitGameButtonPin) == LOW) {
      turnLedsOff();
      displayMenu();
      break;
    }
  }
}
void displayOnMatrix(byte icon[8]) {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      bool ledState = bitRead(icon[row], col);
      lc.setLed(0, row, col, ledState);
    }
  }
}
