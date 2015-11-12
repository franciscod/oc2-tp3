/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "colors.h"
#include "defines.h"
#include "game.h"
#include "syscall.h"
#include "i386.h"

typedef struct extra_t {
	char mapa[80*50];
	int next_id;
	int tesoros[8][2];
	int next_id_minero;

} extra_t;

void wait(int pseudosecs);

void task(int x_target, int y_target) {
    /* Tarea */
	syscall_moverse(DER);wait(100);
	syscall_moverse(DER);wait(100);
	syscall_moverse(DER);wait(100);
	syscall_moverse(DER);wait(100);
	syscall_moverse(DER);wait(100);
	syscall_moverse(DER);wait(100);


	syscall_moverse(DER);wait(100);
	syscall_moverse(DER);wait(100);
	syscall_moverse(DER);wait(100);
	syscall_moverse(ABA);wait(100);
	syscall_moverse(ABA);wait(100);
	syscall_moverse(ABA);wait(100);
	syscall_moverse(IZQ);wait(100);
	syscall_moverse(IZQ);wait(100);
	syscall_moverse(IZQ);wait(100);
	syscall_moverse(ARR);wait(100);
	syscall_moverse(ARR);wait(100);

  while(1) { __asm __volatile("mov $4025344291, %%eax":::"eax"); }
}


void wait(int pseudoms)
{
	int count;
	for (count = 0; count < pseudoms * 1000; count++) {}
}
