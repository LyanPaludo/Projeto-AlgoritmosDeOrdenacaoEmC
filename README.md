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

## Requisitos

Para compilar e executar o projeto, é necessário ter instalados:

- um compilador C, como o **GCC**;
- o **GNU Make**;
- o **Git**, caso o projeto seja baixado usando `git clone`.

No Windows, a forma mais simples de instalar o compilador e o Make é utilizar o [MSYS2](https://www.msys2.org/).

## Windows: instalação do ambiente

### 1. Instale o MSYS2

1. Baixe e instale o MSYS2 pelo site oficial: <https://www.msys2.org/>.
2. Após a instalação, abra o terminal **MSYS2 UCRT64** pelo menu Iniciar.

> É importante utilizar o terminal **MSYS2 UCRT64**, pois ele já fornece um ambiente compatível com o GCC e o Make usados pelo projeto.

### 2. Instale o compilador e o Make

No terminal **MSYS2 UCRT64**, execute:

```bash
pacman -Syu
```

Se o terminal for fechado durante a atualização, abra-o novamente e execute o comando acima mais uma vez, se necessário. Depois, instale o compilador GCC e o GNU Make:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc make
```

Quando o MSYS2 perguntar se deseja continuar, confirme com `Y` e pressione `Enter`.

### 3. Verifique a instalação

Ainda no terminal **MSYS2 UCRT64**, execute:

```bash
gcc --version
make --version
```

Se os dois comandos exibirem informações de versão, o compilador e o Make estão instalados corretamente.

> Caso `gcc` ou `make` não sejam encontrados, confira se o terminal aberto é realmente o **MSYS2 UCRT64**. O projeto não poderá ser compilado enquanto esses comandos não estiverem disponíveis no `PATH`.

## Windows: baixar, compilar e executar

### 1. Baixe o projeto

Se o Git estiver instalado, clone o repositório:

```bash
git clone https://github.com/LyanPaludo/Projeto-AlgoritmosDeOrdenacaoEmC.git
cd Projeto-AlgoritmosDeOrdenacaoEmC
```

Outra opção é baixar o projeto como arquivo `.zip` pelo GitHub e extrair os arquivos. Nesse caso, abra o terminal **MSYS2 UCRT64**, navegue até a pasta extraída e entre nela. Por exemplo:

```bash
cd /c/Users/SeuUsuario/Downloads/Projeto-AlgoritmosDeOrdenacaoEmC-main
```

No MSYS2, as unidades do Windows são acessadas com `/c/`, `/d/` e assim por diante.

### 2. Compile o projeto

Dentro da pasta do projeto, execute:

```bash
make
```

Esse comando compila os arquivos `.c` e gera o executável `benchmark.exe`. Os arquivos intermediários da compilação são armazenados na pasta `build`.

### 3. Execute o benchmark

Para compilar, caso ainda seja necessário, e executar o programa, use:

```bash
make run
```

Também é possível executar o arquivo diretamente no terminal MSYS2 UCRT64:

```bash
./benchmark.exe
```

### 4. Limpe os arquivos gerados

Para remover o executável e os arquivos intermediários da compilação, execute:

```bash
make clean
```

## Uso no Prompt de Comando ou no PowerShell

O comando `make` só funcionará no Prompt de Comando ou no PowerShell se as pastas que contêm o GCC e o Make estiverem adicionadas à variável de ambiente `PATH` do Windows. Caso `make` não seja reconhecido nesses terminais, use o terminal **MSYS2 UCRT64**, conforme descrito acima.

Depois de configurar o `PATH`, verifique a instalação no PowerShell ou no Prompt de Comando com:

```powershell
gcc --version
make --version
```

Se os comandos continuarem não sendo encontrados, o ambiente ainda não está configurado corretamente. Nesse caso, volte a utilizar o terminal **MSYS2 UCRT64** ou configure o `PATH` do Windows apontando para as pastas correspondentes da instalação do MSYS2.

## Linux

No Linux, instale o compilador GCC, o GNU Make e o Git usando o gerenciador de pacotes da sua distribuição. Em distribuições baseadas em Debian ou Ubuntu, por exemplo:

```bash
sudo apt update
sudo apt install build-essential git
```

Depois, dentro da pasta do projeto, compile e execute com:

```bash
make
make run
```

O `Makefile` detecta automaticamente o sistema operacional. No Windows, ele gera `benchmark.exe` e utiliza os comandos compatíveis com o terminal do Windows; no Linux, gera `benchmark` e utiliza os comandos tradicionais do Unix.
