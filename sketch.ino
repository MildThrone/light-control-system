#define LED_PIN 11
#define PIR_PIN 2
#define LDR_PIN A2
#define IR 3
#define btn 5
#define inputSize 3
int IR_data;
int LED_State = LOW;
int PIR_State = LOW;
int LDR_value;
//interrupt variable;
volatile bool motionSensed = false;
bool systemState = true;  // true for on :: false for off

unsigned long previousTime = millis(); 
//Some time function I want to implement because I have learnt something.
void setup() {
  Serial.begin(9600);
  int inputPins[inputSize] = {LDR_PIN, PIR_PIN, IR};
  for (int i = 0; i < inputSize; i++){
    pinMode(inputPins[i], INPUT);
  }
  pinMode(LED_PIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(IR),
                                        IR_Interrupt,
                                        FALLING);
}
void IR_Interrupt(){
  motionSensed = true;
}

void toggleSysState() {
  if (systemState) {
    systemState = false;
  } else {
    systemState = true;
  }
}

void Switching(int LED){
  //Here for the IR codes// functionality;
  analogWrite(LED, 3);  
}

void motionSensing(int LED, int PIR_Sensor, int LDR){
  Serial.println("Scanning for Motion");
  int PIR_data = digitalRead(PIR_Sensor);  
  
  if(PIR_data == HIGH){
    Serial.println("Motion Detected");
    digitalWrite(LED, HIGH);
    adaptiveBrightness(LED_PIN, LDR_PIN);
//       delay(100);
  } else {
    Serial.println("Motion Not Detected");
    // analogWrite(LED, 0);
    digitalWrite(LED, LOW);
  }
}

void adaptiveBrightness(int LED, int LDR){
  int LDR_val = analogRead(LDR);
  LDR_value = map(LDR_val, 0, 1024, 254, 0);
  delay(10);
  analogWrite(LED, LDR_value); 
}

void loop() {
    // Serial.println(digitalRead(PIR_PIN));
    if(motionSensed){
      motionSensed = false;
      toggleSysState();
      // Switching(LED_PIN);
    }

    if (systemState) {
      motionSensing (LED_PIN, PIR_PIN, LDR_PIN);
      Serial.println("System On");
      delay(2000);
    } else {
      digitalWrite(LED_PIN, LOW);
      Serial.println("System Off");
      delay(2000);
    }
}