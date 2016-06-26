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
  
  
; --- Arquivos inclu�dos no projeto ---
  #include <p16f84a.inc>			; inclui o arquivo do PIC16F84A  
  
  
; --- FUSE Bits ---
; Cristal oscilador externo 4MHz, sem watchdog timer, com power up timer, sem prote��o do c�digo
	__config _XT_OSC & _WDT_OFF & _PWRTE_ON & _CP_OFF
	
; --- Os Termos do Set de Instru��es ---
;W: work (registrador de trabalho ou acumulador)
;F: file (registradores especiais e/ou de uso geral | posi��o de mem�ria)
;L: literal (constante, n�mero qualquer) Utilizado como L nas instru��es e k nos argumentos 
;D:  destination  (local  onde  o  resultado  de  uma  opera��o  ser� armazenado). 
;B: bit (bits dentro dos registradores, dentro de um byte, flags) 
;T: test (utilizado para teste de bits) 
;S: skip (pulo, salto, desvio condicional) 
;S: set (setar, tornar n�vel alto) 
;C: clear (limpar, tornar n�vel baixo) 
;Z: zero (testar se � equivalente a zero) 


; --- Pagina��o de Mem�ria ---
 #define		bank0		bcf	STATUS,RP0		;Cria um mnem�nico para o banco 0 de mem�ria
 #define		bank1		bsf STATUS,RP0		;Cria um mnem�nico para o banco 1 de mem�ria