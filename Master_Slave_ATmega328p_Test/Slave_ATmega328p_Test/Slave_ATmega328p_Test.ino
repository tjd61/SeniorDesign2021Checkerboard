void setup() {
  // put your setup code here, to run once:
  spi_init_slave();
}

void loop() {
  // put your main code here, to run repeatedly:

}

// Initialize SPI Slave Device

void spi_init_slave (void){
  DDRB = (1<<6);      //MISO as OUTPUT
  SPCR = (1<<SPE);    //Enable SPI
}

unsigned char spi_tranceiver (unsigned char data){
  // Load data into the buffer
  SPDR = data;
  
  //Wait until transmission complete
  while(!(SPSR & (1<<SPIF)));   // Return received data
  
  return(SPDR);
}
