;
; Curso de Assembly para PIC WR Kits Aula 25
;
;
; Sub-Rotina para Multiplicação
;
; Multiplica dois número (A0 e B0) e armazena o resultado em C0 e C1
;
; Sistema Embarcado Sugerido: PARADOXUS PEPTO
;
; Disponível em https://wrkits.com.br/catalog/show/141 
;
; Clock: 4MHz    Ciclo de máquina = 1µs
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
; - Sem programação em baixa tensão, sem proteção de código, sem proteção da memória EEPROM
	__config _XT_OSC & _WDT_OFF & _PWRTE_ON & _BOREN_OFF & _LVP_OFF & _CP_OFF & _CPD_OFF & _MCLRE_ON
	
	
; --- Paginação de Memória ---
	#define		bank0	bcf	STATUS,RP0		;Cria um mnemônico para selecionar o banco 0 de memória
	#define		bank1	bsf	STATUS,RP0		;Cria um mnemônico para selecionar o banco 1 de memória
	
	
; --- Registradores de Uso Geral ---
	cblock		H'20'						;Início da memória disponível para o usuário
	
	A0										;armazena o conteúdo de um dos números a serem multiplicados
	B0										;armazena o conteúdo de um dos números a serem multiplicados
	C0										;byte menos significativo do resultado
	C1										;byte mais significativo do resultado
 
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
	
 	movlw		D'255'						;Carrega número em W
 	movwf		A0							;A0 = número
 	movlw		D'255'						;Carrega número em W
 	movwf		B0							;B0 = número
 	call		mult						;chama sub-rotina de multiplicação
 											;Retornará -> <C1:C0> = A0 x B0



	goto		$							;prenda o programa nesta linha
		
	
; --- Desenvolvimento das Sub-Rotinas Auxiliares ---
mult:
	clrf		C0							;limpa conteúdo do registrador C0
	clrf		C1							;limpa conteúdo do registrador C1
	movf		A0,W						;envia o conteúdo de A0 para W
	movwf		C0							;envia o conteúdo de W para C0
	
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
	
	