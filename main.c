#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#define MAX_LEN 128 // Leitura maxima para imprimir a Logo

// Cores para o console em ANSII
#define COR_VERMELHA "\033[1;31m"
#define COR_VERDE "\033[1;32m"
#define COR_AZUL "\033[1;94m"
#define COR_BRANCA "\033[1;97m"

// Structs
typedef struct {
  int inicioHora;
  int inicioMinuto;
  int fimHora;
  int fimMinuto;
} Horario;

typedef struct {
  char tag[50];
  char descricao[100];
  Horario horario;
  struct AtividadesDia *prox;
} AtividadesDia;

typedef struct {
  AtividadesDia *seg;
  AtividadesDia *ter;
  AtividadesDia *qua;
  AtividadesDia *qui;
  AtividadesDia *sex;
  AtividadesDia *sab;
  AtividadesDia *dom;
} agendaSemanal;

typedef struct {
  char nomeAtividade[50];
  int diaDaSemana;
  Horario horario;
  char descricao[100];
  char tag[50];
} DadosAtividade;

// Funções

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
void config(agendaSemanal *agenda) {
  agenda->dom = criar_atividades_dia();
  agenda->seg = criar_atividades_dia();
  agenda->ter = criar_atividades_dia();
  agenda->qua = criar_atividades_dia();
  agenda->qui = criar_atividades_dia();
  agenda->sex = criar_atividades_dia();
  agenda->sab = criar_atividades_dia();
}

void criarTag(void) {

  char listaTags[50];
  
  printf("Digite o nome da Tag: ");
  fgets(listaTags, sizeof(listaTags), stdin);

}

// Main
int main() {
  setlocale(LC_ALL, "Portuguese");

  agendaSemanal agendaSemanal;
  config(&agendaSemanal);

  DadosAtividade dadosAtividade;
  int op, opDeAtribuicaoOuCriacao;
  char diaSemana[7][10] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};

  // logo();

  do {  // começo do menu do programa
    
    printf("\nEscolha uma das opções abaixo:");
    printf("\n1 - Cadastrar atividade");
    printf("\n2 - Buscar atividade");
    printf("\n3 - Alterar atividade");
    printf("\n4 - Deletar atividade");
    printf("\n5 - Mostrar Tags");
    printf("\n6 - Mostrar Relatório Semanal");
    printf("\n7 - Mostrar Agenda Semanal");
    printf("\n0 - Sair");

    
    printf("\nDigite a opção:\n");
    scanf("%d", &op);
    
    switch (op) {
      
      case 0:
        printf(COR_AZUL "Até mais\n" COR_BRANCA);
        
      break;

      case 1:

        printf("Qual o nome da atividade? ");
        setbuf(stdin, NULL);
        fgets(dadosAtividade.nomeAtividade, sizeof(dadosAtividade.nomeAtividade), stdin);
        
        printf("Qual dia da semana?\n");
        for (int i = 0; i < 7; i++) 
          printf("%d - %s\n", i+1 ,diaSemana[i]);
        
        scanf("%d", &dadosAtividade.diaDaSemana);

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
        
        printf("Deseja atribuir um tag existente ou Deseja criar uma nova Tag?\n");
        printf("\n1 - Atribuir #Tag");
        printf("\n2 - Criar nova #Tag");
        scanf("%d", &opDeAtribuicaoOuCriacao);

        if (opDeAtribuicaoOuCriacao == 1)
          printf("\nSuas Tags são: ");
        

      break;

      case 2:
        printf("Qual atividade deseja buscar? ");
        
      break;

      case 3:
        printf("Qual atividade deseja alterar? ");
        
      break;

      case 4:
        printf("Qual atividade deseja deletar? ");

      break;

      case 5:
        printf("Qual o nome da sua tag?");

      break;

      case 6:
        printf("Suas tags são: ");


      break;

      case 7:
        printf("Seu relatório Semanal é: ");


      break;

      case 8:
        printf("Em sua semana constam as seguintes atividades: ");
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