#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sha1.h"

void sha1_pad_message(
  const unsigned char *message,
  uint32_t length,
  unsigned char *padded_message,
  uint32_t *padded_length
);

void sha1_process_chunk(const unsigned char *chunk, uint32_t *hash);

/* Constants used by SHA-1 algorithm */
#define H0 0x67452301
#define H1 0xEFCDAB89
#define H2 0x98BADCFE
#define H3 0x10325476
#define H4 0xC3D2E1F0

/* SHA-1 specific constants */
#define K0 0x5A827999
#define K1 0x6ED9EBA1
#define K2 0x8F1BBCDC
#define K3 0xCA62C1D6

/**
 * SHA-1 requires that you pad the message to a length that is congruent to 448
 * bits modulo 512. Then, append the original message length (in bits) as a 64-
 * bit big-endian integer. This step ensures that the total message length is a
 * multiple of 512 bits.
 */
void sha1_pad_message(
  const unsigned char *message,
  uint32_t length,
  unsigned char *padded_message,
  uint32_t *padded_length
) {
  uint32_t i;
  uint32_t original_bits = length * 8;
  
  /* Copy the message into the padded_message array: */
  memcpy(padded_message, message, length);
  
  /* Append the bit '1' to the message: */
  padded_message[length] = 0x80;
  
  /* Append zeroes till the length is congruent to 448 modulo 512: */
  for (i = length + 1; i < ((length + 64) & ~63) - 8; i++) {
    padded_message[i] = 0;
  }
  
  /* Append the original message length as a 64-bit big-endian integer: */
  for (i = 0; i < 8; i++) {
    padded_message[((length + 64) & ~63) - 8 + i] = (original_bits >> (56 - 8 * i)) & 0xFF;
  }
  
  /* Ensure 512-bit multiple length: */
  *padded_length = (length + 64) & ~63;
}

/**
 * The main SHA-1 compression function
 *
 * This is where the logic of the SHA-1 algorithm happens. We need to divide
 * the padded message into 512-bit chunks, process each chunk, and update the
 * hash values.
 */
void sha1_process_chunk(const unsigned char *chunk, uint32_t *hash) {
  uint32_t w[80];
  uint32_t a, b, c, d, e, f, k, temp;
  int t;
  
  /* Prepare the message schedule (words): */
  for (t = 0; t < 16; t++) {
    w[t] = (chunk[t*4] << 24) | (chunk[t*4+1] << 16) | (chunk[t*4+2] << 8) | (chunk[t*4+3]);
  }
  
  for (t = 16; t < 80; t++) {
    w[t] = (w[t-3] ^ w[t-8] ^ w[t-14] ^ w[t-16]);
    /* Left rotate by 1 bit: */
    w[t] = (w[t] << 1) | (w[t] >> 31);
  }
  
  /* Initialise working variables: */
  a = hash[0];
  b = hash[1];
  c = hash[2];
  d = hash[3];
  e = hash[4];
  
  /* Main loop: */
  for (t = 0; t < 80; t++) {
    if (t < 20) {
      f = (b & c) | (~b & d);
      k = K0;
    } else
    if (t < 40) {
      f = b ^ c ^ d;
      k = K1;
    } else
    if (t < 60) {
      f = (b & c) | (b & d) | (c & d);
      k = K2;
    } else {
      f = b ^ c ^ d;
      k = K3;
    }
    temp = ((a << 5) | (a >> 27)) + f + e + k + w[t];
    e = d;
    d = c;
    /* Left rotate by 30 bits: */
    c = (b << 30) | (b >> 2);
    b = a;
    a = temp;
  }
  
  /* Add the working variables back into hash: */
  hash[0] += a;
  hash[1] += b;
  hash[2] += c;
  hash[3] += d;
  hash[4] += e;
}

/**
 * Combining padding and chunk processing to compute the SHA-1 hash.
 */
void sha1(const unsigned char *message, uint32_t length, uint32_t *digest) {
  /* Calculate the padded length (multiple of 64): */
  uint32_t i;
  uint32_t padded_length = (length + 64) & ~63;
  
  unsigned char *padded_message = (unsigned char*)malloc(padded_length);
  if (padded_message == NULL) {
    /* Handle memory allocation failure: */
    printf("Memory allocation failure.\n");
    return;
  }
  
  /* Initial hash values: */
  digest[0] = H0;
  digest[1] = H1;
  digest[2] = H2;
  digest[3] = H3;
  digest[4] = H4;
  
  /* Pad the message: */
  sha1_pad_message(message, length, padded_message, &padded_length);
  
  /* Process each 512-bit chunk: */
  for (i = 0; i < padded_length; i += 64) {
    sha1_process_chunk(&padded_message[i], digest);
  }
  
  /* Free dynamically allocated memory for padded_message: */
  free(padded_message);
}

int main() {
  const char *message = "abc";
  uint32_t digest[5];
  
  sha1((unsigned char*)message, strlen(message), digest);
  
  printf("SHA-1: %08X %08X %08X %08X %08X\n", digest[0], digest[1], digest[2], digest[3], digest[4]);
  
  return 0;
}