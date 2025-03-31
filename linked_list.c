#include "linked_list.h"
#include <stdlib.h>

struct list_node *new_node(size_t value) { 
  struct list_node *n = malloc(sizeof(struct list_node)); 
  n->value = value;
  n->next = NULL;
  return n;
}

void insert_at_head(struct linked_list *list, size_t value) {
  // Allocate memory for the new node
  struct list_node *new_node = malloc(sizeof(struct list_node));
  
  // Initialize the new node's fields
  new_node->value = value;
  new_node->next = list->head;  // Point the new node to the current head
  
  // Update the head of the list to point to the new node
  list->head = new_node;
}

void insert_at_tail(struct linked_list *list, size_t value) {
  // Allocate memory for the new node
  struct list_node *new_node = malloc(sizeof(struct list_node));

  
  // Initialize the new node
  new_node->value = value;
  new_node->next = NULL;  // The new node will be the last node, so its next is NULL
  
  // Check if the list is empty
  if (list->head == NULL) {
    // If the list is empty, the new node becomes the head
    list->head = new_node;
  } 

  else {
    // If the list is not empty, traverse to the last node
    struct list_node *current = list->head;
    
    while (current->next != NULL) {
      current = current->next;
    }
  
    // Step 4: Link the last node to the new node
    current->next = new_node;
  }
}

size_t remove_from_head(struct linked_list *list) { 
  // Check if the list is empty
  if (list->head == NULL) {
    // Return -1 that indicates the list is empty
    return -1;
  }

  // Save the value of the node to be removed
  struct list_node *node_to_remove = list->head;
  size_t v = node_to_remove-> value;

  // Update the head to point to the next node
  list->head = node_to_remove->next;

  // Free the memory occupied by the old head node
  free(node_to_remove);

  // Return v
  return v;
}

size_t remove_from_tail(struct linked_list *list) {
  // Check if the list is empty
  if (list->head == NULL) {
    return -1;  
  }

  // If there's only one node in the list, remove it
  if (list->head->next == NULL) {
      size_t v = list->head->value;
      free(list->head);  // Free the only node
      list->head = NULL;  // Set head to NULL since the list is now empty
      return v;
  }

  // Traverse to the second-to-last node
  struct list_node *current = list->head;
  while (current->next != NULL && current->next->next != NULL) {
      current = current->next;
  }

  // Now, `current` is pointing to the second-to-last node
  struct list_node *tail_node = current->next;  // The tail node
  size_t v = tail_node->value;

  // Remove the tail by setting the second-to-last node's next to NULL
  current->next = NULL;

  // Free the memory of the tail node
  free(tail_node);

  // Return v
  return v;
}

void free_list(struct linked_list list) {
  struct list_node *current = list.head;
  struct list_node *next_node;

  while(current != NULL){
    next_node = current->next;  // Save the next node before freeing the current one
    free(current);  // Free the current node
    current = next_node;  // Move to the next node
  }
}

// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list) {
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}