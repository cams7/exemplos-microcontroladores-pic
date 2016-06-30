;
; Curso de Assembly para PIC WR Kits Aula 27
;
;
; Sub-Rotina para Divis�o
;
; Divide dois n�mero (num1 e num2) e armazena o resultado em aux1
;
; Sistema Embarcado Sugerido: PARADOXUS PEPTO
;
; Dispon�vel em https://wrkits.com.br/catalog/show/141 
;
; Clock: 4MHz    Ciclo de m�quina = 1�s
;
; Autor: Eng. Wagner Rambo   Data: Maio de 2016
;
;

; --- Listagem do Processador Utilizado ---
	list		p=16F628A					;Utilizado PIC16F628A
	
	
; --- Arquivos Inclusos no Projeto ---
	#include<p16F628a.inc>					;inclui o arquivo do PIC 16F628A
	
	
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
	
	
; --- Registradores de Uso Geral ---
	cblock		H'20'						;In�cio da mem�ria dispon�vel para o usu�rio
	aux1									;Armazena o conte�do do quociente em 'aux1'
	aux2 									;Armazena o conte�do do resto em 'aux2'
	
	num1									;Armazena o conte�do do dividendo ou do quociente em 'num1'
	num2									;Armazena o conte�do do divisor ou do resto em 'num2'	
	endc									;Final da mem�ria do usu�rio
	

; --- Vetor de RESET ---
	org			H'0000'						;Origem no endere�o 00h de mem�ria
	goto		inicio						;Desvia para a label in�cio
	

; --- Vetor de Interrup��o ---
	org			H'0004'						;As interrup��es deste processador apontam para este endere�o
	retfie									;Retorna da interrup��o
	
	
inicio:
	bank0									;Seleciona o banco 0 de mem�ria
	movlw		H'07'						;w = 7h
	movwf		CMCON						;CMCON = 7h (apenas I/Os digitais) desabilita os comparadores internos
	bank1									;Seleciona o banco 1 de mem�ria
	movlw		H'00'						;w = 00h
	movwf		OPTION_REG					;Habilita resistores de pull-up internos
	bank0									;Seleciona o banco 0 de mem�ria
	
 	movlw		D'244'						;w = 244
 	movwf		num1						;num1 = w
 	movlw		D'17'						;w = 17
 	movwf		num2						;num2 = w
 	call		div							;Chama sub-rotina de divis�o
 											;Retornar� -> num1(dividendo)/num2(divisor) = num1(quociente) + num2(resto)

	goto		$							;Prenda o programa nesta linha
		
	
; --- Desenvolvimento das Sub-Rotinas Auxiliares ---
div:
	clrf		aux1						;Limpa registrador 'aux1'
	clrf		aux2						;Limpa registrador 'aux2'							
	
	
div_loop:
	movf		num1,W						;w = num1
	movwf		aux2						;aux2 = w
	movf		num2,W						;w = num2
	subwf		num1,F						;num1 = num1 - w
	btfss		STATUS,C					;Testa para ver se houve carry
	goto		div_menor					;num1(dividendo) menor que zero, desvia para label div_menor
	incf		aux1,F						;Se num1(dividendo) maior que zero incrementa o aux1(quociente)
	goto		div_loop					;Retorna para novo ciclo de subtra��o
	
div_menor:
	movf	aux1,W							;w = aux1
	movwf	num1							;num1 = w
	movf	aux2,W							;w = aux2
	movwf	num2							;num2 = w
	return									;Retorna
	
	end										;Final do Programa	
	