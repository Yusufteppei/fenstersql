#include <stdio.h>
#include <stdlib.h>
#include "page.h"
#include "data.h"
#include "connection.h"
#include "fenstersql.h"
#include "parser.h"
#include <signal.h>

#define LOCK_FILE "fenster.pid"

extern GlobalControl *global_control = NULL;
extern Context *ctx = NULL;

void init(){
    printf("Initializing Database\n");
    global_control->next_oid = 1; 
    
    // CREATE INITIAL DATABASE

    // WRITE INITIAL GLOBAL CONTROL DATA INTO FILE
    FILE *gcf = fopen(GLOBAL_CONTROL_FILE, "wb");
    fseek(gcf, 0, SEEK_SET);
    printf("Writing globalcontrol data \n");
    fwrite(global_control, sizeof(global_control), 1, gcf);
    fclose(gcf);
};


void load_global_control(){
    // LOAD GLOBAL CONTROL DATA INTO MEMORY
    FILE *gcf = fopen(GLOBAL_CONTROL_FILE, "rb");
    fseek(gcf, 0, SEEK_SET);
    printf("Reading globalcontrol data \n");
    fread(global_control, sizeof(GlobalControl), 1, gcf);
    printf("Global oid : %d\n", global_control->next_oid);
    fclose(gcf);

};

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
    FILE *gcf = fopen(GLOBAL_CONTROL_FILE, "wb");
    fseek(gcf, 0, SEEK_SET);
    printf("Writing globalcontrol data %d\n", global_control->next_oid);
    fwrite(global_control, sizeof(global_control), 1, gcf);
    fclose(gcf);


    // Now exit the program manually
    exit(0);
}


void connect(){
  //Context ctx;
  ctx->database_oid = 1;
  ctx->user_oid = 9999;
  
  printf("Context : %d\n", ctx->database_oid);
};

int main() {
    //////////////////////////////////////
    pid_t fenster_pid = getpid();
    create_lock_file();    
    signal(SIGINT, handle_sigint);
    ////////////////////////////////////

    BufferPool *bufferpool = malloc(BUFFERPOOL_SIZE);
    global_control = bufferpool;
    ctx = malloc(sizeof(Context));
    //init();
    load_global_control();
    connect();
    
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
        printf("\n\n\n\n\n\n\n\n\nfenstersql# ");
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
