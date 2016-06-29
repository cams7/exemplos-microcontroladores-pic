;
; Curso de Assembly para PIC WR Kits Aula 21
;
;
; Configurando entradas e saídas no PIC 16F628A
;
; Clock: 4MHz    Ciclo de máquina = 1µs
;
; Autor: Eng. Wagner Rambo   Data: Março de 2016
;
;

; --- Listagem do Processador Utilizado ---
	list	p=16F628A						;Utilizado PIC16F628A
	
	
; --- Arquivos Inclusos no Projeto ---
	#include <p16F628a.inc>					;inclui o arquivo do PIC 16F628A
	
	
; --- FUSE Bits ---
; - Cristal de 4MHz
; - Desabilitamos o Watch Dog Timer
; - Habilitamos o Power Up Timer
; - Brown Out desabilitado
; - Sem programação em baixa tensão, sem proteção de código, sem proteção da memória EEPROM
	__config _XT_OSC & _WDT_OFF & _PWRTE_ON & _BOREN_OFF & _LVP_OFF & _CP_OFF & _CPD_OFF & _MCLRE_ON
	
	
; --- Paginação de Memória ---
	#define		bank0	bcf	STATUS,RP0		;Cria um mnemônico para selecionar o banco 0 de memória
	#define		bank1	bsf	STATUS,RP0		;Cria um mnemônico para selecionar o banco 1 de memória
	
	
; --- Saídas ---
	#define     led1	PORTA,3				;LED1 onboard ligado ao pino RA3 (modo current sourcing) 0 - desliga 1 - liga
	#define		led2	PORTA,2				;LED2 onboard ligado ao pino RA2 (modo current sourcing) 0 - desliga 1 - liga
	
	
; --- Entrada ---
	#define		S1		PORTB,0				;Botão S1 ligado em RB0
	
	
	
; --- Vetor de RESET ---
	org			H'0000'						;Origem no endereço 00h de memória
	goto		inicio						;Desvia para a label início
	

; --- Vetor de Interrupção ---
	org			H'0004'						;As interrupções deste processador apontam para este endereço
	retfie									;Retorna da interrupção
	
	
inicio:
	bank0									;Seleciona o banco 0 de memória
	movlw		H'07'						;w = 7h
	movwf		CMCON						;CMCON = 7h (apenas I/Os digitais) desabilita os comparadores internos
	bank1									;Seleciona o banco 1 de memória
	
	movlw		H'13'						;w = 03h
	movwf		TRISA						;Configura saídas para os leds onboard
	movlw		H'FF'						;w = FFh
	movwf		TRISB						;Configura todo o PORTB como entrada
	bank0									;Seleciona o banco 0 de memória
	
	bcf			led1						;LED1 inicia desligado
	bcf			led2						;LED2 inicia desligado
	
	
 
	
	
loop:
	
	goto		loop						;loop infinito
	


	
	
	end										;Final do Programa
	
	