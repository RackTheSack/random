// Control Flow Lab template file
.data
.global lowercase_string
lowercase_string:
    // Specify a string ending with '\0'
    .asciz "ece 362 is awesome!"

.global login_name
login_name:
    .asciz "silva48"

.global puid
puid:
    .asciz "0030099891"

// Align to 4bytes boundary as we have a string above
.balign 4

.text
.global asm_strlen
asm_strlen:
    /* uint32_t asm_strlen(char *str)
     * Take in a pointer to a string and return
     * its length up to the null terminating
     * character `\0`
    **/   

    /* Enter your code after this comment */
    _if:
        beq x0,x10, _then
        li x11, 0
        sub x12, x12, x12
        jal x0, _while


    _then:
        sub x10, x10, x10
        jal x0,_end

    _while:
        lb x11, 0(x10)
        beq x0,x11, _end_while
        addi x10, x10, 1
        addi x12, x12, 1
        jal x0, _while

    _end_while:
        sub x10, x10, x10
        add x10, x10, x12
    
    _end:
    /* Enter your code above this comment */
    ret

.global asm_fib
asm_fib:
    /* void asm_fib(int *arr, uint32_t length)
     * Accept a pointer to an array of integer and
     * the length of the array, then generate fibonacci
     * series starting at `0` and store the result
     * back to the array `arr` at corresponding indices.
     **/

    /* Enter your code after this comment */
    _if2:
        beq x10, x0, _end2
        beq x11, x0, _end2

        sub x12, x12, x12 //prev
        addi x13, x12, 1 // curr
        sub x14, x14, x14 //i
        li x15, 4
        li x18, 1

    _for:
        bge x14, x11, _end2
        mul x17, x14, x15
    _if3:
        beq x14, x0, _then3
        beq x14, x18, _then4
        addi x16, x13, 0 //tmp
        add x13, x13, x12
        addi x12, x16, 0
        add x17, x10, x17
        sw x13, 0(x17)
        addi x14, x14, 1 
        jal x0, _for


    _then3:
        sw x12, 0(x10)
        addi x14, x14, 1 
        jal x0,_for

    _then4:
        sw x13, 4(x10)
        addi x14, x14, 1 
        jal x0,_for

    _end2:


    /* Enter your code above this comment */
    ret

.global asm_toUppercase
asm_toUppercase:
    /* void asm_toUppercase(char *str)
     * loop through an entire string and only convert
     * the lowercase letter to uppercase, leaving the
     * rest of the string intact
     **/

    /* Enter your code after this comment */
    _upperif:
        beq x10, x0, _upperend
        

    _while2:
        lb x11, 0(x10)
        beq x11, x0, _upperend
        addi x16, x11, 0
        li x13, 97      
        bge x16, x13, _and
    _elseand:
        addi x10, x10, 1
        jal x0, _while2

    _and:
        li x13, 122
        bge x13, x16, _and2
        jal x0, _elseand

        
    _and2:
        li x15, 32
        sub x14, x16, x15
        sb x14, 0(x10)
        jal x0, _elseand

    _upperend:
    /* Enter your code above this comment */
    ret
