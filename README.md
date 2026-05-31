# Conversor Universal de Bases Numéricas

Trabalho prático da disciplina **GCC241 – Introdução à Computação**  
Universidade Federal de Lavras (UFLA) — 2026/1

---

# Vídeo apresentação

Vídeo que apresenta o trabalho prático da disciplina: https://youtu.be/iJvx9nAbs30

---


## Descrição

Programa em C++ que converte números inteiros e fracionários entre as bases **2, 8, 10 e 16**, implementando todos os algoritmos do zero — sem uso de funções prontas de conversão (`strtol`, `bin()`, `hex()`, etc.).

---

## Funcionalidades

| Opção | Descrição |
|-------|-----------|
| 1 | Decimal → Binário / Octal / Hexadecimal |
| 2 | Binário / Octal / Hex → Decimal |
| 3 | Binário ↔ Octal e Binário ↔ Hex (agrupamento direto de bits) |
| 4 | Octal ↔ Hexadecimal (via binário como intermediário) |
| 5 | Modo Passo a Passo (qualquer conversão, com raciocínio detalhado) |
| 6 | Modo Batch (lê `entrada.csv`, gera `saida.csv`) |
| 7 | Quiz interativo (100 questões, 5 níveis de dificuldade) |
| 8 | Calculadora de máximos (dado k bits, mostra 2ᵏ − 1 em todas as bases) |
| 9 | Sair |

Suporte a **números fracionários** (ex: `10.625`) em todas as conversões.

---

## Estrutura do projeto

```
.
├── main.cpp            # Menu principal, modo batch e quiz
├── conversor.cpp/.h    # Todos os algoritmos de conversão
├── parser.cpp/.h       # Validação e leitura de entradas
├── formatador.cpp/.h   # Saída formatada no terminal
├── test_conversor.cpp  # Suite de testes (50+ casos)
└── README.md
```

---

## Compilação e execução

### Pré-requisitos

- Compilador C++ com suporte a C++11 ou superior (`g++` recomendado)

### Compilar o programa principal

```bash
g++ -o conversor main.cpp conversor.cpp parser.cpp formatador.cpp
```

### Executar

```bash
./conversor
```

---

## Testes

A suite de testes cobre **83 casos** distribuídos entre todas as funções do projeto:

| Grupo | O que testa |
|-------|-------------|
| F1 | Decimal → Bin / Oct / Hex (incluindo 0, potências de 2, limites) |
| F2 | Bin / Oct / Hex → Decimal |
| F3 | Agrupamento de bits: Bin ↔ Oct (3 bits) e Bin ↔ Hex (4 bits) |
| F4 | Octal ↔ Hex via binário intermediário |
| F5 | Validação de entradas (válidas e inválidas para cada base) |
| F6 | Fracionários (parte inteira e fracionária separadamente) |
| F10 | Valores máximos para 4, 8 e 16 bits |

### Executar os testes (único comando)

```bash
g++ -o testes test_conversor.cpp conversor.cpp parser.cpp && ./testes
```

> Todos os arquivos devem estar no mesmo diretório.

### Saída esperada

```
  ============================================
   SUITE DE TESTES — GCC241 / UFLA 2026/1
  ============================================

  ── F1: Decimal -> Binario ──
  [OK]   0  -> bin
  [OK]   1  -> bin
  ...
  ── F5: Validacao de entradas ──
  [OK]   '1010' valido  base 2
  [OK]   '1012' invalido base 2
  ...
  ============================================
  Total  : 83
  OK     : 83
  FAIL   : 0
  Status : TUDO OK
  ============================================
```

Se algum teste falhar, a linha correspondente mostrará `[FAIL]` com o valor esperado e o obtido. O processo retorna código de saída `1` em caso de falha (útil para integração com CI/CD).

---

## Formato do arquivo de entrada (modo Batch)

O arquivo `.csv` deve usar `;` como separador e ter cabeçalho:

```
valor;base_origem;base_destino
1010;2;10
FF;16;8
255;10;2
10.5;10;2
```

O resultado é gravado em `saida.csv` no mesmo diretório.

---

## Limitações conhecidas

- **Fracionários com origem diferente de decimal:** a conversão passa pelo decimal como intermediário, o que implica duas representações em ponto flutuante e pode acumular pequeno erro numérico em casos extremos. Para os valores usuais do trabalho (até 16 casas) o resultado é correto.
- **Inteiros negativos:** suportados apenas em decimal (bases 2, 8 e 16 não definem representação de sinal neste trabalho).
- **Magnitude máxima:** a parte inteira é armazenada em `long long` (64 bits), suportando valores até 2⁶² − 1. A opção 8 (calculadora de máximos) aceita até k = 62 por este motivo.

---

## Algoritmos implementados

| Conversão | Algoritmo |
|-----------|-----------|
| Decimal → base B | Divisões sucessivas, leitura dos restos de baixo para cima |
| Base B → decimal | Somatório posicional (dígito × baseᵖᵒˢⁱçãᵒ) |
| Bin → Oct | Agrupamento de 3 bits da direita para a esquerda |
| Bin → Hex | Agrupamento de 4 bits da direita para a esquerda |
| Oct/Hex → Bin | Expansão de cada dígito em 3 ou 4 bits |
| Oct ↔ Hex | Passagem obrigatória pelo binário intermediário |
| Fracionários | Multiplicações sucessivas pela base destino |

Nenhuma função de biblioteca realiza conversão de base — tudo é feito manualmente, caractere a caractere.

---

## Níveis do Quiz

| Nível | Faixa de valores |
|-------|-----------------|
| 1 — Iniciante | 1 a 15 |
| 2 — Básico | 16 a 255 |
| 3 — Intermediário | 256 a 4.095 |
| 4 — Avançado | 4.096 a 65.535 |
| 5 — Expert | 65.536 a 1.048.575 |

Cada questão apresenta 5 alternativas (a–e) geradas aleatoriamente. Digite `0` para encerrar antes das 100 questões.
