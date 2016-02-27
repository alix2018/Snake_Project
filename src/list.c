//
// Created by thibaut on 27/02/16.
//

#include <clutter/clutter.h>
#include <stdlib.h>
#include "list.h"

/**
 * @brief    noeud d'une liste
 */
struct _node
{
    void *elt; /*la coordonnée dans la liste*/
    Node next; /*la suite de la liste*/
    Node prev; /*la fin de la liste*/
};

/**
 * @brief   liste d'éléments
 */
struct _list
{
    Node first;
    Node last;
};


/**
 * @brief   Alloue la mémoire nécessaire à un nœud de List.
 *
 * @param[in]       elt   L'élément à ajouter.
 * @param[in,out]   node  Le successeur.
 *
 * @return  Le nœud créé.
 */
Node cons_node(void *elt, Node node)
{
    Node res;
    res = malloc(sizeof(struct _node));
    res->elt = elt;
    res->next = node;
    res->prev = NULL;

    if (node != NULL)
        node->prev = res;

    return res;
}


/**
 * @brief   Crée une liste vide.
 *
 * @return  La liste créée.
 */
List *create_list()
{
    List *res;

    res = malloc(sizeof(struct _list));
    res->first = NULL;
    res->last = NULL;

    return res;
}


/**
 * @brief   Libère la mémoire occupée par une liste.
 *
 * *Attention :* Les éléments de la liste ne sont pas libérés. On peut pour cela
 * utiliser la fonction free_list_fn
 *
 * @param[in]   list    La liste à supprimer.
 */
void free_list(List *list)
{
    Node cur = list->first;
    Node tmp;

    while (cur != NULL)
    {
        tmp = cur->next;
        free(cur);
        cur = tmp;
    }

    free(list);
}


/**
 * @brief   Applique la fonction `fn` à tous les éléments d'une liste, puis la
 *          supprime.
 *
 * @param[in]    list   La liste à supprimer.
 */
void free_list_fn(List *list, void (*fn)(void *))
{
    Node cur;

    for (cur = list->first; cur != NULL; cur = cur->next)
        fn(cur->elt);

    free_list(list);
}


/**
 * @brief   Ajoute un élément à la fin d'une liste.
 *
 * @param[in]    list   La liste à laquelle l'élément sera ajouté.
 * @param[in]    elt    L'élément à ajouter.
 */
void list_add_first(List *list, void *elt)
{
    if (list->first != NULL)
    {
        list->first = cons_node(elt, list->first);
    }
    else
    {
        list->first = cons_node(elt, NULL);
        list->last = list->first;
    }
}


/**
 * @brief   Ajoute un élément au début d'une liste.
 *
 * @param[in]    list   La liste à laquelle l'élément sera ajouté.
 * @param[in]    elt    L'élément à ajouter.
 */
void list_add_last(List *list, void *elt)
{
    if (list->last != NULL)
    {
        list->last->next = cons_node(elt, NULL);
        list->last->next->prev = list->last;
        list->last = list->last->next;
    }
    else
    {
        list->last = cons_node(elt, NULL);
        list->first = list->last;
    }
}


/**
 * @brief   Applique la fonction `fn` en lui passant le paramètre `data`
 *          à une liste.
 *
 * @param[in]    list   Une liste.
 * @param[in]    fn     La fonction à appliquer à chaque élément.
 * @param[in]    data   Un paramètre passé à la fonction `fn`.
 */
void list_foreach(List *list, void (*fn)(void *, void *), void *data)
{
    Node cur;

    for (cur = list->first; cur != NULL; cur = cur->next)
        fn(cur->elt, data);
}
