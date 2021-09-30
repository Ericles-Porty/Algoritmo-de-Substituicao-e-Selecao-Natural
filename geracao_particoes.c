//
// Created by Vanessa Braganholo on 16/09/2018.
// Updated by Raphael Oliveira on 18/09/2021.

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include "geracao_particoes.h"
#include "nomes.h"
#include "cliente.h"

void classificacao_interna(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M)
{
	int fim = 0; //variável de controle para saber se arquivo de entrada terminou
    FILE *arq; //declara ponteiro para arquivo

    //abre arquivo para leitura
    if ((arq = fopen(nome_arquivo_entrada, "rb")) == NULL) {
        printf("Erro ao abrir arquivo de entrada\n");
    } else {

        //le primeiro cliente
        Cliente *cin = le_cliente(arq);

        while (!(fim)) {
            //le o arquivo e coloca no vetor
            Cliente *v[M];

            int i = 0;
            while (!feof(arq) && i < M) {
                v[i] = cin;
                cin = le_cliente(arq);
                i++;
            }

            //ajusta tamanho M caso arquivo de entrada tenha terminado antes do vetor
            if (i != M) {
                M = i;
            }

            //faz ordenacao
            for (int j = 1; j < M; j++) {
                Cliente *c = v[j];
                i = j - 1;
                while ((i >= 0) && (v[i]->cod_cliente > c->cod_cliente)) {
                    v[i + 1] = v[i];
                    i = i - 1;
                }
                v[i + 1] = c;
            }

            //cria arquivo de particao e faz gravacao
            char *nome_particao = nome_arquivos_saida->nome;
            nome_arquivos_saida = nome_arquivos_saida->prox;

            FILE *p;
            if ((p = fopen(nome_particao, "wb")) == NULL) {
                printf("Erro criar arquivo de saida\n");
            } else {
                for (int i = 0; i < M; i++) {
					salva_cliente(v[i], p);
                }
                fclose(p);
            }
            if (feof(arq)) {
                fim = 1;
            }
        }
    }
}

void selecao_com_substituicao(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M)
{
	//TODO: Inserir aqui o codigo do algoritmo de geracao de particoes
}

void selecao_natural(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M, int n)
{
	int fim = 0; //variável de controle para saber se arquivo de entrada terminou
    FILE *arq; //declara ponteiro para arquivo
    /* nome_particao = nome do arquivo de saida ex: "p1.dat","p2.dat" */
    char *nome_particao = nome_arquivos_saida->nome;


        nome_particao = nome_arquivos_saida->nome;
        nome_arquivos_saida = nome_arquivos_saida->prox;

    int contParti = 0; // conta quantos clientes ja foram salvos por particoes
    //abre arquivo para leitura
    if ((arq = fopen(nome_arquivo_entrada, "rb")) == NULL) {
        printf("Erro ao abrir arquivo de entrada\n");
    } else {
        //le primeiro cliente
        Cliente *cin = le_cliente(arq);
        /* se o primeiro cliente e nulo, a lista esta vazia */
        if(cin == NULL){
            //passa no teste 1
            fopen(nome_particao, "wb");
        }else{
            while(!fim){

                /* memoria para guardar M valores */
                Cliente *memoria[M];
                Cliente *menor; // auxiliar para encotnrar o menor cliente na memoria
                Cliente *prox; // guarda o proximo cliente lido appos o 6 da memoria
                int indiceMenor;
                int part = 1;
                /* ler os M valores do arquivo e guarda na memoria */
                int i = 0;
                while (!feof(arq) && i < M) {
                    memoria[i] = cin;
                    cin = le_cliente(arq); // em cin fica o 7 cliente
                    i++;
                }

                //ajusta tamanho M caso arquivo de entrada tenha terminado antes do vetor
                if (i != M) {
                    M = i;
                }
                prox = cin;
                if(prox != NULL){
                    FILE *reservatorio;
                    FILE *p;
                    while(prox != NULL){
                            reservatorio = fopen("Reservatorio.dat", "wb");
                            p = fopen(nome_particao, "wb");
                            //printf("\n%s",nome_particao); // esse printf mostra o nome dos arquivos
                            /*
                            if(part == 1)
                                p = fopen("p1.dat", "wb");
                            else if(part == 2)
                                p = fopen("p2.dat", "wb");
                            else if(part == 3)
                                p = fopen("p3.dat", "wb");
                            else if(part == 4)
                                p = fopen("p4.dat", "wb");
                            */
                        while(n != 0){
                            menor = memoria[0];
                            indiceMenor = 0;
                            // encontra o menor valor na memoria
                            int i;
                            for(i = 0; i<M; i++){
                                if(memoria[i]->cod_cliente < menor->cod_cliente){
                                        menor = memoria[i];
                                        indiceMenor = i;
                                    }
                            }

                            //printf("\n--- %d - %s ---",memoria[indiceMenor]->cod_cliente, memoria[indiceMenor]->nome);
                            if(memoria[indiceMenor]->cod_cliente > prox->cod_cliente){

                                //printf("\nReserva --- %d - %s ---",prox->cod_cliente, prox->nome);
                                salva_cliente(prox,reservatorio);
                                n--;
                            }else{
                                salva_cliente(memoria[indiceMenor],p);
                                printf("\n--- %d - %s ---",memoria[indiceMenor]->cod_cliente, memoria[indiceMenor]->nome);

                                memoria[indiceMenor] = prox;
                                //printf("\nTrocou --- %d - %s ---",memoria[indiceMenor]->cod_cliente, memoria[indiceMenor]->nome);
                                //printf("\nNovoMenor --- %d - %s ---",memoria[indiceMenor]->cod_cliente, memoria[indiceMenor]->nome);
                            }
                            //printf("\nProx --- %d - %s ---",prox->cod_cliente, prox->nome);
                            prox = le_cliente(arq);
                            if(prox == NULL){
                                //M = M - n;

                                n = 0;
                            }
                        }
                        fclose(reservatorio);

                        for (int j = 1; j < M; j++) {
                                        Cliente *c = memoria[j];
                                        i = j - 1;
                                        while ((i >= 0) && (memoria[i]->cod_cliente > c->cod_cliente)) {
                                            memoria[i + 1] = memoria[i];
                                            i = i - 1;
                                        }
                                        memoria[i + 1] = c;
                                    }
                                    //salva no .dat
                                        for (int i = 0; i < M; i++) {
                                            printf("\n--- %d - %s ---",memoria[i]->cod_cliente, memoria[i]->nome);
                                            salva_cliente(memoria[i], p);
                                        }
                                        printf("\n");
                                        fclose(p);

                        FILE* reserva = fopen("Reservatorio.dat", "rb");
                        int i = 0;
                        while (!feof(reserva) && i < M) {
                            memoria[i] = le_cliente(reserva); // ler os 6 clientes do reservatorio
                            if(memoria[i] == NULL)
                                break;
                            //printf("\nAqui %d --- %d - %s ---",i,memoria[i]->cod_cliente, memoria[i]->nome);
                            // descomentar esse printf, vai forca o erro, se descomentar junto com o if ele roda normal
                            i++;
                            n++;
                        }
                        if (i != M) {
                            M = i;
                        }
                        fclose(reserva);
                        if(nome_arquivos_saida != NULL)
                            nome_particao = nome_arquivos_saida->nome;
                        if(nome_arquivos_saida->prox != NULL)
                            nome_arquivos_saida = nome_arquivos_saida->prox;


                    }
                }
                        // aqui Thiago
                        // caso menor seja nulo, basta apenas orderar a memoria e salvar
                        //faz ordenacao

                            for (int j = 1; j < M; j++) {
                                        Cliente *c = memoria[j];
                                        i = j - 1;
                                        while ((i >= 0) && (memoria[i]->cod_cliente > c->cod_cliente)) {
                                            memoria[i + 1] = memoria[i];
                                            i = i - 1;
                                        }
                                        memoria[i + 1] = c;
                                    }
                            //salva no .dat
                            FILE *p;

                                p = fopen(nome_particao, "wb");


                                for (int i = 0; i < M; i++) {
                                    salva_cliente(memoria[i], p);
                                }

                                fclose(p);



                //printf("\nAqui --- %d - %s ---",memoria[indiceMenor]->cod_cliente, memoria[indiceMenor]->nome);
                fim = 1;
            }

        }
    }
}

