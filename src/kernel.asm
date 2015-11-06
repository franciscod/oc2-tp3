; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

extern GDT_DESC
extern IDT_DESC

GDT_IDX_CODE_0 equ 8
GDT_IDX_DATA_0 equ 10

GDT_OFFSET_CODE_0 equ 64 ; 8x8
GDT_OFFSET_CODE_3 equ 72 ; 8x9
GDT_OFFSET_DATA_0 equ 80 ; 8x10
GDT_OFFSET_DATA_3 equ 88 ; 8x11
GDT_OFFSET_UI     equ 96 ; 8x12

STACK_BASE equ 0x27000 ; 5.1.b enunciado

PAGE_DIRECTORY      equ STACK_BASE ; pag 11 enunciado
SET_PAGING_MASK     equ 0x80000000

extern screen_inicializar, screen_pintar_puntajes
extern game_jugador_inicializar, jugadorA, jugadorB
extern idt_inicializar
extern mmu_inicializar, mmu_inicializar_dir_kernel, mmu_unmapear_pagina
extern game_perro_inicializar
extern game_perro_reciclar_y_lanzar
extern mmu_inicializar_memoria_perro
extern habilitar_pic, resetear_pic
extern perrolandia


global start


;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

nombre_grupo_msg db     'Fuga Villera Numero 2'
nombre_grupo_len equ    $ - nombre_grupo_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 4, 4


    ; Habilitar A20
    call habilitar_A20

    ; Cargar la GDT
    LGDT [GDT_DESC]

    ; Setear el bit PE del registro CR0
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Saltar a modo protegido
    jmp GDT_OFFSET_CODE_0:altosalto

BITS 32
altosalto:
    ; Establecer selectores de segmentos
    mov ax, GDT_OFFSET_DATA_0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Establecer la base de la pila
    mov esp, STACK_BASE
    mov ebp, STACK_BASE

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x07, 2, 0

    call pintar_extremo_ui

    ; Inicializar el juego

    push jugadorA
    call game_jugador_inicializar

    push jugadorB
    call game_jugador_inicializar

    ; Inicializar pantalla
    call screen_inicializar
    call screen_pintar_puntajes

    ; Inicializar el manejador de memoria
    call mmu_inicializar

    ; Inicializar el directorio de paginas
    call mmu_inicializar_dir_kernel

    ; Cargar directorio de paginas
    mov eax, PAGE_DIRECTORY
    mov cr3, eax

    ; Habilitar paginacion
    mov eax, cr0
    or eax, SET_PAGING_MASK
    mov cr0, eax

    imprimir_texto_mp nombre_grupo_msg, nombre_grupo_len, 0x07, 0, 0

    mov eax, cr3

    ; desmapea la ultima pagina del kernel (identity mapping) 5.3.f
    ; push eax
    ; push 0x3ff000
    ; call mmu_unmapear_pagina

    ; 5.4.c
    push 0
    push 0
    push jugadorA
    push perrolandia
    call game_perro_inicializar

    push 0
    push perrolandia
    call game_perro_reciclar_y_lanzar

    push 0
    push 0
    push perrolandia
    call mmu_inicializar_memoria_perro
    
    mov cr3, eax

    call pintar_extremo_ui ; con directotrio del perro
    ; 5.4.c


    ; Inicializar tss

    ; Inicializar tss de la tarea Idle

    ; Inicializar el scheduler

    ; Inicializar la IDT
    call idt_inicializar

    ; Cargar IDT
    LIDT [IDT_DESC]

    ; disparar interrupcion accediendo mal a memoria
    ;call escribir_afuera_de_video

    ; Configurar controlador de interrupciones
    call resetear_pic
    call habilitar_pic

    ; Cargar tarea inicial

    ; Habilitar interrupciones
    sti

    ; Saltar a la primera tarea: Idle

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $



pintar_extremo_ui:

    ; Para probar el funcionamiento de la segmentación, agregar a la GDT un segmento adicional
    ; que describa el área de la pantalla en memoria que pueda ser utilizado sólo por el kernel.
    ; Escribir una rutina que pinte el extremo superior izquierdo de la pantalla utilizando este
    ; segmento.
    push ax
    push es
    mov ax, GDT_OFFSET_UI

    mov es, ax
    mov byte [es:0x01], 0x1B ; fondo azul, letra cyan
    mov byte [es:0x00], 'o'
    mov byte [es:0x03], 0x1B ; fondo azul, letra cyan
    mov byte [es:0x02], 'l'
    mov byte [es:0x05], 0x1B ; fondo azul, letra cyan
    mov byte [es:0x04], 'a'
    mov byte [es:0x07], 0x1B ; fondo azul, letra cyan
    mov byte [es:0x06], ' '

    pop es
    pop ax

    ret


escribir_afuera_de_video:
    push ax
    push es

    mov ax, GDT_OFFSET_UI

    mov es, ax
    mov byte [es:0xFFFFFFFF], 0x42 ; escribe bien afuera

    pop es
    pop ax
    ret


;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
