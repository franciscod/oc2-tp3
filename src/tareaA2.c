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

#define fori(i,k) for(int i=0;i<k;i++)

#define mover(k,d) fori(i,k){syscall_moverse(d);wait(20);}

#define cavar(k) fori(i, k){syscall_cavar();}

void task(int x_target, int y_target) {
    /* Perro "EL JARKO" */
	mover(9, ABA);
	mover(8, DER);

	cavar(10);

	mover(10, ARR);
	mover(8, IZQ);
	
	fori(i, 10) {
		mover(1, IZQ);
		mover(1, ABA);
		mover(1, DER);
		mover(1, ARR);
	}
	mover(40, ABA);

  while(1) { __asm __volatile("mov $4025344291, %%eax":::"eax"); }
}


void wait(int pseudoms)
{
	int count;
	for (count = 0; count < pseudoms * 1000; count++) {}
}
