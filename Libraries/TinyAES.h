#ifndef _AES_H_
#define _AES_H_

#include <stdint.h>
#include <stddef.h>

#define AES256          1
#define AES_BLOCKLEN    16 
#define AES_KEYLEN      32
#define AES_KEYEXP_SIZE 240

struct AES_ctx
{
    uint8_t RoundKey    [AES_KEYEXP_SIZE];
    uint8_t Iv          [AES_BLOCKLEN];
};

void AES_init_ctx_iv(struct AES_ctx* ctx, const uint8_t* key, const uint8_t* iv);
void AES_CBC_encrypt_buffer(struct AES_ctx* ctx, uint8_t* buf, size_t length);
void AES_CBC_decrypt_buffer(struct AES_ctx* ctx, uint8_t* buf, size_t length);

#endif // _AES_H_