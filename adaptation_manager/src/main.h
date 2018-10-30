/* --- Generated the 30/10/2018 at 0:44 --- */
/* --- heptagon compiler, version 1.05.00 (compiled sun. oct. 7 16:30:53 CET 2018) --- */
/* --- Command line: /home/gwen/.opam/4.04.0/bin/heptc -nocaus -simple-scheduler -target c -hepts -s main -target ctrln main.ept --- */

#ifndef MAIN_H
#define MAIN_H

#include "main_types.h"
#include "main_controller.h"
typedef struct Main__pref_ver_task_mem {
  int texec_h3_11;
  int texec_h2_11;
  int texec_h1_11;
  int texec_sw_11;
} Main__pref_ver_task_mem;

typedef struct Main__pref_ver_task_out {
  Main__version_type pref_ver;
  int obj;
} Main__pref_ver_task_out;

void Main__pref_ver_task_reset(Main__pref_ver_task_mem* self);

void Main__pref_ver_task_step(Main__version_type task_ver, int current_texec,
                              int tmin, int tmax,
                              Main__pref_ver_task_out* _out,
                              Main__pref_ver_task_mem* self);

typedef struct Main__main_mem {
  Main__st_15 ck;
  Main__st_14 ck_7;
  Main__st_13 ck_10;
  Main__st_12 ck_13;
  Main__st_11 ck_16;
  Main__st_10 ck_19;
  Main__st_9 ck_22;
  Main__st_8 ck_25;
  Main__st_7 ck_28;
  Main__st_6 ck_31;
  Main__st_5 ck_34;
  Main__st_4 ck_37;
  Main__st_3 ck_39;
  Main__st_2 ck_41;
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
  int pref_ver_task_guarantee_12;
  int pref_ver_task_assume_12;
  int pref_ver_task_guarantee_1_1;
  int pref_ver_task_assume_1_1;
  int pref_ver_task_guarantee_2_1;
  int pref_ver_task_assume_2_1;
  int pref_ver_task_guarantee_3_1;
  int pref_ver_task_assume_3_1;
  int pref_ver_task_guarantee_4_1;
  int pref_ver_task_assume_4_1;
  int pref_ver_task_guarantee_5_1;
  int pref_ver_task_assume_5_1;
  int pref_ver_task_guarantee_6_1;
  int pref_ver_task_assume_6_1;
  int pref_ver_task_guarantee_7_1;
  int pref_ver_task_assume_7_1;
  int pref_ver_task_guarantee_8_1;
  int pref_ver_task_assume_8_1;
  int pref_ver_task_guarantee_9_1;
  int pref_ver_task_assume_9_1;
  int pref_ver_task_guarantee_10_1;
  int pref_ver_task_assume_10_1;
  Main__pref_ver_task_mem pref_ver_task_10;
  Main__pref_ver_task_mem pref_ver_task_9;
  Main__pref_ver_task_mem pref_ver_task_8;
  Main__pref_ver_task_mem pref_ver_task_7;
  Main__pref_ver_task_mem pref_ver_task_6;
  Main__pref_ver_task_mem pref_ver_task_5;
  Main__pref_ver_task_mem pref_ver_task_4;
  Main__pref_ver_task_mem pref_ver_task_3;
  Main__pref_ver_task_mem pref_ver_task_2;
  Main__pref_ver_task_mem pref_ver_task_1;
  Main__pref_ver_task_mem pref_ver_task;
} Main__main_mem;

typedef struct Main__main_out {
  int c_img_act;
  int c_img_wt;
  Main__version_type c_img_ver;
  int me_imu_act;
  int me_imu_wt;
  Main__version_type me_imu_ver;
  int me_img_act;
  int me_img_wt;
  Main__version_type me_img_ver;
  int sl_act;
  int sl_wt;
  Main__version_type sl_ver;
  int oa_act;
  int oa_wt;
  Main__version_type oa_ver;
  int tl_act;
  int tl_wt;
  Main__version_type tl_ver;
  int rs_act;
  int rs_wt;
  Main__version_type rs_ver;
  int rb_act;
  int rb_wt;
  Main__version_type rb_ver;
  int rpl_act;
  int rpl_wt;
  Main__version_type rpl_ver;
  int dt_act;
  int dt_wt;
  Main__version_type dt_ver;
  int trk_act;
  int trk_wt;
  Main__version_type trk_ver;
  int obj_occ;
  int obj_pref;
  int obj_tasks;
} Main__main_out;

void Main__main_reset(Main__main_mem* self);

void Main__main_step(int prio_t1, int prio_t2, int prio_t3, int prio_t4,
                     int prio_t5, int prio_t6, int prio_t7, int prio_t8,
                     int prio_t9, int prio_t10, int prio_t11, int texe1,
                     int texe2, int texe3, int texe4, int texe5, int texe6,
                     int texe7, int texe8, int texe9, int texe10, int texe11,
                     int min1, int min2, int min3, int min4, int min5,
                     int min6, int min7, int min8, int min9, int min10,
                     int min11, int max1, int max2, int max3, int max4,
                     int max5, int max6, int max7, int max8, int max9,
                     int max10, int max11, int c_img_r, int c_img_e,
                     int me_imu_r, int me_imu_e, int me_img_r, int me_img_e,
                     int sl_r, int sl_e, int oa_r, int oa_e, int tl_r,
                     int tl_e, int rs_r, int rs_e, int rb_r, int rb_e,
                     int rpl_r, int rpl_e, int dt_r, int dt_e, int trk_r,
                     int trk_e, int f_1, int rp_1, int f_2, int rp_2,
                     int f_3, int rp_3, Main__main_out* _out,
                     Main__main_mem* self);

#endif // MAIN_H
