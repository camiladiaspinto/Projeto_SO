#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include"merge2.h"

#define MAX_RECORD_SIZE 16
#define BLOCK_SIZE 1024


int readLine(int fd, char *buffer, size_t size) {
    size_t bytesRead = 0;

    while (read(fd, buffer + bytesRead, 1) > 0 && bytesRead < size - 1) {
        if (buffer[bytesRead] == '\n') {
            break;
        }
        bytesRead++;
    }

    buffer[bytesRead] = '\0';

    return bytesRead;
}

int writeLine(int fd, const char *buffer) {
    size_t length = strlen(buffer);
    size_t bytesWritten = 0;

    while (bytesWritten < length) {
        ssize_t result = write(fd, buffer + bytesWritten, length - bytesWritten);
        if (result == -1) {
            perror("Erro ao escrever no arquivo");
            return -1; // Erro de escrita
        }
        bytesWritten += result;
    }

    return 0; // Sucesso
}

int merge2(int fi1, int fi2, int fo) {

    char record1[MAX_RECORD_SIZE + 1];
    char record2[MAX_RECORD_SIZE + 1];

    int hasRecord1 = readLine(fi1, record1, sizeof(record1)) > 0;
    int hasRecord2 = readLine(fi2, record2, sizeof(record2)) > 0;

    while (hasRecord1 || hasRecord2) {
        while (hasRecord1 && hasRecord2) {
            if (strcmp(record1, record2) <= 0) {
                writeLine(fo, record1);
                writeLine(fo, "\n");  // Adiciona quebra de linha após cada registro
                hasRecord1 = readLine(fi1, record1, sizeof(record1)) > 0;
            } else {
                writeLine(fo, record2);
                writeLine(fo, "\n");  // Adiciona quebra de linha após cada registro
                hasRecord2 = readLine(fi2, record2, sizeof(record2)) > 0;
            }
        }

        while (hasRecord1) {
            writeLine(fo, record1);
            writeLine(fo, "\n");  // Adiciona quebra de linha após cada registro
            hasRecord1 = readLine(fi1, record1, sizeof(record1)) > 0;
        }

        while (hasRecord2) {
            writeLine(fo, record2);
            writeLine(fo, "\n");  // Adiciona quebra de linha após cada registro
            hasRecord2 = readLine(fi2, record2, sizeof(record2)) > 0;
        }
    }

    // Fechar os descritores de arquivo
    close(fi1);
    close(fi2);
    close(fo);

    return 0;
}
//int main() {
  //  int fd1 = open("f3.txt", O_RDONLY);
    //int fd2 = open("f4.txt", O_RDONLY);
    //int fo=open("r.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);

    //merge2(fd1,fd2,fo);

//}

