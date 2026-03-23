# Maze-Stalker-C
Engine de salas e labirinto em C com IA de perseguição adaptativa e aritmética de ponteiros.

Este projeto consiste em um protótipo de jogo para terminal desenvolvido em C, focado em lógica de programação, aritmética de ponteiros e algoritmos de decisão para inteligência artificial.

Descrição do Projeto
O Maze Stalker C foi concebido originalmente para auxiliar mestres de mesas de RPG no controle de masmorras. O sistema gera um andar com 16 salas procedurais, onde o jogador deve encontrar a saída enquanto é perseguido por uma entidade persistente.

Diferenciais Técnicos
Movimentação por Perspectiva: Implementação de funções de espelhamento de direção para simular a mudança de perspectiva do jogador ao transitar entre salas.

IA Adaptativa: A entidade toma decisões baseadas na distância Euclidiana e linear em relação ao jogador, alternando entre movimentos aleatórios, perseguição agressiva e teleporte estratégico.

Gerenciamento de Memória: Uso intensivo de structs e aritmética de ponteiros para manipulação direta da matriz de salas na memória RAM.

Estabilidade (v1.2): Correção de bugs de segmentação e persistência de dados através de limpeza de estado a cada nova campanha e acesso seguro via índices fixos.

Como Executar
Certifique-se de ter um compilador C (como GCC) instalado.

Clone o repositório.

Compile o arquivo.

Execute o binário gerado.

Controles do Jogo
1-4: Seleção de direção (Norte, Leste, Sul, Oeste).
