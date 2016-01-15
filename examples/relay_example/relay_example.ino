


#define in0 6 // in0
#define in1 7 // in1

#define RELAY_OPEN HIGH
#define RELAY_CLOSED LOW
#define RELAY_DELAY 2000

int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance

void setup() {
 Serial.begin (9600);
 pinMode(in0, OUTPUT);
 pinMode(in1, OUTPUT);
 digitalWrite(in0, RELAY_OPEN); 
 digitalWrite(in1, RELAY_OPEN);
 delay(3000); 
 
}

void loop() {

 digitalWrite(in0, RELAY_CLOSED); 
 Serial.println("IN0 is closed");
 delay(RELAY_DELAY);

 digitalWrite(in1, RELAY_CLOSED); 
 Serial.println("IN1 is closed");
 delay(RELAY_DELAY);


 digitalWrite(in0, RELAY_OPEN); 
 Serial.println("IN0 is open");
 delay(RELAY_DELAY);

 digitalWrite(in1, RELAY_OPEN); 
 Serial.println("IN1 is open");
 delay(RELAY_DELAY);
 
}
