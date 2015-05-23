/*
=========================================================================================================================
|  Implementation of SIMON 32/64 in 32 Rounds. Please checkout the following link for more information on SIMON cipher. |
|  https://eprint.iacr.org/2013/404.pdf                                                                                 |
=========================================================================================================================
Copyright (c) 2015 Milad Khandouzy (m.khandouzy@smartarts.ir) and Nima Azizzadeh (n.azizzadeh@smartarts.ir)
Special Thanks to Dr. Nasour Bagheri

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

*/

#include <stdint.h>
#include <stdio.h>
#include "Functions.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint64_t u64;
typedef uint32_t u32;

static u8 z[62] =
{1,1,0,1,1,0,1,1,1,0,1,0,1,1,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,1,0,0,1,1,1,0,0,1,1,0,1,0,0,0,0,1,1,1,1};

void KeyExpansion ( u16 k[] )
{
    u8 i;
    u16 tmp;
    for ( i=4 ; i<32 ; i++ )
    {
        tmp = ROTATE_RIGHT_16(k[i-1],3);
        tmp = tmp ^ k[i-3];
        tmp = tmp ^ ROTATE_RIGHT_16(tmp,1);
        k[i] = ~k[i-4] ^ tmp ^ z[i-4] ^ 3;
    }
}

void Encrypt ( u16 text[], u16 crypt[], u16 key[] )
{
    u8 i;
    u16 tmp;
    crypt[0] = text[0];
    crypt[1] = text[1];

    for ( i=0 ; i<32 ; i++ )
    {
        tmp = crypt[0];
        crypt[0] = crypt[1] ^ ((ROTATE_LEFT_16(crypt[0],1)) & (ROTATE_LEFT_16(crypt[0],8))) ^ (ROTATE_LEFT_16(crypt[0],2)) ^ key[i];
        crypt[1] = tmp;
    }
}

void Decrypt ( u16 text[], u16 crypt[], u16 key[] )
{
    u8 i;
    u32 tmp;
    crypt[0] = text[0];
    crypt[1] = text[1];

    for ( i=0 ; i<32 ; i++ )
    {
        tmp = crypt[1];
        crypt[1] = crypt[0] ^ ((ROTATE_LEFT_16(crypt[1],1)) & (ROTATE_LEFT_16(crypt[1],8))) ^ (ROTATE_LEFT_16(crypt[1],2)) ^ key[31-i];
        crypt[0] = tmp;
    }
}

int main ()
{

    u16 text[2];
    text[0] = 0x6f72;
    text[1] = 0x6e69;
    u16 crypt[2] = {0};
    u16 k[32];
	k[3] = 0x1b1a;
    k[2] = 0x1312;
    k[1] = 0x0b0a;
    k[0] = 0x0302;
	char input;
	
	
    KeyExpansion ( k );
    Encrypt ( text, crypt, k );
    printf("%x %x\n%x %x\n\n\n", text[0], text[1], crypt[0], crypt[1]);
	
    KeyExpansion ( k );
    Decrypt ( crypt, text, k );
    printf("%x %x\n%x %x\n\n\n", text[0], text[1], crypt[0], crypt[1]);
	printf("Press ENTER To Exit");
	getchar();
    return 0;
}


