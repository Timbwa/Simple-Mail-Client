/*
 * ID     : 2278026
 * Author : David Timbwa Auna
 * Date   : 11/11/2018
 * */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
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
        printf("%-4d%s\t%2d/%2d/%-6d%2d:%2d\t%s\t%s", mail->id, mail->senderReceiver,
               mail->date.day, mail->date.month, mail->date.year, mail->date.hour, mail->date.min, mail->subject, mail->content);
        mail = mail->next;
    }

    mail = sentList->head->next;
    printf("\nSent box\n");

    while (mail != NULL){
        printf("%-4d%s\t%2d/%2d/%-6d%02d:%02d\t%s\t%s", mail->id, mail->senderReceiver,
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
    // append new line to the last email read
    strcat(sentList->tail->content,"\n");
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
    strcat(newMail->content,"\n");
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

EmailList deleteInboxEmail(EmailList inboxList, int deleteID){
    if(!isListEmpty(inboxList)){
        EmailNode *currentNode;
        currentNode = inboxList->head;
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
            inboxList = reassignIDs(inboxList);
            inboxList->size--;
            printf("Email with ID %d has been deleted from your inbox\n",deleteID);
        }
        if (inboxList->size == 0){
            inboxList->tail = inboxList->head;
            printf("The inbox is empty\n");
        }
    }
    return inboxList;
}

EmailList deleteSentEmail(EmailList sentList, int deleteID) {
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
            printf("Email with ID %d has been deleted from your sent box\n",deleteID);
        }
        if (sentList->size == 0){
            sentList->tail = sentList->head;
            printf("The inbox is empty\n");
        }
    }
    return sentList;
}

// functions to remove spaces from entered string
void removeSpaces(char str[])
{
    int count = 0;

    for (int i = 0;str[i]; ++i) {
        if (str[i] != ' '){
            str[count] = str[i];
            count++;
        }
    }
    str[count] = '\0';
}

// function to lower char of string for good comparison
char * lowerChar(char *str){
    int i = 0;
    while(str[i]){
        str[i] = (char) tolower(str[i]);
        i++;
    }
    return str;
}


void searchEmail(EmailList inboxList, EmailList sentList){
    EmailNode *mailInbox = inboxList->head->next;
    EmailNode *mailSent = sentList->head->next;

    int boxChoice, criterion;
    char searchParam[50], tempString[50];

    printf("\nInbox (1) or Sent box (2): ");
    scanf("%d", &boxChoice);
    fflush(stdin);
    printf("Receipt (1) or Subject (2): ");
    scanf("%d", &criterion);
    switch (boxChoice){
        case 1:
            switch (criterion){
                case 1:
                    printf("Receipt: ");
                    fflush(stdin);
                    gets(searchParam);
                    while(mailInbox != NULL){
                        strcpy(tempString,mailInbox->senderReceiver);
                        removeSpaces(tempString);
                        removeSpaces(searchParam);
                        if(!strcmp(lowerChar(searchParam), lowerChar(tempString))){
                            printf("%-4d%s\t%2d/%2d/%-6d%2d:%2d\t%s\t%s \n", mailInbox->id, mailInbox->senderReceiver,
                                   mailInbox->date.day, mailInbox->date.month, mailInbox->date.year, mailInbox->date.hour, mailInbox->date.min, mailInbox->subject, mailInbox->content);
                        }
                        mailInbox = mailInbox->next;
                    }
                    break;
                case 2:
                    printf("Subject: ");
                    fflush(stdin);
                    gets(searchParam);
                    while(mailInbox != NULL){
                        strcpy(tempString,mailInbox->subject);
                        removeSpaces(tempString);
                        removeSpaces(searchParam);
                        if(!strcmp(lowerChar(searchParam), lowerChar(tempString))){
                            printf("%-4d%s\t%2d/%2d/%-6d%2d:%2d\t%s\t%s \n", mailInbox->id, mailInbox->senderReceiver,
                                   mailInbox->date.day, mailInbox->date.month, mailInbox->date.year, mailInbox->date.hour, mailInbox->date.min, mailInbox->subject, mailInbox->content);
                        }
                        mailSent = mailSent->next;
                    }
                    break;
                default:
                    printf("No such choice\n");
            }
            break;
        case 2:
            switch (criterion){
                case 1:
                    printf("Receipt: ");
                    fflush(stdin);
                    gets(searchParam);
                    while(mailSent != NULL){
                        strcpy(tempString,mailSent->senderReceiver);
                        removeSpaces(tempString);
                        removeSpaces(searchParam);
                        if(!strcmp(lowerChar(searchParam), lowerChar(tempString))){
                            printf("%-4d%s\t%2d/%2d/%-6d%2d:%2d\t%s\t%s \n", mailSent->id, mailSent->senderReceiver,
                                   mailSent->date.day, mailSent->date.month, mailSent->date.year, mailSent->date.hour, mailSent->date.min, mailSent->subject, mailSent->content);
                        }
                        mailSent = mailSent->next;
                    }
                    break;
                case 2:
                    printf("Subject: ");
                    fflush(stdin);
                    gets(searchParam);
                    while(mailSent != NULL){
                        strcpy(tempString,mailSent->subject);
                        removeSpaces(tempString);
                        removeSpaces(searchParam);
                        if(!strcmp(lowerChar(searchParam), lowerChar(tempString))){
                            printf("%-4d%s\t%2d/%2d/%-6d%2d:%2d\t%s\t%s \n", mailSent->id, mailSent->senderReceiver,
                                   mailSent->date.day, mailSent->date.month, mailSent->date.year, mailSent->date.hour, mailSent->date.min, mailSent->subject, mailSent->content);
                        }
                        mailSent = mailSent->next;
                    }
                    break;
                default:
                    printf("No such choice\n");
            }
            break;
        default:
            printf("No such choice!\n");
    }

}

// overwrite the files with currrent lists
void overwrite(EmailList inboxList, EmailList sentList){
    FILE *inboxFile, *sentfile;
    inboxFile = fopen("inbox.txt","w+");
    fileError(inboxFile);
    sentfile = fopen("sent.txt","w+");
    fileError(sentfile);

    EmailNode *mailInbox = inboxList->head->next;
    EmailNode *mailSent = sentList->head->next;

    while(mailInbox != NULL){
        fprintf(inboxFile,"%d;%s;%2d/%2d/%4d %2d:%d;%s;%s", mailInbox->id, mailInbox->senderReceiver,
                mailInbox->date.day, mailInbox->date.month, mailInbox->date.year, mailInbox->date.hour, mailInbox->date.min, mailInbox->subject, mailInbox->content);
        mailInbox = mailInbox->next;
    }
    fclose(inboxFile);

    while(mailSent != NULL){
        fprintf(sentfile,"%d;%s;%2d/%2d/%4d %2d:%d;%s;%s", mailSent->id, mailSent->senderReceiver,
                mailSent->date.day, mailSent->date.month, mailSent->date.year, mailSent->date.hour, mailSent->date.min, mailSent->subject, mailSent->content);
        mailSent = mailSent->next;
    }
    fclose(sentfile);

    printf("The inbox.txt and sent.txt have been updated. \n");
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
        printf("\n--- MENU ---\n"
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
                printf("Your email has been sent successfully.\n\n");
                break;
            case 2:
                printf("Email ID: ");
                scanf("%d", &deleteID);
                inboxList = deleteInboxEmail(inboxList, deleteID);
                break;
            case 3:
                printf("Email ID: ");
                scanf("%d", &deleteID);
                sentList = deleteSentEmail(sentList, deleteID);
                break;
            case 4:
                printEmails(inboxList, sentList);
                break;
            case 5:
                searchEmail(inboxList, sentList);
                break;
            case 6:
                overwrite(inboxList, sentList);
                printf("See you later!");
                exit(1);
            default:
                printf("No such option try again!\n");
        }
    }
    return 0;
}