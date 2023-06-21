import socket
import threading

# Number of clients to handle
NUM_CLIENTS_SOURCE = 3
PORT_SOURCE = 5000
PORT_CLIENT = 5005

# Shared data across threads
shared_data = []

# Lock for thread synchronization
data_lock = threading.Lock()


def handle_source(source_socket):
    try:
        while True:
            data = source_socket.recv(1024)
            if not data:
                break
            # Process the received data from the client
            # Add your custom logic here
            print(f"Received data from client: {data.decode()}")

    except socket.error as e:
        print("Socket error:", e)

    finally:
        source_socket.close()


def handle_client(client_socket):
    try:
        while True:
            data = client_socket.recv(1024)
            if not data:
                break

            # Process the received data from the client
            # Add your custom logic here
            print(f"Received data from client: {data.decode()}")

    except socket.error as e:
        print("Socket error:", e)

    finally:
        client_socket.close()



def start_source_server():
    try:
        source_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        server_address = ('127.0.0.1', PORT_SOURCE)
        source_socket.bind(server_address)
        source_socket.listen(NUM_CLIENTS_SOURCE)
        print("Server is listening on port 5000...")

        client_threads = []
        for _ in range(NUM_CLIENTS_SOURCE):
            client_socket, client_address = source_socket.accept()
            print(f"Accepted connection from client: {client_address}")
            client_thread = threading.Thread(target=handle_source, args=(client_socket,))
            client_thread.start()
            client_threads.append(client_thread)

        # Wait for all client threads to finish
        for client_thread in client_threads:
            client_thread.join()

    except socket.error as e:
        print("Socket error:", e)

    finally:
        source_socket.close()


def start_client_server():
    try:
        clients_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        server_address = ('127.0.0.1', PORT_CLIENT)
        clients_socket.bind(server_address)
        clients_socket.listen(NUM_CLIENTS_SOURCE)
        print("Server is listening on port 5005...")

        client_threads = []
        for _ in range(NUM_CLIENTS_SOURCE):
            client_socket, client_address = clients_socket.accept()
            print(f"Accepted connection from client: {client_address}")
            client_thread = threading.Thread(target=handle_client, args=(client_socket,))
            client_thread.start()
            client_threads.append(client_thread)

        # Wait for all client threads to finish
        for client_thread in client_threads:
            client_thread.join()

    except socket.error as e:
        print("Socket error:", e)

    finally:
        client_socket.close()


if __name__ == "__main__":
    source_thread = threading.Thread(target=start_source_server(), args=())
    clients_thread = threading.Thread(target=start_client_server(), args=())
