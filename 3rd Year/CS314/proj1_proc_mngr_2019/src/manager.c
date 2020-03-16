/**
 * @file manager.c
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "manager.h"

#define QUANTUM 1
#define TRUE 1
#define FALSE 0


void process_release(struct processControlBlock *current, struct instruction
  *instruct, struct resourceList *resource);
void process_request(struct processControlBlock *current, struct instruction
  *instruct, struct resourceList *resource);
void process_send_message(struct processControlBlock *pcb, 
    struct instruction *instruct, struct mailbox *mail);
void process_receive_message(struct processControlBlock *pcb, 
    struct instruction *instruct, struct mailbox *mail);
 
int acquire_resource(char *resourceName, struct resourceList *resources, struct
  processControlBlock *p);
int release_resource(char *resourceName, struct resourceList *resources, struct
  processControlBlock *p);

void add_resource_to_process(struct processControlBlock *current, 
  struct resourceList *resource);
void release_resource_from_process(struct processControlBlock *current, 
  struct resourceList *resource);

struct processControlBlock *dequeue_readyq();
struct processControlBlock *dequeue_waitingq();
struct processControlBlock *dequeue_terminatedq();

void process_to_readyq(struct processControlBlock *proc);
void process_to_waitingq(struct processControlBlock *proc);
void process_to_terminatedq(struct processControlBlock *proc);

struct processControlBlock *get_next_process();
struct processControlBlock *get_next_roundRobin();
int get_next_firstCome(struct processControlBlock *pcb);
void print_resources(struct resourceList *resList);

int processes_finished(struct processControlBlock *firstPCB);
int processes_deadlocked(struct processControlBlock *pcb);

void deadlock_recover(struct resourceList *resList);

/**
 * @brief Schedules each instruction of each process in a round-robin fashion.
 * The number of instruction to execute for each process is governed by the
 * QUANTUM variable.
 *
 * @param pcb The process control block which contains the current process as
 * well as a pointer to the next process control block.
 * @param resource The list of resources available to the system.
 * @param mail The list of mailboxes available to the system.
 */

void schedule_processes(struct processControlBlock *pcb, struct resourceList
*resource, struct mailbox *mail) {
  struct processControlBlock *process, *firstPCB;
  struct instruction *instr;
  int count, done = QUANTUM, lock = 0, procC = 0;
  
  for (firstPCB = process = pcb; process != NULL; process = firstPCB) {
	firstPCB = process->next;
	process_to_readyq(process);
	procC++;
  }

  for (process = get_next_roundRobin(); processes_finished(pcb) == 0;
  process = get_next_roundRobin()) {
    //done = get_next_firstCome(process);
	for (count = 0; count < done; count++) {
	  instr = process->nextInstruction;

      if (instr == NULL) {
	    break;
	  }

	  switch(instr->type) {
	    case REQ_V : process_request(process, instr, resource);
		  break;
		case REL_V : process_release(process, instr, resource);
		  break;
		case SEND_V : process_send_message(process, instr, mail);
		  break;
		case RECV_V : process_receive_message(process, instr, mail);
		  break;
	  } 
	}
    if ((instr = process->nextInstruction) == NULL) {
	  process_to_terminatedq(process);
	} else {
	  process_to_waitingq(process);
	}

    if (processes_deadlocked(pcb) == 1) {
	  if (lock == procC + 1) {
	    deadlock_recover(resource);
	    lock = 0;
	  } else {
	    lock++;
	  }
	}
  }
}

/**
 * @brief The code used to get and sift through the next process,
 * further ensuring that the process given back does not contain
 * a null next instruction. If it does it is sent to the terminated
 * queue.
 * 
 * @return the next process control block to be processed
 */
struct processControlBlock *get_next_roundRobin() {
  struct processControlBlock *process;
  struct instruction *instr;

  process = get_next_process();
  if (process != NULL) {
    instr = process->nextInstruction;
    if (instr == NULL) {
      process_to_terminatedq(process);
      while (process != NULL && instr == NULL) {
	    process = get_next_process();
	    if (process != NULL) {
	      instr = process->nextInstruction;
		  if (instr == NULL) {
		    process_to_terminatedq(process);
		  }
	    }
	  }
    }
  }
  return process;
}

/**
 * @brief Takes the next process control block to be processed and
 * returns an integer of the amount of instructions in the process.
 * This number is used to override the QUANTUM variable, ensuring 
 * that the whole process is run before the next pcb is run.
 *
 * @param pcb The next process control block
 *
 * @return Returns the amount of instructions in pcb
 */
int get_next_firstCome(struct processControlBlock *pcb) {
  struct instruction *instr;
  int i;

  for (i = 0, instr = pcb->nextInstruction; instr!= NULL; 
  instr = instr->next, i++) {}

  return i;
}

/**
 * @bried Dequeues the ready queue, and returns the process from the queue. If
 * the dequeued process is NULL, the function will pop and return a process from
 * the waiting dequeue
 *
 * @return The next process control block to process
 */
struct processControlBlock *get_next_process() {
  struct processControlBlock *process;
  
  process = dequeue_readyq();
  if (process == NULL) {
    process = dequeue_waitingq();
  }

  return process;
}

/**
 * @brief Handles the request resource instruction.
 *
 * Executes the request instruction for the process. The function loops
 * through the list of resources and acquires the resource if it is available.
 * If the resource is not available the process sits in the waiting queue and
 * tries to acquire the resource on the next cycle.
 *
 * @param current The current process for which the resource must be acquired.
 * @param instruct The instruction which requests the resource.
 * @param resource The list of resources.
 */

void process_request(struct processControlBlock *current, struct instruction
  *instruct, struct resourceList *resource) {
  current->processState = RUNNING;
  int result;

  result = acquire_resource(instruct->resource, resource, current); 
  if (result == 1) {
    current->nextInstruction = current->nextInstruction->next;
 	printf("%s req %s: acquired; ", current->pagePtr->name, instruct->resource);
	print_resources(resource);
    dealloc_instruction(instruct);
  } else if (result == 2){
    current->nextInstruction = current->nextInstruction->next;
	printf("%s req %s: no such resource;\n", current->pagePtr->name, 
	instruct->resource);
    dealloc_instruction(instruct);
  } else {
    printf("%s req %s: waiting;\n", current->pagePtr->name, instruct->resource);
  }
}


/**
 * @brief Handles the release resource instruction.
 *
 * Executes the release instruction for the process. If the resource can be
 * released the process is ready for next execution. If the resource can not
 * be released the process sits in the waiting queue.
 *
 * @param current The process which releases the resource.
 * @param instruct The instruction to release the resource.
 * @param resource The list of resources.
 */
 
void process_release(struct processControlBlock *current, struct instruction
  *instruct, struct resourceList *resource) {
  current->processState = RUNNING;

  if (release_resource(instruct->resource, resource, current) == 1) {
	current->nextInstruction = current->nextInstruction->next;
    printf("%s rel %s: released; ", current->pagePtr->name, instruct->resource);
	print_resources(resource);   
    dealloc_instruction(instruct);
  } else {
    printf("%s rel %s: ERROR: Nothing to release\n", current->pagePtr->name,
	  instruct->resource);
  }
}


/**
 * @brief Sends the message the prescribed mailbox.
 *
 * Sends the message specified in the instruction of the current process, to
 * the mailbox specified in the instruction. The function gets the pointer to 
 * the first mailbox and loops through all the mailboxes to find the one to
 * which the message must be sent.
 *
 * @param pcb The current process which instruct us to send a message.
 * @param instruct The current send instruction which contains the message.
 * @param mail The list of available mailboxes.
 */
void process_send_message(struct processControlBlock *pcb, 
    struct instruction *instruct, struct mailbox *mail) {
  
  struct mailbox *currentMbox;
  
  pcb->processState = RUNNING;

  currentMbox = mail;
  do {
    if ( strcmp(currentMbox->name, instruct->resource) == 0 ) {
      /* We found the mailbox in which a message should be left */
      break;
    }
    currentMbox = currentMbox->next;
  } while ( currentMbox != NULL );
 
  printf("%s sending message to mailbox %s which says \033[22;31m %s \033[0m\n",
      pcb->pagePtr->name, currentMbox->name, instruct->msg);

  currentMbox->msg = instruct->msg;
  pcb->nextInstruction = pcb->nextInstruction->next;
  dealloc_instruction(instruct);
}

/**
 * @brief Retrieves the message from the mailbox specified in the instruction
 * and stores it in the instruction message field.
 *
 * The function loops through the available mailboxes and finds the mailbox
 * from which the message must be retrieved. The retrieved message is stored
 * in the message field of the instruction of the process.
 *
 * @param pcb The current process which requests a message retrieval.
 * @param instruct The instruction to retrieve a message from a specific
 * mailbox.
 * @param mail The list of mailboxes.
 */
void process_receive_message(struct processControlBlock *pcb, 
    struct instruction *instruct, struct mailbox *mail) {
  
  struct mailbox *currentMbox;

  pcb->processState = RUNNING;
  
  currentMbox = mail;
  do {
    if ( strcmp(currentMbox->name, instruct->resource) == 0 ) {
      /* We found the mailbox from which a message must be read. */
      break;
    }
    currentMbox = currentMbox->next;
  } while ( currentMbox != NULL );

  printf("%s retrieved message from mailbox %s which says \033[22;32m %s \033[0m\n",
      pcb->pagePtr->name, currentMbox->name, currentMbox->msg);

  instruct->msg = currentMbox->msg;
  currentMbox->msg = NULL;
  pcb->nextInstruction = pcb->nextInstruction->next;
  dealloc_instruction(instruct);

}

/**
 * @brief Acquires the resource specified by resourceName.
 *
 * The function iterates over the list of resources trying to acquire the
 * resource specified by resourceName. If the resources is available, the
 * process acquires the resource. The resource is indicated as not available
 * in the resourceList and 1 is returned indicating that the resource has been
 * acquired successfully.
 *
 * @param resourceName The name of the resource to acquire.
 * @param resources The list of resources.
 * @param p The process which acquires the resource.
 *
 * @return 1 for TRUE if the resource is available. 0 for FALSE if the resource
 * is not available.
 */
 
int acquire_resource(char *resourceName, struct resourceList *resources, struct
  processControlBlock *p) {
  struct resourceList *resList;
  int found = 1;

  for (resList = resources; resList != NULL; resList = resList->next) {
	if (strcmp(resList->name, resourceName) == 0) {
	  if (resList->available == 1) {
	    resList->available = 0; 
		add_resource_to_process(p, resList);
		return 1;
	  }
	  found = 0;
	}
  }
  if (found == 0) {
    return 0;
  } else {
    return 2;
  }
}


/**
 * @brief Releases the resource specified by resourceName
 *
 * Iterates over the resourceList finding the resource which must be set to 
 * available again. The resource is then released from the process. 
 *
 * @param resourceName The name of the resource to release.
 * @param resources The list of available resources.
 * @param p The current process.
 *
 * @return 1 (TRUE) if the resource was released succesfully else 0 (FALSE).
 */

int release_resource(char *resourceName, struct resourceList *resources, struct
  processControlBlock *p) {
  struct resourceList *resList;

  for (resList = p->resourceListPtr; resList != NULL; resList = resList->next) {
    if (strcmp(resList->name, resourceName) == 0) {
	  release_resource_from_process(p, resList);

      for (resList = resources; resList != NULL; resList = resList->next) {
	    if (strcmp(resList->name, resourceName) == 0 &&
		resList->available == 0) {
		  resList->available = 1;
		  break;
		}
	  }
	  return 1;
	}
  }
  
  return 0;
}


/**
 * @brief Adds the specified resource to the process acquired resource list.
 *
 * After the resource has succesfully been required by the process. This
 * function is called and adds the resource to the list of resources currently
 * held by this process.
 *
 * @param current The process to which the resource must be added.
 * @param resource The resource to add to the process.
 */
 
void add_resource_to_process(struct processControlBlock *current, struct
    resourceList *resource) {
  struct resourceList *pcbRes, *newRes;

  newRes = malloc(sizeof(struct resourceList));
  newRes->name = resource->name;
  newRes->available = resource->available;
  newRes->next = NULL;

  if (current->resourceListPtr == NULL) {
    current->resourceListPtr = newRes;
  } else {
    pcbRes = current->resourceListPtr;
    while (pcbRes->next != NULL) {
	  pcbRes = pcbRes->next;
	}
    pcbRes->next = newRes;
  }

}

/**
 * @brief Release the specified resource from the process acquired list.
 *
 * The function releases the specified resource from the current process 
 * acquired list.
 *
 * @param current The current process from which the resource must be released.
 * @param resource The resource to release.
 */
 
void release_resource_from_process(struct processControlBlock *current, struct
    resourceList *resource) {
  char *res;
  struct resourceList *pcbRes, *pcbOld = NULL;

  res = resource->name;
  for (pcbRes = current->resourceListPtr; pcbRes != NULL;
        pcbOld = pcbRes, pcbRes = pcbRes->next) {
    if (strcmp(pcbRes->name, res) == 0) {
	  if (pcbOld == NULL) {
	    current->resourceListPtr = pcbRes->next;
	  } else {
	    pcbOld->next = pcbRes->next;
	  }
	  free(pcbRes);
	  return;
	}
  }
}

/**
 * @brief Add process (with id proc) to readyQueue 
 *
 * @param proc The process which must be set to ready.
 */
void process_to_readyq(struct processControlBlock *proc) {

  struct processQueue *pcbQ, *newQ;

  proc->processState = READY;

  newQ = malloc(sizeof(struct processQueue));
  newQ->pcb = proc;
  newQ->next = NULL;

  for (pcbQ = ready_queue(); pcbQ->next != NULL; pcbQ = pcbQ->next) {}
  pcbQ->next = newQ;

}

/**
 * @brief Add process (with id proc) to the waitingQueue 
 *
 * @param proc The process which must be set to waiting.
 */
void process_to_waitingq(struct processControlBlock *proc) {
  struct processQueue *pcbQ, *newQ;

  proc->processState = WAITING;
  newQ = malloc(sizeof(struct processQueue));
  newQ->pcb = proc;
  newQ->next = NULL;

  for (pcbQ = waiting_queue(); pcbQ->next != NULL; pcbQ = pcbQ->next) {}
  pcbQ->next = newQ;

}

/**
 * @brief Add process (with id proc) to the terminatedQueue 
 *
 * @param proc The process which must be set to waiting.
 */
void process_to_terminatedq(struct processControlBlock *proc) {
  struct processQueue *pcbQ, *newQ;
 
  proc->processState = TERMINATED;
  newQ = malloc(sizeof(struct processQueue));
  newQ->pcb = proc;
  newQ->next = NULL;
		 
  for (pcbQ = terminated_queue(); pcbQ->next != NULL; pcbQ = pcbQ->next) {}
  pcbQ->next = newQ;
    
  printf("%s terminated\n", proc->pagePtr->name);
}
/**
 * @brief dequeus a process block from the ready queue
 * @return the popped pcb from the queue
 */
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

/**
 * @brief dequeus a process block from the ready queue
 * @return the popped pcb from the queue
 */
struct processControlBlock *dequeue_waitingq() {

  struct processQueue *first, *queue;
  struct processControlBlock *pcb;

  first = waiting_queue();
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

/**
 * @brief dequeus a process block from the ready queue
 * @return the popped pcb from the queue
 */
struct processControlBlock *dequeue_terminatedq() {

  struct processQueue *first, *queue;
  struct processControlBlock *pcb;

  first = terminated_queue();
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


/**
 * @brief Iterates over each of the loaded processes and checks if it has been
 * terminated.
 *
 * Iterates over the processes to determine if they have terminated.
 *
 * @param termQ A pointer to start of all the processes.
 * @param processes A count of the number of processes.
 *
 * @return 1 (TRUE) if all the processes are terminated else 0 (FALSE).
 */

int processes_finished(struct processControlBlock *pcbList) {
  struct processControlBlock *pcb;
  int done = 1;

  for (pcb = pcbList; pcb != NULL; pcb = pcb->next) {
    if (pcb->processState != TERMINATED) {
	  done = 0;
	}
  }
  return done;
}

/**
 * @brief Detects deadlock. 
 *
 * This function implements a deadlock detection algorithm.
 *
 * @param firstPCB A pointer to the start of all the processes.
 *
 * @return 1 (TRUE) if all the processes are in the waiting state else
 * 0 (FALSE).
 */

int processes_deadlocked(struct processControlBlock *pcbList) {
  struct processControlBlock *pcb;
  int locked = 1;

  for (pcb = pcbList; pcb != NULL; pcb = pcb->next) {
    if (pcb->processState != WAITING) {
	  locked = 0;
	}
  }

  return locked;
}

/**
 * @brief Recovers from a deadlock using a rollback method. The resources
 * of the leading pcb in the queue are released and the instructions to 
 * reallocate resources are added to the process, and the process is added
 * to the back of the queue
 *
 * @param resList The list of resources to be parsed through, to be made
 *     available again
 */
void deadlock_recover(struct resourceList *resList) {
  struct processControlBlock *pcb;
  struct resourceList *resources;
  struct instruction *instruct;

  pcb = dequeue_waitingq();
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
  printf("%s rolled back; ", pcb->pagePtr->name);
  print_resources(resList);
  process_to_waitingq(pcb);
}
					  
/**
 * @brief Prints into terminal the resources that are still available
 *
 * @param resList The list of resources to be parsed through
 */
void print_resources(struct resourceList *resList) {
  struct resourceList *resources;

  printf("Available");
  for (resources = resList; resources != NULL; resources = resources->next) {
    if (resources->available == 1) {
  	  printf(" %s", resources->name);
	}
  }
  printf("\n");
}

