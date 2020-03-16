# Abdullah Karbanee - 21865728 - Process Management project

*Due date: 25 February 2020*

Executing Instructions and Managing Resources
=============================================

**Process Request**

The steps by which a process requests a resource is divided into 3 steps. This 3
step process makes use of the functions `process_request`, `acquire_resource`
and `add_resource_to_process`. 

1. The process control block, the relevant instruction and the resource list is
past to `process_request`. The resource requested, the resource list and the
process requesting the resource are past to the function `acquire_resource` in
step 2. If the request is succesful a message is displayed and with a call to 
deallocate the executed instruction. If the resource does not exist (when
`acquire_resoure` returns 2), the relevant message is displayed. Otherwise the 
next instruction in the process's instruction list is made the next active instruction:
```c
current->nextInstruction = current->nextInstruction->next;
```
Otherwise, a message is displayed that the process is waiting for a resource.

2. The resource requested, the resource list and the process requesting the
resource are past to the function `acquire_resource`. This function then parses
through the linked-list of resources to see if the requested resource is present and
available. If so the process requesting the resource along with the resource
requested are past to the function `add_resource_to_process`. Afterwhich, the
function returns a 1 for a succesful; or 0 if the request was unsuccesful

3. The process requesting the resource along with the resource requested  are
past to the function `add_resource_to_process`. The resource requested is then
added to the end of this process's resource list. This is done by instantiating
a new pointer of type resource list, which will point to the requested resource,
keeping the integrity of the full resource list, while allowing the process to
know it's resource list via pointers.

```c
struct resourceList *newRes;

newRes = malloc(sizeof(struct resourceList));
newRes->name = resource->name;
newRes->available = resource->available;
newRes->next = NULL;

```
This new resource is then added to the end of the linked-list of resources the
requesting process posseses.

**Process Release**

The steps by which a process releases a resource is divided into 3 steps. This 3
step process makes use of the functions `process_release`, `release_resource`
and `release_resource_to_process`. 

1. The process control block, the relevant instruction and the resource list is
past to `process_release`. The resource that the process is trying to release, 
the resource list and the process releasing the resource are past to the
function `release_resource` in step 2. If the resource is succesfully released
the relevant message is displayed and after a call is made to deallocate the executed
instruction from the process's instruction linked-list, the next instruction in the 
process's instruction list is made the next active instruction:

```c
current->nextInstruction = current->nextInstruction->next;
```
Otherwise, a message is displayed that the process could not release the
resource as their is nothing to release

2. The resource the process wishes to release, the resource list and the process
trying to release a resource are past to the function `release_resource`. This 
function then parses through the linked-list of resources the process has to see
if the process can be released. If so the process releases the resource and sets
the resource to being available again with a call to the function 
`add_resource_to_process`. Afterwhich, the function returns a 1 for a succesful
release; or 0 if the release was unsuccesful

3.  The process releasing the resource along with the resource to be released
are past to the function. This function then iterates through the linked-list of
resoruces the process holds and removes the resource from the process current
used resources. It then frees the empty resourceList objetc type with:
```c
free(pcbRes)
```
where *pcbRes* is the resourceList object to be removed from the linked list.

Scheduling Processes
====================

**Queues**

The ready, waiting and terminated queues were created using a linked-list of
object type *processQueue* with the following structure:
```c
struct processQueue {
  struct processControlBlock *pcb;
  struct processQueue *next;
};
```
The advantage of this is that the queue can be created and used without having
to create and copy the relevant processControlBlock objects into the queue
everytime a PCB needs to be enqueued. However, the disadvantage is the overhead
required to create and maintain the link-list. 
Furthermore, the queue was created using an empty node as the head of the
linked-list queue.
The enqueuing process for each queue thus required the allocation of memory
space for the new processQueue node and the linking of it to the end of the
queue. An example, as used for the waiting queue in the function
`process_to_readyq` is as follows:
```c
void process_to_readyq(struct processControlBlock *proc) {
  struct processQueue *pcbQ, *newQ;
  proc->processState = READY;

  newQ = malloc(sizeof(struct processQueue));
  newQ->pcb = proc;
  newQ->next = NULL;
  for (pcbQ = ready_queue(); pcbQ->next != NULL; pcbQ = pcbQ->next) {}
  pcbQ->next = newQ;
}

```
Furthermore, the process of dequeuing is done by linking the head of the queue
to the node next to the first node, if it is a NULL node then the queue will
thus be NULL (empty). The node is then returned to the caller of the function.
This is done as follows (the example is again for the ready queue):
```c
struct processControlBlock *dequeue_readyq() {

  struct processQueue *first, *queue;
  struct processControlBlock *pcb;

  first = ready_queue();
  queue = first->next;

  if (queue == NULL) {
    return NULL;
  } else {
    first->next = queue->next;
    pcb = queue->pcb;
    free(queue);
    return pcb;
  }
}
```
**Scheduling Algorithms**
One can switch between the round robin algorithm or the first-come first-serve
algorithm via the commenting out the function call `get_next_firstCome`
function within the function `schedule_processes`. Commenting out, as the
following shows, will cause the Round Robin algorithm to be used:
```c
//done = get_next_firstCome(process)
```
However uncommenting the code will cause the First-Come First-Serve algorithm to
be used instead:
```c
done = get_next_firstCome(process)
```

The function `schedule_process` works by executing the instructions within the
current process corresponding to the value within the int variable *done* (ie.
if done = 3, 3 instructions will be excecuted for the current process before 
the next process is fetched). Inititally the variable 'done' is set to the value
of QUANTUM, hence the default setting for the function `schedule_process` is to
use the Round Robin algorithm, with the value done = QUANTUM being the amount of
instructions to excecute before fetching the next process and sending the
current process to the waiting queue.

After a process has executed 'done' amount of times, the function
`get_next_roundRobin` is called to fetch the next function.

However, if the function `get_next_firstCome` is uncommented, then the scheduler
switched to FCFS. `get_next_firstcome` parses through the current function and
counts how many instructions the current function has, and returns this value.
This count, of how many instructions the current process has, then overrides the
variable 'done'. Since this function executes a process 'done' amount of times,
this causes the function to excecute all instructions within the process before
sending it ot the terminated queue, hence a FCFS algorithm is executed.

Detecting Deadlock
==================

**Detection**
Deadlocks are detected via the function `processes_deadlocked`. This function
recieves a process control block that points to all processsesas a parameter; 
the function then parses through the processes, and if all processes are WAITING
then the function returns a 1, else it returns the 0

**Recovery**
The program can recover from a deadlock via the function `deadlock_recover`.
This function works via a preempting algorithm. The first element of the waiting
queue is dequeued. All of it's resources are freed, and instructions to
reallocate resources to its process are added to the front of its instructions
linked list. This is done by allocating and creating a new object of insruction
type, and making the instruction a request of the recently released resource:
list.
```c
for (resources = pcb->resourceListPtr; resources != NULL;
      resources = resources->next) {
  resources->available = 1;
  instruct = malloc(sizeof(struct instruction));
  instruct->type = 0;
  instruct->resource = resources->name;
  instruct->next = pcb->nextInstruction;

  release_resource(resources->name, resList, pcb);
 
  pcb->nextInstruction = instruct;
  pcb->resourceListPtr = resources->next;
}
```
This recovery algorithm, however, is not called instantly when all processes are
deadlocked. The program will attempt to resolve the deadlock via normal
requesting and releasing of resources for a few QUANTUMS before calling
`deadlock_recover`. The amount of quantums, 'lock',  that pass before the deadlock is
fixed is 
*lock = (process_count x 2) + 1*
```c
if (processes_deadlocked(pcb) == 1) {
  if (lock == procC + 1) {
     deadlock_recover(resource);
     lock = 0;
  } else {
     lock++;
  }
}

```
