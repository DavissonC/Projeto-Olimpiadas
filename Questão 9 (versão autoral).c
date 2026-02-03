//Calcule quantos atletas participaram das primeiras 10 edições dos Jogos Olímpicos e analise a evolução
//desse número ao longo do tempo, separando Verão e Inverno.

typedef struct {
    int ano;
    int qtdAtletas;
} Edicao; //Struct que guarda o ano e a qtd de atletas

int main() {
    FILE *arquivo = fopen("results.csv", "r");
    // Abre o arquivo e verifica se abriu corretamente
    if (arquivo == NULL) {
        printf("Nao foi possivel abrir o arquivo.\n");
        return 1;
    }
    Edicao verao[10]; //Armazena os dados das 10 primeiras olimpíadas de verão
    Edicao inverno[10]; //Armazena os dados das 10 primeiras olimpíadas de inverno
    int totalVerao = 0, totalInverno = 0;

    char linha[2000];
    int anoAnteriorVerao = -1;
    int anoAnteriorInverno = -1;

    //Preciso ordenar a lista pelo ano

    //Lê a primeira linha (cabeçalho) e não faz nada com ela
    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo)) {
        char *tokenAno = strtok(linha, ","); //Lê o ano
        int ano = atoi(tokenAno); //Transforma string para int

        char *tokenEstacao = strtok(NULL, ","); //Lê a estação
        char *estacao = tokenEstacao; // Armazena a estação em um ponteiro

        char *tokenID = strtok(NULL, ","); //Lê o ID do atleta
        // Preciso verificar se o atleta participou mais de uma vez da mesma edição

        if (strcmp(estacao, "Summer") == 0) {
            if (ano != anoAnteriorVerao && totalVerao < 10) {
                verao[totalVerao].ano = ano;
                verao[totalVerao].qtdAtletas = 1; // Começa a contagem
                anoAnteriorVerao = ano;
                totalVerao++;
            } else if (ano == anoAnteriorVerao && totalVerao <= 10) {
                verao[totalVerao - 1].qtdAtletas++;
            }
        }
        if (strcmp(estacao, "Winter") == 0) {
            if (ano != anoAnteriorInverno && totalInverno < 10) { //Verifica se não é o mesmo ano e se a quantidade de anos desejada(10) não foi atingida
                inverno[totalInverno].ano = ano;
                inverno[totalInverno].qtdAtletas = 1;
                anoAnteriorInverno = ano;
                totalInverno++;
            } else if (ano == anoAnteriorInverno && totalInverno <= 10) {
                inverno[totalInverno - 1].qtdAtletas++;
            }
        }
    }
}
