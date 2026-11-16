# 🚢 Batalha Naval em C (Terminal)

## 📌 Descrição

Este projeto é uma implementação simplificada do clássico jogo **Batalha Naval**, desenvolvido em linguagem C para execução no terminal.

O jogador deve tentar localizar e destruir todos os navios escondidos em um tabuleiro, informando coordenadas (linha e coluna). A cada tentativa, o sistema informa se o jogador acertou ou errou.

---

## 🎯 Objetivo

Aplicar conceitos fundamentais de programação em C, como:

* Estruturas condicionais (`if/else`)
* Laços de repetição (`while`, `for`)
* Matrizes bidimensionais
* Entrada e saída de dados (`scanf` / `printf`)
* Lógica de jogo

---

## 🕹️ Como funciona

* O jogo possui um tabuleiro (ex: 8x8)
* São 3 navios, de tamanhos 3, 2 e 2 (7 casas ao todo)
* Os navios são posicionados automaticamente
* O jogador tem no máximo 30 tentativas.
* O jogador informa coordenadas para atacar
* O sistema responde:

  * 💥 Acertou um navio
  * 🌊 Água (errou)
* O jogo termina quando:

  * Todos os navios são destruídos, ou
  * O número máximo de tentativas é atingido

---

## 🧱 Estrutura do Projeto

```
batalha-naval/
│
├── jogo.c
├── README.md
└── historico.txt   (gerado ao jogar)
```

---

## ⚙️ Como compilar e executar

### 🔹 Compilar:

```bash
gcc Jogo.c -o batalha
```

### 🔹 Executar:

```bash
./batalha(ou batalha.exe)
```

---

## 💡 Possíveis melhorias

* Modo multiplayer (2 jogadores)
* Interface mais organizada no terminal
* Diferentes tamanhos de navios
* Sistema de pontuação
* Dificuldade (fácil, médio, difícil)

---

## 👨‍💻 Autor

Projeto desenvolvido para fins acadêmicos, com foco em aprendizado da linguagem C.

