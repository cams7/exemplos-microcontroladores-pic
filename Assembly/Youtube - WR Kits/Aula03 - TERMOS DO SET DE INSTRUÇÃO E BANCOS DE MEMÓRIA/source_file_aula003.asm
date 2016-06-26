;
; Curso de Assembly para PIC Aula 003
;
; MCU: PIC16F84A   Clock: 4MHz
;
; www.wrkits.com.br | facebook.com/wrkits | youtube.com/canalwrkits
;
; Autor: Eng. Wagner Rambo   Data: Novembro 2015
;


  list		p=16f84A				; microcontrolador utilizado PIC16F84A
  
  
; --- Arquivos incluídos no projeto ---
  #include <p16f84a.inc>			; inclui o arquivo do PIC16F84A  
  
  
; --- FUSE Bits ---
; Cristal oscilador externo 4MHz, sem watchdog timer, com power up timer, sem proteção do código
	__config _XT_OSC & _WDT_OFF & _PWRTE_ON & _CP_OFF
	
; --- Os Termos do Set de Instruções ---
;W: work (registrador de trabalho ou acumulador)
;F: file (registradores especiais e/ou de uso geral | posição de memória)
;L: literal (constante, número qualquer) Utilizado como L nas instruções e k nos argumentos 
;D:  destination  (local  onde  o  resultado  de  uma  operação  será armazenado). 
;B: bit (bits dentro dos registradores, dentro de um byte, flags) 
;T: test (utilizado para teste de bits) 
;S: skip (pulo, salto, desvio condicional) 
;S: set (setar, tornar nível alto) 
;C: clear (limpar, tornar nível baixo) 
;Z: zero (testar se é equivalente a zero) 


; --- Paginação de Memória ---
 #define		bank0		bcf	STATUS,RP0		;Cria um mnemônico para o banco 0 de memória
 #define		bank1		bsf STATUS,RP0		;Cria um mnemônico para o banco 1 de memória