#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* link; // Zeiger auf das nachste Element 
};

struct Node* newNode (int data, struct Node* ptr);
void printList (struct Node* head);
struct Node* constructList(int keys[], int n);

int main() {
    printf("Hello World!\n");
    int keys[] = {1, 2, 3, 4};
    int n = sizeof keys / sizeof(int);

    struct Node* head = constructList(keys, n);

    printList(head);
    return 0;
}

struct Node* newNode (int data, struct Node* ptr) {
    struct Node* node = malloc (sizeof (struct Node));
    // der Cast (struct Node*) ist in C nicht notwendig, aber "sauber" (notwendig in Cpp)
    if (node == NULL) exit;
    node->data = data;
    node->link = ptr;

    return node;
}

void printList (struct Node* head) {
    struct Node* ptr = head;
    while(ptr) {
        printf("%d -> ", ptr->data);
        ptr = ptr->link;
    }
    printf("NULL\n");
}

struct Node* constructList(int keys[], int n) {
    // struct Node* nodeptr = &keys[n-1];
    struct Node* head = NULL, *nodeptr = NULL;

    // wir beginnen am Ende der Liste
    for (int i = n-1; i >= 0; i--) {
        nodeptr = newNode(keys[i], nodeptr);
        head = nodeptr;
    }
    return nodeptr;
}