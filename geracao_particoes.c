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
            FILE *p = fopen(nome_particao, "wb");
            fclose(p);
        }else{
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
                    cin = le_cliente(arq); // em cin fica o setimo cliente
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
                    while(nome_arquivos_saida){
                            reservatorio = fopen("Reservatorio.dat", "wb");
                            p = fopen(nome_particao, "wb");
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
                            /* caso o proximoseja menor que o atual menor na memoria, o prox
                            ira para o reservatorio */
                            if(memoria[indiceMenor]->cod_cliente > prox->cod_cliente){
                                salva_cliente(prox,reservatorio);
                                /* enquanto o n for maior que 0, significa que o reservatorio
                                ainda nao esta cheio */
                                n--;
                            }
                            /* caso contrario, o menor sera salvo na memoria, e o prox ficara
                            em seu lugar na mesma posicao de memoria(indeiceMenor) */
                            else{
                                salva_cliente(memoria[indiceMenor],p);
                                memoria[indiceMenor] = prox;
                            }

                            prox = le_cliente(arq);
                        }
                        fclose(reservatorio);
                        /* encontra o menores valores na memoria e salva na particao */
                        menor = memoria[0];
                        indiceMenor = 0;
                        int repete = M;
                        while(repete){
                            for(int i = 0; i<M; i++){
                                if(memoria[i]->cod_cliente < menor->cod_cliente){
                                        menor = memoria[i];
                                        indiceMenor = i;
                                }
                            }

                                salva_cliente(memoria[indiceMenor],p);
                                memoria[indiceMenor]->cod_cliente = INT_MAX;
                                repete--;
                            }

                        fclose(p);

                        FILE* reserva = fopen("Reservatorio.dat", "rb");
                        int i = 0;
                        /* ler todo o conteudo do reservaotiro */
                        while (!feof(reserva) && i < M) {
                            memoria[i] = le_cliente(reserva); // ler os 6 clientes do reservatorio
                            i++;
                            n++;
                        }

                        fclose(reserva);
                        /* troca o nome para a proxima particao ex p1.dat para p2.dat */
                        nome_particao = nome_arquivos_saida->nome;
                        nome_arquivos_saida = nome_arquivos_saida->prox;

                    }
                }

                FILE *p = fopen(nome_particao, "wb");

                /* encontra o menor valor na memoria */
                menor = memoria[0];
                indiceMenor = 0;
                int repete = M;
                while(repete){
                for(int i = 0; i<M; i++){
                    if(memoria[i]->cod_cliente < menor->cod_cliente){
                        menor = memoria[i];
                        indiceMenor = i;
                    }
                }
                salva_cliente(memoria[indiceMenor],p);
                memoria[indiceMenor]->cod_cliente = INT_MAX;
                repete--;
            }
            /* se o proximo for diferente de null significa que e sobra, pois
            o reservatorio ficou cheio antes dele ser gravado de arquivo e sera salvo */
            if(prox != NULL){
                salva_cliente(prox,p);
            }
            fclose(p);


        }
        /* exclui o reservatorio */
        remove("Reservatorio.dat");
    }
    fclose(arq);
}