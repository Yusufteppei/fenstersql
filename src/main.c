#include <stdio.h>
#include <stdlib.h>
#include "page.h"
#include "signals.h"
#include "data.h"
#include "connection.h"
#include "fenstersql.h"
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "parser.tab.h"

extern int yyparse();
extern void yy_scan_string(const char *);

extern GlobalControl *global_control = NULL;
extern Context *ctx = NULL;

void init(){
    printf("Initializing Database\n");
    global_control->next_oid = 1; 
    
    // CREATE SYSTEM TABLES
    create_system_tables();

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
    //printf("Reading globalcontrol data \n");
    fread(global_control, sizeof(GlobalControl), 1, gcf);
    printf("Global oid : %d\n", global_control->next_oid);
    fclose(gcf);

};

void create_lock_file() {
    FILE *f = fopen(LOCK_FILE, "wx"); // 'x' means fail if file already exists
    if (!f) {
        // File exists! Check if the process inside is actually alive
        //printf("File Exists\n");
        FILE *fp = fopen(LOCK_FILE, "rb"); 
        int pid = 0;
        fseek(fp, 0, SEEK_SET);
        fscanf(fp, "%d", &pid); 
        //printf("pid : %d\n", pid);
        fclose(fp);
        if (kill(pid, 0) == 0 ) {        
          // PROCESS IS ALIVE 
          printf("Another instance of fenstersql seems to be running!!!\n");
          exit(1);
        }
        else{
         // PROCESS IS DEAD
         printf("PID not active. Deleting lock file...\n");
         remove(LOCK_FILE);
         create_lock_file();
        }
    }
    else{
      fprintf(f, "%d", getpid());
      fclose(f);
    }
}

void connect(){

  ctx->database_oid = 1;
  ctx->user_oid = 9999;
  
  printf("Context    : %d\n", ctx->database_oid);
};

int main() {
    
    system("figlet 'FENSTERSQL'");
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
    
    ////////////////////////////////////
    
    Page *pages;
    pages = bufferpool + sizeof(GlobalControl);
    
    ////////////////////////////////////
    printf("pid        : %d\n", getpid()); 
    
    printf("\n\n\n");
    char *line;
    while ((line = readline("fenstersql# ")) != NULL) {
        if (*line) {
            add_history(line);      // Adds to arrow-key history
            yy_scan_string(line);   // Sends the line to Flex/Bison
            yyparse();
        }
        free(line);
    }
    
    /////////////////////////////////////

    free(pages); free(bufferpool);
    
    return 0;
    }
