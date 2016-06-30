;
; Curso de Assembly para PIC WR Kits Aula 27
;
;
; Sub-Rotina para Divisão
;
; Divide dois número (num1 e num2) e armazena o resultado em aux1
;
; Sistema Embarcado Sugerido: PARADOXUS PEPTO
;
; Disponível em https://wrkits.com.br/catalog/show/141 
;
; Clock: 4MHz    Ciclo de máquina = 1µs
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
; - Sem programação em baixa tensão, sem proteção de código, sem proteção da memória EEPROM
	__config _XT_OSC & _WDT_OFF & _PWRTE_ON & _BOREN_OFF & _LVP_OFF & _CP_OFF & _CPD_OFF & _MCLRE_ON
	
	
; --- Paginação de Memória ---
	#define		bank0	bcf	STATUS,RP0		;Cria um mnemônico para selecionar o banco 0 de memória
	#define		bank1	bsf	STATUS,RP0		;Cria um mnemônico para selecionar o banco 1 de memória
	
	
; --- Registradores de Uso Geral ---
	cblock		H'20'						;Início da memória disponível para o usuário
	aux1									;Armazena o conteúdo do quociente em 'aux1'
	aux2 									;Armazena o conteúdo do resto em 'aux2'
	
	num1									;Armazena o conteúdo do dividendo ou do quociente em 'num1'
	num2									;Armazena o conteúdo do divisor ou do resto em 'num2'	
	endc									;Final da memória do usuário
	

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
	movlw		H'00'						;w = 00h
	movwf		OPTION_REG					;Habilita resistores de pull-up internos
	bank0									;Seleciona o banco 0 de memória
	
 	movlw		D'244'						;w = 244
 	movwf		num1						;num1 = w
 	movlw		D'17'						;w = 17
 	movwf		num2						;num2 = w
 	call		div							;Chama sub-rotina de divisão
 											;Retornará -> num1(dividendo)/num2(divisor) = num1(quociente) + num2(resto)

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
	goto		div_loop					;Retorna para novo ciclo de subtração
	
div_menor:
	movf	aux1,W							;w = aux1
	movwf	num1							;num1 = w
	movf	aux2,W							;w = aux2
	movwf	num2							;num2 = w
	return									;Retorna
	
	end										;Final do Programa	
	