#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include "data.h"
#include "page.h"
#include "signals.h"

void handle_sigint(int sig) {
    printf("\nCaught signal %d. Cleaning up...\n", sig);

    // Perform cleanup
    if (remove(LOCK_FILE) == 0) {
        printf("Lock file removed successfully.\n");
    } else {
        perror("Error removing lock file");
    }

    // Handle Global Control Data
    FILE *gcf = fopen(GLOBAL_CONTROL_FILE, "w");
    fseek(gcf, 0, SEEK_SET);
    printf("Writing globalcontrol data %d\n", global_control->next_oid);
    fwrite(global_control, sizeof(global_control), 1, gcf);
    fclose(gcf);


    // Now exit the program manually
    exit(0);
}
    
