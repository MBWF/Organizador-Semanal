#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LEN 128 // Leitura maxima para imprimir a Logo
#define TAM_SEMANA 7 // Quantidades de dias na semana de Domingo a Sabado

// Cores para o console em ANSII
#define COR_VERMELHA "\033[1;31m"
#define COR_ROXA "\033[0;35m"
#define COR_AZUL "\033[1;94m"
#define COR_VERDE "\033[1;32m"
#define COR_BRANCA "\033[1;97m"

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

typedef struct {
  char nomeAtividade[50];

  int inicioHora;
  int inicioMinuto;
  int fimHora;
  int fimMinuto;

  char descricao[100];
  int grau;

  int dia;
} ArquivoDados;

// Variaveis Globais
agendaSemanal agendasemanal;
DadosAtividade dadosAtividade;
char diaSemana[TAM_SEMANA][10] = { "Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado" };

// Funções
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

// Diz a cor de determinado grau escolhido!
void grauCor(int n) {
  switch(n) {
    case 1: 
      printf(COR_BRANCA);
    break;
    case 2: 
      printf(COR_AZUL);
    break;
    case 3: 
      printf(COR_ROXA);
    break;
    case 4: 
      printf(COR_VERDE);
    break;
    case 5: 
      printf(COR_VERMELHA);
    break;

  }
}

// Diz o dia semana dado o Index(n) e diz qual o dia exato
// Ex: 0 -> Domingo
void printDiaSemana(int n) {
  switch(n) {
    case 1: 
      printf("Dia da semana: Segunda");
    break;
    case 2: 
      printf("Dia da semana: Terça");
    break;
    case 3: 
      printf("Dia da semana: Quarta");
    break;
    case 4: 
      printf("Dia da semana: Quinta");
    break;
    case 5: 
      printf("Dia da semana: Sexta");
    break;
    case 6: 
      printf("Dia da semana: Sabado");
    break;
    case 0: 
      printf("Dia da semana: Domingo");
    break;

  }
}

//Serve de debug para ver se o arquivo
//está abrindo normalmentte
void lerArquivo() { 
  ArquivoDados dados;
  FILE *f = fopen("BD/semana.bin", "rb");
  if(!f) {
    printf("erro ao tentar ler o arquivo :(");
  }

  while(fread(&dados, sizeof(ArquivoDados), 1, f)) {
    grauCor(dados.grau);
    printDiaSemana(dados.dia);
    printf("Nome Atividade: %s",dados.nomeAtividade);
    printf("Descrição: %s",dados.descricao);
    printf("Horario: \nDas %d:%d até as %d:%d", dados.inicioHora, dados.inicioMinuto, dados.fimHora, dados.fimMinuto);
    printf("\n--------------------------------\n");
  }
}

//Salva definitivamente nos arquivos
void salvarArquivos(AtividadesDia *no, int index) {
  ArquivoDados dados;
  FILE *f = fopen("BD/semana.bin", "ab");

  strcpy(dados.nomeAtividade, no->nomeAtividade);
  strcpy(dados.descricao, no->descricao);
  dados.dia = index;
  dados.inicioHora = no->horario.inicioHora;
  dados.inicioMinuto = no->horario.inicioMinuto;
  dados.fimHora = no->horario.fimHora;
  dados.fimMinuto = no->horario.fimMinuto;
  dados.grau = no->grau;

  fwrite(&dados, sizeof(ArquivoDados), 1, f);
  fclose(f);
}

// Tem o objetivo de salvar em qualquer tipo de Arquivo ou banco de dados
void salvar(AtividadesDia *inicio, int index) {
  AtividadesDia *atividades = inicio;

  while(atividades!=NULL) {
    salvarArquivos(atividades, index);
    atividades = (AtividadesDia *)atividades->prox;
  }
}

//Troca valores do tipo AtividadeDia
void trocarValoresAtividade(AtividadesDia *primeiro, AtividadesDia *segundo) {
  char nomeAux[50];
  char descricaoAux[100];
  int aux;

  strcpy(nomeAux, primeiro->nomeAtividade);
  strcpy(primeiro->nomeAtividade, segundo->nomeAtividade);
  strcpy(segundo->nomeAtividade, nomeAux);

  strcpy(descricaoAux, primeiro->descricao);
  strcpy(primeiro->descricao, segundo->descricao);
  strcpy(segundo->descricao, descricaoAux);

  aux = primeiro->grau;
  primeiro->grau = segundo->grau;
  segundo->grau = aux;

  aux = primeiro->horario.inicioHora;
  primeiro->horario.inicioHora = segundo->horario.inicioHora;
  segundo->horario.inicioHora = aux;

  aux = primeiro->horario.inicioMinuto;
  primeiro->horario.inicioMinuto = segundo->horario.inicioMinuto;
  segundo->horario.inicioMinuto = aux;

  aux = primeiro->horario.fimHora;
  primeiro->horario.fimHora = segundo->horario.fimHora;
  segundo->horario.fimHora = aux;

  aux = primeiro->horario.fimMinuto;
  primeiro->horario.fimMinuto = segundo->horario.fimMinuto;
  segundo->horario.fimMinuto = aux;
}

//Essa função organiza a lista por horário
//chamando a função trocarValoresAtividade
void organizarLista(AtividadesDia *inicio) {
  AtividadesDia *i = inicio;
  AtividadesDia *j = inicio;

  while (i != NULL) {
    while (j != NULL) {
      if(i->horario.inicioHora > j->horario.inicioHora) {

        trocarValoresAtividade(i, j);
        break;
      }

      if(i->horario.inicioHora == j->horario.inicioHora) {
        if(i->horario.inicioMinuto > j->horario.inicioMinuto) {

          trocarValoresAtividade(i, j);
        }
      }

      j =(AtividadesDia *) j->prox;
    }

    i = (AtividadesDia *) i->prox;
    j = i;
  }
}

void listaAtividades(AtividadesDia *inicio) {
  system("clear");
  logo();

  AtividadesDia *atividades = inicio;

  while(atividades!=NULL) {
    grauCor(atividades->grau);
    printf("Nome Atividade: %s",atividades->nomeAtividade);
    printf("Descrição: %s",atividades->descricao);
    printf("Horario: \nDas %d:%d até as %d:%d", atividades->horario.inicioHora, atividades->horario.inicioMinuto, atividades->horario.fimHora, atividades->horario.fimMinuto);
    printf("\n--------------------------------\n");

    atividades = (AtividadesDia *)atividades->prox;
  }

  printf(COR_BRANCA);
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

  // está inserindo ao final da lista!
  if(inicio == NULL) {
    return atividade;

  }
  else {

    AtividadesDia *p = inicio;

    while(p->prox != NULL) {
      p = (AtividadesDia *) p->prox ;
    }

    p->prox = (struct AtividadesDia *) atividade;
    organizarLista(inicio);
    return inicio;
  }
}

AtividadesDia * criar_atividades_dia(void) {
  return NULL;
}
// Obtém todas as atividades do arquivo
// e adiciona nas devidas listas
void carregarDadosArquivo() {
  ArquivoDados dados;
  DadosAtividade dadosArquivo;
  FILE *f = fopen("BD/semana.bin", "rb");
  if(!f) {
    printf("erro ao tentar ler o arquivo :(");
  }

  while(fread(&dados, sizeof(ArquivoDados), 1, f)) {
    strcpy(dadosArquivo.nomeAtividade, dados.nomeAtividade);
    strcpy(dadosArquivo.descricao, dados.descricao);
    dadosArquivo.grau = dados.grau;
    dadosArquivo.horario.inicioHora = dados.inicioHora;
    dadosArquivo.horario.inicioMinuto = dados.inicioMinuto;
    dadosArquivo.horario.fimHora = dados.fimHora;
    dadosArquivo.horario.fimMinuto = dados.fimMinuto;
    agendasemanal.semana[dados.dia] = inserirAtividade(agendasemanal.semana[dados.dia], &dadosArquivo);
  }

  fclose(f);

}
//Configuração necessária para configurar cada lista
void config(AtividadesDia *semana[], int tam) {

 for(int i = 0; i < tam; i++){
   semana[i] = criar_atividades_dia();
  }
}
//Filtragem de index de dia da Semana 
int indexDiaSemana() {
  int op;

  printf("\n");
  for(int i = 0; i < TAM_SEMANA; i++) {
    printf("%d-%s\n", i + 1, diaSemana[i]);
  }

  printf("Digite o numero correspondente ao dia: ");
  scanf("%d", &op);

  while(op <= 0 || op > TAM_SEMANA) {
    printf(COR_VERMELHA "Ops... você digitou um número invalido\n" COR_BRANCA);
    printf("Digite o numero correspondente ao dia: ");
    scanf("%d", &op);
  }
  
  return op - 1;
}

void cadastrarAtividade() {
  // int opDeAtribuicaoOuCriacao;

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
  
  if (dadosAtividade.grau > 5){
    printf("O grau de importancia esta maior que o limite!\n");
    printf("Escolha o grau de importancia novamente: ");
    scanf("%i", &dadosAtividade.grau);
    printf("\n");
  }

  int index = indexDiaSemana();

  agendasemanal.semana[index] = inserirAtividade(agendasemanal.semana[index], &dadosAtividade);
  
}

bool existiAtividade(AtividadesDia *inicio, char nomeAtividade[]) {
  bool existir = false;
  AtividadesDia *p = inicio;
  
  while(p != NULL) {
    if(strcmp(p->nomeAtividade, nomeAtividade) == 0) {
      existir = true;
      return existir;
    }

    p =(AtividadesDia *) p->prox;
  }
  return existir;
}

void buscarAtividade(AtividadesDia *inicio, char nomeAtividade[], int index) {
  AtividadesDia *p = inicio;

  while(p != NULL) {
    if(strcmp(p->nomeAtividade, nomeAtividade) == 0) {
      
      grauCor(p->grau);
      printDiaSemana(index);
      printf("\n");
      printf("Nome Atividade: %s",p->nomeAtividade);
      printf("Descrição: %s",p->descricao);      
      printf("Horario: \nDas %d:%d até as %d:%d", p->horario.inicioHora, p->horario.inicioMinuto, p->horario.fimHora, p->horario.fimMinuto);
      printf("\n--------------------------------\n");
    }

    p =(AtividadesDia *) p->prox;
  }
}

void atualizarAtividade(AtividadesDia *inicio, char nomeAtividade[], DadosAtividade *dados) {
  AtividadesDia *p = inicio;
  bool existir = false;

  while(p != NULL) {
    if(strcmp(p->nomeAtividade, nomeAtividade) == 0) {
      existir = true;
      p->grau = dados->grau;
      p->horario.inicioHora = dados->horario.inicioHora;
      p->horario.inicioMinuto = dados->horario.inicioMinuto;
      p->horario.fimHora = dados->horario.fimHora;
      p->horario.fimMinuto = dados->horario.fimMinuto; 
      
      strcpy(p->nomeAtividade, dados->nomeAtividade);
      strcpy(p->descricao, dados->descricao);
    }

    p =(AtividadesDia *) p->prox;
  }

  if(existir == false) {
    printf("Ops... nenhuma atividade encontrada com esse nome\n");
  }
}

//Valida se a atividade existe e altera a atividade existente
//Chamando a função de organizar a lista para manter em ordem
void alterarAtividade(int index,char nomeAtividade[] ) {
  bool existir = false;

  for(int i= 0; i < TAM_SEMANA; i++) {
    if(existiAtividade(agendasemanal.semana[i], nomeAtividade)) {
      existir = true;
      break;
    }
  }

  if(existir == false) {
    printf("Ops... não foi encontrado essa atividade :(");
    return;
  }

  printf("novo nome da atividade? ");
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
  printf("Nova Descrição:\n");
  setbuf(stdin, NULL);
  fgets(dadosAtividade.descricao, sizeof(dadosAtividade.descricao), stdin);

  printf("\nQual novo Grau de importância(1 a 5)?\n");
  scanf("%i", &dadosAtividade.grau);
  
  if (dadosAtividade.grau > 5){
    printf("O grau de importancia esta maior que o limite!\n");
    printf("Escolha o grau de importancia novamente: ");
    scanf("%i", &dadosAtividade.grau);
    printf("\n");
  }

  atualizarAtividade(agendasemanal.semana[index], nomeAtividade, &dadosAtividade);
  organizarLista(agendasemanal.semana[index]);
}

//Deletar atividades escolhidas
AtividadesDia * deletarAtividade(AtividadesDia *inicio, char nomeAtividade[]){
  AtividadesDia *p = inicio;
  AtividadesDia *a = NULL;

  if(!existiAtividade(p, nomeAtividade)) {
    printf("Ops... nenhuma atividade encontrada com esse nome\n");
    return inicio;
  }

  while (p != NULL && strcmp(p->nomeAtividade, nomeAtividade) != 0 ) {
    a = p;
    p = (AtividadesDia *)p->prox;
  }

  if(p == NULL) {
    printf(COR_VERMELHA "\nAtividade não encontrado!\n" COR_BRANCA);
    return inicio;
  }else if( a == NULL) {
    printf(COR_VERDE "\nAtividade %sremovido!\n" COR_BRANCA, inicio->nomeAtividade);
    inicio =(AtividadesDia *) p->prox;
    free(p);
    return inicio;
  }else {
    printf( COR_VERDE "\nAtividade %sremovido!\n" COR_BRANCA, p->nomeAtividade);
    a->prox = p->prox;
    free(p);
    return inicio;
  }
}

int contadorAtividadePorGrau(int grau) {
  int contador = 0;
  ArquivoDados dados;

  FILE *f = fopen("BD/semana.bin", "rb");
  if(!f) {
    printf("erro ao tentar ler o arquivo :(");
  }

  while(fread(&dados, sizeof(ArquivoDados), 1, f)) {
      if(dados.grau == grau) {
        contador ++;
      }
  }
  
  fclose(f);

  return contador;

}

int contadorAtividadePorDia(int indexDia) {
  int contador = 0;
  ArquivoDados dados;

  FILE *f = fopen("BD/semana.bin", "rb");
  if(!f) {
    printf("erro ao tentar ler o arquivo :(");
  }

  while(fread(&dados, sizeof(ArquivoDados), 1, f)) {
      if(dados.dia == indexDia) {
        contador ++;
      }
  }
  
  fclose(f);

  return contador;
}

void relatorioSemanal() {
  system("clear");
  logo();

  int contadorAtividadesGrau[5], contadorAtividadeDia[TAM_SEMANA], totalDeAtividades = 0;

  printf(COR_AZUL);
  printf("\n------- Quantidade de atividades --------");
  for (int i = 0; i < TAM_SEMANA; i++) {
    printf("\n");
    contadorAtividadeDia[i] = contadorAtividadePorDia(i);
    printDiaSemana(i);
    printf("\nQuantidade: %d\n", contadorAtividadeDia[i]);
    totalDeAtividades += contadorAtividadeDia[i];
  }
  
  printf(COR_BRANCA);
  printf("\n------- Quantidade de atividades por Grau --------");
  for (int i = 1; i <= 5; i++) {
    contadorAtividadesGrau[i] = contadorAtividadePorGrau(i);
    printf("\nAtividades com Grau %d: %d", i, contadorAtividadesGrau[i]);
  }
  printf("\n");

  printf(COR_VERDE);
  printf("\n------- Atividades Semanal --------\n");
  printf("O total de atividades é: %d\n", totalDeAtividades);

  printf(COR_BRANCA);
  
}

// Main
int main() {
  setlocale(LC_ALL, "Portuguese");

  config(agendasemanal.semana, TAM_SEMANA);

  int op, index;
  char nomeAtividade[50];

  carregarDadosArquivo();

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
        printf("\n");
        printf(COR_BRANCA);
        remove("BD/semana.bin");
        for(int i= 0; i < TAM_SEMANA; i++) {
          salvar(agendasemanal.semana[i], i);
        }
      break;        

      case 2:
       
        printf("Digite a atividade: ");
        setbuf(stdin, NULL);
        fgets(nomeAtividade, sizeof(nomeAtividade), stdin);
        for(int i= 0; i < TAM_SEMANA; i++) {
          buscarAtividade(agendasemanal.semana[i], nomeAtividade, i);
        }
        printf("\n");
        printf(COR_BRANCA);

      break;

      case 3:
        printf("Digite a atividade: ");
        setbuf(stdin, NULL);
        fgets(nomeAtividade, sizeof(nomeAtividade), stdin);

        index = indexDiaSemana();

        alterarAtividade(index, nomeAtividade);

        remove("BD/semana.bin");
        for(int i= 0; i < TAM_SEMANA; i++) {
          salvar(agendasemanal.semana[i], i);
        }
      break;

      case 4:
        printf("Digite a atividade: ");
        setbuf(stdin, NULL);
        fgets(nomeAtividade, sizeof(nomeAtividade), stdin);

        index = indexDiaSemana();

        agendasemanal.semana[index] = deletarAtividade(agendasemanal.semana[index], nomeAtividade);

        remove("BD/semana.bin");
        for(int i= 0; i < TAM_SEMANA; i++) {
          salvar(agendasemanal.semana[i], i);
        }
      break;

      case 5:   
        relatorioSemanal();
      break;

      case 6:  
        index = indexDiaSemana();
        listaAtividades(agendasemanal.semana[index]);
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