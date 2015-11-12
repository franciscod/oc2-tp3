
#include "game.h"
#include "mmu.h"
#include "tss.h"
#include "screen.h"

/// ************** RUTINAS DE ATENCION DE INTERRUPCIONES DE ************** ///
///                              ALTO NIVEL                                ///
///                  (deben ser llamadas desde isr.asm)                    ///
/// ********************************************************************** ///


// gasta un rato en un ciclo infinito, util para hacer pausas y debuguear
void wait(int pseudosecs)
{
	int count;
	for (count = 0; count < pseudosecs * 1000000; count++) {}
}


uint game_syscall_manejar(uint syscall, uint param1)
{
	/* TODO que implemente los servicios según se indica en la sección 4.4.

	0x1 - Moverse. Recibe en ECX el código de dirección, que puede ser:
		 4, arriba
		 7, abajo
		10, derecha
		13, izquierda.
	Deberá copiar el código de la tarea a la nueva ubicación,
	siempre y cuando sea posible: debe chequearse que no se salga del mapa y que no haya
	una tarea del mismo jugador en el lugar destino. Deberá mapear la nueva posición del
	mapa a esa tarea.

	0x2 - Cavar. En caso de estar sobre un escondite aumenta en 1 la cantidad de huesos del
	perro y disminuye en 1 la cantidad de huesos del escondite. La cantidad de huesos en
	cada escondite están dados por una variable global llamada huesos en game.h. El perro
	puede llevar hasta 10 huesos, luego de eso el syscall debe ignorar el pedido.

	0x3 - Olfatear. El tercer caso del syscall le permite a una tarea perro conocer la dirección
	del hueso más cercano (es decir, izquierda, derecha, arriba o abajo).

	0x4 - Recibir orden. Esta función del syscall le permite a una tarea perro conocer la última
	orden emitida por el jugador. Cada raza de perro interpretará libremente la orden. El
	sistema debe devolver un par (x,y) y un numero indicando el numero de orden (1, 2 o
	3). Para poder devolver 3 valores en uno, deberá codificarlos de la siguiente manera:
	id\_orden << 16 | y << 8 | x.

	Por último, es fundamental tener en cuenta que una vez llamado el servicio, el scheduler
	se encargará de desalojar a la tarea que lo llamó para dar paso a la próxima tarea. Este
	mecanismo será detallado mas adelante.
*/
    // ~ completar llamando a las funciones que haga falta ~
    return 0;
}

// ~~~ debe atender la interrupción de reloj para actualizar la pantalla y terminar si es hora,
// ~~~ recibe el perro que está corriendo actualmente
void game_atender_tick(perro_t *perro)
{
	/*T
	    ;TODO Modificar el código necesario para que se realice el intercambio de tareas por cada ciclo de
	    ; reloj. El intercambio se realizará según indique la función sched_proxima_a_ejecutar().

	    ;TODO
	    ;Reemplazar el llamado a game_atender_tick por uno a sched_atender_tick en el handler
	    ;de la interrupción de reloj.

	    ;call sched_atender_tick
	    ;push eax
	    ;call game_atender_tick
	    ;add esr, 4*/

	screen_actualizar_reloj_global();
}


#define KB_w        0x11 // 0x91
#define KB_s        0x1f // 0x9f
#define KB_a        0x1e // 0x9e
#define KB_d        0x20 // 0xa0

#define KB_i        0x17 // 0x97
#define KB_k        0x25 // 0xa5
#define KB_j        0x24 // 0xa4
#define KB_l        0x26 // 0xa6


#define KB_q        0x10 // 0x90
#define KB_e        0x12 // 0x92

#define KB_u        0x16 // 0x96
#define KB_o        0x18 // 0x98

#define KB_z        0x2C // ?
#define KB_x        0x2D // ?
#define KB_c        0x2E // ?

#define KB_b        0x30 // ?
#define KB_n        0x31 // ?
#define KB_m        0x32 // ?

#define KB_shiftL   0x2a // 0xaa
#define KB_shiftR   0x36 // 0xb6


// ~~~ debe atender la interrupción de teclado, se le pasa la tecla presionada
void game_atender_teclado(unsigned char tecla)
{

	if (tecla & 0x80) return;
	print_hex(tecla, 2, 78, 0, 0x17);

	switch (tecla)
	{
		// ~~~ completar ~~~
		case KB_q: game_jugador_lanzar_perro(&jugadorA, TIPO_1, 0, 0); break;

		case KB_w: game_jugador_moverse(&jugadorA, 0,  -1); break;
		case KB_s: game_jugador_moverse(&jugadorA, 0,  1); break;
		case KB_a: game_jugador_moverse(&jugadorA, -1,  0); break;
		case KB_d: game_jugador_moverse(&jugadorA, 1,  0); break;

		case KB_i: game_jugador_moverse(&jugadorB, 0,  -1); break;
		case KB_k: game_jugador_moverse(&jugadorB, 0,  1); break;
		case KB_j: game_jugador_moverse(&jugadorB, -1,  0); break;
		case KB_l: game_jugador_moverse(&jugadorB, 1,  0); break;

		case KB_z: game_jugador_dar_orden(&jugadorA, 1); break;
		case KB_x: game_jugador_dar_orden(&jugadorA, 2); break;
		case KB_c: game_jugador_dar_orden(&jugadorA, 3); break;

		case KB_b: game_jugador_dar_orden(&jugadorB, 1); break;
		case KB_n: game_jugador_dar_orden(&jugadorB, 2); break;
		case KB_m: game_jugador_dar_orden(&jugadorB, 3); break;

		default: break;
		// TODO: debuguear
		//Implementar el mecanismo de debugging explicado en la sección 4.8 que indicará en pan-
		//talla la razón del desalojo de una tarea.
	}

}
