#define BEEPER_PIN 3
#define BUTTON_PIN 2

typedef enum controllerState {NOT_STARTED, STARTING, RUNNING, BEEPING, STOPPED} controllerState;

controllerState state = NOT_STARTED;



byte toggler = 1; //basic, multi-use toggler for the ISR
SIGNAL(TIMER1_COMPA_vect){ //the main reason for using an ISR is so I can call delays and not have to worry (working hard so I can be lazy - yknow)
  if (state == STARTING){
    digitalWrite(BEEPER_PIN, HIGH);
  }
  if (state == BEEPING) {
    digitalWrite(BEEPER_PIN, toggler - 1);
    toggler = ~toggler;
  }
  if (state == RUNNING || state == NOT_STARTED || state == STOPPED){
    digitalWrite(BEEPER_PIN, LOW);
  }
//  Serial.println("Interrupted");
}


void setup() {
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0; //initialize registers to zero
  OCR1A = 1000; //whatever, I don't really care about the interrupt interval I just want someone to tell me to do things
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  
  pinMode(BEEPER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(9600);
  
}

void loop() {
  if (digitalRead(BUTTON_PIN)){ //really, really basic state machine
    delay(100);
    state = STARTING;
    Serial.println("state changed");
    delay(100);
    state = RUNNING;
    Serial.println("going to sleep");
    unsigned long delaytime = random(120000, 180000); //60-120 seconds, approximately
    delay(delaytime);
    state = BEEPING;
    Serial.println("beeping");
    delay(2000);
    state = STOPPED;
  }
}
