#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 50
typedef struct book_list
{
    char title[MAX_SIZE];
    char author[MAX_SIZE];
    char subject[MAX_SIZE];
    int num_copies_issued;
    int num_copies_available;
    int demand;
    struct book_list *next;
} book_list;

typedef struct request_queue
{
    char student_name[MAX_SIZE];
    char title[MAX_SIZE];
    struct request_queue *next;
} request_queue;

typedef struct borrower_list
{
    char student_name[MAX_SIZE];
    char title[MAX_SIZE];
    int return_date;
    struct borrower_list *next;
} borrower_list;

typedef struct defaulters
{
    char student_name[MAX_SIZE];
    struct defaulters *next;
} defaulters;

// For Q6
typedef struct defaulter_list
{
    char name[MAX_SIZE];
    struct defaulter_list *next;
}defaulter_list;

// For Q4 and 5
typedef struct Node 
{
    int data;
    char name[MAX_SIZE];
    char name2[MAX_SIZE];
    struct Node *next;
}Node;

book_list *bptr = NULL;   // Initializing all the pointers to NULL
defaulters *fptr = NULL;
borrower_list *brptr = NULL;
request_queue *rptr = NULL;
defaulter_list *dptr = NULL;

int count = 0;                  // counter for Q3

// Used to initialise library with books 

int max(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}

book_list *create_library()   // Function to create a library
{
    int num_books;
    printf("Enter the total number of books available in the library: ");
    scanf("%d", &num_books);
    fflush(stdin);
    book_list *ptr = (book_list *)malloc(sizeof(book_list));
    ptr = NULL;
    int num=1;
    while (num <= num_books)
    {
        book_list *nptr = (book_list *)malloc(sizeof(book_list));
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
        nptr->next = NULL;
        printf("Enter the number of copies avaliable: ");
        scanf("%d", &nptr->num_copies_available);
        fflush(stdin);
        if (bptr == NULL)
        {
            bptr = nptr;
            ptr = bptr;
        }
        else
        {
            ptr->next = nptr;
            ptr = ptr->next;
        }
        num++;
    }
    ptr->next = NULL;
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
void display_book_list()  // Function to view list of books
{
    printf("------Displaying Book List------\n");
    book_list *curr = bptr;
    printf("Title\tAuthor\tSubject\tAvailable\tIssued\n");
    while (curr != NULL)
    {
        printf("%s\t\t%s\t\t%s\t\t%d\t\t%d\n", curr->title,curr->author,curr->subject, curr->num_copies_available, curr->num_copies_issued);
        curr = curr->next;
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

int is_book_available(char book[])
{
    book_list *curr = bptr;
    while (curr != NULL)
    {
        if (strcmp(book, curr->title) == 0)
        {
            if (curr->num_copies_available == 0)
                return 0;
            else
            {
                curr->num_copies_available--;
                curr->num_copies_issued++;
                return 1;
            }
        }
        curr = curr->next;
    }
    return 0;
}
void insert_faulter(char s[])
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
void insert_defaulter(char s[])
{
    defaulter_list *temp = (defaulter_list *)malloc(sizeof(defaulter_list));
    strcpy(temp->name, s);
    temp->next = NULL;
    defaulter_list *curr = dptr;
    if (curr == NULL)
        dptr = temp;
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

void request_book()     // Function to request a book
{
    char name[MAX_SIZE], book[MAX_SIZE];
    int date;
    fflush(stdin);
    printf("Enter your Name\n");
    scanf("%s", name);
    fflush(stdin);
    printf("Enter the title of book u want to borrow: \n");
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
    else if (is_book_available(book))
    {
        if (date < 4)
            count++;
        add_borrower(date, name, book);
    }
    else
    {
        printf("Book Not Available right now!! Try again after some days!!\n");
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
    int date;  
    fflush(stdin);
    printf("Enter your name: \n");
    scanf("%s", name);
    fflush(stdin);   
    printf("Enter the Title of the book you want to return: \n");
    scanf("%s", book);
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
            book_list *brr = bptr;
            printf("Thanks %s for returning the book\n", name);
            while (brr != NULL)
            {
                if (strcmp(book, brr->title) == 0)
                {
                    brr->num_copies_available++;
                    is_book_available(book);
                    lend_book(book, date);
                    return;
                }
                brr = brr->next;
            }
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("No Entry found\n");
}
int bookPresent(char book[])   // Function to check if a book is present in the library
{
    book_list *curr = bptr;
    while (curr != NULL)
    {
        if (strcmp(curr->title, book) == 0)
            return 1;
        curr = curr->next;
    }
    return 0;
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
void view_demand()     // Function to view the demand of books
{
    book_list *curr = bptr;
    int MAX = -1;
    printf("Book Name\tDemand\n");
    while (curr != NULL)
    {
        curr->demand = curr->num_copies_issued;
        request_queue *rr = rptr;
        while (rr != NULL)
        {
            if (strcmp(rr->title, curr->title) == 0)
                curr->demand++;
            rr = rr->next;
        }
        MAX = max(MAX, curr->demand);
        printf("%s\t\t%d\n", curr->title, curr->demand);
        curr = curr->next;
    }
    curr = bptr;
    printf("List of book(s) having maximum demand is/are\n");
    while (curr != NULL)
    {
        if (curr->demand == MAX)
            printf("%s\n", curr->title);
        curr = curr->next;
    }
}
// Q10
void max_requests()         
{
    printf("List of students who have requested for the maximum books are\n");
    request_queue *curr = rptr;
    int MAX = -1;
    while(curr != NULL)
    {
        request_queue *rr = rptr;
        int count = 0;
        while(rr != NULL)
        {
            if(strcmp(rr->student_name,curr->student_name) == 0)
            count++;
            rr = rr->next;
        }
        MAX = max(MAX,count);
        curr = curr->next;
    }
    curr = rptr;
    // printf("Max = %d\n",MAX);
    while(curr != NULL)
    {
        request_queue *rr = rptr;
        int count = 1;
        while(rr != curr)
        {
            if(strcmp(rr->student_name,curr->student_name) == 0)
            count++;
            rr = rr->next;
        }
        if(count == MAX)
        printf("%s\n",curr->student_name);
        curr = curr->next;
    }
}

void add_defaulters(int date)    // Function to add the defaulters
{
    request_queue *curr = rptr;
    request_queue *prev = NULL;
    while(curr != NULL)
    {
        defaulters *frr = fptr;
        int left = 1;  // To check if the student has left the library
        while(frr != NULL && left)
        {
            if(strcmp(frr->student_name,rptr->student_name) == 0)
            left = 0;
            frr = frr->next;
        }
        if(left == 1)
        {
            borrower_list *brr = brptr;
            while(brr != NULL && left)
            {
                if(strcmp(brr->student_name,curr->student_name) == 0)
                {
                    if(date > brr->return_date)
                    {
                        insert_faulter(curr->student_name);
                        left = 0;
                    }
                }
                brr = brr->next;
            }
        }
        if(left == 0)
        {
            defaulter_list *temp = dptr;
            int found = 0;
            while(temp != NULL && found == 0)
            {
                if(strcmp(temp->name,curr->student_name) == 0)
                found = 1;
                temp = temp->next;
            }
            if(found == 0)
            insert_defaulter(curr->student_name);
            if(curr = rptr)
            {
                rptr = curr->next;
                free(curr);
            }
            else
            {
                prev->next = curr->next;
                free(curr);
            }
        }
        prev = curr;
        curr = curr->next;
    }
    defaulter_list *drr = dptr;
    printf("List of all defaulters is\n");
    while(drr != NULL)
    {
        printf("%s\n",drr->name);
        drr = drr->next;
    }
}
Node *insert_num(int x,char name2[],Node *head)   // Function to insert the number in the list
{
    Node *ptr = head;
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->data = x;
    strcpy(temp->name,name2);
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
Node *mergeLL(Node *head1,Node *head2)        //Function to merge two LL
{
    if(head1 == NULL)
    return head2;
    if(head2 == NULL)
    return head1;
    Node *head = NULL;
    Node *curr2 = head2;
    Node *curr1 = head1;
    Node *curr = head;
    while(curr1 != NULL && curr2 != NULL)
    {
        if(curr1->data < curr2->data)
        {
            if(head == NULL)
            {
                head = head1;
                curr = head;
            }
            else
            {
                curr->next = curr1;
                curr = curr1;
            }
            curr1 = curr1->next;
        }
        else
        {
            if(head == NULL)
            {
                head = head2;
                curr = head;
            }
            else
            {
                curr->next = curr2;
                curr = curr2;
            }
            curr2 = curr2->next;
        }
    }
    if(curr1 == NULL)
    curr->next = curr2;
    else if(curr2 == NULL)
    curr->next = curr1;
    return head;
}
Node *merge_sort(Node *head)
{
    if(head->next == NULL)
    {
        printf("error here\n");
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
Node *reverse_LL(Node *head)   // Function to reverse the LL
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
void book_issue_sort()       // Function to sort book issue list
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
        printf("%d ",temp->data);
        temp = temp->next;
    }
}

Node *insert_book(Node *head,int x,char s1[],char s2[])  // Function to insert the book in the LL
{
    Node *nrr = head;
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->data = x;
    strcpy(temp->name,s1);
    strcpy(temp->name2,s2);
    temp->next = NULL;
    if(nrr == NULL)
    {
        head = temp;
        return head;
    }
    while(nrr->next != NULL)
    nrr = nrr->next;
    nrr->next = temp;
    return head;
}

void subject_sort(char s[])    // Function to sort the book issue list according to subject
{
    book_list *curr = bptr;
    Node *head = NULL;
    while(curr != NULL)
    {
        if(strcmp(s,curr->subject) == 0)
        head = insert_book(head,curr->num_copies_available,curr->title,curr->author);
        curr = curr->next;
    }
    if(head == NULL)
    {
        printf("No books of this subject found\n");
        return ;
    }
    head = merge_sort(head);
    Node *trav = head;
    while(trav != NULL)
    {
        printf("%s wriiten by %s number of copies available %d\n",trav->name,trav->name2,trav->data);
        trav = trav->next;
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
        printf("7) View list of students who will receive books\n");    //Q1
        printf("8) Find out the Book in most demand\n");    //Q2
        printf("9) Check the list of students who are receiving book in first 3 days\n");    //Q3
        printf("10) Sort and display the borrower list on the basis of number of books issued\n");   //Q4
        printf("11) Sort and display the title and author's name for all books of a particular subject on the basis of num_copies_available.\n");  //Q5
        printf("12) Display the defaulter_list after separating them from request queue\n");   //Q6
        printf("13) View the names of the students who have already borrowed books and are asking for other books.\n");  //Q7
        printf("14) Display the names of the requested books whose copies are available.\n");  //Q8
        printf("15) Display the title of all the books which have not been issued by anyone.\n");   //Q9
        printf("16) View the name of the student/s who has requested for the maximum number of  books.\n");    //Q10
        printf("--------------------------------------------------------------------------------\n");
        scanf("%d", &choice);
        if (choice == 1)
            request_book();
        else if (choice == 2)
            return_book();
        else if (choice == 3)
            display_book_list();
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
            int date;
            printf("Enter today's date: \n");
            scanf("%d",&date);
            add_defaulters(date);
        }
        else if(choice == 13)
        {
            printf("------List of students who have already borrowed books and are asking for other books------ \n");
            request_queue *rr = rptr;
            while(rr != NULL)
            {
                borrower_list *curr = brptr;
                while(curr != NULL)
                {
                    if(strcmp(rr->student_name,curr->student_name) == 0)
                    printf("%s Asking for book %s already borrowed %s\n",curr->student_name,rr->title,curr->title);
                    curr = curr->next;
                }
                rr = rr->next;
            }
        }
        else if(choice == 14)
        {
            book_list *curr = bptr;
            while(curr != NULL)
            {
                if(curr->num_copies_available > 0)
                printf("%s\n",curr->title);
                curr = curr->next;
            }
        }
        else if(choice == 15)
        {
            printf("------List of books which are not issued by anyone------\n");
            book_list *curr = bptr;
            while(curr != NULL)
            {
                if(curr->num_copies_issued == 0)
                printf("%s\n",curr->title);
                curr = curr->next;
            }
        }
        else if(choice == 16)
        max_requests();
        else if(choice==0){
           return 0;
        }
        else{
            printf("Invalid Choice");
        }
    }
    return 0;
}