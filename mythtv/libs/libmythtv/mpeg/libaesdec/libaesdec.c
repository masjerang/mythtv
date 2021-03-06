/*
 * libaesdec.c
 *
 *  Created on: Jun 22, 2014
 *      Author: root
 */

#include <stdlib.h>

#include "openssl/aes.h"

#include "libaesdec.h"

struct aes_keys_t {
   AES_KEY even;
   AES_KEY odd;
};

//  Even and Odd cw represent one full 128-bit AES key
void aes_set_control_words(void *keys, const unsigned char *ev, const unsigned char *od)
{
    AES_set_decrypt_key(ev, 128, &((struct aes_keys_t *) keys)->even);
    AES_set_decrypt_key(od, 128, &((struct aes_keys_t *) keys)->odd);
}

//-----key structure
void *aes_get_key_struct(void)
{
    struct aes_keys_t *keys = (struct aes_keys_t *) malloc(sizeof(struct aes_keys_t));
    if (keys)
    {
        static const unsigned char pk[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        aes_set_control_words(keys, pk, pk);
    }
    return keys;
}

void aes_free_key_struct(void *keys)
{
    return free(keys);
}

//----- decrypt
void aes_decrypt_packet(void *keys, unsigned char *packet)
{
    unsigned char *pkt;
    unsigned char ev_od = 0;
    int xc0, offset, n;
    int len, i;
    pkt = packet;
    AES_KEY k;

    xc0 = pkt[3] & 0xc0;
    //skip clear pkt
    if (xc0 == 0x00) 
        return;
    
    //skip reserved pkt
    if (xc0 == 0x40)
        return;
    
    // encrypted
    if (xc0 == 0x80 || xc0 == 0xc0)
    {
        //0 even, 1 odd
        ev_od = (xc0 & 0x40) >> 6; 
        // consider it decrypted now
        pkt[3] &= 0x3f;  
        // incomplete packet  TODO Check this
        if (pkt[3] & 0x20)
        { 
            offset = 4 + pkt[4] + 1;
            len = 188 - offset;
            n = len >> 3;
            if (n == 0)
                return;  
        } 
        else 
        {
            len = 184;
            offset = 4;
        }
    }
    else 
        return;

    if (ev_od == 0) 
        k = ((struct aes_keys_t *) keys)->even;
    else 
        k = ((struct aes_keys_t *) keys)->odd;

    // TODO room for improvement?
    for (i = offset; i <= (len - 16); i += 16) 
    {
        AES_ecb_encrypt(pkt + i, pkt + i, &k, AES_DECRYPT);
    }
    return;
}
