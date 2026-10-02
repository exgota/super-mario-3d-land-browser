extern "C" int strcmp(const char *, const char *);

extern "C" int fn_002D5918(const char **left, const char **right) {
    return strcmp(*left, *right) >= 0;
}

extern "C" int fn_002D5934(const char **left, const char **right) {
    return strcmp(*left, *right) >= 0;
}
