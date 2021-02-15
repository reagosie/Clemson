README.txt

Compiling Instructions/Assumptions:
-to compile use the command: mpicc <filename> -o <executablename>
-to run the sequential program: ./<executablename>
-to run the parallel program: mpirun -np [numberofprocesses] --mca mpi_cuda_support 0 <executablename>
-We are assuming there will be no arguments passed into either program
-We are assuming that the 2D array will remain at size 1000 x 1000 (as stated on lines 4/5 of both .c files)

Contributions from team members:
-Reagan began the project first, fleshing out a basic skeleton of the program based on the assignment PDF. Then Ethan jumped in
  and helped him form a fully functioning sequential program on day 1 of teamwork. We were very happy with this progress
-Then we spent the rest of our time leading up to the deadline working on parallelizing our sequential program. This proved to
  be much more difficult than we expected (and much more difficult than Assignment 1, to be honest).
-Both team members worked a fairly even share on their own and in collaboration via Discord.
-Reagan attended office hours twice (once with Liwei and once with Dr. Jin) and both members emailed Dr. Jin and Liwei several
  times. Yet, we still could not quite complete the parallelized program by the deadline.
-Essentially, the only thing our parallelized program is missing is a final Gather (which Dr. Jin suggested using MPI_Send and
  MPI_Recv for) to get all the data on one process to pass to our PrintGridtoFile function. Because we could not achieve this,
  our parallelized program does not produce a .pnm file. However, I believe that our program is at least 90% correct and designed
  well, especially considering the amount of input we had from Dr. Jin.
