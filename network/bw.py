import socket
import time
import numpy as np

# HOSTNAME="100.81.37.40"
# HOSTNAME="100.81.39.6"
HOSTNAME="localhost"
# HOSTNAME='34.203.236.226'
PORT=8001
TRIALS = 1000
BYTES = 1024 * 1024;

largeFile = b'x' * BYTES

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
    s.sendall(largeFile)
    end = time.time()
    times.append(8*BYTES/(end-start))
    print (i, times[-1])

def printStats(times):
    print ('Running %d trials on (%s, %d)' % (TRIALS, HOSTNAME, PORT))
    print ('Passing %d bytes over a SOCK_STREAM' % BYTES)
    print ('median', np.median(times))
    print ('max', np.max(times))
    print ('min', np.min(times))
    print ('mean', np.mean(times))
    print ('stdev', np.std(times))

printStats(times)
s.close()