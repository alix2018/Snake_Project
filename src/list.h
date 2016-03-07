//
// Created by thibaut on 27/02/16.
//

#ifndef ALPHA_SNAKE_ACTOR_LIST_H
#define ALPHA_SNAKE_ACTOR_LIST_H

typedef struct _node *Node;

/**
 * Une liste doublement chainée avec pointeur sur le dernier élément.
 *
 * *Attention :* Cette liste ne peut contenir que des pointeurs.
 */
typedef struct _list List;

Node cons_node(void *elt, Node node);
void *node_elt(Node node);
Node node_next(Node node);
Node node_prev(Node node);

List *create_list();
void free_list(List *list);
void free_list_fn(List *list, void (*fn)(void *));

void list_add_first(List *list, void *elt);
void list_add_last(List *list, void *elt);
Node list_first_node(List *list);
Node list_last_node(List *list);
int list_is_empty(List *list);

void *list_pop_last(List *list);
void *list_pop_first(List *list);

void list_foreach(List *list, void (*fn)(void *, void *), void *data);

#endif //ALPHA_SNAKE_ACTOR_LIST_H
