#ifndef HISTORICO_H
#define HISTORICO_H

#include "jogo.h"

#define ARQUIVO_HISTORICO "historico.txt"

/* grava o resultado da partida no arquivo; devolve 1 se salvou, 0 se deu erro */
int salvarPartida(const Jogo *jogo, int venceu);

/* mostra todas as partidas gravadas */
void mostrarHistorico(void);

#endif
