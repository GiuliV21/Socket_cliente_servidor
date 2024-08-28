import socket

def main():
    host = '127.0.0.1'
    port = 8080

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((host, port))

    print("Seleccione una opción:")
    print("U: Generar nombre de usuario")
    print("P: Generar contraseña")
    option = input("Opción: ").strip().upper()

    if option in ('U', 'P'):
        length = input("Ingrese la longitud deseada: ").strip()
        message = f"{option}{length}"
        client_socket.send(message.encode())

        response = client_socket.recv(1024).decode()
        print(response)
    else:
        print("Opción no válida.")

    client_socket.close()

if __name__ == "__main__":
    main()