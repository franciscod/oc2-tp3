; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00

;; PIC
extern fin_intr_pic1

;; Sched
extern sched_atender_tick
extern sched_tarea_actual

interrupcion_desconocida_cuelgue_msg db     'INTERRUPCION DESCONOCIDA, KCSHO, ME CUELGO'
interrupcion_desconocida_cuelgue_len equ    $ - interrupcion_desconocida_cuelgue_msg

extern screen_actualizar_reloj_global
extern print_hex
;;
extern game_atender_tick, game_atender_teclado, game_syscall_manejar

;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
    mov eax, %1
    ; TODO
    ; Modificar las rutinas de excepciones del procesador para que desalojen a la tarea que
    ; estaba corriendo y corran la proxima (reemplazar con la idle?)

    imprimir_texto_mp interrupcion_desconocida_cuelgue_msg, interrupcion_desconocida_cuelgue_len, 0x07, 8, 8
    jmp $

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

        push 0 ; FIXME perro actual
        call game_atender_tick
        add esp, 4

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

        push eax
        call game_atender_teclado
        add esp, 4

    popad
    iret

;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
global _isr70
_isr70:
    pushad
    call fin_intr_pic1

        push ecx
        push eax
        call game_syscall_manejar
        add esp, 8

    popad
    iret
