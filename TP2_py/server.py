import socket
import threading

# Number of clients to handle
NUM_CLIENTS_SOURCE = 3
PORT_SOURCE = 5000
PORT_CLIENT = 5005
IP = '127.0.0.1'

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
        server_address = (IP, PORT_SOURCE)
        source_socket.bind(server_address)
        source_socket.listen(NUM_CLIENTS_SOURCE)
        print("Server is listening on port 5000...")

        data_received_source = source_socket.recvfrom(1024)
        print(f"Received data from client: {data_received_source.decode()}")

    except socket.error as e:
        print("Socket error:", e)

    finally:
        source_socket.close()


def start_client_server():
    try:
        clients_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        server_address = (IP, PORT_CLIENT)
        clients_socket.bind(server_address)

        print("Server is listening on port 5005...")

        data_received_clients = clients_socket.recvfrom(1024)
        print(f"Received data from client: {data_received_clients.decode()}")

    except socket.error as e:
        print("Socket error:", e)

    finally:
        clients_socket.close()


if __name__ == "__main__":
    source_thread = threading.Thread(target=start_source_server(), args=())
    clients_thread = threading.Thread(target=start_client_server(), args=())
