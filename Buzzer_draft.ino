#define USE_ARDUINO_INTERRUPTS true 
#include <WiFi.h>
#include <WiFiUdp.h>
#include <PulseSensorPlayground.h> 

// Replace with your network credentials
const char* ssid     = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

const int buzzerPin = 33;     // pin to which the buzzer is connected

int const pulseSensorPin = 36; // pin to which the pulsemeter is connected
int pulseSignal;              // data received from the pulsemeter
int pulseThreshold = 550;     // threshold that determines, which "tump" to consider as a heartbeat 
int lowerPulse = 600;         // threshold for the lower boundary of the pulse
int higherPulse = 800;        // threshold for the higher boundary of the pulse
PulseSensorPlayground pulseSensor;

int boardLED = 13;            // On-board arduino LED 
int greenLED = 26;             // pin to which the green LED is connected
int redLED = 25;               // pin to which the red LED is connected

int microphonePin = 34;        // pin to which the microphone is connected
int soundThreshold = 100;     // threshold used for averaging the "noisy" sound received from the microphone
int clappingThreshold = 800;  // threshold used to detect if the sound received by the microphone is a clap (or could be something else)

int gameButton = 27;           // pin to which the button used for the mental stimulation game is connected
int lastStateGame = LOW;      // last state of the gameButton (initally set to LOW)
int muteButton = 14;           // pin to which the button used for muting the mental stimulation game is connected
int lastStateMute = LOW;      // last state of the muteButton (initally set to LOW)


// put your setup code here, to run once:
void setup() {

  // WIFI setup
  // Serial.print("Connecting to ");
  // Serial.println(ssid);
  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  // // Print local IP address and start web server
  // Serial.println("");
  // Serial.println("WiFi connected.");
  // Serial.println("IP address: ");
  // Serial.println(WiFi.localIP());
  
  // set all of the sensors connected to specific pins
  pinMode(buzzerPin, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(gameButton, INPUT_PULLUP);
  pinMode(muteButton, INPUT_PULLUP);
  Serial.begin(115200);
  
  pulseSensor.analogInput(pulseSensorPin);
  pulseSensor.blinkOnPulse(greenLED);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(pulseThreshold);
  
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
  
  delay(2000);
  Serial.print("ESP32 started");

}
// put your main code here, to run repeatedly:
void loop() {
  
  // code used for the pulsemeter
  int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
                                               // "myBPM" hold this BPM value now. 

  if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened". 
  Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
  Serial.print("BPM: ");                        // Print phrase "BPM: " 
  Serial.println(myBPM);                        // Print the value inside of myBPM. 
  }

  delay(20);  
  
  // pulseSignal = analogRead(pulseSensorPin);
  // Serial.println(pulseSignal);

  // if(pulseSignal > pulseThreshold) {
  //   Serial.println("Pulse detected: " + pulseSignal);

  //   if(pulseSignal > higherPulse) {
  //     digitalWrite(redLED,HIGH);
  //     Serial.println("Your pulse is too high: " + pulseSignal);

  //   } else if(pulseSignal < lowerPulse) {
  //     digitalWrite(redLED,LOW);
  //     Serial.println("Your pulse is too low: " + pulseSignal);
  //   } 
  // } 
  // delay(100);

  // code used for the microphone
  long sum = 0;

  for(int i = 0; i < soundThreshold; i++) { // analog-to-digital converter
    sum += analogRead(microphonePin); // detect and store the voltage in the sum
  }

  sum = sum/soundThreshold; // to removed some noise, create an average

  if (sum > clappingThreshold) {  // detects if the sound received from the user is a clap
    Serial.println("A clap was detected");
  } 
  delay(100);

  // code used for the buttons
  int currentStateGame = digitalRead(gameButton);
  int currentStateMute = digitalRead(muteButton);
  
  if(lastStateGame == LOW && currentStateGame == HIGH) {  // detects if the button is clicked 
    Serial.println("The game button was clicked !!");
    lastStateGame = currentStateGame;    
  }

  if (lastStateMute == LOW && currentStateMute == HIGH) {
     Serial.println("The mute button was clicked !!");
     lastStateMute = currentStateMute;
  }
  delay(100);

  // code used for the buzzer
  tone(buzzerPin, 1000);    //1kHz
  Serial.print("Buzzzzin");
  delay(1000);              // wait 1 second
  
  tone(buzzerPin, 100);     // 100 Hz
  Serial.print("Buzzzzin");
  delay(2000);              // wait 2 seconds
}