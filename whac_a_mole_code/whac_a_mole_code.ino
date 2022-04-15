#include <Servo.h>
Servo myServo;

// assign LEDs and button and potentiometer to pins
int ledPin[] = {4,5,6,11,12,13};
int playerOneButton = 2;
int playerTwoButton = 3;
int whiteLED[] = {7,9};
const int buzzer = 8;

// declare variables
int delayTime; // time delay between lights on/off
int randNumber1;
int randNumber2;
int whiteLEDOn;
int playerOneScore;
int playerTwoScore;
int player;
int counter;
int maxScore = 10;
int correctPress;
int pos = 0; //Variable to store the servo position
int val = 0;


//setup servo, buzzer, interrupts, button input and LED outputs
void setup() {
  myServo.attach(10);
  myServo.write(90);
  attachInterrupt(0, playerOneInput, FALLING); // specify interrupt routine for button 1
  attachInterrupt(1, playerTwoInput, FALLING); // specify interrupt routine for button 2
  for (int i=0; i<6; i++){
    pinMode(ledPin[i], OUTPUT);
  }
  for (int i=0; i<2; i++){
    pinMode(whiteLED[i], OUTPUT);
  }
  pinMode(playerOneButton, INPUT);
  pinMode(playerTwoButton, INPUT);
  pinMode(buzzer, OUTPUT);
}

 

//run main program loop
void loop() {
  randNumber1 = random(3); // select a random number for player 1 leds
  randNumber2 = random(4, 6); // select a random number for player 2 leds
  // reads value from potentiometer and changes delay between LEDs turning off and on(difficulty level) accordingly
  delayTime = analogRead(A2);
  delayTime = map(delayTime, 1, 1024, 250, 1000);
  // turns coloured LEDs on and off
  digitalWrite(ledPin[randNumber1], HIGH);
  digitalWrite(ledPin[randNumber2], HIGH);
  delay (delayTime);
  digitalWrite(ledPin[randNumber1], LOW);
  digitalWrite(ledPin[randNumber2], LOW);
  delay (delayTime); 
  //if whiteLED on = turn it off
  for (int i=0; i<2; i++){
    whiteLEDOn = digitalRead(whiteLED[i]);
    if(whiteLEDOn==HIGH){
      digitalWrite(whiteLED[i], LOW);
    }
  }
}


 //funtion for end game
void endGame(){
  // turn off all LEDs for end game sequence
  for (int i=0; i<6; i++){
    digitalWrite(ledPin[i], LOW);
  }
  for (int i=0; i<2; i++){
    digitalWrite(whiteLED[i], LOW);
  }
  // sees which player has won
  if (playerOneScore > playerTwoScore){
    player = 0;
  }
  else{
    player = 1;
  }
  //flashes the LEDs of the winning player 6 times
  for (int i=0; i<6; i++){
      digitalWrite(whiteLED[player], HIGH);
      for (int i=(player*3); i<((player+1)*3); i++){
        digitalWrite(ledPin[i], HIGH);
      }
      delay(50000);
      digitalWrite(whiteLED[player], LOW);
      for (int i=(player*3); i<((player+1)*3); i++){
        digitalWrite(ledPin[i],LOW);
      }
      delay(50000);
    }
   
  //resets player scores before restarting the game
  playerOneScore = 0;
  playerTwoScore = 0;
  //resets servo
  myServo.write(90);
  delay(50000);
}

// function called by player one button interrupt 
void playerOneInput() {
  playerInput(0); //passes the player identifier 0 into playerInput
}


// function called by player two button interrupt 
void playerTwoInput(){
  playerInput(1); //passes the player identifier 1 into playerInput
}


// function that handles button presses and assessing score
void playerInput(int player){
  for (int i=(player*3); i<((player+1)*3); i++){
    // checks the player's 3 coloured LEDs to see if one is on AND that both of the white LEDs are off
    if (digitalRead(ledPin[i])== HIGH && digitalRead(whiteLED[1-player]) == LOW && digitalRead(whiteLED[player]) == LOW){
      digitalWrite(whiteLED[player], HIGH); // if it was a valid button press then turn on the player's white LED
      // increments points
      if (player == 0){
        playerOneScore += 1;
      }
      else {
        playerTwoScore += 1;
      }
      // moves servo to show who is in the lead
      if (playerOneScore > playerTwoScore){
        myServo.write(135);   
      }
      else if (playerTwoScore > playerOneScore) {
        myServo.write(45);
      }
      else {
        myServo.write(90);
      }
      //checks to see if a player has won the game - if so runs end game function
      if (playerOneScore == maxScore || playerTwoScore == maxScore){
        endGame();
      }
      else {
        // plays tone indicating which player got the point
        if (player == 0){
          tone(8, 100, 500);
        }
        else{
          tone(8, 1000, 500);
        }
      }    
  }
 }
}
