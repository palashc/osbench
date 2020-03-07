import socket
import time
import numpy as np

HOSTNAME='34.203.236.226'
# HOSTNAME='localhost'
PORT=8001
TRIALS = 30

def printStats(times, desc):
    print ('Running %d trials on (%s, %d)' % (TRIALS, HOSTNAME, PORT))
    print (desc)
    print ('median', np.median(times))
    print ('max', np.max(times))
    print ('min', np.min(times))
    print ('mean', np.mean(times))
    print ('stdev', np.std(times))


contimes = []
closetimes = []
for i in range(TRIALS):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    start = time.time()
    s.connect((HOSTNAME, PORT))
    end = time.time()
    contimes.append((end-start)*10000)

    start = time.time()
    s.close()
    end = time.time()
    closetimes.append((end-start)*10000)

    print(i)

printStats(contimes, 'Measuring connection time SOCK_STREAM')
printStats(closetimes, 'Measuring closing socket time SOCK_STREAM')

