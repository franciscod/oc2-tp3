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
#define ATTRS_MASK 		0xFFF

#define CLEAR_ATTRS_MASK	0xFFFFF000

// figura 3
#define AREA_LIBRE_START	0x100000
#define AREA_LIBRE_END		0x3FFFFF

#define PAGE_DIRECTORY		0x027000

/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */

uint prox_pag_libre = AREA_LIBRE_START;

uint mmu_proxima_pagina_fisica_libre() {
	uint ret = prox_pag_libre;
	prox_pag_libre += 0x00001000;
	return ret;
}

void mmu_inicializar_pagina(uint * pagina) {
	for (uint i = 0 ; i < PAGE_SIZE / 4; i+=4) {
		pagina[i] = 0x00000000; // uint, escribe 4 bytes
	}
}


void mmu_mapear_pagina (uint virtual, uint cr3, uint fisica, uint attrs){

	// virtual 	: AAAAAAAAAA     BBBBBBBBBB    000000000000
	//         	   directory       table          offset

	// fisica  	: DDDDDDDDDD     EEEEEEEEEE    000000000000
	// cr3 	   	: XXXXXXXXXX     XXXXXXXXXX    000000000000
	//            32        22 21          12 11   offset 0

	uint *pd_pointer = (uint*) cr3;

	uint pde_index = virtual >> 22;                // AAAAAAAAAA
	uint pte_index = (virtual & 0x003FF000) >> 12; // BBBBBBBBBB

	uint pde = pd_pointer[pde_index];

	char directoryEntryPresent = (pde & 0x1);
	char attributesUserBit = (attrs & 0x4);

	uint *pt_pointer;

	if (!directoryEntryPresent) {
		uint dir_nueva_tabla = mmu_proxima_pagina_fisica_libre();	// Es multiplo de 4K
		mmu_inicializar_pagina((uint*) dir_nueva_tabla);
		pde = dir_nueva_tabla | (attrs & ATTRS_MASK);
		pd_pointer[pde_index] = pde; // escribe la entrada en el directorio
	} else {
		if (!attributesUserBit) { // si existia y me piden atributos de root
			pde &= CLEAR_ATTRS_MASK;
			pde |= attrs;
			pd_pointer[pde_index] = pde; // sobreescribo los attributos del directorio
		}
	}

	pt_pointer = (uint*) (pde & CLEAR_ATTRS_MASK);

	uint pte = fisica | (attrs & ATTRS_MASK);
	pt_pointer[pte_index]= pte;
}

// FIXME para que devuelve uint?
uint mmu_inicializar_dir_kernel() {
	// inicializa el directorio
	mmu_inicializar_pagina((uint*)PAGE_DIRECTORY);

	// identity mapping sobre
	for (uint i = 0; i < 1024; i++) {
		uint addr = i<<12;
		mmu_mapear_pagina(addr, PAGE_DIRECTORY, addr, ATTRS_TABLA);
	}

	return 0;
}
