//Put desired values of off time and on time.
int ledOnTime = 6;
int ledOffTime = 3;

int startHour = 12;
int endHour = 36;

int ledPin1 =  13;
int ledPin2 = 12;
int ledPin3 = 11;
int ledPin4 = 10;
int ledPin5 = 9;
int ledPin6 = 8;
int ledPin7 = 5;
int ledPin8 = 6;

int count16bit = 0;
int count8bit = 0;
int hourCounter = 0;
int secondCounter = 0;
int secCounter = 0;

//Condition flags  
bool ledOn = 0;
bool ledOff = 1;
bool twelvehrFlag = 0;              //Set to 1 if want to start the process imediately.     
bool state = 0;
bool timeEnd = 0;

void initTimer2() {
  TCCR2B |= (TCCR2B & B11111000) | 0x07;
  TIMSK2 |= (TIMSK2 & B11111110) | 0x01;
}

void init16bit() {
  
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 62500;            // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
}

ISR(TIMER2_OVF_vect){
  count8bit++;
  if(count8bit >= 30) {
   secCounter++;
   count8bit = 0;
  }
  
  if((secCounter == ledOffTime) && (ledOff)) {
    state = 1;
    ledOn = 1;
    ledOff = 0;
    secCounter = 0;
  }
  
  if((secCounter == ledOnTime) && (ledOn)) {
    state = 0;
    ledOn = 0;
    ledOff = 1;
    secCounter = 0;
  }
}

// timer compare interrupt service routine for timer 1
ISR(TIMER1_COMPA_vect) {
  secondCounter++;
  if(secondCounter >= 3600) {
    secondCounter = 0;
    hourCounter++;
  }
  
  if(hourCounter == startHour) {
      twelvehrFlag = 1;
  }
  
  if(hourCounter == endHour) {
    timeEnd = 1;
    state = 0;
    blinkLED();
    noInterrupts();
  }
}

void blinkLED() {  
  digitalWrite(ledPin1, state);
  digitalWrite(ledPin2, state);
  digitalWrite(ledPin3, state);
  digitalWrite(ledPin4, state);
  digitalWrite(ledPin5, state);
  digitalWrite(ledPin6, state);
  digitalWrite(ledPin7, state);
  digitalWrite(ledPin8, state);
}

void setup() {
  Serial.begin(9600);
  pinMode(ledPin1, OUTPUT);  
  pinMode(ledPin2, OUTPUT);  
  pinMode(ledPin3, OUTPUT);  
  pinMode(ledPin4, OUTPUT);  
  pinMode(ledPin5, OUTPUT);  
  pinMode(ledPin6, OUTPUT);  
  pinMode(ledPin7, OUTPUT);  
  pinMode(ledPin8, OUTPUT);  
  
  noInterrupts();
 
  init16bit();    // initialize timer1
  initTimer2();     // Timer2 initialize
  interrupts();
}

void loop() {
  
 if(twelvehrFlag) {
  if(timeEnd == 0) {
    blinkLED();
  }  
 } 
}
