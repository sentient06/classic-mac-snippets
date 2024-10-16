#ifndef SHA1_H
#define SHA1_H

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long uint32_t;
void sha1(const unsigned char *message, uint32_t length, uint32_t *digest);

#ifdef __cplusplus
}
#endif

#endif