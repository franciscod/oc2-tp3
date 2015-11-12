/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "syscall.h"


void task(int x, int y) {
    // Perro "CORRO Y ME CUELGO"

    int i = 20;
    while(i--){
        syscall_moverse(ARR);
    }
    while(1) { __asm __volatile("mov $2, %%eax":::"eax"); }
}
