#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>
#include <stdlib.h>

void enqueue(struct queue *q, struct game_state state) {
    
    // Serialize the game state into a 64-bit integer
    uint64_t serialized_state = serialize(state);

    // block duplicate states from entering the queue
    // start at the head
    struct list_node* current = q->data.head;

    // iterate through all nodes in the queue
    while(current->next != NULL){

        // deserialize the node into a game state
        struct game_state curr = deserialize(current->value);

        // check all tiles
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                if(state.tiles[i][j] == curr.tiles[i][j]){
                    return;
                }
            }
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

        // Check if the current state matches the ideal state (goal state)
        int matches = true;
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                if(current_state.tiles[i][j] == ideal_state.tiles[i][j])
                {
                    matches = false;
                }
            }
        }
        if (matches == true)
        {
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