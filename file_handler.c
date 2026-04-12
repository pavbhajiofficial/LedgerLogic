#include "file_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int load_transactions(RawTransaction** out) {
    FILE* fp = fopen(LEDGER_FILE, "r");
    if (!fp) {
        /* File may not exist yet – that's fine */
        *out = NULL;
        return 0;
    }

    int capacity = 16;
    int count    = 0;
    RawTransaction* arr = (RawTransaction*)malloc(capacity * sizeof(RawTransaction));
    if (!arr) { fclose(fp); return -1; }

    RawTransaction tmp;
    while (fscanf(fp, "%d %15s %63s %lf",
                  &tmp.id, tmp.date, tmp.category, &tmp.amount) == 4) {
        if (count == capacity) {
            capacity *= 2;
            RawTransaction* bigger =
                (RawTransaction*)realloc(arr, capacity * sizeof(RawTransaction));
            if (!bigger) { free(arr); fclose(fp); return -1; }
            arr = bigger;
        }
        arr[count++] = tmp;
    }

    fclose(fp);
    *out = arr;
    return count;
}

int save_transactions(const RawTransaction* txns, int count) {
    FILE* fp = fopen(LEDGER_FILE, "w");
    if (!fp) return -1;

    for (int i = 0; i < count; i++) {
        if (fprintf(fp, "%d %s %s %.2f\n",
                    txns[i].id, txns[i].date,
                    txns[i].category, txns[i].amount) < 0) {
            fclose(fp);
            return -1;
        }
    }

    fclose(fp);
    return 0;
}

void free_raw_transactions(RawTransaction* txns) {
    free(txns);
}
