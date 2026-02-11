#ifndef ATLETA_H
#define ATLETA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <windows.h>

// Dados de cada atleta (nome, país, gênero, pior posição)
typedef struct
{
    char name[256];    // Used Name
    char country[100]; // NOC
    char sex[12];      // Sex
    char born[50];     // Born
    char game[50];     // Games
    char sport[50];    // Discipline
    int id;            // athlete_id
    int position;      // Pos
    int age;           // Game year - Birth year
    int birth_year;
    int results;       // Pontuação final
} Athlete;

typedef struct 
{
    char name[100];    // Discipline
    int maxPos;        // Pior posição (de maior índice)
} DisciplineStats;

// Funções de Processamento Modularizadas
void ConfigurarTerminal();
void CarregarBiografias(Athlete *allAthletes, int athletesLimit);
int MapearPiorPosicaoEsportes(DisciplineStats *disciplines, int disciplinesLimit);
void ProcessarResultadosIndividuais(Athlete *allAthletes, int athletesLimit);
void CalcularRankingsFinais(Athlete *allAthletes, int athletesLimit, DisciplineStats *disciplines, int disciplinesQuantity, Athlete *athletesMasc, Athlete *athletesFem, int *cuttingResults, int *filledAthletes);
void ExibirResultados(Athlete *athletesMasc, Athlete *athletesFem, int *filledAthletes);

// Funções de Gráfico
void GerarArquivosGnuplotRankings(Athlete *athletesMasc, Athlete *athletesFem, int *filledAthletes);
void GerarCodigoGnuplotRankings();

// Funções Utilitárias e de Lógica
void ObterCampo(char *line, int column, char *destiny);
int CalculaPioresAtletasNovos(Athlete *athletesMasc, Athlete *athletesFem, int *cuttingResults, int *filledAthletes, Athlete nextAthlete);
int AlteraListaDosPiores(Athlete *athletesList, Athlete athlete, int *cuttingResults, int *filledAthletes);
int CalculaResultados(int age, int position, int maxPosInSport);
int ExtrairAno(char *str);
void NormalizarString(char *nome);
void LimparNome(char *nome);
void RemoverAcentos(char *nome);

#endif