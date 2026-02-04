/* Nesse arquivo, irei pré-instânciar as FUNÇÕES QUE TRATARÃO OS DADOS oriundos do .csv

   @author Mateus Aranha - (github.com/matt-aranha)
*/


# ifndef LEITURA_H              // Se não foi definido...
# define LEITURA_H              // Agora define.

# include "dados.h"

// Função auxiliar para limpar aspas de Strings ("Kalil" => Kalil).
void limpar_string(char *string);

// Função para carregar os dados dos atletas em um vetor indexado pelo ID. Retorna o total de atletas lidos ou -1 (erro).
int carregar_atletas(const char *caminho_arquivo, Atleta *banco_dados);

// Função para ler os resultados. Cruza com o banco de atletas e preenche a lista final. Retorna a quantidade de medalhistas.
int processar_resultados(const char *caminho_arquivo, Atleta *banco_dados, Medalhista *lista_final);


# endif                         // Fim do if.