#pragma once

#define CB_ENABLE_DEPTH 0x03
#define CB_ENABLE_CULL_FACE 0x04
#define CB_CLEAR 0x05
#define CB_SET_POLYGON_MODE 0x06
#define CB_DRAW_MESH 0x07

#define CB_SET_VECTOR_UNIFORM 0x08
#define CB_SET_FLOAT_UNIFORM 0x09
#define CB_SET_MATRIX_UNIFORM 0x0a
#define CB_SET_INTEGER_UNIFORM 0x0b
#define CB_BIND_TEXTURE 0x0c

#define CB_USE_PROGRAM 0xcc

#define CB_POLY_POINT 0x1b
#define CB_POLY_LINE 0x2b
#define CB_POLY_FILL 0x3b

#define FOUR_BYTE_ARR(NAME) \
    unsigned char NAME[4]; \
    ReadValue(NAME[0]); \
    ReadValue(NAME[1]); \
    ReadValue(NAME[2]); \
    ReadValue(NAME[3]);

#define WRITE_FOUR_BYTES(VAL, TYPE) \
    unsigned char b[sizeof(TYPE)]; \
    memcpy(b, &(VAL), sizeof(VAL)); \
    Commands.push_back(b[0]); \
    Commands.push_back(b[1]); \
    Commands.push_back(b[2]); \
    Commands.push_back(b[3]); 

#define READ_FOUR_BYTES(VAL,TYPE) \
    FOUR_BYTE_ARR(bytes) \
    memcpy(&(VAL), bytes, sizeof(TYPE));

#define READ_UINT(VALUE) \
    READ_FOUR_BYTES(VALUE,unsigned int)
    
#define WRITE_UINT(VALUE) \
    WRITE_FOUR_BYTES(VALUE,unsigned int)

#define WRITE_INT(VALUE) \
    WRITE_FOUR_BYTES(VALUE,int)

#define READ_INT(VALUE) \
    READ_FOUR_BYTES(VALUE,int)

#define WRITE_FLOAT(VALUE) \
    WRITE_FOUR_BYTES(VALUE,float)

#define READ_FLOAT(VALUE) \
    READ_FOUR_BYTES(VALUE,float)

// write matrix (col row)
#define WM(C,R) { WRITE_FLOAT(matrix[C][R]) }
#define RM(C,R) { READ_FLOAT(matrix[C][R]) }
