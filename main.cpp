#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


//// Definitions
// Screen Variables
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Time tracking
unsigned long gameTime;

// Score tracking
int scoreP1 = 0;
int scoreP2 = 0;
const int WINNING_SCORE = 2;

const int PADDLE_WIDTH = 2;
const int PADDLE_HEIGHT = 12;

// Position tracking for player 1 paddle
int P1_X = 0;
int P1_Y = 32;
// Position tracking for player 2 paddle
int P2_X = 128 - PADDLE_WIDTH;
int P2_Y = 32;
// Position tracking for ball
int ballX = 64;
int ballY = 32;

// Speed 1
int DP1_Y = 5;

// Button polling 1
const int BTN_P1 = 4;
bool lastBtnP1 = HIGH;

// Speed 2
int DP2_Y = 5;

// Button polling 2
const int BTN_P2 = 2;
bool lastBtnP2 = HIGH;

// Ball speed
int dball_x = 2;
int dball_y = 2;

// Time variables
int time_min = 0;
int time_s = 0;


enum GameState {
  GAMEPLAY,
  GAME_OVER,
  PAUSE
};

// Starting state
GameState state = GAMEPLAY;

//// Setup Code
void setup() {
  Wire.setWireTimeout(10000);
  // put your setup code here, to run once:
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  pinMode(BTN_P1, INPUT_PULLUP);
  pinMode(BTN_P2, INPUT_PULLUP);

  pinMode(LED_BUILTIN, OUTPUT);
  // Declares our button1Pin as an input
  /* INPUT_PULLUP will by default set our pin to HIGH and 
  will be set LOW when the button is pressed*/
}

void loop() {
  // Display must first be cleared
  display.clearDisplay();

  if (state == GAME_OVER) {
    display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
    Serial.println("displaying gameover screen");
    display.setTextSize(1);
    display.setCursor(40, 20);
    display.print("GAME OVER");
    display.setCursor(40, 30);
    display.print("Winner: ");
    if (scoreP1 >= WINNING_SCORE) {
      display.print("Player 1");
    }
    else {
      display.print("Player 2");
    }
    delay(1000);
  }

  else if (state == GAMEPLAY) {
    // Get time
    gameTime = millis();
    time_s = (gameTime - gameTime % 1000) / 1000;
    if (time_s >= 60) {
      time_s -= 60;
    }
    time_min = (gameTime - gameTime % 60000) / 60000;

    // Sets up text for the player scores and time
    display.setTextSize(1);
    display.setCursor(40, 0);
    display.print("P1:");
    display.print(scoreP1);
    display.print("  P2:");
    display.print(scoreP2);
    display.setCursor(50, 10);
    display.print(time_min);
    display.print(":");
    display.print(time_s);
    display.setCursor(40, 15);

    // Set up paddles
    display.fillRect(P1_X, P1_Y, PADDLE_WIDTH, PADDLE_HEIGHT, SSD1306_WHITE);
    display.fillRect(P2_X, P2_Y, PADDLE_WIDTH, PADDLE_HEIGHT, SSD1306_WHITE);
    // Set up ball
    display.fillRect(ballX, ballY, 2, 2, SSD1306_WHITE);


    // ---------- Player 1 Controls
    bool btnP1 = digitalRead(BTN_P1);
    // digitalRead() takes in the button input
    // LED_BUILT_IN is the LED built into every Arduino Board. It can also be accessed by digitalPin 13

    // Paddle-arena collision
    if (P1_Y <= 0 || P1_Y >= SCREEN_HEIGHT - PADDLE_HEIGHT) {
      // constrain prevents the paddle from extending beond the screen
      P1_Y = constrain(P1_Y, 0, SCREEN_HEIGHT - PADDLE_HEIGHT);
      DP1_Y = 0;
      Serial.println("border");
    }
    if (btnP1 == LOW && lastBtnP1 == HIGH) {
      // If the paddle is not moving at the bottom, set dir +ve. Else if paddle is not moving, set dir -ve.
      DP1_Y *= -1;
      if (DP1_Y == 0) {
        DP1_Y = (P1_Y <= 0) ? 5 : -5;
      }
      Serial.println("Speed change");
    }
    lastBtnP1 = btnP1;

    // Paddle movement
    P1_Y += DP1_Y;  // This is what actually updates the paddle location


    // ---------- Player 2 Controls
    bool btnP2 = digitalRead(BTN_P2);
    // digitalRead() takes in the button input
    // LED_BUILT_IN is the LED built into every Arduino Board. It can also be accessed by digitalPin 13
    // Paddle-arena collision
    if (P2_Y <= 0 || P2_Y >= SCREEN_HEIGHT - PADDLE_HEIGHT) {
      // constrain prevents the paddle from extending beond the screen
      P2_Y = constrain(P2_Y, 0, SCREEN_HEIGHT - PADDLE_HEIGHT);
      DP2_Y = 0;
      Serial.println("border");
    }
    if (btnP2 == LOW && lastBtnP2 == HIGH) {
      // If the paddle is not moving at the bottom, set dir +ve. Else if paddle is not moving, set dir -ve.
      DP2_Y *= -1;
      if (DP2_Y == 0) {
        DP2_Y = (P2_Y <= 0) ? 5 : -5;
      }
      Serial.println("Speed change");
    }
    lastBtnP2 = btnP2;

    // Paddle movement
    P2_Y += DP2_Y;  // This is what actually updates the paddle location

    ////BALL

    // ((ballX <= (0 + PADDLE_WIDTH)) && (ballY >= (P1_Y - PADDLE_HEIGHT)) && (ballY <= (P1_Y))) || (((ballX + 2) >= (128 - PADDLE_WIDTH)) && (ballY >= (P2_Y - PADDLE_HEIGHT)) && (ballY <= (P2_Y)))
    //if collide with player, go in opposite x direction, variable y direction
    if ((ballX == P1_X + PADDLE_WIDTH) && (ballY + 2 <= P1_Y + PADDLE_HEIGHT) && (ballY >= P1_Y)) {
      dball_x *= -1;
      Serial.println("ball collision");
    }
    else if ((ballX + 2 == P2_X) && (ballY + 2 <= P2_Y + PADDLE_HEIGHT) && (ballY >= P2_Y)) {
      dball_x *= -1;
      Serial.println("ball collision");
    }
    // if collide with arena, go in opposite y direction
    if ((ballY >= SCREEN_HEIGHT) || (ballY <= 0)) {
      dball_y *= -1;
    }
    // if ball outside screen, game over
    if (ballX < 0) {
      ballX = 64;
      scoreP2++;
    }
    else if (ballX > 128) {
      ballX = 64;
      scoreP1++;
    }

    if ((scoreP1 >= WINNING_SCORE) || (scoreP2 >= WINNING_SCORE)) {
      state = GAME_OVER;
      Serial.print("game over");
    }

    // ball movement
    ballX += dball_x;
    ballY += dball_y;


    display.display();
    // Displays everything
    delay(150);

    /* Debugging
    Serial.print("Ball x: ");
    Serial.println(ballX);
    Serial.print("P1 X: ");
    Serial.println(P1_X);
    Serial.print("P2 X: ");
    Serial.println(P2_X);
    */
  }
}
