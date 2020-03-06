/*JOGO SNAKE*/

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<windows.h>

//pontuacao base dos items
#define COMIDA 2
#define SLOWER 1
#define FASTER 3
#define SKIP 40
//constantes
#define TAMSNAKE 300
#define PLACAR 10
#define NTUNEL 5
#define TAMNOME 4

FILE *arq;



typedef struct
{
    int speed;
    int maxlen;
    int maxitem;
}CONFIG;

typedef struct
{
  int x;
  int y;
}COORDENADA;

typedef struct
{
  COORDENADA cabeca;
  int tam;
  int velo;
  int pont;
  char d;//DIRECAO ATUAL DA COBRA
} SNAKE;

typedef struct
{
    int pontuacao;
    char nome[TAMNOME];
}SCORE;

typedef struct
{
    COORDENADA posicao;
    int id_entrada;
    char entrada;//DIRECAO DE ENTRADA DO TUNEL (a cobra deve estar indo nessa direcao para entrar)
    int id_saida;
}TUNEL;



void salva_config(CONFIG config)
{

    if(!(arq = fopen("arquivo.bin","wb")))
    {
        printf("Erro de criacao");
    }
    else
    {
        fwrite(&config, sizeof(CONFIG), 1, arq);
        fclose(arq);
    }

}

void carrega_config(CONFIG *config)
{
    if(!(arq = fopen("arquivo.bin","rb")))
    {
        printf("Erro de abertura\n");
        config->speed = 1;
        config->maxlen = 10;
        config->maxitem = 1;
        printf("Velocidade: %4d\n", config->speed);
        printf("Comprimento Maximo: %4d\n", config->maxlen);
        printf("Numero de itens: %4d\n", config->maxitem);
    }
    else
    {
        if(fread(config, sizeof(CONFIG), 1, arq) == 1)
        {
            printf("Velocidade: %4d\n", config->speed);
            printf("Comprimento Maximo: %4d\n", config->maxlen);
            printf("Numero de itens: %4d\n", config->maxitem);
        }
        fclose(arq);
    }

}

void salva_score(SCORE score[PLACAR])
{
    int i = 0;
    if(!(arq = fopen("scores.txt","w")))
    {
        printf("Erro de criacao");
    }
    else
    {
        while(i<PLACAR)
        {
            fprintf(arq, "%d %s \n", score[i].pontuacao, score[i].nome);
            i++;
        }

    }
    fclose(arq);


}

void Carrega_score (SCORE score[PLACAR])
{
  int j = 0;

    if(!(arq = fopen("scores.txt", "r")))
        printf("Erro de leitura");

    else
    {

        while(!feof(arq) && j < PLACAR)
        {
            if(fscanf(arq, "%d%s", &score[j].pontuacao, score[j].nome)!= NULL)
            j++;
        }
    }
    fclose(arq);
}

void load_print_score(SCORE score[PLACAR])
{
    int j;
    if(!(arq = fopen("scores.txt", "r")))
    {
        printf("Erro de leitura");
        for(j = 0; j<PLACAR; j++)
        {
            score[j].pontuacao = 0;
            strcpy(score[j].nome, "AAA");
            printf("%d ", score[j].pontuacao);
            printf("%s\n", score[j].nome);

        }

    }
    else
    {
        j = 0;
        while(!feof(arq) && j < PLACAR)
        {
            if(fscanf(arq, "%d%s", &score[j].pontuacao, score[j].nome)!= NULL)
            {
                    printf("%d ", score[j].pontuacao);
                    printf("%s\n", score[j].nome);
            }
            j++;
        }
    }
    fclose(arq);
}

void Score(int pontuacao)
{
    int placar[PLACAR] = {0};
    SCORE score0 = {pontuacao};
    SCORE scores[PLACAR];
    int teste = score0.pontuacao;
    char player[PLACAR][TAMNOME];
    int placaraux[PLACAR];
    char playeraux[PLACAR][TAMNOME];
    int i;
    int j = 0;
    int cont;

    Carrega_score(scores);

    for(i=0;i<PLACAR;i++) //passa os scores para duas matrizes independentes
    {
      strcpy(player[i], scores[i].nome);
      placar[i] = scores[i].pontuacao;
    }

    do
    {
        printf("Digite seu nome(com 3 caracteres)\n");
        scanf(" %s", score0.nome); //pega o nome do jogador
    }
    while(strlen(score0.nome)>3);

    while(score0.pontuacao < placar[j])//encontra indice da posicao correta da nova pont  (j)
      j++;

    for(i=0;i<PLACAR;i++) //copia as matrizes placar e player
    {
        placaraux[i] = placar[i];
        strcpy(playeraux[i],player[i]);
    }
    for(cont=j;cont<PLACAR;cont++)//empurra todos os nomes e pontuações que estavam depois de "j" a partir das auxiliares copiadas
    {
      placar[cont+1] = placaraux[cont];
      strcpy(player[cont+1], playeraux[cont]);
    }

    placar [j] = score0.pontuacao; //coloca a pont e o nome no lugar certo
    strcpy(player[j], score0.nome);


    for(i=0;i<PLACAR;i++)
    {
      strcpy(scores[i].nome, player[i]);
      scores[i].pontuacao = placar[i];
    }

    salva_score(scores);

    printf("\n\n");

    load_print_score(scores);//ja imprime o placar na tela

    //for(j=0; j<PLACAR; j++)
      //printf("%2d- %3d     %s\n", j+1, placar[j], player[j]); //placar das 5 maiores pontuações e seus jogadores

    score0.pontuacao = 0;

}

int Busca_tunel (TUNEL tuneis[NTUNEL], COORDENADA coor)
{
    int i;

    for(i=0; i<NTUNEL; i++)
        if ((coor.x == tuneis[i].posicao.y) && (coor.y == tuneis[i].posicao.x))
        return (i);
 }

 void Entra_tunel (TUNEL tuneis[NTUNEL], SNAKE *cobra, char *tecla)
 {
     int dtun, i;

     dtun = Busca_tunel(tuneis, cobra->cabeca);
     i = tuneis[dtun].id_saida; //para acessar as cordenadas de saida

     if (cobra->d == tuneis[dtun].entrada)//se a direcao da cobra for a mesma da direcao de entrada do tunel
        {
        cobra->cabeca.x = tuneis[i].posicao.y;
        cobra->cabeca.y = tuneis[i].posicao.x;//coloca a cabeca da cobra na saida do tunel
        mgotoxy(cobra->cabeca.x, cobra->cabeca.y);
        }
     else
      *tecla = 27;//acaba o jogo

 }

 void Gera_tunel (char cen[][81], TUNEL tuneis[NTUNEL], int etapa)
 {
     int i=0;


     if(etapa == 2)//cenario 2
         for(i=0; i<2; i++)
         {
             mgotoxy(tuneis[i].posicao.y - 1, tuneis[i].posicao.x);
             printf(" %c", tuneis[i].entrada);
             cen[tuneis[i].posicao.x][tuneis[i].posicao.y] = 'U';
         }
    if (etapa == 3)//cenario 3
        for(i=0; i<5; i++)
        {
          mgotoxy(tuneis[i].posicao.y - 1, tuneis[i].posicao.x);//imprime na tela e na matriz
          printf(" %c", tuneis[i].entrada);
          cen[tuneis[i].posicao.x][tuneis[i].posicao.y] = 'U';
        }

 }

void mgotoxy(int x, int y) //coloca o cursor nas coordenadas da tela
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){x,y});
}

char escolhe_item ()
{
    int num = rand()%100 +1;//gera numero aleatorio entre 1 e 100

    if((num>0)&&(num<=67))//comida 67% de chance
        return 'c';
    if((num>67)&&(num<=87))//faster 20% de chance
        return 'F';
    if((num>87)&&(num<=97))//slower 10% de chance
        return 's';
    if((num>97)&&(num<=100))//Skip 3% de chance
        return '$';

}

void gera_item (char cen[][81])
{
    int x, y;
    int i = 0;
    char item;

    item = escolhe_item();

    do{
        x = rand()%80 + 1;//gera um numero entre 1 e 80
        y = rand()%25 + 1;//gera um numero entre 1 e 25

        if (cen[y][x] == '0')//confere posicao aleatoria se esta vaga
            {
                mgotoxy(x, y);
                printf("%c", item);//se estiver vaga, imprime na tela e na matriz e i recebe 1, avisando que um item foi gerado
                cen[y][x] = item;
                i = 1;
            }
        else
            i = 0;//se nao colocou um item tenta novamente

    }while(i != 1);//controla se colocou o caracter ou nao


}

void Desenha_interface(SNAKE cobra, int cenario)
{
   int velo2 = (200 - cobra.velo)/10; //valor aparente da velo(inverso) e na faixa de 1 - 10
   printf(" Pontuacao: %3d    ", cobra.pont);
   printf("Tamanho: %d      ", cobra.tam);
   printf("velocidade: %d   ", velo2);
   printf("Cenario: %d      ", cenario);
}

void Desenha_matriz (char cen[][81]) //MOSTRA O CONTEUDO DA MATRIZ - **TESTE**
{
  int i, j;

  for (i=0; i<80; i++)
                for (j=0; j<25; j++)//percorrem a matriz
                {
                    if(cen[j][i] == '1')//imprime bloco se na matriz for '1'
                        {
                            mgotoxy(i,j+1);
                            printf("%1");
                        }
                    if(cen[j][i] == '0')//imprime espaco se na matriz fo '0'
                        {
                            mgotoxy(i,j+1);
                            printf(" ");
                        }
                    if(cen[j][i] == 'U')//imprime espaco se na matriz fo '0'
                        {
                            mgotoxy(i,j+1);
                            printf("U");
                        }
                    if(cen[j][i] == 'c')//imprime espaco se na matriz fo '0'
                        {
                            mgotoxy(i,j+1);
                            printf("c");
                        }
                    if(cen[j][i] == 'F')//imprime espaco se na matriz fo '0'
                        {
                            mgotoxy(i,j+1);
                            printf("F");
                        }
                    if(cen[j][i] == '$')//imprime espaco se na matriz fo '0'
                        {
                            mgotoxy(i,j+1);
                            printf("$");
                        }
                    if(cen[j][i] == 's')//imprime espaco se na matriz fo '0'
                        {
                            mgotoxy(i,j+1);
                            printf("s");
                        }
                }
}


void Desenha_cenario (int n)
{
    int i, j;
    char cen1[25][81] ={"1111111111111111111111111111111111111111111111111111111111111111111111111111111",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1111111111111111111111111111111111111111111111111111111111111111111111111111111"};

    char cen2[25][81] ={"1111111111111111111111111111111111111111111111111111111111111111111111111111111",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1111111111111111111111111111111111111111111111111111111111111111111111111111111"};

    char cen3 [25][81]={"1111111111111111111111111111111111111111111111111111111111111111111111111111111",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1111111111111111111111111111111111111111111111111111111111111111111111111111111",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1111111111111111111111111111111111111111111111111111111111111111111111111111111",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1111111111111111111111111111111111111111111111111111111111111111111111111111111"};

    switch(n)
    {
        case 1: //cenario 1
            for (i=0; i<80; i++)
                for (j=0; j<25; j++)//percorrem a matriz
                {
                    if(cen1[j][i] == '1')//imprime bloco se na matriz for '1'
                        {
                            mgotoxy(i,j+1);
                            printf("%c",219);
                        }
                    if(cen1[j][i] == '0')//imprime espaco se na matriz fo '0'
                        {
                            mgotoxy(i,j+1);
                            printf(" ");
                        }
                }
                break;
        case 2://cenario 2
            for (i=0; i<80; i++)
                for (j=0; j<25; j++)
                {
                    if(cen2[j][i] == '1')
                        {
                            mgotoxy(i,j+1);
                            printf("%c",219);
                        }
                    if(cen2[j][i] == '0')
                        {
                            mgotoxy(i,j+1);
                            printf(" ");
                        }
                }
                break;
        case 3://cenario 3
            for (i=0; i<80; i++)
                for (j=0; j<25; j++)
                {
                    if(cen3[j][i] == '1')
                        {
                            mgotoxy(i,j+1);
                            printf("%c",219);
                        }
                    if(cen3[j][i] == '0')
                        {
                            mgotoxy(i,j+1);
                            printf(" ");
                        }
                }
                break;

    }
}

void Colidiu (char *tecla, char cen[][81], SNAKE *cobra, TUNEL tuneis[NTUNEL], int n, int cx[TAMSNAKE], int cy[TAMSNAKE])
{
    int j;
    int i;
    switch (cen[cobra->cabeca.y][cobra->cabeca.x])
    {
        case '1':*tecla = 27    ;//obstáculo     sair do jogo tecla = 27
            break;

        case 'c' : cen[cobra->cabeca.y][cobra->cabeca.x] = '0'; //comida -> 2 pontos
                gera_item(cen);
                calcula_pontuacao (cobra, 2); //função que calcula a pontuação
                cobra->tam = cobra->tam + 1;
            break;

        case 'F': cen[cobra->cabeca.y][cobra->cabeca.x] = '0';  //faster -> 3 pontos
                gera_item(cen);
                calcula_pontuacao (cobra, 3);
                if(cobra->velo > 10) //teste da vel max
                cobra->velo -= 10; //aumenta a velocidade
            break;

        case 's': cen[cobra->cabeca.y][cobra->cabeca.x] = '0';  //slower -> 1 ponto
                gera_item(cen);
                calcula_pontuacao (cobra, 1);
                if(cobra->velo < 190) //teste da vel min
                cobra->velo += 10; //diminui a velocidade

            break;

        case '$': *tecla = 27;                    //skip -> 40 pontos
                cen[cobra->cabeca.y][cobra->cabeca.x] = '0';
                gera_item(cen);
                calcula_pontuacao (cobra, 40);

            break;

        case 'U': Entra_tunel(tuneis, cobra, tecla);//se estiver na direcao certa, entra no tunel, senao perde o jogo
            break;
        default: //testa sempre que não é um dos casos especiais

        for(j=1; j<cobra->tam; j++) //procura se a cabeça da cobra
            {
                if(cobra->cabeca.x==cx[j]&&cobra->cabeca.y==cy[j])
                    *tecla= 27;
            }
            break;


    }
}


void Movimentacao (int cena, char cen[][81], SNAKE *cobra, TUNEL tuneis[NTUNEL])
{
    int x,d=2,cx[TAMSNAKE]= {cobra->cabeca.x},cy[TAMSNAKE]= {cobra->cabeca.y};
    char tecla='i';
    int prox;


    while(tecla!= 27)
    {
        while(tecla!= 27 &&!(tecla=kbhit()))
        {
            for(x=cobra->tam; x>0; x--)
            {
                cx[x]=cx[x-1]; //apagar o rastro(move para direita)
                cy[x]=cy[x-1];
            }

            if(cobra->d=='0')cx[0]--; //movimentação
            if(cobra->d=='1')cy[0]--;
            if(cobra->d=='2')cx[0]++;
            if(cobra->d=='3')cy[0]++;

            cobra->cabeca.x = cx[0];
            cobra->cabeca.y = cy[0];

            mgotoxy(cx[cobra->tam],cy[cobra->tam]); //apaga a ultima casa
            printf(" ");


            mgotoxy(cx[0],cy[0]); //desenha a cobra
            printf("%c",64);

            Colidiu(&tecla, cen, cobra, tuneis, cena, cx, cy); //testa a colisao e redesenha os tuneis
            Gera_tunel(cen, tuneis, cena);

            mgotoxy(0,0); //redesenha a interface no topo e volta para a posicao da cabeca
            Desenha_interface(*cobra, cena);
            mgotoxy(cobra->cabeca.x, cobra->cabeca.y);

            cx[0]=cobra->cabeca.x;
            cy[0]=cobra->cabeca.y;

            Sleep(cobra->velo);

      }
        if (tecla != 27)
          tecla=getch();

        if (tecla == -32)
            tecla=getch();
        switch(tecla)
            {
                case 72:
                        if(cobra->d != '3') //testa se está na direção oposta
                        cobra->d='1';//seta baixo
                        break;
                case 80:
                        if(cobra->d != '1') //testa se está na direção oposta
                        cobra->d='3';//seta cima
                        break;
                case 77:
                        if(cobra->d != '0') //testa se está na direção oposta
                        cobra->d='2';//seta direita
                        break;
                case 75:
                        if(cobra->d != '2') //testa se está na direção oposta
                        cobra->d='0';//seta esquerda
                        break;
            }



    }
}

void calcula_pontuacao (SNAKE *cobra, int vlritem)
{

    cobra->pont += vlritem*(cobra->tam/5)*(200 - cobra->velo)/10;//formula na definicao do trabalho(valor do item X tamanho/5 X velocidade)


}

/*{{03,04}, 0, '3', 1},//seta cima
 {{20,70}, 1, '0', 2},//seta esquerda
{{03,70}, 2, '1', 3},//seta baixo
{{20,04}, 3, '2', 4},//seta direita
{{12,40}, 4, '0', 0}*/

int main()
{
    int h;
    int n, i;
    SNAKE cobra = {{7,7}, 5, 190, 0, '2'};

    TUNEL tuneis2[NTUNEL] ={
                            {{03,04}, 0, '1', 1},//tuneis para o cenario 2
                            {{20,70}, 1, '3', 0},
                            };
    TUNEL tuneis[NTUNEL] = {
                            {{03,04}, 0, '0', 1},//tuneis para o cenario 3
                            {{20,70}, 1, '2', 3},
                            {{03,70}, 2, '2', 4},
                            {{20,04}, 3, '0', 2},
                            {{12,40}, 4, '2', 0}
                           };

    char cen1[25][81] ={"1111111111111111111111111111111111111111111111111111111111111111111111111111111",
                        "1111111111111111111111111111111111111111111111111111111111111111111111111111111",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1111111111111111111111111111111111111111111111111111111111111111111111111111111"};

    char cen2[25][81] ={"1111111111111111111111111111111111111111111111111111111111111111111111111111111",
                        "1111111111111111111111111111111111111111111111111111111111111111111111111111111",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1111111111111111111111111111111111111111111111111111111111111111111111111111111"};

    char cen3 [25][81]={"1111111111111111111111111111111111111111111111111111111111111111111111111111111",
                        "1111111111111111111111111111111111111111111111111111111111111111111111111111111",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1111111111111111111111111111111111111111111111111111111111111111111111111111111",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000000000000000000000000000000000000000000001",
                        "1111111111111111111111111111111111111111111111111111111111111111111111111111111",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1000000000000000000000000000000000000001100000000000000000000000000000000000001",
                        "1111111111111111111111111111111111111111111111111111111111111111111111111111111"};

    srand(time(0));


    printf("Escolha o cenario (1-3):");
    scanf("%d", &n);

    mgotoxy(0,0);
    Desenha_interface(cobra, n);
    Desenha_cenario(n);


        switch(n)
        {
            case 1:
                for(h=0;h<10;h++)
                    gera_item(cen1);
                break;
            case 2:Gera_tunel(cen2, tuneis2, n);
                    for(h=0;h<10;h++)
                    gera_item(cen2);


                break;
            case 3:Gera_tunel(cen3, tuneis, n);
                    for(h=0;h<10;h++)
                    gera_item(cen3);
                break;
        }

    switch(n)
      {
          case 1: Movimentacao(n, cen1, &cobra, tuneis);
              break;
          case 2: Movimentacao(n, cen2, &cobra, tuneis2);
              break;
          case 3: Movimentacao(n, cen3, &cobra, tuneis);
              break;
        }

    system("cls");
    Score(cobra.pont);



    return(0);
}
