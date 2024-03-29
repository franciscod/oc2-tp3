/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Tipos basicos */
/* -------------------------------------------------------------------------- */
#define NULL                    0
#define TRUE                    0x00000001
#define FALSE                   0x00000000
#define ERROR                   1

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;

/* Constantes basicas */
/* -------------------------------------------------------------------------- */
#define PAGE_SIZE               0x00001000
#define TASK_SIZE               4096

#define BOOTSECTOR              0x00001000 /* direccion fisica de comienzo del bootsector (copiado) */
#define KERNEL                  0x00001200 /* direccion fisica de comienzo del kernel */
#define TAREA_IDLE              0x00016000
#define STACK_BASE              0x00027000 /* direccion fisica de la base de la pila del kernel */


#define BASE_UI                 0xb8000
#define SIZE_UI                 0x1f40
#define LIMIT_UI                (BASE_UI + SIZE_UI - 1)
#define LIMIT_500M              (0x1f400 - 1) // 500 * 1024 * 1024 / (4 * 1024) por granularidad

#define BASE_UI_15_00           (BASE_UI & 0x00FFFF)
#define BASE_UI_23_16           ((BASE_UI & 0xFF0000) >> 16)

#define LIMIT_UI_15_00          (LIMIT_UI & 0x0FFFF)
#define LIMIT_UI_19_16          ((LIMIT_UI & 0xF0000) >> 16)

#define LIMIT_500M_15_00        (LIMIT_500M & 0x0FFFF)
#define LIMIT_500M_19_16        ((LIMIT_500M & 0xF0000) >> 16)

/* Indices en la gdt */
/* -------------------------------------------------------------------------- */

#define GDT_COUNT 32

// son 8 entradas nulas, 1 de intel + 7 utilizadas segun el tp
#define GDT_IDX_NULL_DESC           0
#define GDT_IDX_NULL_DESC_LAST      7

#define GDT_IDX_CODE_0              8
#define GDT_IDX_CODE_3              9
#define GDT_IDX_DATA_0              10
#define GDT_IDX_DATA_3              11
#define GDT_IDX_UI_KERNEL           12
#define GDT_IDX_TSS_INICIAL         13
#define GDT_IDX_TSS_IDLE            14
#define GDT_IDX_TSS_PERRO_START     15
#define GDT_IDX_TSS_PERRO_END       (GDT_IDX_TSS_PERRO_START + MAX_CANT_PERROS_VIVOS * 2 - 1)

#define MAX_CANT_PERROS_VIVOS 8
/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC      << 3)
#define GDT_OFF_CODE_0              (GDT_IDX_CODE_0         << 3)

/* Selectores de segmentos */
/* -------------------------------------------------------------------------- */

#define GRANULARITY_4K              1
#define PRESENT                     1
#define S_CODE_OR_DATA              1
#define AVL                         0
#define L_32                        0
#define DB_32                       1
#define TYPE_DATA_RW                0b0010
#define TYPE_CODE_XR                0b1010


#endif  /* !__DEFINES_H__ */
