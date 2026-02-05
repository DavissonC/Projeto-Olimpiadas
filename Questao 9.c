/*Calcule quantos atletas participaram das últimas 10 edições dos Jogos Olímpicos e analise a evolução
desse número ao longo do tempo, separando Verão e Inverno.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int ano;
    int IDsatl[200000]; //Array que guarda os IDs dos atletas, esse array é grande para evitar overflow
    char tipo[10]; //Verão ou Inverno
    int qtdAtletas;
}EdicaoPadrao; //Struct que guarda os dados de cada ediçao, separando Verão e Inverno, sendo 10 vezes cada
EdicaoPadrao verao[10];
EdicaoPadrao inverno[10];

//Por boa prática divido as açoes da funçao principal em outras funçoes mais especificas


void insereAno(int *anos, int ano) {
    for (int i = 0; i < 10; i++)
        if (anos[i] == ano)
            return; // já existe
    for (int i = 0; i < 10; i++) {
        if (anos[i] == 0) {
            anos[i] = ano;
            return;
        }
    }
    // encontra o menor
    int min = 0;
    for (int i = 1; i < 10; i++){
        if (anos[i] < anos[min]){
            min = i;
        }
        if (ano > anos[min]){
            anos[min] = ano;
        }
    }
}


//Funçao que verifica se o ID ja existe na ediçao
int verificarId(EdicaoPadrao* ed, int id){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < ed[i].qtdAtletas; j++){ 
            if(ed[i].IDsatl[j] == id){
                return 1; //Retorno 1 se o ID ja existir
            }
        }
    }
    return 0; //Retorno 0 se o ID nao existir
}

void addIds(EdicaoPadrao* ed, int ano, int id, char* tipo){
    ed->qtdAtletas = 0; //Inicializo a quantidade de atletas como 0
    for(int i = 0; i < 10; i++){
        if(ed[i].ano == ano){ //Verifico se o ano ja existe no array
            if(!verificarId(ed, id)){ //Se o ID nao existir, adiciono
                ed[i].qtdAtletas += 1;
                ed[i].IDsatl[ed[i].qtdAtletas - 1] = id; //Adiciono o ID do novo atleta no array
            }
            return; //Saio da funçao
        }
    }
    //Se o ano nao existir, adiciono um novo ano
    for(int i = 0; i < 10; i++){
        if(ed[i].ano == 0){ //Procuro a primeira posiçao vazia
            ed[i].ano = ano;
            ed[i].IDsatl[0] = id; //Adiciono o primeiro atleta
            ed[i].qtdAtletas = 1;
            strcpy(ed[i].tipo, tipo); //Copio o tipo (Verão ou Inverno)
            return; //Saio da funçao
        }
    }
}




int main()
{
    FILE* arquivo = fopen("results.csv","r"); //Por segurança verifico se o ponteiro nao está pegando lixo na memória
    if(arquivo == NULL){ //Com isso evito um retorno nao esperado, no caso, que nao seja abrir o arquivo
        printf("Não foi possível executar o arquivo");
        return 1;
    }
    char linha[2000];
    int anosVerao[10] = {0}; //Array que guarda os anos das ediçoes de verão
    int anosInverno[10] = {0}; //Array que guarda os anos das ediçoes de inverno
    EdicaoPadrao verao[10] = {0}; //Pego a estrutura de uma unica ediçao de olimpiada e divido, depois preencho um array de tamanho 10
    EdicaoPadrao inverno[10] = {0};
    while(fgets(linha, sizeof(linha), arquivo)){ //Enquanto houver linhas no arquivo}

    }
}