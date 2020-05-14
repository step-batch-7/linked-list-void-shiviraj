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

Status is_valid_position(int list_length, int position)
{
  return position >= 0 && list_length >= position;
}

NodePair_ptr prev_current_node_pair(List_ptr list, int index)
{
  NodePair_ptr node_pair = malloc(sizeof(NodePair));
  node_pair->current = list->first;
  for (int i = 0; i < index; i++)
  {
    node_pair->prev = node_pair->current;
    node_pair->current = node_pair->prev->next;
  }
  return node_pair;
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

Status add_to_start(List_ptr list, Element element)
{
  Node_ptr node = create_node(element);
  node->next = list->first;
  list->first = node;
  list->length++;
  if (list->last == NULL)
    list->last = node;
  return Success;
};

void insert_into_list(List_ptr list, Node_ptr node, NodePair_ptr node_pair)
{
  node_pair->prev->next = node;
  node->next = node_pair->current;
  list->length++;
}

Status insert_at(List_ptr list, Element element, int position)
{
  Status is_valid = is_valid_position(list->length, position);
  if (!is_valid)
    return Failure;
  if (position == 0)
    return add_to_start(list, element);
  if (position == list->length)
    return add_to_list(list, element);
  Node_ptr node = create_node(element);
  NodePair_ptr node_pair = prev_current_node_pair(list, position);
  insert_into_list(list, node, node_pair);
  free(node_pair);
  return Success;
};

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