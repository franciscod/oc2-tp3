/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
definicion de funciones del scheduler
*/

#include "screen.h"
#include "game.h"

#define POSICION_RELOJES_F   46
#define POSICION_RELOJES_C_A 4
#define POSICION_RELOJES_C_B 60

#define POSICION_CUADRO_DEBUG_X 24
#define POSICION_CUADRO_DEBUG_Y 6
#define ANCHO_CUADRO_DEBUG      30
#define ALTO_CUADRO_DEBUG       36
#define ALTO_BANDA_DEBUG        1


extern int ultimo_cambio;

extern jugador_t jugadorA, jugadorB;


static ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
static ca (*p2)[VIDEO_COLS];

const char reloj[] = "|/-\\";
#define reloj_size 4


void screen_actualizar_reloj_global()
{
    static uint reloj_global = 0;

    reloj_global = (reloj_global + 1) % reloj_size;

    screen_pintar(reloj[reloj_global], C_BW, 49, 79);
}

void screen_pintar(uchar c, uchar color, uint fila, uint columna)
{
    p[fila][columna].c = c;
    p[fila][columna].a = color;
}

uchar screen_valor_actual(uint fila, uint columna)
{
    return p[fila][columna].c;
}

void print(const char * text, uint x, uint y, unsigned short attr) {
    int i;
    for (i = 0; text[i] != 0; i++)
     {
        screen_pintar(text[i], attr, y, x);

        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_hex(uint numero, int size, uint x, uint y, unsigned short attr) {
    int i;
    char hexa[8];
    char letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_dec(uint numero, int size, uint x, uint y, unsigned short attr) {
    int i;
    char letras[16] = "0123456789";

    for(i = 0; i < size; i++) {
        int resto  = numero % 10;
        numero = numero / 10;
        p[y][x + size - i - 1].c = letras[resto];
        p[y][x + size - i - 1].a = attr;
    }
}


void screen_pintar_rect(uchar letra, uchar color, int fila, int columna, int alto, int ancho)
{
    int f, c;
    for (f = fila; f < fila + alto; f++)
    {
        for (c = columna; c < columna+ancho; c++)
            screen_pintar(letra, color, f, c);
    }
}

void screen_pintar_linea_h(uchar c, uchar color, int fila, int columna, int ancho)
{
    screen_pintar_rect(c, color, fila, columna, 1, ancho);
}

void screen_pintar_linea_v(uchar c, uchar color, int fila, int columna, int alto)
{
    screen_pintar_rect(c, color, fila, columna, alto, 1);
}

void screen_inicializar()
{
    screen_pintar_rect(' ', C_BG_BLACK | C_FG_WHITE, 0, 0, VIDEO_FILS, VIDEO_COLS);
    screen_pintar_rect(' ', C_BG_RED   | C_FG_WHITE, 45, 33, 5, 7);
    screen_pintar_rect(' ', C_BG_BLUE  | C_FG_WHITE, 45, 40, 5, 7);


    screen_pintar_relojes();
    screen_pintar_puntajes();

    int i, j;
    for (i = 0; i < MAPA_ALTO; i++)
    {
        for (j = 0; j < MAPA_ANCHO; j++)
            screen_actualizar_posicion_mapa(j, i);
    }
}

void screen_pintar_puntajes()
{
    print_dec(jugadorA.puntos, 3, 33+2, 45+2, C_BG_RED  | C_FG_WHITE);
    print_dec(jugadorB.puntos, 3, 40+2, 45+2, C_BG_BLUE | C_FG_WHITE);
}

uchar screen_color_jugador(jugador_t *j)
{
    if (j == NULL)
        return C_FG_LIGHT_GREY;

    if (j->index == JUGADOR_A)
        return C_FG_RED;
    else
        return C_FG_BLUE;
}

uchar screen_caracter_perro(uint tipo)
{
    if (tipo == TIPO_1) return '1';
    if (tipo == TIPO_2) return '2';
    while(1){};
    return '?';
}


void screen_pintar_reloj_perro(perro_t *perro)
{
	jugador_t *j = perro->jugador;
    uint columna = j->index == JUGADOR_A ? POSICION_RELOJES_C_A : POSICION_RELOJES_C_B;

    uchar col_libre = C_BG_BLACK | screen_color_jugador(j);

    uchar c, col;
    if (perro->libre == TRUE) {
        c = 'X';
        col = col_libre;
    } else {
        c = reloj[perro->indice_reloj];
        col = C_BW;
    }

    screen_pintar('1' + perro->index, C_BW, POSICION_RELOJES_F    , columna + perro->index * 2);
    screen_pintar(                  c,  col, POSICION_RELOJES_F + 2, columna + perro->index * 2);
}

void screen_pintar_reloj_perros(jugador_t *j)
{
    int i;
    for(i = 0; i < MAX_CANT_PERROS_VIVOS; i++)
        screen_pintar_reloj_perro(&j->perros[i]);
}

void screen_pintar_relojes()
{
    screen_pintar_reloj_perros(&jugadorA);
    screen_pintar_reloj_perros(&jugadorB);
}

void screen_actualizar_reloj_perro (perro_t *perro)
{
    perro->indice_reloj = (perro->indice_reloj + 1) % reloj_size;
    screen_pintar_reloj_perro(perro);
}


void screen_pintar_perro(perro_t *perro)
{
    uchar c     = screen_caracter_perro(perro->tipo);
    uchar color = C_MAKE_BG(screen_color_jugador(perro->jugador)) | C_FG_WHITE;

    screen_pintar(c, color, perro->y+1, perro->x);
}

void screen_borrar_perro(perro_t *perro)
{
    screen_pintar('.', C_BG_GREEN | C_FG_BLACK, perro->y+1, perro->x);
    //screen_actualizar_posicion_mapa(perro->x, perro->y);
}

void screen_pintar_jugador(jugador_t *j)
{
    uchar c     = 'A' + j->index;
    uchar color = C_MAKE_BG(screen_color_jugador(j)) | C_FG_WHITE;

    screen_pintar(c, color, j->y+1, j->x);
}

void screen_borrar_jugador(jugador_t *j)
{
    screen_pintar('.', C_BG_GREEN | C_FG_BLACK, j->y+1, j->x);
    screen_actualizar_posicion_mapa(j->x, j->y);
}


uchar screen_caracter_tesoro(int valor) {
    if (valor > 100)
        return 'O';
    else
        return 'o';
}

void screen_actualizar_posicion_mapa(uint x, uint y)
{
    uchar bg = C_BG_GREEN;

    uchar letra;
    uint valor = game_huesos_en_posicion(x,y);
    perro_t *perro = game_perro_en_posicion(x, y);
    if (perro != NULL) {
        letra = screen_caracter_perro(perro->tipo);
    } else if (valor > 0) {
        letra = screen_caracter_tesoro(valor);
    } else if ((jugadorA.x_cucha == x && jugadorA.y_cucha == y) || (jugadorB.x_cucha == x && jugadorB.y_cucha == y))
    {
        letra = 'x';
    }
    else
    {
        letra = screen_valor_actual(y+1, x);
    }

    screen_pintar(letra, bg | C_FG_BLACK, y+1, x);

}


void screen_stop_game_show_winner(jugador_t *j) {
    int offy = 14; //(50/2 - 11);
    int offx = 20; //(80/2 - 20);

    int ancho = 40;
    int alto = 17;

    uchar color = screen_color_jugador(j);

    screen_pintar_rect(' ', C_MAKE_BG(color) | C_FG_WHITE, offy, offx, alto, ancho);

    offy++; offx++; alto -= 2; ancho-=2;

    screen_pintar_rect('*', C_BW, offy, offx, alto, ancho);

    offy++; offx++; alto -= 2; ancho-=2;

    screen_pintar_rect(' ', C_BG_LIGHT_GREY | C_FG_BLACK, offy, offx, alto, ancho);

    print("EL GANADOR ES EL JUGADOR",   offx+5  , offy+3 , C_BG_LIGHT_GREY | C_FG_BLACK);

    if(j == NULL)      print("EMPATE", offx+14, offy+6, C_MAKE_BG(color) | C_FG_BLACK);
    if(j == &jugadorA) print("<-- A",  offx+15, offy+6, C_BG_LIGHT_GREY  | color);
    if(j == &jugadorB) print("B -->",  offx+15, offy+6, C_BG_LIGHT_GREY  | color);
    // a partir de aca se termina el unviverso (STOP GAME)
    __asm __volatile( "cli\n" : : : );
    while(1){}
}

void screen_guardar_atras_debug() {
    int i, j;
    for (i = POSICION_CUADRO_DEBUG_X; i < POSICION_CUADRO_DEBUG_X + ANCHO_CUADRO_DEBUG; i++) {
        for (j = POSICION_CUADRO_DEBUG_Y-1; j < POSICION_CUADRO_DEBUG_Y + ALTO_CUADRO_DEBUG; j++) {
            p2[j+1][i] = p[j+1][i];
        }
    }
}

void screen_cargar_atras_debug() {
    int i, j;
    for (i = POSICION_CUADRO_DEBUG_X; i < POSICION_CUADRO_DEBUG_X + ANCHO_CUADRO_DEBUG; i++) {
        for (j = POSICION_CUADRO_DEBUG_Y-1; j < POSICION_CUADRO_DEBUG_Y + ALTO_CUADRO_DEBUG; j++) {
            p[j+1][i] = p2[j+1][i];
        }
    }

    for (i = POSICION_CUADRO_DEBUG_X; i < POSICION_CUADRO_DEBUG_X + ANCHO_CUADRO_DEBUG; i++) {
        for (j = POSICION_CUADRO_DEBUG_Y-1; j < POSICION_CUADRO_DEBUG_Y + ALTO_CUADRO_DEBUG; j++) {
            screen_actualizar_posicion_mapa(i, j);
        }
    }
    screen_pintar_jugador(&jugadorA);
    screen_pintar_jugador(&jugadorB);

    for (i = 0; i < MAX_CANT_PERROS_VIVOS; i++) {
        perro_t *dog = &(jugadorA.perros[i]);
        if (dog->libre) continue;
        screen_pintar_perro(dog);
    }
}

void screen_pintar_info_debug(uint eax,
                              uint ebx,
                              uint ecx,
                              uint edx,
                              uint esi,
                              uint edi,
                              uint ebp,
                              uint cs,
                              uint ds,
                              uint fs,
                              uint gs,
                              uint ss,
                              uint eflags,
                              uint esp,
                              uint eip,
                              uint stack0,
                              uint stack1,
                              uint stack2,
                              uint stack3,
                              uint stack4){
    screen_guardar_atras_debug();

    screen_pintar_rect(' ', C_BG_BLACK, POSICION_CUADRO_DEBUG_Y, POSICION_CUADRO_DEBUG_X, ALTO_CUADRO_DEBUG, ANCHO_CUADRO_DEBUG);
    screen_pintar_rect(' ', C_BG_RED, POSICION_CUADRO_DEBUG_Y + 1, POSICION_CUADRO_DEBUG_X + 1, ALTO_BANDA_DEBUG, ANCHO_CUADRO_DEBUG - 2);
    screen_pintar_rect(' ', C_BG_LIGHT_GREY, POSICION_CUADRO_DEBUG_Y + 1 + ALTO_BANDA_DEBUG, POSICION_CUADRO_DEBUG_X + 1, ALTO_CUADRO_DEBUG - ALTO_BANDA_DEBUG - 2, ANCHO_CUADRO_DEBUG - 2);

    print("eax", POSICION_CUADRO_DEBUG_X + 2, POSICION_CUADRO_DEBUG_Y + 3, C_BG_LIGHT_GREY | C_FG_BLACK);
    print_hex(eax, 8, POSICION_CUADRO_DEBUG_X + 6, POSICION_CUADRO_DEBUG_Y + 3, C_BG_LIGHT_GREY | C_FG_WHITE);

    print("ebx", POSICION_CUADRO_DEBUG_X + 2, POSICION_CUADRO_DEBUG_Y + 5, C_BG_LIGHT_GREY | C_FG_BLACK);
    print_hex(ebx, 8, POSICION_CUADRO_DEBUG_X + 6, POSICION_CUADRO_DEBUG_Y + 5, C_BG_LIGHT_GREY | C_FG_WHITE);

    print("ecx", POSICION_CUADRO_DEBUG_X + 2, POSICION_CUADRO_DEBUG_Y + 7, C_BG_LIGHT_GREY | C_FG_BLACK);
    print_hex(ecx, 8, POSICION_CUADRO_DEBUG_X + 6, POSICION_CUADRO_DEBUG_Y + 7, C_BG_LIGHT_GREY | C_FG_WHITE);

    print("edx", POSICION_CUADRO_DEBUG_X + 2, POSICION_CUADRO_DEBUG_Y + 9, C_BG_LIGHT_GREY | C_FG_BLACK);
    print_hex(edx, 8, POSICION_CUADRO_DEBUG_X + 6, POSICION_CUADRO_DEBUG_Y + 9, C_BG_LIGHT_GREY | C_FG_WHITE);

    print("esi", POSICION_CUADRO_DEBUG_X + 2, POSICION_CUADRO_DEBUG_Y + 11, C_BG_LIGHT_GREY | C_FG_BLACK);
    print_hex(esi, 8, POSICION_CUADRO_DEBUG_X + 6, POSICION_CUADRO_DEBUG_Y + 11, C_BG_LIGHT_GREY | C_FG_WHITE);

    print("edi", POSICION_CUADRO_DEBUG_X + 2, POSICION_CUADRO_DEBUG_Y + 13, C_BG_LIGHT_GREY | C_FG_BLACK);
    print_hex(edi, 8, POSICION_CUADRO_DEBUG_X + 6, POSICION_CUADRO_DEBUG_Y + 13, C_BG_LIGHT_GREY | C_FG_WHITE);

    print("ebp", POSICION_CUADRO_DEBUG_X + 2, POSICION_CUADRO_DEBUG_Y + 15, C_BG_LIGHT_GREY | C_FG_BLACK);
    print_hex(ebp, 8, POSICION_CUADRO_DEBUG_X + 6, POSICION_CUADRO_DEBUG_Y + 15, C_BG_LIGHT_GREY | C_FG_WHITE);

    print("esp", POSICION_CUADRO_DEBUG_X + 2, POSICION_CUADRO_DEBUG_Y + 17, C_BG_LIGHT_GREY | C_FG_BLACK);
    print_hex(esp, 8, POSICION_CUADRO_DEBUG_X + 6, POSICION_CUADRO_DEBUG_Y + 17, C_BG_LIGHT_GREY | C_FG_WHITE);

    print("eip", POSICION_CUADRO_DEBUG_X + 2, POSICION_CUADRO_DEBUG_Y + 19, C_BG_LIGHT_GREY | C_FG_BLACK);
    print_hex(eip, 8, POSICION_CUADRO_DEBUG_X + 6, POSICION_CUADRO_DEBUG_Y + 19, C_BG_LIGHT_GREY | C_FG_WHITE);


    print("cs", POSICION_CUADRO_DEBUG_X + 3, POSICION_CUADRO_DEBUG_Y + 21, C_BG_LIGHT_GREY | C_FG_BLACK);
    print_hex(cs, 8, POSICION_CUADRO_DEBUG_X + 6, POSICION_CUADRO_DEBUG_Y + 21, C_BG_LIGHT_GREY | C_FG_WHITE);

    print("ds", POSICION_CUADRO_DEBUG_X + 3, POSICION_CUADRO_DEBUG_Y + 23, C_BG_LIGHT_GREY | C_FG_BLACK);
    print_hex(ds, 8, POSICION_CUADRO_DEBUG_X + 6, POSICION_CUADRO_DEBUG_Y + 23, C_BG_LIGHT_GREY | C_FG_WHITE);

    print("fs", POSICION_CUADRO_DEBUG_X + 3, POSICION_CUADRO_DEBUG_Y + 25, C_BG_LIGHT_GREY | C_FG_BLACK);
    print_hex(fs, 8, POSICION_CUADRO_DEBUG_X + 6, POSICION_CUADRO_DEBUG_Y + 25, C_BG_LIGHT_GREY | C_FG_WHITE);

    print("gs", POSICION_CUADRO_DEBUG_X + 3, POSICION_CUADRO_DEBUG_Y + 27, C_BG_LIGHT_GREY | C_FG_BLACK);
    print_hex(gs, 8, POSICION_CUADRO_DEBUG_X + 6, POSICION_CUADRO_DEBUG_Y + 27, C_BG_LIGHT_GREY | C_FG_WHITE);

    print("ss", POSICION_CUADRO_DEBUG_X + 3, POSICION_CUADRO_DEBUG_Y + 29, C_BG_LIGHT_GREY | C_FG_BLACK);
    print_hex(ss, 8, POSICION_CUADRO_DEBUG_X + 6, POSICION_CUADRO_DEBUG_Y + 29, C_BG_LIGHT_GREY | C_FG_WHITE);


    print("eflags", POSICION_CUADRO_DEBUG_X + 3, POSICION_CUADRO_DEBUG_Y + 31, C_BG_LIGHT_GREY | C_FG_BLACK);
    print_hex(eflags, 8, POSICION_CUADRO_DEBUG_X + 10, POSICION_CUADRO_DEBUG_Y + 31, C_BG_LIGHT_GREY | C_FG_WHITE);


    print("cr0", POSICION_CUADRO_DEBUG_X + 16, POSICION_CUADRO_DEBUG_Y + 3, C_BG_LIGHT_GREY | C_FG_BLACK);
    print_hex(rcr0(), 8, POSICION_CUADRO_DEBUG_X + 20, POSICION_CUADRO_DEBUG_Y + 3, C_BG_LIGHT_GREY | C_FG_WHITE);

    print("cr2", POSICION_CUADRO_DEBUG_X + 16, POSICION_CUADRO_DEBUG_Y + 5, C_BG_LIGHT_GREY | C_FG_BLACK);
    print_hex(rcr2(), 8, POSICION_CUADRO_DEBUG_X + 20, POSICION_CUADRO_DEBUG_Y + 5, C_BG_LIGHT_GREY | C_FG_WHITE);

    print("cr3", POSICION_CUADRO_DEBUG_X + 16, POSICION_CUADRO_DEBUG_Y + 7, C_BG_LIGHT_GREY | C_FG_BLACK);
    print_hex(rcr3(), 8, POSICION_CUADRO_DEBUG_X + 20, POSICION_CUADRO_DEBUG_Y + 7, C_BG_LIGHT_GREY | C_FG_WHITE);

    print("cr4", POSICION_CUADRO_DEBUG_X + 16, POSICION_CUADRO_DEBUG_Y + 9, C_BG_LIGHT_GREY | C_FG_BLACK);
    print_hex(rcr4(), 8, POSICION_CUADRO_DEBUG_X + 20, POSICION_CUADRO_DEBUG_Y + 9, C_BG_LIGHT_GREY | C_FG_WHITE);


    print("stack", POSICION_CUADRO_DEBUG_X + 16, POSICION_CUADRO_DEBUG_Y + 20, C_BG_LIGHT_GREY | C_FG_BLACK);
    print_hex(stack0, 8, POSICION_CUADRO_DEBUG_X + 16, POSICION_CUADRO_DEBUG_Y + 23, C_BG_LIGHT_GREY | C_FG_WHITE);
    print_hex(stack1, 8, POSICION_CUADRO_DEBUG_X + 16, POSICION_CUADRO_DEBUG_Y + 24, C_BG_LIGHT_GREY | C_FG_WHITE);
    print_hex(stack2, 8, POSICION_CUADRO_DEBUG_X + 16, POSICION_CUADRO_DEBUG_Y + 25, C_BG_LIGHT_GREY | C_FG_WHITE);
    print_hex(stack3, 8, POSICION_CUADRO_DEBUG_X + 16, POSICION_CUADRO_DEBUG_Y + 26, C_BG_LIGHT_GREY | C_FG_WHITE);
    print_hex(stack4, 8, POSICION_CUADRO_DEBUG_X + 16, POSICION_CUADRO_DEBUG_Y + 27, C_BG_LIGHT_GREY | C_FG_WHITE);
}
