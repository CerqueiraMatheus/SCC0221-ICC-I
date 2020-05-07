# Trabalhos de ICC 1

Trabalhos desenvolvidos em C por mim para a disciplina de ICC 1 do curso de Ciências de Computação do ICMC/USP.

Link da disciplina no JupiterWeb:
[https://uspdigital.usp.br/jupiterweb/obterDisciplina?sgldis=SCC0221&codcur=55041&codhab=0](https://uspdigital.usp.br/jupiterweb/obterDisciplina?sgldis=SCC0221&codcur=55041&codhab=0)


## Como executar

Para executar os arquivos usando Linux, basta seguir os passos:

 **1. Instalar o gcc**
 
 O gcc é um conjunto de compiladores de linguagens de programação; dentre elas: C. Instale-o:
 

    $ sudo apt update
    $ sudo apt install build-essential

 **2. Compile (ou execute) os arquivos das pastas**
 
 Caso queira apenas executar o arquivo já compilado, abra uma das pastas e, no terminal:

    $ ./nomedoarquivo
   
Se quiser fazer alterações, será necessário compilar novamente:

    $ gcc -o nomedoarquivo nomedoarquivo.c
Após, apenas execute o novo arquivo gerado.

## Sobre os projetos

**1.  Jogo da velha**

Jogo da velha simples, apenas com X e O e com marcadores de 1 a 9 para seleção de posições.

**2. Bar Plot**

Recebe um conjunto de dados e armazena as repetições. Após, exibe graficamente a quantidade de repetições.

**3. Binário para Decimal (Binary to Decimal)**

Conversor simples.
