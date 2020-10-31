/**
 * @author ECE 3057 TAs
 */
#include <stdio.h>
#include <stdlib.h>
#include "lrustack.h"
/**
 * This file contains some starter code to get you started on your LRU implementation.
 * You are free to implement it however you see fit. You can design it to emulate how this
 * would be implemented in hardware, or design a purely software stack.
 *
 * We have broken down the LRU stack's
 * job/interface into two parts:
 *  - get LRU: gets the current index of the LRU block
 *  - set MRU: sets a certain block's index as the MRU.
 * If you implement it using these suggestions, you will be able to test your LRU implementation
 * using lrustacktest.c
 *
 * NOTES:
 *      - You are not required to use this LRU interface. Feel free to design it from scratch if
 *      that is better for you.
 *      - This will not behave like your traditional LIFO stack
 */

/**
 * Function to initialize an LRU stack for a cache set with a given <size>. This function
 * creates the LRU stack.
 *
 * @param size is the size of the LRU stack to initialize.
 * @return the dynamically allocated stack.
 */
lru_stack_t* init_lru_stack(int size) {
    //  Use malloc to dynamically allocate a lru_stack_t
	lru_stack_t* stack = (lru_stack_t*)malloc(sizeof(lru_stack_t));
    //  Set the stack size the caller passed in
		stack->size = size;
    ////////////////////////////////////////////////////////////////////
    //  TODO: Write any other code needed to initialize your LRU Stack.
    //  Essentially write any initializations needed for anything you
    //  added to lru_stack_t.
    ////////////////////////////////////////////////////////////////////
		stack->queue = (int*)malloc(stack->size * sizeof(int));
		stack->mru = UNDEFINED;
		stack->lru = UNDEFINED;
    ////////////////////////////////////////////////////////////////////
    //  End of your code
    ////////////////////////////////////////////////////////////////////

	return stack;
}

/**
 * Function to get the index of the least recently used cache block, as indicated by <stack>.
 * This operation should not change/mutate your LRU stack.
 *
 * @param stack is the stack to run the operation on.
 * @return the index of the LRU cache block.
 */
int lru_stack_get_lru(lru_stack_t* stack) {
    ////////////////////////////////////////////////////////////////////
    //  TODO: Write code to get the index of the LRU block from the LRU
    //  Stack.
    ////////////////////////////////////////////////////////////////////
		int n = stack->size;

		//  error handler
		if(!stack) return -1;
		if(n <= 1) return n == 1 ? stack->queue[0] : -1;

		/*
			return lru based on stored mru index value
		*/
    return stack->queue[stack->lru];
    ////////////////////////////////////////////////////////////////////
    //  End of your code
    ////////////////////////////////////////////////////////////////////
}

/**
 * Function to mark the cache block with index <n> as MRU in <stack>. This operation should
 * change/mutate the LRU stack.
 *
 * @param stack is the stack to run the operation on.
 * @param n the index to promote to MRU.
 */
void lru_stack_set_mru(lru_stack_t* stack, int n) {
	////////////////////////////////////////////////////////////////////
    //  TODO: Write code to set the passed in block index  as the MRU
    //  element in the LRU Stack.
    ////////////////////////////////////////////////////////////////////

		if(!stack) return ;

		if(stack->mru == UNDEFINED) {
			stack->queue[stack->lru] = n;
			stack->mru++;
		}
		else if(stack->mru == stack->size) {
			stack->lru++;
			stack->size++;
			stack->queue[stack->mru] = n;
			stack->mru++;
		}
		else {
			stack->queue[stack->mru] = n;
			stack-> mru++;
		}
    ////////////////////////////////////////////////////////////////////
    //  End of your code
    ////////////////////////////////////////////////////////////////////
}

bool isFull(lru_stack_t* stack) {
	if(!stack) return false;
	if(stack->mru == stack->size) return true;
	return false;
}

bool find(lru_stack_t* stack, int n) {
	if(!stack) return false;
	for(int i = getLRU(stack); i <= getMRU(stack); i++) {
		if(stack->queue[i] == n) return true;
	}
	return false;
}
/*
int getMRU(lru_stack_t* stack) {
	return stack->mru;
}

int getLRU(lru_stack_t* stack) {
	return stack->lru;
}
*/
/**
 * Function to free up any memory you dynamically allocated for <stack>
 *
 * @param stack the stack to free
 */
void lru_stack_cleanup(lru_stack_t* stack) {
    ////////////////////////////////////////////////////////////////////
    //  TODO: Write any code if you need to do additional heap allocation
    //  cleanup
    ////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////
    //  End of your code
    ////////////////////////////////////////////////////////////////////
		free(stack->queue);
    free(stack);      // Free the stack struct we malloc'd
}
