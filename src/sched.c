/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
definicion de funciones del scheduler
*/

#include "sched.h"
#include "i386.h"
#include "screen.h"

sched_t scheduler;

void sched_inicializar()
{
    // TODO inicializa estructuras del sched
}


int sched_buscar_indice_tarea(uint gdt_index) {
    return MAX_CANT_TAREAS_VIVAS;
}


int sched_buscar_tarea_libre()
{
	int i = 0;

    return i;
}



perro_t* sched_tarea_actual()
{
    return scheduler.tasks[scheduler.current].perro;
}

void sched_agregar_tarea(perro_t *perro)
{
}

void sched_remover_tarea(unsigned int gdt_index)
{
}


uint sched_proxima_a_ejecutar()
{
    /* TODO
    Devuelve el ı́ndice de la próxima tarea a ser ejecutada.
    Construir la rutina de forma que devuelva una tarea de cada jugador por
    vez según se explica en la sección 4.5.
    */

    return MAX_CANT_TAREAS_VIVAS;
}


ushort sched_atender_tick()
{

    /* TODO
    Crear una función sched_atender_tick() que llame a game_atender_tick() pasando el
    numero de tarea actual y luego devuelva el indice en la gdt al cual se deberá saltar. Re-
    emplazar el llamado a game_atender_tick por uno a sched_atender_tick en el handler
    de la interrupción de reloj.
    */

    return scheduler.tasks[scheduler.current].gdt_index;
}
