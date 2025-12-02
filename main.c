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
int remover_aresta(Grafo* g, int origem, int destino);
int remover_vertice(Grafo* g, int id);
void gerar_mermaid(Grafo* g, FILE* arquivo);
const char* tipo_dispositivo_str(TipoDispositivo tipo);
const char* tipo_conexao_str(TipoConexao tipo);
int encontrar_rota_mais_rapida(Grafo* g, int origem, int destino, int* caminho, int* tamanho_caminho);
int obter_peso_conexao(TipoConexao tipo);
void seed_rede(Grafo* g);
void exibir_dispositivos(Grafo* g);
void exibir_menu();

// Função para popular a rede com dispositivos e conexões de exemplo
void seed_rede(Grafo* g) {
    if (!g) return;

    // Adiciona dispositivos conforme o exemplo do README
    int servidor1 = adicionar_vertice(g, SERVIDOR, "Servidor 1");
    int switch1 = adicionar_vertice(g, SWITCH, "Switch 1");
    int computador1 = adicionar_vertice(g, COMPUTADOR, "Computador 1");
    int access_point1 = adicionar_vertice(g, ACCESS_POINT, "Access Point 1");
    int computador2 = adicionar_vertice(g, COMPUTADOR, "Computador 2");
    int servidor2 = adicionar_vertice(g, SERVIDOR, "Servidor 2");
    int switch2 = adicionar_vertice(g, SWITCH, "Switch 2");
    int computador3 = adicionar_vertice(g, COMPUTADOR, "Computador 3");
    int access_point2 = adicionar_vertice(g, ACCESS_POINT, "Access Point 2");
    int computador4 = adicionar_vertice(g, COMPUTADOR, "Computador 4");
    int servidor3 = adicionar_vertice(g, SERVIDOR, "Servidor 3");
    int computador5 = adicionar_vertice(g, COMPUTADOR, "Computador 5");
    int computador6 = adicionar_vertice(g, COMPUTADOR, "Computador 6");
    int computador7 = adicionar_vertice(g, COMPUTADOR, "Computador 7");

    // Adiciona conexões conforme o exemplo
    adicionar_aresta(g, servidor1, switch1, FIBRA);
    adicionar_aresta(g, switch1, computador1, WIFI);
    adicionar_aresta(g, switch1, access_point1, CABO);
    adicionar_aresta(g, access_point1, computador2, WIFI);
    adicionar_aresta(g, switch1, servidor2, SATELITE);
    adicionar_aresta(g, servidor2, switch2, FIBRA);
    adicionar_aresta(g, switch2, computador3, CABO);
    adicionar_aresta(g, switch2, access_point2, WIFI);
    adicionar_aresta(g, access_point2, computador4, WIFI);
    adicionar_aresta(g, switch2, servidor3, SATELITE);
    adicionar_aresta(g, switch1, computador5, CABO);
    adicionar_aresta(g, computador5, computador6, WIFI);
    adicionar_aresta(g, access_point1, computador7, CABO);

    printf("Rede populada com dispositivos e conexões de exemplo!\n");
}

// Exibe todos os dispositivos da rede
void exibir_dispositivos(Grafo* g) {
    if (!g || g->num_vertices == 0) {
        printf("Nenhum dispositivo cadastrado.\n\n");
        return;
    }

    printf("\n=== Dispositivos da Rede ===\n");
    for (int i = 0; i < g->num_vertices; i++) {
        printf("%d - %s (%s)\n",
               i + 1,
               g->vertices[i].nome,
               tipo_dispositivo_str(g->vertices[i].tipo));
    }
    printf("\n");
}

// Exibe o menu principal
void exibir_menu() {
    printf("\n=== MENU PRINCIPAL ===\n");
    printf("1 - Adicionar dispositivo\n");
    printf("2 - Remover dispositivo\n");
    printf("3 - Adicionar conexão\n");
    printf("4 - Remover conexão\n");
    printf("5 - Listar dispositivos\n");
    printf("6 - Exibir informações da rede\n");
    printf("7 - Gerar arquivo Mermaid\n");
    printf("8 - Popular rede (seed)\n");
    printf("9 - Calcular rota mais rápida\n");
    printf("0 - Sair\n");
    printf("Escolha uma opção: ");
}

int main() {
    // Cria o grafo com capacidade inicial
    Grafo* rede = criar_grafo(50);
    if (!rede) {
        printf("Erro ao criar grafo!\n");
        return 1;
    }

    int opcao;
    char nome[50];
    int tipo_disp, tipo_conn;
    int origem, destino;
    int id;

    printf("=== Sistema de Gerenciamento de Rede ===\n");

    do {
        exibir_menu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Adicionar dispositivo
                printf("\n--- Adicionar Dispositivo ---\n");
                printf("Tipo de dispositivo:\n");
                printf("0 - Servidor\n");
                printf("1 - Switch\n");
                printf("2 - Computador\n");
                printf("3 - Access Point\n");
                printf("Escolha: ");
                scanf("%d", &tipo_disp);

                if (tipo_disp < 0 || tipo_disp > 3) {
                    printf("Tipo inválido!\n");
                    break;
                }

                printf("Nome do dispositivo: ");
                scanf(" %[^\n]", nome);

                id = adicionar_vertice(rede, (TipoDispositivo)tipo_disp, nome);
                if (id >= 0) {
                    printf("Dispositivo '%s' adicionado com ID %d!\n", nome, id + 1);
                } else {
                    printf("Erro ao adicionar dispositivo! Capacidade máxima atingida.\n");
                }
                break;

            case 2: // Remover dispositivo
                printf("\n--- Remover Dispositivo ---\n");
                exibir_dispositivos(rede);

                if (rede->num_vertices == 0) {
                    break;
                }

                printf("ID do dispositivo a remover (1-%d): ", rede->num_vertices);
                scanf("%d", &id);
                id--; // Converter para índice baseado em 0

                if (id >= 0 && id < rede->num_vertices) {
                    char nome_removido[50];
                    strcpy(nome_removido, rede->vertices[id].nome);

                    if (remover_vertice(rede, id)) {
                        printf("Dispositivo '%s' removido com sucesso!\n", nome_removido);
                    } else {
                        printf("Erro ao remover dispositivo!\n");
                    }
                } else {
                    printf("ID inválido!\n");
                }
                break;

            case 3: // Adicionar conexão
                printf("\n--- Adicionar Conexão ---\n");
                exibir_dispositivos(rede);

                if (rede->num_vertices < 2) {
                    printf("É necessário pelo menos 2 dispositivos para criar uma conexão.\n");
                    break;
                }

                printf("ID do dispositivo origem (1-%d): ", rede->num_vertices);
                scanf("%d", &origem);
                origem--;

                printf("ID do dispositivo destino (1-%d): ", rede->num_vertices);
                scanf("%d", &destino);
                destino--;

                if (origem < 0 || origem >= rede->num_vertices ||
                    destino < 0 || destino >= rede->num_vertices ||
                    origem == destino) {
                    printf("IDs inválidos!\n");
                    break;
                }

                printf("Tipo de conexão:\n");
                printf("0 - Satélite\n");
                printf("1 - WiFi\n");
                printf("2 - Cabo\n");
                printf("3 - Fibra\n");
                printf("Escolha: ");
                scanf("%d", &tipo_conn);

                if (tipo_conn < 0 || tipo_conn > 3) {
                    printf("Tipo inválido!\n");
                    break;
                }

                if (adicionar_aresta(rede, origem, destino, (TipoConexao)tipo_conn)) {
                    printf("Conexão adicionada com sucesso!\n");
                } else {
                    printf("Erro ao adicionar conexão! Verifique se a conexão é válida ou já existe.\n");
                }
                break;

            case 4: // Remover conexão
                printf("\n--- Remover Conexão ---\n");
                exibir_dispositivos(rede);

                if (rede->num_vertices < 2) {
                    printf("Não há conexões para remover.\n");
                    break;
                }

                printf("ID do dispositivo origem (1-%d): ", rede->num_vertices);
                scanf("%d", &origem);
                origem--;

                printf("ID do dispositivo destino (1-%d): ", rede->num_vertices);
                scanf("%d", &destino);
                destino--;

                if (origem < 0 || origem >= rede->num_vertices ||
                    destino < 0 || destino >= rede->num_vertices ||
                    origem == destino) {
                    printf("IDs inválidos!\n");
                    break;
                }

                if (remover_aresta(rede, origem, destino)) {
                    printf("Conexão removida com sucesso!\n");
                } else {
                    printf("Conexão não encontrada!\n");
                }
                break;

            case 5: // Listar dispositivos
                exibir_dispositivos(rede);
                break;

            case 6: // Exibir informações da rede
                printf("\n=== Informações da Rede ===\n");
                printf("Total de dispositivos: %d\n\n", rede->num_vertices);

                if (rede->num_vertices == 0) {
                    printf("Nenhum dispositivo cadastrado.\n");
                    break;
                }

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
                break;

            case 7: // Gerar arquivo Mermaid
                {
                    FILE* arquivo = fopen("rede.mmd", "w");
                    if (arquivo) {
                        gerar_mermaid(rede, arquivo);
                        fclose(arquivo);
                        printf("Grafo gerado com sucesso em 'rede.mmd'!\n");
                    } else {
                        printf("Erro ao criar arquivo de saída!\n");
                    }
                }
                break;

            case 8: // Popular rede (seed)
                if (rede->num_vertices > 0) {
                    printf("Atenção: A rede já possui dispositivos. Deseja limpar e popular novamente? (1=Sim, 0=Não): ");
                    int confirmar;
                    scanf("%d", &confirmar);
                    if (confirmar == 1) {
                        destruir_grafo(rede);
                        rede = criar_grafo(50);
                        if (!rede) {
                            printf("Erro ao recriar grafo!\n");
                            return 1;
                        }
                        seed_rede(rede);
                    }
                } else {
                    seed_rede(rede);
                }
                break;

            case 9: // Calcular rota mais rápida
                {
                    printf("\n--- Calcular Rota Mais Rápida ---\n");
                    exibir_dispositivos(rede);

                    if (rede->num_vertices < 2) {
                        printf("É necessário pelo menos 2 dispositivos para calcular uma rota.\n");
                        break;
                    }

                    printf("ID do dispositivo origem (1-%d): ", rede->num_vertices);
                    scanf("%d", &origem);
                    origem--;

                    printf("ID do dispositivo destino (1-%d): ", rede->num_vertices);
                    scanf("%d", &destino);
                    destino--;

                    if (origem < 0 || origem >= rede->num_vertices ||
                        destino < 0 || destino >= rede->num_vertices ||
                        origem == destino) {
                        printf("IDs inválidos!\n");
                        break;
                    }

                    // Aloca array para o caminho
                    int* caminho = (int*)malloc(rede->num_vertices * sizeof(int));
                    int tamanho_caminho = 0;

                    if (!caminho) {
                        printf("Erro ao alocar memória!\n");
                        break;
                    }

                    if (encontrar_rota_mais_rapida(rede, origem, destino, caminho, &tamanho_caminho)) {
                        printf("\n=== Rota Mais Rápida Encontrada ===\n");
                        printf("De: %s (%s)\n",
                               rede->vertices[origem].nome,
                               tipo_dispositivo_str(rede->vertices[origem].tipo));
                        printf("Para: %s (%s)\n\n",
                               rede->vertices[destino].nome,
                               tipo_dispositivo_str(rede->vertices[destino].tipo));

                        int peso_total = 0;
                        printf("Caminho:\n");
                        for (int i = 0; i < tamanho_caminho; i++) {
                            printf("  %d. %s (%s)",
                                   i + 1,
                                   rede->vertices[caminho[i]].nome,
                                   tipo_dispositivo_str(rede->vertices[caminho[i]].tipo));

                            if (i < tamanho_caminho - 1) {
                                // Encontra o tipo de conexão entre caminho[i] e caminho[i+1]
                                Aresta* atual = rede->vertices[caminho[i]].lista_adjacencia;
                                TipoConexao tipo_conn = SATELITE;

                                while (atual) {
                                    if (atual->destino == caminho[i + 1]) {
                                        tipo_conn = atual->tipo;
                                        break;
                                    }
                                    atual = atual->proxima;
                                }

                                int peso = obter_peso_conexao(tipo_conn);
                                peso_total += peso;

                                printf(" --[%s (peso: %d)]--> ",
                                       tipo_conexao_str(tipo_conn),
                                       peso);
                            } else {
                                printf("\n");
                            }
                        }

                        printf("\nPeso total da rota: %d\n", peso_total);
                        printf("(Fibra=0, Cabo=1, WiFi=2, Satélite=3 - menor é melhor)\n");
                    } else {
                        printf("Não foi possível encontrar uma rota entre os dispositivos selecionados.\n");
                    }

                    free(caminho);
                }
                break;

            case 0: // Sair
                printf("Encerrando programa...\n");
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }

    } while (opcao != 0);

    // Libera memória
    destruir_grafo(rede);

    return 0;
}
