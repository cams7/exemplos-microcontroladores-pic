;
; Curso de Assembly para PIC WR Kits Aula 20
;
;
; Inicialização dos registradores do PIC 16F628A
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
	
	
	
; --- Vetor de RESET ---
	org			H'0000'						;Origem no endereço 00h de memória
	goto		inicio						;Desvia para a label início
	

; --- Vetor de Interrupção ---
	org			H'0004'						;As interrupções deste processador apontam para este endereço
	retfie									;Retorna da interrupção
	
	
inicio:
	bank0									;Seleciona o banco 0 de memória
	movlw		H'07'						;w = 7h
	movwf		CMCON						;CMCON = 7h
	bank1									;Seleciona o banco 1 de memória
	movlw		H'00'						;w = 00h
	movwf		TRISB						;Configura todo o PORTB como saída digital
	bank0
	movlw		H'00'						;w = 00h
	movwf		PORTB						;inicializa PORTB todo em low
	
	
loop:
	
	goto		loop						;loop infinito
	


	
	
	end										;Final do Programa
	
	