const int ledPin = 13;
volatile byte state = LOW;
int count = 0;
int secondCounter = 0;
int count16 = 0;
int count16B = 0;

void init16bit() {
  
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 62500;            // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
}

void setup() {
   Serial.begin(9600);
   Serial.println("Setup completed");
   noInterrupts();
   pinMode(ledPin, OUTPUT);
   TIMSK2 = (TIMSK2 & B11111110) | 0x01;      //Last bit of TIMSK2 is high to enable overflow interrupt
   TCCR2B = (TCCR2B & B11111000) | 0x07;      //1024 prescalar
   init16bit();
   TCNT1 |= 0;
   TCNT2 |= 0;
   interrupts();
}
 
void loop() {
  
  digitalWrite(ledPin, state);
  
  if(secondCounter == 5) {
    Serial.println("5 Seconds have passed");
    secondCounter = 0;   
  }
}

ISR(TIMER1_COMPA_vect) {
  count16++;
  if(count16 >= 1) {
    Serial.println("IN 16 bit ISR");
    count16 = 0; 
  }  
}


