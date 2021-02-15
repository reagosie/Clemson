#!usr/bin/env python
from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD; rank = comm.Get_rank(); size = comm.Get_size()
N = 64
i = j = counter = local_max = local_min = tmp = 0
rawNum = np.zeros(N, dtype='int')
sortNum = np.zeros(N, dtype='int')
#local_bucket = np.zeros(0, dtype='int')
proc_count = np.zeros(size, dtype='int')
disp = np.zeros(size, dtype='int')

if rank == 0:
    for i in range(0,N):
      rawNum = np.random.randint(0,N,N)

comm.Bcast([rawNum, MPI.INT], root=0)

counter = 0
local_min = int(rank * (N/size))
local_max = int((rank + 1) * (N/size))
for i in range(0,N):
    if ((rawNum[i] >= local_min) and (rawNum[i] < local_max)):
        counter += 1

print('For rank', rank,', max is', local_max,', min is', local_min,', and there are', counter, 'elements in rawNum that falls within max and min')

counter = 0
for i in range(0,N):
    if ((rawNum[i] >= local_min) and (rawNum[i] < local_max)):
        #np.append(local_bucket, rawNum[i])
        counter += 1

local_bucket = np.zeros(counter, dtype='int')
counter = 0
for i in range(0,N):
    if ((rawNum[i] >= local_min) and (rawNum[i] < local_max)):
        local_bucket[counter] = rawNum[i]
        counter += 1

for i in range(0,counter):
  for j in range(i+1,counter):
      if local_bucket[i] > local_bucket[j]:
          tmp = local_bucket[i]
          local_bucket[i] = local_bucket[j]
          local_bucket[j] = tmp

for i in range(0,counter):
    print(rank, local_bucket[i])

sendbuf = np.array([counter], dtype="int")
comm.Gather(sendbuf, proc_count, root=0)

if rank == 0:
    disp[0] = 0
    for i in range(0,size-1):
        disp[i+1] = disp[i] + proc_count[i]

comm.Gatherv([local_bucket, MPI.INT], [sortNum, tuple(proc_count), tuple(disp), MPI.DOUBLE])

if rank == 0:
    print('Before sort:')
    for i in range(0,N):
        print(rawNum[i], end =" ")
    print('\nAfter sort:')
    for i in range(0,N):
        print(sortNum[i], end =" ")
    print('\n')
