# Simple Email Client
**Motivation** : First Abstract Data Structures assignment implemented using linked lists
** **

This assignment aims to help you practice linked list data structure and basic linked list operations. Your main task in this assignment is to write a C program to create a simple email client application that will allow you manage your emails. This application will include two separate lists for the emails you received and the emails you sent. For each email, it stores the following details: sender for the emails you received (max 50 characters) or recipient for the emails you sent (max 50 characters), date-time (Format: DD/MM/YYYY HH:MM), subject (max 50 characters) and content (max 200 characters).

When you start the application, it will read your emails from external text files (inbox.txt and sent.txt), create two separate linked lists (one for the emails you received and another one for the emails you sent) and then populate these lists with the emails stored in those two text files. This application will then provide some operations for you to manage your emails. When you exit from the application, the application will overwrite the external files with the latest version of the lists to include the latest emails in your inbox and sent box. Therefore, this simple email application needs to support the following operations.

• **Reading your emails from external files and initialising your email lists** : The application will start by reading the emails from external text files called inbox.txt and sent.txt.  The inbox.txt file will include the details of the emails you received including unique id, sender, date-time, subject and content separated by a semicolon. Likewise, the sent.txt file will include the details of the emails you sent including unique id, recipient, date-time, subject and content separated by a semicolon. We assume that there is no semicolon in the subjects and contents of emails. Example content for each file is shown below:

** **

inbox.txt
```
1;sjacob@mail.co.uk;22/10/2018 14:11;Hi;How are you?
2;brichard@mail.com;22/10/2018 15:15;Meeting;Hi there, Will we meet at 16:00? 
3;cthomas@xyz.com;23/10/2018 16:40;Book;Can I borrow your book for tomorrow?
```
sent.txt
```
1;cdaniel@mail.com;22/10/2018 15:55;Notes; Can you send me your lecture notes?
2;jennifer@mail.co.uk;23/10/2018 16:45;Hello; Hi, how are you?
3;cthomas@xyz.com;23/10/2018 16:45;Book; Yes you can
```
The application will then internally store these emails in two linked lists. Please note that you cannot make any assumptions about the number of emails sent/received and there is no upper limit for the number of emails. In your internal representation, you will need to decide about the structure of your linked list based on the txt files given above.

- **Sending a new email:** After the initialisation of the application with the external files, you may want to send a new email. In this case, the application will ask you to enter the email address of the recipient, subject and content. In addition to these details, the application will take the date-time from the system and add the new email with all the details in the list of emails you sent. You can find the date-time from the system as follows:
```C
#include <stdio.h>; 
#include <time.h>; 
int main() {
  time\_t ti = time(NULL);
  struct tm t = *localtime(&ti);
  printf("Date: %02d/%02d/%04d\n",t.tm\_mday,t.tm\_mon+1,t.tm\_year+1900);
  printf("Time: %02d:%02d\n", t.tm\_hour, t.tm\_min);     
  return 0; 
}
```
** **

- **Deleting an email:** This application will allow you to select a specific email by using its unique id from one of the lists and delete it.

- **Printing your emails** : This application will allow you to print both of the lists.

- **Searching for an email** : The application will allow you to search for an email by using the following details: sender or receipt and subject. You firstly need to select which list you will use for searching, then specify search criterion and search keys. More than one email can be listed after searching. You can make use of external libraries in C, such as string.h library.

- **Overwriting the external files:** When the application is closed, the external files should be overwritten with the latest versions of the lists.

** **

## Programming Requirements

When you write this application, you need to consider the data structure(s) that will be necessary. Clearly, two linked lists will be required: one for the list of emails you received and another one for the list of emails you sent. Therefore, it is recommended that you implement your basic linked list operations first (add/delete/search/print, etc). By using these basic operations on linked lists, you can implement other required operations. You also need to define and use a structure for datetime.

To achieve the steps you need to have the following functions. Please strictly follow the requirements of the functions given below! In addition to these functions, you can write some helper/auxiliary functions.

| **Function** | **Explanation** | **Take** | **Return** |
| --- | --- | --- | --- |
| initialiseInbox | To create and initialise the lists of emails you received by reading the emails from the inbox.txt file | - | A list of emails you received |
| initialiseSent | To create and initialise the lists of emails you sent by reading the emails from the sent.txt file | - | A list of emails you sent |
| sendEmail | To send a new email (insert a new email to the list of email you sent) by taking the required details. | A list of emails you sent,  recipient, date-time, subject, content | A list of emails you sent |
| deleteInboxEmail | To delete an email from the lists of emails you received by using one of its unique id. | A list of emails you received, a unique id | A list of emails you received |
| deleteSentEmail | To delete an email from the lists of emails you sent by using one of its unique id. | A list of emails you sent, a unique id | A list of emails you sent |
| printEmails | To show the emails in both of the lists. | A list of emails you received A list of emails you sent   | - |
| searchEmail | To search for an email by using sender or receipt and subject). You firstly need to select which list you will use for searching, specify search criterion and search key. More than one email can be listed based on search keys. | A list of emails you received A list of emails you sent | - |
| overwrite | To overwrite the external files with the latest versions of the list.   | A list of emails you received A list of emails you sent | - |

## Grading

Your program will be graded as follows:

| **Grading Point** | **Mark (out of 100)** |
| --- | --- |
| Structures to represent a list of emails and date-time | 5 |
| Main function to control and coordinate the commands | 15 |
| initialiseInbox | 10 |
| initialiseSent | 10 |
| sendEmail | 10 |
| deleteInboxEmail | 5 |
| deleteSentEmail | 5 |
| printEmails | 5 |
| searchEmail | 20 |
| overwrite | 15 |

** **

**Note:** Remember to have good programming style (Appropriate comments, variable names, formulation of selection statements and loops, reusability, extensibility etc.). Each of the items above will include10% for good programming style.

## Sample Run
```
Welcome to Your Email Client!

Your inbox and sent box have been loaded successfully.

--- MENU ---

1. Send an email
2. Delete an email from your inbox
3. Delete an email for your sent box
4. See your emails
5. Search for an email
6. Exit

Option: 1

Recipient: jsmith@email.edu

Subject: Question Book

Content: Do you have office hours today? I&#39;d like to ask you some questions from the book.

Your email has been sent successfully.

--- MENU ---

1. Send an email
2. Delete an email from your inbox
3. Delete an email for your sent box
4. See your emails
5. Search for an email
6. Exit

Option: 2

Email ID: 3

Email with ID 3 has been deleted from your inbox.

--- MENU ---

1. Send an email
2. Delete an email from your inbox
3. Delete an email for your sent box
4. See your emails
5. Search for an email
6. Exit

Option: 2

Email ID: 5

No email with ID 5 in your inbox!

--- MENU ---

1. Send an email
2. Delete an email from your inbox
3. Delete an email for your sent box
4. See your emails
5. Search for an email
6. Exit

Option: 3

Email ID: 2

Email with ID 2 has been deleted from your sent box.

--- MENU ---

1. Send an email
2. Delete an email from your inbox
3. Delete an email for your sent box
4. See your emails
5. Search for an email
6. Exit

Option: 4

Inbox

1 sjacob@mail.co.uk 22/10/2018 14:11 Hi How are you?
2 brichard@mail.com 22/10/2018 15:15 Meeting         Hi there, Will we meet at 16:00?

Sent box

1 cdaniel@mail.com 22/10/2018 15:55 Notes Can you send me your lecture notes?

3 cthomas@xyz.com 23/10/2018 16:45 Book Yes you can
4 jsmith@email.edu 24/10/2018 10:46 Question Book Do you have office hours today? I&#39;d like to ask you some questions from the book.

--- MENU ---

1. Send an email
2. Delete an email from your inbox
3. Delete an email for your sent box
4. See your emails
5. Search for an email
6. Exit

Option: 5

Inbox (1) or Sent box (2): 2

Receipt (1) or Subject (2): 2

Subject: Book

1. 3cthomas@xyz.com 23/10/2018 16:45 Book Yes you can
2. 4jsmith@email.edu 24/10/2018 10:46 Question Book Do you have office hours today? I&#39;d like to ask you some questions from the book.

--- MENU ---

1. Send an email
2. Delete an email from your inbox
3. Delete an email for your sent box
4. See your emails
5. Search for an email
6. Exit

Option: 6

The inbox.txt and sent.txt files have been updated.

See you later!
```
