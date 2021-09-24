int ledPin =  13;
int count16bit = 0;
int count8bit = 0;
int hourCounter = 0;
int secondCounter = 0;
int secCounter = 0;

bool ledOn = 0;
bool ledOff = 1;
bool twelvehrFlag = 0;      //Twelve hour Flag
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
  
  if((secCounter == 1) && (ledOff)) {
    state = 1;
    ledOn = 1;
    ledOff = 0;
    secCounter = 0;
  }
  
  if((secCounter == 1) && (ledOn)) {
    state = 0;
    ledOn = 0;
    ledOff = 1;
    secCounter = 0;
  }
}

// timer compare interrupt service routine for timer 1
ISR(TIMER1_COMPA_vect) {
  secondCounter++;
  if(secondCounter >= 5) {
    secondCounter = 0;
    hourCounter++;
  }
  
  if(hourCounter == 2) {
      twelvehrFlag = 1;
  }
  
  if(hourCounter == 8) {
    timeEnd = 1;
    state = 0;
    blinkLED();
    noInterrupts();
  }
}

void blinkLED() {  
  digitalWrite(ledPin, state);
}

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);  
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
