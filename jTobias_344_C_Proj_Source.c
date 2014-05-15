/* 
 * File:   main.c
 * Author: Jeremy Tobias
 *
 * Created on February 19, 2013, 9:23 AM
 */
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <utmp.h>

/*
 * Create the main struct
 */
struct node {
    char content[500];
    char key[500];
    struct node *next;
};

// Head and tail nodes for top and bottom of list
struct node *head = NULL;
struct node *tail = NULL;

/**
 * Method to create linked list and add new nodes to the list. Adds nodes behind the current node
 * instead of on top.
 * @param parent Top node
 * @param current Node currently being viewed
 * @param newNode New node to be placed in the list
 * @return 
 */
struct node* addToList(struct node *parent, struct node *current, struct node *newNode) {
    
    if (NULL == head && NULL == parent) {
        parent = malloc(sizeof(struct node));
        
        strcpy(parent->key, newNode->key);
        strcpy(parent->content, newNode->content);
        parent->next = head;   
        
        head = tail = parent;
    } else {
        current = malloc(sizeof(struct node));
        
        strcpy(current->key, newNode->key);
        strcpy(current->content, newNode->content);
        current->next = NULL; 
        
        tail->next = current;
        tail = current;
    }
    
    return;
}

/**
 * Method to take a key given by a condition from main() and compare it to a key
 * on a given node. If the keys match, print out the key and it's contents, else
 * move to the next node.
 * @param key
 */
void findKey(char key[15]) {
    struct node *tmp = malloc(sizeof(struct node));
    struct node *ptr = head;
    
    strcpy(tmp->key, key);
    
    while (ptr != NULL) {
        if (strcmp(tmp->key, ptr->key) == 0) {
            printf("\n********** KEY FOUND **********\n");
            printf("\n%s : %s\n", ptr->key, ptr->content);
            printf("\n********** KEY FOUND **********\n");
            
            break;
        } else {
            ptr = ptr->next;
        }
    }
    return;
}

/**
 * So long as there is an available node, iterate through the list and print
 * the node key and contents.
 */
void printList(void) {
    struct node *ptr = head;
    int count = 1;
    
    printf("\n********** PRINTING LIST **********\n");
    while (ptr != NULL) {
        printf("\n%d: %s :: %s\n", count, ptr->key, ptr->content);
        ptr = ptr->next;
        count++;
    }
    printf("\n********** END **********\n");
    
    return;
  }

int main(int argc, char *argv[]) {
    int select;
    char noSelect[256];
    /**
     * Struct that creates progName node
     * passes the program name and it's directory to the list 
     */
    struct node *progName = (struct node*)malloc(sizeof(struct node));
    char *program_name;
    program_name = argv[0];
    strcpy(progName->key, "Progname");
    strcpy(progName->content, program_name);
    progName->next = NULL;
    addToList(NULL, head, progName);

    /**
     * Struct that creates user node
     * passes the users login name to the list 
     */
    struct node *user = (struct node*)malloc(sizeof(struct node));
    strcpy(user->key, "User");
    strcpy(user->content, getlogin());
    user->next = NULL;
    addToList(NULL, head, user);
    
    /**
     * Struct that creates host node
     * passes the name of the machine that the program is running on 
     */
    struct node *host = (struct node*)malloc(sizeof(struct node));
    struct utsname unameData;
    uname(&unameData);
    strcpy(host->key, "Host");
    strcpy(host->content, unameData.sysname);
    addToList(NULL, head, host);
    
    /**
     * Struct that creates userID node
     * gets the actual user ID on the machine and passes it to the list 
     */
    struct node *userID = (struct node*)malloc(sizeof(struct node));
    char userIdString [256];
    int uID;
    
    uid_t getuid(void);
    uID = getuid;
    
    //convert the user ID to a character array to be passed to list
    snprintf(userIdString, sizeof(userIdString), "%d", uID);
    
    strcpy(userID->key, "UID");
    strcpy(userID->content, userIdString);
    addToList(NULL, head, userID);
    
    /**
     * Struct that creates tty node
     * passes the name of the current user's terminal to the list 
     */
    struct node *tty = (struct node*)malloc(sizeof(struct node));
    strcpy(tty->key, "TTY");
    strcpy(tty->content, ttyname(0));
    tty->next = NULL;
    addToList(NULL, head, tty);
    
    /**
     * Struct that creates date node
     * passes the current date and time to the list 
     */
    struct node *date = (struct node*)malloc(sizeof(struct node));
    time_t rawTime;
    rawTime = time(NULL);
    strcpy(date->key, "Date");
    strcpy(date->content, asctime(localtime(&rawTime)));
    date->next = NULL;
    addToList(NULL, head, date);
    
    /**
     * Struct that creates cwd node
     * passes the name of the current working directory to the list 
     */
    struct node *cwd = (struct node*)malloc(sizeof(struct node));
    char *callDir;
    callDir = getcwd(0,0);
    strcpy(cwd->key, "CWD");
    strcpy(cwd->content, callDir);
    cwd->next = NULL;
    addToList(NULL, head, cwd);
    
    /**
     * Struct that creates files node
     * gets the current working directory and counts the number of files and
     * passes total number of files to the list 
     */
    struct node *files = (struct node*)malloc(sizeof(struct node));
    int count;
    char file_count[256];
    
    DIR *dir;
    struct dirent *entry;
    
    dir = opendir(callDir);
    
    while ((entry = readdir(dir)) != NULL){
        count++;
    }
    closedir(dir);
    
    snprintf(file_count, sizeof(file_count), "%d", count);
    
    strcpy(files->key, "Files");
    strcpy(files->content, file_count);
    files->next = NULL;
    addToList(NULL, head, files);
    
    /**
     * Struct that creates term node
     * passes the user's terminal type to the list 
     */
    struct node *term = (struct node*)malloc(sizeof(struct node));
    char *termPath;
    termPath = getenv("TERM");
    strcpy(term->key, "Term");
    strcpy(term->content, termPath);
    term->next = NULL;
    addToList(NULL, head, term);
    
    /**
     * Struct that creates nUsers node
     * passes the number of users currently on the machine to the list 
     */
    struct node *nUsers = (struct node*)malloc(sizeof(struct node));
    char cmd[256];
    char reader[256];
    
    sprintf(cmd, "echo \"$(ps aux | cut -d' ' -f1 | sort -d | uniq | wc -l)\" > who.txt");
    system(cmd);
    
    FILE *fr;
    fr = fopen("who.txt", "r");
    
    while (fgets(reader, 256, fr) != NULL) {
        sscanf(reader, "%s", &reader);
        sprintf("%s", reader);
    }
    fclose(fr);
    
    strcpy(nUsers->key, "Nusers");
    strcpy(nUsers->content, reader);
    nUsers->next = NULL;
    addToList(NULL, head, nUsers);
    
    /**
     * Create the initial menu 
     */
    printf("\nEnter a number:\n");
    printf("0: Quit\n");
    printf("1: Progname\n");
    printf("2: User\n");
    printf("3: Host\n");
    printf("4: UID\n");
    printf("5: TTY\n");
    printf("6: Date\n");
    printf("7: CWD\n");
    printf("8: Files\n");
    printf("9: Term\n");
    printf("10: Nusers\n");
    printf("11: Print all nodes\n");
    printf("\n>>> ");

    scanf("%d", &select);
    scanf("%s", noSelect);
    
    /**
     * Take the input number and find the key associated with the selected input
     * If the input is 0, quit the program. If the input does not match any selection,
     * ask for new input. 
     */
    while (select != 0) {
        if (select == 1) {
            findKey("Progname");
        } else if (select == 2) {
            findKey("User");
        } else if (select == 3) {
            findKey("Host");
        } else if (select == 4) {
            findKey("UID");
        } else if (select == 5) {
            findKey("TTY");
        } else if (select == 6) {
            findKey("Date");
        } else if (select == 7) {
            findKey("CWD");
        } else if (select == 8) {
            findKey("Files");
        } else if (select == 9) {
            findKey("Term");
        } else if (select == 10) {
            findKey("Nusers");
        } else if (select == 11) {
            printList();
        } else {
            printf("\nYour current selection does not match anything. Goodbye...\n");
            break;
        }
        
        /**
         * Reprint menu 
         */
        printf("\nEnter a number:\n");
        printf("0: Quit\n");
        printf("1: Progname\n");
        printf("2: User\n");
        printf("3: Host\n");
        printf("4: UID\n");
        printf("5: TTY\n");
        printf("6: Date\n");
        printf("7: CWD\n");
        printf("8: Files\n");
        printf("9: Term\n");
        printf("10: Nusers\n");
        printf("11: Print all nodes\n");
        printf("\n>>> ");

        scanf("%d", &select);
    }
    
    getchar();
    
    return (0);
}