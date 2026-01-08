#include <Stepper.h>

const int stepsPerRev = 250;
const int iterations = 2; 

Stepper myStepper(stepsPerRev, 3, 4);
int outputPin = 9;
int inputPin = 8;
int basisPin = 6; //Bob sends his basis to alice
int correctPin = 7;
int bitsPin = 5;
int finishedPin = 10;


int lightPin0 = 11;
int lightPin1 = 12;
int laserVal = 200;

void setup() {
  // put your setup code here, to run once:
  myStepper.setSpeed(50);
  Serial.begin(9600);
  randomSeed(analogRead(A4));
  pinMode(inputPin,INPUT);
  pinMode(outputPin,OUTPUT);
  pinMode(correctPin,INPUT);
  pinMode(basisPin,OUTPUT);
  pinMode(bitsPin,OUTPUT);
  pinMode(finishedPin,INPUT);

  digitalWrite(correctPin,LOW);
  digitalWrite(inputPin,LOW);
  digitalWrite(outputPin,LOW);
  digitalWrite(basisPin,LOW);
  digitalWrite(bitsPin,LOW); 
}
void send(){
  digitalWrite(outputPin, HIGH);
  digitalWrite(outputPin, LOW);
 // delay(100);
}
void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}
void loop() {
  int basis[iterations];
  int key[iterations];
  int bits[iterations];
  int matchingBases[iterations];

  int count = 0;
  int randNumber;
  digitalWrite(inputPin,LOW);
  digitalWrite(outputPin,LOW);
  Serial.println("start");
  
  for (int i = 0; i < iterations; i++){
    randNumber = random(2);
    basis[i] = randNumber;
  }

  for (int i = 0; i < iterations; i++){  
    // wait for user to enter message
    while (digitalRead(inputPin) == LOW){}

    int turn = 0;
    if(basis[i] == 0){
      turn = 0;
    }
    else{
      turn = 250;
      myStepper.step(turn);
    }

    delay(2500); //however long it takes for both motors to finish

    while (digitalRead(inputPin) == LOW){
    }
    while (digitalRead(lightPin0) == LOW && digitalRead(lightPin1) == LOW){
      if(digitalRead(lightPin0) == HIGH && digitalRead(lightPin1) == LOW){ //sensor0 is true, sensor1 is false
        bits[i] = 0;
        break;
      }
      else if (digitalRead(lightPin0) == LOW && digitalRead(lightPin1) == HIGH){ //sensor0 is false, sensor2 is true
        bits[i] = 1;
        break;
      }
      else if (digitalRead(lightPin0) == LOW && digitalRead(lightPin1) == LOW){ //sensor0 is true, sensor2is true
        bits[i] = random(2);
        break;
      }
      else { // none are true
        delay(100);
      }
    }
  
    myStepper.step(-turn);
    send(); // tell alice that bob is done reading
    Serial.println(i);
    delay(1000);
  }//end for loop
  //while (digitalRead(inputPin) == LOW){}

  for(int i = 0; i < iterations; i++){
    Serial.println("Basis:");
    //digitalWrite(outputPin,LOW);
    delay(500); //

    if (basis[i]==1){
      digitalWrite(basisPin,HIGH);
    }
    else{
      digitalWrite(basisPin,LOW);

    }
    if (bits[i]==1){
      digitalWrite(bitsPin,HIGH);
    }
    else{
      digitalWrite(bitsPin,LOW);

    }
    digitalWrite(outputPin,HIGH);

    while (digitalRead(inputPin) == LOW){
    }
        digitalWrite(outputPin,LOW);

    if((digitalRead(correctPin) == HIGH && basis[i]==1) || digitalRead(correctPin)==LOW && basis[i]==0){
      key[count] = basis[i]; //change
      matchingBases[count] = basis[i]; //debugging purposes
      count++;
      Serial.print(basis[i]);
      Serial.print(basis[i]);
    }
    else{
      Serial.print("wrong!");
      Serial.print(basis[i]);

      //correctPin == LOW;
    }    
    //digitalWrite(outputPin,LOW);
  }
  delay(10);
  //HERE
  int encrypted[8]; //decrypted message

  int y = 0;

  int t = 0;
  int place[8];
  char finalMessage[y/8];

  while (true){
    //Serial.println("the buss");
    if(digitalRead(finishedPin) == HIGH) break;
    while (digitalRead(inputPin) == LOW){}
   // Serial.println("ok m");

    delay(50);
    if((key[y%count]== 1 && digitalRead(correctPin)== HIGH)||(key[y%count]== 0 && digitalRead(correctPin) == LOW)){
      encrypted[y%8] = 0;
    }
    else{
      encrypted[y%8] = 1;
    }
    int c = encrypted[y%8];
    //Serial.println();
    place[y%8] = c; 
    if (t==7){   
      
      char dest_char = 0; // the output, 'H' -- must initialize all bits to 0
      for (int source_bit_pos = 7; source_bit_pos >= 0; source_bit_pos--) // start from rightmost position
      {
        //char c = place[source_bit_pos];
        int the_bit = place[source_bit_pos]; // convert
        dest_char |= the_bit<<(7-source_bit_pos); 
      }
      finalMessage[y/8] = dest_char;
    }
    y++; 
    t++;
    if(t == 8){
      t=0;
    }
    send();
    delay(50);
  }
  // Serial.println("message received");
  // for (int i = 0; i < y; i++){
  //   Serial.print(encrypted[i]);
  // }
Serial.println("Message received!");
 for (int i = 0; i < y/8; i++){
    Serial.print(finalMessage[i]);
  }
  


  /*
  int len[8];
  for(int i = 0; i < 8; i++){
    if(digitalRead(correctPin) == HIGH){
      len[i] = 1;
    }
    else{
      len[i] =0;
    }
  }*/
  Serial.println();
  Serial.println("original bases");
  for (int i = 0; i < iterations; i++){
    Serial.print(basis[i]);
  }
  Serial.println();
  Serial.println("keys");
  for(int i = 0; i< count; i++){
    Serial.print(key[i]);
  }
  Serial.println();
  Serial.println("matching bases");
  for (int i = 0; i < count; i++){
    Serial.print(matchingBases[i]);
  }
 Serial.println();
  
 

  

  //resetFunc();


}
