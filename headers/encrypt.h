#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#define AES_KEY_SIZE 32 // for 256 bits

unsigned char keys[AES_KEY_SIZE]; // keys for the aes encryption
unsigned char iv[AES_BLOCK_SIZE]; // init vectors

// Encrypt plaintext using AES by the ciph
int encrypt(unsigned char *plaintext, int text_len, unsigned char *key);
