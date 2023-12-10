#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include "merge2.h"


void mergeN(int fi[], int fo, int N) {
    if (N == 2) {
        // Caso base: dois ficheiros, chama a função merge2
        merge2(fi[0], fi[1], fo);
        return;
    }
    //dividir os ficheiros em duas metades 
    int half = N / 2;
    int fd[2];
    pid_t pid1, pid2;

    // Criar pipe
    if (pipe(fd) == -1) {
        perror("Erro ao criar pipe");
        _exit(1);
    }

    pid1 = fork();
    if (pid1 == 0) { //primeiro filho
        close(fd[0]);
        //redireciona saida para pipe
        dup2(fd[1], STDOUT_FILENO);

        // Chama mergeN recursivamente para o primeira metade de ficheiros
        mergeN(fi, fd[1], half);
        exit(0);
    }

   
    pid2 = fork();
    if (pid2 == 0) { //segundo filho
        close(fd[1]);
        //redireciona entrada para pipe
        dup2(fd[0], STDIN_FILENO);

        // Chama mergeN recursivamente para a ultima
        mergeN(fi + half, fo, N - half);
        exit(0);
    }

    // Processo pai
    close(fd[0]);
    close(fd[1]);
    close(fo);
    wait(NULL);
    wait(NULL);

}

int main(int argc, char *argv[]) {
    int N = atoi(argv[1]); // Número de ficheiros
    int fi[N];
    int fo;
    fo = open("output1.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);

    for (int i=0; i< N;i++){
        fi[i] = open(argv[i + 2], O_RDONLY);

        if (fi[i] == -1) {
            perror("Erro ao abrir um dos ficheiros de entrada");
            exit(EXIT_FAILURE);
        }
    }

    // Chama mergeN para fundir os N ficheiros
    mergeN(fi, fo, N);

    return 0;
}
