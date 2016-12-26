/* timer module */
// timer on/off state
int timer_on_state = LOW;
// pint 13 indicate if the timer is on or off
const int timer_on_pin = 13;
// reed swith pin
const int switch_pin = 2;
// store last time switch was update
unsigned long previousMillis = 0;

// constant of time which triggers the timer on/off
const unsigned long switch_interval = 3000; // 3 seconds for now
// constant of time which triggers the solenoid on/off
const unsigned long solenoid_delay = 5*1000; // 5 seconds

// solenoid pin
const int solenoid_pin = 8;
// solenoid state
int solenoid_state = LOW;

// timer counting flag
int timer_counting = 0;
// counting down start time
int time_start;
// counting down current time
int time_current;
void setup() {
  // debug console
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(switch_pin, INPUT);
  pinMode(timer_on_pin, OUTPUT);
  pinMode(solenoid_pin, OUTPUT);
  // set initial timer state
  digitalWrite(timer_on_pin, timer_on_state);
  // set initial solenoid_state state
  digitalWrite(solenoid_pin, solenoid_state);
  /* timer */
}

void loop() {
  // put your main code here, to run repeatedly:
  if (timer_on_state == LOW && trigger() == 1) {
    timer_counting = 1;
    time_start = millis();
  }
  
  if (timer_counting == 1) {
    time_current = millis();
    if (time_current - time_start >= solenoid_delay) {
        solenoid_state = HIGH;
        timer_on_state = LOW;
        timer_counting = 0;
        digitalWrite(timer_on_pin, timer_on_state);
        digitalWrite(solenoid_pin, solenoid_state);
        delay(5000);
        solenoid_state = LOW;
        digitalWrite(solenoid_pin, solenoid_state);
    }
  }
}

int trigger() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(switch_pin);
  // debug
  Serial.println(reading);
  if (reading == HIGH) {
      previousMillis = millis();
  }
  while (reading == HIGH) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= switch_interval) {
      timer_on_state = HIGH;
      digitalWrite(timer_on_pin, timer_on_state);
      return 1;
    }
    reading = digitalRead(switch_pin);
  }
  return 0;
}

