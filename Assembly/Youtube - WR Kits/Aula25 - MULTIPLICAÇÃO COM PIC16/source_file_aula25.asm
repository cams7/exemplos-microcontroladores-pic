;
; Curso de Assembly para PIC WR Kits Aula 25
;
;
; Sub-Rotina para Multiplica��o
;
; Multiplica dois n�mero (A0 e B0) e armazena o resultado em C0 e C1
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
	
	
; --- Registradores de Uso Geral ---
	cblock		H'20'						;In�cio da mem�ria dispon�vel para o usu�rio
	
	A0										;armazena o conte�do de um dos n�meros a serem multiplicados
	B0										;armazena o conte�do de um dos n�meros a serem multiplicados
	C0										;byte menos significativo do resultado
	C1										;byte mais significativo do resultado
 
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
	
 	movlw		D'255'						;Carrega n�mero em W
 	movwf		A0							;A0 = n�mero
 	movlw		D'255'						;Carrega n�mero em W
 	movwf		B0							;B0 = n�mero
 	call		mult						;chama sub-rotina de multiplica��o
 											;Retornar� -> <C1:C0> = A0 x B0



	goto		$							;prenda o programa nesta linha
		
	
; --- Desenvolvimento das Sub-Rotinas Auxiliares ---
mult:
	clrf		C0							;limpa conte�do do registrador C0
	clrf		C1							;limpa conte�do do registrador C1
	movf		A0,W						;envia o conte�do de A0 para W
	movwf		C0							;envia o conte�do de W para C0
	
loop_mult:
	decf		B0,F						;decrementa B0
	btfsc		STATUS,Z					;B0 igual a zero?
	return									;Sim, retorna
	movf		A0,W						;copia A0 para W
	addwf		C0,F						;Soma A0 com C0 e armazena resultado em C0
	btfsc		STATUS,C					;Houve transbordo em C0?
	incf		C1,F						;Sim, incrementa C1
	goto		loop_mult					;Vai para label loop_mult
	

	
	end										;Final do Programa
	
	