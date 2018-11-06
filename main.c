#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    } else{
        printf("Files opened successfully\n");
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

void printEmails(EmailList inboxList){
    EmailNode *mail;
    // skip the dummy node
    mail = inboxList->head->next;

    while (mail != NULL){
        printf("%d\t%s %s \n", mail->id, mail->senderReceiver, mail->content);
        mail = mail->next;
        printf("\n\n");
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
    while (fscanf(inboxFile, "%s", line) != EOF){
        size++;
    }

    fseek(inboxFile, 0L, SEEK_SET);

    EmailList inboxList = createList();
    makeEmptyList(inboxList);

    // read from file, create node for each line and add it to the list
    while(fscanf(inboxFile, "%s", line) != EOF && i < size){
        EmailNode *mail = (EmailNode *)malloc(sizeof(EmailNode));
        mail->next = NULL;
        sscanf(line, "%d;%[^;];%[^;];%[^;];%[^;]",&mail->id,
                      mail->senderReceiver,dateTime,mail->subject,mail->content);
        inboxList->tail->next = mail;
        inboxList->tail = mail;
        inboxList->size++;
    }

    fclose(inboxFile);
    return inboxList;
}

int main() {
    time_t ti = time(NULL);
    struct tm t = *localtime(&ti);
    printf("Date: %02d/%02d/%04d\n",t.tm_mday,t.tm_mon+1,t.tm_year+1900);
    printf("Time: %02d:%02d\n\n", t.tm_hour, t.tm_min);

    EmailList inboxList = initialiseInbox();
    // to be changed...
    printEmails(inboxList);
    return 0;
}