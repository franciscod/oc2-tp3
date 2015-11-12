/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "mmu.h"
#include "tss.h"
#include "screen.h"

#include <stdarg.h>


int escondites[ESCONDITES_CANTIDAD][3] = { // TRIPLAS DE LA FORMA (X, Y, HUESOS)
                                        {76,  25, 50}, {12, 15, 50}, {9, 10, 100}, {47, 21, 100} ,
                                        {34,  11, 50}, {75, 38, 50}, {40, 21, 100}, {72, 17, 100}
                                    };

jugador_t jugadorA;
jugador_t jugadorB;

extern unsigned char debugging_mode;
int ultimo_cambio = MAX_SIN_CAMBIOS;

void ASSERT_OR_ERROR(uint value, char* error_msg)
{
	if (!value) {
		print(error_msg, 5, 5, C_BG_LIGHT_GREY | C_FG_BLACK);
		breakpoint(); //assert or error
	}
}

void* error()
{
	__asm__ ("int3");
	return 0;
}

uint game_xy2lineal (uint x, uint y) {
	return (y * MAPA_ANCHO + x);
}

uint game_es_posicion_valida(int x, int y) {
	return (x >= 0 && y >= 0 && x < MAPA_ANCHO && y < MAPA_ALTO);
}


void game_inicializar()
{
	game_jugador_inicializar(&jugadorA);
	game_jugador_inicializar(&jugadorB);

    screen_pintar_puntajes();
}


// devuelve la cantidad de huesos que hay en la posición pasada como parametro
uint game_huesos_en_posicion(uint x, uint y)
{
	int i;
	for (i = 0; i < ESCONDITES_CANTIDAD; i++)
	{
		if (escondites[i][0] == x && escondites[i][1] == y)
			return escondites[i][2];
	}
	return 0;
}




// devuelve algun perro que esté en la posicion pasada (hay max 2, uno por jugador)
perro_t* game_perro_en_posicion(uint x, uint y)
{
	int i;
	for (i = 0; i < MAX_CANT_PERROS_VIVOS; i++)
	{
		if (!jugadorA.perros[i].libre && jugadorA.perros[i].x == x && jugadorA.perros[i].y == y)
			return &jugadorA.perros[i];
		if (!jugadorB.perros[i].libre && jugadorB.perros[i].x == x && jugadorB.perros[i].y == y)
			return &jugadorB.perros[i];
	}
	return NULL;
}



void game_terminar_si_es_hora()
{

	int termina = 0;
	int quedanhuesos = 0;

	int ancho_barrita = (ultimo_cambio+1)* 40/MAX_SIN_CAMBIOS ;
	screen_pintar_rect(' ', 0x00, 0, 38, 1, 40-ancho_barrita);
	screen_pintar_rect('#', 0x03, 0, 38 + 40-ancho_barrita, 1, ancho_barrita);
	ultimo_cambio--;


    screen_pintar_rect(debugging_mode ? 'd' : ' ', C_BG_BLACK | C_FG_WHITE, 0, 36, 1, 1);

	if (ultimo_cambio <= 0) {
		termina |= 1;
	}

    for (int i = 0; i < ESCONDITES_CANTIDAD; i++)
	{
		if (escondites[i][2] > 0) {
			quedanhuesos = 1;
			break;
		};
	}

	if (!quedanhuesos) termina |= 1;


	if (!termina) return;

	if (jugadorA.puntos > jugadorB.puntos)
		screen_stop_game_show_winner(&jugadorA);
	else
		screen_stop_game_show_winner(&jugadorB); // empates -> gana B


}
