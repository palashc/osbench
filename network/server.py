import socket

HOST = socket.gethostbyname('localhost')  # Standard loopback interface address (localhost)
PORT = 8001             # Port to listen on (non-privileged ports are > 1023)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print ('starting server')
s.bind((HOST, PORT))
s.listen()

while True:
    conn, addr = s.accept()
    with conn:
        print('Connected by', addr)
        while True:
            data = conn.recv(150*1024*1024)
            if not data:
                print ('breaking')
                break
            conn.sendall(data)
    conn.close()
s.close()