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
}


int sched_buscar_indice_tarea(uint gdt_index) {
    return MAX_CANT_TAREAS_VIVAS;
}


int sched_buscar_tarea_libre()
{
//<<<REMOVE END>>>
    // ee_printf("gdt_i0 %x",scheduler.tasks[0].gdt_index);
    // ee_printf("gdt_i1 %x",scheduler.tasks[1].gdt_index);
    // ee_printf("gdt_i2 %x",scheduler.tasks[2].gdt_index);
	int i = sched_buscar_indice_tarea(GDT_IDX_NULL_DESC);
    
	ASSERT_OR_ERROR(i != MAX_CANT_TAREAS_VIVAS, "no hay tareas disponibles!?");

    return i;
//<<<REMOVE END>>>
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
    return MAX_CANT_TAREAS_VIVAS;
}


ushort sched_atender_tick()
{
    return scheduler.tasks[scheduler.current].gdt_index;
}


