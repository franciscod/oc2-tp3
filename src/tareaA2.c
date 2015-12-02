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

#define fori(i,k) for(i=0;i<k;i++)

#define mover(i,k,d) fori(i,k){syscall_moverse(d);wait(20);}

#define cavar(i,k) fori(i, k){syscall_cavar();}

void task(int x_target, int y_target) {
	int i, j;
    /* Perro "EL JARKO" */
	mover(i, 9, ABA);
	mover(i, 8, DER);

	cavar(i, 10);

	mover(i, 10, ARR);
	mover(i, 8, IZQ);
	
	fori(j, 10) {
		mover(i, 1, IZQ);
		mover(i, 1, ABA);
		mover(i, 1, DER);
		mover(i, 1, ARR);
	}
	mover(i, 40, ABA);

  while(1) { __asm __volatile("mov $4025344291, %%eax":::"eax"); }
}


void wait(int pseudoms)
{
	int count;
	for (count = 0; count < pseudoms * 1000; count++) {}
}
