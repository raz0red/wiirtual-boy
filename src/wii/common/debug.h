#ifndef DEBUG_H
#define DEBUG_H

#ifdef __cplusplus
extern "C"
{
#endif
        void gcprintf(const char *fmt, ...);
        void gprintf(const char *fmt, ...);
        void InitGecko();

#ifdef __cplusplus
}
#endif

#endif
