
/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "defines.h"
#include "idt.h"
#include "isr.h"

#include "tss.h"

idt_entry idt[255] = { };

idt_descriptor IDT_DESC = {
    sizeof(idt) - 1,
    (unsigned int) &idt
};


// descomentar para usar, seteando segsel y attr segun corresponda
#define SEGMENT_PRESENT           0b1000000000000000
//                                0bDD00000000000000  // aca va la dpl, lo pone la macro IDT_ENTRY
#define INTERRUPT_D_SIZE_32       0b0000100000000000
#define INTERRUPT_MAGIC_BITS      0b0000011000000000

#define INTERRUPT_ATTR            SEGMENT_PRESENT | INTERRUPT_D_SIZE_32 | INTERRUPT_MAGIC_BITS


// FIXME quedo hardcodeada la cfg para interrupts, arreglar para traps/tasks
#define IDT_ENTRY(numero, dpl)                                                                                   \
    idt[numero].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## numero) & (unsigned int) 0xFFFF);        \
    idt[numero].segsel = (unsigned short) GDT_OFF_CODE_0;                                                                  \
    idt[numero].attr = (unsigned short) INTERRUPT_ATTR | (((unsigned short)(dpl & 0x3)) << 13);                          \
    idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 & (unsigned int) 0xFFFF);


void idt_inicializar() {
    // Excepciones

    IDT_ENTRY(0, 0)
    IDT_ENTRY(1, 0)
    IDT_ENTRY(2, 0)
    IDT_ENTRY(3, 0)
    IDT_ENTRY(4, 0)
    IDT_ENTRY(5, 0)
    IDT_ENTRY(6, 0)
    IDT_ENTRY(7, 0)
    IDT_ENTRY(8, 0)
    IDT_ENTRY(9, 0)
    IDT_ENTRY(10, 0)
    IDT_ENTRY(11, 0)
    IDT_ENTRY(12, 0)
    IDT_ENTRY(13, 0)
    IDT_ENTRY(14, 0)
    IDT_ENTRY(15, 0)
    IDT_ENTRY(16, 0)
    IDT_ENTRY(17, 0)
    IDT_ENTRY(18, 0)
    IDT_ENTRY(19, 0)

    IDT_ENTRY(32, 0) // clock
    IDT_ENTRY(33, 0) // teclado

    IDT_ENTRY(70, 3) // syscall

}
