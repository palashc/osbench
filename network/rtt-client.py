import socket
import time
import numpy as np

# HOSTNAME="100.81.37.40"
# HOSTNAME="100.81.39.6"
HOSTNAME="localhost"
# HOSTNAME='34.203.236.226'
PORT=8001
TRIALS = 100
BYTES = 56

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
try:
    conn = s.connect((HOSTNAME, PORT))
except Exception as e:
    print(e)
    raise e

times = []
start, end = 0,0
for i in range(TRIALS):
    start = time.time()

    s.send(b'x'*BYTES)
    s.recv(BYTES)
    end = time.time()
    times.append(end-start)
    print (times[-1])

print ('Running %d trials on (%s, %d)' % (TRIALS, HOSTNAME, PORT))
print ('Passing %d bytes over a SOCK_STREAM' % BYTES)
print ('median', np.median(times))
print ('mean', np.mean(times))
print ('stdev', np.std(times))

s.close()