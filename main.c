#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
// defining necessary structures
struct dateTime{
    int day;
    int month;
    int year;
    int hour;
    int min;
};

typedef struct dateTime DateTime;

struct emailNode{
    int id;
    char senderReceiver[50];
    DateTime date;
    char subject[50];
    char content[200];
    struct emailNode *next;
};

typedef struct emailNode EmailNode;

struct emailList{
    EmailNode *head;
    EmailNode *tail;
    int size;
};

typedef struct emailList *EmailList;

// function to check for succesful file opening
void fileError(FILE *file){
    if (file == NULL){
        printf("Could not open file !");
        exit(1);
    }
}

void makeEmptyList(EmailList mailList){
    // create dummy node
    mailList->head = (EmailNode *) malloc(sizeof(EmailNode));
    if(mailList->head == NULL) {
        printf("Out of memory !");
        exit(1);
    }
    mailList->head->next = NULL;
    mailList->tail = mailList->head;
    mailList->size = 0;
}

EmailList createList(){
    EmailList mailList;
    mailList = (EmailList) malloc(sizeof(EmailList));
    if(mailList == NULL) {
        printf("Out of memory !");
        exit(1);
    }
    makeEmptyList(mailList);
    return mailList;
}

void printEmails(EmailList inboxList, EmailList sentList){
    EmailNode *mail;
    // skip the dummy node
    mail = inboxList->head->next;
    printf("\nInbox\n");

    while (mail != NULL){
        printf("%-4d%s\t%2d/%2d/%-6d%2d:%2d\t%s\t%s \n", mail->id, mail->senderReceiver,
               mail->date.day, mail->date.month, mail->date.year, mail->date.hour, mail->date.min, mail->subject, mail->content);
        mail = mail->next;
    }

    mail = sentList->head->next;
    printf("\nSent box\n");

    while (mail != NULL){
        printf("%-4d%s\t%2d/%2d/%-6d%02d:%02d\t%s\t%s \n", mail->id, mail->senderReceiver,
               mail->date.day, mail->date.month, mail->date.year, mail->date.hour, mail->date.min, mail->subject, mail->content);
        mail = mail->next;
    }
}

EmailList initialiseInbox(){
    FILE *inboxFile;
    inboxFile = fopen("inbox.txt","r");
    fileError(inboxFile);
    int i = 0;
    char line[350]; // max size of each line in the file
    int size = 0;
    char dateTime[20];

    // count the number of lines
    while (fgets(line,350, inboxFile)){
        size++;
    }

    fseek(inboxFile, 0L, SEEK_SET);

    EmailList inboxList = createList();
    makeEmptyList(inboxList);
    // read from file, create node for each line and add it to the list
    while(fgets(line,350, inboxFile) && i < size){
        EmailNode *mail = (EmailNode *)malloc(sizeof(EmailNode));
        mail->next = NULL;
        // initialise email node
        sscanf(line, "%d;%[^;];%[^;];%[^;];%[^;]",&mail->id,
                      mail->senderReceiver,dateTime,mail->subject,mail->content);
        sscanf(dateTime,"%2d/%2d/%4d %2d:%2d", &mail->date.day,
                        &mail->date.month,
                        &mail->date.year,
                        &mail->date.hour,
                        &mail->date.min);
        inboxList->tail->next = mail;
        inboxList->tail = mail;
        inboxList->size++;

    }

    fclose(inboxFile);
    return inboxList;
}

EmailList initialiseSent(){
    FILE *sentFile;
    sentFile = fopen("sent.txt","r");
    fileError(sentFile);
    int i = 0;
    char line[350]; // max size of each line in the file
    int size = 0;
    char dateTime[20];

    // count the number of lines
    while (fgets(line,350, sentFile)){
        size++;
    }

    fseek(sentFile, 0L, SEEK_SET);

    EmailList sentList = createList();
    makeEmptyList(sentList);

    // read from file, create node for each line and add it to the list
    while(fgets(line,350, sentFile) && i < size){
        EmailNode *mail = (EmailNode *)malloc(sizeof(EmailNode));
        mail->next = NULL;
        sscanf(line, "%d;%[^;];%[^;];%[^;];%[^;]",&mail->id,
               mail->senderReceiver,dateTime,mail->subject,mail->content);
        sscanf(dateTime,"%2d/%2d/%4d %2d:%2d", &mail->date.day,
               &mail->date.month,
               &mail->date.year,
               &mail->date.hour,
               &mail->date.min);
        sentList->tail->next = mail;
        sentList->tail = mail;
        sentList->size++;

    }
    fclose(sentFile);
    return sentList;
}


EmailList sendEmail(EmailList sentList, char receiver[50], DateTime date, char subject[50], char content[200]) {
    // new mail node declaration and initialisation.
    EmailNode *newMail = (EmailNode *) malloc(sizeof(EmailNode));
    newMail->next = NULL;
    newMail->id = sentList->tail->id + 1;
    strcpy(newMail->senderReceiver, receiver);
    strcpy(newMail->subject, subject);
    strcpy(newMail->content, content);
    newMail->date.day = date.day;
    newMail->date.month = date.month;
    newMail->date.year = date.year;
    newMail->date.hour = date.hour;
    newMail->date.min = date.min;

    // add tp the end of the list
    sentList->tail->next = newMail;
    sentList->tail = newMail;
    sentList->size++;

    return sentList;
}

int isListEmpty(EmailList mailList){
    return (mailList->size == 0);
}

// function to overwrite ID's when deletion occurs
EmailList reassignIDs(EmailList mailList){
    int i = 1;
    EmailNode *tempNode = mailList->head->next;

    while (tempNode != NULL){
        tempNode->id = i;
        tempNode = tempNode->next;
        i++;
    }
    return mailList;
}

EmailList deleteInboxEmail(EmailList sentList, int deleteID) {
    if(!isListEmpty(sentList)){
        EmailNode *currentNode;
        currentNode = sentList->head;
        while (currentNode->next != NULL && currentNode->next->id != deleteID){
            currentNode = currentNode->next;
        }
        if(currentNode->next == NULL){
            printf("No email with ID %d is in your inbox!\n", deleteID);
        } else{
            EmailNode *deleteNode;
            deleteNode = currentNode->next;
            currentNode->next = currentNode->next->next;
            free(deleteNode);
            sentList = reassignIDs(sentList);
            sentList->size--;
            printf("Email with ID %d has been deleted from your inbox\n",deleteID);
        }
        if (sentList->size == 0){
            sentList->tail = sentList->head;
            printf("The inbox is empty\n");
        }
    }
}

int main() {
    time_t ti = time(NULL);
    struct tm t = *localtime(&ti);
    int option = -1;
    int deleteID;
    char senderReceiver[50];
    char subject[50];
    char content[200];
    DateTime date;

    //printf("Date: %02d/%02d/%04d\n",t.tm_mday,t.tm_mon+1,t.tm_year+1900);
    //printf("Time: %02d:%02d\n\n", t.tm_hour, t.tm_min);

    EmailList inboxList = initialiseInbox();
    EmailList sentList = initialiseSent();

    printf("Welcome to Your Email Client!\n"
                   "Your inbox and sent box have been loaded successfully.\n");
    while(option){
        printf("--- MENU ---\n"
                       "1. Send an email\n"
                       "2. Delete an email from your inbox\n"
                       "3. Delete an email from your sent box\n"
                       "4. See your emails\n"
                       "5. Search for an email\n"
                       "6. Exit\n"
                       "Option: ");
        scanf("%d", &option);
        fflush(stdin);
        switch (option){
            case 1:
                printf("Recipient: ");
                gets(senderReceiver);
                printf("Subject: ");
                gets(subject);
                printf("Content: ");
                gets(content);
                date.day = t.tm_mday;
                date.month = t.tm_mon + 1;
                date.year = t.tm_year + 1900;
                date.hour = t.tm_hour;
                date.min = t.tm_min;
                sentList = sendEmail(sentList, senderReceiver, date, subject, content);
                printf("Your email has been sent successfully.\n");
                break;
            case 2:
                printf("Email ID: ");
                scanf("%d", &deleteID);
                deleteInboxEmail(sentList, deleteID);
                break;
            case 3:
                break;
            case 4:
                printEmails(inboxList, sentList);
                break;
            case 5:
                break;
            case 6:
                printf("See you later!");
                exit(1);
            default:
                printf("No such option try again!\n");
        }
    }
    return 0;
}