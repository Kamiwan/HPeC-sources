/* --- Generated the 23/7/2019 at 16:28 --- */
/* --- heptagon compiler, version 1.05.00 (compiled mon. may. 20 10:42:2 CET 2019) --- */
/* --- Command line: /home/gwen/.opam/4.07.0/bin/heptc -nocaus -simple-scheduler -target c main_controller.ept --- */

#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

#include "main_controller_types.h"
#include "main.h"
typedef struct Main_controller__main_ctrlr0_out {
  int c_c_img_pref;
  Main__version_type c_c_img_ver;
  int c_dt_pref;
  Main__version_type c_dt_ver;
  int c_me_img_pref;
  Main__version_type c_me_img_ver;
  int c_me_imu_pref;
  Main__version_type c_me_imu_ver;
  int c_oa_pref;
  Main__version_type c_oa_ver;
  int c_rb_pref;
  Main__version_type c_rb_ver;
  int c_rpl_pref;
  Main__version_type c_rpl_ver;
  int c_rs_pref;
  Main__version_type c_rs_ver;
  int c_sl_pref;
  Main__version_type c_sl_ver;
  int c_tl_pref;
  Main__version_type c_tl_ver;
  int c_trk_pref;
  Main__version_type c_trk_ver;
} Main_controller__main_ctrlr0_out;

void Main_controller__main_ctrlr0_step(int c_img_e, int c_img_r,
                                       Main__st_18 ck, Main__st_17 ck_10,
                                       Main__st_16 ck_13, Main__st_15 ck_16,
                                       Main__st_14 ck_19, Main__st_13 ck_22,
                                       Main__st_12 ck_25, Main__st_11 ck_28,
                                       Main__st_10 ck_31, Main__st_9 ck_34,
                                       Main__st_8 ck_37, Main__st_7 ck_40,
                                       Main__st_6 ck_42, Main__st_5 ck_44,
                                       int detection_e, int detection_r,
                                       int dt_e, int dt_r, int dummy,
                                       int f_1, int f_2, int f_3, int max1,
                                       int max10, int max11, int max2,
                                       int max3, int max4, int max5,
                                       int max6, int max7, int max8,
                                       int max9, int me_img_e, int me_img_r,
                                       int me_imu_e, int me_imu_r, int min1,
                                       int min10, int min11, int min2,
                                       int min3, int min4, int min5,
                                       int min6, int min7, int min8,
                                       int min9, int ncc, int oa_e, int oa_r,
                                       int obj_12, int obj_13, int obj_14,
                                       int obj_15, int obj_16, int obj_17,
                                       int obj_18, int obj_19, int obj_20,
                                       int obj_21, int obj_22, int pnr,
                                       int pnr_10, int pnr_11, int pnr_12,
                                       int pnr_13, int pnr_14, int pnr_2,
                                       int pnr_3, int pnr_4, int pnr_5,
                                       int pnr_6, int pnr_7, int pnr_8,
                                       int pnr_9, int pref_verH1,
                                       int pref_verH1_1, int pref_verH1_10,
                                       int pref_verH1_2, int pref_verH1_3,
                                       int pref_verH1_4, int pref_verH1_5,
                                       int pref_verH1_6, int pref_verH1_7,
                                       int pref_verH1_8, int pref_verH1_9,
                                       int pref_verH2, int pref_verH2_1,
                                       int pref_verH2_10, int pref_verH2_2,
                                       int pref_verH2_3, int pref_verH2_4,
                                       int pref_verH2_5, int pref_verH2_6,
                                       int pref_verH2_7, int pref_verH2_8,
                                       int pref_verH2_9, int pref_verH3,
                                       int pref_verH3_1, int pref_verH3_10,
                                       int pref_verH3_2, int pref_verH3_3,
                                       int pref_verH3_4, int pref_verH3_5,
                                       int pref_verH3_6, int pref_verH3_7,
                                       int pref_verH3_8, int pref_verH3_9,
                                       int pref_verS, int pref_verS_1,
                                       int pref_verS_10, int pref_verS_2,
                                       int pref_verS_3, int pref_verS_4,
                                       int pref_verS_5, int pref_verS_6,
                                       int pref_verS_7, int pref_verS_8,
                                       int pref_verS_9,
                                       Main__version_type pref_ver_12,
                                       Main__version_type pref_ver_13,
                                       Main__version_type pref_ver_14,
                                       Main__version_type pref_ver_15,
                                       Main__version_type pref_ver_16,
                                       Main__version_type pref_ver_17,
                                       Main__version_type pref_ver_18,
                                       Main__version_type pref_ver_19,
                                       Main__version_type pref_ver_20,
                                       Main__version_type pref_ver_21,
                                       Main__version_type pref_ver_22,
                                       int pref_ver_task_assume_10_1,
                                       int pref_ver_task_assume_12,
                                       int pref_ver_task_assume_1_1,
                                       int pref_ver_task_assume_2_1,
                                       int pref_ver_task_assume_3_1,
                                       int pref_ver_task_assume_4_1,
                                       int pref_ver_task_assume_5_1,
                                       int pref_ver_task_assume_6_1,
                                       int pref_ver_task_assume_7_1,
                                       int pref_ver_task_assume_8_1,
                                       int pref_ver_task_assume_9_1,
                                       int pref_ver_task_guarantee_10_1,
                                       int pref_ver_task_guarantee_12,
                                       int pref_ver_task_guarantee_1_1,
                                       int pref_ver_task_guarantee_2_1,
                                       int pref_ver_task_guarantee_3_1,
                                       int pref_ver_task_guarantee_4_1,
                                       int pref_ver_task_guarantee_5_1,
                                       int pref_ver_task_guarantee_6_1,
                                       int pref_ver_task_guarantee_7_1,
                                       int pref_ver_task_guarantee_8_1,
                                       int pref_ver_task_guarantee_9_1,
                                       int rb_e, int rb_r, int rp_1,
                                       int rp_2, int rp_3, int rpl_e,
                                       int rpl_r, int rs_e, int rs_r,
                                       int sl_e, int sl_r,
                                       Main__version_type task_ver_12,
                                       Main__version_type task_ver_13,
                                       Main__version_type task_ver_14,
                                       Main__version_type task_ver_15,
                                       Main__version_type task_ver_16,
                                       Main__version_type task_ver_17,
                                       Main__version_type task_ver_18,
                                       Main__version_type task_ver_19,
                                       Main__version_type task_ver_20,
                                       Main__version_type task_ver_21,
                                       Main__version_type task_ver_22,
                                       int texe1, int texe10, int texe11,
                                       int texe2, int texe3, int texe4,
                                       int texe5, int texe6, int texe7,
                                       int texe8, int texe9, int tl_e,
                                       int tl_r, int tracking_e,
                                       int tracking_r, int trk_e, int trk_r,
                                       Main_controller__main_ctrlr0_out* _out);

typedef struct Main_controller__pref_ver_task_ctrlr0_out {
  int c_2ndbest;
  int c_best;
  Main__version_type c_pref_ver;
} Main_controller__pref_ver_task_ctrlr0_out;

void Main_controller__pref_ver_task_ctrlr0_step(int current_texec,
                                                Main__version_type task_ver,
                                                int texec_h1_11,
                                                int texec_h2_11,
                                                int texec_h3_11,
                                                int texec_sw_11, int tmax,
                                                int tmin, int v,
                                                Main_controller__pref_ver_task_ctrlr0_out* _out);

#endif // MAIN_CONTROLLER_H
