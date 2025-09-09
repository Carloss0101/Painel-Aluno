#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define TAM_Padrao 10

typedef struct {
    int dia;
    int mes;
    int ano;
} td_data;

typedef struct {
    int hora;
    int min;
} td_hora;

typedef struct{
    int codigo_dis;
    char disciplina[100];
}td_disciplinas;

typedef struct {
    int ra;
    char nome[100];
    char email[100]; 
} td_aluno;

typedef struct{
    int ra;      
    int codigo_dis;   
    td_data data_matricula; 
    float notas[4];
} td_matricula;

typedef struct {
    td_aluno aluno;
    td_hora horario;
    td_data data;
    char descricao[300];
} td_compromisso;


void cadastrar_aluno(td_aluno [], int); //Feito
void cadastra_disciplina(td_disciplinas [], int); //Feito
void cadastra_matricula(td_matricula [], int, td_aluno[], td_disciplinas[]); //Feito
void cadastra_compromisso(td_compromisso [], int);
void imprime_vetor_de_alunos(td_aluno [], int);
int verifica_data(td_data *);
int verifica_horario(td_hora *);
int verifica_email(char []); //Feito
int encontra_aluno(td_aluno *, int, int); //Feito
int encontra_disciplina(td_disciplinas *, int, int); //Feito
int procura_data(td_compromisso *, int, td_data *, int);
int procura_horario(td_compromisso *, int, td_hora *, td_data *, int);
int apresentar_menu(); //Feito

// Alocação inicial 
void* aloca_vetor(int quantidade, size_t tamanho_tipo) {
    return malloc(quantidade * tamanho_tipo);
}

// Realocação 
void* realoca_vetor(void* vetor, int nova_quantidade, size_t tamanho_tipo) {
    return realloc(vetor, nova_quantidade * tamanho_tipo);
}



int main() {
    td_aluno* alunos;
    td_disciplinas* disciplinas;
    td_matricula* matriculas;
	
	int cap_alunos = 10;
	int qtd_alunos = 0;

	int cap_disciplinas = 10;
	int qtd_disciplinas = 0;

	int cap_matriculas = 20;
	int qtd_matriculas = 0;

	alunos = aloca_vetor(cap_alunos, sizeof(td_aluno));
	disciplinas = aloca_vetor(cap_disciplinas, sizeof(td_disciplinas));
	matriculas = aloca_vetor(cap_matriculas, sizeof(td_matricula));

	int menu; 
	do {
		system("cls");
		menu = apresentar_menu();
		
		switch(menu)
		{
		case 1:
			system("cls");
			if(qtd_alunos == cap_alunos) {
				cap_alunos += 10;
				alunos = realoca_vetor(alunos, cap_alunos, sizeof(td_aluno));
			}
			cadastrar_aluno(alunos, qtd_alunos);
			qtd_alunos++;
			break;
		case 2:
			system("cls");
			if(qtd_disciplinas == cap_disciplinas) {
				cap_disciplinas += 5;
				disciplinas = realoca_vetor(disciplinas, cap_disciplinas, sizeof(td_disciplinas));
			}
			cadastra_disciplina(disciplinas, qtd_disciplinas);
			qtd_disciplinas++;
			break;
		case 3:
			system("cls");
			if(qtd_matriculas == cap_matriculas) {
				cap_matriculas += 10;
				matriculas = realoca_vetor(matriculas, cap_matriculas, sizeof(td_matricula));
			}
			cadastra_matricula(matriculas, qtd_matriculas, alunos, disciplinas);
			qtd_matriculas++;
			break;
		case 8:
			system("cls");
			printf("\nVoce saiu do programa!\n");
			break;
		default:
			printf("\n[ERRO] Opcao invalida, tente novamente!\n");
			break;
		}
	}while(menu != 8);

	return 0;
}

void cadastrar_aluno(td_aluno *alunos, int posicao) {
	int ra, menu;
	char email[100];
	
	do {
    	printf("Digite o ra do aluno: "); 
    	scanf("%i", &ra);
    
    	int validacao = encontra_aluno(alunos, TAM_Padrao, ra);
    
    	if(validacao != -1) {
        	printf("\nAluno ja cadastrada: %s", alunos[validacao].nome);
        
        	printf("\nPressione 1 para repetir ou outra tecla para voltar\n");
        	scanf("%i", &menu);
        
			if(menu != 1) {
            	return;
        	}
    	} else {
        	break; // Sai do loop quando o R.A é válido
    	}
	} while(1);
	
	getchar(); 
	
	do {
		printf("\nDigite o email do aluno: ");
		fgets(email, sizeof(email), stdin);
		
		if(verifica_email(email)) {
			break; 
		} else {
			printf("\nEmail Invalido, tente novamente!");
		} 
	}while(1);
	
	printf("\nDigite o nome do aluno: ");
	fgets(alunos[posicao].nome, sizeof(alunos[posicao].nome), stdin);
	
	alunos[posicao].ra = ra;
	strcpy(alunos[posicao].email, email); 
}

void cadastra_disciplina(td_disciplinas *disciplinas, int posicao) {
	int codigoDisciplina, menu;
	
	do {
		printf("\nDigite o codigo da disciplina: ");
		scanf("%i", &codigoDisciplina);

		int validacao = encontra_disciplina(disciplinas, TAM_Padrao, codigoDisciplina);
		
		if(validacao == -1) {
			break;
		} 
		printf("\nDisciplina ja cadastrada: %s", disciplinas[validacao].disciplina);
			
		printf("\nPressione 1 para repetir ou outro numero para voltar\n");
		scanf("%i", &menu);
			
		if(menu != 1) {
			return;
		}
	}while(1);
	
	disciplinas[posicao].codigo_dis = codigoDisciplina;
		
	getchar();
	printf("\nNome da disciplina: ");
	fgets(disciplinas[posicao].disciplina, sizeof(disciplinas[posicao].disciplina), stdin);
	
}

void cadastra_matricula(td_matricula *matriculas, int qtd_matriculas, td_aluno *alunos, td_disciplinas *disciplinas) {
	int ra, verificaAluno, menu =0;
	
	do {
		printf("\nDigite o R.A do aluno: ");
		scanf("%i", &ra);
		
		verificaAluno = encontra_aluno(alunos, TAM_Padrao, ra);
		
		if(verificaAluno != -1) {
			printf("Aluno: %s", alunos[verificaAluno].nome);
			break;
		}else {
			printf("\nAluno nao encontrado!");
			printf("\nDigite 1 para tentar novamente ou qualquer numero para voltar.");
			
			scanf("%i", &menu);
			if(menu != 1) {
				return;
			}
		}
	}while(1);
	
	int codigoDisciplina, verificarDisciplina;
	menu = 0;
	
	do {
		printf("\nDigite o codigo da disciplina: ");
		scanf("%i", &codigoDisciplina);
		
		verificarDisciplina = encontra_disciplina(disciplinas, TAM_Padrao, codigoDisciplina);
		
		if(verificarDisciplina != -1) {
			printf("Disciplina: %s", disciplinas[verificarDisciplina].disciplina);
			break;
		}else {
			printf("\nDisciplina nao encontrado!");
			printf("\nDigite 1 para tentar novamente ou qualquer numero para voltar.");
			
			scanf("%i", &menu);
			if(menu != 1) {
				return;
			}
		}
	}while(1);
	
	int i;
	float notas[4];
	
	for(i =0; i < 4; i++) {
    	printf("\nDigite a nota 0%i: ", i+1);
    	scanf("%f", &matriculas[qtd_matriculas].notas[i]);
	}
	time_t t = time(NULL);
	struct tm *dataAtual = localtime(&t);
	
	matriculas[qtd_matriculas].ra = ra;
	matriculas[qtd_matriculas].codigo_dis = codigoDisciplina;
	matriculas[qtd_matriculas].data_matricula.dia = dataAtual->tm_mday;
	matriculas[qtd_matriculas].data_matricula.mes = dataAtual->tm_mon + 1;
	matriculas[qtd_matriculas].data_matricula.ano = dataAtual->tm_year + 1900;
}

int verifica_email(char email[]) {
    int arroba_cont = 0;
    int pontos_cont = 0;
    int i;
	    
    // Verifica cada caractere do email
    for (i = 0; email[i] != '\n'; i++) {
        char c = email[i];
	 	
   		if(!isalnum(c) && c != '_' && c != '@' && c != '.') {
			return 0;			  		  	   
		} 
		
        if (c == '@') {
            arroba_cont++;
        }
        
        if (c == '.') {
            pontos_cont++;
        }
    }
    
	if (arroba_cont== 1 && pontos_cont > 0) {
        // Verifica se o ponto esta apos o '@'   
		char *at_pos = strchr(email, '@');
        
		if (strchr(at_pos, '.') != NULL) {
            return 1; // Email valido
        }
    }
    return 0; // Email invalido
}

int encontra_aluno(td_aluno *alunos, int tamanhoVetor, int codigo) {
  	  int i =0;
	for(i =0; i < tamanhoVetor; i++) {
   	   if(alunos[i].ra == codigo) {
		return i; //Aluno encontrada, retorna a posicao do aluno
        }
    }
    return -1; //Alunp não encontrada	
}

int encontra_disciplina(td_disciplinas *disciplinas, int tamanhoVetor, int codigo) {
    int i;
    for(i =0; i < tamanhoVetor; i++) {
        if(disciplinas[i].codigo_dis == codigo) {
            return i;//Disciplina encontrada, retorna a posicao da disciplina
        }
    }
    return -1; //Disciplina não encontrada
}

int apresentar_menu() {
	int menu;
	printf("------------------------------");
	printf("\n 1 | Cadastrar Aluno");
	printf("\n 2 | Cadastrar Disciplina");
	printf("\n 3 | Cadastrar Matricula");
	printf("\n\n 8 | Sair\n");
	printf("------------------------------\n");
		
	scanf("%i", &menu);
	
	return menu;
}
