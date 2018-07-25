/* --- Generated the 20/6/2018 at 0:20 --- */
/* --- heptagon compiler, version 1.04.00 (compiled mon. jun. 18 20:41:8 CET 2018) --- */
/* --- Command line: /home/gueyes/.opam/4.03.0/bin/heptc -hepts -s main -target c -target ctrln main.ept --- */

#ifndef MAIN_H
#define MAIN_H

#include "main_types.h"
#include "main_controller.h"
typedef struct Main__priorityInt_out {
  int val2;
} Main__priorityInt_out;

void Main__priorityInt_step(Main__priority_type val1,
                            Main__priorityInt_out* _out);

typedef struct Main__tile_mem {
  Main__st ck;
  int pnr;
} Main__tile_mem;

typedef struct Main__tile_out {
  int act;
  int err;
} Main__tile_out;

void Main__tile_reset(Main__tile_mem* self);

void Main__tile_step(int cr, int f, int rp, Main__tile_out* _out,
                     Main__tile_mem* self);

typedef struct Main__contrast_img_mem {
  Main__st_1 ck;
  int pnr;
} Main__contrast_img_mem;

typedef struct Main__contrast_img_out {
  int size;
  int act;
  int wt;
  Main__version_type ver;
} Main__contrast_img_out;

void Main__contrast_img_reset(Main__contrast_img_mem* self);

void Main__contrast_img_step(int r, int c1, int c2, int c3, int c4, int e,
                             Main__contrast_img_out* _out,
                             Main__contrast_img_mem* self);

typedef struct Main__motion_estim_imu_mem {
  Main__st_2 ck;
  int pnr;
} Main__motion_estim_imu_mem;

typedef struct Main__motion_estim_imu_out {
  int size;
  int act;
  int wt;
  Main__version_type ver;
} Main__motion_estim_imu_out;

void Main__motion_estim_imu_reset(Main__motion_estim_imu_mem* self);

void Main__motion_estim_imu_step(int r, int c, int e,
                                 Main__motion_estim_imu_out* _out,
                                 Main__motion_estim_imu_mem* self);

typedef struct Main__motion_estim_img_mem {
  Main__st_3 ck;
  int pnr;
} Main__motion_estim_img_mem;

typedef struct Main__motion_estim_img_out {
  int size;
  int act;
  int wt;
  Main__version_type ver;
} Main__motion_estim_img_out;

void Main__motion_estim_img_reset(Main__motion_estim_img_mem* self);

void Main__motion_estim_img_step(int r, int c1, int c2, int c3, int c4,
                                 int e, Main__motion_estim_img_out* _out,
                                 Main__motion_estim_img_mem* self);

typedef struct Main__search_landing_mem {
  Main__st_4 ck;
  int pnr;
} Main__search_landing_mem;

typedef struct Main__search_landing_out {
  int size;
  int act;
  int wt;
  Main__version_type ver;
} Main__search_landing_out;

void Main__search_landing_reset(Main__search_landing_mem* self);

void Main__search_landing_step(int r, int c1, int c2, int c3, int c4, int e,
                               Main__search_landing_out* _out,
                               Main__search_landing_mem* self);

typedef struct Main__obstacle_avoidance_mem {
  Main__st_5 ck;
  int pnr;
} Main__obstacle_avoidance_mem;

typedef struct Main__obstacle_avoidance_out {
  int size;
  int act;
  int wt;
  Main__version_type ver;
} Main__obstacle_avoidance_out;

void Main__obstacle_avoidance_reset(Main__obstacle_avoidance_mem* self);

void Main__obstacle_avoidance_step(int r, int c1, int c2, int c3, int c4,
                                   int e, Main__obstacle_avoidance_out* _out,
                                   Main__obstacle_avoidance_mem* self);

typedef struct Main__t_landing_mem {
  Main__st_6 ck;
  int pnr;
} Main__t_landing_mem;

typedef struct Main__t_landing_out {
  int size;
  int act;
  int wt;
  Main__version_type ver;
} Main__t_landing_out;

void Main__t_landing_reset(Main__t_landing_mem* self);

void Main__t_landing_step(int r, int c1, int c2, int c3, int c4, int e,
                          Main__t_landing_out* _out,
                          Main__t_landing_mem* self);

typedef struct Main__rotoz_s_mem {
  Main__st_7 ck;
  int pnr;
} Main__rotoz_s_mem;

typedef struct Main__rotoz_s_out {
  int size;
  int act;
  int wt;
  Main__version_type ver;
} Main__rotoz_s_out;

void Main__rotoz_s_reset(Main__rotoz_s_mem* self);

void Main__rotoz_s_step(int r, int c1, int c2, int c3, int c4, int e,
                        Main__rotoz_s_out* _out, Main__rotoz_s_mem* self);

typedef struct Main__rotoz_b_mem {
  Main__st_8 ck;
  int pnr;
} Main__rotoz_b_mem;

typedef struct Main__rotoz_b_out {
  int size;
  int act;
  int wt;
  Main__version_type ver;
} Main__rotoz_b_out;

void Main__rotoz_b_reset(Main__rotoz_b_mem* self);

void Main__rotoz_b_step(int r, int c1, int c2, int e,
                        Main__rotoz_b_out* _out, Main__rotoz_b_mem* self);

typedef struct Main__replanning_mem {
  Main__st_9 ck;
  int pnr;
} Main__replanning_mem;

typedef struct Main__replanning_out {
  int size;
  int act;
  int wt;
  Main__version_type ver;
} Main__replanning_out;

void Main__replanning_reset(Main__replanning_mem* self);

void Main__replanning_step(int r, int c1, int c2, int c3, int e,
                           Main__replanning_out* _out,
                           Main__replanning_mem* self);

typedef struct Main__detection_mem {
  Main__st_10 ck;
  int pnr;
} Main__detection_mem;

typedef struct Main__detection_out {
  int size;
  int act;
  int wt;
  Main__version_type ver;
} Main__detection_out;

void Main__detection_reset(Main__detection_mem* self);

void Main__detection_step(int r, int c1, int c2, int c3, int e,
                          Main__detection_out* _out,
                          Main__detection_mem* self);

typedef struct Main__tracking_mem {
  Main__st_11 ck;
  int pnr;
} Main__tracking_mem;

typedef struct Main__tracking_out {
  int size;
  int act;
  int wt;
  Main__version_type ver;
} Main__tracking_out;

void Main__tracking_reset(Main__tracking_mem* self);

void Main__tracking_step(int r, int c1, int c2, int c3, int e,
                         Main__tracking_out* _out, Main__tracking_mem* self);

typedef struct Main__main_mem {
  Main__st_25 ck_23;
  Main__st_24 ck_25;
  Main__st_23 ck_27;
  Main__st_22 ck_29;
  Main__st_21 ck_31;
  Main__st_20 ck_33;
  Main__st_19 ck_35;
  Main__st_18 ck_37;
  Main__st_17 ck_39;
  Main__st_16 ck_41;
  Main__st_15 ck_43;
  Main__st_14 ck_45;
  Main__st_13 ck_47;
  Main__st_12 ck_49;
  int v_1045;
  int v_1034;
  int v_1027;
  int v_1018;
  int v_1007;
  int v_1000;
  int v_991;
  int v_980;
  int v_973;
  int pnr_13;
  int pnr_12;
  int pnr_11;
  int pnr_10;
  int pnr_9;
  int pnr_8;
  int pnr_7;
  int pnr_6;
  int pnr_5;
  int pnr_4;
  int pnr_3;
  int pnr_2;
  int pnr_1;
  int pnr;
} Main__main_mem;

typedef struct Main__main_out {
  int c_img_size;
  int c_img_act;
  int c_img_wt;
  Main__version_type c_img_ver;
  int me_imu_size;
  int me_imu_act;
  int me_imu_wt;
  Main__version_type me_imu_ver;
  int me_img_size;
  int me_img_act;
  int me_img_wt;
  Main__version_type me_img_ver;
  int sl_size;
  int sl_act;
  int sl_wt;
  Main__version_type sl_ver;
  int oa_size;
  int oa_act;
  int oa_wt;
  Main__version_type oa_ver;
  int tl_size;
  int tl_act;
  int tl_wt;
  Main__version_type tl_ver;
  int rs_size;
  int rs_act;
  int rs_wt;
  Main__version_type rs_ver;
  int rb_size;
  int rb_act;
  int rb_wt;
  Main__version_type rb_ver;
  int rpl_size;
  int rpl_act;
  int rpl_wt;
  Main__version_type rpl_ver;
  int dt_size;
  int dt_act;
  int dt_wt;
  Main__version_type dt_ver;
  int trk_size;
  int trk_act;
  int trk_wt;
  Main__version_type trk_ver;
  int act_1;
  int err_1;
  int act_2;
  int err_2;
  int act_3;
  int err_3;
  int obj;
  int obj_task;
  int obj_tiles;
  int wcet1;
  int wcet2;
  int wcet3;
  int wcet4;
  int wcet5;
  int wcet6;
  int wcet7;
  int wcet8;
  int wcet9;
  int wcet10;
  int wcet11;
  int wcet1_sw;
  int wcet1_hw1;
  int wcet1_hw2;
  int wcet1_hw3;
  int wcet2_sw;
  int wcet2_hw1;
  int wcet2_hw2;
  int wcet2_hw3;
  int wcet3_sw;
  int wcet3_hw1;
  int wcet3_hw2;
  int wcet3_hw3;
  int wcet4_sw;
  int wcet4_hw1;
  int wcet4_hw2;
  int wcet4_hw3;
  int wcet5_sw;
  int wcet5_hw1;
  int wcet5_hw2;
  int wcet5_hw3;
  int wcet6_sw;
  int wcet6_hw1;
  int wcet6_hw2;
  int wcet6_hw3;
  int wcet7_sw;
  int wcet7_hw1;
  int wcet7_hw2;
  int wcet7_hw3;
  int wcet8_sw;
  int wcet8_hw1;
  int wcet8_hw2;
  int wcet8_hw3;
  int wcet9_sw;
  int wcet9_hw1;
  int wcet9_hw2;
  int wcet9_hw3;
  int wcet10_sw;
  int wcet10_hw1;
  int wcet10_hw2;
  int wcet10_hw3;
  int wcet11_sw;
  int wcet11_hw1;
  int wcet11_hw2;
  int wcet11_hw3;
} Main__main_out;

void Main__main_reset(Main__main_mem* self);

void Main__main_step(int texe1, int texe2, int texe3, int texe4, int texe5,
                     int texe6, int texe7, int texe8, int texe9, int texe10,
                     int texe11, int mintexe1, int mintexe2, int mintexe3,
                     int mintexe4, int mintexe5, int mintexe6, int mintexe7,
                     int mintexe8, int mintexe9, int mintexe10,
                     int mintexe11, int maxtexe1, int maxtexe2, int maxtexe3,
                     int maxtexe4, int maxtexe5, int maxtexe6, int maxtexe7,
                     int maxtexe8, int maxtexe9, int maxtexe10,
                     int maxtexe11, int c_img_r, int c_img_e, int me_imu_r,
                     int me_imu_e, int me_img_r, int me_img_e, int sl_r,
                     int sl_e, int oa_r, int oa_e, int tl_r, int tl_e,
                     int rs_r, int rs_e, int rb_r, int rb_e, int rpl_r,
                     int rpl_e, int dt_r, int dt_e, int trk_r, int trk_e,
                     int f_1, int rp_1, int f_2, int rp_2, int f_3, int rp_3,
                     Main__main_out* _out, Main__main_mem* self);

#endif // MAIN_H
