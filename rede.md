``` mermaid
graph TD
    0["Servidor 1"]
    1["Switch 1"]
    2["Computador 1"]
    3["Access Point 1"]
    4["Computador 2"]
    5["Servidor 2"]
    6["Computador 3"]
    7["Access Point 2"]
    8["Computador 4"]
    9["Servidor 3"]
    10["Computador 5"]
    11["Computador 6"]
    12["Computador 7"]
    13["Teste1"]
    0 -- Fibra --> 1
    1 -- Cabo --> 10
    1 -- Satélite --> 5
    1 -- Cabo --> 3
    1 -- WiFi --> 2
    3 -- Cabo --> 12
    3 -- WiFi --> 4
    7 -- WiFi --> 8
    10 -- WiFi --> 11
