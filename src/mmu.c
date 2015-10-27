/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"

/* Atributos paginas */
/* -------------------------------------------------------------------------- */

//						IGN  PS IGN A PCD PWT U/S R/W P
//						0000 0  0   0 0   0   0   0   1
#define ATTRS_TABLA 	0x001

//						IGN G PAT D A PCD PWT U/S R/W P
//						000 0 0   0 0 0   0   0   0   1
#define ATTRS_PAGINA	0x001

#define CLEAR_ATTRS 0xFFFFF000


/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */

uint prox_pag_libre = 0x100000;

void mmu_mapear_pagina (uint virtual, uint cr3, uint fisica, uint attrs){
	// virtual 	: AAAAAAAAAA BBBBBBBBBB CCCCCCCCCCCC
	// fisica  	: DDDDDDDDDD EEEEEEEEEE CCCCCCCCCCCC
	// cr3 	   	: XXXXXXXXXX XXXXXXXXXX 000000000000

	uint *pd_pointer = (uint*) cr3;

	uint pde_index = virtual >> 22;
	uint pte_index = (virtual & 0x003FF000) >> 12;

	uint pde = *(pd_pointer + pde_index * 4);
	char isTablePresent = (pde % 2) == 1;

	uint *pt_pointer;
	if(isTablePresent){
		pt_pointer = (uint*) (pde & CLEAR_ATTRS);
	}
	else{
		uint dir_nueva_tabla = mmu_proxima_pagina_fisica_libre();	// Es multiplo de 4K
		pde = dir_nueva_tabla + ATTRS_TABLA;
		*(pd_pointer + pde_index * 4) = pde;
		pt_pointer = (uint*) (pde & CLEAR_ATTRS);
	}

	uint pte = fisica + ATTRS_PAGINA;
	*(pt_pointer + pte_index * 4) = pte;
}

uint mmu_proxima_pagina_fisica_libre(){
	prox_pag_libre += 0x00001000;
	return(prox_pag_libre - 0x00001000);
}