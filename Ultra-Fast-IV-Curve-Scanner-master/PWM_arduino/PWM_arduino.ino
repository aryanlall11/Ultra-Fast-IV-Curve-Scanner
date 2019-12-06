int numSamples0=0;
int numSamples1=0;
uint16_t a,b,y;
const unsigned int num=256;
unsigned int red0[num];
unsigned int red1[num];

void init(byte x)          // Initialization done before starting the ADC conversion
{
  ADCSRA = 0;              // clear ADCSRA register
  ADCSRB = 0;              // clear ADCSRB register
  ADMUX =x;             
  ADCSRA |= (1 << ADPS2);  // 16 prescaler for 76.9 KHz
  ADCSRA |= (1 << ADEN); 
  ADCSRA |= (1 << ADSC); 
  a=b=y=0;
}
void setup()
{
  Serial.begin(115200);   //Set the baud rate for communication with PC
  DDRC = B11111111;       //PORTC declared as output port
  delay(5000);
}

void loop()
{
  for(uint8_t i=0;i<255;i++)
    {
      PORTC=(255-i);          //Current is proportional to voltage applied on PORTC (5V(255)-0V(0))
      delayMicroseconds(50);  //Adjustable Pulse Width

      /*******************Measurement-I**********************/

      init(0x61);                 //Initialise input port 1 (Refer to manual for arguments)
      while(ADCSRA & (1<<ADSC));
      a = ADCL>>6 ;
      b = ADCH <<2;
      y = b|a;

      PORTC=0;
      if(numSamples0<256)
      {
        red0[numSamples0]=y;      //Store measured value
        numSamples0++;
      }
      delayMicroseconds(2000);    //Delay between two pulses
        
      PORTC=(255-i);
      delayMicroseconds(50);  //Adjustable Pulse Width

      /*******************Measurement-II**********************/

      init(0x62);                   //Initialise input port 2 (Refer to manual for arguments)
      while(ADCSRA & (1<<ADSC));
      a = ADCL>>6 ;
      b = ADCH <<2;
      y = b|a;

      PORTC=0;
      if(numSamples1<256)
      {
        red1[numSamples1]=y;       //Store measured value
        numSamples1++;
      }
      delayMicroseconds(2000);     //Delay between two pulses
     
    }


  if (numSamples0>=254||numSamples1>=254)     //Display the measured values (Digital numbers=> 5V(1023), 0V(0))
  {
    Serial.println("------------------------- START");

    for(int i=0;i<255;i++)
     {
      Serial.print(red0[i]);
      Serial.print("  ");Serial.print(red1[i]);Serial.print("\n");
     }
    Serial.println("------------------------- STOP");
    delay(1000);
    numSamples0=0;
    numSamples1=0; 
  }

}