/*seg_mem.h
 *      Jonah Feldman & Sydney Strzempko, 16 Nov 16
 *     HW6 - UM
 *
 * Contains interface of 32-bit segmented memory module for the universal 
 * machine
 */

 #include <stdint.h>
 #include <seq.h>
 #include <stack.h>
typedef struct Seg_mem *Seg_mem;


/*
 * creates a New Segmented memory architecture, intialized empty, and 
 * returns a pointer to the struct containig that architecture
 */
Seg_mem new_memory();

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
 uint32_t map_segment( int words, Seg_mem memory);

/*
 * frees a segment of a memory at a given bit-tag
 * Arguments: 
 *           tag - the identifier for the segment of memory 
 *           memory: a struct pointer containing th-e segements and 
 *                  neccesary information about the memory architecture, 
 *                  and the segments          
 */

 void unmap_segment(uint32_t tag, Seg_mem memory);


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
void copy_segment( uint32_t dest_tag, uint32_t source_tag, Seg_mem memory);



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
uint32_t get_word(uint32_t tag, int offset, Seg_mem memory);

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
void set_word(uint32_t tag, int offset, uint32_t word, Seg_mem memory);


/*
 * Frees all memory allocated by the Segmented Memory architecture
 *
 * Arguments: 
 *          memory - A pointer to a memory-struct containing Segmented memory
 */
void free_memory(Seg_mem memory);