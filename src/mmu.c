/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"
#include "screen.h"

/* Atributos paginas */
/* -------------------------------------------------------------------------- */

//						IGN  PS IGN A PCD PWT U/S R/W P
//						0000 0  0   0 0   0   0   0   1
#define ATTRS_TABLA 	0x001

//						IGN G PAT D A PCD PWT U/S R/W P
//						000 0 0   0 0 0   0   0   0   1
#define ATTRS_PAGINA	0x001
#define ATTRS_MASK 		0xFFF

#define CLEAR_ATTRS_MASK		0xFFFFF000
#define CLEAR_PRESENT_BIT_MASK	0xFFFFFFFE

#define PRESENT_BIT_MASK		0x00000001

// figura 3
#define AREA_LIBRE_START	0x100000
#define AREA_LIBRE_END		0x3FFFFF

#define PAGE_DIRECTORY		0x027000

#define ADDR_VIRTUAL_COMPARTIDA		0x400000
#define ADDR_VIRTUAL_CODIGO			0x401000

/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */

uint pag_compartida[2];

uint prox_pag_libre = AREA_LIBRE_START;

uint mmu_proxima_pagina_fisica_libre() {
	uint ret = prox_pag_libre;
	prox_pag_libre += 0x00001000;
	return ret;
}

void mmu_inicializar_pagina(uint * pagina) {
	for (uint i = 0 ; i < PAGE_SIZE / 4; i++) {
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

	char directoryEntryPresent = (pde & PRESENT_BIT_MASK);
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
	tlbflush();
}

uint mmu_unmapear_pagina (uint virtual, uint cr3){
	uint *pd_pointer = (uint*) cr3;
	uint pde_index = virtual >> 22;
	uint pde = pd_pointer[pde_index];

	if (!(pde & PRESENT_BIT_MASK)) {
		return 0;
	}

	uint *pt_pointer = (uint*) (pde & CLEAR_ATTRS_MASK);
	uint pte_index = (virtual & 0x003FF000) >> 12;
	uint pte = pt_pointer[pte_index];

	pte &= CLEAR_PRESENT_BIT_MASK;
	pt_pointer[pte_index] = pte;

	tlbflush();
	return 1;
}

uint mmu_inicializar_dir_kernel() {
	// inicializa el directorio
	mmu_inicializar_pagina((uint*) PAGE_DIRECTORY);

	// identity mapping
	for (uint i = 0; i < 1024; i++) {
		uint addr = i<<12;
		mmu_mapear_pagina(addr, PAGE_DIRECTORY, addr, ATTRS_TABLA);
	}

	return PAGE_DIRECTORY;
}


void mmu_inicializar() {
	pag_compartida[0] = mmu_proxima_pagina_fisica_libre();
	pag_compartida[1] = mmu_proxima_pagina_fisica_libre();

	return;
}

// crea el directorio, las paginas, copia el codigo e inicializa el stack
// el perro viene completo, tiene x y
uint mmu_inicializar_memoria_perro(perro_t *perro, int index_jugador, int index_tipo) {
	uint directorio =  mmu_proxima_pagina_fisica_libre();
	mmu_inicializar_pagina((uint*) directorio);
	//asumiendo index_jugador y index_tipo [0, 1]
	uint fisica_codigo_src = 0x10000 + index_jugador * 0x2000 + index_tipo * 0x1000;
	uint fisica_codigo_dst = mmu_xy2fisica(perro->x, perro->y);
	uint virtual_codigo_dst = mmu_xy2virtual(perro->x, perro->y);
	mmu_mapear_pagina(virtual_codigo_dst, PAGE_DIRECTORY, fisica_codigo_dst, ATTRS_TABLA);

	// TODO quiza debamos cambiar los atributos de estas paginas
	// identity mapping
	for (uint i = 0; i < 1024; i++) {
		uint addr = i<<12;
		mmu_mapear_pagina(addr, directorio, addr, ATTRS_TABLA);
	}

	mmu_mapear_pagina(ADDR_VIRTUAL_COMPARTIDA, directorio, pag_compartida[index_jugador], ATTRS_TABLA);
	mmu_mapear_pagina(ADDR_VIRTUAL_CODIGO, directorio, fisica_codigo_dst, ATTRS_TABLA);
	mmu_mapear_pagina(virtual_codigo_dst, directorio, fisica_codigo_dst, ATTRS_TABLA);

	mmu_copiar_pagina(fisica_codigo_src, virtual_codigo_dst);

	uint * virtual_codigo_dst_ptr = (uint *) virtual_codigo_dst;

	virtual_codigo_dst_ptr[0x400 - 1] = perro->x;

	virtual_codigo_dst_ptr[0x400 - 2] = perro->y;

	return directorio;
}

void mmu_copiar_pagina (uint src, uint dst){
	uint* psrc = (uint*) src;
	uint* pdst = (uint*) dst;

	for (uint i = 0 ; i < PAGE_SIZE / 4 ; i++) {
		pdst[i] = psrc[i];
	}
}

uint mmu_xy2fisica(uint x, uint y) {
	return MAPA_BASE_FISICA + game_xy2lineal(x, y) * 0x1000;
}

uint mmu_xy2virtual(uint x, uint y) {
	return MAPA_BASE_VIRTUAL + game_xy2lineal(x, y) * 0x1000;
}

// debe remapear y copiar el codigo
void mmu_mover_perro(perro_t *perro, int viejo_x, int viejo_y) {

}
