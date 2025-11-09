#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>

#define MAX_TEXT 100

struct msg_buffer {
    long msg_type;
    char msg_text[MAX_TEXT];
};

int main() {
    struct msg_buffer message;
    int msgid;
    key_t key = ftok("sender.c", 65);

    msgid = msgget(key, 0666 | IPC_CREAT); 
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    printf("Receiver ready. Waiting for messages...\n");

    while (1) {
        // ✅ correct size argument
        if (msgrcv(msgid, &message, sizeof(message.msg_text), 0, 0) == -1) {
            perror("msgrcv failed");
            exit(1);
        }

        if (strcmp(message.msg_text, "exit") == 0) {
            printf("\nExit message received. Receiver terminated.\n");
            msgctl(msgid, IPC_RMID, NULL); // cleanup
            break;
        }
        // Convert message text to uppercase
        for (int i = 0; message.msg_text[i] != '\0'; i++) {
            message.msg_text[i] = toupper(message.msg_text[i]);
        }

        printf("Received (type %ld): %s\n", message.msg_type, message.msg_text);
    }

    return 0;
}
