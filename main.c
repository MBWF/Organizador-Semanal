#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#define MAX_LEN 128 // Leitura maxima para imprimir a Logo
#define TAM_SEMANA 7 // Quantidades de dias na semana de Domingo a Sabado

// Cores para o console em ANSII
#define COR_VERMELHA "\033[1;31m"
#define COR_VERDE "\033[1;32m"
#define COR_AZUL "\033[1;94m"
#define COR_BRANCA "\033[1;97m"
#define COR_ROXA "\033[0;35m"

// Structs
typedef struct {
  int inicioHora;
  int inicioMinuto;
  int fimHora;
  int fimMinuto;
} Horario;

typedef struct {
  char nomeAtividade[50];
  int grau;
  char descricao[100];
  Horario horario;
  struct AtividadesDia *prox;
} AtividadesDia;

typedef struct {
  AtividadesDia *semana[TAM_SEMANA];
} agendaSemanal;

typedef struct {
  char nomeAtividade[50];
  Horario horario;
  char descricao[100];
  int grau;
} DadosAtividade;

// Variaveis Globais
agendaSemanal agendasemanal;
DadosAtividade dadosAtividade;
char diaSemana[TAM_SEMANA][10] = { "Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado" };

// Funções

void listaAtividades(AtividadesDia *inicio) {
  AtividadesDia *atividades = inicio;

  while(atividades!=NULL) {
    printf(COR_BRANCA "Nome Atividade: %s",atividades->nomeAtividade);
    printf("Descrição: %s",atividades->descricao);
    printf("Horario: \nDas %d:%d até as %d:%d", atividades->horario.inicioHora, atividades->horario.inicioMinuto, atividades->horario.fimHora, atividades->horario.fimMinuto);
    printf("\n--------------------------------\n");

    atividades = (AtividadesDia *)atividades->prox;
  }
}

AtividadesDia * inserirAtividade(AtividadesDia *inicio, DadosAtividade *dados) {

  // alocando o novo nó
  AtividadesDia *atividade;
  atividade = (AtividadesDia *) malloc(sizeof(AtividadesDia));

  // dados da atividade
  strcpy(atividade->nomeAtividade, dados->nomeAtividade);
  strcpy(atividade->descricao, dados->descricao);

  // horario da atividade
  atividade->horario.inicioHora = dados->horario.inicioHora;
  atividade->horario.fimHora = dados->horario.fimHora;
  atividade->horario.inicioMinuto = dados->horario.inicioMinuto;
  atividade->horario.fimMinuto = dados->horario.fimMinuto;

  atividade->grau = dados->grau;

  atividade->prox = NULL;

  // está inserindo ao final da lista! ainda sem criterio para inserir
  if(inicio == NULL) {
    return atividade;

  }
  else {

    AtividadesDia *p = inicio;

    while(p->prox != NULL) {
      p = (AtividadesDia *) p->prox ;
    }

    p->prox = (struct AtividadesDia *) atividade;

    return inicio;
  }
}

AtividadesDia * criar_atividades_dia(void) {
  return NULL;
}

void print_image(FILE *fptr) {
  char read_string[MAX_LEN];

  while(fgets(read_string,sizeof(read_string),fptr) != NULL)
    printf("%s",read_string);

  printf("\n");
}

void logo() {
  char *filename = "BD/logo.txt";
  FILE *fptr = NULL;

  if((fptr = fopen(filename,"r")) == NULL) {
    fprintf(stderr,"error opening %s\n",filename);    
  }

  print_image(fptr);

  fclose(fptr);

}

void config(AtividadesDia *semana[], int tam) {

 for(int i = 0; i < tam; i++){
   semana[i] = criar_atividades_dia();
  }
}

int indexDiaSemana() {
  int op;

  printf("\n");
  for(int i = 0; i < TAM_SEMANA; i++) {
    printf("%d-%s\n", i+1, diaSemana[i]);
  }

  printf("Digite o numero correspondente ao dia: ");
  scanf("%d", &op);

  while(op <= 0 || op > TAM_SEMANA) {
    printf(COR_VERMELHA "Ops... você digitou um número invalido\n" COR_BRANCA);
    printf("Digite o numero correspondente ao dia: ");
    scanf("%d", &op);
  }
  
  return op - 1 ;
}

void cadastrarAtividade() {
  int opDeAtribuicaoOuCriacao;

  printf("Qual o nome da atividade? ");
  setbuf(stdin, NULL);
  fgets(dadosAtividade.nomeAtividade, sizeof(dadosAtividade.nomeAtividade), stdin);

  printf("Que horas começa a atividade?");
  printf("\nHora/Minutos: ");
  scanf("%d", &dadosAtividade.horario.inicioHora);
  scanf("%d", &dadosAtividade.horario.inicioMinuto);

  printf("---------------------------------\n");

  printf("Que horas termina a atividade?");
  printf("\nHora/Minutos: ");
  scanf("%d", &dadosAtividade.horario.fimHora);
  scanf("%d", &dadosAtividade.horario.fimMinuto);
  printf("---------------------------------\n");
  printf("Descrição:\n");
  setbuf(stdin, NULL);
  fgets(dadosAtividade.descricao, sizeof(dadosAtividade.descricao), stdin);

  printf("\nQual o Grau de importância(1 a 5)?\n");
  scanf("%i", &dadosAtividade.grau);

  int index = indexDiaSemana();

  agendasemanal.semana[index] = inserirAtividade(agendasemanal.semana[index], &dadosAtividade);
  
}

// Main
int main() {
  setlocale(LC_ALL, "Portuguese");

  config(agendasemanal.semana, TAM_SEMANA);

  int op, index;
  

  logo();

  do {  // começo do menu do programa

    printf("\nEscolha uma das opções abaixo:");
    printf("\n1 - Cadastrar atividade");
    printf("\n2 - Buscar atividade");
    printf("\n3 - Alterar atividade");
    printf("\n4 - Deletar atividade");
    printf("\n5 - Mostrar Relatório Semanal");
    printf("\n6 - Mostrar Agenda Semanal");
    printf("\n0 - Sair");

    printf("\nDigite a opção:\n");
    scanf("%d", &op);

    switch (op) {

      case 0:
        printf(COR_AZUL "Até mais\n" COR_BRANCA);

      break;

      case 1:
        cadastrarAtividade();

      break;        

      case 2:
        index = indexDiaSemana();
        // buscar atividade
      break;

      case 3:
        
      break;

      case 4:

      break;

      case 5:   

      break;

      case 6:  

        index = indexDiaSemana();
        listaAtividades(agendasemanal.semana[index]);
        
      break;

      case 7:

      break;

      default:

        printf("\n************************");
        printf(COR_VERMELHA "\nDigite uma opção valida!" COR_BRANCA);
        printf("\n************************\n");

      break;

    }

  } while (op != 0);

  return 0;

}