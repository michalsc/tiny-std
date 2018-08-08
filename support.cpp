#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "support.h"

void * AllocMem(int size, int type)
{
    int *ptr = reinterpret_cast<int *>(malloc(size + 32));
    ptr[0] = size;
    ptr[1] = 0xdeadbeef;
    ptr[2] = 0xdeadbeef;
    ptr[3] = 0xdeadbeef;

    ptr[4 + size/4] = 0xcafebabe;
    ptr[5 + size/4] = 0xcafebabe;
    ptr[6 + size/4] = 0xcafebabe;
    ptr[7 + size/4] = 0xcafebabe;

    if (type & MEMF_CLEAR)
    {
        bzero(&ptr[4], size);
    }

    return &ptr[4];
}

void FreeMem(void *ptr, int size)
{
    unsigned int *p = reinterpret_cast<unsigned int *>(ptr);

    p -= 4;
    if (*p != (unsigned int)size)
        printf("Size mismatch at FreeMem!! %d != %d\n", *p, size);

    if (p[1] != 0xdeadbeef || p[2] != 0xdeadbeef || p[3] != 0xdeadbeef)
    {
        printf("FreeMem(): left wall damaged %08x%08x%08x\n", p[1], p[2], p[3]);
    }
    if (p[4 + size/4] != 0xcafebabe || p[5+size/4] != 0xcafebabe || p[6+size/4] != 0xcafebabe || p[7+size/4] != 0xcafebabe)
    {
        printf("FreeMem(): right wall damaged %08x%08x%08x%08x\n", p[4 + size/4], p[5+size/4], p[6+size/4],p[7+size/4]);
    }
    
    free(p);
}

void CopyMem(const void *src, void *dst, int size)
{
    memmove(dst, src, size);
}

void SetMem(void *dst, int size, char fill)
{
    memset(dst, (int)fill, size);
}

int _strcmp_(const char *str1, const char *str2)
{
    const char * s1 = str1;
    const char * s2 = str2;
    unsigned char c1;
    unsigned char c2;
    do {
        c1 = *s1++;
        c2 = *s2++;

        if (c1 == 0)
            break;
    } while (c1 == c2);

    return static_cast<unsigned char>(c1) - static_cast<unsigned char>(c2);
}