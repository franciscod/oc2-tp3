/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "syscall.h"

void wait(int pseudosecs);

void task(int x_target, int y_target) {
	// Perro "BAILE SUICIDA"
	syscall_moverse(IZQ);wait(100);
	syscall_moverse(IZQ);wait(100);
	syscall_moverse(IZQ);wait(100);
	syscall_moverse(IZQ);wait(100);
	syscall_moverse(IZQ);wait(100);
	syscall_moverse(IZQ);wait(100);

	int d = 400;
	while (d>2) {
		syscall_moverse(ARR); wait(d);
		syscall_moverse(ABA); wait(d);
		d= d / 10 * 4;
	}

	*((int*) 1) = 123; // BOOM!

	// esto no va a correr nunca porque lo anterior me desaloja
	syscall_moverse(IZQ);wait(100);
	syscall_moverse(IZQ);wait(100);
	syscall_moverse(IZQ);wait(100);
	while(1) { __asm __volatile("mov $2, %%eax":::"eax"); }
}


void wait(int pseudoms)
{
	int count;
	for (count = 0; count < pseudoms * 1000; count++) {}
}
