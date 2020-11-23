
/**********************************************************************
//   NOM DU PROGRAMME
//   Programme Prototype des controle des moteurs et des capteurs de présence
//   Mon version 37 dspic33fj128gp802 + PWM + ADC + c30 + PLLPRE +PLLDIV + PLLPOST :
//   AUTEUR:  
//   Estefan Mauricio Gomez
//
//   DATE DE CRÉATION:    22 Marz 2011   
//
//
**********************************************************************/
#include "p33FJ128mc802.h"
#include "glk19264-7t-1u.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <outcompare.h> 
#include <timer.h> 
#include <uart.h> 
#if defined (__dsPIC33F__)
_FGS(GSS_OFF & GWRP_OFF);//GSS_OFF code protect off/Write Protect:GWRP_OFF Disabled
_FOSCSEL(FNOSC_FRCPLL & IESO_OFF);//Primary oscillator (XT, HS, EC) w/ PLL//**   Two-speed Oscillator Startup :IESO_OFF Disabled
_FOSC(FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMD_NONE);  // this sets I/O on OSC2 pin
_FWDT(FWDTEN_OFF);
#define __dsPIC33F__ 1
#endif

void OSC_init(void);
void Delay( unsigned int delay_count );
void Init_ADC( int amask );
int readADC( int ch);
int ch_sel;
int read_adc1=2000;
int read_adc2=2000;
int read_adc3=2000;
int read_adc4=2000;
int Canal=1;
int equilibre=2000;
int max=4000;
int sans=100;
unsigned int ReadUART1_mod(void);

		unsigned int code;
		unsigned int data1;
		unsigned int data2;
		unsigned int data3;
		unsigned int data4;
		unsigned int dataready1=0;
		unsigned int dataready2=0;


//int writeSPI1( int data);
int calcular_alfa(double x, double y);

void _ISR __attribute__((__no_auto_psv__)) _T2Interrupt(void);

unsigned int temp_count;
volatile unsigned char adc_lcd_update;

// Modulo para Calcular el reloj dependiendo del pll
// Module pour le Calcul de l'horloge en fonction de la PLL

//If PLLPRE<4:0> = 0, then N1 = 2. This yields a
//VCO input of 10/2 = 5 MHz, which is within the
//acceptable range of 0.8-8 MHz.
//• If PLLDIV<8:0> = 0x1E, then M = 32. This yields a
//VCO output of 5 x 32 = 160 MHz, which is within
//the 100-200 MHz ranged needed.
//• If PLLPOST<1:0> = 0, then N2 = 2. This provides
//a Fosc of 160/2 = 80 MHz. The resultant device
//operating speed is 80/2 = 40 MIPS.

#define N1 2
#define ValueN1 0b00000  	//_PLLPRE = ValueN1; // PLL Phase Detector Input Divider bits (N1)
						//CLKDIVbits.PLLPPRE = ValueN1; // N2 = 2
#define M 50 
#define ValueM 0b000110000 //_PLLDIV = ValueM;             // PLL divider (M=50)
#define N2 4
#define ValueN2 0b01 //_PLLPOST = ValueN2; // PLL VCO Output Divider Select bits (N2)
						//CLKDIVbits.PLLPOST = ValueN2; // N2 = 2
				 
#define FIN 7372800LL  // clock-frequecy in Hz with suffix LL (64-bit-long), eg. 32000000LL for 32MHz
// Configure Oscillator to operate the device at 40 MHz
// Fosc = Fin * M/(N1 * N2), Fcy = Fosc/2
// Fosc = 7372800 * 50/(2 * 4) = 46 062 500Hz for FRC_PLL input clock
#define FOSC  (FIN * (M/(N1*N2)))
#define FCY (FOSC/2)  // MCU is running at FCY MIPS
#define BAUDRATE 9600
#define BRGVAL 149;
					//((FCY/BAUDRATE)/16)-1=155,46      Aprox 155 9200
					//((FCY/BAUDRATE)/16)-1=148,94      Aprox 149 9600
					//((FCY/BAUDRATE)/16)-1=73.97       Aprox 74 19200
					//((FCY/BAUDRATE)/16)-1=36,48       Aprox 36 38400					
					//((FCY/BAUDRATE)/16)-1=24          Aprox 24 57600
					//((FCY/BAUDRATE)/16)-1=11.49		Aprox 12 115200
#define BRGVAL2 12

#define PERIODE 1850

#ifndef __DELAY_H
 #define delay_us(x) __delay32(((x*FCY)/1000000L)) // delays x us
 #define delay_ms(x) __delay32(((x*FCY)/1000L))  // delays x ms
 #define __DELAY_H 1
 #include <libpic30.h>
 #endif

int main (void)
{
	TRISBbits.TRISB15=0;
	TRISBbits.TRISB14=0;
	TRISBbits.TRISB4=0;
	TRISBbits.TRISB13=0;
	TRISAbits.TRISA1=1;//entrada A/D conversor	
	OSC_init();
	

	
	//Serial  UART 1
	_RP9R=0b00011;//U1TX 00011 RPn tied to UART1 Transmit pin 18 RP9
	RPINR18bits.U1RXR=0x8;//UART1 Receive U1RX RPINR18 U1RXR<4:0> pin 17 RP8
	
	//Serial  UART 1
//	bit 4-0 U1RXR<4:0>: Assign UART1 Receive (U1RX) to the corresponding RPn pin
//	11111 = Input tied to VSS
//	11001 = Input tied to RP25
//	•
//	•
//	•
//	00001 = Input tied to RP1
//	00000 = Input tied to RP0
	//uart pour GLK19264
	_RP6R=0b00101;//U2TX 00101 RPn tied to UART2 Transmit pin 15 RP16
	RPINR19bits.U2RXR=0x7;//UART2 Receive U2RX RPINR19 U2RXR<4:0> pin 16 RP7	
	


	PORTBbits.RB15=0;
	PORTBbits.RB4=0;
	
	/* Initialisation du timer 2   */
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
	/*******************************************/
	
	PORTBbits.RB13=0;
	  while(1)
	  {
			mogd_write_int(1,1,read_adc1);
			mogd_write_int(8,1,read_adc2);
			if (U1STAbits.PERR) PORTBbits.RB13=1;
			if (U1STAbits.FERR) PORTBbits.RB13=1;
			if (U1STAbits.OERR) PORTBbits.RB13=1;
			if (U1STAbits.OERR) PORTBbits.RB13=1;
			if (U1STAbits.UTXBF) PORTBbits.RB13=1;
			if (!U1STAbits.TRMT) PORTBbits.RB13=1;
			
	  }
 
}//main


void OSC_init(void)
{
//_PLLPRE = ValueN1;
//_PLLPOST = ValueN2;            // PLL VCO Output Divider Select bits (N2)
//             // PLL Phase Detector Input Divider bits (N1)
//_PLLDIV = ValueM;
CLKDIVbits.PLLPRE = ValueN1;
PLLFBDbits.PLLDIV = ValueM;             // PLL divider (M=2)
CLKDIVbits.PLLPOST = ValueN2;
CLKDIVbits.DOZE=0b011;  ////011 = FCY/8 (default)	
OSCTUN = 0; // Tune FRC oscillator, if FRC is used
RCONbits.SWDTEN = 0; // Disable Watch Dog Timer
while(OSCCONbits.LOCK != 1) {}; // Wait for PLL to lock
//U1MODEbits.STSEL = 0; // 1 Stop bit
//U1MODEbits.PDSEL = 0; // No Parity, 8 data bits
//U1MODEbits.ABAUD = 0; // Auto-Baud Disabled
//U1MODEbits.BRGH = 0; // Low Speed mode
//U1BRG = BRGVAL;//BRGVAL; // BAUD Rate 
//U1MODEbits.UARTEN = 1; // Enable UART
//U1STAbits.UTXEN = 1; // Enable UART TX
//
U2MODEbits.STSEL = 0; // 1 Stop bit
U2MODEbits.PDSEL = 0; // No Parity, 8 data bits
U2MODEbits.ABAUD = 0; // Auto-Baud Disabled
U2MODEbits.BRGH = 0; // Low Speed mode
U2BRG = BRGVAL2;//BRGVAL; // BAUD Rate 
U2MODEbits.UARTEN = 1; // Enable UART
U2STAbits.UTXEN = 1; // Enable UART TX

U1BRG  = BRGVAL;         //Baud Rate Generator Prescaler
U1MODE = 0x8000;         // Reset UART to 8-n-1, alt pins, and enable 
U1MODEbits.UARTEN = 1; // Enable UART
U1STAbits.UTXEN = 1; // Enable UART TX   U1STA  = 0x0440;         // Reset status register and enable TX & RX
   
   IPC2bits.U1RXIP = 4;    //set recieve interrupt priority to 4
   IEC0bits.U1RXIE = 1;    //recieve interrupt enable
   _U1RXIF=0;              // Clear UART RX Interrupt Flag
   U1STAbits.URXISEL = 0;  //interrupt when any character is recieved}
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
int enteroRecibido,enteroRecibido2;
char coderecu;
unsigned int aux;
TMR2 = 0;
int i;
//int j;
//int k;

int suma1=1850;
//int suma2=1850;
//int suma3=1850;

	PORTBbits.RB15=0; //pour le pwm
	PORTBbits.RB14=0; //pour le pwm
	PORTBbits.RB4=0;
	//PORTBbits.RB13=0;
	
if (read_adc1>equilibre+sans)
	{
		for(i=0; i<=((read_adc1-equilibre)*16);i++)              // sets up next output for each pin
		{
			PORTBbits.RB15=1;//pour le pwm
		} 
		PORTBbits.RB15=0;//pour le pwm
		Delay(1);
	}
if (read_adc1==0)read_adc1=equilibre;
if (read_adc1<equilibre-sans)
	{
		for(i=0; i<=((max-read_adc1-equilibre)*14);i++)              // sets up next output for each pin
		{
			PORTBbits.RB14=1;//pour le pwm
		} 
		PORTBbits.RB14=0;//pour le pwm
		Delay(1);
	}

/*
		for(i=0; i<=(read_adc2+suma1);i++)              // sets up next output for each pin
		{
			PORTBbits.RB4=1;//pour le pwm
		} 
		PORTBbits.RB4=0;//pour le pwm
		Delay(1);
*/
//	Delay(1);	
		
	
		//T2IF: Timer2 Interrupt Flag Status bit	//1 = Interrupt request has occurred
													//0 = Interrupt request has not occurred
		IFS0bits.T2IF = 0;


}

unsigned int prochendata=0;

void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
 {
	if (U1STAbits.PERR) mogd_write_int(1,2,101);
	if (U1STAbits.FERR) mogd_write_int(8,2,102);
	if (U1STAbits.OERR) mogd_write_int(1,3,103);
	if (U1STAbits.OERR) mogd_write_int(8,3,104);
	if (U1STAbits.UTXBF) mogd_write_int(1,4,105);
	if (!U1STAbits.TRMT) mogd_write_int(8,4,106);

       _U1RXIF=0;                    // Clear UART RX Interrupt Flag.
		if (prochendata == 0) 
		{
			code=ReadUART1_mod();
			if (code ==(unsigned int)0x00fd) {prochendata=1;dataready1=0;return;}
			if (code ==(unsigned int)0x00fe) {prochendata=3;dataready2=0;return;}
		}	
		if (prochendata == 1)
		{
			data1=ReadUART1_mod();
			if (data1==0x00fd){prochendata==0; return;}
			if (data1==0x00fe){prochendata==0; return;}
			prochendata=2;
			return;
		}	
		if (prochendata == 2)
		{
			data2=ReadUART1_mod();
			if (data2==0x00fd){prochendata==0; return;}
			if (data2==0x00fe){prochendata==0; return;}
			prochendata=0;
			dataready1=1;
		}	
		if (prochendata == 3)
		{
			data3=ReadUART1_mod();
			if (data3==0x00fd){prochendata==0; return;}
			if (data3==0x00fe){prochendata==0; return;}
			prochendata=4;
			return;
		}	
		if (prochendata == 4)
		{
			data4=ReadUART1_mod();
			if (data4==0x00fd)
			{prochendata==0; return;}
			if (data4==0x00fe)
			{prochendata==0; return;}
			prochendata=0;
			dataready2=1;
		}
		if (dataready1==1)
		{
			read_adc1= (unsigned int)(data1& 0x00ff | data2 <<8 & 0xff00);
			dataready1=0;
			return;
		}	
		if (dataready2==1)
		{
			read_adc2= (unsigned int)(data3& 0x00ff | data4 <<8 & 0xff00);
			dataready2=0;
			return;
		}	
		return;
}

unsigned int ReadUART1_mod(void)
{
	if (U1STAbits.PERR) mogd_write_int(1,2,101);
	if (U1STAbits.FERR) mogd_write_int(8,2,102);
	if (U1STAbits.OERR) mogd_write_int(1,3,103);
	if (U1STAbits.OERR) mogd_write_int(8,3,104);
	if (U1STAbits.UTXBF) mogd_write_int(1,4,105);
	if (!U1STAbits.TRMT) mogd_write_int(8,4,106);
    if(U1MODEbits.PDSEL == 3)
        return (U1RXREG);
    else
        return (U1RXREG & 0xFF);
}
