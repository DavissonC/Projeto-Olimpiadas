//Calcule quantos atletas participaram das primeiras 10 edições dos Jogos Olímpicos e analise a evolução
//desse número ao longo do tempo, separando Verão e Inverno.

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
    char estacao[30];
} Registro; //Struct que será utilizado para ordenação

int compararRegistros(const void *ptrGenericoA, const void *ptrGenericoB) {

    Registro *registroAtletaA = (Registro *)ptrGenericoA;
    Registro *registroAtletaB = (Registro *)ptrGenericoB;

    if (registroAtletaA->ano != registroAtletaB->ano) { // Compara o ano da copa
        return (registroAtletaA->ano - registroAtletaB->ano); //se der neg, A vem antes de B
    }

    return (registroAtletaA->id - registroAtletaB->id); // Se os anos forem iguais, compara o ID do atleta
} //Função de comparação para o qsort, usando ano e ID

void pegarColuna(char *linha, int colunaAlvo, char *resultado) { //Pega uma coluna específica e ignora virgulas entre aspas
    int colunaAtual = 0;
    int dentroDeAspas = 0; //Se for 0, está fora das aspas. Se for 1, está dentro das aspas
    int j = 0;

    for (int i = 0; linha[i] != '\0'; i++) { //Percorre até o fim da linha
        if (linha[i] == '\"') {
            dentroDeAspas = !dentroDeAspas; //Muda o valor a cada aspas
        } 
        else if (linha[i] == ',' && !dentroDeAspas) {
            if (colunaAtual == colunaAlvo) {
                break;
            } //Para o código se já processou a coluna desejada
            colunaAtual++; //Passa pra próxima coluna se não estiver na desejada
        } 
        else if (colunaAtual == colunaAlvo) {
            resultado[j++] = linha[i]; //Salva os char lidos em sequência
        }
    }
    resultado[j] = '\0'; //Põe o char nulo no final do array
}

void GerarArquivosGnuplot(Edicao *verao, int totalVerao, Edicao *inverno, int totalInverno) { //Cria arquivos .dat com os dados para gerar os gráficos
    FILE *fVerao = fopen("verao.dat", "w"); //abre o arquivo para escrita ("w")
    if (fVerao == NULL) {
        printf("Erro ao criar verao.dat.\n");
        return;
    }
    for (int i = 0; i < totalVerao; i++) { // Escreve ano e qtd de atletas no arquivo
        fprintf(fVerao, "%d %d\n", verao[i].ano, verao[i].qtdAtletas);
    }
    fclose(fVerao);

    FILE *fInverno = fopen("inverno.dat", "w"); //Faz a mesma coisa para o verão
    if (fInverno == NULL) {
        printf("Erro ao criar inverno.dat.\n");
        return;
    }
    for (int i = 0; i < totalInverno; i++) {
        fprintf(fInverno, "%d %d\n", inverno[i].ano, inverno[i].qtdAtletas);
    }
    fclose(fInverno);
}

void GerarCodigoGnuplot() { //Cria um arquivo gnuplot que configura os gráficos
    FILE *fCodigo = fopen("grafico.gp", "w");
    if (fCodigo == NULL) {
        printf("Erro ao criar o arquivo de script.\n");
        return;
    }

    fprintf(fCodigo, "set terminal qt size 1000,800\n");
    fprintf(fCodigo, "set multiplot layout 2,1 title \"Evolucao de Atletas: Primeiras 10 Edicoes\" font \",16\"\n\n");

    fprintf(fCodigo, "set key top left\n");
    fprintf(fCodigo, "set grid y\n\n");

    fprintf(fCodigo, "set style data histograms\n");
    fprintf(fCodigo, "set style fill solid 0.8 border -1\n");
    fprintf(fCodigo, "set boxwidth 1.5 relative\n\n");

    fprintf(fCodigo, "set xtics rotate by 0\n");
    fprintf(fCodigo, "set ylabel \"Qtd de Atletas\"\n\n");

    fprintf(fCodigo, "set title \"Jogos de Verao\" font \",13\"\n");
    fprintf(fCodigo, "plot \"verao.dat\" using 2:xtic(1) lc rgb \"#fcbd00\" title \"Atletas Verao\"\n\n");

    fprintf(fCodigo, "set title \"Jogos de Inverno\" font \",13\"\n");
    fprintf(fCodigo, "set xlabel \"Ano da Edicao\"\n");
    fprintf(fCodigo, "plot \"inverno.dat\" using 2:xtic(1) lc rgb \"#90eeff\" title \"Atletas Inverno\"\n\n");

    fprintf(fCodigo, "unset multiplot\n");

    fclose(fCodigo);
}

int main() {
    FILE *arquivo = fopen("../results.csv", "r");
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

    fgets(linha, sizeof(linha), arquivo); // Lê o cabeçalho e não faz nada com ele

    while (fgets(linha, sizeof(linha), arquivo) && totalLido < totalLinhas) {
        char tokenOlimpiada[100], tokenID[50];

        pegarColuna(linha, 0, tokenOlimpiada);  //Coluna 0 tem ano e estação
        
        pegarColuna(linha, 6, tokenID); // Coluna 6 tem o ID do Atleta

        sscanf(tokenOlimpiada, "%d %[^\n]", &totalRegistros[totalLido].ano, totalRegistros[totalLido].estacao);
        
        totalRegistros[totalLido].id = atoi(tokenID);
        totalLido++;
    }

    fclose(arquivo);

    qsort(totalRegistros, totalLido, sizeof(Registro), compararRegistros);  //ordena por Ano e ID

    for (int i = 0; i < totalLido; i++) { //Processa os dados ordenados
        int ano = totalRegistros[i].ano;
        char *estacao = totalRegistros[i].estacao;
        int id = totalRegistros[i].id;

        if (strcmp(estacao, "Summer Olympics") == 0) {
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
                } //A quantidade de atletas só será incrementada se for um atleta diferente (com base no ID)
            }
        }
        if (strcmp(estacao, "Winter Olympics") == 0) { //Alteração do trecho de cima para as Olimpíadas de inverno
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

    printf("\n+--------+------------------------+------------------------+");
    printf("\n|        |        VERAO           |        INVERNO         |");
    printf("\n+--------+-------+----------------+-------+----------------+");
    printf("\n| Edicao |  Ano  | Qtd de atletas |  Ano  | Qtd de atletas |");
    printf("\n+--------+-------+----------------+-------+----------------+\n");

    for (int i = 0; i < 10; i++) {
        int anoVerao = (i < totalVerao) ? verao[i].ano : 0;
        int qtdVerao = (i < totalVerao) ? verao[i].qtdAtletas : 0;
        int anoInverno = (i < totalInverno) ? inverno[i].ano : 0;
        int qtdInverno = (i < totalInverno) ? inverno[i].qtdAtletas : 0;
        printf("|   %02d   | %5d |     %5d      | %5d |     %5d      |\n", 
               i + 1, anoVerao, qtdVerao, anoInverno, qtdInverno);
    }
    printf("+--------+-------+----------------+-------+----------------+\n");

    GerarArquivosGnuplot(verao, totalVerao, inverno, totalInverno);

    GerarCodigoGnuplot();

    system("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\" -p grafico.gp"); //O caminho relativo ao executavel do gnuplot (talvez precise mudar ao rodar em outra máquina)

    free(totalRegistros); // Libera a memória
    return 0;
}
