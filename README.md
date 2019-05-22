# Sistema de Arquivos EXT3

Este sistema foi desenvolvido como proposta de trabalho para a disciplina de Organização de Sistema Operacionais.

Sistema implementado na linguagem C utilizando compilador GNU GCC;
Utilizado chamada de sistemas do padrão POSIX;

O sistema de arquivos implementado pode realizar as seguintes operações:

- criar (sub)diretório
- remover (sub)diretório
- entrar em (sub)diretório
- mostrar conteúdo do diretório
- criar arquivo
- remover arquivo
- escrever um sequência de caracteres em um arquivo
- mostrar conteúdo do arquivo

#Como rodar o sistema ?

1. Execute o makefile
2. Após executado o script, execute o sistema digitando ./server/server
3. Com a comunicação agora aberta, para você se comunicar com o socket TCP, abra um novo terminal e se você se encontra no linux execute o comando nc localhost 3000
