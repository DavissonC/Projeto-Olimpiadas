# 🏅 Análise de Dados Olímpicos (1896-2022)

Este projeto foi desenvolvido como conclusão para a disciplina de **Programação Imperativa (PI)** do curso de Engenharia de Computação na **Universidade Federal de Sergipe (UFS)**, sob a orientação do **Prof. Dr. Kalil Bispo**.

O sistema processa bases de dados históricas para responder a questões complexas sobre o desempenho e a demografia de atletas ao longo das décadas, utilizando técnicas de manipulação de arquivos e memória em **C.**

## 👥 Equipe
* Bruno Danton ([BrunoDanton](https://github.com/BrunoDanton))
* Davísson Cavalcante ([DavissonC](https://github.com/DavissonC))
* Eduardo Souza ([EdmSouza](https://github.com/EdmSouza))
* Mateus Aranha ([matt-aranha](https://github.com/matt-aranha))

---

## 🚀 O Projeto

O software realiza o *parsing* de arquivos CSV de larga escala (mais de 300.000 registros) para extrair dados estatísticos. O nosso foco principal foi a criação de um código limpo, modularizado e eficiente.

### Questões Respondidas
1.  **Evolução das Últimas 10 Edições (Q9):** Análise do crescimento do número de atletas nas Olimpíadas de Verão e Inverno da era moderna.
2.  **Evolução das Primeiras 10 Edições (Q9 - Autoral):** Análise do crescimento do número de atletas nas Olimpíadas de Verão e Inverno do início da competição.
3.  **Ranking de Medalhistas Mais Velhos (Q19):** Identificação dos 10 atletas mais velhos a conquistarem medalhas, segmentados por gênero.
4.  **Ranking de Atletas Mais Novos e Piores Colocados (Q19 - Autoral):** Uma análise estatística inversa para identificar jovens estreantes e sua performance.

---

## 🛠️ Tecnologias e Bibliotecas

Para garantir eficiência e a pontuação extra de interface visual, utilizamos:

* **Linguagem C:** Implementação seguindo o paradigma imperativo com modularização em arquivos `.c` e `.h`.
* **Gnuplot:** Geração automática de gráficos de barras e histogramas via scripts `.gp`.
* **Raylib:** Biblioteca gráfica para a criação de uma interface (GUI) interativa e responsiva.
* **Gestão de Memória:** Uso de alocação dinâmica (`malloc`, `calloc`) para processar grandes volumes de dados sem estourar a pilha (stack).

---

## ⚙️ Como Executar

1.  **Pré-requisitos:**
    * Compilador GCC.
    * [Gnuplot](http://www.gnuplot.info/) instalado e configurado no PATH do sistema.
    * Biblioteca [Raylib](https://www.raylib.com/) instalada.

2.  **Arquivos de Dados:**
    * Os arquivos `bios.csv` e `results.csv` devem estar na pasta superior (`../`) para serem lidos corretamente pelo programa.
    * Para obtê-los, baixe do seguinte repositório: ([Olympics-Dataset](https://github.com/KeithGalli/Olympics-Dataset)) 

3.  **Compilação:**
   * Q09:          
    

    gcc 

   

   * Q09 - Autoral:       
    

    gcc 
   

   * Q19:         

   
    gcc analise.c gui.c leitura.c main.c -o questao_19_versao_original -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    

  * Q19 - Autoral: 
   ```
   gcc 
   ```


---
#### **© 2025. All rights reserved. Federal University of Sergipe.**
