#ifndef WPROGRAM_H
#define WPROGRAM_H

#include <QString>


#ifndef abs
#   define abs(x) (((x) > 0) ? (x) : (-(x)))
#endif


class Serial
{
public:
    Serial() {}
    void println(const char*) {}
    void println(const QString &) {}
    void println(const int) {}
    void println(const double) {}
    void print(const char*) {}
    void print(const QString &) {}
    void print(const int) {}
    void print(const double) {}
};

Serial Serial;


#endif // WPROGRAM_H
