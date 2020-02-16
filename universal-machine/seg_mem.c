/*seg_mem.c
 *      Jonah Feldman & Sydney Strzempko, 16 Nov 16
 *     HW6 - UM
 *
 * Contains implementation of 32-bit segmented memory module for the universal 
 * machine
 */
#include "seg_mem.h"
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

struct Seg_mem{
    Seq_T segments;
    Stack_T unused_segments;
};


/*
 * creates a New Segmented memory architecture, intialized empty, and 
 * returns a pointer to the struct containig that architecture
 */
Seg_mem new_memory()
{
        Seg_mem memory = malloc(sizeof(struct Seg_mem));

        memory->segments = Seq_new(5);

        memory->unused_segments = Stack_new();

        return memory;
}


/*
 * creates a new segment of memory and returns its bit_tag.  Initializes all 
 * words in the segment to zero
 * Arguments:
 *          memory: a struct pointer containing the segements and 
 *                  neccesary information about the memory architecture, 
 *                  and the segments
 *
 * Returns: a 32-bit unique tag identifier for the newly created segment
 */
 uint32_t map_segment(int words, Seg_mem memory)
 {
        Seq_T seg = Seq_new(words);
        Seq_T segments = memory->segments;
        Stack_T extras = memory->unused_segments;
        uint32_t tag;
        uint32_t init = 0;

        if (Stack_empty(extras)){

                Seq_addhi(segments, (void*)seg);
                tag = Seq_length(segments) - 1;
        }
        else{
                tag = (int)(uintptr_t)Stack_pop(extras);
                Seq_put(segments, tag, (void*)seg);
        }

        /* sets the inital value of all words to 0 */
        for (int i = 0; i < words; i++){
                Seq_addhi(seg, (void*)(uintptr_t)init);
        }
        return tag;
}

/*
 * Deep copies a segment onto another segment
 *
 * Arguments:
 *           dest_tag - the bit tag identifier of location we're copying to
 *           source_tag - the bit tag identifier of location we're copying from
 *           memory- a struct pointer containing the segements and 
 *                  neccesary information about the memory architecture, 
 *                  and the segments  
 */
void copy_segment( uint32_t dest_tag, uint32_t source_tag, Seg_mem memory)
{
    
        if (dest_tag == source_tag){
            return;
        }

        Seq_T segments = memory->segments;
        Seq_T source = Seq_get(segments, source_tag);
        Seq_T dest = Seq_get(segments, dest_tag);
        int source_size = Seq_length(source);
        int dest_size = Seq_length(dest);

        for (int i = 0; i < source_size; i++){
                if (i >= dest_size){
                        Seq_addhi(dest, Seq_get(source, i));
                }
                else{
                        Seq_put(dest, i, Seq_get(source, i));
                }
        }
}


/*
 * frees a segment of a memory at a given bit-tag
 * Arguments: 
 *           tag - the identifier for the segment of memory 
 *           memory: a struct pointer containing th-e segements and 
 *                  neccesary information about the memory architecture, 
 *                  and the segments          
 */
void unmap_segment(uint32_t tag, Seg_mem memory)
{
       Seq_T seg= Seq_put(memory->segments, tag, NULL);

       Seq_free(&seg);

       Stack_push(memory->unused_segments, (void*)(uintptr_t)tag);
}

/*
 * returns a word in memory from a given bit-tag and offset
 * i.e., returns $m[tag][offset]
 * 
 * Arguments:
 *           tag: the identifier of the segment we are getting the word from
 *           offset - the index of the word we are returning within the segment
 *           memory - a struct pointer containing the segements and 
 *                  neccesary information about the memory architecture, 
 *                  and the segments  
 *
 */
uint32_t get_word(uint32_t tag, int offset, Seg_mem memory)
{

        return (uintptr_t)Seq_get(Seq_get(memory->segments, tag), offset);
         
}

/*
 * sets a word in memory from a given bit-tag and offset
 * i.e.,  $m[tag][offset]= word
 * 
 * Arguments:
 *           tag - the identifier of the segment we are modifying
 *           offset - the index of the word we are modifying within the segment
 *           memory - a struct pointer containing the segements and 
 *                  neccesary information about the memory architecture, 
 *                  and the segments  
 *
 */
void set_word(uint32_t tag, int offset, uint32_t word, Seg_mem memory)
{
        Seq_T temp = Seq_get(memory->segments, tag);
        Seq_put(temp, offset, (void *)(uintptr_t)word);
}


/*
 * Frees all memory allocated by the Segmented Memory architecture
 *
 * Arguments: 
 *          memory - A pointer to a memory-struct containing Segmented memory
 */
void free_memory(Seg_mem memory)
{
        Seq_T segments = memory->segments;
        Stack_T extra = memory->unused_segments;
        int size = Seq_length(segments);

        for (int i = 0; i < size; i++){
                Seq_T temp = Seq_get(segments, i);
                /*if a mapped segment */
                if (temp != NULL){
                        Seq_free(&temp);
                }
        }

        Seq_free(&segments);
        Stack_free(&extra);
        free(memory);
}