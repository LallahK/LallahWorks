bdullah Karbanee - 21865728 - Process Management project

*Due date: 25 February 2020*

# Synchronisation - Banking
=============================================

### Input
The spec of the tutorial does not specify to do any handling. As such, I have
not implemented any safety measures for errors. The input for the program takes
the integer of the amount of threads to create as well as the corresponding
files to use as input. 

The number of threads created is equal to the number of
files that need to be inputted. Furthermore, the number of bank accounts created
corresponds to the number of threads created
The `run.sh` has the following code
```c
make clean
make
./bank_simulation "$@"

```
It first cleans the object files, then calls the make file. This will both
create the /obj file as well as populate it with the relevent object files

### Loading and Execution
The function `main` in `bank.c` begins multithreading before parsing through the
files and executing the transactions
The code can be summarised as follows:
```c
#pragma omp parallel num_threads(thread_count)
{
	
	/*Each thread parses through a file, loading the transactions*/

	/*The barrier stops all threads and waits till all threads have parsed their
	 * files*/
	#pragma omp barrier

	/*Each thread executes their transactions*/

}
```

The management of the synchronisation is done using **locks**. A lock is created
for each bank account. When a thread wishes to access a bank account it attempts
to set a lock. If succesful in setting the lock, it then executes the
transaction. If not, it will block till able to set the lock. After finishing
with the execution, the thread will unset the lock

The transaction **transfer** is a special case. In the event where 2 threads
attempt to transfer, there is a possibility of a deadlock. This is due to the
fact that in order for a transfer to take place a thread needs to lock  2
threads. The deadlock occurs when each thread attempting to transfer has set a
lock the other thread is attempting to set. This situation is circumvented by
having another lock called **trans** that is set before a thread wishes to
transfer. It thus only allows one thread to transfer at a time, avoiding a
deadlock.

### Output

The output of the transactions is outputted as per the guidelines. Blank spaces
are used to align the output correctly
