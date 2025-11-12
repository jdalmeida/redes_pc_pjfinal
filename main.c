/**
 * @author João Gabriel de Almeida
 */

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
void gerar_mermaid(Grafo* g, FILE* arquivo);
const char* tipo_dispositivo_str(TipoDispositivo tipo);
const char* tipo_conexao_str(TipoConexao tipo);

int main() {
    // Cria o grafo com capacidade inicial
    Grafo* rede = criar_grafo(20);
    if (!rede) {
        printf("Erro ao criar grafo!\n");
        return 1;
    }
    
    // Adiciona dispositivos conforme o exemplo do README
    int servidor1 = adicionar_vertice(rede, SERVIDOR, "Servidor 1");
    int switch1 = adicionar_vertice(rede, SWITCH, "Switch 1");
    int computador1 = adicionar_vertice(rede, COMPUTADOR, "Computador 1");
    int access_point1 = adicionar_vertice(rede, ACCESS_POINT, "Access Point 1");
    int computador2 = adicionar_vertice(rede, COMPUTADOR, "Computador 2");
    int servidor2 = adicionar_vertice(rede, SERVIDOR, "Servidor 2");
    int switch2 = adicionar_vertice(rede, SWITCH, "Switch 2");
    int computador3 = adicionar_vertice(rede, COMPUTADOR, "Computador 3");
    int access_point2 = adicionar_vertice(rede, ACCESS_POINT, "Access Point 2");
    int computador4 = adicionar_vertice(rede, COMPUTADOR, "Computador 4");
    int servidor3 = adicionar_vertice(rede, SERVIDOR, "Servidor 3");
    int computador5 = adicionar_vertice(rede, COMPUTADOR, "Computador 5");
    int computador6 = adicionar_vertice(rede, COMPUTADOR, "Computador 6");
    int computador7 = adicionar_vertice(rede, COMPUTADOR, "Computador 7");
    
    // Adiciona conexões conforme o exemplo
    adicionar_aresta(rede, servidor1, switch1, FIBRA);
    adicionar_aresta(rede, switch1, computador1, WIFI);
    adicionar_aresta(rede, switch1, access_point1, CABO);
    adicionar_aresta(rede, access_point1, computador2, WIFI);
    adicionar_aresta(rede, switch1, servidor2, SATELITE);
    adicionar_aresta(rede, servidor2, switch2, FIBRA);
    adicionar_aresta(rede, switch2, computador3, CABO);
    adicionar_aresta(rede, switch2, access_point2, WIFI);
    adicionar_aresta(rede, access_point2, computador4, WIFI);
    adicionar_aresta(rede, switch2, servidor3, SATELITE);
    adicionar_aresta(rede, switch1, computador5, CABO);
    adicionar_aresta(rede, computador5, computador6, WIFI);
    adicionar_aresta(rede, access_point1, computador7, CABO);
    
    // Gera o arquivo Mermaid
    FILE* arquivo = fopen("rede.mmd", "w");
    if (arquivo) {
        gerar_mermaid(rede, arquivo);
        fclose(arquivo);
        printf("Grafo gerado com sucesso em 'rede.mmd'!\n");
    } else {
        printf("Erro ao criar arquivo de saída!\n");
    }
    
    // Exibe informações da rede
    printf("\n=== Informações da Rede ===\n");
    printf("Total de dispositivos: %d\n\n", rede->num_vertices);
    
    for (int i = 0; i < rede->num_vertices; i++) {
        printf("%s %d (%s):\n", 
               tipo_dispositivo_str(rede->vertices[i].tipo),
               i + 1,
               rede->vertices[i].nome);
        
        Aresta* atual = rede->vertices[i].lista_adjacencia;
        int num_conexoes = 0;
        while (atual) {
            num_conexoes++;
            printf("  -> Conectado a %s %d via %s\n",
                   tipo_dispositivo_str(rede->vertices[atual->destino].tipo),
                   atual->destino + 1,
                   tipo_conexao_str(atual->tipo));
            atual = atual->proxima;
        }
        printf("  Total de conexões: %d\n\n", num_conexoes);
    }
    
    // Libera memória
    destruir_grafo(rede);
    
    return 0;
}
