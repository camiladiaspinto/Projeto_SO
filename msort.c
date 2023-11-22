#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include "merge4.h"

int msort(int fi[4], int fo) {
    pid_t pid[4];
    int fd[4][2];

    // Cria os pipes
    for (int i = 0; i < 4; ++i) {
        if (pipe(fd[i]) == -1) {
            perror("Erro ao criar pipe");
            _exit(1);
        }
    }
    //criar os filhos
    pid[1] = fork();
    if (pid[1] == 0) { // processo filho 1
        //fecha descritor de leitura
        close(fd[1][0]);
        //redireciona a saída padrão para o pipe 
        dup2(fd[1][1], STDOUT_FILENO);

        //ordenar os fragmentos, este merge foi testado e esta a dar certo 
        execlp("sort", "sort", "f1.txt", (char *)NULL);
        //fechar descritor que nao estão a ser usados
        close(fd[1][1]);
        close(fd[2][0]);
        close(fd[2][1]);
        close(fd[3][0]);
        close(fd[3][1]);
        close(fd[4][0]);
        close(fd[4][1]);
        exit(0);
    } else {
        pid[2] = fork();
        if (pid[2] == 0) { // processo filho 2
            //fecha descritor de leitura
            close(fd[2][0]);
            //redireciona a saída padrão para o pipe 
            dup2(fd[2][1], STDOUT_FILENO);

            //ordenar os fragmentos, este merge foi testado e esta a dar certo 
            execlp("sort", "sort", "f2.txt", (char *)NULL);
            //fechar descritor que nao estão a ser usados
            close(fd[2][1]);
            close(fd[1][0]);
            close(fd[1][1]);
            close(fd[3][0]);
            close(fd[3][1]);
            close(fd[4][0]);
            close(fd[4][1]);
            exit(0);
        }else {
            pid[3] = fork();
            if (pid[3] == 0) { // processo filho 3
                //fecha descritor de leitura
                close(fd[3][0]);
                //redireciona a saída padrão para o pipe 
                dup2(fd[3][1], STDOUT_FILENO);

                //ordenar os fragmentos, este merge foi testado e esta a dar certo 
                execlp("sort", "sort", "f3.txt", (char *)NULL);
                //fechar descritor que nao estão a ser usados
                close(fd[3][1]);
                close(fd[1][0]);
                close(fd[1][1]);
                close(fd[2][0]);
                close(fd[2][1]);
                close(fd[4][0]);
                close(fd[4][1]);
                exit(0);
            }else {
                pid[4] = fork();
                if (pid[4] == 0) { // processo filho 4
                    //fecha descritor de leitura
                    close(fd[4][0]);
                    //redireciona a saída padrão para o pipe 
                    dup2(fd[4][1], STDOUT_FILENO);

                    //ordenar os fragmentos, este merge foi testado e esta a dar certo 
                    execlp("sort", "sort", "f4.txt", (char *)NULL);
                    //fechar descritor que nao estão a ser usados
                    close(fd[4][1]);
                    close(fd[1][0]);
                    close(fd[1][1]);
                    close(fd[2][0]);
                    close(fd[2][1]);
                    close(fd[3][0]);
                    close(fd[3][1]);
                    exit(0);
                }else { // processo pai
                    // Processo Pai
                    int result_fds[4]; // Matriz para armazenar os descritores dos pipes 

                    for (int i = 0; i < 4; ++i) {
                        close(fd[i][1]); // Fecha os descritores de escrita do processo pai
                        result_fds[i] = fd[i][0]; // Salve o descritor de leitura do pipe 
                    }

                    // Utiliza a função merge4 para ordenar o resultado os processos filho
                    merge4(result_fds, fo);

                    // Fecha os restantes descritores
                    for (int i = 0; i < 4; ++i) {
                        close(result_fds[i]);
                    }

                    // Espera que todos os filhos terminem
                    for (int i = 0; i < 4; ++i) {
                        waitpid(pid[i], NULL, 0);
                    }
                    return 0;
                }
            }
        }
    }
}


int main() {
    int fi[4];
    int fo;

    fi[0] = open("f1.txt", O_RDONLY);
    fi[1] = open("f2.txt", O_RDONLY);
    fi[2] = open("f3.txt", O_RDONLY);
    fi[3] = open("f4.txt", O_RDONLY);

    // Verificar se os arquivos de entrada foram abertos corretamente
    for (int i = 0; i < 4; ++i) {
        if (fi[i] == -1) {
            perror("Erro ao abrir um dos ficheiros de entrada");
            exit(EXIT_FAILURE);
        }
    }

    fo = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fo == -1) {
        perror("Erro ao abrir o arquivo de saída");
        exit(EXIT_FAILURE);
    }

    msort(fi, fo);

    return 0;
}