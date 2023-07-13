unsigned int version_ = 1;

   // pin on which to receive the trigger (2 and 3 can be used with interrupts, although this code does not use interrupts)
   const int inPin = 2;


   const int outPin = 8;
   
   const unsigned long timeOut = 1000;

   unsigned int counter = 0;
   unsigned int limit;
   boolean counting = false;
   boolean inputWas;

void setup() {
  // put your setup code here, to run once:
   Serial.begin(57600);

     
   pinMode(inPin, INPUT);
   pinMode(outPin, OUTPUT);

   inputWas = digitalRead(inPin);
   
}

void loop() {
  // put your main code here, to run repeatedly:

   if (Serial.available() > 0) {
     int inByte = Serial.read();
     switch (inByte) {

        // go (i.e. start) 'g'
       case 103: 
        if (waitForSerial(timeOut)) {          
          limit = Serial.parseInt();
          Serial.write("Starting with "); 
          Serial.println(limit, DEC);
          // byte byte1 = Serial.read();
          //if (waitForSerial(timeOut)) {
          //  byte byte2 = Serial.read();
            // set limit here
          counting = true;
          counter = 0;
          inputWas = digitalRead(inPin);
          break;
        }

        // stop 's'
        case 115:
          counting = false;
          Serial.println("Stopping"); 
          break;

        case 105:
          Serial.println("ArduinoCounter version 1.0");
      }

      
   }



   if (counting) {
      if (inputWas && !digitalRead(inPin)) {
         counter++;
         inputWas = LOW;
         if (counter <= limit) {
            // digitalWrite(outPin, LOW);
            PORTB = 0;
         }
      } else if (!inputWas && digitalRead(inPin)) {
          inputWas = HIGH;
          // digitalWrite(outPin, HIGH);
          PORTB = 1;
      }
   } else {
       PORTB = (PIND & B00000100) >> 2;
   }
}

bool waitForSerial(unsigned long timeOut)
{
    unsigned long startTime = millis();
    while (Serial.available() == 0 && (millis() - startTime < timeOut) ) {}
    if (Serial.available() > 0)
       return true;
    return false;
} 