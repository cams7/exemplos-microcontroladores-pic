;
; Curso de Assembly para PIC WR Kits Aula 21
;
;
; Aula pr�tica de Entradas e Sa�das
;
; Sistema Embarcado Sugerido: PARADOXUS PEPTO
;
; Dispon�vel em https://wrkits.com.br/catalog/show/141 
;
; Clock: 4MHz    Ciclo de m�quina = 1�s
;
; Autor: Eng. Wagner Rambo   Data: Abril de 2016
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
; - Sem programa��o em baixa tens�o, sem prote��o de c�digo, sem prote��o da mem�ria EEPROM
	__config _XT_OSC & _WDT_OFF & _PWRTE_ON & _BOREN_OFF & _LVP_OFF & _CP_OFF & _CPD_OFF & _MCLRE_ON
	
	
; --- Pagina��o de Mem�ria ---
	#define		bank0	bcf	STATUS,RP0		;Cria um mnem�nico para selecionar o banco 0 de mem�ria
	#define		bank1	bsf	STATUS,RP0		;Cria um mnem�nico para selecionar o banco 1 de mem�ria
	
	
; --- Sa�das (Dispositivos onboard PEPTO) ---
	#define		led1	PORTA,RA2			;LED1 onboard ligado ao pino RA2 (modo current sourcing) 0 - desliga 1 - liga
	#define     led2	PORTA,RA3			;LED2 onboard ligado ao pino RA3 (modo current sourcing) 0 - desliga 1 - liga
	
	
	
; --- Entrada (Dispositivo onboard PEPTO) ---
	#define		S1		PORTB,RB0			;Bot�o S1 ligado em RB0 (bot�o pressionado - n�vel low)
	
	
	
; --- Vetor de RESET ---
	org			H'0000'						;Origem no endere�o 00h de mem�ria
	goto		init						;Desvia para a label in�cio
	

; --- Vetor de Interrup��o ---
	org			H'0004'						;As interrup��es deste processador apontam para este endere�o
	retfie									;Retorna da interrup��o
	
	
init:
	bank0									;Seleciona o banco 0 de mem�ria
	movlw		H'07'						;w = 7h
	movwf		CMCON						;CMCON = 7h (apenas I/Os digitais) desabilita os comparadores internos
	bank1									;Seleciona o banco 1 de mem�ria
	movlw		H'00'						;w = 00h
	movwf		OPTION_REG					;Habilita resistores de pull-up internos
	movlw		H'13'						;w = 03h
	movwf		TRISA						;Configura sa�das para os leds onboard
	movlw		H'FF'						;w = FFh
	movwf		TRISB						;Configura todo o PORTB como entrada
	bank0									;Seleciona o banco 0 de mem�ria
	
leds_desligados:
	bcf			led1						;O LED1 inicia desligado
	bcf			led2						;O LED2 inicia desligado
	
main:
	btfss		S1							;Bot�o pressionado?
	goto		leds_ligados				;Sim, desvia para a subrotina 'leds_ligados'
	goto		leds_desligados				;N�o, desvia para a subrotina 'leds_desligados'
	
leds_ligados:
	bsf			led1						;Liga o LED1
	bsf			led2						;Liga o LED2
	goto		main						;loop infinito	
	
	end										;Final do Programa	