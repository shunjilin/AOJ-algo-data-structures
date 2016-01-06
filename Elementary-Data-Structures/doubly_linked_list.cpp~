/**
 Doubly Linked List
 
 Should have the operations:
 > insert x: insert an element with key x into the front of the list
 > delete x: delete the first element which has the key of x from the list
 > deleteFirst: delete the first element from the list
 > deleteLast: delete the last element from the list
 
 Shunji Lin
 **/

#include <cstdio>
#include <cstring>

struct node {
    unsigned int element;
    node *next;
    node *prev;
};

struct list {
    int count;
    node *first;
    node *last;
};

// initialize list
list *list_init(void) {
    list *new_list = new list;
    new_list->count = 0;
    new_list->first = nullptr;
    return new_list;
}

//destroy list
void list_destroy(list *my_list) {
    node *nodePtr = my_list->first;
    while (nodePtr != nullptr) {
      node *temp = nodePtr;
      nodePtr = nodePtr->next;
      delete temp;
    }
    delete my_list;
}
  

// insert element at head of list
void list_insert(list &my_list, unsigned int element) {
    node *new_node = new node;
    new_node->element = element;
    new_node->prev = nullptr;
    
    if (my_list.count == 0) {
        new_node->next = nullptr;
        my_list.first = new_node;
        my_list.last = new_node;
    } else {
        new_node->next = my_list.first; // forward link
        my_list.first->prev = new_node; // backward link
        my_list.first = new_node;
    }
    my_list.count++;
}

// for list with only one element
void list_delete_single(list &my_list) {
    node *nodePtr = my_list.first;
    my_list.first = nullptr;
    my_list.last = nullptr;
    my_list.count--;
    delete nodePtr;
}

// delete first element from list
void list_delete_first(list &my_list) {
    if (my_list.count == 1) {
        list_delete_single(my_list);
    } else {
        node *nodePtr = my_list.first; // temporary node pointer
        my_list.first = nodePtr->next; // change head of list
        my_list.first->prev = nullptr; //remove backward link
        delete nodePtr;
        my_list.count--;
    }
}

// delete last element from list
void list_delete_last(list &my_list) {
    if (my_list.count == 1) {
        list_delete_single(my_list);
    } else {
        node *nodePtr = my_list.last;
        my_list.last = nodePtr->prev; // change tail of list
        my_list.last->next = nullptr; // remove forward link
        delete nodePtr;
        my_list.count--;
    }
}

// deleting the first instance of element in list
void list_delete(list &my_list, int element) {
    
    for (node *nodePtr = my_list.first; nodePtr != nullptr; nodePtr = nodePtr->next) {
        if (nodePtr->element == element) {
            if (nodePtr->prev == nullptr) { //if at head of list
                list_delete_first(my_list);
            } else if (nodePtr->next == nullptr) { //if at tail of list
                list_delete_last(my_list);
            } else { // somewhere within list
                nodePtr->prev->next = nodePtr->next;
                nodePtr->next->prev = nodePtr->prev; //"detach" from list
                delete nodePtr;
            }
            break;
        }
    }
}

void list_print(list &my_list) {
    for (node *nodePtr = my_list.first; nodePtr != nullptr; nodePtr = nodePtr->next) {
        printf("%u", nodePtr->element);
        if (nodePtr->next == nullptr) { // if end of input
            printf("\n"); // print newline
        } else {
            printf(" "); //put a space
        }
    }
}


int main()
{
    int n;
    scanf("%d", &n); // number of operations
    
    list *my_list = list_init();
    
    for (int i = 0; i < n; ++i) {
        char com[20];
        int key;
        scanf("%s%d", com, &key);
        if ( com[0] == 'i' ) { //insert element
            list_insert(*my_list, key);
        } else if ( com[0] == 'd' ) { // delete
            if (strlen(com) > 6){
	      if ( com[6] == 'F' ) { // deleteFirst
		list_delete_first(*my_list);
	      } else if ( com[6] == 'L' ) { // deleteLast
		list_delete_last(*my_list);
	      }
	    } else {
	      list_delete(*my_list, key); // delete element
	    } 
        }
    }
    
    list_print(*my_list);

    list_destroy(my_list);
    my_list = nullptr;
    
}
