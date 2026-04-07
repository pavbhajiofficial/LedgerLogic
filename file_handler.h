#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "transaction.h"

#ifdef __cplusplus
extern "C" {
#endif

void loadFromFile();
void saveToFile();
void appendTransactionToFile(const Transaction* t);

#ifdef __cplusplus
}
#endif

#endif
