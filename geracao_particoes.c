//
// Created by Vanessa Braganholo on 16/09/2018.
// Updated by Raphael Oliveira on 18/09/2021.

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#define INT_MAX 2147483647
#include <stdio.h>
#include "geracao_particoes.h"
#include "nomes.h"
#include "cliente.h"
#include <stdlib.h>

void classificacao_interna(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M)
{
    int fim = 0; //variável de controle para saber se arquivo de entrada terminou
    FILE *arq;   //declara ponteiro para arquivo

    //abre arquivo para leitura
    if ((arq = fopen(nome_arquivo_entrada, "rb")) == NULL)
    {
        printf("Erro ao abrir arquivo de entrada\n");
    }
    else
    {

        //le primeiro cliente
        Cliente *cin = le_cliente(arq);

        while (!(fim))
        {
            //le o arquivo e coloca no vetor
            Cliente *v[M];

            int i = 0;
            while (!feof(arq) && i < M)
            {
                v[i] = cin;
                cin = le_cliente(arq);
                i++;
            }

            //ajusta tamanho M caso arquivo de entrada tenha terminado antes do vetor
            if (i != M)
            {
                M = i;
            }

            //faz ordenacao
            for (int j = 1; j < M; j++)
            {
                Cliente *c = v[j];
                i = j - 1;
                while ((i >= 0) && (v[i]->cod_cliente > c->cod_cliente))
                {
                    v[i + 1] = v[i];
                    i = i - 1;
                }
                v[i + 1] = c;
            }

            //cria arquivo de particao e faz gravacao
            char *nome_particao = nome_arquivos_saida->nome;
            nome_arquivos_saida = nome_arquivos_saida->prox;

            FILE *p;
            if ((p = fopen(nome_particao, "wb")) == NULL)
            {
                printf("Erro criar arquivo de saida\n");
            }
            else
            {
                for (int i = 0; i < M; i++)
                {
                    salva_cliente(v[i], p);
                }
                fclose(p);
            }
            if (feof(arq))
            {
                fim = 1;
            }
        }
    }
}

int procuraMenor(Cliente **v, int M)
{
    int pos_menor = 0, cont = 0;
    Cliente *menor = (Cliente *)malloc(sizeof(Cliente));
    menor = v[0];
    for (int i = 0; i < M; i++)
    {
        if ((menor->cod_cliente >= v[i]->cod_cliente) && (v[i]->cod_cliente != INT_MAX))
        {
            pos_menor = i;
            menor = v[i];
        }
        if (v[i]->cod_cliente == INT_MAX)
        {
            cont += 1;
        }
        if (cont == M)
        {
            return INT_MAX;
        }
    }
    return pos_menor;
}

void congelado(Cliente **v, Cliente **freezer, Cliente *cin, int menor, FILE *p)
{
    salva_cliente(v[menor], p);
    freezer[menor] = cin;
    v[menor]->cod_cliente = INT_MAX;
}

void libera_congelado(Cliente **v, Cliente **freezer, int i, int M)
{
    int menor = procuraMenor(freezer, M);
    for (int ind = 0; ind < M; ind++)
    {
        if (menor != INT_MAX)
        {
            *v[ind] = *freezer[menor];
            freezer[menor]->cod_cliente = INT_MAX;
            menor = procuraMenor(freezer, M);
        }
        else
            break;
    }
}

void existe_menor(Cliente **v, int i, FILE *p)
{
    int menor = procuraMenor(v, i);
    while (menor != INT_MAX)
    {
        salva_cliente(v[menor], p);
        v[menor] = cliente(INT_MAX, "");
        menor = procuraMenor(v, i);
    }
}

void cria_particao(char *nome_particao, Nomes *nome_arquivos_saida, Cliente **v, FILE *p, int requisita_funcao, int menor, int i)
{
    if ((p = fopen(nome_particao, "wb")) == NULL)
    {
        printf("Erro criar arquivo de saida\n");
    }
    else{
        if(requisita_funcao == 1)
            existe_menor(v, i, p);

        fclose(p);
    }
}

void selecao_com_substituicao(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M)
{
    Cliente *cin;
    Cliente *freezer[M], *v[M];
    int menor = 0;
    int fim = 0, i = 0; //variável de controle para saber se arquivo de entrada terminou
    FILE *arq, *p;   //declara ponteiro para arquivo

    char *nome_particao = nome_arquivos_saida->nome;
    nome_arquivos_saida = nome_arquivos_saida->prox;
    
    //abre arquivo para leitura
    if ((arq = fopen(nome_arquivo_entrada, "rb")) == NULL)
    {
        printf("Erro ao abrir arquivo de entrada\n");
    }
    else
    {
        //le primeiro cliente
        cin = le_cliente(arq);
        //se tentou ler o arquivo e não tinha nenhum dado dentro dele, cria um arquivo vazio
        if (!cin)
        {
            cria_particao(nome_particao, nome_arquivos_saida, v, p, 0, 0, 0);
        }
        //se tinha algum elemento
        else
        {
            while (!(fim))
            {
                i = 0;
                //tenta ler o maximo de dados possiveis para a memória
                while (!feof(arq) && i < M)
                {
                    v[i] = cin;
                    cin = le_cliente(arq);
                    i++;
                }

                if (i < M) // particao de sobra
                {
                    cria_particao(nome_particao, nome_arquivos_saida, v, p, 1, 0, i);
                    return;
                }
                else if (i == M)
                {
                    if (!cin)
                    {
                        cria_particao(nome_particao, nome_arquivos_saida, v, p, 1, 0, i);
                        return;
                    }
                   
                    // Parte do congelamento
                    if (!(p = fopen(nome_particao, "wb")))
                        printf("Erro criar arquivo de saida\n");
                    else {
                        i = 0;
                        menor = procuraMenor(v, M);
                        while (menor != INT_MAX)
                        {
                            if (!cin) {
                                existe_menor(v, M, p);
                                fclose(p);
                                return;
                            }
    
                            if (v[menor]->cod_cliente < cin->cod_cliente) {
                                salva_cliente(v[menor], p);
                                v[menor] = cin;
                            }
                            else {
                                i++;
                                congelado(v, freezer, cin, menor, p);
                            }
                            menor = procuraMenor(v, M);
                            cin = le_cliente(arq);
                            if (menor == INT_MAX) {
                                fclose(p);
                                
                                char *nome_particao = nome_arquivos_saida->nome;
                                nome_arquivos_saida = nome_arquivos_saida->prox;

                                if (!(p = fopen(nome_particao, "wb")))
                                    printf("Erro criar arquivo de saida\n");
                                else
                                {
                                    if (i != 0)
                                        libera_congelado(v, freezer, i, M);
                                    else {
                                        fclose(p);
                                        return;
                                    }
                                }
                            }
                            menor = procuraMenor(v, M);
                        }
                        fclose(p);
                    }
                }
                if (feof(arq)) {
                    fim = 1;
                }
            }

        }
        fclose(arq);
    }
    //TODO: Inserir aqui o codigo do algoritmo de geracao de particoes
}

void selecao_natural(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M, int n)
{
    //TODO: Inserir aqui o codigo do algoritmo de geracao de particoes
}
