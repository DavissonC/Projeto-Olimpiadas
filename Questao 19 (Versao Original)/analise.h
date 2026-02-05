/* Nesse arquivo, irei pré-instânciar as FUNÇÕES QUE ENVOLVAM A LÓGICA da questão 19
   
   @author Mateus Aranha - (github.com/matt-aranha)
*/


#ifndef ANALISE_H                 // Se não foi definido...
   #define ANALISE_H              // Agora define.

   #include "dados.h"

   // Função para ordenar o vetor de medalhistas do mais velho para o mais novo.
   void ordenar_medalhistas ( Medalhista* lista, int total_medalhistas );

   // Função para exibir o relatório final (Top N por gênero).
   void exibir_ranking_idade ( Medalhista* lista, int total_medalhistas, int top_n );

   
#endif                            // Fim do if.