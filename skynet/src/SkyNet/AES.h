/**
* Copyright (c) 
*
* @author 
*/
// AES.h
//
// from Python Crypto Tool
#if !defined(INCLUDED_AES)
#define INCLUDED_AES

#define BLOCK_SIZE 16
#define KEY_SIZE 0
#define MAXNR    14

typedef unsigned char    u8;    
typedef unsigned short    u16;    
typedef unsigned int    u32;

typedef struct block_state {
    u32 ek[ 4*(MAXNR+1) ]; 
    u32 dk[ 4*(MAXNR+1) ];
    int rounds;
} block_state;

/**
 * AES 초기화
 *
 * @return 0 성공, -1 실패
 */
int AES_init(block_state *state, const char *key, int keylen);

void AES_encrypt(block_state *self, const char *in, u8 *out);

void AES_decrypt(block_state *self, const char *in, u8 *out);


#endif // !defined(INCLUDED_AES)
