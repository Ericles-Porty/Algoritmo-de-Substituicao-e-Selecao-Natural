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
#include <stdlib.h>

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

// int chamaProximo (int i, Clinete **v){

//     {
//         /* code */
//     }

// }

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

void selecao_com_substituicao(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M)
{
    Cliente *temp;
    Cliente *freezer[M];
    int trava = 1;
    int menor = 0, cont = 0;
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
        if (!cin)
        {
            char *nome_particao = nome_arquivos_saida->nome;
            nome_arquivos_saida = nome_arquivos_saida->prox;

            FILE *p;
            if ((p = fopen(nome_particao, "wb")) == NULL)
            {
                printf("Erro criar arquivo de saida\n");
            }
            else
                fclose(p);
        }
        else
        {
            while (!(fim))
            {
                Cliente *v[M];
                int i = 0;
                while (!feof(arq) && i < M)
                {
                    v[i] = cin;
                    cin = le_cliente(arq);
                    i++;
                }

                //menor = procuraMenor(v, i);
                // salva_cliente(v,arq);
                // maior = chamaProximo(menor,v);
                // if (maior>menor)
                //     v[i]=maior;
                // else{
                //     congela[i]=maior;
                //     v[i]=NULL;
                //     if (!v[M-1])
                // }

                // }

                if (i < M) // particao de sobra
                {
                    menor = procuraMenor(v, i);
                    if (menor != INT_MAX)
                    {
                        char *nome_particao = nome_arquivos_saida->nome;
                        nome_arquivos_saida = nome_arquivos_saida->prox;

                        FILE *p;
                        if ((p = fopen(nome_particao, "wb")) == NULL)
                        {
                            printf("Erro criar arquivo de saida\n");
                        }
                        else
                        {
                            while ((menor != INT_MAX))
                            {
                                salva_cliente(v[menor], p);
                                v[menor]->cod_cliente = INT_MAX;
                                menor = procuraMenor(v, i);
                            }
                            fclose(p);
                            return;
                        }
                    }
                }
                else if (i == M)
                {
                    if (!(cin = le_cliente(arq)))
                    {
                        menor = procuraMenor(v, i);
                        if (menor != INT_MAX)
                        {
                            char *nome_particao = nome_arquivos_saida->nome;
                            nome_arquivos_saida = nome_arquivos_saida->prox;

                            FILE *p;
                            if ((p = fopen(nome_particao, "wb")) == NULL)
                            {
                                printf("Erro criar arquivo de saida\n");
                            }
                            else
                            {
                                while ((menor != INT_MAX))
                                {
                                    salva_cliente(v[menor], p);
                                    v[menor]->cod_cliente = INT_MAX;
                                    menor = procuraMenor(v, i);
                                }
                                fclose(p);
                                return;
                            }
                        }
                    }
                    else
                    {
                        // Parte do congelamento
                        // FILE *p;
                        // if ((p = fopen(nome_particao, "wb")) == NULL)
                        // {
                        //     printf("Erro criar arquivo de saida\n");
                        // }
                        // else
                        // {
                        //     if (trava)
                        //     {
                        //         salva_cliente(v[menor], p);
                        //         i++;
                        //         trava = 0;
                        //         temp = le_cliente(arq);
                        //         printf("%s\n", &temp->nome);
                        //         if (!temp)
                        //             printf("Macaco");
                        //         if (!v[menor])
                        //             printf("Macaco2");
                        //         if (temp->cod_cliente > v[menor]->cod_cliente)
                        //         {
                        //             printf("edededed");
                        //             salva_cliente(v[menor], p);
                        //             printf("edededed");
                        //             v[menor] = temp;
                        //         }
                        //         else
                        //         {
                        //             printf("edededed");
                        //             freezer[menor] = v[menor];
                        //             v[menor]->cod_cliente = INT_MAX;
                        //         }
                        // if (i != M)
                        // {
                        //     M = i;
                        // }
                        //     }

                        //     fclose(p);
                        // }
                    }
                }

                if (feof(arq))
                {
                    fim = 1;
                }
            }
        }
    }
    fclose(arq);
    //TODO: Inserir aqui o codigo do algoritmo de geracao de particoes
}

void selecao_natural(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M, int n)
{
    //TODO: Inserir aqui o codigo do algoritmo de geracao de particoes
}
