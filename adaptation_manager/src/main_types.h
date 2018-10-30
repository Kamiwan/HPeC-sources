/* --- Generated the 30/10/2018 at 0:44 --- */
/* --- heptagon compiler, version 1.05.00 (compiled sun. oct. 7 16:30:53 CET 2018) --- */
/* --- Command line: /home/gwen/.opam/4.04.0/bin/heptc -nocaus -simple-scheduler -target c -hepts -s main -target ctrln main.ept --- */

#ifndef MAIN_TYPES_H
#define MAIN_TYPES_H

#include "stdbool.h"
#include "assert.h"
#include "pervasives.h"
#include "main_controller_types.h"
typedef enum {
  Main__St_15_Idle,
  Main__St_15_Active
} Main__st_15;

Main__st_15 Main__st_15_of_string(char* s);

char* string_of_Main__st_15(Main__st_15 x, char* buf);

typedef enum {
  Main__St_14_Idle,
  Main__St_14_Active
} Main__st_14;

Main__st_14 Main__st_14_of_string(char* s);

char* string_of_Main__st_14(Main__st_14 x, char* buf);

typedef enum {
  Main__St_13_Idle,
  Main__St_13_Active
} Main__st_13;

Main__st_13 Main__st_13_of_string(char* s);

char* string_of_Main__st_13(Main__st_13 x, char* buf);

typedef enum {
  Main__St_12_Idle,
  Main__St_12_Active
} Main__st_12;

Main__st_12 Main__st_12_of_string(char* s);

char* string_of_Main__st_12(Main__st_12 x, char* buf);

typedef enum {
  Main__St_11_Idle,
  Main__St_11_Active
} Main__st_11;

Main__st_11 Main__st_11_of_string(char* s);

char* string_of_Main__st_11(Main__st_11 x, char* buf);

typedef enum {
  Main__St_10_Idle,
  Main__St_10_Active
} Main__st_10;

Main__st_10 Main__st_10_of_string(char* s);

char* string_of_Main__st_10(Main__st_10 x, char* buf);

typedef enum {
  Main__St_9_Idle,
  Main__St_9_Active
} Main__st_9;

Main__st_9 Main__st_9_of_string(char* s);

char* string_of_Main__st_9(Main__st_9 x, char* buf);

typedef enum {
  Main__St_8_Idle,
  Main__St_8_Active
} Main__st_8;

Main__st_8 Main__st_8_of_string(char* s);

char* string_of_Main__st_8(Main__st_8 x, char* buf);

typedef enum {
  Main__St_7_Idle,
  Main__St_7_Active
} Main__st_7;

Main__st_7 Main__st_7_of_string(char* s);

char* string_of_Main__st_7(Main__st_7 x, char* buf);

typedef enum {
  Main__St_6_Idle,
  Main__St_6_Active
} Main__st_6;

Main__st_6 Main__st_6_of_string(char* s);

char* string_of_Main__st_6(Main__st_6 x, char* buf);

typedef enum {
  Main__St_5_Idle,
  Main__St_5_Active
} Main__st_5;

Main__st_5 Main__st_5_of_string(char* s);

char* string_of_Main__st_5(Main__st_5 x, char* buf);

typedef enum {
  Main__St_4_Free,
  Main__St_4_Fail
} Main__st_4;

Main__st_4 Main__st_4_of_string(char* s);

char* string_of_Main__st_4(Main__st_4 x, char* buf);

typedef enum {
  Main__St_3_Free,
  Main__St_3_Fail
} Main__st_3;

Main__st_3 Main__st_3_of_string(char* s);

char* string_of_Main__st_3(Main__st_3 x, char* buf);

typedef enum {
  Main__St_2_Free,
  Main__St_2_Fail
} Main__st_2;

Main__st_2 Main__st_2_of_string(char* s);

char* string_of_Main__st_2(Main__st_2 x, char* buf);

typedef enum {
  Main__St_1_Idle,
  Main__St_1_Active
} Main__st_1;

Main__st_1 Main__st_1_of_string(char* s);

char* string_of_Main__st_1(Main__st_1 x, char* buf);

typedef enum {
  Main__St_Free,
  Main__St_Fail
} Main__st;

Main__st Main__st_of_string(char* s);

char* string_of_Main__st(Main__st x, char* buf);

typedef enum {
  Main__H1,
  Main__H2,
  Main__H3,
  Main__S
} Main__version_type;

Main__version_type Main__version_type_of_string(char* s);

char* string_of_Main__version_type(Main__version_type x, char* buf);

typedef enum {
  Main__T1,
  Main__T2,
  Main__T3
} Main__tile_number;

Main__tile_number Main__tile_number_of_string(char* s);

char* string_of_Main__tile_number(Main__tile_number x, char* buf);

#endif // MAIN_TYPES_H
