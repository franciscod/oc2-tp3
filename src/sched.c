/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
definicion de funciones del scheduler
*/

#include "defines.h"
#include "sched.h"
#include "i386.h"
#include "screen.h"

sched_t scheduler;

void sched_inicializar()
{
    scheduler.current = NULL;
    for (int i = 0; i<MAX_CANT_TAREAS_VIVAS; i++) {
        sched_task_t *t = &scheduler.tasks[i];
        t->gdt_index = NULL;
        t->perro = NULL;
    }
}


int sched_buscar_indice_tarea(uint gdt_index) {
    int i;
    for (i = 0; i < MAX_CANT_TAREAS_VIVAS; i++) {
        if (scheduler.tasks[i].gdt_index == gdt_index) break;
    }
    return i;
}


int sched_buscar_tarea_libre()
{
    return sched_buscar_indice_tarea(NULL);
}

perro_t* sched_tarea_actual()
{
    return scheduler.tasks[scheduler.current].perro;
}

void sched_agregar_tarea(perro_t *perro)
{
    sched_task_t *t = &scheduler.tasks[sched_buscar_tarea_libre()];
    t->perro = perro;
    t->gdt_index = perro->id;
}

void sched_remover_tarea(unsigned int gdt_index)
{
    int i = sched_buscar_indice_tarea(gdt_index);
    scheduler.tasks[i].gdt_index = NULL;
    scheduler.tasks[i].perro = NULL;
}


uint sched_proxima_a_ejecutar()
{
    int i = scheduler.current + 1;
    jugador_t *j = sched_tarea_actual()->jugador;

    // busca una del jugador opuesto
    while (i != scheduler.current) {
        if (scheduler.tasks[i].gdt_index != NULL) {
            if (scheduler.tasks[i].perro->jugador != j) {
                return scheduler.tasks[i].gdt_index << 3 | 3;
            }
        }
        i++;
        i %= MAX_CANT_TAREAS_VIVAS;
    }

    // busca una del mismo jugador
    while (i != scheduler.current) {
        if (scheduler.tasks[i].gdt_index != NULL) {
            if (scheduler.tasks[i].perro->jugador == j) {
                return scheduler.tasks[i].gdt_index << 3 | 3;
            }
        }

        i++;
        i %= MAX_CANT_TAREAS_VIVAS;
    }


    return GDT_IDX_TSS_IDLE << 3 | 0; // esto se alcanzaria si no hay tareas
}


ushort sched_atender_tick()
{
    game_atender_tick(sched_tarea_actual());

    return sched_proxima_a_ejecutar();
}
