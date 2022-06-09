// BT20CSE032_DWEEJAREDDY

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 50

typedef struct book_node
{
    int book_id;                    // book ID
    char title[MAX_SIZE];           // book title
    char author[MAX_SIZE];          // book author
    char subject[MAX_SIZE];         // book subject
    int num_copies_issued;          // number of copies issued
    int num_copies_available;       // number of copies available
    int demand;                     // number of copies demanded (for Q2)
    struct book_node *left;         // left child
    struct book_node *right;        // right child
    int height;                     // height of the node
} book_node;  

typedef struct request_queue        // queue for requests
{
    char student_name[MAX_SIZE];
    char title[MAX_SIZE];
    struct request_queue *next;
} request_queue;

typedef struct borrower_list        // list of borrowers
{
    char student_name[MAX_SIZE];
    char title[MAX_SIZE];
    int return_date;
    struct borrower_list *next;
} borrower_list;

typedef struct defaulters           // list of defaulters
{
    char student_name[MAX_SIZE];
    struct defaulters *next;
} defaulters;

typedef struct Node                 
{
    int data;
    char name[MAX_SIZE];
    char name2[MAX_SIZE];
    struct Node *next;
}Node;

book_node *bptr = NULL;   // Initializing all the pointers to NULL
defaulters *fptr = NULL;
borrower_list *brptr = NULL;
request_queue *rptr = NULL;
Node *num_list = NULL;

int count = 0;                  // counter for Q3

int most_demand;    // Variable to store the most demand book

// Used to initialise library with books 

int max(int a, int b)       // returns the max of two integers
{
    if (a > b)
        return a;
    else
        return b;
}

int height_of_node(book_node *Node)         // returns the height of the node
{
    if (Node == NULL)
    {
        return 0;
    }
    return Node->height;
}

// AVL TREE

book_node *rotateLeft(book_node *x)     // Rotate left
{
    book_node *y = x->right;
    book_node *t = y->left;

    y->left = x;
    x->right = t;

    x->height = max(height_of_node(x->left), height_of_node(x->right)) + 1;
    y->height = max(height_of_node(y->left), height_of_node(y->right)) + 1;

    return y;
}

book_node *rotateRight(book_node *y)    // Rotate right    
{
    book_node *x = y->left;
    book_node *t = x->right;

    x->right = y;
    y->left = t;

    y->height = max(height_of_node(y->left), height_of_node(y->right)) + 1;
    x->height = max(height_of_node(x->left), height_of_node(x->right)) + 1;
    return x;
}


int balanceVal(book_node *N)        // returns the balance value of the node
{
    if (N == NULL)
    {
        return 0;
    }
    return height_of_node(N->left) - height_of_node(N->right);
}

book_node *insert(book_node *Node, book_node *ptr)      
{
    // Finding correct position to insert the node
    if (Node == NULL)
    {
        return ptr;
    }
    if (ptr->book_id < Node->book_id)
    {
        Node->left = insert(Node->left, ptr);
    }
    else if (ptr->book_id > Node->book_id)
    {
        Node->right = insert(Node->right, ptr);
    }
    else
    {
        return Node;
    }
    // Updating balance factor of each node and balance tree

    Node->height = 1 + max(height_of_node(Node->left), height_of_node(Node->right));

    int balance = balanceVal(Node);
    if (balance > 1 && ptr->book_id < Node->left->book_id)
    {
        return rotateRight(Node);
    }

    if (balance < -1 && ptr->book_id > Node->right->book_id)
    {
        return rotateLeft(Node);
    }

    if (balance > 1 && ptr->book_id > Node->left->book_id)
    {
        Node->left = rotateLeft(Node->left);
        return rotateRight(Node);
    }

    if (balance < -1 && ptr->book_id < Node->right->book_id)
    {
        Node->right = rotateRight(Node->right);
        return rotateLeft(Node);
    }
    return Node;
}

book_node *create_library()   // Function to create a library
{
    int num_books;
    printf("Enter the total number of books available in the library: ");
    scanf("%d", &num_books);
    fflush(stdin);
    int num=1;
    while (num <= num_books)
    {
        book_node *nptr = (book_node *)malloc(sizeof(book_node));
        printf("Enter the Id of Book %d: ", num);
        scanf("%d", &nptr->book_id);
        fflush(stdin);
        printf("Enter the title of Book %d: ", num);
        scanf("%s", nptr->title);
        fflush(stdin);
        printf("Enter the name of author of the Book %d: ", num);
        scanf("%s", nptr->author);
        fflush(stdin);
        printf("Enter the subject of the book %d: ", num);
        scanf("%s", nptr->subject);
        fflush(stdin);
        nptr->num_copies_issued = 0;
        nptr->demand = 0;
        printf("Enter the number of copies avaliable: ");
        scanf("%d", &nptr->num_copies_available);
        fflush(stdin);
        nptr->left = NULL;
        nptr->right = NULL;
        nptr->height = 1;
        bptr = insert(bptr, nptr);
        num++;
    }
}

void display_borrow_list()   //Function to view list of borrowers
{
    printf("------Displaying Borrowers List------\n");
    borrower_list *curr = brptr;
    printf("Name\t\tTitle\t\tReturn Date\n");
    while (curr != NULL)
    {
        printf("%s\t\t%s\t\t%d\n", curr->student_name, curr->title, curr->return_date);
        curr = curr->next;
    }
}
 
void display_request_list()   // Function to view people present in request queue
{
    printf("------Displaying Request List------\n");
    request_queue *curr = rptr;
    printf("Name\t\tBook Title\n");
    while (curr != NULL)
    {
        printf("%s\t\t%s\n", curr->student_name, curr->title);
        curr = curr->next;
    }
}

void display_defaulters()  // Function to view list of defaulters (person(s) who returns book late and is not allowed to request more books )
{
    printf("------Displaying Defaulters List------\n");
    defaulters *curr = fptr;
    printf("Name\n");
    while (curr != NULL)
    {
        printf("%s\n", curr->student_name);
        curr = curr->next;
    }
}

int isDefaulter(char s[])  // Function to check if a person is a defaulter or not
{
    defaulters *curr = fptr;
    while (curr != NULL)
    {
        if (strcmp(s, curr->student_name) == 0)
            return 1;
        curr = curr->next;
    }
    return 0;
}

void display_book_details(book_node *book)   // Function to print details of a book
{
    printf("Book ID = %d\n", book->book_id);
    printf("Title of Book is %s\n", book->title);
    printf("Author of Book is %s\n", book->author);
    printf("Number of Books available is %d\n", book->num_copies_available);
    printf("Number of Books issued is %d\n", book->num_copies_issued);
}

void books_available(book_node *node)     // Function to view list of books available in library
{
    if (node != NULL)
    {
        display_book_details(node);
        books_available(node->left);
        books_available(node->right);
    }
}

int num_books_borrowed(char s[])  // Function to count the number of books a person has borrowed
{
    int cnt = 0;
    borrower_list *curr = brptr;
    while (curr != NULL)
    {
        if (strcmp(curr->student_name, s) == 0)
        {
            cnt++;
        }
        curr = curr->next;
    }
    if (cnt >= 3)
        return 1;
    else
        return 0;
}

void insert_faulter(char s[])       // Function to insert a person who is a defaulter in the list of defaulters
{
    defaulters *temp = (defaulters *)malloc(sizeof(defaulters));
    strcpy(temp->student_name, s);
    temp->next = NULL;
    defaulters *curr = fptr;
    if (curr == NULL)
        fptr = temp;
    else
    {
        while (curr->next != NULL)
            curr = curr->next;
        curr->next = temp;
    }
}

int add_faulters(char s[], int date)        
{
    borrower_list *curr = brptr;
    int found = 1;  // flag to check if the person is in the borrower list
    while (curr != NULL && found)
    {
        if (strcmp(s, curr->student_name) == 0)
        {
            if (date > curr->return_date) // if the person has returned the book late
            {
                insert_faulter(s);
                return 1;
            }
        }
        curr = curr->next;
    }
}

void *add_request(char name[], char book[])  // Function to add a request to the request queue
{
    request_queue *temp = (request_queue *)malloc(sizeof(request_queue));
    strcpy(temp->student_name, name);
    strcpy(temp->title, book);
    temp->next = NULL;
    request_queue *curr = rptr;
    if (curr == NULL)
        rptr = temp;
    else
    {
        while (curr->next != NULL)
            curr = curr->next;
        curr->next = temp;
    }
}

void *add_borrower(int date, char s[], char book[])  // Function to add a borrower to the borrower list
{
    borrower_list *temp = (borrower_list *)malloc(sizeof(borrower_list));
    borrower_list *curr = brptr;
    strcpy(temp->student_name, s);
    strcpy(temp->title, book);
    temp->next = NULL;
    char pref[10];
    char coming[10] = "coming";
    char current[10] = "current"; 
    if(date+15>30){       //Assuming all months to have 30 days
        temp->return_date = date-15;
        strcpy(pref, coming);
    }
    else{
        temp->return_date = date+15;
        strcpy(pref, current);
    }
    printf("Hi, %s, Your request is successfully placed. Borrow the book before %d of the %s month!!\n", s, temp->return_date,pref);
    printf("Join the queue again to place more requests\n");
    if (curr == NULL)
        brptr = temp;
    else
    {
        while (curr->next != NULL)
            curr = curr->next;
        curr->next = temp;
    }
}

int is_book_available(char book[], int id)     // Function to check if a book is available or not
{
    book_node *curr = bptr;
    while (curr != NULL)
    {
        if ((id == curr->book_id) && (strcmp(curr->title, book) == 0))
        {
            if (curr->num_copies_available > 0)
            {
                curr->num_copies_available--;
                curr->num_copies_issued++;
                return 1;
            }
            return 0;
        }
        else if (id > curr->book_id){
            curr = curr->right;
        }
        else{
            curr = curr->left;
        }
    }
    return 0;
}

void request_book()     // Function to request a book
{
    char name[MAX_SIZE], book[MAX_SIZE];
    int date, id;
    fflush(stdin);
    printf("Enter your Name\n");
    scanf("%s", name);
    fflush(stdin);
    printf("Enter ID of the book you want to borrow: \n");
    scanf("%d", &id);
    fflush(stdin);
    printf("Enter the Title of book u want to borrow: \n");
    scanf("%s", book);
    fflush(stdin);
    printf("Enter date of borrowing the book\n");
    scanf("%d", &date);
    fflush(stdin);
    if (isDefaulter(name))
        printf("Sorry!! You cannot request a book. Your are in defaulter list\n");
    else if (add_faulters(name, date))
        printf("Sorry!! You cannot request a book. Your are in defaulter list\n");
    else if (num_books_borrowed(name))
        printf("Sorry!! You cannot request a book now as you already borrowed 3 books.\n");
    else if (is_book_available(book,id))
    {
        if (date < 4)
            count++;
        add_borrower(date, name, book);
    }
    else
    {
        printf("Book Not Available right now!! Check if you have entered correct ID or try again after some days!!\n");
        add_request(name, book);
    }
}

void lend_book(char book[], int date)   // Function to lend a book
{
    request_queue *curr = rptr;
    request_queue *prev = NULL;
    while (curr != NULL)
    {
        if (strcmp(curr->title, book) == 0)
        {
            add_borrower(date, curr->student_name, book);
            if (curr == rptr)
            {
                rptr = curr->next;
                free(curr);
            }
            else
            {
                prev->next = curr->next;
                free(curr);
            }
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void return_book()      // Function to return a book
{
    char name[MAX_SIZE];
    char book[MAX_SIZE];
    int date, id;  
    fflush(stdin);
    printf("Enter your name: \n");
    scanf("%s", name);
    fflush(stdin);   
    printf("Enter the Title of the book you want to return: \n");
    scanf("%s", book);
    fflush(stdin);
    printf("Enter the ID of the book you want to return: \n");
    scanf("%d", &id);
    fflush(stdin);
    printf("Enter date of return: \n");
    scanf("%d", &date);
    borrower_list *curr = brptr;
    borrower_list *prev = NULL;
    while (curr != NULL)
    {
        if (strcmp(name, curr->student_name) == 0 & strcmp(book, curr->title) == 0)
        {
            if (isDefaulter(name) == 0 && date > curr->return_date)
            {
                printf("Caution: You have returned book after due date!!\n");
                insert_faulter(name);
            }
            if (curr == brptr)
            {
                brptr = curr->next;
                free(curr);
            }
            else
            {
                prev->next = curr->next;
                free(curr);
            }
            book_node *brr = bptr;
            printf("Thanks %s for returning the book\n", name);
            while (brr != NULL)
            {
                if (strcmp(book, brr->title) == 0)
                {
                    brr->num_copies_available++;
                    is_book_available(book,id);
                    lend_book(book, date);
                    return;
                }
                else if (id > brr->book_id){
                    brr = brr->right;
                }
                else{
                    brr = brr->left;
                }
            }
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("No Entry found\n");
}

int found;

void inorder(book_node *curr, char book[])
{
    if (curr != NULL)
    {
        if (strcmp(curr->title, book) == 0)
            found = 1;
        inorder(curr->left, book);
        inorder(curr->right, book);
    }
}

void Q3_inorder(book_node *node, char book[])       
{
    if (node != NULL)
    {
        if (strcmp(node->title, book) == 0)
            found = 1;
        inorder(node->left, book);
        inorder(node->right, book);
    }
}


int bookPresent(char book[])     // Used to check whether book with given name exists in lirary or not
{
    found = 0;
    inorder(bptr, book);
    return found;
}

// Q1
void display_receive_list()  // Function to display the list of books received
{
    printf("List of people who will receive books\n");
    request_queue *curr = rptr;
    while (curr != NULL)
    {
        if (bookPresent(curr->title))
            printf("%s\n", curr->student_name);
        curr = curr->next;
    }
}

// Q2 
void setDemand(book_node *curr)      // Function to view books in most demand
{
    if(curr !=  NULL)
    {
        setDemand(curr->left);
        curr->demand = curr->num_copies_issued;
        setDemand(curr->right);
    }
}


void most_in_demand(book_node *curr)        // Function to find the most demand book
{
    if(curr != NULL)
    {
        most_in_demand(curr->left);
        request_queue *r1 = rptr;
        while(r1 != NULL)
        {
            if(strcmp(r1->title,curr->title) == 0)
            curr->demand++;
            r1 = r1->next;
        }
        most_demand = max(most_demand,curr->demand);
        most_in_demand(curr->right);
    }
}

void inorder_book(book_node *curr,int m)    // Function to print the books in order of demand
{
    if(curr != NULL)
    {
        inorder_book(curr->left,m);
        if(curr->demand == m)
        printf("%s\n",curr->title);
        inorder_book(curr->right,m);
    }
}

void view_demand()      // Function to view books in most demand
{
    most_demand = -1;
    setDemand(bptr);
    most_in_demand(bptr);
    printf("List of book(s) having maximum demand is/are\n");
    inorder_book(bptr,most_demand);
}

// Q4 and Q5
Node *insert_num(int d,char Name[],Node *head)     
{
    Node *ptr = head;
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->data = d;
    strcpy(temp->name,Name);
    temp->next = NULL;
    if(ptr == NULL)
    {
        head = temp;
        return head;
    }
    while(ptr->next != NULL)
    ptr = ptr->next;
    ptr->next = temp;
    return head;
}

Node *mergeLL(Node *head1,Node *head2)        // Merge function for merge sort for Q4 and Q5
{
    if(head1 == NULL){
        return head2;
    }
    if(head2 == NULL){
        return head1;
    }

    Node *head = NULL;
    Node *temp2 = head2;
    Node *temp1 = head1;
    Node *temp = head;
    while(temp1 != NULL && temp2 != NULL)
    {
        if(temp1->data < temp2->data)
        {
            if(head == NULL)
            {
                head = head1;
                temp = head;
            }
            else
            {
                temp->next = temp1;
                temp = temp1;
            }
            temp1 = temp1->next;
        }
        else
        {
            if(head == NULL)
            {
                head = head2;
                temp = head;
            }
            else
            {
                temp->next = temp2;
                temp = temp2;
            }
            temp2 = temp2->next;
        }
    }
    if(temp1 == NULL)
    temp->next = temp2;
    else if(temp2 == NULL)
    temp->next = temp1;
    return head;
}

Node *merge_sort(Node *head)        // Merge sort used for Q4 and Q5
{
    if(head->next == NULL)
    {
        return head;
    }
    Node *slow = head;
    Node *fast = head;
    while(fast->next != NULL && fast->next->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    Node *temp = slow->next;
    slow->next = NULL;
    head = merge_sort(head);
    temp = merge_sort(temp);
    head = mergeLL(head,temp);
    return head;
}

Node *reverse_LL(Node *head)    // Reverse Linked List used for reversing linked list in Q4
{
    Node *curr = head;
    Node *prev = NULL;
    Node *after = NULL;
    while(curr != NULL)
    {
        after = curr->next;
        curr->next = prev;
        prev = curr;
        curr = after;
    }
    return prev;
}

// Q4
void book_issue_sort()
{
    borrower_list *curr = brptr;
    Node *head = NULL;
    while(curr != NULL)
    {
        borrower_list *brr = brptr;
        int countleft = 0,countright = 0;
        while(brr != curr)
        {
            if(strcmp(curr->student_name,brr->student_name) == 0)
            countleft++;
            brr = brr->next;
        }
        while(brr != NULL)
        {
            if(strcmp(curr->student_name,brr->student_name) == 0)
            countright++;
            brr = brr->next;
        }
        if(countright == 1)
        head = insert_num(countleft+countright,curr->student_name,head);
        curr = curr->next;
    }
    Node *temp = head;
    head = merge_sort(head);
    head = reverse_LL(head);
    while(temp != NULL)
    {
        printf("%s borrowed %d book(s)\n ",temp->name,temp->data);
        temp = temp->next;
    }
}

// Q5
void insert_book(int x,char s1[],char s2[])     
{
    Node *ptr = num_list;
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->data = x;
    strcpy(temp->name,s1);
    strcpy(temp->name2,s2);
    temp->next = NULL;
    if(ptr == NULL)
    {
        num_list = temp;
        return ;
    }
    while(ptr->next != NULL)
    ptr = ptr->next;
    ptr->next = temp;
}

void Q5_inorder(book_node *curr,char s[])
{
    if(curr != NULL)
    {
        Q5_inorder(curr->left,s);
        if(strcmp(s,curr->subject) == 0)
        insert_book(curr->num_copies_available,curr->title,curr->author);
        Q5_inorder(curr->right,s);
    }
}

void subject_sort(char s[])     // Function to sort books according to subject
{
    Q5_inorder(bptr,s);
    Node *head = num_list;
    if(head == NULL)
    {
        printf("No books of this subject found\n");
        return ;
    }
    head = merge_sort(head);
    Node *temp = head;
    while(temp != NULL)
    {
        printf("%s wriiten by %s number of copies available %d\n",temp->name,temp->name2,temp->data);
        temp = temp->next;
    }
}

// Q6
void copies_avail(book_node *curr)
{
    if(curr != NULL)
    {
        copies_avail(curr->left);
        if(curr->num_copies_available > 0)
        printf("%s\n",curr->title);
        copies_avail(curr->right);
    }
}

// Q7
void nonIssued(book_node *curr)
{
    if(curr != NULL)
    {
        nonIssued(curr->left);
        if(curr->num_copies_issued == 0)
        printf("%s\n",curr->title);
        nonIssued(curr->right);
    }
}

// Q8
void ID_range_display(book_node *curr,int start,int end)
{
    if(curr != NULL)
    {
        ID_range_display(curr->left,start,end);
        if(curr->book_id >= start && curr->book_id <= end)
        printf("Book Title %s Book ID %d\n",curr->title,curr->book_id);
        ID_range_display(curr->right,start,end);
    }
}


int main()
{
    printf("************************************************************************************\n");
    printf("------------------------ WELCOME TO LIBRARY MANAGEMENT SYSTEM ----------------------\n");
    printf("************************************************************************************\n\n");
    create_library();
    int choice;
    while (1)
    {
        printf("--------------------------------------------------------------------------------\n");
        printf("---Choose one of the following options to perform the required operation------\n");
        printf("0) Exit\n");
        printf("1) Request a Book from the library\n");
        printf("2) Return a book issued\n");
        printf("3) View book list\n");
        printf("4) View Book request list\n");
        printf("5) View Borrows list\n");
        printf("6) View Defaulters list\n");

        // DSPD Assignment Question
        printf("7) View list of students who will receive books (in request list)\n");    //Q1
        printf("8) Find out the Book in most demand\n");    //Q2
        printf("9) Check the list of students who are receiving book in first 3 days\n");    //Q3
        printf("10) Sort and display the borrower list on the basis of number of books issued\n");   //Q4
        printf("11) Sort and display the title and author's name for all books of a particular subject on the basis of num_copies_available.\n");  //Q5
        printf("12) Display the names of the requested books whose copies are available.\n");  //Q6
        printf("13) Display the title of all the books which have not been issued by anyone.\n");   //Q7
        printf("14) Display all books with their IDs in the range.\n");    //Q8
        printf("--------------------------------------------------------------------------------\n");

        scanf("%d", &choice);
        if (choice == 1)
            request_book();
        else if (choice == 2)
            return_book();
        else if (choice == 3)
            books_available(bptr);
        else if (choice == 4)
            display_request_list();
        else if (choice == 5)
            display_borrow_list();
        else if (choice == 6)
            display_defaulters();
        else if(choice == 7)
            display_receive_list();
        else if(choice == 8)
            view_demand();
        else if(choice == 9)
            printf("Number of students who received books in first 3 days are %d\n",count);
        else if(choice == 10)
            book_issue_sort();
        else if(choice == 11)
        {
            char s[MAX_SIZE];
            printf("Enter the subject: \n");
            fflush(stdin);
            scanf("%s",s);
            subject_sort(s);
        }
        else if(choice == 12)
        {
            copies_avail(bptr);
        }
        else if(choice == 13)
        {
            printf("List of books which are not issued by anyone are\n");
            nonIssued(bptr);
        }
        else if(choice == 14){
            int from,to;
            printf("Enter the range of book id: \n");
            scanf("%d %d",&from,&to);
            ID_range_display(bptr,from,to);
        }
        else if(choice == 0){
            printf("------------------------ Thank you for using our library management system ------------------------\n");
            return 0;
        }
        else{
            printf("Invalid Choice");
        }
    }
    return 0;
}