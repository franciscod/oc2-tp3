/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "mmu.h"
#include "defines.h"

#define EEFLAGS_INTERRUPCIONES  0x00000202
// inicioTSSs
tss tss_inicial;
tss tss_idle;
tss tss_jugadorA[MAX_CANT_PERROS_VIVOS];
tss tss_jugadorB[MAX_CANT_PERROS_VIVOS];

void tss_inicializar() {
  // la inicial la dejamos como esta

  // inicializar la idle
  completar_tss(&tss_idle,
    GDT_IDX_CODE_0,
    GDT_IDX_DATA_0,
    STACK_BASE,
    TAREA_IDLE,
    EEFLAGS_INTERRUPCIONES,
    PAGE_DIRECTORY
  );

  // volcarlos a la gdt?
}


void completar_tss(tss* entrada_tss,
                   uint cs,
                   uint ds,
                   uint esp,
                   uint eip,
                   uint eeflags,
                   uint cr3) {
  entrada_tss->cr3 = cr3;

  entrada_tss->eip = eip;

  entrada_tss->esp = esp;
  entrada_tss->ebp = esp;

  entrada_tss->cs = cs;

  entrada_tss->ds = ds;
  entrada_tss->es = ds;
  entrada_tss->fs = ds;
  entrada_tss->gs = ds;
  entrada_tss->ss = ds;

  entrada_tss->eax = 0x00000000;
  entrada_tss->ecx = 0x00000000;
  entrada_tss->edx = 0x00000000;
  entrada_tss->ebx = 0x00000000;

  entrada_tss->esi = 0x00000000;
  entrada_tss->edi = 0x00000000;

  entrada_tss->eip = eip;

  entrada_tss->eflags = eeflags;
  entrada_tss->iomap = 0xFFFF;

}

void cargar_tss_en_gdt(tss* entrada_tss,
                       gdt_entry* entrada_gdt) {

}
