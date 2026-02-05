/*Calcule quantos atletas participaram das últimas 10 edições dos Jogos Olímpicos e analise a evolução
desse número ao longo do tempo, separando Verão e Inverno.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int ano;
    int IDsatl[200000]; //Array que guarda os IDs dos atletas, esse array é grande para evitar overflow
    char tipo[15]; //Verão ou Inverno
    int qtdAtletas;
}EdicaoPadrao; //Struct que guarda os dados de cada ediçao, separando Verão e Inverno, sendo 10 vezes cada

EdicaoPadrao verao[10];
EdicaoPadrao inverno[10];

//Por boa prática divido as açoes da funçao principal em outras funçoes mais especificas
void processarAtleta(EdicaoPadrao* arrayEdicoes, int ano, int id, char* tipo);
int anovalido(int *anos, int ano);
void ordenaredicoes(EdicaoPadrao* arrayEdicoes, int tamanho);
void insereAno(int *anos, int ano);
int verificarId(EdicaoPadrao* ed, int id);

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

void ordenaredicoes(EdicaoPadrao* arrayEdicoes, int tamanho) { //Funçao que ordena as ediçoes por ano, para facilitar a leitura dos dados
    for (int i = 0; i < tamanho - 1; i++) { //eu vou usar o bubble sort aqui, apesar de nao ser o mais eficiente, é simples e o tamanho do array é pequeno (10), entao nao vai pesar o programa
        for (int j = 0; j < tamanho - i - 1; j++) { //A lógica do bubble sort é comparar o elemento atual com o próximo e trocar se estiverem na ordem errada, isso é repetido até que o array esteja ordenado
            if (arrayEdicoes[j].ano > arrayEdicoes[j + 1].ano) { //A verificacao dos anos vai ser feita do penultimo até o primeiro elemento, para evitar acessar um elemento fora do array, o ultimo elemento nao precisa ser verificado pois ele ja vai estar no lugar certo apos as primeiras passagens
                EdicaoPadrao temp = arrayEdicoes[j]; 
                arrayEdicoes[j] = arrayEdicoes[j + 1];
                arrayEdicoes[j + 1] = temp;
            }
        }
    }
}

//Funçao que verifica se o ID ja existe na ediçao
int verificarId(EdicaoPadrao* ed, int id){
    for(int i = 0; i < ed->qtdAtletas; i++){
        if(ed->IDsatl[i] == id){
            return 1; //Retorno 1 se o ID ja existir
            }
        }
    return 0; //Retorno 0 se o ID nao existir
}

void processarAtleta(EdicaoPadrao* arrayEdicoes, int ano, int id, char* tipo){
    int indexEdicao = -1;

    //Tenta encontrar a edição já existente no array
    for(int i = 0; i < 10; i++){
        if(arrayEdicoes[i].ano == ano){
            indexEdicao = i;
            break;
        }
    }
    //Se não encontrou o ano, procura um slot vazio (ano == 0)
    if (indexEdicao == -1) {
        for(int i = 0; i < 10; i++){
            if(arrayEdicoes[i].ano == 0){
                arrayEdicoes[i].ano = ano;
                strcpy(arrayEdicoes[i].tipo, tipo);
                arrayEdicoes[i].qtdAtletas = 0; // Inicializa contador
                indexEdicao = i;
                break;
            }
        }
    }

    // Se não achou lugar (teoricamente impossível se a lógica do anovalido estiver certa), retorna
    if (indexEdicao == -1) return;

    //Verifica se o ID já existe nessa edição. Se não, adiciona
    if (!verificarId(&arrayEdicoes[indexEdicao], id)) {
        int pos = arrayEdicoes[indexEdicao].qtdAtletas;
        if (pos < 200000) { // Proteção contra overflow do array de IDs
            arrayEdicoes[indexEdicao].IDsatl[pos] = id;
            arrayEdicoes[indexEdicao].qtdAtletas++;
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
    while(fgets(linha, sizeof(linha), arquivo)){ //Enquanto houver linhas no arquivo o programa continua lendo e preenchendo os arrays
        int col = 0;
        int ano = 0;
        char linhaTemp[2000];
        strcpy(linhaTemp, linha); //Crio uma cópia da linha para evitar problemas com o strtok
        char tipo[15] = "";
        char* token = strtok(linha, ",");
        if(token != NULL){
            sscanf(token, "%d %9s", &ano, tipo); //Pego o ano e o tipo da olimpíada
        } if(ano > 1800){ //Verifico se o ano é válido para evitar lixo na memória, por garantia
            if(strstr(tipo, "Summer") != NULL){
                insereAno(anosVerao, ano); //Chamo a funçao que insere o ano no array de anos de verão
            }
            else if(strstr(tipo, "Winter") != NULL){
                insereAno(anosInverno, ano); //Chamo a funçao que insere o ano no array de anos de inverno
            }
        }
    }
    rewind(arquivo); //Volto o ponteiro do arquivo para o inicio

    while(fgets(linha, sizeof(linha), arquivo)){ //Leio o arquivo novamente para preencher as estruturas
        int col = 0;
        int ano = 0;
        int id = 0;
        char tipo[15] = "";
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
        if(strstr(tipo, "Summer") != NULL && anovalido(anosVerao, ano)){ //Verifico se o tipo é verão e se o ano é válido
            processarAtleta(verao, ano, id, tipo); //Chamo a funçao que processa o atleta para adicionar o ID na ediçao correspondente
        }
        else if(strstr(tipo, "Winter") != NULL && anovalido(anosInverno, ano)){
            processarAtleta(inverno, ano, id, tipo);
        }
    }
    fclose(arquivo); //Fecho o arquivo após o uso, é uma boa prática para liberar recursos do sistema
    ordenaredicoes(verao, 10); //Ordeno as ediçoes para facilitar a leitura dos dados
    ordenaredicoes(inverno, 10);

    printf("Evolução | Olimpíadas de Verão:\n");
    for(int i = 0; i < 10; i++){
        if(verao[i].ano != 0){ //Imprimo apenas os anos que foram preenchidos, por segurança e boa prática, para evitar imprimir anos vazios
            printf("Ano: %d, Quantidade de Atletas: %d\n", verao[i].ano, verao[i].qtdAtletas);
        }
    }
    printf("Evolução | Olimpíadas de Inverno:\n");
    for(int i = 0; i < 10; i++){
        if(inverno[i].ano != 0){
            printf("Ano: %d, Quantidade de Atletas: %d\n", inverno[i].ano, inverno[i].qtdAtletas);
        }
    }
}