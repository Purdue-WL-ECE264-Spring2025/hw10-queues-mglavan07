#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>
#include <stdlib.h>

#define TILES(num) (num & ((((uint64_t)1 << 49)-1)<<15))
void enqueue(struct queue *q, struct game_state state) {
    
    // Serialize the game state into a 64-bit integer
    uint64_t serialized_state = serialize(state);

    // block duplicate states from entering the queue
    // start at the head
    struct list_node* current = q->data.head;

    // iterate through all nodes in the queue
    while(current != NULL){

        // check all tiles
        if(TILES(serialized_state) == TILES(current->value)){
	  return;
	}

        // increment to next node
        current = current->next;
    }

    // insert at head
    insert_at_head(&q->data, serialized_state);
}

struct game_state dequeue(struct queue *q) {

    // Remove the tail node b
    size_t v = remove_from_tail(&q->data);

    // Deserialize the game state from the tail node
    struct game_state state = deserialize(v);

    // Return the deserialized game state
    return state;
}

int number_of_moves(struct game_state start) {  
    // Ideal state setup (solved state)
    struct game_state ideal_state = {
        .tiles = {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12},
            {13, 14, 15, 0}  // The empty space is represented by 0
        },
        .empty_row = 3,
        .empty_col = 3,
        .num_steps = 0
    };

    // Queue for BFS
    struct queue q = { .data = NULL };
    
    // Enqueue the start state in the BFS queue
    enqueue(&q, start);

    // While there are states in the queue
    while (q.data.head != NULL) {
    
        // Dequeue the front state
        struct game_state current_state = dequeue(&q);

        //for(int k = 0; k < 4; k++){
	 // for(int n=0; n < 4; n++){
	   // printf("%d - ", current_state.tiles[k][n]);
	 // }
	 // printf("\n");
	//} 
	//int f;
	//scanf("%d",&f);	

        // Check if the current state matches the ideal state (goal state)
        int matches = false;
        if(TILES(serialize(current_state)) == TILES(serialize(ideal_state))){
            matches = true;
        }
        if (matches == true)
        {
            free_list(q.data);
            //free(q);
            return current_state.num_steps;
        }

        // Generate all possible moves (up, down, left, right)
        struct game_state new_state;

        // Check all possible moves (up, down, left, right) and enqueue valid new states

        // Move up
        if (current_state.empty_row < 3) {  // Can move up
            new_state = current_state;
            move_up(&new_state);
            enqueue(&q, new_state);
        }

        // Move down
        if (current_state.empty_row > 0) {  // Can move down
            new_state = current_state;
            move_down(&new_state);
            enqueue(&q, new_state);
        }

        // Move left
        if (current_state.empty_col < 3) {  // Can move left
            new_state = current_state;
            move_left(&new_state);
            enqueue(&q, new_state);
        }

        // Move right
        if (current_state.empty_col > 0) {  // Can move right
            new_state = current_state;
            move_right(&new_state);
            enqueue(&q, new_state);
        }
    }
    return 0;
}
