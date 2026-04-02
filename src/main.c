#include <stdio.h>
#include <stdlib.h>
#include "page.h"
#include "data.h"
#include "fenstersql.h"
#include "parser.h"
#include <signal.h>

#define LOCK_FILE "fenster.pid"

extern GlobalControl *global_control = NULL;

void create_lock_file() {
    FILE *f = fopen(LOCK_FILE, "wx"); // 'x' means fail if file already exists
    if (!f) {
        // File exists! Check if the process inside is actually alive
        // (Using kill(pid, 0) is a trick to check if a PID is active)
        printf("Error: Lock file exists. Is another instance running?\n");
        exit(1);
    }
    fprintf(f, "%d", getpid());
    fclose(f);
}

void handle_sigint(int sig) {
    printf("\nCaught signal %d. Cleaning up...\n", sig);
    
    // Perform cleanup
    if (remove(LOCK_FILE) == 0) {
        printf("Lock file removed successfully.\n");
    } else {
        perror("Error removing lock file");
    }
    
    // Handle Global Control Data


    // Now exit the program manually
    exit(0);
}

// INIT
void init(){
    printf("Initializing Database\n");
    GlobalControl gc;
    gc.next_oid = 4;  // READ FROM FILE
    gc.database_oid = 1;
    // LOAD GLOBAL CONTROL DATA INTO MEMORY
    FILE *gcf = fopen(GLOBAL_CONTROL_FILE, "wb");
    fseek(gcf, 0, SEEK_SET);
    printf("Writing globalcontrol data \n");
    fwrite(&gc, sizeof(gc), 1, gcf);
    fclose(gcf);
};

int main() {
    //////////////////////////////////////
    pid_t fenster_pid = getpid();
    create_lock_file();    
    signal(SIGINT, handle_sigint);
    ////////////////////////////////////
    init();
    BufferPool *bufferpool = malloc(BUFFERPOOL_SIZE);
    global_control = bufferpool;
    // LOAD GLOBAL CONTROL DATA INTO MEMORY
    FILE *gcf = fopen(GLOBAL_CONTROL_FILE, "rb");
    fseek(gcf, 0, SEEK_SET);
    fread(global_control, sizeof(GlobalControl), 1, gcf);
    fclose(gcf);
    Page *pages;
    pages = bufferpool + sizeof(GlobalControl);
    /* LOAD METADATA INTO PAGE 0 */

    
    char object[32];
    char value[1024];
    char action[32];

    system("figlet 'FENSTERSQL'");

    while (1) {
        printf("\nfenstersql# ");
        fflush(stdout); // Ensure prompt shows up immediately

        // Limit width to prevent buffer overflows
        if (scanf("%31s %31s %1023s", action, object, value) == 3) {
            printf("%s %s %s\n", action, object, value);
            execute(action, object, value);
        } else {
            // Clear buffer on failure to prevent infinite loops
            while(getchar() != '\n');
        }
    }
    free(bufferpool);

    return 0;
}
