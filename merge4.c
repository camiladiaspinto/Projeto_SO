#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include "merge2.h"

int merge4(int fi[4], int fo) {
    fi[0] = open("f1.txt", O_RDONLY);
    fi[1] = open("f2.txt", O_RDONLY);
    fi[2] = open("f3.txt", O_RDONLY);
    fi[3] = open("f4.txt", O_RDONLY);
    //verifica se os ficheiros de entrada foram abertos corretamente
    for (int i = 0; i < 4; ++i) {
        if (fi[i] == -1) {
            perror("Erro ao abrir um dos ficheiros de entrada");
            exit(EXIT_FAILURE);
        }
    }
    pid_t pid1, pid2;
    int fd1[2];
    int fd2[2];

    //criar os pipes 
    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        perror("Erro ao criar pipe");
        _exit(1);
    }
    //criar os filhos
    pid1 = fork();
       if (pid1 == 0) { // processo filho 1
       //fecha descritor de leitura
        close(fd1[0]);

  

        dup2(fd1[1], STDOUT_FILENO);

        //ordenar os fragmentos, este merge foi testado e esta a dar certo 

        merge2(fi[0], fi[1], STDOUT_FILENO);

        

        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);
        exit(0);
    } else {
        pid2 = fork();
        if (pid2 == 0) { // processo filho 2
            close(fd2[0]);
            // Redireciona a saída padrão para o pipe
            dup2(fd2[1], STDOUT_FILENO);

            //este merge foi testado, mas nao esta a dar certo 
            merge2(fi[2], fi[3], STDOUT_FILENO);

            close(fd2[1]);
            close(fd1[0]);
            close(fd1[1]);
            exit(0);
        } else { // processo pai
            close(fd1[1]);
            close(fd2[1]);

            merge2(fd1[0], fd2[0], fo);


            close(fd1[0]);
            close(fd2[0]);
            close(fo);

            wait(NULL);
            wait(NULL);
            return 0;
        }
    }
}

int main() {
    int fi[4];
    int fo;
    fo = open("output2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);    
    
    merge4(fi, fo);



    return 0;
}
