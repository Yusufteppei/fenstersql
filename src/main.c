#include <stdio.h>
#include <stdlib.h>
#include "page.h"
#include "data.h"
#include "fenstersql.h"
#include "parser.h"


extern GlobalControl *global_control = NULL;
 
// INIT
void init(){
    printf("Initializing Database\n");
    GlobalControl gc;
    gc.magic_number = FSTR_MAGIC;
    gc.next_oid = 1;
    // LOAD GLOBAL CONTROL DATA INTO MEMORY
    FILE *gcf = fopen(GLOBAL_CONTROL_FILE, "wb");
    fseek(gcf, 0, SEEK_SET);
    printf("Writing magic number %d\n", sizeof(gc.magic_number));
    fwrite(&gc, sizeof(gc), 1, gcf);
    fclose(gcf);
};

int main() {
    init();
    global_control = malloc(sizeof(GlobalControl));
    struct BufferPool *bufferpool = malloc(1024*1024*1024);
    // LOAD GLOBAL CONTROL DATA INTO MEMORY
    FILE *gcf = fopen(GLOBAL_CONTROL_FILE, "rb");
    fseek(gcf, 0, SEEK_SET);
    fread(global_control, sizeof(GlobalControl), 1, gcf);

    Page *pages;
    pages = bufferpool;
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
