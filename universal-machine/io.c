/*
 * Jonah Feldman and Sydney Strzempko
 * 11-13-16
 *
 * io.h: implementation for input and output for univeral machine
 */

#include "io.h"
#include <stdio.h>
#include <stdlib.h>

void output(int val)
{
        putchar(val);
}

uint32_t input()
{
        return getchar();

}