/*um.c
 *      Jonah Feldman & Sydney Strzempko, 16 Nov 16
 *     HW6 - UM
 *
 * contains implementation for simple 14-instruction Universal Virtual Machine
 */

#include <stdlib.h>
#include <stdio.h>
#include "um.h"
#include "io.h"
#include "seg_mem.h"
#include <assert.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h>
#include "bitpack.h"
const long MAX_NUM = 4294967296;
const int REG_SIZE = 8;

struct Um{
        Seg_mem memory;
        uint32_t prog_counter;
        uint32_t *registers;
};

typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV, NAND, HALT, ACTIVATE,
        INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;


static inline void cmov(uint32_t a, uint32_t b, uint32_t c, 
                        uint32_t registers[]);
static inline void s_load(uint32_t a, uint32_t b, uint32_t c, Um um);
static inline void s_store(uint32_t a, uint32_t b, uint32_t c, Um um);
static inline void add(uint32_t a, uint32_t b, uint32_t c, 
                        uint32_t registers[]);
static inline void mul(uint32_t a, uint32_t b, uint32_t c, 
                        uint32_t registers[]);
static inline void divide(uint32_t a, uint32_t b, uint32_t c,
                        uint32_t registers[]);
static inline void nand(uint32_t a, uint32_t b, uint32_t c, 
                        uint32_t registers[]);
static inline void map_seg(uint32_t b, uint32_t c, Um um);
static inline void unmap_seg(uint32_t c, Um um);
static inline void load_prog(uint32_t b, uint32_t c, Um um);
static inline void load_val(uint32_t a, uint32_t value, uint32_t registers[]);



/*
 * creates a universal machine and allocates all neccesary memory. Initializes
 * mem[0] in the UM to a program given from a file pointer.  
 *
 * Arguments:
 *           fp - a pointer to a file that contains a um program
 *
 * Returns: a pointer to the newly created UM struct 
 * c.r.e
 *      fp == NULL
 */
Um new_Um(char *fname)
{
        FILE *fp = fopen(fname, "r");
        assert(fp);
        int bytes, words, i;
        struct stat fileStat;/* File statistics in struct */        
        uint32_t word;
        char *arr = (char *)&word;

        Um um = malloc(sizeof(struct Um));
        Seg_mem memory = new_memory();
        um->memory = memory;
        um->prog_counter = 0;
        um->registers = calloc(REG_SIZE, sizeof(uint32_t));

 
        assert(stat(fname, &fileStat) == 0);
        bytes = fileStat.st_size;/*Returns # of bytes in file - off_t type*/
        words = bytes / 4;

        map_segment(words, memory);

        /*write a loop reading in bytess and adding them to program*/
        for (i = 0; i < words; i++) {
                arr[3] = fgetc(fp);
                arr[2] = fgetc(fp);
                arr[1] = fgetc(fp);
                arr[0] = fgetc(fp);
                set_word(0, i, word, memory);
        }
        fclose(fp);
        return um;
}


/*
 * given a word from the program, unpacks the word and executes its command
 *
 * Arguments:
 *           word - a packed word containing an opcode specifying the 
 *                 the instruction, and three register indices
 *           um -  a pointer to the um struct, to be acted upon
 *
 * Returns: 0 if halt has been called, zero if otherwises        
 */
static int interpret_instruction(uint32_t word, Um um)
{
        Um_opcode opcode = (Um_opcode)Bitpack_getu(word,4, 28);
        uint32_t *registers = um->registers;
        um->prog_counter++;
        uint32_t a = Bitpack_getu(word, 3, 6);
        uint32_t b = Bitpack_getu(word, 3, 3);
        uint32_t c = Bitpack_getu(word, 3, 0);

        switch (opcode) {
                case CMOV:
                        cmov(a, b, c,registers);
                break;
                case SLOAD:
                        s_load(a, b, c, um);
                break;
                case SSTORE:
                        s_store(a, b, c, um);
                break;
                case ADD:
                        add(a, b, c, registers);
                break; 
                case MUL:
                        mul(a, b, c, registers);
                break;
                case DIV:
                        divide(a, b, c, registers);
                break;
                case NAND:
                        nand(a, b, c, registers);
                break;
                case HALT:
                        return 0;
                break;
                case ACTIVATE:
                        map_seg(b, c, um);
                break;
                case INACTIVATE:
                        unmap_seg(c, um);
                break;
                case OUT:
                        output(registers[c]);
                break;
                case IN:
                        registers[c] = input();
                break;
                case LOADP:
                        load_prog(b, c, um);
                break;
                case LV:
                        a = Bitpack_getu(word, 3, 25);
                        b = Bitpack_getu(word, 25, 0);
                        load_val(a, b, registers);
                break;
        }
        return 1; 
}

/*
 * Runs the loaded program in the UM
 * 
 * Arguments:
 *           um - a pointer to a um struct, contains preloaded program at
 *           spot zero in memory
 */
void run_Um(Um um)
{
        int run = 1;
        while (run){
          uint32_t word = get_word(0, um->prog_counter, um->memory);
          run = interpret_instruction(word, um);

        }
          
                
}


/*
 * de-allocates all memory allocated in um, as well as freeing the struct
 * itself
 *
 * Arguments:
 *           um - a pointer to a UM struct 
 */
void free_Um(Um um)
{
        free_memory(um->memory);
        free(um->registers);
        free(um);
}

/*
 * if a conditon is met, copies a value from one register onto another 
 * register
 *
 *
 * Arguments:
 *           a - index of register recieving value
 *           b - index of register that is source of value
 *           c - index of register that determines if value is moved
 *           registers - an array containing 8 registers
 */
static inline void cmov(uint32_t a, uint32_t b, uint32_t c, 
                        uint32_t registers[])
{
        if (registers[c]){
                registers[a] = registers[b];
        }
}


/*
 * copies a word from a segment onto a register
 *
 * Arguments:
 *           a - index of register that will recieve the word
 *           b - index of register that contains the bit tag of the segment
 *               we are copying from
 *           c - index of register that contains the index of the word we are 
 *                copying from a segment
 *           um - the um struct containing all neccesry data for the UM
 */
static inline void s_load(uint32_t a, uint32_t b, uint32_t c, Um um)
{
       uint32_t *registers = um->registers;
       uint32_t tag = registers[b];
       uint32_t offset = registers[c];

       registers[a] = get_word(tag, offset, um->memory);
}


/*
 * copies a value into a spot on a segment in memory
 *
 * Arguments:
 *           a - index of register that contains the bit tag of the segment
 *                we are copying into
 *           b - index of register that contains the index in the segment
 *                we are copying into
 *           c - index of register the value we are copying from
 *           um - the um struct containing all neccesry data for the UM
 */
static inline void s_store(uint32_t a, uint32_t b, uint32_t c, Um um)
{
       uint32_t *registers = um->registers;
       uint32_t tag = registers[a];
       uint32_t offset = registers[b];
       uint32_t value = registers[c];
    
       set_word(tag, offset, value, um->memory); 

}

/*
 * adds two registers together and places the value in a third register
 *
 * Arguments:
 *           a - index of the register we are placing the new value in
 *           b - index of register we are adding
 *           c - index of register we are adding
 *           registers - an array containing 8 registers
 */
static inline void add(uint32_t a, uint32_t b, uint32_t c, 
                        uint32_t registers[])
{
        registers[a] = (registers[b] + registers[c]) % MAX_NUM;
}

/*
 * multiplies two registers together and places the value in a third register
 *
 * Arguments:
 *           a - index of the register we are placing the new value in
 *           b - index of register we are multiplying
 *           c - index of register we are multiplying
 *           registers - an array containing 8 registers
 */
static inline void mul(uint32_t a, uint32_t b, uint32_t c, 
                        uint32_t registers[])
{
        registers[a] = (registers[b] * registers[c]) % MAX_NUM;
}


/*
 * divides a registers by another and places the result in a third register
 *
 * Arguments:
 *           a - index of the register we are placing the new value in
 *           b - index of register containing the dividence
 *           c - index of register containing the divisor
 *           registers - an array containing 8 registers
 */
static inline void divide(uint32_t a, uint32_t b, uint32_t c, 
                        uint32_t registers[])
{
        registers[a] = registers[b] / registers[c];
}


/*
 * performs a NAND operation on two registers and places the result in a 
 * third register
 *
 * Arguments:
 *           a - index of the register we are placing the new value in
 *           b - index of register whose value we are NANDing
 *           c - index of register whose vlaue we are NANDing
 *           registers - an array containing 8 registers
 */
static inline void nand(uint32_t a, uint32_t b, uint32_t c, 
                        uint32_t registers[])
{
      
        registers[a] = ~(registers[b] &registers[c]);
}


/*
 * maps a new segment and updates a register with the adress of the new segment
 *
 * Arguments:
 *           b - index of register we are storing memory adress in
 *           c - index of register that contains the number of words in the 
 *               new segment 
 *          um - the um struct containing all neccesry data for the UM
 */
static inline void map_seg(uint32_t b, uint32_t c, Um um)
{
       uint32_t *registers= um->registers;
       registers[b] = map_segment(registers[c], um->memory); 

}

/*
 * maps a new segment and updates a register with the adress of the new segment
 *
 * Arguments:
 *           c - index of register that contains the bit tag of the segment 
 *               we are unmapping
 *           um - the um struct containing all neccesry data for the UM
 */
static inline void unmap_seg(uint32_t c, Um um)
{
       uint32_t *registers = um->registers;
 
       unmap_segment(registers[c], um->memory); 

}

/*
 * loads a new program from a certain segment and updates the program counter
 *
 * Arguments:
 *          b - the index of the register contaning the bit tag of the segment
 *              we are getting the new program from
 *          c - what the new program counter will be set equal to, i.e. 
 *              the index at which word in the segment the program will start
 *          um - the um struct containing all neccesry data for the UM
 */
static inline void load_prog(uint32_t b, uint32_t c, Um um)
{
        uint32_t *registers = um->registers;
        copy_segment(0, registers[b], um->memory);
        um->prog_counter = registers[c];
}

/*
 * loads a a given value into a register
 *
 * Arguments:
 *          a - the index of the register we are placing the value into
 *          value - the new value we are placing into the register
 *          um - the um struct containing all neccesry data for the UM
 */
static inline void load_val(uint32_t a, uint32_t value, uint32_t registers[])
{
        registers[a] = value;
}
