
/**********************************************************************
//   NOM DU PROGRAMME
//   Programme Prototype des controle des moteurs et des capteurs de présence
//   Mon version 46 dspic33fj128gp802 + PWM + ADC + c30 + PLLPRE +PLLDIV + PLLPOST :
//   AUTEUR:  
//   Mauricio Gomez + Philippe Connor + Antoine Langevin
//
//	 Modifier par : Antoine Langevin
//
//   DATE DE CRÉATION: 22 Marz 2011   
//
//
**********************************************************************/
#include "p33FJ128MC802.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <outcompare.h> 
#include <timer.h> 
#include <uart.h> 
#include <string.h>
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 //Configuration bits pour le dspic33fj128gp802   Mauricio Gomez + Philippe Connor                                                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 _FBS( RBS_NO_RAM  & BSS_NO_BOOT_CODE & BWRP_WRPROTECT_OFF );
 // No Boot Ram & No Boot program Flash segment &  Write Protect :  Disabled
 _FSS( RSS_NO_RAM  & SSS_NO_FLASH & SWRP_WRPROTECT_OFF );
 // No Secure Ram & No Secure Segment & Write Protect :  Disabled
 _FGS( GSS_OFF & GCP_OFF  & GWRP_OFF);
 // code protect off & Code Protect: disabled & Write Protect:Disabled
 _FOSCSEL( FNOSC_FRCPLL & IESO_OFF );
 // Internal Fast RC (FRC) & Start-up device with user-selected oscillator source 
 _FOSC( POSCMD_NONE & OSCIOFNC_ON & IOL1WAY_OFF & FCKSM_CSDCMD );
 // Primary Oscillator Disabled (car interne) & OSC2 pin has digital I/O function 
 //&  Allow Multiple Re-configurations &  Both Clock Switching and Fail-Safe Clock Monitor are disabled
 _FWDT( FWDTEN_OFF );
 // Watchdog timer enabled/disabled by user software
 _FPOR( FPWRT_PWR1 &  ALTI2C_OFF & LPOL_OFF & HPOL_OFF & PWMPIN_ON); 
 // POR Timer Value: Disabled & I2C mapped to SDA1/SCL1 pins & PWM module low side output pins have 
 //active-low output polarity & PWM module high side output pins have active-low output polarity & 
 //PWM module pins ----->NOT<----- controlled by PWM module at device Reset
 _FICD( JTAGEN_OFF  );
 // JTAG is Disabled
 
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 //"DEFINES IMPORTANTS" pour le fonctionement  Mauricio Gomez + Philippe Connor                                                                   //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 #define dsPIC33FJ128MC802 // LE PIC QU'ON UTILISE
 #define FIN 7372800LL  // clock-frequecy in Hz with suffix LL (64-bit-long), eg. 32000000LL for 32MHz
 #define N1 2 //
 #define N2 4 // 2,4 ou 8
 #define M 50 //

					//((FCY/BAUDRATE)/16)-1=155      9200
					//((FCY/BAUDRATE)/16)-1=149      9600
					//((FCY/BAUDRATE)/16)-1=74       19200
					//((FCY/BAUDRATE)/16)-1=36       38400					
					//((FCY/BAUDRATE)/16)-1=24       57600
					//((FCY/BAUDRATE)/16)-1=12		 115200
 #define U1BRGVAL 149; // pour le baud rate du uart1
 #define U2BRGVAL2 12; // pour le baud rate du uart2
 
 
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 //verification pour les "DEFINES IMPORTANTS"    Mauricio Gomez + Philippe Connor                                                                  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//calcul fosc et fcy
 #define FOSC  (FIN * (M/(N1*N2))) // FOSC = FIN* PLLDIV/(PLLPRE*PLLPOST)
 #define FCY (FOSC/2) //FCY = FOSC/2... MCU is running at FCY MIPS

//convertion pour respecter la datasheet
 #define ValueN1 (N1-2) //CLKDIVbits.PLLPRE = ValueN1; // PLL Phase Detector Input Divider bits (N1)
 #define ValueM (M-2) //PLLFBDbits.PLLDIV = ValueM;  // PLL divider (M=50)
 #if N2 == 8
 #define valueN2 0b11 //CLKDIVbits.PLLPOST = ValueN2; // PLL VCO Output Divider Select bits (N2)
 #elif N2 == 4
 #define ValueN2 0b01 //CLKDIVbits.PLLPOST = ValueN2; // PLL VCO Output Divider Select bits (N2)
 #elif N2 == 2
 #define valueN2 0b00 //CLKDIVbits.PLLPOST = ValueN2; // PLL VCO Output Divider Select bits (N2)
 #endif

//error if something important missing - Philippe Connor    
 #ifndef dsPIC33FJ128MC802
 #error "mauvais pic ??? Si tu utilise ce codes pour un autre pic, sa risque de ne pas marcher"
 #endif 
 //
 #ifndef N1
 #error "definir n1 svp...."
 #endif
 #ifndef N2
 #error "definir n2 svp...."
 #endif
 #ifndef M
 #error "definir m svp...."
 #endif

 #define PERIODE 1850

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 // PROTOTYPES FONTIONS et VARIABLE GLOBALES  Mauricio Gomez + Philippe Connor                                      //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int temp_count;
void PC_Write_Str(unsigned char *str, int len);
void PC_Write_Int(unsigned char *str, unsigned int ul, unsigned int length);
void Init_OSC(void);
void Init_UART1(void);
void Init_UART2(void);
void Delay( unsigned int delay_count );
void Init_ADC( int amask );
int readADC( int ch);
int itoa_m(int val);
int atoi_m(int val);
int ch_sel;
int read_adc1=2000;
int read_adc2=2000;
int last_read_adc1=2000;
int last_read_adc2=2000;
int changement = 0;
int compteur_changement = 0;
int changement2 = 0;
int compteur_changement2 = 0;
int relai = 0;
int read_adc3=2000;
int read_adc4=2000;
int Canal=1;
int equilibre=2000;
int max=4000;
int sans=100;
int state=0;
int avant_aux;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// définition des interruptions                                                                                                  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _ISR __attribute__((__no_auto_psv__)) _T2Interrupt(void);
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void);
void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void);


int main (void)
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Init Remappable port 5 pour allumer un petit del                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	TRISBbits.TRISB5=0; // Led
	PORTBbits.RB5=0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Init PLLPRE,PLLDIV,PLLPOST,                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Init_OSC();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Init UART1, UART2                                                                                              //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
Init_UART1();
Init_UART2();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Serial  UART 1                                                                                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   
   //Serial  UART 1
   _RP9R=0b00011;//U1TX 00011 RPn tied to UART1 Transmit pin 18 RP9
   RPINR18bits.U1RXR=0x8;//UART1 Receive U1RX RPINR18 U1RXR<4:0> pin 17 RP8
 
   //Serial  UART 1
//   bit 4-0 U1RXR<4:0>: Assign UART1 Receive (U1RX) to the corresponding RPn pin
//   11111 = Input tied to VSS
//   11001 = Input tied to RP25
//   •
//   •
//   •
//   00001 = Input tied to RP1
//   00000 = Input tied to RP0
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Serial  UART 2                                                                                                  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   _RP6R=0b00101;//U2TX 00101 RPn tied to UART2 Transmit pin 15 RP16
   RPINR19bits.U2RXR=0x7;//UART2 Receive U2RX RPINR19 U2RXR<4:0> pin 16 RP7   

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation du timer 2                                                                                                  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
   T2CONbits.TON= 1;// active timer1
   T2CONbits.TCKPS = 3;// bit 5-4 TCKPS<1:0>: Timer1 Input Clock Prescale Select bits
                  //11 = 1:256
   //T2CONbits.TON= 1;// active timer1
   T2CONbits.TCS= 0; // Slectionne la source 
   PR2 =PERIODE;// période 
   // Clear counter
   T2CONbits.TON= 1;// active timer2
   TMR2 = 0;
   IPC1bits.T2IP = 2;// Interrupt priority 2 (med-low)
   IEC0bits.T2IE = 1;// active interrupt
   
     while(1)
     {

     }
 
}//main

void Init_OSC(void)
{ // Init des parametre des clocks (FIN * (M/(N1*N2)))  
 CLKDIVbits.PLLPRE = ValueN1;
 PLLFBDbits.PLLDIV = ValueM;
 CLKDIVbits.PLLPOST = ValueN2;
 //CLKDIVbits.DOZE=0b011;  ////011 = FCY/8 (default)   
 OSCTUN = 0; // Tune FRC oscillator, if FRC is used
 //RCONbits.SWDTEN = 0; // Disable Watch Dog Timer
 while(OSCCONbits.LOCK != 1) {}; // Wait for PLL to lock
}

void Init_UART1(void)
{
U1MODEbits.STSEL = 0; // 1 Stop bit
U1MODEbits.PDSEL = 0; // No Parity, 8 data bits
U1MODEbits.ABAUD = 0; // Auto-Baud Disabled
U1MODEbits.BRGH = 0; // Low Speed mode
U1BRG = U1BRGVAL;//BRGVAL; // BAUD Rate 
U1MODEbits.UARTEN = 1; // Enable UART
U1STAbits.UTXEN = 1; // Enable UART TX

U1BRG  = U1BRGVAL;         //Baud Rate Generator Prescaler
U1MODE = 0x8000;         // Reset UART to 8-n-1, alt pins, and enable 
U1MODEbits.UARTEN = 1; // Enable UART
U1STAbits.UTXEN = 1; // Enable UART TX   U1STA  = 0x0440;         // Reset status register and enable TX & RX
   
IPC2bits.U1RXIP = 4;    //set recieve interrupt priority to 4
IEC0bits.U1RXIE = 1;    //recieve interrupt enable
_U1RXIF=0;              // Clear UART RX Interrupt Flag
U1STAbits.URXISEL = 0;  //interrupt when any character is recieved}
}

void Init_UART2(void)
{

U2MODEbits.STSEL = 0; // 1 Stop bit
U2MODEbits.PDSEL = 0; // No Parity, 8 data bits
U2MODEbits.ABAUD = 0; // Auto-Baud Disabled
U2MODEbits.BRGH = 0; // Low Speed mode
U2BRG = U2BRGVAL2;//BRGVAL; // BAUD Rate 
U2MODEbits.UARTEN = 1; // Enable UART
U2STAbits.UTXEN = 1; // Enable UART TX   U2STA  = 0x0440;         // Reset status register and enable TX & RX
   
IPC7bits.U2RXIP = 7;    //set recieve interrupt priority to 7
IEC1bits.U2RXIE = 1;    //recieve interrupt enable
_U2RXIF=0;              // Clear UART RX Interrupt Flag
U2STAbits.URXISEL = 0;  //interrupt when any character is recieved}

}


void Delay( unsigned int delay_count ) 
{
   temp_count = delay_count +1;
   asm volatile("outer: dec _temp_count");   
   asm volatile("cp0 _temp_count");
   asm volatile("bra z, done");
   asm volatile("do #3200, inner" );   
   asm volatile("nop");
   asm volatile("inner: nop");
   asm volatile("bra outer");
   asm volatile("done:");
}



void Init_ADC( int amask )
{
   AD1PCFGL = amask;//select channel
   //bit 10 AD12B: 10-Bit or 12-Bit Operation Mode bit
   //1 = 12-bit, 1-channel ADC operation
   //0 = 10-bit, 4-channel ADC operation
   //bit 7-5 SSRC<2:0>: Sample Clock Source Select bits
   //111 = Internal counter ends sampling and starts conversion (auto-convert)
   //110 = Reserved
   //101 = Reserved
   //100 = GP timer (Timer5 for ADC1) compare ends sampling and starts conversion
   //011 = Reserved
   //010 = GP timer (Timer3 for ADC1) compare ends sampling and starts conversion
   //001 = Active transition on INT0 pin ends sampling and starts conversion
   //000 = Clearing sample bit ends sampling and starts conversion
   AD1CON1 = 0b0000010011100000; // automatic conversion start after sampling
   //AD1CON1 = 0b0000010000000000; // automatic conversion start after sampling
   AD1CSSL = 0b11111111; // scanning required
   //bit 10 CSCNA: Scan Input Selections for CH0+ during Sample A bit
   //1 = Scan inputs
   //0 = Do not scan inputs
   //bit 9-8 CHPS<1:0>: Selects Channels Utilized bits
   //When AD12B = 1, CHPS<1:0> is: U-0, Unimplemented, Read as ‘0’
   //1x = Converts CH0, CH1, CH2 and CH3
   //01 = Converts CH0 and CH1
   //00 = Converts CH0
   AD1CON2 = 0b0000000000000000; 
   AD1CON3 = 0b0001111100000010; // Tsamp = 32 x Tad; Tad=125ns
   AD1CON1bits.ADON = 1; // turn on the ADC
   }

int readADC( int ch)
{
   Delay(1);
   AD1CHS0 = ch; // 1. select analog input channel
   AD1CON1bits.SAMP = 1; // 2. start sampling
   Delay(1);
   while (!AD1CON1bits.DONE); // 5. wait for the conversion to complete
   return ADC1BUF0; // 6. read the conversion result
} // readADC

void _ISR __attribute__((__no_auto_psv__)) _T2Interrupt(void)
 {
TMR2 = 0;
//int i;
char dato1;
char dato2;
char dato3;
char dato4;
      
    // Compteur pour envoyer les données au moins tous les ms s'il n'y a pas de changement + Antoine Langevin
    if(compteur_changement++ > 25)
    {
	    changement = 1;
	    compteur_changement = 0;
	    _RB5 = 1;
	}   
	if(compteur_changement2++ > 2)
    {
	    changement2 = 1;
	    compteur_changement2 = 0;
	    _RB5 = 1;
	}   
	
	
   
     //T2IF: Timer2 Interrupt Flag Status bit   //1 = Interrupt request has occurred
                                      //0 = Interrupt request has not occurred
    IFS0bits.T2IF = 0;
//    ch_sel=0b00000001;
//    Init_ADC(ch_sel);      
//    read_adc1=readADC(ch_sel);
//    read_adc1=4096-read_adc1;
//    ch_sel=0b00000100;
//    Init_ADC(ch_sel);
//    read_adc2=readADC(ch_sel);
//    read_adc2=4096-read_adc2;
    
    // Vérification de changement pour envoyer les nouvelles valeurs + Antoine Langevin
    if(last_read_adc1 <= 30 && last_read_adc1 + 30 < read_adc1) changement = 1;
    else if(last_read_adc1 >= 993 && last_read_adc1 - 30 > read_adc1) changement = 1;
    else if(last_read_adc1 + 30 < read_adc1 || last_read_adc1 - 30 > read_adc1) changement = 1;
    
    if(last_read_adc2 <= 30 && last_read_adc2 + 30 < read_adc2) changement = 1;
    else if(last_read_adc2 >= 993 && last_read_adc2 - 30 > read_adc2) changement = 1;
    else if(last_read_adc2 + 30 < read_adc2 || last_read_adc2 - 30 > read_adc2) changement = 1;
	
	// S'il y a un changement + Antoine Langevin
	if(changement & changement2)
	{	
		// Si la variable relai est à 1 c'est parce qu'il y a un relai + Antoine Langevin
		if(relai)
		{
			dato1 = read_adc1 & 0x00ff;
		    dato2 = (read_adc1  >> 8) & 0x00ff;
			dato3 = read_adc2 & 0x00ff;
		    dato4 = (read_adc2  >> 8) & 0x00ff;
		    
		    if (dato1==(char)0xf1)dato1=0xf0;
		    if (dato1==(char)0xf2)dato1=0xf0;
		    if (dato1==(char)0xf3)dato1=0xf0;
		    if (dato2==(char)0xf1)dato2=0xf0;
		    if (dato2==(char)0xf2)dato2=0xf0;
		    if (dato2==(char)0xf3)dato2=0xf0;
		    if (dato3==(char)0xf1)dato3=0xf0;
		    if (dato3==(char)0xf2)dato3=0xf0;
		    if (dato3==(char)0xf3)dato3=0xf0;
		    if (dato4==(char)0xf1)dato4=0xf0;
		    if (dato4==(char)0xf2)dato4=0xf0;
		    if (dato4==(char)0xf3)dato4=0xf0;
		    
		    WriteUART1(0xF2);
		    while(U1STAbits.UTXBF==1);
		    WriteUART1(dato1);
		    while(U1STAbits.UTXBF==1);
		    WriteUART1(dato2);
		    while(U1STAbits.UTXBF==1);
		    WriteUART1(dato3);
		    while(U1STAbits.UTXBF==1);
		    WriteUART1(dato4);
		    while(U1STAbits.UTXBF==1);
		    WriteUART1(0xff);
		    while(U1STAbits.UTXBF==1);
		    
//			WriteUART2(0xF2);
//		    while(U2STAbits.UTXBF==1);
//		    WriteUART2(dato1);
//		    while(U2STAbits.UTXBF==1);
//		    WriteUART2(dato2);    
//		    
		    last_read_adc1 = read_adc1;
		    last_read_adc2 = read_adc2; 
		}
		else // Sinon nous avons le même code qu'à la version précédente
		{


		    char dato3 = read_adc2 & 0x000000ff;
		    char dato4 = (read_adc2  >> 8) & 0x000000ff;
		    char dato1 = read_adc1 & 0x00ff;
		    char dato2 = (read_adc1  >> 8) & 0x00ff;
		
		    if (dato1==(char)0xfd)dato1=0xfc;
		    if (dato1==(char)0xfe)dato1=0xfc;
		    if (dato2==(char)0xfd)dato2=0xfc;
		    if (dato2==(char)0xfe)dato2=0xfc;
		    if (dato3==(char)0xfd)dato3=0xfc;
		    if (dato3==(char)0xfe)dato3=0xfc;
		    if (dato4==(char)0xfd)dato4=0xfc;
		    if (dato4==(char)0xfe)dato4=0xfc;
		
		    WriteUART1(0xfd);
		    while(U1STAbits.UTXBF==1);
		    WriteUART1(dato1);
		    while(U1STAbits.UTXBF==1);
		    WriteUART1(dato2);
		    while(U1STAbits.UTXBF==1);
		    WriteUART1(0xfe);
		    while(U1STAbits.UTXBF==1);
		    WriteUART1(dato3);
		    while(U1STAbits.UTXBF==1);
		    WriteUART1(dato4);
		    while(U1STAbits.UTXBF==1);
		    
//		    WriteUART2(0xF2);
//		    while(U2STAbits.UTXBF==1);
//		    WriteUART2(dato1);
//		    while(U2STAbits.UTXBF==1);
//		    WriteUART2(dato2);    
		    
		    last_read_adc1 = read_adc1;
		    last_read_adc2 = read_adc2;
		} 
		unsigned char displayTitle1[6] = "adc1:\0"; 
		unsigned char  val[5] = "    "; // conteneur
		unsigned char displayTitle2[6] = "adc2:\0";
//		strcpy(val,"    "); 
//		PC_Write_Str(displayTitle1, strlen(displayTitle1));
	//		PC_Write_Int(val,read_adc1,4);
	//		PC_Write_Str("\n\r\0", strlen("\n\r\0"));
//		strcpy(val,"    ");
//		PC_Write_Str(displayTitle2, strlen(displayTitle2));
//		PC_Write_Int(val,read_adc2,4);
//		PC_Write_Str("\n\r\0", strlen("\n\r\0"));


		changement = 0;
		changement2 = 0;
	}
}

void PC_Write_Str(unsigned char *str, int len)
{
 int i = 0;
  while (i < len)
 {
 WriteUART2(str[i++]);
 while(U2STAbits.UTXBF==1);
 }
}


void PC_Write_Int(unsigned char *str, unsigned int ul, unsigned int length) // by Phil
{
 int inclol = length; // -- mod 1
   do 
 { 
 str[--length] = (ul % 10) + '0'; 
    ul = ul / 10;
 inclol--;
   } while (ul);  //leading blanks 
  
  while (inclol--)
  {
   str[--length] = ' ';
  } 
  strcat(str,"\0");
  PC_Write_Str(str, strlen(str));
  
}


void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
{	
	_U1RXIF=0;
	
	int code;
	
	code = ReadUART1();
		char displayTitle1[6] = "reciA"; 
		PC_Write_Str(displayTitle1, strlen(displayTitle1));
		PC_Write_Str("\n\r\0", strlen("\n\r\0"));
	if(code == 0xF1)
	{
		if(ReadUART1() == 0xF2)
		{
			_RB5 = 0;
		}	
	}
}	
	static int M1Switch=0;
	static int M2Switch=0;
	int M1=0;
	int M2=0;

void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void)
{	
	char displayTitle2[2];
	char displayTitle1[2];
	static int inc=0;
	int aux=0;
	_U2RXIF=0;
	
	int code;
	char  val[5] = "    "; // conteneur
	
	code = ReadUART2();
	unsigned int prochendata=0;
		switch (code)
		{
		case 97:
				strcpy(displayTitle2,"a\0");
				PC_Write_Str(displayTitle2, strlen(displayTitle2));
				PC_Write_Str("\n\r\0", strlen("\n\r\0"));
				read_adc1=50;
				read_adc2=4000;
				prochendata=0;
				break;
		case 100:
				strcpy(displayTitle2,"d\0");
				PC_Write_Str(displayTitle2, strlen(displayTitle2));
				PC_Write_Str("\n\r\0", strlen("\n\r\0"));
				read_adc1=4000;
				read_adc2=50;
				prochendata=0;
				break;
		case 119:
				strcpy(displayTitle2,"w\0");
				PC_Write_Str(displayTitle2, strlen(displayTitle2));
				PC_Write_Str("\n\r\0", strlen("\n\r\0"));
				read_adc1=50;
				read_adc2=50;
				prochendata=0;
				break;
		case 120:
				strcpy(displayTitle2,"x\0");
				PC_Write_Str(displayTitle2, strlen(displayTitle2));
				PC_Write_Str("\n\r\0", strlen("\n\r\0"));
				read_adc1=4000;
				read_adc2=4000;
				prochendata=0;
				break;
		case 115://s
				strcpy(displayTitle2,"s\0");
				PC_Write_Str(displayTitle2, strlen(displayTitle2));
				PC_Write_Str("\n\r\0", strlen("\n\r\0"));
				read_adc1=2000;
				read_adc2=2000;
				prochendata=0;
				break;
		case 108://l
				//strcpy(displayTitle2,"l\0");
				//PC_Write_Str(displayTitle2, strlen(displayTitle2));
				//PC_Write_Str("\n\r\0", strlen("\n\r\0"));
				//read_adc1=2000;
				//read_adc2=2000;
				M1Switch=1;
				inc=0;
				break;
		case 114://r
				//strcpy(displayTitle2,"r\0");
				//PC_Write_Str(displayTitle2, strlen(displayTitle2));
				//PC_Write_Str("\n\r\0", strlen("\n\r\0"));
				//read_adc1=2000;
				//read_adc2=2000;
				M2Switch=2;
				inc=0;
				break;
		case 102://f
				//strcpy(displayTitle2,"f\0");
				//PC_Write_Str(displayTitle2, strlen(displayTitle2));
				//PC_Write_Str("\n\r\0", strlen("\n\r\0"));
				M1Switch=0;
				//M2Switch=0;
				inc=0;
				read_adc1=M1;
				//read_adc2=M2;
				M1=0;
				//M2=0;
				break;
		case 103://g
				//strcpy(displayTitle2,"g\0");
				//PC_Write_Str(displayTitle2, strlen(displayTitle2));
				//PC_Write_Str("\n\r\0", strlen("\n\r\0"));
				//M1Switch=0;
				M2Switch=0;
				inc=0;
				//read_adc1=M1;
				read_adc2=M2;
				//M1=0;
				M2=0;
				break;
		
		default:
				if(inc>3)
				{ 	
					break;
				}
					if(M1Switch==1){
					aux=atoi_m(code);
					if (inc==0) {M1=M1+(aux*1000);}
					if (inc==1) {M1=M1+(aux*100);}
					if (inc==2) {M1=M1+(aux*10);}
					if (inc==3) {M1=M1+(aux*1);}
					inc++;
				}
				if(M2Switch==2){
					aux=atoi_m(code);
					if (inc==0) {M2=M2+(aux*1000);}
					if (inc==1) {M2=M2+(aux*100);}
					if (inc==2) {M2=M2+(aux*10);}
					if (inc==3) {M2=M2+(aux*1);}
					inc++;
				}
				if((M2Switch!=2)&(M1Switch!=1))
				{
				inc=0;
				}
				code=0;
		
		break;
		}
}

int itoa_m(int val)
{
	if (val==0)return 48;
	if (val==1)return 49;
	if (val==2)return 50;
	if (val==3)return 51;
	if (val==4)return 52;
	if (val==5)return 53;
	if (val==6)return 54;
	if (val==7)return 55;
	if (val==8)return 56;
	if (val==9)return 57;
	if (val>9)return 43;
	if (val<0)return 45;
	return 0;
}
int atoi_m(int val)
{
	if (val==48)return 0;
	if (val==49)return 1;
	if (val==50)return 2;
	if (val==51)return 3;
	if (val==52)return 4;
	if (val==53)return 5;
	if (val==54)return 6;
	if (val==55)return 7;
	if (val==56)return 8;
	if (val==57)return 9;
	if (val>57)return 10;
	if (val<48)return 11;
	return 0;
}