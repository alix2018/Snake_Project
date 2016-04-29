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
Node cons_node_next(void *elt, Node node);

void *node_elt(Node node);
void node_set_elt(Node node, void *elt);
Node node_next(Node node);
Node node_prev(Node node);
int node_egalite(Node n1, Node n2);

List *create_list();
void free_list(List *list);

void free_list_fn(List *list, void (*fn)(void *));
void list_add_first(List *list, void *elt);
void list_add_last(List *list, void *elt);
void list_add_node_first(List *list, Node node);
void list_add_node_last(List *list, Node node);
void list_delete_node(List *list, Node n);
Node list_first_node(List *list);
Node list_last_node(List *list);
int list_is_empty(List *list);

int list_size(List * list);
void *list_pop_last(List *list);
void *list_pop_first(List *list);
void *list_get_last(List *list);

void *list_get_first(List *list);
void list_foreach(List *list, void (*fn)(void *, void *), void *data);

#endif //ALPHA_SNAKE_ACTOR_LIST_H
