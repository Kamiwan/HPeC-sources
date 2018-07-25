/* --- Generated the 20/6/2018 at 0:20 --- */
/* --- heptagon compiler, version 1.04.00 (compiled mon. jun. 18 20:41:8 CET 2018) --- */
/* --- Command line: /home/gueyes/.opam/4.03.0/bin/heptc -hepts -s main -target c -target ctrln main.ept --- */

#ifndef MAIN_TYPES_H
#define MAIN_TYPES_H

#include "stdbool.h"
#include "assert.h"
#include "pervasives.h"
#include "main_controller_types.h"
typedef enum {
  Main__St_25_Wait,
  Main__St_25_Sw,
  Main__St_25_Inactive,
  Main__St_25_Hw2,
  Main__St_25_Hw1
} Main__st_25;

Main__st_25 Main__st_25_of_string(char* s);

char* string_of_Main__st_25(Main__st_25 x, char* buf);

typedef enum {
  Main__St_24_Wait,
  Main__St_24_Sw,
  Main__St_24_Inactive,
  Main__St_24_Hw2,
  Main__St_24_Hw1
} Main__st_24;

Main__st_24 Main__st_24_of_string(char* s);

char* string_of_Main__st_24(Main__st_24 x, char* buf);

typedef enum {
  Main__St_23_Wait,
  Main__St_23_Sw,
  Main__St_23_Inactive,
  Main__St_23_Hw2,
  Main__St_23_Hw1
} Main__st_23;

Main__st_23 Main__st_23_of_string(char* s);

char* string_of_Main__st_23(Main__st_23 x, char* buf);

typedef enum {
  Main__St_22_Wait,
  Main__St_22_Inactive,
  Main__St_22_Hw2,
  Main__St_22_Hw1
} Main__st_22;

Main__st_22 Main__st_22_of_string(char* s);

char* string_of_Main__st_22(Main__st_22 x, char* buf);

typedef enum {
  Main__St_21_Wait,
  Main__St_21_Sw,
  Main__St_21_Inactive,
  Main__St_21_Hw3,
  Main__St_21_Hw2,
  Main__St_21_Hw1
} Main__st_21;

Main__st_21 Main__st_21_of_string(char* s);

char* string_of_Main__st_21(Main__st_21 x, char* buf);

typedef enum {
  Main__St_20_Wait,
  Main__St_20_Sw,
  Main__St_20_Inactive,
  Main__St_20_Hw3,
  Main__St_20_Hw2,
  Main__St_20_Hw1
} Main__st_20;

Main__st_20 Main__st_20_of_string(char* s);

char* string_of_Main__st_20(Main__st_20 x, char* buf);

typedef enum {
  Main__St_19_Wait,
  Main__St_19_Sw,
  Main__St_19_Inactive,
  Main__St_19_Hw3,
  Main__St_19_Hw2,
  Main__St_19_Hw1
} Main__st_19;

Main__st_19 Main__st_19_of_string(char* s);

char* string_of_Main__st_19(Main__st_19 x, char* buf);

typedef enum {
  Main__St_18_Wait,
  Main__St_18_Sw,
  Main__St_18_Inactive,
  Main__St_18_Hw3,
  Main__St_18_Hw2,
  Main__St_18_Hw1
} Main__st_18;

Main__st_18 Main__st_18_of_string(char* s);

char* string_of_Main__st_18(Main__st_18 x, char* buf);

typedef enum {
  Main__St_17_Wait,
  Main__St_17_Sw,
  Main__St_17_Inactive,
  Main__St_17_Hw3,
  Main__St_17_Hw2,
  Main__St_17_Hw1
} Main__st_17;

Main__st_17 Main__st_17_of_string(char* s);

char* string_of_Main__st_17(Main__st_17 x, char* buf);

typedef enum {
  Main__St_16_Wait,
  Main__St_16_Inactive,
  Main__St_16_Active
} Main__st_16;

Main__st_16 Main__st_16_of_string(char* s);

char* string_of_Main__st_16(Main__st_16 x, char* buf);

typedef enum {
  Main__St_15_Wait,
  Main__St_15_Sw,
  Main__St_15_Inactive,
  Main__St_15_Hw3,
  Main__St_15_Hw2,
  Main__St_15_Hw1
} Main__st_15;

Main__st_15 Main__st_15_of_string(char* s);

char* string_of_Main__st_15(Main__st_15 x, char* buf);

typedef enum {
  Main__St_14_Free,
  Main__St_14_Fail,
  Main__St_14_Busy
} Main__st_14;

Main__st_14 Main__st_14_of_string(char* s);

char* string_of_Main__st_14(Main__st_14 x, char* buf);

typedef enum {
  Main__St_13_Free,
  Main__St_13_Fail,
  Main__St_13_Busy
} Main__st_13;

Main__st_13 Main__st_13_of_string(char* s);

char* string_of_Main__st_13(Main__st_13 x, char* buf);

typedef enum {
  Main__St_12_Free,
  Main__St_12_Fail,
  Main__St_12_Busy
} Main__st_12;

Main__st_12 Main__st_12_of_string(char* s);

char* string_of_Main__st_12(Main__st_12 x, char* buf);

typedef enum {
  Main__St_11_Wait,
  Main__St_11_Sw,
  Main__St_11_Inactive,
  Main__St_11_Hw2,
  Main__St_11_Hw1
} Main__st_11;

Main__st_11 Main__st_11_of_string(char* s);

char* string_of_Main__st_11(Main__st_11 x, char* buf);

typedef enum {
  Main__St_10_Wait,
  Main__St_10_Sw,
  Main__St_10_Inactive,
  Main__St_10_Hw2,
  Main__St_10_Hw1
} Main__st_10;

Main__st_10 Main__st_10_of_string(char* s);

char* string_of_Main__st_10(Main__st_10 x, char* buf);

typedef enum {
  Main__St_9_Wait,
  Main__St_9_Sw,
  Main__St_9_Inactive,
  Main__St_9_Hw2,
  Main__St_9_Hw1
} Main__st_9;

Main__st_9 Main__st_9_of_string(char* s);

char* string_of_Main__st_9(Main__st_9 x, char* buf);

typedef enum {
  Main__St_8_Wait,
  Main__St_8_Inactive,
  Main__St_8_Hw2,
  Main__St_8_Hw1
} Main__st_8;

Main__st_8 Main__st_8_of_string(char* s);

char* string_of_Main__st_8(Main__st_8 x, char* buf);

typedef enum {
  Main__St_7_Wait,
  Main__St_7_Sw,
  Main__St_7_Inactive,
  Main__St_7_Hw3,
  Main__St_7_Hw2,
  Main__St_7_Hw1
} Main__st_7;

Main__st_7 Main__st_7_of_string(char* s);

char* string_of_Main__st_7(Main__st_7 x, char* buf);

typedef enum {
  Main__St_6_Wait,
  Main__St_6_Sw,
  Main__St_6_Inactive,
  Main__St_6_Hw3,
  Main__St_6_Hw2,
  Main__St_6_Hw1
} Main__st_6;

Main__st_6 Main__st_6_of_string(char* s);

char* string_of_Main__st_6(Main__st_6 x, char* buf);

typedef enum {
  Main__St_5_Wait,
  Main__St_5_Sw,
  Main__St_5_Inactive,
  Main__St_5_Hw3,
  Main__St_5_Hw2,
  Main__St_5_Hw1
} Main__st_5;

Main__st_5 Main__st_5_of_string(char* s);

char* string_of_Main__st_5(Main__st_5 x, char* buf);

typedef enum {
  Main__St_4_Wait,
  Main__St_4_Sw,
  Main__St_4_Inactive,
  Main__St_4_Hw3,
  Main__St_4_Hw2,
  Main__St_4_Hw1
} Main__st_4;

Main__st_4 Main__st_4_of_string(char* s);

char* string_of_Main__st_4(Main__st_4 x, char* buf);

typedef enum {
  Main__St_3_Wait,
  Main__St_3_Sw,
  Main__St_3_Inactive,
  Main__St_3_Hw3,
  Main__St_3_Hw2,
  Main__St_3_Hw1
} Main__st_3;

Main__st_3 Main__st_3_of_string(char* s);

char* string_of_Main__st_3(Main__st_3 x, char* buf);

typedef enum {
  Main__St_2_Wait,
  Main__St_2_Inactive,
  Main__St_2_Active
} Main__st_2;

Main__st_2 Main__st_2_of_string(char* s);

char* string_of_Main__st_2(Main__st_2 x, char* buf);

typedef enum {
  Main__St_1_Wait,
  Main__St_1_Sw,
  Main__St_1_Inactive,
  Main__St_1_Hw3,
  Main__St_1_Hw2,
  Main__St_1_Hw1
} Main__st_1;

Main__st_1 Main__st_1_of_string(char* s);

char* string_of_Main__st_1(Main__st_1 x, char* buf);

typedef enum {
  Main__St_Free,
  Main__St_Fail,
  Main__St_Busy
} Main__st;

Main__st Main__st_of_string(char* s);

char* string_of_Main__st(Main__st x, char* buf);

typedef enum {
  Main__A,
  Main__B,
  Main__C,
  Main__D,
  Main__E,
  Main__F,
  Main__G,
  Main__H,
  Main__I,
  Main__J,
  Main__K
} Main__priority_type;

Main__priority_type Main__priority_type_of_string(char* s);

char* string_of_Main__priority_type(Main__priority_type x, char* buf);

typedef enum {
  Main__N,
  Main__S,
  Main__H1,
  Main__H2,
  Main__H3
} Main__version_type;

Main__version_type Main__version_type_of_string(char* s);

char* string_of_Main__version_type(Main__version_type x, char* buf);

#endif // MAIN_TYPES_H
