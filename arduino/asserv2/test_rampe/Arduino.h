#ifndef WPROGRAM_H
#define WPROGRAM_H



#ifndef abs
#   define abs(x) (((x) > 0) ? (x) : (-(x)))
#endif


class _Serial
{
public:
    _Serial() {}
    void println(const char*);
    void println(const int);
    void println(const double);
    void print(const char*);
    void print(const int);
    void print(const double);
};

extern _Serial Serial;

#endif // WPROGRAM_H
