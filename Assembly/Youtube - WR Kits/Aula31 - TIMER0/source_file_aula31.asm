;
; Curso de Assembly para PIC WR Kits Aula 30
;
;
; Como Entrar e Sair de Uma Interrupção em Assembly (Verificando o salvamento de Contexto)
;
; Aprenda o processo de Salvamento de Contexto
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
	
	W_TEMP									;Registrador para armazenar o conteúdo temporário de work
	STATUS_TEMP								;Registrador para armazenar o conteúdo temporário de STATUS
	 
 
	endc									;Final da memória do usuário
	

; --- Vetor de RESET ---
	org			H'0000'						;Origem no endereço 00h de memória
	goto		inicio						;Desvia para a label início
	

; --- Vetor de Interrupção ---
	org			H'0004'						;As interrupções deste processador apontam para este endereço
	
; -- Salva Contexto --
	movwf 		W_TEMP						;Copia o conteúdo de Work para W_TEMP
	swapf 		STATUS,W  					;Move o conteúdo de STATUS com os nibbles invertidos para Work
	bank0									;Seleciona o banco 0 de memória (padrão do RESET) 
	movwf 		STATUS_TEMP					;Copia o conteúdo de STATUS com os nibbles invertidos para STATUS_TEMP
; -- Final do Salvamento de Contexto --


	; Trata ISR...



; -- Recupera Contexto (Saída da Interrupção) --
exit_ISR:

	swapf 		STATUS_TEMP,W				;Copia em Work o conteúdo de STATUS_TEMP com os nibbles invertidos
	movwf 		STATUS 						;Recupera o conteúdo de STATUS
	swapf 		W_TEMP,F 					;W_TEMP = W_TEMP com os nibbles invertidos 
	swapf 		W_TEMP,W  					;Recupera o conteúdo de Work
	
	retfie									;Retorna da interrupção
	
	
inicio:

	bank1									;Seleciona o banco 1 de memória
	movlw		H'88'						;w = 80h
	movwf		OPTION_REG					;CONFIGURAR OPTION_REG...
											;Página 23 do datasheet
											;OPTION_REG – OPTION REGISTER (ADDRESS: 81h, 181h)											
											;RBPU INTEDG T0CS T0SE PSA PS2 PS1 PS0
											;1    0      0    0    1   0   	0   0

											;	RBPU: PORTB Pull-up Enable bit
											;(x)1 = PORTB pull-ups are disabled
											;	0 = PORTB pull-ups are enabled by individual port latch values

											;	INTEDG: Interrupt Edge Select bit
											;	1 = Interrupt on rising edge of RB0/INT pin
											;(x)0 = Interrupt on falling edge of RB0/INT pin

											;	T0CS: TMR0 Clock Source Select bit
											;	1 = Transition on RA4/T0CKI/CMP2 pin
											;(x)0 = Internal instruction cycle clock (CLKOUT)

											;	T0SE: TMR0 Source Edge Select bit
											;	1 = Increment on high-to-low transition on RA4/T0CKI/CMP2 pin
											;(x)0 = Increment on low-to-high transition on RA4/T0CKI/CMP2 pin
	
											;	PSA: Prescaler Assignment bit
											;(x)1 = Prescaler is assigned to the WDT
											;	0 = Prescaler is assigned to the Timer0 module

											;	PS<2:0>: Prescaler Rate Select bits

											;	Bit Value	TMR0 Rate	WDT Rate
											;(x)000			1:2			1:1
											;	001			1:4			1:2
											;	010			1:8			1:4
											;	011			1:16		1:8
											;	100			1:32		1:16
											;	101			1:64		1:32
											;	110			1:128		1:64
											;	111			1:256		1:128

	movlw		H'FE'						;w = FEh
	movwf		TRISB						;Configura entradas/saídas PORTB
	bank0									;Seleciona o banco 0 de memória
	movlw		H'07'						;w = 7h
	movwf		CMCON						;CMCON = 7h desabilita os comparadores internos
											;Página 61 do datasheet
											;CMCON – COMPARATOR CONFIGURATION REGISTER (ADDRESS: 01Fh)
											;C2OUT C1OUT C2INV C1INV CIS CM2 CM1 CM0
											;0     0     0     0     0   1   1   1

											;	C2OUT: Comparator 2 Output bit
											;	When C2INV = 0:
											;		1 = C2 VIN+ > C2 VIN-
											;(x)	0 = C2 VIN+ < C2 VIN-
											;	When C2INV = 1:
											;		1 = C2 VIN+ < C2 VIN-
											;		0 = C2 VIN+ > C2 VIN- 

											;	C1OUT: Comparator 1 Output bit
											;	When C1INV = 0:
											;		1 = C1 VIN+ > C1 VIN-
											;(x)	0 = C1 VIN+ < C1 VIN-
											;	When C1INV = 1:
											;		1 = C1 VIN+ < C1 VIN-
											;		0 = C1 VIN+ > C1 VIN-
											
											;	C2INV: Comparator 2 Output Inversion bit
											;	1 = C2 Output inverted
											;(x)0 = C2 Output not inverted
											
											;	C1INV: Comparator 1 Output Inversion bit
											;	1 = C1 Output inverted
											;(x)0 = C1 Output not inverted
											
											;	CIS: Comparator Input Switch bit
											;	When CM<2:0>: = 001 Then:
											;		1 = C1 VIN- connects to RA3
											;		0 = C1 VIN- connects to RA0
											;	When CM<2:0> = 010 Then:
											;		1 = C1 VIN- connects to RA3
											;       	C2 VIN- connects to RA2
											;		0 = C1 VIN- connects to RA0
											;       	C2 VIN- connects to RA1

											;	CM<2:0>: Comparator Mode bits
											;	000 - Comparators Reset   (POR Default Value)
											;(x)111 - Comparators Off
											;	100 - Two Independent Comparators
											;	010 - Four Inputs Multiplexed to Two Comparators
											;	011 - Two Common Reference Comparators
											;	110 - Two Common Reference Comparators with Outputs
											;	101 - One Independent Comparator
											;	001 - Three Inputs Multiplexed to Two Comparators

	movlw		H'00'						;w = 00h
	movwf		INTCON						;CONFIGURAR INTCON...
											;Página 24 do datasheet
											;INTCON – INTERRUPT CONTROL REGISTER (ADDRESS: 0Bh, 8Bh, 10Bh, 18Bh)
											;GIE PEIE T0IE INTE RBIE T0IF INTF RBIF
											;0   0    0    0    0    0    0    0

											;	GIE: Global Interrupt Enable bit
											;	1 = Enables all un-masked interrupts
											;(x)0 = Disables all interrupts
											
											;	PEIE: Peripheral Interrupt Enable bit
											;	1 = Enables all un-masked peripheral interrupts
											;(x)0 = Disables all peripheral interrupts
											
											;	T0IE: TMR0 Overflow Interrupt Enable bit
											;	1 = Enables the TMR0 interrupt
											;(x)0 = Disables the TMR0 interrupt
											
											;	INTE: RB0/INT External Interrupt Enable bit
											;	1 = Enables the RB0/INT external interrupt
											;(x)0 = Disables the RB0/INT external interrupt
											
											;	RBIE: RB Port Change Interrupt Enable bit
											;	1 = Enables the RB port change interrupt
											;(x)0 = Disables the RB port change interrupt
											
											;	T0IF: TMR0 Overflow Interrupt Flag bit
											;	1 = TMR0 register has overflowed (must be cleared in software)
											;(x)0 = TMR0 register did not overflow
											
											;	INTF: RB0/INT External Interrupt Flag bit
											;	1 = The RB0/INT external interrupt occurred (must be cleared in software)
											;(x)0 = The RB0/INT external interrupt did not occur
											
											;	RBIF: RB Port Change Interrupt Flag bit
											;	1 = When at least one of the RB<7:4> pins changes state (must be cleared in software)
											;(x)0 = None of the RB<7:4> pins have changed state
	
 
loop:

	nop
	


	goto		loop						;Loop infinito

	 
	 
		
	
	
	end										;Final do Programa
	
	
	
