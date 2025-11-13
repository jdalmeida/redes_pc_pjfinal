## Funções

- Grafo

``` C
Grafo* criar_grafo(int capacidade);

void destruir_grafo(Grafo* g);

int adicionar_vertice(Grafo* g, TipoDispositivo tipo, const char* nome);

int adicionar_aresta(Grafo* g, int origem, int destino, TipoConexao tipo);

int remover_aresta(Grafo* g, int origem, int destino);

int remover_vertice(Grafo* g, int id);

int validar_conexao(TipoDispositivo origem, TipoDispositivo destino);

void gerar_mermaid(Grafo* g, FILE* arquivo);

const char* tipo_dispositivo_str(TipoDispositivo tipo);

const char* tipo_conexao_str(TipoConexao tipo);

int obter_peso_conexao(TipoConexao tipo);

int dfs_rota_mais_rapida(Grafo* g, int atual, int destino, int* visitado, int* caminho_atual, int* melhor_caminho, int profundidade, 

int peso_atual, int* melhor_peso, int max_profundidade);

int encontrar_rota_mais_rapida(Grafo* g, int origem, int destino, int* caminho, int* tamanho_caminho);
```

- Main

``` C
void seed_rede(Grafo* g);

void exibir_dispositivos(Grafo* g);

void exibir_menu();
```
