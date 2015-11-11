/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "mmu.h"
#include "defines.h"
#include "screen.h"

#define EEFLAGS_INTERRUPCIONES  0x00000202
#define INICIO_CODIGO_TAREAS    0x00401000
#define INICIO_PILA_TAREAS      0x00402000
// inicioTSSs
tss tss_inicial;
tss tss_idle;
tss tss_perrito;
tss tss_jugadorA[MAX_CANT_PERROS_VIVOS];
tss tss_jugadorB[MAX_CANT_PERROS_VIVOS];

void tss_inicializar() {
  // cargamos fruta en la tarea inicial para poder verlo con bochs
  tss_inicial.eax = 0x12344321;
  tss_inicial.ebx = 0x0CACAFEA;
  tss_inicial.ecx = 0x42424242;
  tss_inicial.edx = 0xCAFEC170;

  completar_tss(&tss_idle,
    (GDT_IDX_CODE_0 << 3) | 0, //FIXME privs
    (GDT_IDX_DATA_0 << 3) | 0, //FIXME privs
    STACK_BASE,
    TAREA_IDLE,
    EEFLAGS_INTERRUPCIONES,
    PAGE_DIRECTORY,
    (GDT_IDX_DATA_0 << 3) | 0, //FIXME privs
    STACK_BASE
  );

  cargar_tss_en_gdt(&tss_inicial, &gdt[GDT_IDX_TSS_INICIAL]);
  cargar_tss_en_gdt(&tss_idle, &gdt[GDT_IDX_TSS_IDLE]);
}


void completar_tss(tss* entrada_tss,
                   unsigned short cs,
                   unsigned short ds,
                   uint esp,
                   uint eip,
                   uint eeflags,
                   uint cr3,
                   unsigned short ss0,
                   uint esp0
                 ) {

  print_hex((uint) &tss_perrito, 8, 10, 6, 0x7c);
  print_hex((uint) entrada_tss,  8, 10, 7, 0x7c);


  entrada_tss->cr3 = cr3;

  entrada_tss->eip = eip;

  entrada_tss->esp = esp;
  entrada_tss->ebp = esp;

  entrada_tss->esp0 = esp0;
  entrada_tss->esp1 = esp0;
  entrada_tss->esp2 = esp0;

  entrada_tss->cs = cs;

  entrada_tss->ds = ds;
  entrada_tss->es = ds;
  entrada_tss->fs = ds;
  entrada_tss->gs = ds;
  entrada_tss->ss = ds;

  entrada_tss->ss0 = ss0;
  entrada_tss->ss1 = ss0;
  entrada_tss->ss2 = ss0;

  entrada_tss->eax = 0x00000000;
  entrada_tss->ecx = 0x00000000;
  entrada_tss->edx = 0x00000000;
  entrada_tss->ebx = 0x00000000;

  entrada_tss->esi = 0x00000000;
  entrada_tss->edi = 0x00000000;

  entrada_tss->eflags = eeflags;
  entrada_tss->iomap = 0xFFFF;

}

void completar_tss_tarea(tss* entrada_tss, perro_t *perro, int index_jugador, int index_tipo) {

  uint directorio_tarea = mmu_inicializar_memoria_perro(perro, index_jugador, index_tipo);

  print_hex((uint) &tss_perrito, 8, 10, 3, 0x7c);
  print_hex((uint) entrada_tss,  8, 10, 4, 0x7c);

  completar_tss(entrada_tss,
    (GDT_IDX_CODE_3 << 3) | 0, //FIXME privs
    (GDT_IDX_DATA_3 << 3) | 0, //FIXME privs
    INICIO_PILA_TAREAS - 3 * sizeof(uint),
    INICIO_CODIGO_TAREAS,
    EEFLAGS_INTERRUPCIONES,
    directorio_tarea,
    (GDT_IDX_DATA_0 << 3) | 0,
    // la pila arranca desde el fin de la pagina y va subiendo al principio
    mmu_proxima_pagina_fisica_libre() + PAGE_SIZE
  );
}

void cargar_tss_en_gdt(tss* entrada_tss,
                       gdt_entry* entrada_gdt) {

    print_hex((uint) &tss_inicial,  8, 10, 10, 0x7c);
    print_hex((uint) &tss_idle,     8, 10, 11, 0x7c);
    print_hex((uint) &tss_perrito, 8, 10, 12, 0x7c);
    print_hex((uint) &tss_jugadorA, 8, 10, 13, 0x7c);
    print_hex((uint) &tss_jugadorB, 8, 10, 14, 0x7c);
    print_hex((uint) entrada_tss,  8, 10, 16, 0x7c);

    print_hex((uint) &gdt[GDT_IDX_TSS_INICIAL],  8, 20, 10, 0x7c);
    print_hex((uint) &gdt[GDT_IDX_TSS_IDLE],     8, 20, 11, 0x7c);
    print_hex((uint) &gdt[GDT_IDX_TSS_PERRO_START], 8, 20, 12, 0x7c);
    print_hex((uint) &gdt[GDT_IDX_TSS_PERRO_START+8], 8, 20, 13, 0x7c);
    print_hex((uint) entrada_gdt,  8, 20, 16, 0x7c);

    //breakpoint();

    uint base = (uint) entrada_tss;

    entrada_gdt->limit_0_15  = 0x67; // 104 bytes - 1
    entrada_gdt->limit_16_19 = 0x0;
    entrada_gdt->base_0_15   = base & 0xFFFF;
    entrada_gdt->base_23_16  = (base >> 16) & 0xFF;
    entrada_gdt->base_31_24  = (base >> 24) & 0xFF;

    entrada_gdt->type        = 0b1001;
    entrada_gdt->s           = 0b0;
    entrada_gdt->dpl         = 0b00; //FIXME privs
    entrada_gdt->p           = 0b1;
    entrada_gdt->avl         = 1;
    entrada_gdt->l           = 0;
    entrada_gdt->db          = 0;
    entrada_gdt->g           = 0; // granularidad de 1byte

}
