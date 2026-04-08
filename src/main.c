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

extern BufferPool *bufferpool = NULL;
extern Context *ctx = NULL;
extern PageTable *page_table = NULL;
//extern Column *columns = NULL;
extern TableMetadata *tables_metadata = NULL;

void init(){
    printf("Initializing Database\n");
    bufferpool->global_control.next_oid = 1; 
    
    // CREATE SYSTEM TABLES
    create_system_tables();

    // CREATE INITIAL DATABASE

    // WRITE INITIAL GLOBAL CONTROL DATA INTO FILE
    FILE *gcf = fopen(GLOBAL_CONTROL_FILE, "wb");
    fseek(gcf, 0, SEEK_SET);
    printf("Writing Global Control Data \n");
    fwrite(&bufferpool->global_control, sizeof(GlobalControl), 1, gcf);
    fclose(gcf);
};


void load_global_control(){
    // LOAD GLOBAL CONTROL DATA INTO MEMORY
    FILE *gcf = fopen(GLOBAL_CONTROL_FILE, "rb");
    fseek(gcf, 0, SEEK_SET);
    fread(&bufferpool->global_control, sizeof(GlobalControl), 1, gcf);
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
  
};

int main() {
    
    //////////////     ALLOCATE MEMORY   //////////////////////
    bufferpool = malloc(BUFFERPOOL_SIZE);
    page_table = malloc(4*1024); // 4KB PAGE TABLE
    tables_metadata = malloc(16*1024); //16B PER METADATA
    ctx = malloc(sizeof(Context));
    
    //////////////////////////////////////////////////////////
    
    //////////////   LOAD GLOBAL STATES  /////////////////////
    create_lock_file();    
    signal(SIGINT, handle_sigint);
    //init();
    load_global_control();
    load_tables_metadata();
    connect();
    
    //////////////////////////////////////////////////////////

    //////////////     INTRO OUTPUT       ////////////////////
    system("figlet 'FENSTERSQL'");
    fflush(stdout);
    
    printf("Column Count    : %d\n", tables_metadata[2].column_count);
    //printf("Column 1        : %d\n", tables_metadata->columns[0].column_oid);
    printf("Process ID      : %d\n", getpid());     
    printf("Context         : %d\n", ctx->database_oid);
    printf("Next Global OID : %d\n", bufferpool->global_control.next_oid);
    printf("\n\n\n");

    /////////////////////////////////////////////////////////

    //////////////      INPUT PARSING     //////////////////////
    char *line;
    while ((line = readline("fenstersql# ")) != NULL) {
        if (*line) {
            add_history(line);      
            yy_scan_string(line);   
            yyparse();
        }
        free(line);
    }

    //////////////////////////////////////////////////////////
    
    ///////////////     FREE MEMORY     //////////////////////
    free(bufferpool);
    free(page_table);    
    free(tables_metadata);

    /////////////////////////////////////////////////////////

    return 0;
    }
