#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int ano;
    int qtdAtletas;
} Edicao; //Struct que guarda o ano e a qtd de atletas

typedef struct {
    int ano;
    int id;
    char estacao[10];
} Registro; //Struct que será utilizado para ordenação

int compararRegistros(const void *ptrGenericoA, const void *ptrGenericoB) {

    Registro *registroAtletaA = (Registro *)ptrGenericoA;
    Registro *registroAtletaB = (Registro *)ptrGenericoB;

    if (registroAtletaA->ano != registroAtletaB->ano) { // Compara o ano da copa
        return (registroAtletaA->ano - registroAtletaB->ano); //se der neg, A vem antes de B
    }

    return (registroAtletaA->id - registroAtletaB->id); // Se os anos forem iguais, compara o ID do atleta
} //Função de comparação para o qsort, usando ano e ID

int main() {
    FILE *arquivo = fopen("results.csv", "r");
    // Abre o arquivo e verifica se abriu corretamente
    if (arquivo == NULL) {
        printf("Nao foi possivel abrir o arquivo.\n");
        return 1;
    }

    int totalLinhas = 310000; 
    Registro *totalRegistros = malloc(sizeof(Registro) * totalLinhas); //Aloca espaço na memoria dinamicamente
    int totalLido = 0;

    Edicao verao[10]; //Armazena os dados das 10 primeiras olimpíadas de verão
    Edicao inverno[10]; //Armazena os dados das 10 primeiras olimpíadas de inverno
    int totalVerao = 0, totalInverno = 0;

    char linha[2000];
    int anoAnteriorVerao = -1;
    int anoAnteriorInverno = -1;
    
    int idAnteriorVerao = -1;
    int idAnteriorInverno = -1;

    //Lê a primeira linha (cabeçalho) e não faz nada com ela
    fgets(linha, sizeof(linha), arquivo);

    //Falta considerar que as colunas que eu preciso não estão em sequência
    while (fgets(linha, sizeof(linha), arquivo) && totalLido < totalLinhas) {
        char *tokenAno = strtok(linha, ","); //Lê o ano
        if (tokenAno == NULL) continue;
        totalRegistros[totalLido].ano = atoi(tokenAno); //Transforma string para int

        char *tokenEstacao = strtok(NULL, ","); //Lê a estação
        strcpy(totalRegistros[totalLido].estacao, tokenEstacao);

        char *tokenID = strtok(NULL, ","); //Lê o ID do atleta
        totalRegistros[totalLido].id = atoi(tokenID);
        
        totalLido++;
    }
    fclose(arquivo);

    qsort(totalRegistros, totalLido, sizeof(Registro), compararRegistros);  //ordena por Ano e ID

    for (int i = 0; i < totalLido; i++) { //Processa os dados ordenados
        int ano = totalRegistros[i].ano;
        char *estacao = totalRegistros[i].estacao;
        int id = totalRegistros[i].id;

        if (strcmp(estacao, "Summer") == 0) {
            if (ano != anoAnteriorVerao && totalVerao < 10) { //Verifica se não é o mesmo ano e se a quantidade de anos desejada(10) não foi atingida
                verao[totalVerao].ano = ano; //Guarda o ano
                verao[totalVerao].qtdAtletas = 1; // Começa a contagem
                anoAnteriorVerao = ano; //Atualiza o ano
                idAnteriorVerao = id; // Guarda o primeiro ID do ano
                totalVerao++; //Indica que mudou de olímpiada
            } else if (ano == anoAnteriorVerao && totalVerao <= 10) { //Caso seja o mesmo ano
                if (id != idAnteriorVerao) {
                    verao[totalVerao - 1].qtdAtletas++;
                    idAnteriorVerao = id;
                } //A quantidade de atletas só será incrementada se for um atleta diferente
            }
        }
        if (strcmp(estacao, "Winter") == 0) { //Alteração do trecho de cima para as Olimpíadas de inverno
            if (ano != anoAnteriorInverno && totalInverno < 10) { 
                inverno[totalInverno].ano = ano;
                inverno[totalInverno].qtdAtletas = 1;
                anoAnteriorInverno = ano;
                idAnteriorInverno = id;
                totalInverno++;
            } else if (ano == anoAnteriorInverno && totalInverno <= 10) {
                if (id != idAnteriorInverno) {
                    inverno[totalInverno - 1].qtdAtletas++;
                    idAnteriorInverno = id;
                }
            }
        }
    }

    // Liberação da memória e exibição (Opcional adicionar printf aqui)
    free(totalRegistros);
    return 0;
}
