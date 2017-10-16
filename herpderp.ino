#include <stdio.h>

int MAXSIZE = 8;       
int stack[8];
int backup[8];     
int top = -1;
int stackSize;           

int isempty() {

   if(top == -1)
      return 1;
   else
      return 0;
}
   
int isfull() {

   if(top == MAXSIZE)
      return 1;
   else
      return 0;
}

int peek() {
   return stack[top];
}

int pop() {
   int data;
  
   if(!isempty()) {
      data = stack[top];
      top = top - 1;   
      return data;
   } else {
      printf("Could not retrieve data, Stack is empty.\n");
   }
}

int push(int data) {

   if(!isfull()) {
      top = top + 1;   
      stack[top] = data;
   } else {
      printf("Could not insert data, Stack is full.\n");
   }
}

// digital pin 2 has a pushbutton attached to it. Give it a name:
const int ledPin =  13;      // the number of the LED pin
int pushButton2 = 2;
int pushButton3 = 3;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // Set light
  pinMode(ledPin, OUTPUT);
  // make the pushbutton's pin an input:
  pinMode(pushButton2, INPUT);
  pinMode(pushButton3, INPUT);

  push(2);
  push(3);
  push(2);
  
  int i;
  for(i = 0; i <= top; i++) {
    backup[i] = stack[i];
    Serial.println(backup[i]);
  }
  stackSize = top;
}

void reset() {
  int i;
  for(i = 0; i <= stackSize; i++) {
    stack[i] = backup[i];
  }
  top = stackSize;
}

void addNumber() {
  reset();
}

// the loop routine runs over and over again forever:
void loop() {
  
  // read the input pin:
  int buttonState2 = digitalRead(pushButton2);
  int buttonState3 = digitalRead(pushButton3);
  // print out the state of the button:
  if(buttonState2 == 1) {
    if(peek() == pushButton2) {
      Serial.println("Correct");
      pop();
      if(top == -1) {
        digitalWrite(ledPin, HIGH);
        delay(1000);
        addNumber();
        digitalWrite(ledPin, LOW);
      }
    } else {
      reset();
      Serial.println("Fail Fam");
    }
    while(buttonState2 == 1) {
      buttonState2 = digitalRead(pushButton2);
    }
  } else if(buttonState3 == 1) {
    if(peek() == pushButton3) {
      Serial.println("Correct");
      pop();
      if(top == -1) {
        digitalWrite(ledPin, HIGH);
        delay(1000);
        addNumber();
        digitalWrite(ledPin, LOW);
      }
    } else {
      reset();
      Serial.println("Fail Fam");
    }
    while(buttonState3 == 1) {
      buttonState3 = digitalRead(pushButton3);
    }
  }
  
  delay(1);        // delay in between reads for stability
}
