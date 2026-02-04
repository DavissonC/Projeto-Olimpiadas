/*Calcule quantos atletas participaram das últimas 10 edições dos Jogos Olímpicos e analise a evolução
desse número ao longo do tempo, separando Verão e Inverno.*/


typedef struct{
    int ano;
    int qtdatlet;
}EdicaoPadrao;


int fcalatletasultimos10an(){ //Por boa prática divido as açoes da funçao principal em outra funçao mais especifica
    for(int i = 0; i<)


}


int main()
{
    FILE* arquivo = fopen("results.csv","r"); //Por segurança verifico se o ponteiro nao está pegando lixo na memória
    if(arquivo == NULL){ //Com isso evito um retorno nao esperado, no caso, que nao seja abrir o arquivo
        printf("Não foi possível executar o arquivo");
        return 1;
    }
    EdicaoPadrao verao[10]; //Para isso eu pego a estrutura de uma unica ediçao de olimpiada e divido, depois preencho um array de tamanho 10
    EdicaoPadrao inverno[10];

}