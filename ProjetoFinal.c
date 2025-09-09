#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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

//Funções de menu
int apresentar_menu(); 
void apresentar_menu_imprimir(td_compromisso [], int);

//Funções de cadastros
void cadastrar_aluno(td_aluno [], int*); 
void cadastra_disciplina(td_disciplinas [], int*); 
void cadastra_matricula(td_matricula [], int*, td_aluno[], int, td_disciplinas[], int); 
void cadastra_compromisso(td_compromisso [], int*, td_aluno [], int); 

//Funções de impressão
void imprime_aluno(td_aluno [], int*); 
void imprime_vetor_alunos(td_aluno [], int*); 
void imprime_compromissos_um_aluno(td_compromisso[], int); 
void imprime_compromissos_todos_alunos(td_compromisso[], int); 
void imprime_compromissos_uma_data(td_compromisso[], int); 
void imprime_compromissos_todas_datas(td_compromisso[], int); 

//Funções de verificação
int verifica_data(td_data []); 
int verifica_horario(td_hora []); 
int verifica_email(char []); 

//Funções de busca
int encontra_aluno(td_aluno [], int, int); 
int encontra_disciplina(td_disciplinas [], int, int); 
int procura_data(td_compromisso [], int, td_data *, int); 
int procura_horario(td_compromisso [], int, td_data [], td_hora [], int); 

//Funções de comparação (qsort)
int compara_aluno_ra(const void *, const void *); 
int compara_data_hora(const void *, const void *); 
int compara_ra_data_hora(const void *, const void *); 
int compara_hora_ra(const void *, const void *); 
int compara_data_hora_ra(const void *, const void *); 

//Funções de memoria
void* aloca_vetor(int, size_t); 
void* realoca_vetor(void*, int, size_t); 

int main() {
    td_aluno* alunos;
    td_disciplinas* disciplinas;
    td_matricula* matriculas;
	td_compromisso* compromissos;
	
	//cap = Capacidade do vetor
	//qtd = posicao atual do vetor
	int cap_alunos = 10;
	int qtd_alunos = 0;

	int cap_disciplinas = 10;
	int qtd_disciplinas = 0;

	int cap_matriculas = 20;
	int qtd_matriculas = 0;

	int cap_compromissos = 10;
	int qtd_compromissos = 0;
	
	//Locação inicial dos vetores
	alunos = aloca_vetor(cap_alunos, sizeof(td_aluno));
	disciplinas = aloca_vetor(cap_disciplinas, sizeof(td_disciplinas));
	matriculas = aloca_vetor(cap_matriculas, sizeof(td_matricula));
	compromissos = aloca_vetor(cap_compromissos, sizeof(td_compromisso));
	
	int menu; 
	do {
		system("cls");
		menu = apresentar_menu();
		
		switch(menu)
		{
		case 1:
			system("cls"); //Limpa o terminal
			//Se a quantidade do vetor for iqual a sua capacidade, ele realoca uma nova quantidade para o vetor.
			if(qtd_alunos == cap_alunos) { 
				cap_alunos += 10;
				alunos = realoca_vetor(alunos, cap_alunos, sizeof(td_aluno));
			}
			cadastrar_aluno(alunos, &qtd_alunos);
			break;
		case 2:
			system("cls");
			if(qtd_disciplinas == cap_disciplinas) {
				cap_disciplinas += 5;
				disciplinas = realoca_vetor(disciplinas, cap_disciplinas, sizeof(td_disciplinas));
			}
			cadastra_disciplina(disciplinas, &qtd_disciplinas);
			break;
		case 3:
			system("cls");
			if(qtd_matriculas == cap_matriculas) {
				cap_matriculas += 10;
				matriculas = realoca_vetor(matriculas, cap_matriculas, sizeof(td_matricula));
			}
			cadastra_matricula(matriculas, &qtd_matriculas, alunos, qtd_alunos, disciplinas, qtd_disciplinas);
			break;
		case 4:
			system("cls");
			if(qtd_compromissos == cap_compromissos) {
				cap_compromissos += 5;
				compromissos = realoca_vetor(compromissos, cap_compromissos, sizeof(td_compromisso));
			}
			cadastra_compromisso(compromissos, &qtd_compromissos, alunos, qtd_alunos);
			break;
		case 5:
			system("cls");
			apresentar_menu_imprimir(compromissos, qtd_compromissos);
			break;
		case 6:
			system("cls");
			imprime_aluno(alunos, &qtd_alunos);
			break;
		case 7:
			system("cls");
			imprime_vetor_alunos(alunos, &qtd_alunos);
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

//===============Funções de menu===============

int apresentar_menu() {
	int menu;
	printf("------------------------------");
	printf("\n 1 | Cadastrar aluno");
	printf("\n 2 | Cadastrar disciplina");
	printf("\n 3 | Cadastrar matricula");
	printf("\n 4 | Cadastrar compomisso");
	printf("\n 5 | Imprimir compromissos");
	printf("\n 6 | Imprimir dado de um aluno");
	printf("\n 7 | Imprimir dado de todos os aluno");
	printf("\n\n 8 | Sair\n");
	printf("------------------------------\n");
		
	scanf("%d", &menu);
	
	return menu;
}

void apresentar_menu_imprimir(td_compromisso *compromissos, int qtd_compromissos) {
	int menu;
	
	do {
		system("cls");
		printf("--------------------------------------------");
		printf("\n 1 | Imprimir compromissos de um aluno");
		printf("\n 2 | Imprimir compromissos de todos alunos");
		printf("\n 3 | Imprimir compromissos de uma data");
		printf("\n 4 | Imprimir compromissos de todas datas");
		printf("\n\n 8 | voltar\n");
		printf("--------------------------------------------\n");
		
		
		scanf("%d", &menu);
		switch(menu) {
		case 1:
			system("cls");
			imprime_compromissos_um_aluno(compromissos, qtd_compromissos);
			break;
		case 2:
			system("cls");
			imprime_compromissos_todos_alunos(compromissos, qtd_compromissos);
			break;
		case 3:
			system("cls");
			imprime_compromissos_uma_data(compromissos, qtd_compromissos);
			break;
		case 4:
			system("cls");
			imprime_compromissos_todas_datas(compromissos, qtd_compromissos);
			break;
		case 8:
			printf("\nVoltando para o menu principal!");
			break;
		default:
			printf("\n[ERRO] Opcao invalida! Tente novamente");
			break;
		}
	}while(menu != 8);
}

//===============Funções de cadastros===============

void cadastrar_aluno(td_aluno alunos[], int* qtd_alunos) {
	int ra, menu;
	char email[100];
	
	do {
    	printf("Digite o ra do aluno: "); 
		scanf("%d", &ra);
    	
    	int validacao = encontra_aluno(alunos, *qtd_alunos, ra);
    
    	//Verifica se o aluno já existe.
    	if(validacao != -1) {
        	printf("\nAluno ja cadastrado: %s", alunos[validacao].nome);
        	
			printf("\nPressione 1 para repetir ou outra tecla para voltar\n");
        	scanf("%d", &menu);
        
			if(menu != 1) {
            	return;
        	}
    	} else {
        	break;
    	}
	} while(1);
	
	getchar(); 

	do {
		printf("\nDigite o email do aluno: ");
		fgets(email, sizeof(email), stdin);
		
		email[strcspn(email, "\n")] = '\0'; // substitui o \n por \0
		
		//Verifica se o email está no formato correto.
		if(verifica_email(email)) {
			break; 
		} else {
			printf("\nEmail Invalido");
  	      
			printf("\nPressione 1 para repetir ou outra tecla para voltar\n");
        	scanf("%d", &menu);
        
			if(menu != 1) {
            	return;
        	}
  	      getchar(); 
		} 
	}while(1);
	
	printf("\nDigite o nome do aluno: ");
	fgets(alunos[*qtd_alunos].nome, sizeof(alunos[*qtd_alunos].nome), stdin);
	
	alunos[*qtd_alunos].nome[strcspn(alunos[*qtd_alunos].nome, "\n")] = '\0'; // substitui o \n por \0
	alunos[*qtd_alunos].ra = ra;
	strcpy(alunos[*qtd_alunos].email, email);
	
	(*qtd_alunos)++;
}

void cadastra_disciplina(td_disciplinas disciplinas[], int* qtd_disciplinas) {
	int codigoDisciplina, menu;
	
	do {
    	printf("\nDigite o codigo da disciplina: ");
    	scanf("%d", &codigoDisciplina);

    	int validacao = encontra_disciplina(disciplinas, *qtd_disciplinas, codigoDisciplina);
		
		//Verifica se a disciplina já existe
		if (validacao != -1) {
			printf("\nDisciplina ja existente: %s\n", disciplinas[validacao].disciplina);
			
			printf("\nDigite 1 para tentar novamente ou qualquer outro numero para voltar ao menu: ");
			scanf("%d", &menu);
			
			if (menu != 1) {
       	    		return;
	 	 	 }
   		 } else
			break; //Sai do loop
	} while (1);

	
	getchar();
	printf("\nNome da disciplina: ");
	fgets(disciplinas[*qtd_disciplinas].disciplina, sizeof(disciplinas[*qtd_disciplinas].disciplina), stdin);
	disciplinas[*qtd_disciplinas].codigo_dis = codigoDisciplina;
	(*qtd_disciplinas)++;
}

void cadastra_matricula(td_matricula matriculas[], int* qtd_matriculas, td_aluno alunos[], int qtd_alunos, td_disciplinas disciplinas[], int qtd_disciplinas) {
	int i, ra, verificaAluno, menu =0;
	
	do {
		printf("\nDigite o R.A do aluno: ");
		scanf("%d", &ra);
		
		verificaAluno = encontra_aluno(alunos, qtd_alunos, ra);
		
		//Verifica se o aluno existe.
		if(verificaAluno != -1) {
			printf("Aluno: %s", alunos[verificaAluno].nome);
			break;
		}else {
			printf("\nAluno nao encontrado!");
			
			printf("\nDigite 1 para tentar novamente ou qualquer outro número para voltar ao menu: ");
			scanf("%d", &menu);
			
			if(menu != 1) {
				return;
			}
		}
	}while(1);
	
	int codigoDisciplina, verificarDisciplina;
	menu = 0;
	
	do {
		printf("\nDigite o codigo da disciplina: ");
		scanf("%d", &codigoDisciplina);
		
		verificarDisciplina = encontra_disciplina(disciplinas, qtd_disciplinas, codigoDisciplina);
		
		//Verifica se a disciplina existe
		if(verificarDisciplina != -1) {
			printf("Disciplina: %s", disciplinas[verificarDisciplina].disciplina);
			break;
		}else {
			printf("\nDisciplina nao encontrado!");
			
			printf("\nDigite 1 para tentar novamente ou qualquer outro numero para voltar ao menu: ");
			scanf("%d", &menu);
			if(menu != 1) {
				return;
			}
		}
	}while(1);
	
	//Repetição para pegar as 4 notas do aluno.
	for(i =0; i < 4; i++) {
    	printf("\nDigite a nota 0%d: ", i+1);
    	scanf("%f", &matriculas[*qtd_matriculas].notas[i]);
	}
	time_t t = time(NULL);
	struct tm *dataAtual = localtime(&t); //Pega a data em que a matricula foi efetuada
	
	matriculas[*qtd_matriculas].ra = ra;
	matriculas[*qtd_matriculas].codigo_dis = codigoDisciplina;
	matriculas[*qtd_matriculas].data_matricula.dia = dataAtual->tm_mday; //Dia atual
	matriculas[*qtd_matriculas].data_matricula.mes = dataAtual->tm_mon + 1; //Mes atual
	matriculas[*qtd_matriculas].data_matricula.ano = dataAtual->tm_year + 1900; //Ano atual
	
	(*qtd_matriculas)++;
}

void cadastra_compromisso(td_compromisso *compromisso, int* qtd_compromissos, td_aluno alunos[], int qtd_alunos) {
	int ra, menu=0;
	int verificarRa;
	
	td_data data;
	td_hora hora;
	
	do {
		printf("\nDigite o r.a do aluno: ");
		scanf("%d", &ra);
		
		verificarRa = encontra_aluno(alunos, qtd_alunos, ra);
		
		//Verifica se o aluno existe
		if(verificarRa != -1) {
			printf("Aluno: %s \n", alunos[verificarRa].nome);
			break;	
		}else {
			printf("\nAluno nao encontrado!");
			
			printf("\nDigite 1 para tentar novamente ou qualquer outro numero para voltar ao menu: ");
			scanf("%d", &menu);
			
			getchar();
			
			if(menu != 1) {
				return;
			}
		}
	}while(1);
	
	do {
    	printf("\nDigite a data do compromisso (DD/MM/AAAA): ");
    	scanf("%d/%d/%d", &data.dia, &data.mes, &data.ano);
		
		//Verifica se a data está correta e se a data é futura a data atual
   		 if (!verifica_data(&data)) {
   		     printf("\nData invalida!");
    	} else if (!procura_data(compromisso, *qtd_compromissos, &data, ra)) {
    	    printf("\nAluno ja possui dois compromissos nessa data!");
    	} else {
    	    break; // data válida e aluno pode cadastrar
   	 	 }

		printf("\nDigite 1 para tentar novamente ou qualquer outro numero para voltar ao menu: ");
    	scanf("%d", &menu);
		
		getchar();
    	
		if (menu != 1) {
        	return; // volta ao menu
    	}
	} while (1);
	
	do {
		printf("\nDigite o horario do compromisso (HH:MM): ");
 	   	scanf("%d:%d", &hora.hora, &hora.min);
    
    	//Verifica o horario	
    	if (!verifica_horario(&hora)) {
    		printf("Hora invalida!");
        	} else if(!procura_horario(compromisso, *qtd_compromissos, &data, &hora, ra)){
            	printf("\nAluno ja possui compromisso nesse horario!");
        	} else {
				break; //hora valida, sai do loop
			}
    
    	printf("\nDigite 1 para tentar novamente ou qualquer outro numero para voltar ao menu: ");
    	scanf("%d", &menu);
    	getchar();
    	if (menu != 1) {
        	return; // volta ao menu
    	}
	} while (1);

	
	//Depois de todas as informações válidas, adiciona no array.
	getchar();
	printf("\nDigite a descricao do compromisso: ");
	fgets(compromisso[*qtd_compromissos].descricao, sizeof(compromisso[*qtd_compromissos].descricao), stdin);
	
	compromisso[*qtd_compromissos].aluno = alunos[verificarRa];
	compromisso[*qtd_compromissos].data = data;
	compromisso[*qtd_compromissos].horario = hora;

	
	(*qtd_compromissos)++;
}

//===============Funções de impressão===============

void imprime_aluno(td_aluno *alunos, int* qtd_alunos) {
    int ra, verificador, menu;

    do {
        printf("Digite o R.A do aluno: ");
        scanf("%d", &ra);
		
		//Procura pelo cadastro do aluno, retorna a posicao do aluno no array
        verificador = encontra_aluno(alunos, *qtd_alunos, ra);
        if (verificador != -1) {
            printf("\nNome: %s", alunos[verificador].nome);
            printf("\nRA: %d", alunos[verificador].ra);
            printf("\nEmail: %s", alunos[verificador].email);
        } else {
            printf("\nAluno nao encontrado!");
        }

        printf("\nDigite 1 para imprimir outro RA ou qualquer outro numero para voltar ao menu: ");
        scanf("%d", &menu);

    } while (menu == 1);
}

void imprime_vetor_alunos(td_aluno *alunos, int *qtd_alunos) {
	int i, menu;
	
	//Verifica se tem alunos cadastrados
    if(*qtd_alunos == 0) {
		printf("Nenhum aluno cadastrado!");
	} else {
    
		// Ordena os alunos por RA
		qsort(alunos, *qtd_alunos, sizeof(td_aluno), compara_aluno_ra);
		
    	printf("\nLista de Alunos (ordenada por RA):\n");
    	for (i = 0; i < *qtd_alunos; i++) {
        	printf("\nRA: %d", alunos[i].ra);
        	printf("\nNome: %s", alunos[i].nome);
        	printf("\nEmail: %s\n", alunos[i].email);
    	}
	}
    printf("\n\nDigite qualquer numero para voltar ao menu: ");
    scanf("%d", &menu);
}

void imprime_compromissos_um_aluno(td_compromisso *compromissos, int qtd_compromissos) {
    int ra, i, count = 0, j=0, menu;
    
	printf("Digite o RA do aluno: ");
    scanf("%d", &ra);

    //Contar quantos compromissos o aluno tem
    for (i = 0; i < qtd_compromissos; i++) {
        if (compromissos[i].aluno.ra == ra) {
            count++;
        }
    }

    if (count > 0) {
 	   //Criar vetor temporário com os compromissos desse aluno
    	td_compromisso *filtrados = malloc(count * sizeof(td_compromisso));
    	for (i = 0; i < qtd_compromissos; i++) {
        	if (compromissos[i].aluno.ra == ra) {
            	filtrados[j++] = compromissos[i];
 	 	 }
    	}

    	//Ordenar por data e hora
    	qsort(filtrados, count, sizeof(td_compromisso), compara_data_hora);

    	//Imprimir
    	printf("\nCompromissos do aluno %s (RA: %d):\n", filtrados[0].aluno.nome, ra);
    	for (i = 0; i < count; i++) {
        	printf("\nData: %02i/%02i/%04i\n", filtrados[i].data.dia, filtrados[i].data.mes, filtrados[i].data.ano);
        	printf("Horario: %02i:%02i\n", filtrados[i].horario.hora, filtrados[i].horario.min);
        	printf("Descricao: %s\n", filtrados[i].descricao);
    	}
    	
    	free(filtrados); //Libera a memoria
    } else {
		   printf("\nNenhum compromisso encontrado para esse aluno.\n");	
	}
	
	
	printf("\n\nDigite qualquer numero para voltar ao menu: ");
    scanf("%d", &menu);
    
}

void imprime_compromissos_todos_alunos(td_compromisso *compromissos, int qtd_compromissos) {
    int i, menu;
	
	//Verifica se tem compromissos cadastrados
    if (qtd_compromissos == 0) {
        printf("Nenhum compromisso cadastrado.\n");
    } else {
        //Ordenar todos os compromissos por RA, data e hora
        qsort(compromissos, qtd_compromissos, sizeof(td_compromisso), compara_ra_data_hora);

        printf("\nCompromissos de todos os alunos:\n");

        for (i = 0; i < qtd_compromissos; i++) {
            printf("\nAluno: %s (RA: %d)\n", compromissos[i].aluno.nome, compromissos[i].aluno.ra);
            printf("Data: %02i/%02i/%04i\n", compromissos[i].data.dia, compromissos[i].data.mes, compromissos[i].data.ano);
            printf("Horario: %02i:%02i\n", compromissos[i].horario.hora, compromissos[i].horario.min);
            printf("Descricao: %s\n", compromissos[i].descricao);
        }
    }

    printf("\n\nDigite qualquer numero para voltar ao menu: ");
    scanf(" %d", &menu);
}

void imprime_compromissos_uma_data(td_compromisso *compromissos, int qtd_compromissos) {
    td_data data;
    int menu, count = 0, i, j = 0;

    printf("\nDigite a data (DD/MM/AAAA): ");
	scanf("%d/%d/%d", &data.dia, &data.mes, &data.ano);

    //Conta quantos compromissos tem na data
    for (i = 0; i < qtd_compromissos; i++) {
        if (compromissos[i].data.dia == data.dia &&
            compromissos[i].data.mes == data.mes &&
            compromissos[i].data.ano == data.ano) {
            count++;
        }
    }

    if (count == 0) {
        printf("\nData invalida ou sem compromissos\n");
    } else {
        td_compromisso *filtrados = malloc(count * sizeof(td_compromisso));

        for (i = 0; i < qtd_compromissos; i++) {
            if (compromissos[i].data.dia == data.dia &&
                compromissos[i].data.mes == data.mes &&
                compromissos[i].data.ano == data.ano) {
                filtrados[j++] = compromissos[i];
            }
        }

        // Ordena por hora e RA
        qsort(filtrados, count, sizeof(td_compromisso), compara_hora_ra);

        printf("\nCompromissos do dia %02d/%02d/%04d:\n", data.dia, data.mes, data.ano);
        
        for (i = 0; i < count; i++) {
            printf("\nAluno: %s (RA: %d)", filtrados[i].aluno.nome, filtrados[i].aluno.ra);
            printf("\nHorario: %02i:%02i", filtrados[i].horario.hora, filtrados[i].horario.min);
            printf("\nDescricao: %s\n", filtrados[i].descricao);
        }

        free(filtrados); //Libera a memoria
    }

    printf("\nDigite qualquer numero para voltar ao menu: ");
    scanf("%d", &menu); //Volta para o menu
}

void imprime_compromissos_todas_datas(td_compromisso *compromissos, int qtd_compromissos) {
    int i, menu;
    
    if (qtd_compromissos == 0) {
        printf("\nNenhum compromisso cadastrado.\n");
    } else {
        // Ordenar por data, hora, RA
        qsort(compromissos, qtd_compromissos, sizeof(td_compromisso), compara_data_hora_ra);

        printf("\nCompromissos ordenados por data, hora e RA:\n");

		//imprimi os vetores
        for (i = 0; i < qtd_compromissos; i++) {
        	printf("\nAluno: %s (RA: %d)\n", compromissos[i].aluno.nome, compromissos[i].aluno.ra);
            printf("Data: %02d/%02d/%04d\n", compromissos[i].data.dia, compromissos[i].data.mes, compromissos[i].data.ano);
            printf("Horario: %02d:%02d\n", compromissos[i].horario.hora, compromissos[i].horario.min);
            printf("Descrição: %s\n", compromissos[i].descricao);
        }
    }

    printf("\n\nDigite qualquer numero para voltar ao menu: ");
    scanf("%d", &menu); //volta para o menu.
}

//===============Funções de verificação===============

int verifica_email(char email[]) {
    int arroba_cont = 0;
    int pontos_cont = 0;
    int i;
	    
    for (i = 0; email[i] != '\0'; i++) {
        char c = email[i];
	 
	    //Verifica se tem algum caractere fora do permitido
   		if(!isalnum(c) && c != '_' && c != '@' && c != '.') {
			return 0;			  	   
		} 

        if (c == '@') {
            arroba_cont++; //Conta quantos arrobas tem
        }
        
        if (c == '.') {
            pontos_cont++; //Conta quantos pontos tem
        }
    }

	//Verifica a contidade de arrobas e de pontos.
    if (arroba_cont == 1 && pontos_cont > 0) {
        char *at_pos = strchr(email, '@');

        if (at_pos != email && strchr(at_pos, '.') != NULL) {
            return 1;
        }
    }
    return 0;
}

int verifica_data(td_data *data) {
	int dd = data->dia;
    int mm = data->mes;
    int aa = data->ano;
    
    //Data atual
    time_t t = time(NULL);
    struct tm *dataAtual = localtime(&t);
	
    //Verifica se a data é menor que a data atual
    if (aa < (dataAtual->tm_year + 1900) ||
       (aa == (dataAtual->tm_year + 1900) && mm < (dataAtual->tm_mon + 1)) ||
       (aa == (dataAtual->tm_year + 1900) && mm == (dataAtual->tm_mon + 1) && dd < dataAtual->tm_mday)) {
        return 0; //Data inválida
    }    
    
    //Valida se a data é valida
    if (aa >= 1900 && aa <= 9999) {
        if (mm >= 1 && mm <= 12) {
            if ((dd >= 1 && dd <= 31) && (mm == 1 || mm == 3 || mm == 5 || mm == 7 || mm == 8 || mm == 10 || mm == 12)) {
                return 1; //Data valida
            }
            else if ((dd >= 1 && dd <= 30) && (mm == 4 || mm == 6 || mm == 9 || mm == 11)) {
                return 1; //Data valida
            }
            else if ((dd >= 1 && dd <= 28) && (mm == 2)) {
                return 1; //Data valida
            }
            else if (dd == 29 && mm == 2 && (aa % 400 == 0 || (aa % 4 == 0 && aa % 100 != 0))) {
                return 1; //Data valida
            }
            else {
                return 0; //Dia invalida
            }
        }
        else {
            return 0; //Mes invalido
        }
    }
    else {
        printf("Ano inválido.\n");
        return 0;
    }
}

int verifica_horario(td_hora *hora) {
    if (hora->hora < 0 || hora->hora > 23) {
		return 0; //Hora inválida   	
	}

    if (hora->min < 0 || hora->min > 59) {
        return 0; //Minuto inválido
	}
    
	return 1; // Horario válido
}

//===============Funções de busca===============

int encontra_aluno(td_aluno *alunos, int tamanhoVetor, int ra) {
    int i;
    //Percorre por todos os alunos verificando se ele já existe
	for (i = 0; i < tamanhoVetor; i++) {
        if (alunos[i].ra == ra) {
            return i;
        }
    }
    return -1;
}

int encontra_disciplina(td_disciplinas *disciplinas, int tamanhoVetor, int codigo) {
    int i;
    //Percorre por todos as disciplinas verificando se ela já existe.
	for(i =0; i < tamanhoVetor; i++) {
        if(disciplinas[i].codigo_dis == codigo) {
            return i;
        }
    }
    return -1;
}

int procura_data(td_compromisso *compromissos, int tamanho, td_data *data, int ra) {
    int contador = 0, i;
    for (i = 0; i < tamanho; i++) {
        if (compromissos[i].aluno.ra == ra && compromissos[i].data.dia == data->dia && compromissos[i].data.mes == data->mes && compromissos[i].data.ano == data->ano) {
            contador++;
        }
    }
    return (contador < 2); //retorna 1 se contador for menor que 2, caso contrario retorna 0.
}

int procura_horario(td_compromisso *compromissos, int tamanho, td_data *data, td_hora *horario, int ra) {
    int i;
	for (i = 0; i < tamanho; i++) {
        if (compromissos[i].aluno.ra == ra && compromissos[i].data.dia == data->dia && compromissos[i].data.mes == data->mes &&
            compromissos[i].data.ano == data->ano && compromissos[i].horario.hora == horario->hora && compromissos[i].horario.min == horario->min) {
            return 0; //Ja tem compromisso no mesmo horario
        }
    }
    return 1; //Horario valido
}

//===============Funções de comparação (qsort)===============

int compara_aluno_ra(const void *a, const void *b) {
    td_aluno *aluno1 = (td_aluno *)a;
    td_aluno *aluno2 = (td_aluno *)b;

    return aluno1->ra - aluno2->ra;
}

int compara_data_hora(const void *a, const void *b) {
    td_compromisso *c1 = (td_compromisso *)a;
    td_compromisso *c2 = (td_compromisso *)b;

    //Compara ano
    if (c1->data.ano != c2->data.ano)
        return c1->data.ano - c2->data.ano;

    //Compara mes
    if (c1->data.mes != c2->data.mes)
        return c1->data.mes - c2->data.mes;

    //Compara dia
    if (c1->data.dia != c2->data.dia)
        return c1->data.dia - c2->data.dia;

    //Compara hora
    if (c1->horario.hora != c2->horario.hora)
        return c1->horario.hora - c2->horario.hora;

    //Compara minutos
    return c1->horario.min - c2->horario.min;
}

int compara_ra_data_hora(const void *a, const void *b) {
    td_compromisso *c1 = (td_compromisso *)a;
    td_compromisso *c2 = (td_compromisso *)b;

    //Usa a função de RA que você já tem
    int resultado_ra = compara_aluno_ra(&(c1->aluno), &(c2->aluno));
    if (resultado_ra != 0) return resultado_ra;

    //Usa a função de data/hora que você já tem
    return compara_data_hora(c1, c2);
}

int compara_hora_ra(const void *a, const void *b) {
    const td_compromisso *c1 = (td_compromisso *)a;
    const td_compromisso *c2 = (td_compromisso *)b;

    // Ordenar por hora
    if (c1->horario.hora != c2->horario.hora)
        return c1->horario.hora - c2->horario.hora;

    if (c1->horario.min != c2->horario.min)
        return c1->horario.min - c2->horario.min;

    // Se a hora for igual, ordena por RA
    return c1->aluno.ra - c2->aluno.ra;
}

int compara_data_hora_ra(const void *a, const void *b) {
    const td_compromisso *c1 = (const td_compromisso *)a;
    const td_compromisso *c2 = (const td_compromisso *)b;

    //Compara data e hora primeiro
    int comparacao_data_hora = compara_data_hora(c1, c2);
    if (comparacao_data_hora != 0) return comparacao_data_hora;

    //Se data e hora forem iguais, compara RA
    return compara_aluno_ra(&(c1->aluno), &(c2->aluno));
}

//===============Funções de memoria===============

void* aloca_vetor(int quantidade, size_t tamanho_tipo) {
    return malloc(quantidade * tamanho_tipo);
}

void* realoca_vetor(void* vetor, int nova_quantidade, size_t tamanho_tipo) {
    return realloc(vetor, nova_quantidade * tamanho_tipo);
}
