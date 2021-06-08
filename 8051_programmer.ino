/*********************************************************************************************************/
/*                                       8051 PROGRAMMER                                                 */
/*********************************************************************************************************/


/*********************************************************************************************************/
/*                                                                                                       */
/* Author: SYEED MOHD AMEEN                                                   Date: 31-05-2021           */
/* Email:   ameensyeed2@gmail.com                                                                        */
/*                                                                                                       */
/*        THIS IS THE SIMPLE BIT BAGGING IMPLEMENTATION OF 8051 PROGRAMMER (AT89S51 | AT89S52)           */
/*********************************************************************************************************/

/*********************************************************************************************************/
/*                                      THE MIT LICENSE (MIT)                                            */
/*-------------------------------------------------------------------------------------------------------*/
/* Permission is hereby granted, free of charge, to any person obtaining a copy of this software         */
/* and associated documentation files (the "Software"), to deal in the Software without restriction,     */
/* including without limitation the rights to use, copy, modify, merge, publish, distribute,             */
/* sublicense, and/or sell copies of the Software, and to permit persons to whom the Software            */
/* is furnished to do so, subject to the following conditions:                                           */
/*                                                                                                       */
/* The above copyright notice and this permission notice shall be included in all copies                 */
/* or substantial portions of the Software.                                                              */
/*                                                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,                   */
/* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE      */
/* AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,      */
/* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF */
/* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                      */
/*********************************************************************************************************/









/***************************************************************************************************************************/                                                                                                                                       
/*                                          PROGRAMMING THE FLASH - SERIAL MODE                                            */                                                                                                                                                                                                                                                   
/*                                                                                                                         */                                            
/* Working of serial programming is based on the datasheet specifications of Atmel AT89S51                                 */                                                                                            
/* Datasheet (http://www.keil.com/dd/docs/datashts/atmel/at89s51_ds.pdf)                                                   */                                                                          
/*                                                                                                                         */   
/*                                                                                                                         */    
/* The Code memory array can be programmed using the serial ISP interface while RST is                                     */                                                                                        
/* pulled to VCC. The serial interface consists of pins SCK, MOSI (input) and MISO (output). After                         */                                                                                                    
/* RST is set high, the Programming Enable instruction needs to be executed first before other                             */                                                                                                
/* operations can be executed. Before a reprogramming sequence can occur, a Chip Erase                                     */                                                                                        
/* operation is required.                                                                                                  */                        
/*                                                                                                                         */    
/* The Chip Erase operation turns the content of every memory location in the Code array into FFH.                         */                                                                                                     
/* Either an external system clock can be supplied at pin XTAL1 or a crystal needs to be connected                         */                                                                                              
/* across pins XTAL1 and XTAL2. The maximum serial clock (SCK) frequency should be less than 1/16 of the crystal frequency.*/                                                                                                                          
/* With a 33 MHz oscillator clock, the maximum SCK frequency is 2 MHz.                                                     */                                                                      
/*                                                                                                                         */    
/*                                                                                                                         */    
/*-------------------------------------------------------------------------------------------------------------------------*/ 

/**************************************************************************************************************************************************************/
/*                                                       SERIAL PROGRAMMING ALGORITHM                                                                         */                                
/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/                                                                                                                                    
/*                                                                                                                                                            */
/*                                                                                                                                                            */
/* To program and verify the AT89S51 in the serial programming mode, the following sequence is recommended:                                                   */                                                                                                          
/*                                                                                                                                                            */  
/* 1. Power-up sequence:                                                                                                                                      */                        
/*      Apply power between VCC and GND pins.                                                                                                                 */                                              
/*      Set RST pin to “H”.                                                                                                                                   */                          
/*      If a crystal is not connected across pins XTAL1 and XTAL2, apply a 3 MHz to 33 MHz clock to XTAL1 pin and wait for at least 10 milliseconds.          */                                                                                                                                                    
/*                                                                                                                                                            */  
/* 2. Enable serial programming by sending the Programming Enable serial instruction to                                                                       */                                                                                       
/*       pin MOSI/P1.5. The frequency of the shift clock supplied at pin SCK/P1.7 needs to be less than the CPU clock at XTAL1 divided by 16.                 */                                                                                                                                             
/*                                                                                                                                                            */  
/*                                                                                                                                                            */    
/* 3. The Code array is programmed one byte at a time in either the Byte or Page mode.                                                                        */                                                                                      
/*      The write cycle is self-timed and typically takes less than 0.5 ms at 5V.                                                                             */                                                                                  
/*                                                                                                                                                            */    
/* 4. Any memory location can be verified by using the Read instruction that returns the content at the selected address at serial output MISO/P1.6.          */                                                                                                                                                    
/*                                                                                                                                                            */  
/* 5. At the end of                                                                                                                                           */                  
/**************************************************************************************************************************************************************/




/**************************************************************************************************************************************************************/
/*                                                        SERIAL PROGRAMMING INSTRUCTIONS                                                                     */
/**************************************************************************************************************************************************************/
/*                                                                                                                                                            */
/* Serial Programming Instruction Set used in this  Programmer are mentioned below and can be read at the AT89S51 DATASHEET                                   */
/*                                                                                                                                                            */
/* INSTRUCTION               | BYTE 1    | BYTE 2      | BYTE 3      |   BYTE 4            | Operation                                                        */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Programming Enable        | 0xAC      | 0x53        | 0xXX        | 0xXX (0x69 output)  | Enable Serial Programming while RST is high                      */
/* Chip Erase                | 0xAC      | 0x8X        | 0xXX        | 0xXX                | Chipe Erase Flash memory array                                   */
/* Read Program Memory<BM>   | 0x20      | 0xX(A11-8)  | 0x(A7-0)    | 0x(D7-0)            | Read data from Program memory in the byte mode                   */
/* Write Program Memory<BM>  | 0x40      | 0xX(A11-8)  | 0x(A7-0)    | 0x(D7-0)            | Write data to Program memory in the byte mode                    */
/* Write Lock Bits           | 0xAC      | 0xE(00B1B2) | 0xXX        | 0xXX                | Write Lock bits                                                  */
/* Read Lock Bits            | 0x24      | 0xXX        | 0xXX        | (xxxLB3LB2LB1xx)    | Read back current status of lock bits (programmed read as "1")   */
/* Read Signature Bytes      | 0x28      | (xxxA5-A1)  | 0x(A0xxx)X  | <Signature Byte>    | Read Signature Byte                                              */
/*                                                                                                                                                            */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* < Signature Byte>                                                                                                                                          */
/*   1- Singature bytes are not readable in Lock Bit Modes 3 & 4                                                                                              */
/*                                                                                                                                                            */
/*  <BM> (Byte Mode)                                                                                                                                          */
/**************************************************************************************************************************************************************/

/**************************************************************************************************************************************************************/
/*                                                          CONSTANTS DECLARATION                                                                             */
/**************************************************************************************************************************************************************/


/* spi bus pins definations */
#define MISO 2 
#define MOSI 3 
#define SCK  4 
#define RST  5

/* AT89S51 Datasheet flags defination */
#define DONT_CARE 0x00 

/* Programming enable instructions */
#define PROGRAM_EN0 0xAC
#define PROGRAM_EN1 0x53
#define PROGRAM_EN2 DONT_CARE
#define PROGRAM_EN3 DONT_CARE

/* chip erase instructions */
#define CP_ERASE0 0xAC
#define CP_ERASE1 0x80
#define CP_ERASE2 DONT_CARE
#define CP_ERASE3 DONT_CARE

/* Read program memory instrcution */
#define RD_BYTE0 0x20

/* Write program memory instruction */
#define WR_BYTE0 0x40 

#define WRITE_LOCK_BITS_1 0xAC
#define WRITE_LOCK_BITS_2 0xE0
#define WRITE_LOCK_BITS_3 DONT_CARE
#define WRITE_LOCK_BITS_4 DONT_CARE

#define READ_LOCK_BITS_1 0x24
#define READ_LOCK_BITS_2 DONT_CARE
#define READ_LOCK_BITS_3 DONT_CARE
#define READ_LOCK_BITS_4 DONT_CARE

#define READ_SIG_BITS_1 0x28
#define READ_SIG_BITS_2 DONT_CARE
#define READ_SIG_BITS_3 DONT_CARE
#define READ_SIG_BITS_4 DONT_CARE

/**************************************************************************************************************************************************/
/*  The delay in each SCK flip-flop which should be atleast 1/16th times that of the crystal attached (according to datasheet of AT89S51)         */
/*  In my case I used a 12 MHz crystal and the Arduino has 16MHz crystal.                                                                         */
/**************************************************************************************************************************************************/
#define DELAY_INSTRUCTION_US 2

/**************************************************************************************************************************************************/
/*  After the Chip Erase Instruction has been issued we should atleast wait 500ms                                                                 */
/*  The actual value used was 512 just to be on the safe side                                                                                     */
/**************************************************************************************************************************************************/
#define DELAY_ERASE_MS 512 

/**************************************************************************************************************************************************/
/*  When starting the programming we must first pull up the RST pin to "1" and, wait 64 cycles (on a 12MHz crystal it is equivalent to 6ms)       */
/*  The actual value used was 8 just to be on the safe side                                                                                       */
/**************************************************************************************************************************************************/
#define DELAY_RESET_MS 10


/* test data to burn into flash memory */
byte data[] = { 0x81, 0xcf, 0x92, 0x86, 0xcc, 0xa4, 0xa0, 0x8f, 0x80, 0x84, 0x88, 0xe0, 0xb1, 0xc2, 0xb0, 0xb8 };


/* Setup all the I/O Pins */
void setup()
{
  pinMode(SCK,OUTPUT);                /* clock pin */      
  pinMode(RST,OUTPUT);                /* reset pin */
  pinMode(MOSI,OUTPUT);               /* master out slave in (serial out)*/
  pinMode(MISO,INPUT);                /* master in slave out (serial in)*/
  Serial.begin(57600);
  Serial.println("                8051 PROGRAMMER             ");
  


  digitalWrite(RST,HIGH);
  delay(DELAY_RESET_MS);
  digitalWrite(SCK,LOW);
  delay(DELAY_RESET_MS);

repeat:
  if (program_enable() == 0x69)
  {
    Serial.println("  Sucessufully enable the programming mode....... ");
  }
  else 
  {
    Serial.println("  Program is not enable:  go to repeat the connection ");
    goto repeat;
  }
  
  /* to reprogram memory chip erase instrcution is required */
  chip_erase();
  Serial.println("  ERASE THE ENTIRE FLASH MEMORY ");

  

  // program test data array 
  for (int i=0; i<sizeof(data); i++)
  {
    // address starting with 0 
    write_byte(i,data[i]);
  }
  Serial.println("   \n \n  PROGRAM FLASH ");

  // read the Flash memory content 
  print_contents(4096);
}


void loop()
{
  
}

/********************************************************************************************/
/*                  Function for Data Thourgh SPI Communication BUS                         */
/********************************************************************************************/
byte send_spi(byte instruction)
{
  byte bit_order;                 
  boolean flag;                     
  byte return_value = 0;                   /* return value flag variable */
  
  for (byte i=0; i<8; i++)
  {
    flag = instruction & (0x80 >> i);       /* Masking instruction data (MSB First)*/
    digitalWrite(MOSI,flag);                /* write flag bit because it's boolean */
    
    digitalWrite(SCK,true);
    delayMicroseconds(DELAY_INSTRUCTION_US);
    bit_order = digitalRead(MISO);          /* read input pin (MISO)*/
    digitalWrite(SCK,false);

    /* masking input bit stream */
    return_value += (bit_order << i) & 0xff;
  }
  return return_value;
}

/********************************************************************************************/
/*                  Function for Data Transfer through SPI Communication                    */
/********************************************************************************************/
byte program_enable(void)
{
  byte temp;

  temp = send_spi(PROGRAM_EN0);
  temp = send_spi(PROGRAM_EN1);
  temp = send_spi(PROGRAM_EN2);
  return (send_spi(PROGRAM_EN3));
}

/********************************************************************************************/
/*                              CHIP Erase Function                                         */
/********************************************************************************************/
byte chip_erase()
{
  byte temp;
  
  temp = send_spi(CP_ERASE0);
  temp = send_spi(CP_ERASE1);
  temp = send_spi(CP_ERASE2);
  temp = send_spi(CP_ERASE3);
  delay(DELAY_ERASE_MS);
}

/********************************************************************************************/
/*                      Function that read the byte from address                            */
/********************************************************************************************/
byte read_byte(int address)
{
  /* 16-bit address divided into two bytes */
  byte addrH = address & 0xff00;
  byte addrL = address & 0x00ff;                            
  byte temp;        
  
  temp = send_spi(RD_BYTE0);
  temp = send_spi(addrH);
  temp = send_spi(addrL);
  
  return (send_spi(DONT_CARE)); 
}

/********************************************************************************************/
/*                            Write byte data from address                                  */  
/********************************************************************************************/
void write_byte(int address, byte data)
{
  byte addrH = address & 0xff00;
  byte addrL = address & 0x00ff;
  byte temp;
  
  temp = send_spi(WR_BYTE0);
  temp = send_spi(addrH);
  temp = send_spi(addrL);
  temp = send_spi(data);
}

/********************************************************************************************/
/*                                 Write Lock bits                                          */
/********************************************************************************************/
void write_lockbits(byte bits)
{
  byte temp;
  
  temp = send_spi(WRITE_LOCK_BITS_1);
  temp = send_spi(WRITE_LOCK_BITS_2);
  temp = send_spi(WRITE_LOCK_BITS_3 | (bits & 0x03));
  temp = send_spi(WRITE_LOCK_BITS_4); 
}

/********************************************************************************************/
/*                                  Read Lock bits                                          */
/********************************************************************************************/
byte read_lockbits(void)
{
  byte temp;
   
  temp = send_spi(READ_LOCK_BITS_1);
  temp = send_spi(READ_LOCK_BITS_2);
  temp = send_spi(READ_LOCK_BITS_3);
  temp = send_spi(READ_LOCK_BITS_4);

  temp = temp >> 2;
  temp = temp & 0x03;

  return (temp);
}

/********************************************************************************************/
/*                              Read Signature bytes                                        */
/********************************************************************************************/
byte read_signature(byte address)
{
  byte temp;

  temp = send_spi(READ_SIG_BITS_1);
  temp = send_spi(READ_SIG_BITS_2 | (address >> 1));
  temp = send_spi(READ_SIG_BITS_3 | (address << 7));
  temp = send_spi(READ_SIG_BITS_4);
  
  return temp;
}

/********************************************************************************************/
/*              Print the content of flash memory on the serial Plotter                     */
/********************************************************************************************/
{
  for (int base=0; base<=limit; base+=16) 
  {
    byte data[16];
    
    /* Print 16 Byte at a line */
    for (int offset = 0; offset <= 15; offset += 1) 
    {
      data[offset] = read_byte(base + offset);
    }

    char buf[80];                                 /* print buffer 80 bytes */
    
    sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);
            
    Serial.println(buf);
  }
}

/********************************************************************************************/
/*                  Function to convert the hexadecimal number into decimal                 */
/********************************************************************************************/
byte hex_dec(byte number)
{
  if (number >= '0' && number <= '9')
  {
    return (number - '0');
  }
  else 
  {
    return ((number - 'A') + 10);
  }
}
