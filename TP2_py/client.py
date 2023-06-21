import socket

# Server configuration
IP = '127.0.0.1'
PORT = 5005

def menu():
    # Start receiving messages
    while True:
        # Display the menu
        print("\n=== MENU ===")
        print("1. Start Server")
        print("2. Exit")

        # Get user choice
        choice = input("Enter your choice: ")

        if choice == '1':

            client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            server_address = (IP, PORT)
            client_socket.connect(server_address)

            print("1. list")
            print("2. info (D)")
            print("3. play (D)")
            print("4. stop (D)")

            choice_2 = input("Enter option: ")

            if choice_2 == '1':

                message_to_send = "list"

                # Send to the server the option
                client_socket.sendall(message_to_send.encode())

                # Receive from the server the available channels
                data_received = client_socket.recv(1024)
                print(f"Available channels: {data_received.decode()}")


            elif choice_2 == '2':

                string_id = str(input("Enter the channel id: "))

                message_to_send = "info" + " " + string_id

                # Send to the server the option
                client_socket.send(message_to_send.encode())

                data_received = client_socket.recv(1024)
                print(f"Available channels: {data_received.decode()}")

            elif choice_2 == '3':

                string_id = str(input("Enter the channel id: "))

                # Send to the server the option
                message_to_send = "play" + " " + string_id

                client_socket.sendall(message_to_send.encode())

            elif choice_2 == '4':

                string_id = str(input("Enter the channel id: "))

                # Send to the server the option
                message_to_send = "stop" + " " + string_id

                client_socket.sendall(message_to_send.encode())

            elif choice_2 == '5':
                print("Exiting the program...")
                break

            else:
                print("Invalid choice. Please try again.")

        elif choice == '2':
            print("Exiting the program...")
            break

        else:
            print("Invalid choice. Please try again.")

    # Close the server socket
    client_socket.close()


if __name__ == "__main__":
    menu()
