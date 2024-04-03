import socket
import threading
import time

def connect_to_server():
    try:
        # Connect to the server
        server_ip = '0.0.0.0'
        server_port = 1111
        irc_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        irc_socket.connect((server_ip, server_port))

        # Send data (You may need to adjust this according to your IRC server protocol)
        nickname = "User" + str(threading.current_thread().ident)
        irc_socket.send(bytes("PASS Password\r\n", "UTF-8"))  # Change 'your_password' to your actual password
        time.sleep(0.2)
        irc_socket.send(bytes("NICK {}\r\n".format(nickname), "UTF-8"))
        time.sleep(0.2)
        irc_socket.send(bytes("USER {} 0 * \r\n".format(nickname), "UTF-8"))

        # Receive data (Optional)
        i = 0
        irc_socket.send(bytes(f"JOIN #{i}\r\n", "UTF-8"))
        while True:
            message = irc_socket.recv(2048).decode("UTF-8")
            if(message == ""):
                break
            print(message)

    except Exception as e:
        print("Error:", e)

# Number of clients to connect
num_clients = 20

# Create and start threads for each client
threads = []
for i in range(num_clients):
    thread = threading.Thread(target=connect_to_server)
    threads.append(thread)
    time.sleep(0.1)
    thread.start()

# Wait for all threads to complete
for thread in threads:
    thread.join()
