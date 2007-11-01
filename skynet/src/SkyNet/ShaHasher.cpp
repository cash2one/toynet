/**
* Copyright (c) 
*
* @author 
*/
#include "stdafx.h"
#include <skynet/security/ShaHasher.h>
#include <sstream>

namespace skynet
{

#define SHA_ROTL(X,n) (((X) << (n)) | ((X) >> (32-(n))))

ShaHasher::ShaHasher()
{
    reset();
}


bool ShaHasher::hash(const char* data, int slen)
{
    if (finalized_)
        return false;

    unsigned char* dataIn = (unsigned char*)data;
    int len = (slen == -1) ? static_cast<int>(strlen(data)) : slen;

    for (int i = 0; i < len; i++) {
        W_[lenW_ / 4] <<= 8;
        W_[lenW_ / 4] |= (unsigned long)dataIn[i];
        if ((++lenW_) % 64 == 0) {

            // shaHashBlock
            {
                int t;
                unsigned long A,B,C,D,E,TEMP;

                for (t = 16; t <= 79; t++)
                    W_[t] = 
                    SHA_ROTL(W_[t-3] ^ W_[t-8] ^ W_[t-14] ^ W_[t-16], 1);

                A = H_[0];
                B = H_[1];
                C = H_[2];
                D = H_[3];
                E = H_[4];

                for (t = 0; t <= 19; t++) {
                    TEMP = SHA_ROTL(A,5) + (((C^D)&B)^D)     + E + W_[t] 
                    + 0x5a827999L;
                    E = D; D = C; C = SHA_ROTL(B, 30); B = A; A = TEMP;
                }
                for (t = 20; t <= 39; t++) {
                    TEMP = SHA_ROTL(A,5) + (B^C^D)           + E + W_[t] 
                    + 0x6ed9eba1L;
                    E = D; D = C; C = SHA_ROTL(B, 30); B = A; A = TEMP;
                }
                for (t = 40; t <= 59; t++) {
                    TEMP = SHA_ROTL(A,5) + ((B&C)|(D&(B|C))) + E + W_[t] 
                    + 0x8f1bbcdcL;
                    E = D; D = C; C = SHA_ROTL(B, 30); B = A; A = TEMP;
                }
                for (t = 60; t <= 79; t++) {
                    TEMP = SHA_ROTL(A,5) + (B^C^D)           + E + W_[t] 
                    + 0xca62c1d6L;
                    E = D; D = C; C = SHA_ROTL(B, 30); B = A; A = TEMP;
                }

                H_[0] += A;
                H_[1] += B;
                H_[2] += C;
                H_[3] += D;
                H_[4] += E;
            }

            lenW_ = 0;
        }
        sizeLo_ += 8;
        sizeHi_ += (sizeLo_ < 8);
    }
    return true;
}


std::string ShaHasher::toString()
{
    char pad0x80 = static_cast<unsigned char>(0x80);
    char pad0x00 = 0x00;
    char padlen[8];
    const int bitshifts[4] = { 24, 16, 8, 0 };

    if (! finalized_) {
        /* Pad with a binary 1 (e.g. 0x80), then zeroes, then length
        */
        padlen[0] = (unsigned char)((sizeHi_ >> 24) & 255);
        padlen[1] = (unsigned char)((sizeHi_ >> 16) & 255);
        padlen[2] = (unsigned char)((sizeHi_ >>  8) & 255);
        padlen[3] = (unsigned char)((sizeHi_ >>  0) & 255);
        padlen[4] = (unsigned char)((sizeLo_ >> 24) & 255);
        padlen[5] = (unsigned char)((sizeLo_ >> 16) & 255);
        padlen[6] = (unsigned char)((sizeLo_ >>  8) & 255);
        padlen[7] = (unsigned char)((sizeLo_ >>  0) & 255);
        hash(&pad0x80, 1);
        while (lenW_ != 56)
            hash(&pad0x00, 1);
        hash(padlen, 8);

        finalized_ = true;
    }

    /* Output hash
    */
    std::string hashbuf(40, 0);
    char* pos = &(hashbuf[0]);

    for (int i = 0; i < 20; i++) {
#ifdef WIN32
        _snprintf(pos, 3, "%02x", 
            (unsigned char)(H_[i / 4] >> bitshifts[i % 4]));
#else
        snprintf(pos, 3, "%02x", 
            (unsigned char)(H_[i / 4] >> bitshifts[i % 4]));
#endif
        pos += 2;
    }

    return hashbuf;
}


void ShaHasher::reset()
{
    lenW_   = 0;
    sizeHi_ = 0;
    sizeLo_ = 0;

    /* Initialize H with the magic constants (see FIPS180 for constants)
    */
    H_[0] = 0x67452301L;
    H_[1] = 0xefcdab89L;
    H_[2] = 0x98badcfeL;
    H_[3] = 0x10325476L;
    H_[4] = 0xc3d2e1f0L;

    for (int i = 0; i < 80; i++)
        W_[i] = 0;

    finalized_ = false;
} 

} // namespace skynet
