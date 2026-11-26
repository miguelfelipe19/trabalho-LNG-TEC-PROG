/* ===== PARTE 1: BIBLIOTECAS ===== */
#include <stdio.h>   /* mostrar e ler coisas na tela */
#include <stdlib.h>  /* sorteio e limpar tela */
#include <time.h>    /* pegar a hora (para o sorteio) */

/* ===== PARTE 2: VALORES FIXOS ===== */
#define TAMANHO_TABULEIRO 8    /* tabuleiro 8x8*/
#define QUANTIDADE_NAVIOS 3    /* numero de navios */
#define MAX_TENTATIVAS 30      /* maximo de tiros */

/* ===== PARTE 3: STRUCTS (guardam varias informacoes juntas) ===== */

/* guarda as infos de UM navio */
typedef struct {
    int tamanho;    /* quantas casas o navio ocupa */
    int acertos;    /* quantas dessas casas ja foram atingidas */
    int afundado;   /* 0 = ainda flutuando, 1 = afundado */
} Navio;

/* guarda tudo do jogo */
typedef struct {
    char tabuleiro [TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];   /* onde estao os navios (escondido) */
    char tiros [TAMANHO_TABULEIRO] [TAMANHO_TABULEIRO];      /* tiros dados (o jogador ve) */
    int navioNaCasa [TAMANHO_TABULEIRO] [TAMANHO_TABULEIRO]; /* qual navio esta em cada casa */
    Navio navios [QUANTIDADE_NAVIOS];                        /* lista de navios */
    int tentativas;                                          /* tiros dados */
    int acertos;                                             /* tiros certos */
} Jogo;

/* ===== PARTE 4: PREPARAR O JOGO ===== */
void inicializarJogo(Jogo *jogo) {
    int linha;
    int coluna;
    int indice;
    int tamanhos[QUANTIDADE_NAVIOS] = {3, 2, 2};   /* tamanho dos navios */

    /* zera os contadores */
    jogo->tentativas = 0;
    jogo->acertos = 0;

    /* limpa o tabuleiro todo */
    for (linha = 0; linha < TAMANHO_TABULEIRO; linha++) {
        for (coluna = 0; coluna < TAMANHO_TABULEIRO; coluna++) {
            jogo->tabuleiro[linha][coluna] = '~';   /* agua */
            jogo->tiros[linha][coluna] = ' ';       /* sem tiro */
            jogo->navioNaCasa[linha][coluna] = -1;  /* sem navio */
        }
    }

    /* prepara cada navio */
    for (indice = 0; indice < QUANTIDADE_NAVIOS; indice++) {
        jogo->navios[indice].tamanho = tamanhos[indice];
        jogo->navios[indice].acertos = 0;
        jogo->navios[indice].afundado = 0;
    }
}

/* ===== PARTE 5: COLOCAR OS NAVIOS NO TABULEIRO ===== */
void posicionarNavios(Jogo *jogo) {
    int indice;
    int tamanho;
    int linha;
    int coluna;
    int horizontal;
    int cabe;
    int posicao;

    /* faz para cada navio */
    for (indice = 0; indice < QUANTIDADE_NAVIOS; indice++) {
        tamanho = jogo->navios[indice].tamanho;
        cabe = 0;

        /* sorteia ate achar um lugar bom */
        while (!cabe) {
            horizontal = rand() % 2;                 /* sorteia a direcao */
            linha = rand() % TAMANHO_TABULEIRO;      /* sorteia a linha */
            coluna = rand() % TAMANHO_TABULEIRO;     /* sorteia a coluna */

            if (horizontal && coluna + tamanho <= TAMANHO_TABULEIRO) {
                /* na horizontal: ve se as casas estao livres */
                cabe = 1;
                for (posicao = 0; posicao < tamanho; posicao++) {
                    if (jogo->tabuleiro[linha][coluna + posicao] == 'N') {
                        cabe = 0;
                    }
                }
            } else if (!horizontal && linha + tamanho <= TAMANHO_TABULEIRO) {
                /* na vertical: ve se as casas estao livres */
                cabe = 1;
                for (posicao = 0; posicao < tamanho; posicao++) {
                    if (jogo->tabuleiro[linha + posicao][coluna] == 'N') {
                        cabe = 0;
                    }
                }
            }
        }

        /* coloca o navio no tabuleiro */
        for (posicao = 0; posicao < tamanho; posicao++) {
            if (horizontal) {
                jogo->tabuleiro[linha][coluna + posicao] = 'N';
                jogo->navioNaCasa[linha][coluna + posicao] = indice;
            } else {
                jogo->tabuleiro[linha + posicao][coluna] = 'N';
                jogo->navioNaCasa[linha + posicao][coluna] = indice;
            }
        }
    }
}

/* ===== PARTE 6: TELA INICIAL ===== */
void mostrarApresentacao(void) {
    printf("\n");
    printf("   ==========================================\n");
    printf("   ||                                      ||\n");
    printf("   ||          BATALHA   NAVAL             ||\n");
    printf("   ||                                      ||\n");
    printf("   ==========================================\n");
    printf("\n");         
    printf("\t         |    |    |\n");
    printf("\t        )_)  )_)  )_)\n");
    printf("\t       )___))___))___)\ \n");
    printf("\t      )____)____)_____)\\ \n");
    printf("\t     _____|____|____|____\\\__ \n");
    printf("\t-----\                   /----- \n");
    printf("\t  ^^^^^ ^^^^^^^^^^^^^^^^^^^^^ \n");
    printf("\t    ^^^^      ^^^^     ^^^    ^^ \n"); 
    printf("\t         ^^^^      ^^^           \n");
    printf("\n");
    printf("   Projeto de Linguagem e Tecnicas de Programacao\n");
    printf("\n   Pressione ENTER para comecar...");
    getchar();   /* espera o ENTER */
}

/* ===== PARTE 7: MOSTRAR O TABULEIRO ===== */
void mostrarTabuleiro(Jogo *jogo) {
    int linha;
    int coluna;
    char casa;

    printf("\n       1 2 3 4 5 6 7 8\n");          /* numeros das colunas */
    printf("     -------------------\n");
    for (linha = 0; linha < TAMANHO_TABULEIRO; linha++) {
        printf("   %c | ", 'A' + linha);          /* letra da linha */
        for (coluna = 0; coluna < TAMANHO_TABULEIRO; coluna++) {
            casa = jogo->tiros[linha][coluna];
            if (casa == ' ') {
                casa = '~';   /* casa ainda nao atacada */
            }
            printf("%c ", casa);
        }
        printf("|\n");
    }
    printf("     -------------------\n");
    printf("\n Legenda: ~ nao tentado | O agua | X navio atingido\n");
}

/* ===== PARTE 8: ATIRAR (retorna 1 se acertou, 0 se errou) ===== */
int realizarAtaque(Jogo *jogo, int linha, int coluna) {
    int indice;

    jogo->tentativas++;   /* conta o tiro */

    /* se tem navio na casa, acertou */
    if (jogo->tabuleiro[linha][coluna] == 'N') {
        indice = jogo->navioNaCasa[linha][coluna];   /* qual navio foi atingido */
        jogo->tiros[linha][coluna] = 'X';
        jogo->tabuleiro[linha][coluna] = 'x';
        jogo->acertos++;
        jogo->navios[indice].acertos++;

        /* se acertou todas as casas do navio, ele afundou */
        if (jogo->navios[indice].acertos == jogo->navios[indice].tamanho) {
            jogo->navios[indice].afundado = 1;
            printf("\n >>> BOOM! Voce afundou um navio! <<<\n");
        } else {
            printf("\n >>> Acertou um navio! <<<\n");
        }
        return 1;
    }

    /* se nao tem navio, errou */
    jogo->tiros[linha][coluna] = 'O';
    printf("\n >>> Agua! <<<\n");
    return 0;
}

/* ===== PARTE 9: VER SE TODOS OS NAVIOS AFUNDARAM ===== */
int todosNaviosAfundados(Jogo *jogo) {
    int indice;
    for (indice = 0; indice < QUANTIDADE_NAVIOS; indice++) {
        if (jogo->navios[indice].afundado == 0) {
            return 0;   /* ainda tem navio */
        }
    }
    return 1;           /* todos afundaram */
}

/* ===== PARTE 10: LIMPAR A TELA ===== */
/* limpa o terminal (funciona no Windows e em Linux/macOS) */
void limparTerminal(void) {
#ifdef _WIN32
    system("cls");     /* Windows */
#else
    system("clear");   /* Linux e macOS */
#endif
}

/* ===== PARTE 11: TELA DE DERROTA ===== */
/* tela exibida quando acabam todas as tentativas */
void mostrarTelaDerrota(Jogo *jogo) {
    char entrada[80];

    limparTerminal();
    printf("\n");
    printf("  ==========================================\n");
    printf("  ||                                      ||\n");
    printf("  ||            VOCE PERDEU!              ||\n");
    printf("  ||                                      ||\n");
    printf("  ==========================================\n");
    printf("\n");
    printf("                      |\n");
    printf("                      |   \\\n");
    printf("                  ____|____\\__\n");
    printf("                  \\__________/\n");
    printf("      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("        ~~~~~~~  GLUB  GLUB  ~~~~~~~~\n");
    printf("            ~~~~~~~~~~~~~~~~~~~~\n");
    printf("\n");
    printf("   Suas %d tentativas acabaram.\n", MAX_TENTATIVAS);
    printf("   Voce acertou %d de 7 pedacos de navio.\n", jogo->acertos);
    printf("   A frota inimiga escapou, capitao...\n");
    printf("\n   Pressione ENTER para voltar ao menu...");

    fgets(entrada, sizeof(entrada), stdin);   /* espera o ENTER */
    limparTerminal();
}

/* ===== PARTE 12: A PARTIDA ===== */
void jogar(void) {
    Jogo jogo;
    char entrada[80];
    char linhaLetra;
    int coluna;
    int linha;
    int lidos;
    int resultado;
    int venceu;
    FILE *arquivo;

    /* prepara o jogo */
    srand((unsigned int)time(NULL));   /* faz o sorteio mudar a cada partida */
    inicializarJogo(&jogo);
    posicionarNavios(&jogo);
    venceu = 0;

    printf("\n==============================================\n");
    printf("  BATALHA NAVAL\n");
    printf("  Encontre os 7 pedacos de navio em ate %d tentativas.\n", MAX_TENTATIVAS);
    printf("==============================================\n");

    /* repete ate acabar os tiros ou afundar todos os navios */
    while (jogo.tentativas < MAX_TENTATIVAS && !todosNaviosAfundados(&jogo)) {
        mostrarTabuleiro(&jogo);
        printf("\n Tentativa %d de %d. Digite linha e coluna (ex.: A 3): ",
               jogo.tentativas + 1, MAX_TENTATIVAS);

        /* le o que o jogador digitou */
        if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
            printf("Entrada encerrada.\n");
            return;
        }
        lidos = sscanf(entrada, " %c %d", &linhaLetra, &coluna);   /* separa a letra e o numero */
        if (lidos != 2) {
            printf("\n Entrada invalida. Use uma letra de A a H e um numero de 1 a 8.\n");
            continue;   /* volta ao inicio do loop */
        }

        /* transforma a letra e o numero em posicao do tabuleiro */
        if (linhaLetra >= 'a' && linhaLetra <= 'h') {
            linhaLetra = (char)(linhaLetra - 'a' + 'A');   /* minuscula vira maiuscula */
        }
        linha = linhaLetra - 'A';   /* A vira 0, B vira 1... */
        coluna--;                   /* 1 vira 0, 2 vira 1... */

        /* confere se a jogada e valida */
        if (linha < 0 || linha >= TAMANHO_TABULEIRO ||
            coluna < 0 || coluna >= TAMANHO_TABULEIRO) {
            printf("\n Coordenada fora do tabuleiro. Tente novamente.\n");
            continue;
        }
        if (jogo.tiros[linha][coluna] != ' ') {
            printf("\n Voce ja tentou essa casa. Escolha outra.\n");
            continue;
        }

        /* da o tiro */
        resultado = realizarAtaque(&jogo, linha, coluna);
        if (resultado) {
            printf(" Total de acertos: %d de 7\n", jogo.acertos);
        }
    }

    /* ve se o jogador ganhou */
    venceu = todosNaviosAfundados(&jogo);

    /* salva o resultado no historico antes de mostrar a tela final */
    arquivo = fopen("historico.txt", "a");   /* abre o arquivo para adicionar no final */
    if (arquivo != NULL) {
        fprintf(arquivo, "Resultado: %s | Tentativas: %d | Acertos: %d\n",
                venceu ? "vitoria" : "fim de jogo", jogo.tentativas, jogo.acertos);
        fclose(arquivo);   /* fecha o arquivo */
    }

    /* mostra a tela final */
    if (venceu) {
        mostrarTabuleiro(&jogo);
        printf("\n==============================================\n");
        printf("  PARABENS! Voce venceu em %d tentativas!\n", jogo.tentativas);
        printf("==============================================\n");
        if (arquivo == NULL) {
            printf("Aviso: nao foi possivel abrir o arquivo historico.txt.\n");
        }
    } else {
        /* derrota: mostra a tela de derrota, limpa o terminal e volta ao menu */
        mostrarTelaDerrota(&jogo);
    }
}

/* ===== PARTE 13: MOSTRAR O HISTORICO ===== */
void mostrarHistorico(void) {
    FILE *arquivo;
    char linha[160];
    int encontrou;

    arquivo = fopen("historico.txt", "r");   /* abre o arquivo para ler */
    if (arquivo == NULL) {
        printf("\n Ainda nao ha partidas salvas.\n");
        return;
    }

    encontrou = 0;
    printf("\n==============================================\n");
    printf("  HISTORICO DE PARTIDAS\n");
    printf("==============================================\n");
    /* le e mostra o arquivo linha por linha */
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        printf(" %s", linha);
        encontrou = 1;
    }
    if (!encontrou) {
        printf(" O historico esta vazio.\n");
    }
    fclose(arquivo);   /* fecha o arquivo */
}

/* ===== PARTE 14: PROGRAMA PRINCIPAL (MENU) ===== */
int main(void) {
    char entrada[80];
    int opcao;

    mostrarApresentacao();

    /* repete o menu ate o jogador escolher sair */
    do {
        printf("\n==============================\n");
        printf("       BATALHA NAVAL\n");
        printf("------------------------------\n");
        printf("Digite:                       \n");
        printf("  1 - Começar jogo \n");
        printf("  2 - Ver historico de partidas\n");
        printf("  0 - Sair\n");
        printf("==============================\n");
        printf("ESCOLHA( 1 | 2 | 0 ): ");

        /* le a opcao */
        if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
            break;
        }
        if (sscanf(entrada, "%d", &opcao) != 1) {
            opcao = -1;   /* nao digitou numero */
        }

        /* faz o que o jogador escolheu */
        switch (opcao) {
            case 1: jogar(); break;
            case 2: mostrarHistorico(); break;
            case 0: printf("\nAte a proxima, capitao!\n"); break;
            default: printf("\nOpcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}
