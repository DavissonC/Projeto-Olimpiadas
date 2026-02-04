/*Calcule quantos atletas participaram das últimas 10 edições dos Jogos Olímpicos e analise a evolução
desse número ao longo do tempo, separando Verão e Inverno.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int ano;
    int IDatl;
    char tipo[10]; //Verão ou Inverno
    int qtdAtletas;
}EdicaoPadrao;

//Por boa prática divido as açoes da funçao principal em outra funçao mais especifica
int verificarId(EdicaoPadrao* ed, int id){
    for(int i = 0; i < 10; i++){
        if(ed[i].IDatl == id){
            return 1; //Retorno 1 se o ID ja existir
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
                ed[i].IDatl = id; //Adiciono o ID do novo atleta
            }
            return; //Saio da funçao
        }
    }
    //Se o ano nao existir, adiciono um novo ano
    for(int i = 0; i < 10; i++){
        if(ed[i].ano == 0){ //Procuro a primeira posiçao vazia
            ed[i].ano = ano;
            ed[i].IDatl = id; //Adiciono o primeiro atleta
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
    EdicaoPadrao verao[10]; //Pego a estrutura de uma unica ediçao de olimpiada e divido, depois preencho um array de tamanho 10
    EdicaoPadrao inverno[10];

}