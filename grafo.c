#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Tipos de dispositivo
typedef enum {
    SERVIDOR,
    SWITCH,
    COMPUTADOR,
    ACCESS_POINT
} TipoDispositivo;

// Tipos de conexão
typedef enum {
    SATELITE,
    WIFI,
    CABO,
    FIBRA
} TipoConexao;

// Estrutura de uma aresta (conexão)
typedef struct Aresta {
    int destino;
    TipoConexao tipo;
    struct Aresta* proxima;
} Aresta;

// Estrutura de um vértice (dispositivo)
typedef struct Vertice {
    int id;
    TipoDispositivo tipo;
    char nome[50];
    Aresta* lista_adjacencia;
} Vertice;

// Estrutura do grafo
typedef struct {
    Vertice* vertices;
    int num_vertices;
    int capacidade;
} Grafo;

// Declarações das funções
Grafo* criar_grafo(int capacidade);
void destruir_grafo(Grafo* g);
int adicionar_vertice(Grafo* g, TipoDispositivo tipo, const char* nome);
int adicionar_aresta(Grafo* g, int origem, int destino, TipoConexao tipo);
int validar_conexao(TipoDispositivo origem, TipoDispositivo destino);
void gerar_mermaid(Grafo* g, FILE* arquivo);
const char* tipo_dispositivo_str(TipoDispositivo tipo);
const char* tipo_conexao_str(TipoConexao tipo);

// Cria um novo grafo
Grafo* criar_grafo(int capacidade) {
    Grafo* g = (Grafo*)malloc(sizeof(Grafo));
    if (!g) return NULL;
    
    g->vertices = (Vertice*)malloc(capacidade * sizeof(Vertice));
    if (!g->vertices) {
        free(g);
        return NULL;
    }
    
    g->num_vertices = 0;
    g->capacidade = capacidade;
    
    for (int i = 0; i < capacidade; i++) {
        g->vertices[i].id = i;
        g->vertices[i].lista_adjacencia = NULL;
    }
    
    return g;
}

// Destrói o grafo e libera memória
void destruir_grafo(Grafo* g) {
    if (!g) return;
    
    for (int i = 0; i < g->num_vertices; i++) {
        Aresta* atual = g->vertices[i].lista_adjacencia;
        while (atual) {
            Aresta* prox = atual->proxima;
            free(atual);
            atual = prox;
        }
    }
    
    free(g->vertices);
    free(g);
}

// Adiciona um vértice ao grafo
int adicionar_vertice(Grafo* g, TipoDispositivo tipo, const char* nome) {
    if (!g || g->num_vertices >= g->capacidade) {
        return -1;
    }
    
    int id = g->num_vertices;
    g->vertices[id].id = id;
    g->vertices[id].tipo = tipo;
    strncpy(g->vertices[id].nome, nome, sizeof(g->vertices[id].nome) - 1);
    g->vertices[id].nome[sizeof(g->vertices[id].nome) - 1] = '\0';
    g->vertices[id].lista_adjacencia = NULL;
    
    g->num_vertices++;
    return id;
}

// Valida se uma conexão é permitida
int validar_conexao(TipoDispositivo origem, TipoDispositivo destino) {
    // Servidor só conecta com Switch
    if (origem == SERVIDOR) {
        return destino == SWITCH;
    }
    
    // Computador conecta com Switch e Access Point
    if (origem == COMPUTADOR) {
        return destino == SWITCH || destino == ACCESS_POINT || destino == COMPUTADOR;
    }
    
    // Access Point só conecta com Switch
    if (origem == ACCESS_POINT) {
        return destino == SWITCH || destino == COMPUTADOR;
    }
    
    // Switch pode conectar com qualquer dispositivo
    if (origem == SWITCH) {
        return 1;
    }
    
    return 0;
}

// Adiciona uma aresta ao grafo (grafo não orientado)
int adicionar_aresta(Grafo* g, int origem, int destino, TipoConexao tipo) {
    if (!g || origem < 0 || destino < 0 || 
        origem >= g->num_vertices || destino >= g->num_vertices ||
        origem == destino) {
        return 0;
    }
    
    // Valida restrições de conexão
    if (!validar_conexao(g->vertices[origem].tipo, g->vertices[destino].tipo) &&
        !validar_conexao(g->vertices[destino].tipo, g->vertices[origem].tipo)) {
        return 0;
    }
    
    // Verifica se a aresta já existe
    Aresta* atual = g->vertices[origem].lista_adjacencia;
    while (atual) {
        if (atual->destino == destino) {
            return 0; // Aresta já existe
        }
        atual = atual->proxima;
    }
    
    // Adiciona aresta origem -> destino
    Aresta* nova_aresta = (Aresta*)malloc(sizeof(Aresta));
    if (!nova_aresta) return 0;
    
    nova_aresta->destino = destino;
    nova_aresta->tipo = tipo;
    nova_aresta->proxima = g->vertices[origem].lista_adjacencia;
    g->vertices[origem].lista_adjacencia = nova_aresta;
    
    // Adiciona aresta destino -> origem (grafo não orientado)
    Aresta* nova_aresta_reversa = (Aresta*)malloc(sizeof(Aresta));
    if (!nova_aresta_reversa) {
        free(nova_aresta);
        return 0;
    }
    
    nova_aresta_reversa->destino = origem;
    nova_aresta_reversa->tipo = tipo;
    nova_aresta_reversa->proxima = g->vertices[destino].lista_adjacencia;
    g->vertices[destino].lista_adjacencia = nova_aresta_reversa;
    
    return 1;
}

// Retorna string do tipo de dispositivo
const char* tipo_dispositivo_str(TipoDispositivo tipo) {
    switch (tipo) {
        case SERVIDOR: return "Servidor";
        case SWITCH: return "Switch";
        case COMPUTADOR: return "Computador";
        case ACCESS_POINT: return "Access Point";
        default: return "Desconhecido";
    }
}

// Retorna string do tipo de conexão
const char* tipo_conexao_str(TipoConexao tipo) {
    switch (tipo) {
        case SATELITE: return "Satélite";
        case WIFI: return "WiFi";
        case CABO: return "Cabo";
        case FIBRA: return "Fibra";
        default: return "Desconhecido";
    }
}

// Gera saída em formato Mermaid
void gerar_mermaid(Grafo* g, FILE* arquivo) {
    if (!g || !arquivo) return;
    
    fprintf(arquivo, "graph TD\n");
    
    // Gera os nós
    for (int i = 0; i < g->num_vertices; i++) {
        fprintf(arquivo, "    %d[\"%s\"]\n", 
                i, g->vertices[i].nome);
    }
    
    // Gera as arestas (apenas uma vez, já que é não orientado)
    int* visitado = (int*)calloc(g->num_vertices * g->num_vertices, sizeof(int));
    if (!visitado) return;
    
    for (int i = 0; i < g->num_vertices; i++) {
        Aresta* atual = g->vertices[i].lista_adjacencia;
        while (atual) {
            int j = atual->destino;
            // Evita duplicar arestas (i-j e j-i são a mesma aresta)
            if (i < j) {
                int idx = i * g->num_vertices + j;
                if (!visitado[idx]) {
                    fprintf(arquivo, "    %d -- %s --> %d\n",
                            i, tipo_conexao_str(atual->tipo), j);
                    visitado[idx] = 1;
                }
            }
            atual = atual->proxima;
        }
    }
    
    free(visitado);
}

