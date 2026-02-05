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
void insereAno(int *anos, int ano) { //Funçao que insere o ano no array de anos, se ja existir, nao insere novamente
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
    for (int i = 1; i < 10; i++){ //A lógica aqui é encontrar o menor ano e substituí-lo se o novo ano for maior
        if (anos[i] < anos[min]){ //Isso garante que sempre teremos os 10 anos mais recentes
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

int anovalido(int *anos, int ano) { //Funçao que verifica se o ano é válido, ou seja, se ele existe no array de anos
    for (int i = 0; i < 10; i++){ //Isso é importante para garantir que só consideramos os anos das últimas 10 edições para nao pesar o programa com anos antigos que nao sao relevantes para a análise
        if (anos[i] == ano) //Logo agora podemos chamar a fução de adicionar os IDs sabendo que o ano é válido e que ele está dentro dos 10 anos mais recentes
        return 1; //Retorno 1 se o ano for válido
    }
    return 0; //Retorno 0 se o ano for inválido
}

EdicaoPadrao getedicao(EdicaoPadrao* ed, int ano, char* tipo){ //Funçao que retorna a ediçao correspondente ao ano e tipo
    for(int i = 0; i < 10; i++){
        if(ed[i].ano == ano && strcmp(ed[i].tipo, tipo) == 0){
            return ed[i]; //Retorno a ediçao se encontrar
        }
    }
    EdicaoPadrao edicaoVazia = {0}; //Retorno uma ediçao vazia se nao encontrar, isso é importante para evitar erros de acesso a memória
    return edicaoVazia;
}

int main()
{
    FILE* arquivo = fopen("results.csv","r"); //Por segurança verifico se o ponteiro nao está pegando lixo na memória
    if(arquivo == NULL){
        printf("Não foi possível executar o arquivo");
        return 1;
    }
    char linha[2000];
    int anosVerao[10] = {0}; //Array que guarda os anos das ediçoes de verão
    int anosInverno[10] = {0}; //Array que guarda os anos das ediçoes de inverno
    EdicaoPadrao verao[10] = {0}; //Pego a estrutura de uma unica ediçao de olimpiada e divido, depois preencho um array de tamanho 10
    EdicaoPadrao inverno[10] = {0};
    while(fgets(linha, sizeof(linha), arquivo)){ //Enquanto houver linhas no arquivo o programa continua lendo e preenchendo os arrays
        int col = 0;
        int ano = 0;
        int id = 0;
        char tipo[10] = "";
        char* token = strtok(linha, ",");
        while(token != NULL){
            if(col == 0){
                sscanf(token, "%d %9s", &ano, tipo);
            }
            else if(col == 6){
                id = atoi(token);
            }
            col++;
            token = strtok(NULL, ",");
        }
        if(strcmp(tipo, "Summer") == 0){
            insereAno(anosVerao, ano);
        }
        else if(strcmp(tipo, "Winter") == 0){
            insereAno(anosInverno, ano);
        }
    }
    rewind(arquivo); //Volto o ponteiro do arquivo para o inicio

    EdicaoPadrao verao[10] = {0}; //Reinicializo as estruturas
    EdicaoPadrao inverno[10] = {0};
    while(fgets(linha, sizeof(linha), arquivo)){ //Leio o arquivo novamente para preencher as estruturas
        int col = 0;
        int ano = 0;
        int id = 0;
        char tipo[10] = "";
        char* token = strtok(linha, ",");
        while(token != NULL){ //O código aqui é similar ao anterior só que agora chamo a funçao que adiciona os IDs
            if(col == 0){
                sscanf(token, "%d %9s", &ano, tipo);
            }
            else if(col == 6){
                id = atoi(token);
            }
            col++;
            token = strtok(NULL, ",");
        }
        if(strcmp(tipo, "Summer") == 0 && anovalido(anosVerao, ano)){ //Verifico se o tipo é verão e se o ano é válido
            addIds(verao, ano, id, tipo);
            EdicaoPadrao* e = getEdicao(verao, ano, tipo); //Pego a ediçao correspondente ao ano
            if(!verificarId(e, id)) //Verifico se o ID ja existe na ediçao, se nao existir, adiciono
                addIds(verao, ano, id, tipo);
        }
        else if(strcmp(tipo, "Winter") == 0 && anovalido(anosInverno, ano)){
            EdicaoPadrao *e = getEdicao(inverno, ano, tipo);
            if(!verificarId(e, id)) //Pego a ediçao correspondente ao ano
                addIds(inverno, ano, id, tipo);
        }
    }
    fclose(arquivo); //Fecho o arquivo após o uso, é uma boa prática para liberar recursos do sistema

    printf("Olimpíadas de Verão:\n");
    for(int i = 0; i < 10; i++){
        if(verao[i].ano != 0){ //Imprimo apenas os anos que foram preenchidos, por segurança e boa prática, para evitar imprimir anos vazios
            printf("Ano: %d, Quantidade de Atletas: %d\n", verao[i].ano, verao[i].qtdAtletas);
        }
    }
    printf("Olimpíadas de Inverno:\n");
    for(int i = 0; i < 10; i++){
        if(inverno[i].ano != 0){
            printf("Ano: %d, Quantidade de Atletas: %d\n", inverno[i].ano, inverno[i].qtdAtletas);
        }
    }
}