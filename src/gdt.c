/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#include "gdt.h"




/* Definicion de la GDT */
/* -------------------------------------------------------------------------- */

gdt_entry gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */

    [GDT_IDX_NULL_DESC ... GDT_IDX_NULL_DESC_LAST] = (gdt_entry) {
        (unsigned short)    0x0000,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x00,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x00,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },



    [GDT_IDX_CODE_0] = (gdt_entry) {
        (unsigned short)    LIMIT_500M_15_00,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     TYPE_CODE_XR,   /* type         */
        (unsigned char)     S_CODE_OR_DATA, /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     PRESENT,        /* p            */
        (unsigned char)     LIMIT_500M_19_16, /* limit[16:19] */
        (unsigned char)     AVL,  /* avl          */
        (unsigned char)     L_32,           /* l            */
        (unsigned char)     DB_32,          /* db           */
        (unsigned char)     GRANULARITY_4K, /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    [GDT_IDX_CODE_3] = (gdt_entry) {
        (unsigned short)    LIMIT_500M_15_00,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     TYPE_CODE_XR,   /* type         */
        (unsigned char)     S_CODE_OR_DATA, /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     PRESENT,        /* p            */
        (unsigned char)     LIMIT_500M_19_16, /* limit[16:19] */
        (unsigned char)     AVL,  /* avl          */
        (unsigned char)     L_32,           /* l            */
        (unsigned char)     DB_32,          /* db           */
        (unsigned char)     GRANULARITY_4K, /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    [GDT_IDX_DATA_0] = (gdt_entry) {
        (unsigned short)    LIMIT_500M_15_00,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     TYPE_DATA_RW,   /* type         */
        (unsigned char)     S_CODE_OR_DATA, /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     PRESENT,        /* p            */
        (unsigned char)     LIMIT_500M_19_16, /* limit[16:19] */
        (unsigned char)     AVL,  /* avl          */
        (unsigned char)     L_32,           /* l            */
        (unsigned char)     DB_32,          /* db           */
        (unsigned char)     GRANULARITY_4K, /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    [GDT_IDX_DATA_3] = (gdt_entry) {
        (unsigned short)    LIMIT_500M_15_00,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     TYPE_DATA_RW,   /* type         */
        (unsigned char)     S_CODE_OR_DATA, /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     PRESENT,        /* p            */
        (unsigned char)     LIMIT_500M_19_16, /* limit[16:19] */
        (unsigned char)     AVL,  /* avl          */
        (unsigned char)     L_32,           /* l            */
        (unsigned char)     DB_32,          /* db           */
        (unsigned char)     GRANULARITY_4K, /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },


    [GDT_IDX_UI_KERNEL] = (gdt_entry) {
        (unsigned short)    LIMIT_UI_15_00,         /* limit[0:15]  */
        (unsigned short)    BASE_UI_15_00,         /* base[0:15]   */
        (unsigned char)     BASE_UI_23_16,           /* base[23:16]  */
        (unsigned char)     TYPE_DATA_RW,   /* type         */
        (unsigned char)     S_CODE_OR_DATA, /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     PRESENT,        /* p            */
        (unsigned char)     LIMIT_UI_19_16, /* limit[16:19] */
        (unsigned char)     AVL,  /* avl          */
        (unsigned char)     L_32,           /* l            */
        (unsigned char)     DB_32,          /* db           */
        (unsigned char)     GRANULARITY_4K, /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

};

gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (unsigned int) &gdt
};
