#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "ed25519/ed25519.h"
#define DUMP(s,m)				\
    for(int n=0;n<m;n++)			\
      printf("%02x", s[n]),			\
	printf((n+1)%32?" ":"\n");
int main() {
    unsigned char public_key[32], private_key[64], seed[32], scalar[32];
    unsigned char other_public_key[32], other_private_key[64];
    unsigned char shared_secret[32], other_shared_secret[32];
    unsigned char signature[64];
    int i;
    const unsigned char message[] = "Hello, world!";
    const int message_len = strlen((char*) message);
    /* create a random seed, and a keypair out of that seed */
    ed25519_create_seed(seed);
    printf(" * seed\n");
    DUMP(seed, 32);

    ed25519_create_keypair(public_key, private_key, seed);
    printf(" * private key\n");
    DUMP(private_key, 64);
    printf(" * public key\n");
    DUMP(public_key, 32);

    /* create signature on the message with the keypair */
    ed25519_sign(signature, message, message_len, public_key, private_key);
    printf(" * signature\n");
    DUMP(signature, 64);

    /* verify the signature */
    if (ed25519_verify(signature, message, message_len, public_key)) {
        printf("valid signature\n");
    } else {
        printf("invalid signature\n");
    }

    /* create scalar and add it to the keypair */
    ed25519_create_seed(scalar);
    printf(" * scalar\n");
    DUMP(scalar, 32);

    ed25519_add_scalar(public_key, private_key, scalar);
    printf(" * private key\n");
    DUMP(private_key, 64);
    printf(" * public key\n");
    DUMP(public_key, 32);

    /* create signature with the new keypair */
    ed25519_sign(signature, message, message_len, public_key, private_key);
    printf(" * signature\n");
    DUMP(signature, 64);

    /* verify the signature with the new keypair */
    if (ed25519_verify(signature, message, message_len, public_key)) {
        printf("valid signature\n");
    } else {
        printf("invalid signature\n");
    }

    /* make a slight adjustment and verify again */
    signature[44] ^= 0x10;
    if (ed25519_verify(signature, message, message_len, public_key)) {
        printf("did not detect signature change\n");
    } else {
        printf("correctly detected signature change\n");
    }

    /* generate two keypairs for testing key exchange */
    ed25519_create_seed(seed);
    printf(" * seed\n");
    DUMP(seed, 32);
    ed25519_create_keypair(public_key, private_key, seed);
    printf(" * private key\n");
    DUMP(private_key, 64);
    printf(" * public key\n");
    DUMP(public_key, 32);
    ed25519_create_seed(seed);
    DUMP(seed, 32);
    printf(" * other seed\n");
    ed25519_create_keypair(other_public_key, other_private_key, seed);
    printf(" * other private key\n");
    DUMP(other_private_key, 64);
    printf(" * other public key\n");
    DUMP(other_public_key, 32);

    /* create two shared secrets - from both perspectives - and check if they're equal */
    ed25519_key_exchange(shared_secret, other_public_key, private_key);
    ed25519_key_exchange(other_shared_secret, public_key, other_private_key);
    printf(" * shared secret\n");
    DUMP(shared_secret, 32);
    printf(" * other shared secret\n");
    DUMP(other_shared_secret, 32);

    for (i = 0; i < 32; ++i) {
        if (shared_secret[i] != other_shared_secret[i]) {
            printf("key exchange was incorrect\n");
            break;
        }
    }
    if (i == 32) {
        printf("key exchange was correct\n");
    }
}
