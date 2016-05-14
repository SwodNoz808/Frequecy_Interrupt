/*
  Example Timer1 Interrupt
  Flash LED every second
*/

#define ledPin 13
#define LED2 12
long timer1_counter;
unsigned volatile long viCountnew, viCountold, viCount, viCount2;
unsigned long iCountH, iCountL;
unsigned volatile long c;
unsigned long freq;
unsigned long freqAvg = 0;
int N =10;
void setup()
{
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  // initialize timer1
  //noInterrupts();           // disable all interrupts
  TCCR4A = 0;
  //TCCR1B = 0;
  TCCR4B = 1 << ICNC4 | 1 << ICES4 | 1 << CS42 | 0 << CS41 | 0 << CS40;
  //TCCR4B|= (1<<ICNC4) | (1<<ICES4) | (1<<CS42)| (0<<CS41) | (1<<CS40);
  // Set timer1_counter to the correct value for our interrupt interval
  //timer1_counter = 64911;   // preload timer 65536-16MHz/256/100Hz
  //timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
  timer1_counter = 34286;   // preload timer 65536-16MHz/256/2Hz

  //TCNT4 = timer1_counter;   // preload timer
  //TCCR4B |= (1 << CS42);    // 256 prescaler
  TIMSK4 |= (1 << ICIE4);   // enable timer overflow interrupt
  //interrupts();    // enable all interrupts

  //TCNT4=0;
  delay(200);
}

//ISR(TIMER1_OVF_vect)        // interrupt service routine
ISR(TIMER4_CAPT_vect)
{
  //  c=0;
  TCNT4 = 0;   // preload timer
  //viCount =c++;
  viCount2 = ICR4L;
  viCount = ICR4H;

  //TCNT4=0;
  digitalWrite(ledPin, digitalRead(ledPin) ^ 1);
}

void loop() {
  //  if (TI
  cli();
  iCountH = viCount;
  iCountL = viCount2;
  c = (iCountH << 8) | iCountL;
  //digitalWrite(13, state);
  freq = 62500 / c;
  for (int i = 0; i < N; i++) {
    freqAvg = freqAvg + freq;
  }
  freqAvg = freqAvg / (N+1);
  
  sei();

  Serial.print("OLD: ");
  Serial.print(iCountH, HEX);
  Serial.println(iCountL, HEX );
  Serial.print("hex:");
  Serial.println(c, HEX);
  Serial.print("Freq:");
  Serial.print(freq, DEC);
  Serial.println("Hz");
  Serial.print("Avg Freq: ");
  Serial.println(freqAvg, DEC);

}


