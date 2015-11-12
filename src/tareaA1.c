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


void ir_hacia_desde(char x_dest, char y_dest, int *x_actual, int *y_actual);
void actualizar(int *x, int *y, int direccion);


void task(int x_origen, int y_origen) {
    /* Tarea */
    //breakpoint();
	//while(1);


	int x_actual = x_origen;
	int y_actual = y_origen;

	//breakpoint();

	while (TRUE)
	{
		int direccion = syscall_olfatear();
		if (direccion == AQUI)
			break;

		actualizar(&x_actual, &y_actual, direccion);
		syscall_moverse(direccion);
	}
	while (syscall_cavar() != 0)
	{}


	ir_hacia_desde(0, y_actual, &x_actual, &y_actual);
	ir_hacia_desde(0, 0, &x_actual, &y_actual);
	ir_hacia_desde(x_origen, 0, &x_actual, &y_actual);
	ir_hacia_desde(x_origen, y_origen, &x_actual, &y_actual);

    while(1) { __asm __volatile("mov $2, %%eax":::"eax"); }
}

void actualizar(int *x, int *y, int direccion)
{
	switch (direccion)
	{
		case ARR: (*y)--; break;
		case ABA: (*y)++; break;
		case IZQ: (*x)--; break;
		case DER: (*x)++; break;
	}
}

void ir_hacia_desde(char x_dest, char y_dest, int * x_actual, int * y_actual)
{
	char dir_h = x_dest > *x_actual ? DER : IZQ;
	char dir_v = y_dest > *y_actual ? ABA : ARR;

	int dist_x = x_dest > *x_actual ? x_dest - *x_actual : *x_actual - x_dest;
	int dist_y = y_dest > *y_actual ? y_dest - *y_actual : *y_actual - y_dest;

	int i = 0;
	for (i = dist_x; i>0; i--) {
		actualizar(x_actual, y_actual, (int)dir_h);
		syscall_moverse(dir_h);
	}

	for (i = dist_y; i>0; i--) {
		actualizar(x_actual, y_actual, (int)dir_v);
		syscall_moverse(dir_v);
	}


}
