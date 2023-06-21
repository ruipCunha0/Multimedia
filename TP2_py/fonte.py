import socket
import sys
import math


PI = 3.14159265


if __name__=="__main__":

    if len(sys.argv) < 3:
        print("ERROR, not enough args!")
        sys.exit(1)

    port = int(sys.argv[1])
    id_string = sys.argv[2]

    F = int(input("Set number for F: "))
    if F <= 0:
        print("ERROR!")
        sys.exit(1)

    N = int(input("Set number for N, N > 3: "))
    if N <= 3:
        print("ERROR!")
        sys.exit(1)

    Fa = F * N  # Calculate the sampled frequency

    try:
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        server_address = ('127.0.0.1', port)
        client_socket.connect(server_address)

        for i in range(Fa + 1):
            t = i / Fa
            sample = 1 + (1 + math.sin(2 * PI * t / N)) * 30

            message = f"{id_string}|{i}|{int(sample)}|0|{F}|{N}|0|"
            client_socket.sendall(message.encode())

        client_socket.close()

    except socket.error as e:
        print("Socket error:", e)
        sys.exit(1)
