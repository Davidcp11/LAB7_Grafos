/*  David Costa Pereira                               */
/*  Turma 3                                           */
/*  Exercicio 7 : PERT                                */
/*                                                    */
/*  Programa compilado com CodeBlocks 20.03           */
#include <iostream>
typedef struct dados dados;
typedef struct pilha pilha;
///Arquivos de entrada e saida de dados
FILE *entrada, *saida;
char leitura[65];
///Numero de vertices, ordem topologica(se nao existir ciclo) e Tempo Minimo de Termino(uma posicao para o TMT
///e outra para o seu antecessor
int n, ord_topol[52], TMT[52][2];
///Matriz de adjacencias e booleana para verificar se o grafo eh aciclico
bool matriz[52][52], tem_ciclo;
struct pilha{
    int vet[52];
    int quantidade;
};
struct dados{
    int time;
    char tarefa;
    char descricao[31];
    bool descoberto, visitado;
    int antecessor;
};
///Vetor de structs (O grafo)
dados vet[52];
///*******************************************************************************
///
///                        FUNCOES PARA A PILHA
///
///*******************************************************************************
void inicializar_pilha(pilha *p){
    p->quantidade = 0;
}
///*******************************************************************************
///Inseir na pilha
void push(pilha *p, int i){
    p->vet[p->quantidade] = i;
    p->quantidade++;
}
///*******************************************************************************
///retorna o topo da pilha e o remove
int top_pop(pilha *p){
    p->quantidade--;
    return p->vet[p->quantidade];
}
///*******************************************************************************
///Verifica se a pilha esta vazia(true) ou nao(false)
bool  pilha_vazia(pilha *p){
    if(p->quantidade == 0)
        return true;
    return false;
}
///*******************************************************************************
///
///                          FUNCOES PARA O GRAFO
///
///*******************************************************************************
///Recebe o char de uma tarefa e retorna a posicao dela no vetor. Se nao existir
int procurar(char tarefa){
    for(int i=0;i<n;i++){
        if(vet[i].tarefa==tarefa)
            return i;
    }
    return -1;
}
///*******************************************************************************
///Procura um vertice que nao eh apontado por nenhum outro
int sem_antecessor(){
    bool achou;
    for(int i=0;i<n;i++) {
        achou = false;
        for (int k = 0; k < n and !achou; k++) {
            if (matriz[k][i])
                achou = true;
        }
        if(!achou){
            if(!vet[i].descoberto){
                vet[i].descoberto = true;
                return i;
            }
        }
    }
    return -1;
}
///*******************************************************************************
///Ler o arquivo pela primeira vez e guarda todas as tarefas
void primeira_leitura(){
    entrada = fopen("entrada7.txt","r");
    char tarefa;
    n = 0;
    for(int i=0; i<7; i++){
        fgets(leitura, 65, entrada);
    }
    do {
        fscanf(entrada," %c ", &tarefa);
        if(tarefa!='-'){
            vet[n].tarefa = tarefa;
            fgets(vet[n].descricao,30, entrada);
            fscanf(entrada," %d ",&vet[n].time);
            fgets(leitura, 65, entrada);
            n++;
        }
    } while (tarefa!='-');
    fclose(entrada);
}
///*******************************************************************************
///Ler o arquivo e guarda quais sao os precedentes de cada tarefa
void segunda_leitura(){
    entrada = fopen("entrada7.txt","r");
    int i, k, numb, j;
    char tarefa_precedente, tarefa;
    for(j=0; j<7; j++){
        fgets(leitura, 65, entrada);
    }
    for(numb=0;numb<n;numb++){
        fscanf(entrada," %c ", &tarefa);
        fgets(leitura,30, entrada);
        fscanf(entrada," %d ", &j);
        fscanf(entrada," %c", &tarefa_precedente);
        while(tarefa_precedente!='.' and tarefa_precedente!='\n') {
            if (tarefa_precedente != ',') {
                i = procurar(tarefa_precedente);
                k = procurar(tarefa);
                if (k >= 0 and i >= 0) {
                    matriz[i][k] = true;
                }
            }
            fscanf(entrada,"%c", &tarefa_precedente);
       }
    }
    fclose(entrada);
}
///*******************************************************************************
void resetar(){
    for(int i=0; i<n; i++){
        vet[i].visitado = false;
        vet[i].descoberto = false;
    }
}
///*******************************************************************************
///Verificar se o grafo eh ciclico. Se sim, guardar o ciclo na pilha.
void DFS_ciclo( int i, pilha *p){
    int j;
    vet[i].visitado = true;
    for(int k=0; k<n; k++) {
        if (matriz[i][k]) {
            vet[k].antecessor = i;
            if (vet[k].visitado) {
                tem_ciclo = true;
                j = i;
                push(p, k);
                while (j != k) {
                    push(p, j);
                    j = vet[j].antecessor;
                }
                push(p, k);
            } else vet[k].visitado = true;
            if (!tem_ciclo)
                DFS_ciclo(k, p);
        }
    }
    vet[i].visitado = false;
}
///*******************************************************************************
///Busca em profundidade e armazenamento da ordem topologica
void DFS(int vertex, pilha *p){
    for(int i=0;i<n;i++){
        if(matriz[vertex][i] and !vet[i].visitado){
            vet[i].visitado = true;
            DFS(i, p);
        }
    }
    push(p, vertex);
}
///*******************************************************************************
///Procura a posicao do noh na ordem topologica
int topological_ord(int i){
    int k=0;
    while (i!=ord_topol[k]){
        k++;
    }
    return k;
}
///*******************************************************************************
///Retorna uma posicao sem TMT na ordem topologica
int sem_TMT(){
    for(int i =0; i<n; i++){
        if(TMT[i][0]==0)
            return i;
    }
    return -1;
}
///*******************************************************************************
///Retorna o valor do maior TMT anterior a um dado vertice
int MAX_TMT_anterior(int i){
    int aux = 0, x_topol, y_topol;
    ///Pegando o numero do vertice que esta na posicao i da ordem topologica
    x_topol = ord_topol[i];
    for(int k = 0; k<i; k++){
        ///Pegando o numero do vertice que esta na posicao k da ordem topologica
        y_topol = ord_topol[k];
        if(matriz[y_topol][x_topol]){
            if (TMT[k][0]>=aux) {
                aux = TMT[k][0];
                TMT[i][1] = k;
            }
        }
    }
    ///Max ate a posicao i
    return aux;
}
///*******************************************************************************
///Caminho critico
int CaminhoCritico(pilha *p){
    ///Grafo original, sem nenhuma visita ou descoberta
    resetar();
    int i, k, j;
    ///O TMT de um vertice sem antecessores eh seu proprio custo
    i = sem_antecessor();
    while (i != -1) {
        k = topological_ord(i);
        TMT[k][0] = vet[i].time;
        TMT[k][1] = -1;
        i = sem_antecessor();
    }
    i = sem_TMT();
    k = MAX_TMT_anterior(i);
    ///Pegando o numero do vertice que esta na posicao i da ordem topologica
    j = ord_topol[i];
    ///Enquanto existir vertice sem um TMT
    while (i != -1) {
        TMT[i][0] = vet[j].time + k;
        i = sem_TMT();
        k = MAX_TMT_anterior(i);
        j = ord_topol[i];
    }
    /// Guardar caminho critico na pilha
    int max = 0,posi, anterior;
    for (int x = 0; x < n; x++) {
        if (TMT[x][0]> max) {
            max = TMT[x][0];
            posi = x;
            anterior = TMT[x][1];
        }
    }
    do {
        push(p,ord_topol[posi]);
        posi = anterior;
        if(anterior!=-1) {
            anterior = TMT[anterior][1];
        }
    } while (posi != -1);
    ///push(p,posi);
    ///Retorna a soma de todos os custos do caminho critico (ou o maior dos TMT)
    return max;
}
int main() { 
    saida = fopen("Lab7_David_Costa_Pereira.txt","w");
    int inicial, max;
    tem_ciclo = false;
    ///Pilha para armazenar os caminhos ou ciclos
    pilha pilha;
    inicializar_pilha(&pilha);
    ///Lendo o arquivo e armazenando os vertices e seus antecessores
    primeira_leitura();
    segunda_leitura();
    fprintf(saida,"Bom dia\n");
    fprintf(saida,"Boa tarde\n");
    fprintf(saida,"Boa noite\n");
    fprintf(saida,"Vamos fingir que a obra nao vai atrasar novamente?\n");
    fprintf(saida,"\n");
    fprintf(saida,"------------------------------------------------------------\n\n");
    ///Verificando se existe ciclo
    for(int i=0; i<n and !tem_ciclo; i++) {
        vet[i].descoberto = true;
        DFS_ciclo(i,&pilha);
    }
    ///Se nao existir, procurar o caminho critico
    if(!tem_ciclo) {
        resetar();
        do {
            inicial = sem_antecessor();
            if (inicial != -1) {
                DFS(inicial, &pilha);
            }
        } while (inicial != -1);
        fprintf(saida,"UMA ORDENACAO TOPOLOGICA:\n\n");
        ///Ordem topologica
        int i = 0;
        while (!pilha_vazia(&pilha)) {
            ord_topol[i] = top_pop(&pilha);
            i++;
        }
        for (i = 0; i < n; i++) {
            fprintf(saida, "%c ", vet[ord_topol[i]].tarefa);
        }
        fprintf(saida,"\n\n------------------------------------------------------------\n\n");
        max = CaminhoCritico(&pilha);
        fprintf(saida,"CAMINHO CRITICO:\n\n");
        fprintf(saida,"TAREFA        DESCRICAO           DURACAO\n\n");
        while (!pilha_vazia(&pilha)){
            i = top_pop(&pilha);
            fprintf(saida,"  %c   %s %3d\n", vet[i].tarefa, vet[i].descricao, vet[i].time);
        }
        fprintf(saida,"                                   ----\n");
        fprintf(saida,"TEMPO MINIMO PARA O PROJETO:        %3d semanas", max);

    } else{
        int k;
        fprintf(saida,"EXISTE UM CICLO:\n\n");
        while (!pilha_vazia(&pilha)){
            k = top_pop(&pilha);
            fprintf(saida,"%c ", vet[k].tarefa);
        }
        fprintf(saida,"\n\nIMPOSSIVEL BUSCAR CAMINHO CRITICO.\n\n");
        fprintf(saida,"------------------------------------------------------------");
    }
    ///fechando arquivo de saida
    fclose(saida);
    return 0;
}
