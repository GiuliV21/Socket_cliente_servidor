#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib") // Enlaza la biblioteca de sockets de Windows

#define PORT 8080
#define MAX_CLIENTS 5

// Función para generar un nombre de usuario
void generate_username(int length, char *username) {
    const char vowels[] = "aeiou";
    const char consonants[] = "bcdfghjklmnpqrstvwxyz";
    int is_vowel = rand() % 2;

    for (int i = 0; i < length; i++) {
        if (is_vowel) {
            username[i] = vowels[rand() % (sizeof(vowels) - 1)];
        } else {
            username[i] = consonants[rand() % (sizeof(consonants) - 1)];
        }
        is_vowel = !is_vowel; // Alternar entre vocal y consonante
    }
    username[length] = '\0';
}

// Función para generar una contraseña
void generate_password(int length, char *password) {
    const char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    for (int i = 0; i < length; i++) {
        password[i] = characters[rand() % (sizeof(characters) - 1)];
    }
    password[length] = '\0';
}

int main() {
    WSADATA wsa;
    SOCKET server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char response[1024];

    srand(time(NULL)); // Semilla para números aleatorios

    // Inicializar Winsock
    printf("Inicializando Winsock...\n");
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Error: WSAStartup falló. Código de error: %d\n", WSAGetLastError());
        return 1;
    }

    // Crear socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Error: No se pudo crear el socket. Código de error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Configurar socket
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
        printf("Error: setsockopt falló. Código de error: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Asociar socket al puerto
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
        printf("Error: bind falló. Código de error: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // Escuchar conexiones
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        printf("Error: listen falló. Código de error: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printf("Servidor escuchando en el puerto %d...\n", PORT);

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (int *)&addrlen)) == INVALID_SOCKET) {
            printf("Error: accept falló. Código de error: %d\n", WSAGetLastError());
            closesocket(server_fd);
            WSACleanup();
            return 1;
        }

        int valread = recv(new_socket, buffer, 1024, 0);
        if (valread <= 0) {
            closesocket(new_socket);
            continue;
        }

        int length;
        char option = buffer[0];
        sscanf(buffer + 1, "%d", &length);

        if (option == 'U') {
            if (length < 5 || length > 15) {
                sprintf(response, "Error: La longitud del nombre de usuario debe ser entre 5 y 15.\n");
            } else {
                char username[16];
                generate_username(length, username);
                sprintf(response, "Nombre de usuario generado: %s\n", username);
            }
        } else if (option == 'P') {
            if (length < 8 || length > 50) {
                sprintf(response, "Error: La longitud de la contraseña debe ser entre 8 y 50.\n");
            } else {
                char password[51];
                generate_password(length, password);
                sprintf(response, "Contraseña generada: %s\n", password);
            }
        } else {
            sprintf(response, "Error: Opción no válida.\n");
        }

        send(new_socket, response, strlen(response), 0);
        closesocket(new_socket);
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}
