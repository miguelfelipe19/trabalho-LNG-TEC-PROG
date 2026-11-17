/*
 * ============================================================
 *  BATALHA NAVAL (versao de 1 jogador contra o computador)
 * ============================================================
 *  Resumo do que o programa faz:
 *   - Cria um tabuleiro 8x8 e sorteia a posicao de 3 navios
 *     (tamanhos 3, 2 e 2 = 7 casas de navio no total).
 *   - O jogador digita coordenadas (ex.: A 3) para atirar.
 *   - Ele tem no maximo 30 tentativas para afundar todos os navios.
 *   - Se acabarem as tentativas, aparece uma tela de derrota.
 *   - No fim, o resultado da partida e salvo em "historico.txt",
 *     que pode ser consultado pelo menu.
 */

/* ---------- BIBLIOTECAS ---------- */
#include <stdio.h>    /* printf, fgets, sscanf, fopen, fprintf, fclose... */
#include <stdlib.h>   /* rand() e srand() para sortear posicoes; system() para limpar a tela */
#include <time.h>     /* time() para criar a "semente" do sorteio */

/* ---------- CONSTANTES ----------
 * Usar #define evita "numeros magicos" no codigo: se quiser um tabuleiro
 * maior ou mais navios, basta mudar aqui.
 */
#define TAMANHO_TABULEIRO 8    /* tabuleiro 8x8*/
#define QUANTIDADE_NAVIOS 3    /* quantos navios existem no jogo */
#define MAX_TENTATIVAS 30      /* limite de tiros do jogador */

/* ---------- ESTRUTURAS DE DADOS ---------- */

/* guarda as infos de UM navio */
typedef struct {
    int tamanho;    /* quantas casas o navio ocupa */
    int acertos;    /* quantas dessas casas ja foram atingidas */
    int afundado;   /* 0 = ainda flutuando, 1 = afundado */
} Navio;

/* Guarda TUDO que pertence a uma partida. Agrupar tudo em uma struct
 * facilita passar o jogo inteiro para as funcoes por ponteiro (Jogo *).
 */
typedef struct {
    /* Tabuleiro "secreto": '~' = agua, 'N' = navio, 'x' = navio ja atingido.
     * O jogador NUNCA ve esta matriz. */
    char tabuleiro [TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];

    /* Tabuleiro "visivel" ao jogador: ' ' = nao tentado, 'O' = agua,
     * 'X' = acertou navio. E este que e desenhado na tela. */
    char tiros [TAMANHO_TABULEIRO] [TAMANHO_TABULEIRO];

    /* Para cada casa, guarda QUAL navio esta nela (indice no vetor navios)
     * ou -1 se nao tem navio. Assim, ao acertar, sabemos qual navio foi
     * atingido e podemos contar os acertos dele. */
    int navioNaCasa [TAMANHO_TABULEIRO] [TAMANHO_TABULEIRO];

    Navio navios [QUANTIDADE_NAVIOS];   /* os 3 navios da partida */
    int tentativas;                     /* quantos tiros o jogador ja deu */
    int acertos;                        /* quantos tiros acertaram navio */
} Jogo;

/* ============================================================
 *  inicializarJogo
 *  Deixa a partida "zerada": tabuleiros vazios, contadores em 0
 *  e os navios com seus tamanhos definidos.
 *  Recebe um PONTEIRO (Jogo *) para modificar a struct original,
 *  e nao uma copia dela.
 * ============================================================ */
void inicializarJogo(Jogo *jogo) {
    int linha;
    int coluna;
    int indice;
    int tamanhos[QUANTIDADE_NAVIOS] = {3, 2, 2};   /* tamanho de cada navio */

    jogo->tentativas = 0;
    jogo->acertos = 0;

    /* Percorre todas as casas do tabuleiro (linha por linha) */
    for (linha = 0; linha < TAMANHO_TABULEIRO; linha++) {
        for (coluna = 0; coluna < TAMANHO_TABULEIRO; coluna++) {
            jogo->tabuleiro[linha][coluna] = '~';    /* tudo comeca como agua */
            jogo->tiros[linha][coluna] = ' ';        /* nenhum tiro dado ainda */
            jogo->navioNaCasa[linha][coluna] = -1;   /* nenhum navio na casa */
        }
    }

    /* Configura cada navio: tamanho vindo do vetor acima, sem acertos
     * e ainda flutuando. */
    for (indice = 0; indice < QUANTIDADE_NAVIOS; indice++) {
        jogo->navios[indice].tamanho = tamanhos[indice];
        jogo->navios[indice].acertos = 0;
        jogo->navios[indice].afundado = 0;
    }
}

/* ============================================================
 *  posicionarNavios
 *  Sorteia onde cada navio vai ficar. Para cada navio, repete
 *  sorteios ate achar uma posicao valida (dentro do tabuleiro
 *  e sem bater em outro navio).
 * ============================================================ */
void posicionarNavios(Jogo *jogo) {
    int indice;       /* qual navio estamos posicionando */
    int tamanho;      /* tamanho desse navio */
    int linha;        /* linha sorteada (ponto de partida) */
    int coluna;       /* coluna sorteada (ponto de partida) */
    int horizontal;   /* 1 = horizontal, 0 = vertical */
    int cabe;         /* 1 = posicao valida, 0 = invalida */
    int posicao;      /* contador para percorrer as casas do navio */

    for (indice = 0; indice < QUANTIDADE_NAVIOS; indice++) {
        tamanho = jogo->navios[indice].tamanho;
        cabe = 0;

        /* Fica sorteando ate encontrar uma posicao em que o navio caiba */
        while (!cabe) {
            horizontal = rand() % 2;                    /* 0 ou 1 */
            linha = rand() % TAMANHO_TABULEIRO;         /* 0 a 7 */
            coluna = rand() % TAMANHO_TABULEIRO;        /* 0 a 7 */

            if (horizontal && coluna + tamanho <= TAMANHO_TABULEIRO) {
                /* Horizontal: o navio ocupa (linha, coluna), (linha, coluna+1)...
                 * A condicao acima garante que ele nao ultrapassa a borda direita.
                 * Assume que cabe e so desmarca se encontrar outro navio. */
                cabe = 1;
                for (posicao = 0; posicao < tamanho; posicao++) {
                    if (jogo->tabuleiro[linha][coluna + posicao] == 'N') {
                        cabe = 0;   /* ja tem navio aqui: sorteia de novo */
                    }
                }
            } else if (!horizontal && linha + tamanho <= TAMANHO_TABULEIRO) {
                /* Vertical: mesma ideia, mas descendo as linhas.
                 * Garante que nao ultrapassa a borda de baixo. */
                cabe = 1;
                for (posicao = 0; posicao < tamanho; posicao++) {
                    if (jogo->tabuleiro[linha + posicao][coluna] == 'N') {
                        cabe = 0;
                    }
                }
            }
            /* Se nao entrou em nenhum if (passou da borda), cabe continua 0
             * e o while sorteia novamente. */
        }

        /* Posicao valida encontrada: agora "desenha" o navio de verdade,
         * marcando 'N' no tabuleiro e registrando qual navio ocupa cada casa. */
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

/* ============================================================
 *  mostrarApresentacao
 *  Tela de abertura com o titulo e um desenho de navio em ASCII.
 *  Espera o jogador apertar ENTER com getchar().
 *  Obs.: "\\" no printf imprime uma barra "\" (a barra e um
 *  caractere especial em C, por isso precisa ser "escapada").
 * ============================================================ */
void mostrarApresentacao(void) {
    printf("\n");
    printf("  ==========================================\n");
    printf("  ||                                      ||\n");
    printf("  ||          BATALHA   NAVAL             ||\n");
    printf("  ||                                      ||\n");
    printf("  ==========================================\n");
    printf("\n");
    printf("                   |\\\n");
    printf("                   | \\\n");
    printf("              _____|__\\_____\n");
    printf("              \\___________/\n");
    printf("      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("\n");
    printf("   Projeto de Linguagem e Tecnicas de Programacao\n");
    printf("\n   Pressione ENTER para comecar...");
    getchar();   /* pausa ate o usuario apertar ENTER */
}

/* ============================================================
 *  mostrarTabuleiro
 *  Desenha na tela o tabuleiro que o jogador enxerga (a matriz
 *  "tiros"), com letras nas linhas (A-H) e numeros nas colunas (1-8).
 *  Os navios NAO aparecem ate serem atingidos.
 * ============================================================ */
void mostrarTabuleiro(Jogo *jogo) {
    int linha;
    int coluna;
    char casa;

    printf("\n       1 2 3 4 5 6 7 8\n");     /* cabecalho das colunas */
    printf("     -------------------\n");
    for (linha = 0; linha < TAMANHO_TABULEIRO; linha++) {
        /* 'A' + linha converte 0->'A', 1->'B', ... (tabela ASCII) */
        printf("   %c | ", 'A' + linha);
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

/* ============================================================
 *  realizarAtaque
 *  Processa UM tiro na casa (linha, coluna).
 *  Retorna 1 se acertou um navio e 0 se foi agua.
 * ============================================================ */
int realizarAtaque(Jogo *jogo, int linha, int coluna) {
    int indice;

    jogo->tentativas++;   /* todo tiro conta como tentativa, acertando ou nao */

    if (jogo->tabuleiro[linha][coluna] == 'N') {
        /* ACERTOU: descobre qual navio estava na casa */
        indice = jogo->navioNaCasa[linha][coluna];
        jogo->tiros[linha][coluna] = 'X';       /* mostra 'X' para o jogador */
        jogo->tabuleiro[linha][coluna] = 'x';   /* marca internamente como atingido
                                                   (assim a casa nao e mais 'N') */
        jogo->acertos++;                        /* acertos totais da partida */
        jogo->navios[indice].acertos++;         /* acertos DESTE navio */

        /* Se o navio ja recebeu tantos acertos quanto o seu tamanho, afundou */
        if (jogo->navios[indice].acertos == jogo->navios[indice].tamanho) {
            jogo->navios[indice].afundado = 1;
            printf("\n >>> BOOM! Voce afundou um navio! <<<\n");
        } else {
            printf("\n >>> Acertou um navio! <<<\n");
        }
        return 1;
    }

    /* ERROU: marca 'O' (agua) no tabuleiro visivel */
    jogo->tiros[linha][coluna] = 'O';
    printf("\n >>> Agua! <<<\n");
    return 0;
}

/* ============================================================
 *  todosNaviosAfundados
 *  Verifica a condicao de vitoria. Retorna 1 se TODOS os navios
 *  estao afundados e 0 se ainda resta pelo menos um.
 * ============================================================ */
int todosNaviosAfundados(Jogo *jogo) {
    int indice;
    for (indice = 0; indice < QUANTIDADE_NAVIOS; indice++) {
        if (jogo->navios[indice].afundado == 0) {
            return 0;   /* achou um navio vivo: ainda nao venceu */
        }
    }
    return 1;           /* nenhum navio vivo: vitoria */
}

/* ============================================================
 *  limparTerminal
 *  Limpa a tela do terminal. Cada sistema tem um comando diferente:
 *  "cls" no Windows e "clear" no Linux/macOS. O #ifdef escolhe qual
 *  usar na hora de compilar (_WIN32 so existe no Windows).
 * ============================================================ */
void limparTerminal(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/* ============================================================
 *  mostrarTelaDerrota
 *  Tela exibida quando acabam todas as tentativas: mostra um navio
 *  afundando, quantos acertos o jogador fez e espera o ENTER para
 *  voltar ao menu.
 * ============================================================ */
void mostrarTelaDerrota(Jogo *jogo) {
    char entrada[80];   /* so para "engolir" o ENTER digitado */

    limparTerminal();   /* apaga a tela do jogo antes de mostrar a derrota */
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

    fgets(entrada, sizeof(entrada), stdin);   /* pausa ate o ENTER */
    limparTerminal();                         /* limpa a tela para mostrar o menu */
}

/* ============================================================
 *  jogar
 *  Controla UMA partida inteira: prepara o jogo, repete o ciclo
 *  "mostrar tabuleiro -> ler tiro -> validar -> atacar" e, no fim,
 *  grava no arquivo de historico e mostra o resultado.
 * ============================================================ */
void jogar(void) {
    Jogo jogo;              /* a partida (variavel local: some ao fim da funcao) */
    char entrada[80];       /* texto digitado pelo jogador */
    char linhaLetra;        /* letra da linha digitada (A-H) */
    int coluna;             /* numero da coluna digitado (1-8) */
    int linha;              /* linha convertida para indice (0-7) */
    int lidos;              /* quantos valores o sscanf conseguiu ler */
    int resultado;          /* retorno do ataque (1 = acertou, 0 = agua) */
    int venceu;             /* 1 se o jogador ganhou */
    FILE *arquivo;          /* ponteiro para o arquivo de historico */

    /* Semente do sorteio = hora atual. Sem isso, rand() geraria sempre
     * a mesma sequencia e os navios ficariam sempre no mesmo lugar. */
    srand((unsigned int)time(NULL));
    inicializarJogo(&jogo);     /* zera tudo */
    posicionarNavios(&jogo);    /* sorteia os navios */
    venceu = 0;

    printf("\n==============================================\n");
    printf("  BATALHA NAVAL\n");
    printf("  Encontre os 7 pedacos de navio em ate %d tentativas.\n", MAX_TENTATIVAS);
    printf("==============================================\n");

    /* LOOP PRINCIPAL: continua enquanto ainda ha tentativas E
     * ainda ha navios para afundar. */
    while (jogo.tentativas < MAX_TENTATIVAS && !todosNaviosAfundados(&jogo)) {
        mostrarTabuleiro(&jogo);
        printf("\n Tentativa %d de %d. Digite linha e coluna (ex.: A 3): ",
               jogo.tentativas + 1, MAX_TENTATIVAS);

        /* fgets le a linha inteira com seguranca (limita ao tamanho do
         * buffer). Retorna NULL se a entrada acabou (ex.: Ctrl+D). */
        if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
            printf("Entrada encerrada.\n");
            return;
        }

        /* sscanf extrai uma letra e um inteiro do texto digitado.
         * O espaco antes de %c ignora espacos em branco iniciais.
         * Retorna quantos valores leu: se nao foi 2, a entrada e invalida. */
        lidos = sscanf(entrada, " %c %d", &linhaLetra, &coluna);
        if (lidos != 2) {
            printf("\n Entrada invalida. Use uma letra de A a H e um numero de 1 a 8.\n");
            continue;   /* volta ao inicio do while SEM gastar tentativa */
        }

        /* Aceita letra minuscula: converte 'a'-'h' para 'A'-'H' */
        if (linhaLetra >= 'a' && linhaLetra <= 'h') {
            linhaLetra = (char)(linhaLetra - 'a' + 'A');
        }

        /* Converte para indices de matriz (comecam em 0):
         * 'A' -> 0, 'B' -> 1...   e   coluna 1 -> 0, 2 -> 1... */
        linha = linhaLetra - 'A';
        coluna--;

        /* Valida se a coordenada esta dentro do tabuleiro */
        if (linha < 0 || linha >= TAMANHO_TABULEIRO ||
            coluna < 0 || coluna >= TAMANHO_TABULEIRO) {
            printf("\n Coordenada fora do tabuleiro. Tente novamente.\n");
            continue;
        }

        /* Impede atirar duas vezes na mesma casa (nao gasta tentativa) */
        if (jogo.tiros[linha][coluna] != ' ') {
            printf("\n Voce ja tentou essa casa. Escolha outra.\n");
            continue;
        }

        /* Entrada valida: executa o tiro */
        resultado = realizarAtaque(&jogo, linha, coluna);
        if (resultado) {
            printf(" Total de acertos: %d de 7\n", jogo.acertos);
        }
    }

    /* ---------- FIM DA PARTIDA ---------- */
    venceu = todosNaviosAfundados(&jogo);

    /* Grava o resultado no historico ANTES de mostrar a tela final
     * (assim ele e salvo tanto na vitoria quanto na derrota).
     * Modo "a" (append) = acrescenta no final sem apagar o que ja existe;
     * cria o arquivo se ele ainda nao existir. */
    arquivo = fopen("historico.txt", "a");
    if (arquivo != NULL) {
        /* O operador ternario (venceu ? "vitoria" : "fim de jogo") escolhe
         * o texto conforme o valor de venceu. */
        fprintf(arquivo, "Resultado: %s | Tentativas: %d | Acertos: %d\n",
                venceu ? "vitoria" : "fim de jogo", jogo.tentativas, jogo.acertos);
        fclose(arquivo);   /* sempre fechar o arquivo para garantir a gravacao */
    }

    if (venceu) {
        /* VITORIA: mostra o tabuleiro final e a mensagem de parabens */
        mostrarTabuleiro(&jogo);
        printf("\n==============================================\n");
        printf("  PARABENS! Voce venceu em %d tentativas!\n", jogo.tentativas);
        printf("==============================================\n");
        if (arquivo == NULL) {
            printf("Aviso: nao foi possivel abrir o arquivo historico.txt.\n");
        }
    } else {
        /* DERROTA: mostra a tela de derrota, limpa o terminal e volta ao menu */
        mostrarTelaDerrota(&jogo);
    }
}

/* ============================================================
 *  mostrarHistorico
 *  Le o arquivo "historico.txt" e imprime cada partida salva.
 * ============================================================ */
void mostrarHistorico(void) {
    FILE *arquivo;
    char linha[160];    /* buffer para uma linha do arquivo */
    int encontrou;      /* indica se leu pelo menos uma linha */

    arquivo = fopen("historico.txt", "r");   /* "r" = somente leitura */
    if (arquivo == NULL) {
        /* Arquivo nao existe: nenhuma partida foi jogada ainda */
        printf("\n Ainda nao ha partidas salvas.\n");
        return;
    }

    encontrou = 0;
    printf("\n==============================================\n");
    printf("  HISTORICO DE PARTIDAS\n");
    printf("==============================================\n");

    /* Le linha por linha ate o fim do arquivo (fgets retorna NULL) */
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        printf(" %s", linha);
        encontrou = 1;
    }
    if (!encontrou) {
        printf(" O historico esta vazio.\n");   /* arquivo existe, mas sem linhas */
    }
    fclose(arquivo);
}

/* ============================================================
 *  main
 *  Ponto de entrada: mostra a tela de abertura uma vez e depois
 *  o menu em loop ate o usuario escolher sair (opcao 0).
 * ============================================================ */
int main(void) {
    char entrada[80];   /* texto digitado no menu */
    int opcao;          /* opcao escolhida */

    mostrarApresentacao();   /* tela de abertura (so uma vez, fora do loop) */

    do {
        /* Desenha o menu */
        printf("\n==============================\n");
        printf("       BATALHA NAVAL\n");
        printf("==============================\n");
        printf("  1 - Jogar\n");
        printf("  2 - Ver historico\n");
        printf("  0 - Sair\n");
        printf("==============================\n");
        printf("Escolha: ");

        /* Le a opcao como texto (mais seguro que scanf direto) e depois
         * converte para numero. Se nao for um numero, usa -1, que cai
         * no "Opcao invalida" do switch. */
        if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
            break;   /* entrada encerrada: sai do programa */
        }
        if (sscanf(entrada, "%d", &opcao) != 1) {
            opcao = -1;
        }

        /* Executa a acao correspondente a opcao escolhida */
        switch (opcao) {
            case 1: jogar(); break;
            case 2: mostrarHistorico(); break;
            case 0: printf("\nAte a proxima, capitao!\n"); break;
            default: printf("\nOpcao invalida.\n");
        }
    } while (opcao != 0);   /* repete o menu ate escolher 0 */

    return 0;   /* 0 = programa terminou sem erros */
}
