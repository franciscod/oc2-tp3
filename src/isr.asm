; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00

GDT_SELECTOR_TSS_IDLE        equ 112 ; 8x14

;; PIC
extern fin_intr_pic1

;; Sched
extern sched_atender_tick
extern sched_tarea_actual
extern sched_desalojame_esta

excepcion_cpu_0_msg db 'Excepcion 0 -- me cuelgo'
excepcion_cpu_0_len equ $ - excepcion_cpu_0_msg

excepcion_cpu_1_msg db 'Excepcion 1 -- me cuelgo'
excepcion_cpu_1_len equ $ - excepcion_cpu_1_msg

excepcion_cpu_2_msg db 'Excepcion 2 -- me cuelgo'
excepcion_cpu_2_len equ $ - excepcion_cpu_2_msg

excepcion_cpu_3_msg db 'Excepcion 3 -- me cuelgo'
excepcion_cpu_3_len equ $ - excepcion_cpu_3_msg

excepcion_cpu_4_msg db 'Excepcion 4 -- me cuelgo'
excepcion_cpu_4_len equ $ - excepcion_cpu_4_msg

excepcion_cpu_5_msg db 'Excepcion 5 -- me cuelgo'
excepcion_cpu_5_len equ $ - excepcion_cpu_5_msg

excepcion_cpu_6_msg db 'Excepcion 6 -- me cuelgo'
excepcion_cpu_6_len equ $ - excepcion_cpu_6_msg

excepcion_cpu_7_msg db 'Excepcion 7 -- me cuelgo'
excepcion_cpu_7_len equ $ - excepcion_cpu_7_msg

excepcion_cpu_8_msg db 'Excepcion 8 -- me cuelgo'
excepcion_cpu_8_len equ $ - excepcion_cpu_8_msg

excepcion_cpu_9_msg db 'Excepcion 9 -- me cuelgo'
excepcion_cpu_9_len equ $ - excepcion_cpu_9_msg

excepcion_cpu_10_msg db 'Excepcion 10 -- me cuelgo'
excepcion_cpu_10_len equ $ - excepcion_cpu_10_msg

excepcion_cpu_11_msg db 'Excepcion 11 -- me cuelgo'
excepcion_cpu_11_len equ $ - excepcion_cpu_11_msg

excepcion_cpu_12_msg db 'Excepcion 12 -- me cuelgo'
excepcion_cpu_12_len equ $ - excepcion_cpu_12_msg

excepcion_cpu_13_msg db 'Excepcion 13: GENERAL PROTECTION -- me cuelgo'
excepcion_cpu_13_len equ $ - excepcion_cpu_13_msg

excepcion_cpu_14_msg db 'Excepcion 14: PAGE FAULT -- me cuelgo'
excepcion_cpu_14_len equ $ - excepcion_cpu_14_msg

excepcion_cpu_15_msg db 'Excepcion 15 -- me cuelgo'
excepcion_cpu_15_len equ $ - excepcion_cpu_15_msg

excepcion_cpu_16_msg db 'Excepcion 16 -- me cuelgo'
excepcion_cpu_16_len equ $ - excepcion_cpu_16_msg

excepcion_cpu_17_msg db 'Excepcion 17 -- me cuelgo'
excepcion_cpu_17_len equ $ - excepcion_cpu_17_msg

excepcion_cpu_18_msg db 'Excepcion 18 -- me cuelgo'
excepcion_cpu_18_len equ $ - excepcion_cpu_18_msg

excepcion_cpu_19_msg db 'Excepcion 19 -- me cuelgo'
excepcion_cpu_19_len equ $ - excepcion_cpu_19_msg


extern screen_actualizar_reloj_global, screen_pintar_info_debug
extern print_hex

extern game_atender_tick, game_atender_teclado, game_syscall_manejar
extern debugging_mode

;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
    mov eax, %1

    ;TODO debugger
    ; Veo si esta el modo debugging activo
    ; Si esta tengo que mostrar el estado del perro destruido (¿TSS?)
    ; El juego se detiene hasta la proxima pulsacion de 'y'
    ; (¿Como hago para dejarlo detenido y que sea capaz de atrapar las interrupciones de teclado?)
    ; (Una vez que haya mostrado en pantalla deberia de alguna forma proseguir con el desalojo de la tarea?)
    ; Una vez que repulse 'y' se espera hasta el proximo ciclo de reloj para decidir la proxima tarea a ejecutar
    ; Se sigue en modo debuggeo para siempre
    ; mov dword edx, [debugging_mode]
    ; cmp edx, 0

    cmp byte [debugging_mode], 0
    je .desaloja

    call screen_pintar_info_debug

    mov byte [debugging_mode], 0    ; Cuando vuelva a tocar la y vuelve a poner el flag en 1

    .paused:
    cmp byte [debugging_mode], 0
    je .paused

    .desaloja:

    call sched_desalojame_esta
    jmp GDT_SELECTOR_TSS_IDLE:0

    ;imprimir_texto_mp excepcion_cpu_%1_msg, excepcion_cpu_%1_len, 0x07, 8, 8
    ;jmp $
    
%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler

;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19

;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
global _isr32
_isr32:
    pushad

    call fin_intr_pic1

    call sched_atender_tick

    str cx
    cmp ax, cx
    je .fin

        mov word [sched_tarea_selector], ax
        jmp far [sched_tarea_offset]

    .fin:

    popad
iret
;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
global _isr33
_isr33:
    pushad
    call fin_intr_pic1

        xor eax, eax
        in al, 0x60
        test al, 10000000b
        jnz .fin

        push eax
        call game_atender_teclado
        add esp, 4

    .fin:

    popad
    iret

;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
global _isr70
_isr70:
    push 0x42424242 ; le hace lugarcito al ret value
    pushad ; pushea 32bytes
        push ecx
        push eax
        call fin_intr_pic1

        call game_syscall_manejar
        add esp, 8

        mov dword [esp + 32], eax ; entierra el ret value despues del pushad

        jmp GDT_SELECTOR_TSS_IDLE:0 ; idlea un toque
        ;...vuelve del idle

    popad ; levanta 32 bytes
    pop eax ; levanta el ret value
    iret ; y se lo devuelve a la tarea
