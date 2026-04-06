#include <stdio.h>



char *int_to_string(int val) {
  char *string_val;

  snprintf(&string_val, strlen(string_val), "%d", val);
};
