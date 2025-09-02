#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "queue.h"
#include "stack.h"


/*
 * Define your call struct, and call center struct here.
 */
struct call{
    int ID;
    char name[100];
    char reason[100];
    
};

struct call_center{

    //answered calls are moved to stack
    struct stack* answered;
    //recieved call are moved to queue
    struct queue* recieved;
    int ans_call;
};

//  Function name: options()
//  Parameters: NONE
//  Description:
// this function simply displays options to the user
int options(){
    int choice=0;
    do{
        printf("\n1. Receive a new call\n");
        printf("2. Answer a call\n");
        printf("3. Current state of the stack – answered calls\n");
        printf("4. Current state of the queue – calls to be answered\n");
        printf("5. Quit\n");
        printf("Choose an option: ");
    
        scanf("%d", &choice);
        while(getchar() != '\n');
        if(choice < 1 || choice > 5){
            printf("Invalid choice. Please enter a number between 1 and 5.\n");
            choice = 0;
        }
    }while (choice == 0);
    return choice;
}

//  Function name: create_call()
//  Parameters: NONE
//  Description:
//  This function creates call, set it to nothing, and return the newly created call
struct call *create_call(){
    struct call* call = malloc(sizeof(struct call)); 
    
    call->ID = 0;
    call->name[0] = '\0';
    call->reason[0] = '\0';
    
    return call;

}
//  Function name: create_call_center()
//  Parameters: NONE
//  Description:
// This function initializes call center and sets all its members to NULL.
// it is then returned to where ever called
struct call_center* create_call_center(){
    struct call_center* CC = malloc(sizeof(struct call_center));
    
    CC->answered = stack_create();
    
    CC->recieved = queue_create();
    
    CC->ans_call = 0;
    
    return CC;
}

//  option 1
//  Function name:Recieve(~~)
//  Parameters: struct call_center* CC and struct call* call
//  Description:
//  this function is the functionality behind option 1
//  when the user presses option 1, they will enter specifed information
//  this information then be enqueued to the queue member "recieved" from the struct call_center
//  
//
void recieve(struct call_center* CC, struct call* call){
    call->ID ++;
   
    printf("\nEnter caller’s name: \n");
    scanf("%s", &call->name);
    
    printf("Enter call reason: \n");
    scanf("%s", &call->reason);
    
    // add 
    queue_enqueue(CC->recieved, call);
    recieved(CC->recieved, call);
    
    return;    
}

//  option 2
//  Function name:
//  Parameters:
//  Description:
//  checks whether there is anyone in the queue if the queue is
//  empty then it will print the statement, if not then the call will be 
//  answered. The details of the answered call will be stored in the stack member
//  "answered" from call center struct.
//
void answer(struct call_center* CC){
    
    if (queue_isempty(CC->recieved) == 1){
        printf("\nNo more calls need to be answered at the moment!\n"); 

    }else{
        struct call* temp_call = queue_dequeue(CC->recieved); 
        stack_push(CC->answered, temp_call);
        
        if(stack_top(CC->answered) == temp_call){
            CC->ans_call++;
        
            printf("\nThe following call has been answered and added to the stack!\n");
            
            printf("\nCall ID: %d", temp_call->ID);
            printf("\nCaller's Name: %s",&temp_call->name);
            printf("\nCaller Reason: %s \n", &temp_call->reason);
        
        }else{
            printf("\n The following call has FAILED to be answered and added to the stack!\n");
                    
            printf("\nCall ID: %d", temp_call->ID);
            printf("\nCaller's Name: %s", &temp_call->name);
            printf("\nCaller Reason: %s \n", &temp_call->reason);
        }

    }
    return;
}

//  option 3
//  this function checks the current of status status.
//  It will print the number of calls answered and the details of 
//  the most recent answered call.
//
void curr_stack(struct call_center* CC){
//print number of calls answered
    printf("\nNumber of calls answered: %d", CC->ans_call);
    printf("\nDetails of the last call answered:\n");

//print details of most recent answer/
    struct call* temp_call = (struct call*)stack_top(CC->answered);

    printf("\nCall ID: %d", temp_call->ID);
    printf("\nCaller's Name: %s", &temp_call->name);
    printf("\nCaller Reason: %s \n", &temp_call->reason);
}

// option 4
// Function Name: void curr_queue(struct call_center* CC, struct call* call){
// Parameters: struct call_center* CC and struct call* call
// Description: 
// Just like option 3 this checks the current status of the queue, if empty 
// it print 0 callers in queue, if callers it will print the details of the 
// next callers details
void curr_queue(struct call_center* CC, struct call* call){
//if no calls in queue print 
    int que_size = size(CC->recieved);
    if (que_size == 0) {
        printf("\nNumber of calls to be answered: 0\n");
    }else{
        struct call* temp_call = (struct call*) queue_front(CC->recieved); 
        printf("\nNumber of calls to be answered: %d \n",que_size);
        
        printf("\nCall ID: %d", temp_call->ID);
        printf("\nCaller's Name: %s", &temp_call->name);
        printf("\nCaller Reason: %s \n", &temp_call->reason);
    }

// if calls print num_calls(this will be equal to size of arr)
// and print first in queue
}

// frees call
void free_call(struct call* call){
    assert(call);
    if(call != NULL){
        free(call);
    }
}

// frees call center
void free_call_center(struct call_center* CC){
    assert(CC);
    stack_free(CC->answered);
    queue_free(CC->recieved);
    free(CC);

}

// frees all
void free_all(struct call* call, struct call_center* CC){
    free_call(call);
    free_call_center(CC);
}

// runs the entirety of the program
void run(){
    struct call_center* CC = create_call_center();
    struct call* call = create_call();

    while(1){
        int choice = options();
        switch(choice){
            case 1:
                recieve(CC, call);
                break;
            case 2:
                answer(CC);
                break;
            case 3:
                curr_stack(CC);
                break;
            case 4:
                curr_queue(CC, call);
                break;
            case 5:
                printf("Bye!\n");
                free_all(call, CC);
                return;
        }
    }
}

int main(int argc, char *argv[]) {
    run();

    return 0;
}
