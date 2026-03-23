#include <stdio.h>
#include <stdlib.h>
#include "pager.h"
#include "page.h"
#include "data.h"
#include "fenstersql.h"
#include "parser.h"

int main() {
    struct Pager pager;
    struct BufferPool *bufferpool = malloc(1024*1024*1024);

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
