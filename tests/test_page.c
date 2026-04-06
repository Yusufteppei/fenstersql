#include <stdio.h>
#include <stdlib.h>


#define PAGE_STATUS_FREE    (1 << 0)
#define PAGE_STATUS_OCCUPIED (1 << 1)
#define PAGE_STATUS_LOADING (1 << 2)

int main() {
  printf("%8d\n", PAGE_STATUS_LOADING & PAGE_STATUS_OCCUPIED);  
return 0;
};
