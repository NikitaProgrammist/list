#ifdef CHECK

#ifndef CHECK_LIST_H
#define CHECK_LIST_H

#define listVerify(list, arg) {                                                \
    if (verify(list, __FILE__, __func__, __LINE__, arg)) {                     \
        printErrorLines("../" __FILE__, __PRETTY_FUNCTION__, __LINE__);        \
        return CHECK_FAILED;                                                   \
    }                                                                          \
}

#define checkStatement(arg) {                                                  \
    if (arg) {                                                                 \
        colorPrintf(RED, BOLD, "\nList died, because: " #arg "\n");            \
        return INVALID;                                                        \
    }                                                                          \
}

#define myAssert(arg) {                                                        \
    if (!(arg)) {                                                              \
        colorPrintf(RED, BOLD, "\nAssertion failed: " #arg "\n");              \
        printErrorLines("../" __FILE__, __PRETTY_FUNCTION__, __LINE__);        \
        return ASSERTED_S;                                                     \
    }                                                                          \
}

enum Errors {
  VALID,
  INVALID
};

Errors verify(const List * list, const char * file, const char * func, const size_t len, const char * position);
Errors checkCycle(const List * list);
void dump(const List * list, const char * file, const char * func, const size_t len, const char * position);
void createGraph(const List * list, size_t picture_number);
void printErrorLines(const char * file, const char * func, size_t line_number);

#endif // CHECK_LIST_H


#else

#define listVerify(list, arg)
#define myAssert(arg)
#define dump(arg)

#endif // CHECK
