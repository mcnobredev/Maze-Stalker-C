#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h> 

#define VAZIO 0
#define JOGADORES 1
#define ENTIDADE 2

typedef struct{
	int numero; // para saber a posição da sala
	int direcao; // 1 a 4 : 1 - norte, 2 - leste, 3 - sul, 4 - oeste;
	int anterior; // vaiser a direção anterior, ou seja, ele seria a parte pra voltar a sala anterior | aqui há tambem o caso 0, que é onde não ha direção anterior;
	int estado; // soma das entidades presentes : 0 - Vazio, 1 - Jogadores, 2 - Entidade, 3 - Jogadores e Entidade;
} Salas;

int TransformaAnterior(int direcao){
	return direcao>2 ? direcao - 2 : direcao + 2;
} /* Basicamente essa função espelha a direção, porque quando se muda de sala, o centro dela faz com que as direções se invertam, por exemplo, uma pessoa que 
esteja na dua direita de enxerga como se voce que estivessa a esquerda dela, é questão de perspectiva, e como estaremos trabalhando a mudança de perspectiva mais
a frente, usaremos sim essa capacidade de espelha mento. Lembrando que 1 - norte, 2 - leste, 3 - sul, 4 - oeste, ou seja, o inverso de 1 é 3 e o inverso de 2 é 4
por isso o calculo trabalha do jeito que foi colocado */ 

int ProximaDirecao(int Anterior){
	int Proxima = (rand() % 4) + 1;
	if(Proxima == Anterior){
		return TransformaAnterior(Proxima);
	}
	return Proxima;
} /*Aqui parece confuso mas é tranquilo de pegar, primeiro ele define um valor da direção, depois ele verifica se o anterior também seria essa direção, se for,
ele inverte a direção pra evitar conflito, e se não for, retorna o valor criado originalmente*/

void defPrimeiroAndar(Salas *PA){ //PA = Primeiro Andar
	int Entidade, Anterior = 0; // a Anterior vai servir pra guardar o valor da ultima sala
	srand(time(NULL)); // Semente para garantir que o mapa mude a cada execução

	Entidade = (rand() % 15) + 1; /* escolhe aleatoriamente a posição da entdade entre as casas, agora, porque 15 + 1 e não logo 16? é 
	é simples, primeiramente, 15 é o numero que precisaremos porque a contagem no loop começa no 0, ou seja,
	mesmo que o numero maximo de casas seja 16, por começar no 0 o maior numero da sequencia será 15, agora, porque 15 + 1 e não 16? bom isso é porque na função
	para gerar os numeros aleatórios rand() quando definimos o range como 16, ele vai de 0 a 15, e no 0 colocaremos o jogador, como não queremos que o jogador
	comece na mesma sala que a entidade, fazemos com que mesmo que no rand() saia zero ele seja no minimo 1, porém, se for 15 a resposta, +1  tornaria ele em 16
	e não existe essa sala no mapa, portanto, precisamos que o numero maximo desça um no rand() para poder acrescentar +1 externamente */                         
	
	for(int i = 0; i<16; i++){
		//Definindo Estado*
		if(i==0){
			PA->estado = JOGADORES;
		} else if(i==Entidade){
			PA->estado = ENTIDADE;
		} else{
			PA->estado = VAZIO;
		}
		/* O que essa primeira parte do loop faz é bem simples, ele identifica qual sala esta sendo modificada, se for a primeira sala, coloca os jogadores
		se for a sala que foi designada para a entidade, coloca a entidade, caso não seja nenhuma das duas, estara vazia */
		
		//Definindo Direção*
		PA->anterior = Anterior;
		PA->direcao = ProximaDirecao(Anterior);
		// aqui é simples, define o valor anterior da sala como anterior guaradado na função, e escolhe um proximo valor para a direção;

		// Aqui armazena para o proximo loop qual é a casa anterior*
		Anterior = TransformaAnterior(PA->direcao);
		
		// Aqui define o numero da sala*
		PA->numero = i+1;
	
		// Vai para o proximo item*
		PA++;
	}
}                                               

char TraduzCasa(int estado){
	return estado == JOGADORES ? 'J' : estado == ENTIDADE ? 'E' : estado ==  JOGADORES + ENTIDADE ? 'A' : ' '; 
} /* Esse é bem obvio, mas mesmo assim vou explicar resumindo, se for jogador, ele retorna a primeira letra de Jogadores, o mesmo se tiver uma entidade, 
caso tenha os dois, ele retorna A de Ambos, por fim, caso não seja nenhum desses valores, ele retorna ' ' que representa o Vazio */ 

void MostraMapa(Salas *PA){
	int sequencia[16];// Guarda a sequencia pra mostrar depois
	
	// Printa mapa*
	for(int i = 0; i<16; i++){
		if(i>0 && i%4 == 0){
			printf("\n");
		}
		printf(" [ %c ] ", TraduzCasa(PA->estado));
		sequencia[i]=PA->direcao;
		PA++;
	} /* printa o mapa, pulando linha em todos os numeros diviziveis por 4 a cima de 0, mas porque? simples, aqui, como estamos lidando com matrizes, pensa que só
	temos 4 casas por fileira, ou seja a primeira fileira termina no 4, a segunda no 8, a terceira no 12, e a ultima no 16, porém começamos pelo 0, ou seja, a 
	primeira fileira termina no 3 e o 4 é o começo da segunda, então antes de printar o valor de 4, temos que pular uma linha, pra maior entendimento basta olhar 
	tabela a seguir: 
	00 01 02 03
	04 05 06 07
	08 09 10 11
	12 13 14 15
	Além de printar o mapa, ele também adiciona à sequencia a direção da sala atual. */
	
	//Pula linha*
	printf("\n");
	
	//Printa a sequencia*
	for(int i = 0; i<16; i++){
		printf("[%d] ", sequencia[i]);
	}
}

int AchaPosicaoJogadores(Salas *PA){
	int i = 0;
	while(PA->estado != 1 && PA->estado != 3 && i < 16){
		PA++;
		i++;
	}
	return (i >= 16) ? 1 : PA->numero;
}//Procura na matriz a posição dos jogadores, que é representada pelos estados 1 e 3 das salas, passando de casa em casa, e retornando o numero da casa que eles esta


void MovimentacaoJogadores(Salas *PA){
	int Direcao; // guarda a direcao escolhida
	Salas *pInicial = PA;
	
	int pos = AchaPosicaoJogadores(pInicial);
	while(PA->numero != pos) PA++;
	// acha a posição do jogador para poder manipula-la
	
	//Pede a direção do movimento*
	printf("\nDirecao escolhida: ");
	if(scanf("%d", &Direcao) != 1) return;
	
	//Analise de coerência e definição da sala*
	if(Direcao == PA->direcao){
		PA->estado -= JOGADORES;
		PA++;
		PA->estado += JOGADORES;
	} else if(Direcao == PA->anterior){
		PA->estado -= JOGADORES;
		if(PA->numero > 1) PA--;
		PA->estado += JOGADORES;
	} else{
		PA->estado -= JOGADORES;
		while(PA->anterior != 0){
			PA--;
		}
		PA->estado += JOGADORES;
	} /* Ta aqui a parte mais essencial de como vai ser a mecanica de movimentação no mapa, primeiro, a intenção aqui é fazer o jogador ter que seguir de forma 
	correta as direções estipuladas de cada sala, ele pode ou ir para a proxima, ou voltar, caso eles escolham a sala errada, voltam para a primeira, que é 
	marcada justamente por não ter anterior, ou mais preciso, anterior == 0. 
	Para executar isso eu fiz as condicionais, onde sempre reduzimos do estado da sala original o jogador, depois colocamos ele na sala de acordo com a escolha,
	e adicionamos ao estado da sala o jogador, isso faz com que não apaguemos a entidade sem querer tornando o estado em 0 manualmente, além disso, o loop criado
	só existe porque a casa que é colcoada na função sempre é a que o jogador ja esta atualmente, mas poderia ser feito de maneiras diferentes, por exemplo, 
	pedindo na função a posição do jogador e o inicio da fila, é possivel, mas eu preferi fazer dessa maneira porque o loop é pequeno. */
	
}

int VerificaDisponibilidade(Salas *PA, int Valor, int Sentido){
	int Disponiveis = 0; //guarda o numero de casas que ele pode ander efetivamente
	Salas *pTemp = PA;
	if(Sentido){
		while(pTemp->anterior != 0 && Disponiveis < Valor){ //loop pra descobrir quantas casas ele realmente tem para se movimentar.
			Disponiveis++;
			pTemp--;
		}
	} else{
		while(pTemp->numero < 16 && Disponiveis < Valor){ //loop pra descobrir quantas casas ele realmente tem para se movimentar.
			Disponiveis++;
			pTemp++;
		}
	}
	/* Basicamente, ele vai incrementando Disponiveis até bater no teto das salas, ou a primeira ou a ultima, ou até o valor de Disponiveis ser igual ao valor 
	requisitado originalmente na chamada da função, que é representado pela variavel Valor, além disso a distinção se ele esta se movimentando para cima ou para
	baixo é simples também, logica gooleana, como em C qualquer coisa que tenha valor é verdadeiro e algo que tenha valor 0 é falso, basta colocar 0 na chamada
	para indicar que voce quer andar pra tras, ou 1 se for andar pra frente */
	
	return Disponiveis == Valor ? Valor : 0;
} /* Essa função só existe para auxiliar na movimentação da Entidade, porque dentro do mapa, ela vai poder andar não só para os lados, mas para cima e para baixo
na explicação da movimentação dela eu explico isso melhor */

void MovimentacaoEntidade(Salas *PA, int Direcao){
	int pulo = 0;
	if(Direcao == 1){
		pulo = -VerificaDisponibilidade(PA, 4, 1);
	} else if(Direcao == 2){
		pulo = VerificaDisponibilidade(PA, 1, 0);
	} else if(Direcao == 3){
		pulo = VerificaDisponibilidade(PA, 4, 0);
	}  else if(Direcao == 4){
		pulo = -VerificaDisponibilidade(PA, 1, 1);
	} 

	if(pulo != 0){
		PA->estado -= ENTIDADE;
		(PA + pulo)->estado += ENTIDADE;
	}
} /* Aqui o bixo pega, para entender a movimentação da entidade, precisamos enxergar a nossa matriz de duas formas ao mesmo tempo, segue abaixo ambas as formas:
Primeira Forma      |      Segunda Forma      
01 02 03 04
05 06 07 08                001 002 003 004 005 006 007 008 009 010 011 012 013 014 015 016
09 10 11 12                i00 i01 i02 i03 i04 i05 i06 i07 i08 i09 i10 i11 i12 i13 i14 i15
13 14 15 16
Mas porque temos que ver assim? por que quando falamos de armazenamento de memoria, a nossa matriz é guardada da segunda maneira, como uma lista, porem para
acessar direto pelos indices ela é vista da primeira, e isso é importante porque para nos que estamos usando ponteiros, o acesso das "casas" da matriz sempre 
sera adicionando no valor do vetor, mas se esquecermos da propriedade de subir e descer nas linhas, ela se torna uma lista, e como a entidade pode se mover
de forma horizontal e vertical dentro da matriz ela não vai adicionar um para se mover toda vez, mas sim variando entre 1 e 4, sendo 1 na vertical, e 4 na 
horizontal, mas porque 4 na horizontal? porque todas as linhas tem 4 casas, se voce subir uma linha sem mudar a coluna, voce apenas adiciona a quantidade de
casas de uma linha, é igual dias da semana, uma semana tem 7 dias, se voce esta numa segunda dia 1, a proxima segunda é dia 8, ou seja +7, caso ainda esteja 
confuso, pode olhar na tabela, onde de voce esta na posição 1 e quer descer um casa, voce vai para a casa 5, que é igual a 1 + 4.
porem ainda vamos criar uma inteligencia artificial pra nossa entidade, bem basica mas vamos. */

int AchaPosicaoEntidade(Salas *PA){
	int i = 0;
	while(PA->estado<2 && i < 16){
		PA++;
		i++;
	}
	return (i >= 16) ? -1 : PA->numero;
} //Procura na matriz a posição da Entidade, que é representada pelos estados 2 e 3 das salas, passando de casa em casa, e retornando o numero da casa que ela esta

int DecisaoEntidade(Salas *PA, int Movimentou){
	int SalaEntidade = AchaPosicaoEntidade(PA), SalaJogadores = AchaPosicaoJogadores(PA), opcao;
	Salas *SalaAtual, *SalaAlvo, *SalaInicial = PA;

	if(SalaEntidade == -1) Movimentou = 0; // Fix: Se a entidade sumir por erro de memória, força o teleporte

	while(PA->numero!=SalaEntidade && PA->numero <= 16){
		PA++;
	}
	SalaAtual = PA;
	PA = SalaInicial;
	while(PA->numero!=SalaJogadores && PA->numero <= 16){
		PA++;
	}
	SalaAlvo = PA;
	/* Aqui fizemos o primeiro passo, a entidade precisa saber a sala inicial e a que se encontra, isso porque as funções que ela utiliza precisam de ambas,
	fazendo essa distinção, podemor pegar agora sim e programar a tomada de decisão dela sem medo de nos perdermos na hora de chamar as funções, além disso, é 
	legal alterarmos os nomes pra ficar mais facil de entender. Além disso damos a informação de onde estão os jogadores para que ela tenha uma pseudo 
	bussola para onde ir */
	
	// interpretação da ultima rodada*
	/* Aqui eu vou deixar o comentario antes de começar porque ela vai dividir o tipo de ação da entidade ent o comentario ficaria muito longe e seria muito 
	estranho a primeira vista, mas é o seguinte, essa função aqui, "DecisaoEntidade", sempre vai retornar ou 1 ou 0, sendo 1 conseguindo se movimentar, e 0 
	não conseguindo se movimentar, porque pode acontecer, e caso aconteça ele agira de maneira diferente, para que o jogador não seja capado ou para que ele 
	alcançavel, precisamos que as ações da entidade sejam separadas em dois tipos, uma que é aleatória, onde ela não necessáriamente vai se movimentar em 
	direção ao jogador e uma em que ela vai atras do jogador, nessa ideia a entidade é algo que pode se teleportar, e isso é bom porque gera inecerteza sobre 
	quando ela vai aparecer, então para começo das decisões, vamos diferencia-las assim: a entidade se moveu na ultima rodada? se sim, ela sera menos agressiva
	e vamos fazer ela decidir como vai se mover baseado em sua distância em relação aos jogadores, ja se ela não se moveu, ela perdeu um turno, ou seja, esta 
	atras dos jogadores, pra que ela possa de recuperar, vamos tornala mais agressiva e "rapida", mas deixoavisado que essa chanse dela perder o turno é baixa
	então só serve como garantia que não prejudicara a entidade, tenha em mente que ainda haveram mais divisões dentre essa primeira, mas com o conceito geral
	mais explicado fica simples de identificar mais pra frente coisas semelhantes. */
	if(Movimentou){
		/*Aqui vamos fazer mais uma diferenciação, se o jogador estiver a mais de 10 casas de distância, a entidade vai ser bem agressiva, se estiver a mais de 5,
		sera consciente mas não tão agressiva, e caso esteja a menos de 5 ela sera ativa mas bem aleatória */
		if(abs(SalaEntidade - SalaJogadores) >= 10){ // caso a distância seja maior igual a 10
			/* Como aqui ela vai ser mais agressiva, vamos querere que a entidade possa se mover de forma mais livre, e evidentemente que ela vai querer se 
			mover majoritariamente na horizontal para poder pegar os jogadores por isso vamos fazer a movimentação aqui ser apenas na horizontal, pra isso
			vamos separar os dois casos mais simples, caso a Sala dos Jogadores seja menor que a da entidade e caso a da entidade seja menor que a dos jogadores */
			if(SalaEntidade > SalaJogadores){
				MovimentacaoEntidade(SalaAtual, 1);
				/* Aqui eu vou explicar o porque de eu escolher o sentido 1, pensando na nossa estrutura, no primeiro andar, estar em uma sala maior que a de
				outra pessoa é igual a estar mais ao sul do andar, por isso ele quer se mover para o norte, para se aproximar da posição dos jogadores, se
				estiver confuso, olhe novamente nas tabelas feitas em explicações anteriores que ficara mais claro */
			} else{
				MovimentacaoEntidade(SalaAtual, 3); // a explicação é a mesma da de cima mas ao contrario, aqui a entidade esta ao note e o jogador ao sul
			}
			
		} else if(abs(SalaEntidade - SalaJogadores) > 5){ // caso a distância seja maior que 5
			/* Aqui vamo fazer diferente, vamos começar a trabalhar com probabilidade, pra isso vamos fazer o seguinte, aqui temos um caso onde a entidade quer
			andar até o jogador, porem não é garantido que ela estara indo corretamente, então vamos manter uma probabilidade de 40% de chanse de ele ser 
			totalmente aleatório que é equivalente a 2/5 de errar, pra isso, vou usar o switch e um rand() % 5 + 1 para obter o resultado, porque se usassemos
			o rand() % 6, e colocassemoso 0 como uma opção, seria na verdade 2/6 (1/3), e eu não busco por essa probabilidade */
			opcao = (rand() % 5) + 1;
			switch(opcao){
				case 1:
				case 2:
				case 3:
					/* Aqui vamos fazer algo diferente, vamos recalcular o valor de opcao, para que decidamos qual tipo de movimentação será feita, 
					horizontal ou vertival e baseado nisso agiremos, aqui vamos fazer uma chanse de 1/3 vertical, 2/3 horizontal, porque ele ainda esta meio 
					longe do nosso jogador */
					opcao = (rand() % 3) + 1;
					if(opcao > 1){ // horizontal
						//Aqui vamos fazer a verificação de que sentido vamos nos mover, mesma coisa da ultima vez
						if(SalaEntidade > SalaJogadores){
							MovimentacaoEntidade(SalaAtual, 1); // move pro norte
						} else{
							MovimentacaoEntidade(SalaAtual, 3); // move pro sul
						}
					} else { // vertical
						if(SalaEntidade > SalaJogadores){
							MovimentacaoEntidade(SalaAtual, 4); // move pro oeste
						} else{
							MovimentacaoEntidade(SalaAtual, 2); // move pro leste
						}
					}
					break;
				case 4:
				case 5:
					// Aqui é full Aleatório
					MovimentacaoEntidade(SalaAtual, (rand() % 4) + 1);
					break;
				default:
					exit(1); // se chegar ao ponto de aqui sair um valor diferente tem um erro grave e encerramos o codigo
					break;
			}
		} else{ // caso a distância seja menor que 5
			/* Esse ultimo caso vai ser se ele estiver bem proximo, vamos por uma chanse de 50% de ser ser totalmente aleatório, e vamos dar prioridade a 
			movimentação vertical, lembrando que as probabilidades estão sendo decididas mas podem ser "rebalanceadas" a qualquer momento por voce */
			opcao = rand() % 2; // eu não estou usando o rand() % 2 + 1 porque como ele vai retornar 1 ou 0, é opção booleana, então é mais simples
			if(opcao){
				// sem segredo, mesmo processo do anterior mas vamos inverter as chanses
				opcao = (rand() % 3) + 1;
				if(opcao > 2){ // horizontal
					//Aqui vamos fazer a verificação de que sentido vamos nos mover, mesma coisa da ultima vez
					if(SalaEntidade > SalaJogadores){
						MovimentacaoEntidade(SalaAtual, 1); // move pro norte
					} else{
						MovimentacaoEntidade(SalaAtual, 3); // move pro sul
					}
				} else { // vertical
					if(SalaEntidade > SalaJogadores){
						MovimentacaoEntidade(SalaAtual, 4); // move pro oeste
					} else{
						MovimentacaoEntidade(SalaAtual, 2); // move pro leste
					}
				}
			} else{
				MovimentacaoEntidade(SalaAtual, (rand() % 4) + 1);
			}
		}
	} else{
		/* Agora, nesse caso aqui, vamos fazer o seguinte, como ele "perdeu" seu turno, vamos forçadamente aproxima-lo dos jogadores, no meu caso, vou coloca-lo
		à 2 casa de distância, sendo 50% de ser pra frente deles, ou 50% pra trás deles */
		if(SalaEntidade != -1) SalaAtual->estado -= ENTIDADE;
		
		int alvo;
		opcao = rand() % 2; // mesma justificativa da probabilidade anterior
		if(opcao){
			if(SalaJogadores < 15){
				alvo = SalaJogadores + 2;
			} else{
				alvo = SalaJogadores - 2;
			} // Aqui esse if serve pra garantir que a entidade não va ser teleportada pra um lugar que não existe caso o jogador esteja na casa 15 ou 16 	
		} else{
			if(SalaJogadores > 2){
				alvo = SalaJogadores - 2;
			} else{
				alvo = SalaJogadores + 2;
			} // Mesma coisa, mas pra casa 1 e 2 agora
			
		}
		// Fix: Usando acesso direto por índice via SalaInicial para evitar sumiço por aritmética de ponteiro local
		SalaInicial[alvo - 1].estado += ENTIDADE; 
		/* Lembra que eu falei que seria importante? agora pra facilitar nossa vida pegamos a sala dos jogadores, a sala alvo, e voltamos duas ou avançamos duas
		pra poder chegar rápido no objetivo */
	}
	int check = AchaPosicaoEntidade(SalaInicial);
	return (check == SalaEntidade && SalaEntidade != -1) ? 0 : 1; // avisa se a entidade se movimentou ou não
} /* A maior parte dessa função é comentário, mas eu quis deixar redondinho o que cada coisa faz e porque é como é, mas o codigo em sí não é muito grande... */

int Turno(Salas *PA, int ResultadoEntidade){ 
	// Como ja lidamos com o corpo do jogo, tudo que precisamos fazer é unir no turno a mostragem do mapa, a jogada do player e a decisão da Entidade.
	system("cls");
	MostraMapa(PA);
	MovimentacaoJogadores(PA);
	return DecisaoEntidade(PA, ResultadoEntidade);
}

void Campanha(Salas *PA){
	/* Estamos perto do final, agora só vamos fazer o loop principal do jogo, onde criamos o mapa, rodamos os turnos até que a posição dos jogadores atinja a 
	16° sala, terminando assim a campanha de jogo, por enquanto claro. */
	int SalaJogadores = 0, ResultadoEntidade = 1; // para armazenar e checar se o jogo acabou, e responder se a entidade se moveu ou não.
	
	// Criando mapa e colocando tudo no devido lugar*
	defPrimeiroAndar(PA); 
	
	// Loop principal de rodar turnos*
	while(SalaJogadores < 16){
		ResultadoEntidade = Turno(PA, ResultadoEntidade); /* Aqui é a parte legal, porque pode parecer estranho fazer uma variavel receber um valor que vem de
		uma função que precisa do seu valor pra rodar, mas como ja definimos a base, "ResultadoEntidade = 0", ele basicamente vai rodar a função no valor antigo
		e depois de finalizar retorna um valor novo */
		SalaJogadores = AchaPosicaoJogadores(PA); // Aqui é bem intuitivo, ele só ta atualizando o valor da Sala Jogadores
	}
	
	system("cls"); // Limpa terminal depois de finalizar o jogo
	
	printf("\nVoce terminou! Parabens!"); // mensagem final da campanha
	getch();
}

int Menu(Salas *PA){
	int opt;
	
	system("cls");
	
	printf("Bem vindo! Esse e o menu do Maze Stalker C");
	printf("\n[1] - Iniciar nova campanha \n[2] - sair");
	printf("\nEscolha uma das opcoes acima: ");
	if(scanf("%d", &opt) != 1) return 0;
	
	switch(opt){
		case 1:
			Campanha(PA);
			break;
		case 2:
			printf("\nNos Vemos em Breve!");
			break;
		default:
			printf("Esta nao e uma opcao valida!");
			break;
	}
	//sistema de seleção de opção simples
	return opt;
}

int main() {
	int opt = 0;
	Salas PrimeiroAndar[4][4];
	
	do{ // loop de execussão do jogo
		opt = Menu(&PrimeiroAndar[0][0]);
	}while(opt != 2);
	
	return 0;
}

/* Considerações finais da versão 1.2 do prototipo, eu fiz com a intenção de ser usado para controlar uma mesa de rpg, onde os player teriam que descobrir por 
pistas como sair do labirinto e em todo o encontro com a entidade eles se esconderiam para fugir porque ela não seria derrotavel, por isso não coloquei mecanicas
de combate, mas quem sabe no futuro né?, além do mais, sou um estudante do curso de ADS, atualmente no 1 semestre mas ja tinha feito antes 2 semestres, então
tenho um pouco de familiaridade com C, com certeza não é o melhor codigo e deve ter muitas formas de otimizar ou fazer melhor, mas fiz só por curiosidade e 
porque não tava com nada pra fazer, perdão pelos erros de gramatica, sou dev não escritor kkkk, mas deixando de lado, se alguem conseguiu ler e gostou ja é
o bastante, não sei se vou publicar no github, porque eu sei que isso ta bem amador, mas mesmo assim, ainda vou pensar sobre. 
Resolvi postar com algumas correções que eu tive que fazer comparado a versão original que funcionava mas com algumas falhas estranhas dependendo de quantas 
vezes era rodado, agora não deve ter mais problemas tecnicos no funcionamento embora tenha a certeza de que ainda deve dar pra otimizar mais, vou ver mas ja 
tenho ideia pra um outro projeto um pouco mais elaborado, então talzes essa seja a versão definitiva*/
