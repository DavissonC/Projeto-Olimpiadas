/* Nesse arquivo, irei pré-instânciar os DADOS que utilizarei no programa para tornar o código modularizado e organizado 
   
   @author Mateus Aranha - (github.com/matt-aranha)
*/

#ifndef DADOS_H              // Se não foi definido...
   #define DADOS_H              // Agora define.


   // Definindo tamanhos fixo para facilitar a alocação de memória:
   #define MAX_NOME 150
   #define MAX_EVENTO 100
   #define MAX_MEDALHA 20
   #define MAX_PAIS 60


   // Struct Atleta:
   typedef struct {
      int id;
      char nome[MAX_NOME];
      char genero;                 // 'M' ou 'F'
      int ano_nascimento;          // p/ calcular a idade posteriormente (ano da competição - ano de nascimento)
      char pais[MAX_PAIS];

   } Atleta;


   // Struct Medalhista:
   typedef struct {
      char nome[MAX_NOME];
      char genero;
      int idade_no_evento;
      char medalha[MAX_MEDALHA];
      int ano_olimpiada;
      char modalidade[MAX_EVENTO];
      char pais[MAX_PAIS];

   } Medalhista;


#endif                         // Fim do if.