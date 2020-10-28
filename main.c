#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#define MAX_LEN 128
#define COR_VERMELHA "\033[1;31m"
#define COR_VERDE "\033[1;32m"
#define COR_AZUL "\033[1;94m"
#define COR_BRANCA "\033[1;97m"

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

int main() {
  setlocale(LC_ALL, "Portuguese");
  
  char op;
  
  //logo();
  
  do {  // começo do menu do programa;
    
    printf("\nEscolha uma das opções abaixo:");
    printf("\n1 - Cadastrar atividade");
    printf("\n2 - Buscar atividade");
    printf("\n3 - Alterar atividade");
    printf("\n4 - Deletar atividade");
    printf("\n0 - Sair");

    
    printf("\nDigite a opção:\n");
    scanf("%c", &op);
    
    switch (op) {
      
      case '0':
        printf("Até mais\n");
      break;
      case '1':
        
      break;
      case '2':
        
      break;
      case '3':
        
      break;
      case '4':
        
      break;

      default:
      printf("\n************************");
        printf(COR_VERMELHA "\nDigite uma opção valida!" COR_BRANCA);
      printf("\n************************\n");
      break;
      
    }


  } while (op != '0');
  
  return 0;
}