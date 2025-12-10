// Functions Lab template file
.data

.global login_name
login_name:
    .asciz "silva48"

.global puid
puid:
    .asciz "0030099891"

.balign 4

.equ TEST_SORT,         0x1
.equ TEST_STRCONCAT,    0x2
.equ TEST_FIB,          0x4
.equ TEST_BSEARCH,      0x8
.equ TEST_ALL,          (TEST_SORT | TEST_STRCONCAT | TEST_FIB | TEST_BSEARCH)

// Replace the TEST_ALL with the tests you want to run
// OR expression also support like the TEST_ALL above
// i.e. replace the TEST_ALL below with TEST_SORT | TEST_STRCONCAT
//      will test on the sort and strconcat
.global test
test:
    .word TEST_ALL


// Align to 4bytes boundary as we have a string above
.balign 4

.text
.global asm_sort_int
asm_sort_int:
    /* void asm_sort_int(int32_t* arr, uint32_t n)
     * relies on `qsort` in C standard library to 
     * sort in ascending order
     **/

    /* Enter your code after this comment */
    li x12, 4
    la x13, asm_cmp
    addi sp, sp, -4
    sw ra, 0(sp)
    jal x1, qsort

    lw ra, 0(sp)
    addi sp, sp, 4


    /* Enter your code above this comment */
    ret

.global asm_cmp
asm_cmp:
    // int asm_cmp(const void * a, const void * b)
    // Compare function to int32_t
    // used for qsort
    /* Enter your code after this comment */
    lw x16, 0(x10)
    lw x17, 0(x11)
    blt x16, x17, _less_than
    addi x10, x0, 1
    jal x0, _cmp_end

_less_than:
    addi x10, x0, -1

_cmp_end:

    /* Enter your code above this comment */
    ret

.global asm_strconcat
asm_strconcat:
    /* char* asm_strconcat(char * str1, char * str2)
     * First assign memory space with `malloc`, 
     * concatenating `str1` and `str2`, 
     * and return the resulted string.
     **/
    /* Enter your code after this comment */
    addi sp, sp, -28
    sw ra, 0(sp)
    sw x10, 4(sp) //str1
    sw x11, 8(sp) // str2

    jal x1, strlen
    //addi x12, x10, 0 //x12 is n1
    sw x10, 12(sp) // n1

    lw x10, 8(sp)
    jal x1, strlen
    
    sw x10, 16(sp) // n2

    lw x12, 12(sp)
    lw x13, 16(sp)
    add x14, x12, x13 //x14 is size
    addi x14, x14, 1
    sw x14, 20(sp) // size

    mv x10, x14
    jal x1, malloc
    sw x10, 24(sp) //buff

    lw x10, 24(sp)
    lw x11, 4(sp)
    lw x12, 12(sp)// x12 = n1
    jal x1, memcpy


    lw x10, 24(sp) 
    lw x11, 8(sp) // str2
    lw x12, 16(sp) // n2

    lw t0, 12(sp) //n1

    add x10, x10 ,t0 // buff + n1
    jal x1, memcpy


    lw x15, 24(sp) // buff

    lw x14, 20(sp) // size
    addi x16, x14, -1
    add x16, x15, x16 // buff + size - 1

    sb x0, 0(x16)

    lw x10, 24(sp)
    lw ra, 0(sp)
    addi sp, sp, 28

    /* Enter your code above this comment */
    ret

.global asm_fib
asm_fib:
    /* uint32_t asm_fib(uint32_t n)
     * accept an index term `n` and return 
     * the Fn fibonacci term (F0 = 0, F1 = 1)
     **/

    /* Enter your code after this comment */
    addi sp, sp, -12
    sw ra, 0(sp)
    sw x10, 4(sp)

    li x11, 2

    blt x10, x11, _fib_if
    addi x10, x10, -1
    jal x1, asm_fib
    sw x10, 8(sp)
    lw x10, 4(sp)   
    addi x10, x10, -2
    jal x1, asm_fib
    lw x11, 8(sp)
    add x10, x10, x11
    lw ra, 0(sp)
    addi sp, sp, 12
    ret
_fib_if:

    /* Enter your code above this comment */
    lw ra, 0(sp)
    lw x10, 4(sp)
    addi sp, sp, 12
    ret

.global asm_bsearch
asm_bsearch:
    /* int32_t asm_bsearch(int *arr, int key, 
                    int32_t start,
                    int32_t end)
     * Return the index of the element in the array
     * if the element does not exist, return -1 instead
     * arr: integer array to be searched
     * key: the element we want to search on
     * start: start index of subarray, inclusive
     * end: end index of subarray, inclusiveloop through an entire string and only convert
     **/

    /* Enter your code after this comment */
    addi sp, sp, -4
    sw ra, 0(sp)
    bgt x12, x13, _bsearch_if
    add x14, x12, x13
    srli x14, x14, 1//x14 is mid
    slli x18, x14, 2//x14 is mid
    add x15, x18, x10
    lw x16, 0(x15)
    blt x16, x11, _mid_less_than_key
    bgt x16, x11, _key_less_than_mid
    mv x10, x14
    lw ra, 0(sp)
    addi sp, sp, 4
    ret

_mid_less_than_key:
    addi x12, x14, 1
    jal x1, asm_bsearch
    lw ra, 0(sp)
    addi sp, sp, 4
    ret

_key_less_than_mid:
    addi x13, x14, -1
    jal x1, asm_bsearch
    lw ra, 0(sp)
    addi sp, sp, 4
    ret

_bsearch_if:
    li x10, -1
    lw ra, 0(sp)
    addi sp, sp, 4
    /* Enter your code above this comment */
    ret
