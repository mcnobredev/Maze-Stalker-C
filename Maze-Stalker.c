#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h> // Adicionado para o rand() variar de verdade

#define VAZIO 0
#define JOGADORES 1
#define ENTIDADE 2

typedef struct{
	int numero; 
	int direcao; 
	int anterior; 
	int estado; 
} Salas;

int TransformaAnterior(int direcao){
	return direcao > 2 ? direcao - 2 : direcao + 2;
} 

int ProximaDirecao(int Anterior){
	int Proxima = (rand() % 4) + 1;
	if(Proxima == Anterior){
		return TransformaAnterior(Proxima);
	}
	return Proxima;
}

void defPrimeiroAndar(Salas *PA){ 
	int Entidade, Anterior = 0;
	srand(time(NULL)); // Garante que cada mapa seja diferente do anterior
	
	Entidade = (rand() % 15) + 1;                         
	
	for(int i = 0; i < 16; i++){
		if(i == 0){
			PA->estado = JOGADORES;
		} else if(i == Entidade){
			PA->estado = ENTIDADE;
		} else {
			PA->estado = VAZIO; // Aqui ele limpa qualquer lixo de campanhas passadas
		}
		
		PA->anterior = Anterior;
		PA->direcao = ProximaDirecao(Anterior);
		Anterior = TransformaAnterior(PA->direcao);
		PA->numero = i + 1;
		PA++;
	}
}                                               

char TraduzCasa(int estado){
	return estado == JOGADORES ? 'J' : estado == ENTIDADE ? 'E' : estado == JOGADORES + ENTIDADE ? 'A' : ' '; 
} 

void MostraMapa(Salas *PA){
	int sequencia[16];
	for(int i = 0; i < 16; i++){
		if(i > 0 && i % 4 == 0) printf("\n");
		printf(" [ %c ] ", TraduzCasa(PA->estado));
		sequencia[i] = PA->direcao;
		PA++;
	}
	printf("\n");
	for(int i = 0; i < 16; i++) printf("[%d] ", sequencia[i]);
}

int AchaPosicaoJogadores(Salas *PA){
	int i = 0;
	// Trava de segurança: i < 16 impede o loop infinito se o jogador "sumir"
	while((PA->estado != 1 && PA->estado != 3) && i < 16){
		PA++;
		i++;
	}
	return (i >= 16) ? 1 : PA->numero; // Se não achar, assume que está na 1 para não quebrar
}

void MovimentacaoJogadores(Salas *PA){
	int Direcao;
	Salas *pInicial = PA;
	
	int pos = AchaPosicaoJogadores(pInicial);
	while(PA->numero != pos) PA++;
	
	printf("\nDirecao escolhida: ");
	scanf("%d", &Direcao);
	
	if(Direcao == PA->direcao){
		PA->estado -= JOGADORES;
		PA++;
		PA->estado += JOGADORES;
	} else if(Direcao == PA->anterior){
		PA->estado -= JOGADORES;
		if(PA->numero > 1) PA--;
		PA->estado += JOGADORES;
	} else {
		PA->estado -= JOGADORES;
		while(PA->anterior != 0) PA--;
		PA->estado += JOGADORES;
	}
}

int VerificaDisponibilidade(Salas *PA, int Valor, int Sentido){
	int Disponiveis = 0;
	Salas *pTemp = PA;
	if(Sentido){
		while(pTemp->anterior != 0 && Disponiveis < Valor){
			Disponiveis++;
			pTemp--;
		}
	} else {
		while(pTemp->numero < 16 && Disponiveis < Valor){
			Disponiveis++;
			pTemp++;
		}
	}
	return (Disponiveis == Valor) ? Valor : 0;
}

void MovimentacaoEntidade(Salas *PA, int Direcao){
	int pulo = 0;
	if(Direcao == 1) pulo = -VerificaDisponibilidade(PA, 4, 1);
	else if(Direcao == 2) pulo = VerificaDisponibilidade(PA, 1, 0);
	else if(Direcao == 3) pulo = VerificaDisponibilidade(PA, 4, 0);
	else if(Direcao == 4) pulo = -VerificaDisponibilidade(PA, 1, 1);

	if(pulo != 0){
		PA->estado -= ENTIDADE;
		(PA + pulo)->estado += ENTIDADE;
	}
}

int AchaPosicaoEntidade(Salas *PA){
	int i = 0;
	while(PA->estado < 2 && i < 16){
		PA++;
		i++;
	}
	return (i >= 16) ? -1 : PA->numero; // Retorna -1 se ela sumiu (para ativar o teleporte)
}

int DecisaoEntidade(Salas *PA, int Movimentou){
	int SalaEntidade = AchaPosicaoEntidade(PA);
	int SalaJogadores = AchaPosicaoJogadores(PA);
	int opcao;
	Salas *SalaInicial = PA;
	Salas *SalaAtual;

	// Se ela sumiu, forçamos o teleporte (Movimentou = 0)
	if(SalaEntidade == -1) Movimentou = 0;

	if(Movimentou){
		while(PA->numero != SalaEntidade) PA++;
		SalaAtual = PA;

		if(abs(SalaEntidade - SalaJogadores) >= 10){
			if(SalaEntidade > SalaJogadores) MovimentacaoEntidade(SalaAtual, 1);
			else MovimentacaoEntidade(SalaAtual, 3);
		} else if(abs(SalaEntidade - SalaJogadores) > 5){
			opcao = (rand() % 5) + 1;
			if(opcao <= 3){
				if(rand() % 3 > 0){ // Horizontal
					if(SalaEntidade > SalaJogadores) MovimentacaoEntidade(SalaAtual, 1);
					else MovimentacaoEntidade(SalaAtual, 3);
				} else { // Vertical
					if(SalaEntidade > SalaJogadores) MovimentacaoEntidade(SalaAtual, 4);
					else MovimentacaoEntidade(SalaAtual, 2);
				}
			} else {
				MovimentacaoEntidade(SalaAtual, (rand() % 4) + 1);
			}
		} else {
			if(rand() % 2){
				if(rand() % 3 > 1){ 
					if(SalaEntidade > SalaJogadores) MovimentacaoEntidade(SalaAtual, 1);
					else MovimentacaoEntidade(SalaAtual, 3);
				} else {
					if(SalaEntidade > SalaJogadores) MovimentacaoEntidade(SalaAtual, 4);
					else MovimentacaoEntidade(SalaAtual, 2);
				}
			} else {
				MovimentacaoEntidade(SalaAtual, (rand() % 4) + 1);
			}
		}
	} else {
		/* TELEPORTE SEGURO: Usando SalaInicial + índice */
		if(SalaEntidade != -1){
			while(PA->numero != SalaEntidade) PA++;
			PA->estado -= ENTIDADE;
		}
		
		int alvo;
		opcao = rand() % 2;
		if(opcao){
			alvo = (SalaJogadores < 14) ? (SalaJogadores + 2) : (SalaJogadores - 2);
		} else {
			alvo = (SalaJogadores > 3) ? (SalaJogadores - 2) : (SalaJogadores + 2);
		}
		// Acesso direto via SalaInicial garante que não vaza da memória
		SalaInicial[alvo - 1].estado += ENTIDADE; 
	}
	
	// Verifica se ela conseguiu se mover para avisar o próximo turno
	int novaPos = AchaPosicaoEntidade(SalaInicial);
	return (novaPos == SalaEntidade && SalaEntidade != -1) ? 0 : 1;
} 

int Turno(Salas *PA, int ResultadoEntidade){ 
	system("cls");
	MostraMapa(PA);
	MovimentacaoJogadores(PA);
	return DecisaoEntidade(PA, ResultadoEntidade);
}

void Campanha(Salas *PA){
	int SalaJogadores = 0, ResultadoEntidade = 1;
	defPrimeiroAndar(PA); 
	while(SalaJogadores < 16){
		ResultadoEntidade = Turno(PA, ResultadoEntidade);
		SalaJogadores = AchaPosicaoJogadores(PA);
	}
	system("cls");
	printf("\nVoce terminou! Parabens!");
	getch(); // Espera tecla para não fechar o menu direto
}

int Menu(Salas *PA){
	int opt;
	system("cls");
	printf("Bem-vindo! Esse e o menu do Maze Stalker C");
	printf("\n[1] - Iniciar nova campanha \n[2] - Sair");
	printf("\nEscolha: ");
	if(scanf("%d", &opt) != 1) return 0;
	if(opt == 1) Campanha(PA);
	return opt;
}

int main() {
	Salas PrimeiroAndar[4][4];
	int opt = 0;
	do {
		opt = Menu(&PrimeiroAndar[0][0]);
	} while(opt != 2);
	return 0;
}