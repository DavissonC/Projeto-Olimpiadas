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
char* ler_campo(char** cursor);

void insereAno(int *anos, int ano) { //Funçao que insere o ano no array de anos, se ja existir, nao insere novamente
    //Verifica se o ano já existe no array para não duplicar
    for (int i = 0; i < 10; i++) {
        if (anos[i] == ano) return;
    }
    //Procura um slot vazio (0)
    for (int i = 0; i < 10; i++) {
        if (anos[i] == 0) {
            anos[i] = ano;
            return;
        }
    }
    //Se o array está cheio, encontra o índice do MENOR ano atual
    int indiceMenor = 0;
    for (int i = 1; i < 10; i++) {
        if (anos[i] < anos[indiceMenor]) {
            indiceMenor = i;
        }
    }
    //Se o ano lido for MAIOR que o menor ano guardado, substitui
    if (ano > anos[indiceMenor]) {
        anos[indiceMenor] = ano;
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

//Esta função pega o texto até a próxima vírgula, retornando vazio se não tiver nada, ela vai servir para ler os campos do CSV corretamente, mesmo que tenham aspas ou vírgulas internas, tratando esses casos especiais
char* ler_campo(char** cursor) {
    char* inicio = *cursor;
    if (inicio == NULL) return NULL;
    char* atual = inicio;
    int dentro_aspas = 0;

    //Se o campo começa com aspas, pulamos a aspa inicial
    if (*atual == '"') {
        dentro_aspas = 1;
        inicio++;
        atual++;
    }
    while (*atual != '\0') {
        //Se achou outra aspa, inverte o estado (abre/fecha)
        if (*atual == '"') {
            // Verifica se é aspa dupla ("") que conta como aspa literal, ou fechamento
            //É importante que só vamos parar na virgula se dentro_aspas for 0
            if (dentro_aspas && *(atual+1) == ',') {
               dentro_aspas = 0; // Vai fechar na proxima checagem
               *atual = '\0'; // Remove a aspa final
            }
        }
        // Se achou vírgula E NÃO está dentro de aspas, é o fim do campo
        else if (*atual == ',' && !dentro_aspas) {
            *atual = '\0';     // Corta a string aqui
            *cursor = atual + 1; // Próxima leitura começa depois da vírgula
            return inicio;
        }
        // Tratamento de fim de linha (\n ou \r)
        else if ((*atual == '\n' || *atual == '\r') && !dentro_aspas) {
             *atual = '\0';
             *cursor = NULL; // Indica que a linha acabou
             // Remove aspa final se houver (caso: ..."Texto"\n)
             if (atual > inicio && *(atual-1) == '"') *(atual-1) = '\0';
                return inicio;
        }
        atual++;
    }
    // Se chegou no fim da string sem achar virgula
    *cursor = NULL;
    return inicio;
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

//Para processar um gráfico de dados, utilizando o gnuplot, é necessário organizar os dados em um formato específico, geralmente em colunas, onde cada coluna representa uma variável diferente. No caso da evolução do número de atletas nas Olimpíadas de Verão e Inverno, com isso organizo os dados da seguinte maneira:

void gerarArquivosGnuplot(EdicaoPadrao* verao, EdicaoPadrao* inverno) {
    FILE* fVerao = fopen("verao.dat", "w");
    FILE* fInverno = fopen("inverno.dat", "w");

    if (!fVerao || !fInverno) {
        printf("Erro ao criar arquivos .dat\n");
        return;
    }

    for (int i = 0; i < 10; i++) {
        if (verao[i].ano != 0) {
            fprintf(fVerao, "%d %d\n", verao[i].ano, verao[i].qtdAtletas);
        }
    }

    for (int i = 0; i < 10; i++) {
        if (inverno[i].ano != 0) {
            fprintf(fInverno, "%d %d\n", inverno[i].ano, inverno[i].qtdAtletas);
        }
    }

    fclose(fVerao);
    fclose(fInverno);
}

void gerarScriptGnuplot() { //Aqui eu crio um script para o gnuplot, que é uma ferramenta de plotagem de gráficos, esse script vai ler os arquivos .dat gerados anteriormente e criar um gráfico de barras para cada tipo de Olimpíada, configurando também seu layout e títulos para facilitar a visualização dos dados
    FILE* f = fopen("grafico.gp", "w");
    if (!f) {
        printf("Erro ao criar grafico.gp\n");
        return;
    }

    fprintf(f, "set terminal qt size 1000,800\n");
    fprintf(f, "set multiplot layout 2,1 title \"Evolucao de Atletas: Primeiras 10 Edicoes\" font \",16\"\n\n");

    fprintf(f, "set key top left\n");
    fprintf(f, "set grid y\n\n");

    fprintf(f, "set style data histograms\n");
    fprintf(f, "set style fill solid 0.8 border -1\n");
    fprintf(f, "set boxwidth 1.5 relative\n\n");

    fprintf(f, "set xtics rotate by 0\n");
    fprintf(f, "set ylabel \"Qtd de Atletas\"\n\n");

    fprintf(f, "set title \"Jogos de Verao\" font \",13\"\n");
    fprintf(f, "plot \"verao.dat\" using 2:xtic(1) lc rgb \"#0076fc\" title \"Atletas Verao\"\n\n");

    fprintf(f, "set title \"Jogos de Inverno\" font \",13\"\n");
    fprintf(f, "set xlabel \"Ano da Edicao\"\n");
    fprintf(f, "plot \"inverno.dat\" using 2:xtic(1) lc rgb \"#90eeff\" title \"Atletas Inverno\"\n\n");

    fprintf(f, "unset multiplot\n");

    fclose(f);
}

int main()
{
    FILE* arquivo = fopen("../results.csv","r"); //Por segurança verifico se o ponteiro nao está pegando lixo na memória
    if(arquivo == NULL){
        printf("Não foi possível executar o arquivo");
        return 1;
    }
    char linha[4000];
    char linhaTemp[4000]; //Crio uma cópia da linha para evitar problemas com o strtok
    int anosVerao[10] = {0}; //Array que guarda os anos das ediçoes de verão
    int anosInverno[10] = {0}; //Array que guarda os anos das ediçoes de inverno
    while(fgets(linha, sizeof(linha), arquivo)){ //Enquanto houver linhas no arquivo o programa continua lendo e preenchendo os arrays
        char* cursor = linha;
        int col = 0;
        int ano = 0;
        char tipo[15] = "";
        char* token;
        int ehYouth = 0; //Variável que verifica se a ediçao é Youth, caso seja, ela nao conta para o total de ediçoes
        while((token = ler_campo(&cursor)) != NULL){
            if(col == 0){
                if(strstr(token, "Youth") != NULL) {
                    ehYouth = 1;
                }
                sscanf(token, "%d %s", &ano, tipo);
            }
            col++;
        }
        if(ano > 1800 && !ehYouth){ //Verifico se o ano é válido para evitar lixo na memória, por garantia e também verifico se a edição não é Youth
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
        char* token;
        char* cursor = linha;
        int ehYouth = 0; //Variável que verifica se a ediçao é Youth, caso seja, ela nao conta para o total de ediçoes
        while((token = ler_campo(&cursor)) != NULL){ //O código aqui é similar ao anterior só que agora chamo a funçao que adiciona os IDs
            if(col == 0){
                if(strstr(token, "Youth") != NULL) {
                    ehYouth = 1;
                }
                sscanf(token, "%d %s", &ano, tipo);
            }
            else if(col == 6){
                id = atoi(token);
            }
            col++;
        }
        if(ano > 0 && id > 0 && !ehYouth){
            if(strstr(tipo, "Summer") && anovalido(anosVerao, ano)) processarAtleta(verao, ano, id, tipo);
            else if(strstr(tipo, "Winter") && anovalido(anosInverno, ano)) processarAtleta(inverno, ano, id, tipo);
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
    printf("\n");
    printf("Evolução | Olimpíadas de Inverno:\n");
    for(int i = 0; i < 10; i++){
        if(inverno[i].ano != 0){
            printf("Ano: %d, Quantidade de Atletas: %d\n", inverno[i].ano, inverno[i].qtdAtletas);
        }
    }
    //Agora que temos os dados organizados, podemos gerar os arquivos para o gnuplot e o script para plotar os gráficos
    gerarArquivosGnuplot(verao, inverno);
    gerarScriptGnuplot();


    system("start gnuplot -p grafico.gp"); //Aqui eu chamo o gnuplot para executar o script que gera os gráficos, a opção -p é para manter a janela do gráfico aberta após a execução
}