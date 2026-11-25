# sistema de Registro e Gestão de Contas Bancárias - Etapa 3

trabalho da disciplina INF101- programação de Computadores I (UNIVIÇOSA).

## sobre o projeto

continuação da Etapa 2, agora os dados não se perdem mais quando o programa fecha, as contas e as movimentações são salvas em arquivos de texto e carregadas de novo quando o programa abre.

usei  `fstream` (`ifstream`, `ofstream`, `is_open()`, `getline()` e `close()`). os arrays agr sao globais pra poder usar nas funções de leitura e gravação.

## arquivos

- `contas.txt`: uma conta por linha, campos separados por `;`

```
1001;Ana Silva;12345678900;1;700.00;1
```

número;nome;cpf;tipo;saldo;situação (1 = ativa, 0 = inativa)

- `movimentacoes.txt`: uma movimentação por linha

```
1001;D;500.00
1001;S;300.00
```

numero da conta;tipo (D = depósito, S = saque);valor

na transferência ficam dois registros: um `S` na origem e um `D` no destino.

se os arquivos não existirem (primeira vez rodando) o programa começa vazio e cria eles quando salvar a primeira coisa.

## funções

- `carregarContas()`: lê o `contas.txt` e preenche os arrays
- `salvarContas()`: grava todas as contas no `contas.txt`
- `carregarMovimentacoes()`: lê o `movimentacoes.txt`
- `salvarMovimentacoes()`: grava o histórico no `movimentacoes.txt`

o carregamento acontece no começo do `main()`. o salvamento acontece depois de cadastrar conta, depositar, sacar e transferir.

## menu

1. Cadastrar conta
2. Listar contas
3. Consultar conta
4. Depositar
5. Sacar
6. Transferir
7. Extrato
8. Sair

## como compilar

```
g++ banco.cpp -o banco
./banco
```

##OBS
não consegui rodar pelo onlinegdb, utilizei a aba Codespaces do github e ai sim deu certo,aparentemente o gdb nao estava guardando o contas.txt

## autor

alice lima
