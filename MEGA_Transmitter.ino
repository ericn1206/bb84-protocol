#include <Stepper.h>

String message;
const int stepsPerRev = 200;
const int iterations = 8; 

Stepper myStepper(stepsPerRev, 3, 4);
int laserPin = 10;
int outputPin = 33;
int inputPin = 31;
int basisPin = 29;
int correctPin = 27;
int bitPin = 25;
int finishedPin = 23;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  randomSeed(analogRead(A5));

  pinMode(inputPin, INPUT);
  pinMode(outputPin, OUTPUT);
  pinMode(correctPin, OUTPUT);
  pinMode(basisPin, INPUT);
  pinMode(bitPin, INPUT);
  pinMode(finishedPin, OUTPUT);

  digitalWrite(correctPin, LOW);
  digitalWrite(inputPin, LOW);
  digitalWrite(outputPin, LOW);
  digitalWrite(basisPin, LOW);
  digitalWrite(finishedPin, LOW);

  myStepper.setSpeed(200); //rpm

  pinMode(laserPin, OUTPUT);

  //generate random bases
  
  //printing out random basis
  
}
//01000011
//01010010
//010001
void send(){
  digitalWrite(outputPin, HIGH);
  digitalWrite(outputPin, LOW);
  //delay(100);
}

void loop() {
  int basis[iterations];
  int matchingBases[iterations];

  int bits[iterations];
  int key[iterations];
  int count = 0;
  int randNumber;

  //generate random bases
  for (int i = 0; i < iterations; i++){
    randNumber = random(2);
    basis[i] = randNumber;
  }
  //generate random bits
  for (int i = 0; i < iterations; i++){
    randNumber = random(2);
    bits[i] = randNumber;
  }

  
  
  Serial.println("Enter secret message...");  
  while(Serial.available()==0) { // Wait for User to Input Data
  }
  message = Serial.readString();   //Read the data the user has input
  message.trim();  
  
  //convert message into binary
  byte letters[message.length() * 8];
  int x=0;
  for(int i=0; i<message.length(); i++){
   char myChar = message.charAt(i);
    for(int i=7; i>=0; i--){
      byte bytes = bitRead(myChar,i);
      letters[x] = bytes;
      x++;
    }
 }
  
 for(int i = 0; i < message.length()*8; i++){
  Serial.print(letters[i]);
 }
  

for (int i = 0; i < iterations; i++){
  digitalWrite(laserPin, LOW);
  send();
  int turn = 250; 
  if (basis[i]==0 && bits[i] == 0){ // + and 0
    turn = 0;
    myStepper.step(turn);
  }
  else if (basis[i]==0 && bits[i] == 1){ // + and 1
    turn = 250;
  }
  else if (basis[i]==1 && bits[i] == 0){ // x and 0
    turn = -250;
    myStepper.step(turn);
  }
  else { // + and 1
    turn = 250*2;
    myStepper.step(turn);
  }
  Serial.println("alice is moving!");
    
  digitalWrite(laserPin, HIGH);
  Serial.println("laser ready"); //alice says laser is ready
  
  digitalWrite(outputPin, HIGH);
  while(digitalRead(inputPin)==LOW){ //wait for bob to say done reading 
    
  }
  digitalWrite(outputPin, LOW);
  Serial.println(i); 
  myStepper.step(-turn); //reset motor
  delay(500); 
  digitalWrite(laserPin, LOW);
  delay(500);


  }
  send();
  Serial.print("testings!");

  //compare basis with bob
  digitalWrite(outputPin,LOW);
  bool eve = false;
  for(int i = 0; i< iterations; i++){
    digitalWrite(outputPin,LOW);
    //digitalWrite(correctPin, LOW);
    Serial.print("TWO testing!");

    while (digitalRead(inputPin)==LOW){}

    Serial.print("received!"); //bob sent basis and bit
    Serial.print(digitalRead(basisPin));
    Serial.print(digitalRead(bitPin));

    Serial.print(basis[i]);
    Serial.println();
    delay(50);
    if ((digitalRead(basisPin) == HIGH && basis[i]== 1)|| (digitalRead(basisPin)==LOW && basis[i]==0)){
      key[count] = basis[i];  //CHANGE
      matchingBases[count] = basis[i]; //debugging purposes
      /*
      //check for eve
      if ((bits[i] == 0 && digitalRead(bitPin) == HIGH) || (bits[i]==1 && digitalRead(bitPin) == LOW)){
        eve = true;
      }
      */
      count++;
      //digitalWrite(correctPin, HIGH);
      Serial.println("matching basis");
    }
    else{
      //digitalWrite(correctPin, LOW);
      Serial.println("not matching basis");

    }
    if(basis[i]==1){
      digitalWrite(correctPin, HIGH);
    }
    else{
      digitalWrite(correctPin, LOW);
    }
    Serial.println("not matching basis");
    send();
  }
  /*
  if (eve){
    Serial.println("WARNING: Eavesdropper detected.");
  }
  */

  delay(500);
  //count = key length
  //len = number of total bytes
  int len = message.length() * 8;
  int encrypt[len];
  for (int i = 0; i < len; i++){
    if (key[i%count] == letters[i]){
      encrypt[i] = 0;
    }
    else{
      encrypt[i] = 1;
    }
  }
  
  for (int i = 0; i < len; i++){
    delay(60); //do not remove delay >100
    send();
    digitalWrite(outputPin,HIGH);
    if (encrypt[i]== 1){
      digitalWrite(correctPin, HIGH);
    }
    else{
      digitalWrite(correctPin, LOW);
    }
    while (digitalRead(inputPin)==LOW){}
    digitalWrite(outputPin,LOW);


  }
  digitalWrite(finishedPin, HIGH);

  /*
  for(int i = 0; i < len; i++){
    if(encypt[i] == 1){
      digitalWrite(correctPin, HIGH);
    }
    else{
      digitalWrite(correctPin, LOW);
    }
  }
  
  for(int i = 0; i < 8; i++){
    //send len into binary form
    
  }
  //send len value
  */
  //debugging

  Serial.println();
  Serial.println("original bases");
  for (int i = 0; i < iterations; i++){
    Serial.print(basis[i]);
  }
  Serial.println();
  Serial.println("key");
  for (int i = 0; i < count; i++){
    Serial.print(key[i]);
  }
  Serial.println("matching bases");
  for (int i = 0; i < count; i++){
    Serial.print(matchingBases[i]);
  }
  Serial.println("before encryption");
  for (int i = 0; i < len; i++){
    Serial.print(letters[i]);
  }
  Serial.println();
  Serial.println("encryption");
  for (int i = 0; i < len; i++){
    Serial.print(encrypt[i]);
  }
  
  
  //resetFunc();
  /*
  0110101101101011
  11110111101111011110
  1001110011010110
  1001110011010110
  */
  //send encrypted message to Bob
}
