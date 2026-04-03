#ifndef CONNECTION_H
#define CONNECTION_H

typedef struct {
  int64_t database_oid;
  int64_t user_oid;
}
Context;
extern Context *ctx;
#endif
