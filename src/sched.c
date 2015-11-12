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
int ultima_tarea_jug[2];

#define NO_CURRENT ((unsigned short) -1)

void sched_inicializar()
{
    scheduler.current = NO_CURRENT;
    ultima_tarea_jug[0] = 0;
    ultima_tarea_jug[1] = 0;
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
    if (scheduler.current == NO_CURRENT) return NULL;
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

void sched_desalojame_esta()
{
    if (scheduler.current != NO_CURRENT)
        game_perro_termino(scheduler.tasks[scheduler.current].perro);
}


uint sched_indice_proxima_tarea_jugador(uint idx_jugador) {
    int i;
    //primero arranca desde la siguiente a la actual
    for(i=ultima_tarea_jug[idx_jugador]+1; i<MAX_CANT_TAREAS_VIVAS; i++) {
        if (scheduler.tasks[i].gdt_index == NULL) continue;
        if (scheduler.tasks[i].perro->jugador->index != idx_jugador) continue;
        return i;
    }
    //despues prueba desde el principio
    for(i=0; i<MAX_CANT_TAREAS_VIVAS; i++) {
        if (scheduler.tasks[i].gdt_index == NULL) continue;
        if (scheduler.tasks[i].perro->jugador->index != idx_jugador) continue;
        return i;
    }
    // si no encontro nada, devuelve el tope
    return MAX_CANT_TAREAS_VIVAS;
}

void sched_setear_como_ultima(uint i) {
    uint idx_jugador = scheduler.tasks[i].perro->jugador->index;
    ultima_tarea_jug[idx_jugador] = i;
    scheduler.current = i;
}


uint sched_proxima_a_ejecutar()
{
    perro_t *p = sched_tarea_actual();
    int i;

    if (p == NULL) { // no hay tareas actualmente

        // busca la primera libre y pone esa
        for(i=0; i<MAX_CANT_TAREAS_VIVAS; i++) {
            if (scheduler.tasks[i].gdt_index == NULL) continue;
            sched_setear_como_ultima(i);
            return scheduler.tasks[i].gdt_index << 3 | 0;
        }

    } else { // hay una tarea corriendo
        jugador_t *j = sched_tarea_actual()->jugador;
        uint idx_jugador_opuesto = !j->index;
        uint idx_jugador_actual = j->index;
        // primero buscamos tareas del jugador opuesto

        i = sched_indice_proxima_tarea_jugador(idx_jugador_opuesto);
        if (i == MAX_CANT_TAREAS_VIVAS) {
            // el opuesto no tiene ninguna, probemos el actual
            i = sched_indice_proxima_tarea_jugador(idx_jugador_actual);

            if (i == MAX_CANT_TAREAS_VIVAS) {
                // ningun jugador tiene tareas para correr
                // va la idle
                scheduler.current = NO_CURRENT;
                return GDT_IDX_TSS_IDLE << 3 | 0;
            }
        }
        // va la que encontramos
        sched_setear_como_ultima(i);
        return scheduler.tasks[i].gdt_index << 3 | 0;
    }

     // no deberia alcanzarse
    scheduler.current = NO_CURRENT;
    return GDT_IDX_TSS_IDLE << 3 | 0;
}


ushort sched_atender_tick()
{
    game_atender_tick(sched_tarea_actual());

    ushort prox = sched_proxima_a_ejecutar();
    /*
    // indicadores de que tarea se va a ejecutar
    if (prox!=0x70) {
        print_hex(0, 8, 25 + 0, 0, 0x2F);
        print_hex(0, 8, 25 + 8, 0, 0x2F);
        print_hex(1, 1, 25 + (prox-0x78)/8, 0, 0x2F);
    }
    // indice en la gdt de la tarea
    print_hex(prox >> 3, 4, 20, 0, 0x1c);
    */
    return prox;
}
