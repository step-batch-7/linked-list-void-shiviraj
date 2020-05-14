#include "linkedlist.h"

List_ptr create_list(void)
{
  List_ptr list = malloc(sizeof(LinkedList));
  list->first = NULL;
  list->last = NULL;
  list->length = 0;
  return list;
};

Node_ptr create_node(Element element)
{
  Node_ptr node = malloc(sizeof(Node));
  node->element = element;
  node->next = NULL;
  return node;
}

Status add_to_list(List_ptr list, Element element)
{
  Node_ptr node = create_node(element);
  if (list->first == NULL)
    list->first = node;
  list->last->next = node;
  list->last = node;
  list->length++;
  return Success;
};

Status add_to_start(List_ptr, Element);
Status insert_at(List_ptr, Element element, int position);

List_ptr reverse(List_ptr);

List_ptr map(List_ptr, Mapper);
List_ptr filter(List_ptr, Predicate);
Element reduce(List_ptr, Element, Reducer);
void forEach(List_ptr, ElementProcessor processor);

Element remove_from_start(List_ptr); // Returns Element which was removed
Element remove_from_end(List_ptr);
Element remove_at(List_ptr, int position);

Element remove_first_occurrence(List_ptr, Element element, Matcher matcher);
List_ptr remove_all_occurrences(List_ptr, Element element, Matcher matcher); // Returns List of removed elements

Status add_unique(List_ptr list, Element element, Matcher matcher);

Status clear_list(List_ptr);