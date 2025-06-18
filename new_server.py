import socket
import threading

# Server configuration
host = '192.168.252.52'
port = 12345

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)  # Avoid WinError 10048
server.bind((host, port))
server.listen()

clients = []
nicknames = []

# Broadcast message to all clients
def broadcast(message):
    for client in clients:
        client.send(message)

# Handle a single client
def handle(client):
    while True:
        try:
            message = client.recv(2048)
            if message:
                index = clients.index(client)
                nickname = nicknames[index]
                full_message = f"{nickname}: {message.decode('utf-8')}"
                print(full_message)
                broadcast(full_message.encode('utf-8'))
        except:
            index = clients.index(client)
            client.close()
            nickname = nicknames[index]
            clients.remove(client)
            nicknames.remove(nickname)
            broadcast(f"{nickname} left the chat.".encode('utf-8'))
            break

# Accept new connections
def receive():
    print("Server is listening...")
    while True:
        client, address = server.accept()
        print(f"Connected with {str(address)}")

        client.send("NICK".encode('utf-8'))  # Ask for nickname
        nickname = client.recv(1024).decode('utf-8')

        nicknames.append(nickname)
        clients.append(client)

        print(f"Nickname of client: {nickname}")
        broadcast(f"{nickname} joined the chat!".encode('utf-8'))
        client.send("Welcome to the group chat!".encode('utf-8'))

        thread = threading.Thread(target=handle, args=(client,))
        thread.start()

receive()
