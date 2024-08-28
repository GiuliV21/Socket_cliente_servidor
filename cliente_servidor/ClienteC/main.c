#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>


#define PORT 8080

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char option;
    int length;

    // Inicializar Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Error: WSAStartup falló. Código de error: %d\n", WSAGetLastError());
        return 1;
    }

    // Crear socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Error: No se pudo crear el socket. Código de error: %d\n", WSAGetLastError());
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Conectar al servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Error de conexión\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("Seleccione una opción:\nU: Generar nombre de usuario\nP: Generar contraseña\n");
    scanf(" %c", &option);

    if (option == 'U' || option == 'P') {
        printf("Ingrese la longitud deseada: ");
        scanf("%d", &length);

        snprintf(buffer, sizeof(buffer), "%c%d", option, length);
        send(sock, buffer, strlen(buffer), 0);

        int valread = recv(sock, buffer, sizeof(buffer), 0);
        if (valread > 0) {
            buffer[valread] = '\0';
            printf("%s\n", buffer);
        }
    } else {
        printf("Opción no válida.\n");
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
