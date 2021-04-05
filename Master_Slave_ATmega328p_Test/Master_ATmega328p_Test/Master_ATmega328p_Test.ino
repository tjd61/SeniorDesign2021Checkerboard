void setup() {
  // put your setup code here, to run once:
  spi_init_master();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void spi_init_master (void){
  // Set MOSI, SCK as Output
  DDRB=(1<<5)|(1<<3);
  
  // Enable SPI, Set as Master
  // Prescaler: Fosc/16, Enable Interrupts
  //The MOSI, SCK pins are as per ATMega8
  
  SPCR=(1<<SPE)|(1<<MSTR);
  
  // Enable Global Interrupts  
  sei();
}

unsigned char spi_tranceiver (unsigned char data){
  // Load data into the buffer
  SPDR = data;
  
  //Wait until transmission complete
  while(!(SPSR & (1<<SPIF)));   // Return received data
  
  return(SPDR);
}
