/*  Programa: Dots and Boxes
       Autor: Rafael Timbó Matos (RA 106228)
       Turma MC102Y
       Versao: a
       Concluída em: 17/05/2010
       Breve descricao:
           Este programa simula o jogo Dots and Boxes.
*/

#include <stdio.h>
#include <allegro.h>


#define TRUE -1
#define FALSE 0

#define JOGADOR1 2
#define JOGADOR2 -2
#define MARCADO 12
#define NULO 0
#define COR_CIRCULO makecol( 255, 255, 255)
#define COR_RETANGULO makecol( 255, 255, 255)
#define COR_RETANGULO_VISUALIZAR makecol( 50, 120, 50)
#define COR_FUNDO makecol( 0, 0, 0)
#define COR1 makecol( 250, 0, 0)
#define COR2 makecol( 0, 0, 250)

#define LADO 5
#define RAIO 3
#define MARGEM LADO
#define MARGEM_MOUSE 3
#define PROPORCAO LADO * 10
#define TELA_DIMENSAO (2 * MARGEM + LADO*PROPORCAO)


//POSSIVEIS COMBINACOES para funcionar a tela
//LADO PROPORCAO
//20   2
//7    10
//5    10

int Confere();
int Rodada(int linha[LADO + 1][LADO + 1], int coluna[LADO + 1][LADO + 1], int vez, BITMAP *buffer);

void Screen(BITMAP *buffer)
{
    blit(buffer, screen, 0, 0, 0, 0, TELA_DIMENSAO, TELA_DIMENSAO);
}

void Sair()
{
if (key[KEY_ESC])
        {
            allegro_message(" A Tecla ESC foi pressionada : o jogo sera interrompido");
            exit(EXIT_SUCCESS);
        }
}

int JogadaInvalida(int linha[LADO + 1][LADO + 1], int coluna[LADO + 1][LADO + 1], int vez, BITMAP *buffer)
{
    allegro_message("Este espaco ja foi marcado!!");
    return Rodada(linha, coluna, vez, buffer);
}

void init(BITMAP **buffer)
//Carrega as configurações iniciais necessárias para o jogo
{
    allegro_init();

    install_keyboard();

    install_timer();

    srand(time(NULL));

    install_mouse();

    set_color_depth(desktop_color_depth());

    if ( (set_gfx_mode(GFX_AUTODETECT_WINDOWED,TELA_DIMENSAO, TELA_DIMENSAO, 0, 0) ) != 0) {
        if ( (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0) ) != 0){
        allegro_message(allegro_error);
        exit(EXIT_FAILURE);
        }
        else *buffer = create_bitmap(640 ,480);
    }
    else *buffer = create_bitmap(TELA_DIMENSAO,TELA_DIMENSAO);
    set_window_title("Dots and Boxes - Pressione ESC para sair");
}

void Dots(BITMAP *buffer)
//Esta funcao desenha os pontos que guiarão os jogadores ao longo do jogo
//Usada somente uma vez
{
    int i, j
    ;

    rectfill( buffer, 0 , 0, TELA_DIMENSAO, TELA_DIMENSAO, COR_FUNDO);
    for (i = 0; i <= LADO ; i++)
        for (j = 0 ; j <= LADO ; j++)
            circlefill(buffer,MARGEM + j * PROPORCAO,MARGEM + i * PROPORCAO, RAIO, COR_CIRCULO);
            //void circlefill(BITMAP *bmp, int x, int y, int  radius, int color);

    Screen(buffer);
}

int Verificay( int *y)
//esta funcao modela o valor de y = mouse_y e deixa-o coerente para manipular o retangulo que o usuario quer escolher
{
    short int i;
    if ( *y > TELA_DIMENSAO || *y < 0 )
    {
        rest_callback(900, Sair);
        return Verificay(y);
    }
    else
    {
        if (*y <= 3*RAIO + MARGEM)
        {
            *y = 0;
            return FALSE;
        }
        else
        for ( i = 1 ; i <= LADO ; i++)
            if ( *y <= MARGEM + i * PROPORCAO - RAIO && *y >= MARGEM + (i - 1) * PROPORCAO + RAIO)
            {
                *y = i;
                return TRUE;
            }
            else if ( *y >= MARGEM + i * PROPORCAO - RAIO && *y <= MARGEM + i * PROPORCAO + RAIO)
            {
                *y = i;
                return FALSE;
            }

        return Confere();
    }
}

int LinhaOuColuna(int *x, int *y)
//Esta funcao verifica se a regiao clicada corresponde a um  retangulo horizontal ou a um retangulo vertical
//alem de adaptar os valores de x e y;
{
    //printf("x=%d y = %d \n",*x,*y);
    short int j , i
    ;
    if (*x > TELA_DIMENSAO || *y > TELA_DIMENSAO || *y < 0 || *x < 0) // nao e necessario executar a funcao se o mouse estiver fora da tela
    {
        rest_callback(900, Sair);
        return LinhaOuColuna( x, y);
    }
    else
    {
        if ( *x <= RAIO + MARGEM)
        {
            *x = 0;
            Verificay( y );
            return FALSE;
        }
        for (j = 1 ; j <= LADO ; j++)
            if ( *x >= MARGEM + (j - 1) * PROPORCAO + RAIO && (*x <= MARGEM + j * PROPORCAO - RAIO))
            {
                *x = j ;
                Verificay( y );
                return TRUE;
                //if (Verificay( y )) return Verificay(y);
                //else allegro_message("return de LinhaOuColuna(TRUE) Nao Bateu Com Verificay(FALSE)");
            }
            else if ( *x >= MARGEM + j * PROPORCAO - RAIO && ( *x <= MARGEM + j * PROPORCAO + RAIO ) )
            {
                *x = j;
                Verificay( y );
                return FALSE;
                //if (Verificay( y
                //if (!Verificay ( y ) ) return Verificay(y);
                //else allegro_message("return de LOC FALSE Nao bateu com Verificay TRUE");
            }

        poll_mouse();
        *x = mouse_x;
        *y = mouse_y;
        return LinhaOuColuna( x , y);
    }
}

int Confere()
{
    poll_mouse();

    int x = mouse_x,
    y = mouse_y;
    return LinhaOuColuna(&x, &y);
}

int PertoZero()
{
    int x, y;
    poll_mouse();
    x = mouse_x;
    y = mouse_y;
    if (mouse_x < MARGEM + RAIO) x = MARGEM;
    else if (mouse_y < MARGEM + RAIO) y = MARGEM;



}
void VisualizaRetangulo(int x, int y, BITMAP *buffer)
{

        if (LinhaOuColuna( &x , &y ))
        {
            //printf(" x VR %d y VR %d \n", x, y);
            x = x * PROPORCAO;
            y = y * PROPORCAO;
            rectfill( screen, MARGEM + x - PROPORCAO, MARGEM + y - RAIO, MARGEM + x , MARGEM + y + RAIO , COR_RETANGULO_VISUALIZAR );
            //void rectfill(BITMAP *bmp, int x1, int y1, int x2, int y2, int color);
        }
        else
        {
            x = x * PROPORCAO;
            y = y * PROPORCAO;
            //printf(" screen, MARGEM + x - RAIO, MARGEM + y - PROPORCAO,\n MARGEM + x + RAIO, MARGEM + y , COR_RETANGULO_VISUALIZAR ) \n");
            printf(" screen,%d,%d,%d,%d,COR) \n",MARGEM + x - RAIO, MARGEM + y - PROPORCAO, MARGEM + x + RAIO, MARGEM + y);
            rectfill( screen, MARGEM + x - RAIO, MARGEM + y - PROPORCAO, MARGEM + x + RAIO, MARGEM + y , COR_RETANGULO_VISUALIZAR );

        }
        //Screen(buffer);
}

int Rodada(int linha[LADO + 1][LADO + 1], int coluna[LADO + 1][LADO + 1], int vez, BITMAP *buffer)
{
    int x , y , clicou = FALSE;
    ;

    printf(" \n Rodada \n");

    rest(100);
    x = -5;
    y = -5;

    fflush(stdin);
    while ( mouse_b & 1) poll_mouse(); // se o jogador mantiver o botao pressionado, o jogo nao contara isso como 2 ou mais  jogadas.
    clicou = FALSE;
    while (!clicou)
    {
        poll_mouse();
        if ( !(x >= mouse_x - MARGEM_MOUSE && x<= mouse_x +MARGEM_MOUSE) || !(y >= mouse_y - MARGEM_MOUSE && y <= mouse_y + MARGEM_MOUSE) ) //so atualize se houver variações significativas na posição do mouse
        {
            x =(int) mouse_x ;
            y =(int) mouse_y ;
            Screen(buffer);
            if (!(mouse_b & 1) ) VisualizaRetangulo( x, y, buffer);
        }

        if (mouse_b & 1) clicou = TRUE;
            //else VisualizaRetangulo( x , y, buffer);
        Sair();
    }
    Screen(buffer);
    //Screen(buffer);

    if (LinhaOuColuna( &x , &y )) //Retorna TRUE para LINHA e FALSE para COLUNA, além de ajustar os valores x e y para que o retangulo seja corretamente processado e desenhado.
    {
        printf("Marcando linha[%d][%d] \n",y,x-1);
        if (linha[y][x-1] == NULO) linha[y][x-1] = MARCADO;   else return JogadaInvalida(linha, coluna, vez, buffer);
        x = x * PROPORCAO;
        y = y * PROPORCAO;
        rectfill( buffer, MARGEM + x - PROPORCAO, MARGEM + y - RAIO, MARGEM + x , MARGEM + y + RAIO , COR_RETANGULO );
        Screen(buffer);
        return TRUE;
        //void rectfill(BITMAP *bmp, int x1, int y1, int x2, int y2, int color);
    }
    else
    {
        printf("Marcando coluna[%d][%d] \n",y-1,x);
        if (coluna[y-1][x] == NULO) coluna[y-1][x] = MARCADO; else return JogadaInvalida(linha, coluna, vez, buffer);
        x = x * PROPORCAO;
        y = y * PROPORCAO;
        rectfill( buffer, MARGEM + x - RAIO, MARGEM + y - PROPORCAO, MARGEM + x + RAIO, MARGEM + y , COR_RETANGULO );
        Screen(buffer);
        return TRUE;
    }
    return Rodada(linha, coluna, vez, buffer);
}

int Quadrado(int linha[LADO + 1][LADO + 1],int coluna [LADO + 1][LADO + 1], int vez, int Box[LADO][LADO],int *i, int *j)
{
    for (*i = 0; *i <= LADO ; (*i)++)
        for (*j = 0; *j <= LADO ; (*j)++)
            if (linha[(*i)+1 ][*j] == linha[(*i)][*j] && linha[(*i)][*j] != NULO)
                if (coluna[*i][(*j)] != NULO && coluna[*i][(*j)+ 1] != NULO)
                    if (Box[*i][*j] == NULO)
                    {
                        Box[*i][*j] = vez;
                        if (vez  == JOGADOR1)
                            return JOGADOR1;
                        else
                            return JOGADOR2;
                    }
    return FALSE;
}

int main(void)
{
    int Box[LADO][LADO], jogadasvalidas = 2 * LADO * (LADO + 1), vez,
    linhas[LADO + 1][LADO + 1], colunas[LADO + 1][ LADO + 1],
    i, j, comeco, aux;
    ;

    BITMAP *buffer;
    init(&buffer);
    Dots(buffer);

    switch( rand() % 2 + 1)
    {
        case 1:
        vez = JOGADOR1;

        case 2:
        vez = JOGADOR2;
    }
    comeco = vez;

    //zere as matrizes
    for (i = 0; i <= LADO ; i++)
        for (j = 0; j <= LADO ; j++)
        {
            if (j != LADO && i != LADO) Box[i][j] = NULO;
            linhas[i][j] = NULO;
            colunas[i][j] = NULO;
        }
    show_mouse(screen);
    do
    {
        printf("Vez do jogador %d \n", vez==JOGADOR2? 2 : 1);
        Rodada(linhas, colunas, vez, buffer);
        aux = Quadrado(linhas,colunas,vez,Box, &i, &j);
        printf("QUADRADO %d", aux);
        if (aux == JOGADOR1)
        {
            rectfill(buffer, MARGEM + j * PROPORCAO, MARGEM + i * PROPORCAO, MARGEM + j * PROPORCAO + PROPORCAO, MARGEM + i * PROPORCAO + PROPORCAO, COR1);
            aux  = Quadrado(linhas,colunas,vez,Box, &i, &j);
            if (aux == JOGADOR1)
                rectfill(buffer, MARGEM + j * PROPORCAO, MARGEM + i * PROPORCAO, MARGEM + j * PROPORCAO + PROPORCAO, MARGEM + i * PROPORCAO + PROPORCAO, COR1);
            else if (aux == JOGADOR2)
                rectfill(buffer, MARGEM + j * PROPORCAO, MARGEM + i * PROPORCAO, MARGEM + j * PROPORCAO + PROPORCAO, MARGEM + i * PROPORCAO + PROPORCAO, COR2);
        }
        else if ( aux ==JOGADOR2)
        {
            rectfill(buffer, MARGEM + j * PROPORCAO, MARGEM + i * PROPORCAO, MARGEM + j * PROPORCAO + PROPORCAO, MARGEM + i * PROPORCAO + PROPORCAO, COR2);
            aux = Quadrado(linhas,colunas,vez,Box, &i, &j);
            if (aux == JOGADOR1)
                rectfill(buffer, MARGEM + j * PROPORCAO, MARGEM + i * PROPORCAO, MARGEM + j * PROPORCAO + PROPORCAO, MARGEM + i * PROPORCAO + PROPORCAO, COR1);
            else if (aux == JOGADOR2)
                rectfill(buffer, MARGEM + j * PROPORCAO, MARGEM + i * PROPORCAO, MARGEM + j * PROPORCAO + PROPORCAO, MARGEM + i * PROPORCAO + PROPORCAO, COR2);
        }
        else vez = -vez;

        Screen(buffer);
        printf(" jogadasvalidas %d  \n", jogadasvalidas);
        jogadasvalidas--;

    }while ( jogadasvalidas != 0 );
    aux = 0;

    for (i = 0 ; i < LADO ; i++)
        for (j = 0 ; j < LADO ; j++)
            if (Box[i][j] == JOGADOR1) aux++;
    if (aux < LADO*LADO/2) allegro_message("O Jogador 1(VERMELHO) venceu!!");
    else if (aux > LADO*LADO/2) allegro_message("O Jogador 2(AZUL) venceu!!");
    if (LADO*LADO % 2 ==0 )
    {
        if (aux==LADO*LADO/2)
            if (comeco == JOGADOR1) allegro_message("O Jogador 2(AZUL) venceu!!");
        else allegro_message("O Jogador 1(VERMELHO) venceu!!");
    }
    else if (aux==LADO*LADO/2) allegro_message("O Jogador 2(AZUL) venceu!!");

allegro_message("fim");
 return 0;
}
END_OF_MAIN();

// ERA PRA C COLUNA VIRA LINHA QUANDO ERA LINHA APARECE NA PROXIMA LINHA
