# MPLAB IDE generated this makefile for use with GNU make.
# Project: emetteur.mcp
# Date: Mon Sep 09 10:53:08 2013

AS = pic30-as.exe
CC = pic30-gcc.exe
LD = pic30-ld.exe
AR = pic30-ar.exe
HX = pic30-bin2hex.exe
RM = rm

emetteur.hex : emetteur.cof
	$(HX) "emetteur.cof"

emetteur.cof : dsPIC33FJ128GP802-01_lémetteur_ver_60.o
	$(CC) -mcpu=33FJ128MC802 "dsPIC33FJ128GP802-01_lémetteur_ver_60.o" -o"emetteur.cof" -Wl,-Tp33FJ128MC802.gld,--defsym=__MPLAB_BUILD=1,-Map="emetteur.map",--report-mem

dsPIC33FJ128GP802-01_lémetteur_ver_60.o : ../../../../../program\ files\ (x86)/microchip/mplabc30/v3.30c/include/string.h ../../../../../program\ files\ (x86)/microchip/mplabc30/v3.30c/support/dsPIC33F/h/p33FJ128MC802.h ../../../../../program\ files\ (x86)/microchip/mplabc30/v3.30c/support/dsPIC33F/h/p33Fxxxx.h ../../../../../program\ files\ (x86)/microchip/mplabc30/v3.30c/support/peripheral_30F_24H_33F/uart.h ../../../../../program\ files\ (x86)/microchip/mplabc30/v3.30c/support/dsPIC33F/h/p33FJ128MC802.h ../../../../../program\ files\ (x86)/microchip/mplabc30/v3.30c/support/dsPIC33F/h/p33Fxxxx.h ../../../../../program\ files\ (x86)/microchip/mplabc30/v3.30c/support/peripheral_30F_24H_33F/timer.h ../../../../../program\ files\ (x86)/microchip/mplabc30/v3.30c/support/dsPIC33F/h/p33FJ128MC802.h ../../../../../program\ files\ (x86)/microchip/mplabc30/v3.30c/support/dsPIC33F/h/p33Fxxxx.h ../../../../../program\ files\ (x86)/microchip/mplabc30/v3.30c/support/peripheral_30F_24H_33F/outcompare.h ../../../../../program\ files\ (x86)/microchip/mplabc30/v3.30c/include/math.h ../../../../../program\ files\ (x86)/microchip/mplabc30/v3.30c/include/stdlib.h ../../../../../program\ files\ (x86)/microchip/mplabc30/v3.30c/include/stdarg.h ../../../../../program\ files\ (x86)/microchip/mplabc30/v3.30c/include/stddef.h ../../../../../program\ files\ (x86)/microchip/mplabc30/v3.30c/include/stdio.h ../../../../../program\ files\ (x86)/microchip/mplabc30/v3.30c/support/dsPIC33F/h/p33FJ128MC802.h dsPIC33FJ128GP802-01_lémetteur_ver_60.c
	$(CC) -mcpu=33FJ128MC802 -x c -c "dsPIC33FJ128GP802-01_lémetteur_ver_60.c" -o"dsPIC33FJ128GP802-01_lémetteur_ver_60.o" -g -Wall

clean : 
	$(RM) "dsPIC33FJ128GP802-01_lémetteur_ver_60.o" "emetteur.cof" "emetteur.hex"

