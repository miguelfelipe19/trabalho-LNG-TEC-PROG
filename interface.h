#ifndef INTERFACE_H
#define INTERFACE_H

#include "jogo.h"

/* cores ANSI (terminal do Linux, macOS e Windows 10+) */
#define COR_RESET    "\033[0m"
#define COR_NEGRITO  "\033[1m"
#define COR_CINZA    "\033[90m"
#define COR_VERMELHO "\033[91m"
#define COR_VERDE    "\033[92m"
#define COR_AMARELO  "\033[93m"
#define COR_AZUL     "\033[94m"
#define COR_CIANO    "\033[96m"
#define COR_BRANCO   "\033[97m"

/* utilidades de terminal */
void prepararTerminal(void);
void limparTela(void);
void esperar(int milissegundos);
int lerLinha(char *destino, int tamanho);
void pausar(void);

/* telas */
void mostrarApresentacao(void);
void mostrarMenu(void);
void mostrarMenuDificuldade(void);
void mostrarComoJogar(void);
void mostrarDespedida(void);
void mostrarCabecalho(const char *titulo);

/* tela da partida */
void mostrarTabuleiro(const Jogo *jogo, int revelar);
void mostrarPainel(const Jogo *jogo);
void mostrarResultado(const Jogo *jogo, int venceu);

#endif
