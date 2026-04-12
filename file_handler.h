#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#define LEDGER_FILE "ledger.txt"
#define MAX_CATEGORY_LEN 64
#define MAX_DATE_LEN     16

typedef struct {
    int    id;
    char   date[MAX_DATE_LEN];
    char   category[MAX_CATEGORY_LEN];
    double amount;
} RawTransaction;

/* Returns number of records read, -1 on error.
   Caller must free(*out) with free_raw_transactions(). */
int  load_transactions(RawTransaction** out);

/* Returns 0 on success, -1 on error. */
int  save_transactions(const RawTransaction* txns, int count);

/* Frees memory allocated by load_transactions. */
void free_raw_transactions(RawTransaction* txns);

#ifdef __cplusplus
}
#endif
