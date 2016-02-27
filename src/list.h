//
// Created by thibaut on 27/02/16.
//

#ifndef ALPHA_SNAKE_ACTOR_LIST_H
#define ALPHA_SNAKE_ACTOR_LIST_H

typedef struct _node *Node;
typedef struct _list List;

Node cons_node(void *elt, Node node);

List *create_list();
void free_list(List *list);
void free_list_fn(List *list, void (*fn)(void *));

void list_add_first(List *list, void *elt);
void list_add_last(List *list, void *elt);
void list_foreach(List *list, void (*fn)(void *, void *), void *data);

#endif //ALPHA_SNAKE_ACTOR_LIST_H
