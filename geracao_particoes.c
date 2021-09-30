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
            int j;
            for (j = 1; j < M; j++) {
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
                int i;
                for (i = 0; i < M; i++) {
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
    char *nome_particao;
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
                Cliente *menor;
                int indiceMenor;
                /* ler os M valores do arquivo e guarda na memoria */
                int i = 0;
                while (!feof(arq) && i < M) {
                    memoria[i] = cin;
                    cin = le_cliente(arq);
                    i++;
                }

                //ajusta tamanho M caso arquivo de entrada tenha terminado antes do vetor
                if (i != M) {
                    M = i;
                }
                menor = le_cliente(arq);
                if(menor != NULL){
                        /*
                    while(n != 0){
                        menor = memoria[0];
                        indiceMenor = 0;
                        // encontra o menor valor na memoria
                        int i;
                        for(i = 0; i<M; i++){
                            if(memoria[i]->cod_cliente < menor->cod_cliente){
                                    indiceMenor = i;
                                }
                        }
                        FILE *p;
                        p = fopen(nome_particao, "wb");
                        salva_cliente(memoria[indiceMenor],p);
                        if(memoria[indiceMenor]->cod_cliente > menor->cod_cliente){

                        }
                        n = 0;
                    }
                    nome_particao = nome_arquivos_saida->nome;
                    nome_arquivos_saida = nome_arquivos_saida->prox;
                    */
                    M = M;
                }else{ // aqui Thiago
                        // caso menor seja nulo, basta apenas orderar a memoria e salvar
                        //faz ordenacao



                        /* Objetivos desse else
                                1-gravar na memoria
                                2-ordena reservatorio na memoria
                                3-guarda na particao nova
                                4-fechar particao Nova
                        */



                        //--------------------1)guarda o que ta na memoria no arquivo
                        int k=0;

                        FILE* particaoAtual;
                        particaoAtual = fopen(nome_particao, "wb");

                        if(particaoAtual == NULL){
                            //printf("Erro ao criar arquivo particao");
                            return;
                        }

                        for(int cont = 0; cont < M; cont++){
                            i = 0;
                            menor = memoria[i];

                            for(int j = 1; j < M; j++){
                                if(memoria[j]->cod_cliente < menor->cod_cliente){
                                    menor = memoria[j];
                                    i = j;
                                }
                            }

                            salva_cliente(menor, particaoAtual);
                            //printf("\n%d\n",menor->cod_cliente);
                            memoria[i]->cod_cliente = 99999; //INT_MAX?
                        }
                        fclose(particaoAtual);


                        if(feof(arq)){
                            fim = 1;
                        }
                        //---------------------------------



                        //nome_particao = nome_arquivos_saida->nome;
                        //nome_arquivos_saida = nome_arquivos_saida->prox;
                        //


/*

                        //--------------------2)retira do reservatório e insere ordenado na memoria
                        FILE *FReservatorio;
                        FReservatorio = fopen("Reservatorio.dat", "rb");

                        if(FReservatorio == NULL){
                            printf("\nERRO NA CRIAÇÃO DO RESERVATORIO!");
                            return;
                        }

                        Cliente* cliente;
                        i = 0;
                        do{


                            fread(&cliente->cod_cliente, sizeof(int), 1, FReservatorio);
                            fread(cliente->nome, sizeof(char), sizeof(cliente->nome), FReservatorio);

                            //printf(" | i -> %d",i++);


                            if(cliente == NULL){
                                printf("SEM CLIENTE!");
                                //desalocar o cliente?
                            }
                            else{
                                //printf("cliente atual -> %d",cliente->cod_cliente);
                                memoria[i++] = cliente;
                            }

                        }while(cliente && i < M);
                        //---------------------------------




                        //printf("COD-> %d",memoria[0]->cod_cliente);

                        //printf(" | M -> %d\n",M);

                        if(cliente){

                            int j=0;
                            k = 0;

                            particaoAtual = fopen(nome_particao, "wb");


                            //ordena reservatorio na memoria
                            while(j < M){ //e se o n for maior que o m?
                                k = 0;
                                menor = memoria[k];
                                for(i = 1; i < M; i++){
                                    if (memoria[i]->cod_cliente < menor->cod_cliente){
                                        menor = memoria[i];
                                        k = i; //entrega a k a posição do menor valor
                                    }
                                }
                                //guarda na particao nova
                                salva_cliente(menor, particaoAtual); //3-guarda na particao nova
                                memoria[k]->cod_cliente = 99999;
                                j++;

                            }

                            fclose(particaoAtual);

                        }
                        fclose(FReservatorio);


                        //fechar particao Nova
                        //nome_particao = nome_arquivos_saida->nome;
                        //nome_arquivos_saida = nome_arquivos_saida->prox;
*/
                    }
//TERMINA AQUI

                //printf("\nAqui --- %d - %s ---",memoria[indiceMenor]->cod_cliente, memoria[indiceMenor]->nome);


                //fim = 1;
            }


        }
    }
}



//void salvaDaMemoria
