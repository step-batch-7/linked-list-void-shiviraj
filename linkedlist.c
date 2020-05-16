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
  node_pair->prev = NULL;
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
  else
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

List_ptr reverse(List_ptr list)
{
  list->last = list->first;
  Node_ptr prev = NULL;
  Node_ptr current = list->first;
  Node_ptr next = NULL;
  while (current != NULL)
  {
    next = current->next;
    current->next = prev;
    prev = current;
    current = next;
  }
  list->first = prev;
  return list;
};

List_ptr map(List_ptr list, Mapper mapper)
{
  List_ptr mapped_list = create_list();
  Node_ptr node = list->first;
  for (int i = 0; i < list->length; i++)
  {
    Element mapped_element = mapper(node->element);
    add_to_list(mapped_list, mapped_element);
    node = node->next;
  }
  return mapped_list;
};
List_ptr filter(List_ptr list, Predicate predicate)
{
  List_ptr filter_list = create_list();
  Node_ptr node = list->first;
  for (int i = 0; i < list->length; i++)
  {
    if (predicate(node->element))
      add_to_list(filter_list, node->element);
    node = node->next;
  }
  return filter_list;
};

Element reduce(List_ptr list, Element element, Reducer reducer)
{
  Node_ptr node = list->first;
  for (int i = 0; i < list->length; i++)
  {
    element = reducer(element, node->element);
    node = node->next;
  }
  return element;
};

void forEach(List_ptr list, ElementProcessor processor)
{
  Node_ptr node = list->first;
  for (int i = 0; i < list->length; i++)
  {
    processor(node->element);
    node = node->next;
  }
};

Element remove_from_start(List_ptr list)
{
  if (list->length == 0)
    return NULL;
  if (list->length == 1)
    list->last = NULL;
  Node_ptr node = list->first;
  list->first = node->next;
  list->length--;
  Element removed_element = node->element;
  free(node);
  return removed_element;
};

Element remove_from_end(List_ptr list)
{
  if (list->first == NULL)
    return NULL;
  if (list->first->next == NULL)
    return remove_from_start(list);
  Node_ptr second_last = list->first;
  while (second_last->next->next != NULL)
    second_last = second_last->next;
  Element element = second_last->next->element;
  free(second_last->next);
  second_last->next = NULL;
  list->last = second_last;
  list->length--;
  return element;
};

Element remove_at(List_ptr list, int position)
{
  if (!is_valid_position(list->length - 1, position))
    return NULL;
  if (position == 0)
    return remove_from_start(list);
  if (position == list->length - 1)
    return remove_from_end(list);
  NodePair_ptr node_pair = prev_current_node_pair(list, position);
  node_pair->prev->next = node_pair->current->next;
  list->length--;
  Element removed_element = node_pair->current->element;
  free(node_pair->current);
  free(node_pair);
  return removed_element;
};

Status is_include_element(List_ptr list, Element element, Matcher matcher)
{
  Node_ptr node = list->first;
  for (int i = 0; i < list->length; i++)
  {
    Status is_included = matcher(node->element, element);
    if (is_included)
      return Success;
    node = node->next;
  }
  return Failure;
}

Element remove_first_occurrence(List_ptr list, Element element, Matcher matcher)
{
  Node_ptr prev = NULL;
  Node_ptr current = list->first;
  while (current != NULL && !(*matcher)(element, current->element))
  {
    prev = current;
    current = current->next;
  }
  if (current == NULL)
    return NULL;
  if (current == list->first)
    return remove_from_start(list);
  if (current == list->last)
    return remove_from_end(list);
  Element removed_element = current->element;
  prev = current->next;
  free(current);
  list->length--;
  return removed_element;
};

List_ptr remove_all_occurrences(List_ptr list, Element element, Matcher matcher)
{
  if (list->first == NULL)
    return NULL;
  List_ptr removed_list = create_list();
  Element removed_element = remove_first_occurrence(list, element, matcher);
  while (removed_element != NULL)
  {
    add_to_list(removed_list, removed_element);
    removed_element = remove_first_occurrence(list, element, matcher);
  }
  return removed_list;
};

Status add_unique(List_ptr list, Element element, Matcher matcher)
{
  Status is_element_in_list = is_include_element(list, element, matcher);
  if (is_element_in_list)
    return Failure;
  return add_to_list(list, element);
};

Status clear_list(List_ptr list)
{
  Node_ptr node_to_remove = list->first;
  while (node_to_remove != NULL)
  {
    remove_from_start(list);
    node_to_remove = list->first;
  }
  return Success;
};