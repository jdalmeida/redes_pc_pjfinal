# Projeto final Estrutura de Dados

## Tema: **Rede de computadores**

- Grafo com topografia da rede
- Inserção de dispositivos com diferentes tipos de conexão
- Exibição do grafo com Mermaid
- Utilizar lista de Adjacência


### Tipo de grafo

- Grafo não orientado
- Valorado por tipo ( Satélite | wifi | Cabo | fibra )
- Vertices por tipo de dispositivo ( Servidor | Switch | Computador | Access Point )


### Restrições de Conexção

- Servidor só conecta com switch
- Computador conecta com switch e com Access Point
- Access Point só conecta com Switch


### Exemplo de mermaid

```mermaid
graph TD
    A[Servidor 1] -- Fibra --> B[Switch 1]
    B -- WiFi --> C[Computador 1]
    B -- Cabo --> D[Access Point 1]
    D -- WiFi --> E[Computador 2]
    B -- Satélite --> F[Servidor 2]
    F -- Fibra --> G[Switch 2]
    G -- Cabo --> H[Computador 3]
    G -- WiFi --> I[Access Point 2]
    I -- WiFi --> J[Computador 4]
    G -- Satélite --> K[Servidor 3]
    B -- Cabo --> L[Computador 5]
    L -- WiFi --> M[Computador 6]
    D -- Cabo --> N[Computador 7]
```

Neste exemplo:
- Cada nó representa um dispositivo na rede, podendo ser: Servidor, Switch, Computador ou Access Point.
- As setas representam conexões, rotuladas pelo tipo: WiFi, Cabo, Satélite, Fibra.
