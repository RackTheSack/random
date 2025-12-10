// Lab 1 template file
.data
.global lowercase_string
lowercase_string:
    // Specify a string ending with '\0'
    // The lowercase 'e' in eCE is not a typo!
    // You'll convert it to uppercase later.
    .asciz "eCE 362 is awesome!"

.global login_name
login_name:
    .asciz "silva48"

.global puid
puid:
    .asciz "0030099891"

// Align to 4bytes boundary as we have a string above
.balign 4

.text

.global q2_1_example
q2_1_example:
    // Assembly example
    /* Enter your code after this comment */
    add x10, x10, x11
    add x10, x10, x12
    add x10, x10, x13
    
    /* Enter your code above this comment */
    ret


.global q2_2_delta
q2_2_delta:
    // Compute the discriminant of a quadratic
    // function: b^2 - 4ac
    // x10: a
    // x11: b
    // x12: c
    // Put result back in x10

    /* Enter your code after this comment */
    mul x11, x11, x11
    mul x10, x10, x12
    slli x10, x10, 2
    sub x10, x11, x10

    /* Enter your code above this comment */
    ret


.global q2_3_dot_product
q2_3_dot_product:
    // Compute the dot product of two 2-D vectors
    // dot(A, B) = a1b1 + a2b2
    // x10: a1
    // x11: a2
    // x12: b1
    // x13: b2
    // Put result back in x10

    /* Enter your code after this comment */
    mul x10, x10, x12
    mul x11, x11, x13
    add x10, x10, x11
    /* Enter your code above this comment */
    ret

.global q3_1_MSB
q3_1_MSB:
    // Extract the MSB of the incoming
    // 32-bit word and store in register
    // x10

    /* Enter your code after this comment */  
    srli x10, x10, 24
    /* Enter your code above this comment */
    ret

.global q3_1_LSB
q3_1_LSB:
    // Extract the LSB of the incoming
    // 32-bit word and store in register x10

    /* Enter your code after this comment */
    slli x10, x10, 24
    srli x10, x10, 24
    
    /* Enter your code above this comment */
    ret

.global q3_2_flag_set
q3_2_flag_set:
    // Set the flag of a 32-bit value to 1
    // value: x10
    // flag:  x11
    // Put result back in x10

    /* Enter your code after this comment */
    or x10, x10, x11

    /* Enter your code above this comment */
    ret

.global q3_2_flag_reset
q3_2_flag_reset:
    // Set the flag of a 32-bit value to 0
    // value: x10
    // flag:  x11
    // Put result back in x10

    /* Enter your code after this comment */
    xori x11, x11, 0xFFFFFFFF 
    and x10, x10, x11

    /* Enter your code above this comment */
    ret

.global q3_2_flag_toggle
q3_2_flag_toggle:
    // Toggle the flag of a 32-bit value
    // value: x10
    // flag:  x11
    // Put result back in x10

    /* Enter your code after this comment */
    xor x10, x10, x11
    /* Enter your code above this comment */
    ret

.global q3_3_swap_byte
q3_3_swap_byte:
    // Swap the LSB and MSB of a 32-bit word

    /* Enter your code after this comment */
    srli x11, x10, 24
    slli x12, x10, 24
    slli x10, x10, 8
    srli x10, x10, 16
    slli x10, x10, 8
    or x10, x10, x11
    or x10, x10, x12

    /* Enter your code above this comment */
    ret

.global q4_1_toupper
q4_1_toupper:
    // Take in an address of a lowercase letter and save its uppercase form
    // to that address

    /* Enter your code after this comment */
    lb x11, 0(x10)
    li x13, 32
    sub x11, x11, x13
    sb x11, 0(x10)
    

    /* Enter your code above this comment */
    ret

.global q4_2_swap
q4_2_swap:
    // Take two 32-bit integers addresses and swap the content

    /* Enter your code after this comment */
    lw x12, 0(x10)
    lw x13, 0(x11)
    sw x12, 0(x11)
    sw x13, 0(x10)

    /* Enter your code above this comment */
    ret

.global q4_3_cutter
q4_3_cutter:
    // Take two arguments, a C character string starting address and a cut position
    // Passed in x10 and x11 respectively
    // After the function return, the string will be cut to position in register x11
    // i.e. string "Hello World!" with cut position 5 will results in "Hello"

    /* Enter your code after this comment */
    add x12, x10, x11
    li x13, 0
    sb x13, 0(x12)

    /* Enter your code above this comment */
    ret

.global q4_4_cvt_endian
q4_4_cvt_endian:
    // Take in an 32-bit integer and reverse its endianness

    /* Enter your code after this comment */
    lb x11, 0(x10)
    lb x12, 1(x10)
    lb x13, 2(x10)
    lb x14, 3(x10)
    sb x14, 0(x10)
    sb x13, 1(x10)
    sb x12, 2(x10)
    sb x11, 3(x10)

    /* Enter your code above this comment */
    ret
