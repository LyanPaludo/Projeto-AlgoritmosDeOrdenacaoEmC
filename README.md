# Projeto-AlgoritmosDeOrdenacaoEmC

Projeto da disciplina PROJETO INTEGRADOR 1, envolvendo benchmark de diferentes tipos de algoritmos de ordenação para diferentes tipos de listas em C.

## Estrutura

```text
projeto/
├── src/
│   ├── sorts.c / sorts.h      # algoritmos
│   ├── generators.c / .h      # geração das listas
│   ├── benchmark.c            # main com as medições
│   └── utils.c / utils.h      # cópia de vetor, verificação, etc.
├── resultados/                # CSVs e gráficos
├── Makefile
└── README.md
```

## Como compilar e executar

```bash
make
make run
```

O `Makefile` detecta automaticamente o sistema operacional. No Windows,
ele gera `benchmark.exe` e usa os comandos compatíveis com o terminal do
Windows; no Linux, gera `benchmark` e usa os comandos tradicionais do Unix.
