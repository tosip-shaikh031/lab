#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 

#define MAX_TEXT 100

struct msg_buffer {
    long msg_type;
    char msg_text[MAX_TEXT];
};

int main() {
    struct msg_buffer message;
    int msgid;

    // Create unique key
    key_t key = ftok("sender.c", 65); // generate unique key for message queue
    msgid = msgget(key, IPC_CREAT | 0666); // create message queue
    if (msgid == -1) {
        perror("msgget failed"); 
        exit(1);
    }

    printf("Message Queue created. Sender ready.\n");
    printf("Enter 'exit' to stop sending.\n");

    while (1) {
        printf("\nEnter message type (1/2/3...): ");
        if (scanf("%ld", &message.msg_type) != 1) { 
            printf("Invalid type. Try again.\n");
            while (getchar() != '\n'); // clear input buffer
            continue;
        }

        while (getchar() != '\n'); // clear leftover newline

        printf("Enter message text: ");
        fgets(message.msg_text, MAX_TEXT, stdin); // read message text
        message.msg_text[strcspn(message.msg_text, "\n")] = '\0'; // remove newline
        // strcspn is used to find the first occurrence of '\n' and replace it with '\0'

        // ✅ correct size argument
        // Send message to message queue
        if (msgsnd(msgid, &message, sizeof(message.msg_text), 0) == -1) {
            perror("msgsnd failed");
            exit(1);
        }

        // Check for exit condition
        if (strcmp(message.msg_text, "exit") == 0) {
            printf("Exit signal sent. Sender terminated.\n");
            break;
        }

        printf("Message sent successfully.\n");
    }

    return 0;
}
