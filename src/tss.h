/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#ifndef __TSS_H__
#define __TSS_H__

#include "defines.h"
#include "i386.h"
#include "gdt.h"
#include "game.h"


typedef struct str_tss {
    unsigned short  ptl;
    unsigned short  unused0;
    unsigned int    esp0;
    unsigned short  ss0;
    unsigned short  unused1;
    unsigned int    esp1;
    unsigned short  ss1;
    unsigned short  unused2;
    unsigned int    esp2;
    unsigned short  ss2;
    unsigned short  unused3;
    unsigned int    cr3;
    unsigned int    eip;
    unsigned int    eflags;
    unsigned int    eax;
    unsigned int    ecx;
    unsigned int    edx;
    unsigned int    ebx;
    unsigned int    esp;
    unsigned int    ebp;
    unsigned int    esi;
    unsigned int    edi;
    unsigned short  es;
    unsigned short  unused4;
    unsigned short  cs;
    unsigned short  unused5;
    unsigned short  ss;
    unsigned short  unused6;
    unsigned short  ds;
    unsigned short  unused7;
    unsigned short  fs;
    unsigned short  unused8;
    unsigned short  gs;
    unsigned short  unused9;
    unsigned short  ldt;
    unsigned short  unused10;
    unsigned short  dtrap;
    unsigned short  iomap; // 0xFFFF cuando no se usa
} __attribute__((__packed__, aligned (8))) tss;

void tss_inicializar();
void completar_tss(tss* entrada_tss, unsigned short cs, unsigned short ds, uint esp, uint eip, uint eeflags, uint cr3, unsigned short ss0, uint esp0); // algo mas?
void cargar_tss_en_gdt(tss* entrada_tss, gdt_entry* entrada_gdt);
void completar_tss_tarea(tss* entrada_tss, perro_t *perro, int index_jugador, int index_tipo);
#endif  /* !__TSS_H__ */
