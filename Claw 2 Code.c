#define VCC_PIN 13
#define TRIGGER_PIN 12    // sonar trigger pin will be attached to Arduino pin 12
#define ECHO_PIN 11       // sonar echo pint will be attached to Arduino pin 11
#define GROUND_PIN 10     //
#define MAX_DISTANCE 200  // maximum distance set to 200 cm

// defines variables
long duration;
int distance;

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

#define OPEN_POSITION 175
#define CLOSED_POSITION 80
#define CHANGE_DISTANCE 3

int clawIsOpen = 1;

void setup() {
  // begin serial port
  Serial.begin (9600);

  // configure pins for sonar
  pinMode(ECHO_PIN, INPUT) ;  
  pinMode(TRIGGER_PIN, OUTPUT) ;
  pinMode(GROUND_PIN, OUTPUT);    // tell pin 10 it is going to be an output
  pinMode(VCC_PIN, OUTPUT);       // tell pin 13 it is going to be an output
  digitalWrite(GROUND_PIN,LOW);   // tell pin 10 to output LOW (OV, or ground)
  digitalWrite(VCC_PIN, HIGH) ;   // tell pin 13 to output HIGH (+5V)

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

}

void loop() {

  digitalWrite(TRIGGER_PIN, LOW);   // Clears the trigPin
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);  // Sets the trigPin on HIGH state for 10 micro seconds
  delayMicroseconds(20);
  digitalWrite(TRIGGER_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration*0.034/2;       // Calculating the distance

  // if the distance measurement becomes extremely high, it is likely an error.  
  // Default to a maximum value of MAX_DISTANCE to identify this condition.
  if (distance > MAX_DISTANCE)   
    {distance = MAX_DISTANCE ;}

  Serial.print("Distance: ") ;      // Prints the distance on the Serial Monitor
  Serial.println(distance);

  // closes open claw if measured distance is less than CHANGE_DISTANCE
  if (clawIsOpen == 1 && distance <= CHANGE_DISTANCE) {
    for (pos = OPEN_POSITION; pos >= CLOSED_POSITION; pos--) { // goes from 0 degrees to 180 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    }

    clawIsOpen = 0;
    delay(5000);                       // keep claw closed for 5 seconds before measuring position again
  }

  // opens closed claw if measured distance is less than CHANGE_DISTANCE
  else if (clawIsOpen == 0 && distance <= CHANGE_DISTANCE) {
    for (pos = CLOSED_POSITION; pos <= OPEN_POSITION; pos += 1) { // goes from 0 degrees to 180 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    }
    
    clawIsOpen = 1;
    delay(5000);                       // keep claw closed for 5 seconds before measuring position again
  }
}