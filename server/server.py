import socket
import sys
import time

ADDRESS = '127.0.0.1'
PORT = 8080

def main():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((ADDRESS, PORT))
    
    server_socket.listen(5)
    print("Server listening on port 8080...")

    # Accept a new connection
    client_socket, client_address = server_socket.accept()
    print(f"Connection from {client_address}")

    while True:
        # Receive data from the client
        try:
           data = client_socket.recv(8024).decode('utf-8')
        except UnicodeDecodeError:
           print("Received data could not be decoded as UTF-8")

        sys.stdout.write(data)

        command = input()
        command += "\n"

        client_socket.send(command.encode())
        time.sleep(0.2)

if __name__ == "__main__":
    main()
