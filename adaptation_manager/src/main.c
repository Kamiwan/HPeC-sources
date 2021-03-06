/* --- Generated the 30/10/2018 at 9:38 --- */
/* --- heptagon compiler, version 1.05.00 (compiled sun. oct. 7 16:30:53 CET 2018) --- */
/* --- Command line: /home/gwen/.opam/4.04.0/bin/heptc -nocaus -simple-scheduler -target c -hepts -s main -target ctrln main.ept --- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

void Main__pref_ver_task_reset(Main__pref_ver_task_mem* self) {
  self->texec_sw_11 = 200;
  self->texec_h1_11 = 10;
  self->texec_h2_11 = 10;
  self->texec_h3_11 = 10;
}

void Main__pref_ver_task_step(Main__version_type task_ver, int current_texec,
                              int tmin, int tmax,
                              Main__pref_ver_task_out* _out,
                              Main__pref_ver_task_mem* self) {
  Main_controller__pref_ver_task_ctrlr0_out Main_controller__pref_ver_task_ctrlr0_out_st;
  
  int c_best;
  Main__version_type c_pref_ver;
  int texec_h3_H3;
  int texec_h2_H3;
  int texec_h1_H3;
  int texec_sw_H3;
  int texec_h3_H2;
  int texec_h2_H2;
  int texec_h1_H2;
  int texec_sw_H2;
  int texec_h3_H1;
  int texec_h2_H1;
  int texec_h1_H1;
  int texec_sw_H1;
  int texec_h3_S;
  int texec_h2_S;
  int texec_h1_S;
  int texec_sw_S;
  Main__version_type ck;
  int expected_texec_H3;
  int expected_texec_H2;
  int expected_texec_H1;
  int expected_texec_S;
  Main__version_type ck_2;
  int v_2;
  int v_1;
  int v;
  int texec_sw;
  int texec_h1;
  int texec_h2;
  int texec_h3;
  int expected_texec;
  Main_controller__pref_ver_task_ctrlr0_step(current_texec, task_ver,
                                             self->texec_h1_11,
                                             self->texec_h2_11,
                                             self->texec_h3_11,
                                             self->texec_sw_11, tmax, tmin,
                                             &Main_controller__pref_ver_task_ctrlr0_out_st);
  c_best = Main_controller__pref_ver_task_ctrlr0_out_st.c_best;
  c_pref_ver = Main_controller__pref_ver_task_ctrlr0_out_st.c_pref_ver;
  ck = task_ver;
  switch (ck) {
    case Main__H3:
      texec_h3_H3 = current_texec;
      texec_h3 = texec_h3_H3;
      break;
    case Main__H2:
      texec_h3_H2 = self->texec_h3_11;
      texec_h3 = texec_h3_H2;
      break;
    case Main__H1:
      texec_h3_H1 = self->texec_h3_11;
      texec_h3 = texec_h3_H1;
      break;
    case Main__S:
      texec_h3_S = self->texec_h3_11;
      texec_h3 = texec_h3_S;
      break;
    default:
      break;
  };
  _out->pref_ver = c_pref_ver;
  ck_2 = _out->pref_ver;
  switch (ck_2) {
    case Main__H3:
      expected_texec_H3 = texec_h3;
      break;
    default:
      break;
  };
  switch (ck) {
    case Main__H3:
      texec_h2_H3 = self->texec_h2_11;
      texec_h2 = texec_h2_H3;
      break;
    case Main__H2:
      texec_h2_H2 = current_texec;
      texec_h2 = texec_h2_H2;
      break;
    case Main__H1:
      texec_h2_H1 = self->texec_h2_11;
      texec_h2 = texec_h2_H1;
      break;
    case Main__S:
      texec_h2_S = self->texec_h2_11;
      texec_h2 = texec_h2_S;
      break;
    default:
      break;
  };
  switch (ck_2) {
    case Main__H2:
      expected_texec_H2 = texec_h2;
      break;
    default:
      break;
  };
  switch (ck) {
    case Main__H3:
      texec_h1_H3 = self->texec_h1_11;
      texec_h1 = texec_h1_H3;
      break;
    case Main__H2:
      texec_h1_H2 = self->texec_h1_11;
      texec_h1 = texec_h1_H2;
      break;
    case Main__H1:
      texec_h1_H1 = current_texec;
      texec_h1 = texec_h1_H1;
      break;
    case Main__S:
      texec_h1_S = self->texec_h1_11;
      texec_h1 = texec_h1_S;
      break;
    default:
      break;
  };
  switch (ck_2) {
    case Main__H1:
      expected_texec_H1 = texec_h1;
      break;
    default:
      break;
  };
  switch (ck) {
    case Main__H3:
      texec_sw_H3 = self->texec_sw_11;
      texec_sw = texec_sw_H3;
      break;
    case Main__H2:
      texec_sw_H2 = self->texec_sw_11;
      texec_sw = texec_sw_H2;
      break;
    case Main__H1:
      texec_sw_H1 = self->texec_sw_11;
      texec_sw = texec_sw_H1;
      break;
    case Main__S:
      texec_sw_S = current_texec;
      texec_sw = texec_sw_S;
      break;
    default:
      break;
  };
  switch (ck_2) {
    case Main__S:
      expected_texec_S = texec_sw;
      expected_texec = expected_texec_S;
      break;
    case Main__H3:
      expected_texec = expected_texec_H3;
      break;
    case Main__H2:
      expected_texec = expected_texec_H2;
      break;
    case Main__H1:
      expected_texec = expected_texec_H1;
      break;
    default:
      break;
  };
  v = (expected_texec>=tmin);
  v_1 = (expected_texec<=tmax);
  v_2 = (v&&v_1);
  _out->obj = (!(c_best)||v_2);
  self->texec_sw_11 = texec_sw;
  self->texec_h1_11 = texec_h1;
  self->texec_h2_11 = texec_h2;
  self->texec_h3_11 = texec_h3;;
}

void Main__main_reset(Main__main_mem* self) {
  Main__pref_ver_task_reset(&self->pref_ver_task);
  Main__pref_ver_task_reset(&self->pref_ver_task_1);
  Main__pref_ver_task_reset(&self->pref_ver_task_2);
  Main__pref_ver_task_reset(&self->pref_ver_task_3);
  Main__pref_ver_task_reset(&self->pref_ver_task_4);
  Main__pref_ver_task_reset(&self->pref_ver_task_5);
  Main__pref_ver_task_reset(&self->pref_ver_task_6);
  Main__pref_ver_task_reset(&self->pref_ver_task_7);
  Main__pref_ver_task_reset(&self->pref_ver_task_8);
  Main__pref_ver_task_reset(&self->pref_ver_task_9);
  Main__pref_ver_task_reset(&self->pref_ver_task_10);
  self->ck = Main__St_15_Idle;
  self->pnr_13 = false;
  self->ck_7 = Main__St_14_Idle;
  self->pnr_12 = false;
  self->ck_10 = Main__St_13_Idle;
  self->pnr_11 = false;
  self->ck_13 = Main__St_12_Idle;
  self->pnr_10 = false;
  self->ck_16 = Main__St_11_Idle;
  self->pnr_9 = false;
  self->ck_19 = Main__St_10_Idle;
  self->pnr_8 = false;
  self->ck_22 = Main__St_9_Idle;
  self->pnr_7 = false;
  self->ck_25 = Main__St_8_Idle;
  self->pnr_6 = false;
  self->ck_28 = Main__St_7_Idle;
  self->pnr_5 = false;
  self->ck_31 = Main__St_6_Idle;
  self->pnr_4 = false;
  self->ck_34 = Main__St_5_Idle;
  self->pnr_3 = false;
  self->ck_37 = Main__St_4_Free;
  self->pnr_2 = false;
  self->ck_39 = Main__St_3_Free;
  self->pnr_1 = false;
  self->ck_41 = Main__St_2_Free;
  self->pnr = false;
  self->pref_ver_task_assume_10_1 = true;
  self->pref_ver_task_guarantee_10_1 = true;
  self->pref_ver_task_assume_9_1 = true;
  self->pref_ver_task_guarantee_9_1 = true;
  self->pref_ver_task_assume_8_1 = true;
  self->pref_ver_task_guarantee_8_1 = true;
  self->pref_ver_task_assume_7_1 = true;
  self->pref_ver_task_guarantee_7_1 = true;
  self->pref_ver_task_assume_6_1 = true;
  self->pref_ver_task_guarantee_6_1 = true;
  self->pref_ver_task_assume_5_1 = true;
  self->pref_ver_task_guarantee_5_1 = true;
  self->pref_ver_task_assume_4_1 = true;
  self->pref_ver_task_guarantee_4_1 = true;
  self->pref_ver_task_assume_3_1 = true;
  self->pref_ver_task_guarantee_3_1 = true;
  self->pref_ver_task_assume_2_1 = true;
  self->pref_ver_task_guarantee_2_1 = true;
  self->pref_ver_task_assume_1_1 = true;
  self->pref_ver_task_guarantee_1_1 = true;
  self->pref_ver_task_assume_12 = true;
  self->pref_ver_task_guarantee_12 = true;
}

void Main__main_step(int texe1, int texe2, int texe3, int texe4, int texe5,
                     int texe6, int texe7, int texe8, int texe9, int texe10,
                     int texe11, int min1, int min2, int min3, int min4,
                     int min5, int min6, int min7, int min8, int min9,
                     int min10, int min11, int max1, int max2, int max3,
                     int max4, int max5, int max6, int max7, int max8,
                     int max9, int max10, int max11, int c_img_r,
                     int c_img_e, int me_imu_r, int me_imu_e, int me_img_r,
                     int me_img_e, int sl_r, int sl_e, int oa_r, int oa_e,
                     int tl_r, int tl_e, int rs_r, int rs_e, int rb_r,
                     int rb_e, int rpl_r, int rpl_e, int dt_r, int dt_e,
                     int trk_r, int trk_e, int f_1, int rp_1, int f_2,
                     int rp_2, int f_3, int rp_3, Main__main_out* _out,
                     Main__main_mem* self) {
  Main_controller__main_ctrlr7_out Main_controller__main_ctrlr7_out_st;
  Main_controller__main_ctrlr3_out Main_controller__main_ctrlr3_out_st;
  Main_controller__main_ctrlr8_out Main_controller__main_ctrlr8_out_st;
  Main_controller__main_ctrlr11_out Main_controller__main_ctrlr11_out_st;
  Main_controller__main_ctrlr0_out Main_controller__main_ctrlr0_out_st;
  Main__pref_ver_task_out Main__pref_ver_task_out_st;
  Main_controller__main_ctrlr4_out Main_controller__main_ctrlr4_out_st;
  Main_controller__main_ctrlr5_out Main_controller__main_ctrlr5_out_st;
  Main_controller__main_ctrlr2_out Main_controller__main_ctrlr2_out_st;
  Main_controller__main_ctrlr10_out Main_controller__main_ctrlr10_out_st;
  Main_controller__main_ctrlr9_out Main_controller__main_ctrlr9_out_st;
  Main_controller__main_ctrlr6_out Main_controller__main_ctrlr6_out_st;
  Main_controller__main_ctrlr1_out Main_controller__main_ctrlr1_out_st;
  
  int v;
  int c_c_img_pref;
  int c_me_imu_pref;
  int c_me_img_pref;
  int c_sl_pref;
  int c_oa_pref;
  int c_tl_pref;
  int c_rs_pref;
  int c_rb_pref;
  int c_dt_pref;
  int c_rpl_pref;
  int c_trk_pref;
  Main__version_type c_c_img_ver;
  Main__version_type c_me_imu_ver;
  Main__version_type c_me_img_ver;
  Main__version_type c_sl_ver;
  Main__version_type c_oa_ver;
  Main__version_type c_tl_ver;
  Main__version_type c_rs_ver;
  Main__version_type c_rb_ver;
  Main__version_type c_dt_ver;
  Main__version_type c_rpl_ver;
  Main__version_type c_trk_ver;
  int r_13_St_15_Active;
  Main__st_15 s_13_St_15_Active;
  int r_13_St_15_Idle;
  Main__st_15 s_13_St_15_Idle;
  int t3_St_15_Active_H3;
  int t2_St_15_Active_H3;
  int t1_St_15_Active_H3;
  int t3_St_15_Active_H2;
  int t2_St_15_Active_H2;
  int t1_St_15_Active_H2;
  int t3_St_15_Active_H1;
  int t2_St_15_Active_H1;
  int t1_St_15_Active_H1;
  int t3_St_15_Active_S;
  int t2_St_15_Active_S;
  int t1_St_15_Active_S;
  Main__version_type ck_6;
  int nr_13_St_15_Active;
  Main__st_15 ns_13_St_15_Active;
  int t3_St_15_Active;
  int t2_St_15_Active;
  int t1_St_15_Active;
  int act_St_15_Active;
  int nr_13_St_15_Idle;
  Main__st_15 ns_13_St_15_Idle;
  int t3_St_15_Idle;
  int t2_St_15_Idle;
  int t1_St_15_Idle;
  int act_St_15_Idle;
  Main__st_15 ck_5;
  int r_12_St_14_Active;
  Main__st_14 s_12_St_14_Active;
  int r_12_St_14_Idle;
  Main__st_14 s_12_St_14_Idle;
  int t3_1_St_14_Active_H3;
  int t2_1_St_14_Active_H3;
  int t1_1_St_14_Active_H3;
  int t3_1_St_14_Active_H2;
  int t2_1_St_14_Active_H2;
  int t1_1_St_14_Active_H2;
  int t3_1_St_14_Active_H1;
  int t2_1_St_14_Active_H1;
  int t1_1_St_14_Active_H1;
  int t3_1_St_14_Active_S;
  int t2_1_St_14_Active_S;
  int t1_1_St_14_Active_S;
  Main__version_type ck_9;
  int nr_12_St_14_Active;
  Main__st_14 ns_12_St_14_Active;
  int t3_1_St_14_Active;
  int t2_1_St_14_Active;
  int t1_1_St_14_Active;
  int act_1_St_14_Active;
  int nr_12_St_14_Idle;
  Main__st_14 ns_12_St_14_Idle;
  int t3_1_St_14_Idle;
  int t2_1_St_14_Idle;
  int t1_1_St_14_Idle;
  int act_1_St_14_Idle;
  Main__st_14 ck_8;
  int r_11_St_13_Active;
  Main__st_13 s_11_St_13_Active;
  int r_11_St_13_Idle;
  Main__st_13 s_11_St_13_Idle;
  int t3_2_St_13_Active_H3;
  int t2_2_St_13_Active_H3;
  int t1_2_St_13_Active_H3;
  int t3_2_St_13_Active_H2;
  int t2_2_St_13_Active_H2;
  int t1_2_St_13_Active_H2;
  int t3_2_St_13_Active_H1;
  int t2_2_St_13_Active_H1;
  int t1_2_St_13_Active_H1;
  int t3_2_St_13_Active_S;
  int t2_2_St_13_Active_S;
  int t1_2_St_13_Active_S;
  Main__version_type ck_12;
  int nr_11_St_13_Active;
  Main__st_13 ns_11_St_13_Active;
  int t3_2_St_13_Active;
  int t2_2_St_13_Active;
  int t1_2_St_13_Active;
  int act_2_St_13_Active;
  int nr_11_St_13_Idle;
  Main__st_13 ns_11_St_13_Idle;
  int t3_2_St_13_Idle;
  int t2_2_St_13_Idle;
  int t1_2_St_13_Idle;
  int act_2_St_13_Idle;
  Main__st_13 ck_11;
  int r_10_St_12_Active;
  Main__st_12 s_10_St_12_Active;
  int r_10_St_12_Idle;
  Main__st_12 s_10_St_12_Idle;
  int t3_3_St_12_Active_H3;
  int t2_3_St_12_Active_H3;
  int t1_3_St_12_Active_H3;
  int t3_3_St_12_Active_H2;
  int t2_3_St_12_Active_H2;
  int t1_3_St_12_Active_H2;
  int t3_3_St_12_Active_H1;
  int t2_3_St_12_Active_H1;
  int t1_3_St_12_Active_H1;
  int t3_3_St_12_Active_S;
  int t2_3_St_12_Active_S;
  int t1_3_St_12_Active_S;
  Main__version_type ck_15;
  int nr_10_St_12_Active;
  Main__st_12 ns_10_St_12_Active;
  int t3_3_St_12_Active;
  int t2_3_St_12_Active;
  int t1_3_St_12_Active;
  int act_3_St_12_Active;
  int nr_10_St_12_Idle;
  Main__st_12 ns_10_St_12_Idle;
  int t3_3_St_12_Idle;
  int t2_3_St_12_Idle;
  int t1_3_St_12_Idle;
  int act_3_St_12_Idle;
  Main__st_12 ck_14;
  int r_9_St_11_Active;
  Main__st_11 s_9_St_11_Active;
  int r_9_St_11_Idle;
  Main__st_11 s_9_St_11_Idle;
  int t3_4_St_11_Active_H3;
  int t2_4_St_11_Active_H3;
  int t1_4_St_11_Active_H3;
  int t3_4_St_11_Active_H2;
  int t2_4_St_11_Active_H2;
  int t1_4_St_11_Active_H2;
  int t3_4_St_11_Active_H1;
  int t2_4_St_11_Active_H1;
  int t1_4_St_11_Active_H1;
  int t3_4_St_11_Active_S;
  int t2_4_St_11_Active_S;
  int t1_4_St_11_Active_S;
  Main__version_type ck_18;
  int nr_9_St_11_Active;
  Main__st_11 ns_9_St_11_Active;
  int t3_4_St_11_Active;
  int t2_4_St_11_Active;
  int t1_4_St_11_Active;
  int act_4_St_11_Active;
  int nr_9_St_11_Idle;
  Main__st_11 ns_9_St_11_Idle;
  int t3_4_St_11_Idle;
  int t2_4_St_11_Idle;
  int t1_4_St_11_Idle;
  int act_4_St_11_Idle;
  Main__st_11 ck_17;
  int r_8_St_10_Active;
  Main__st_10 s_8_St_10_Active;
  int r_8_St_10_Idle;
  Main__st_10 s_8_St_10_Idle;
  int t3_5_St_10_Active_H3;
  int t2_5_St_10_Active_H3;
  int t1_5_St_10_Active_H3;
  int t3_5_St_10_Active_H2;
  int t2_5_St_10_Active_H2;
  int t1_5_St_10_Active_H2;
  int t3_5_St_10_Active_H1;
  int t2_5_St_10_Active_H1;
  int t1_5_St_10_Active_H1;
  int t3_5_St_10_Active_S;
  int t2_5_St_10_Active_S;
  int t1_5_St_10_Active_S;
  Main__version_type ck_21;
  int nr_8_St_10_Active;
  Main__st_10 ns_8_St_10_Active;
  int t3_5_St_10_Active;
  int t2_5_St_10_Active;
  int t1_5_St_10_Active;
  int act_5_St_10_Active;
  int nr_8_St_10_Idle;
  Main__st_10 ns_8_St_10_Idle;
  int t3_5_St_10_Idle;
  int t2_5_St_10_Idle;
  int t1_5_St_10_Idle;
  int act_5_St_10_Idle;
  Main__st_10 ck_20;
  int r_7_St_9_Active;
  Main__st_9 s_7_St_9_Active;
  int r_7_St_9_Idle;
  Main__st_9 s_7_St_9_Idle;
  int t3_6_St_9_Active_H3;
  int t2_6_St_9_Active_H3;
  int t1_6_St_9_Active_H3;
  int t3_6_St_9_Active_H2;
  int t2_6_St_9_Active_H2;
  int t1_6_St_9_Active_H2;
  int t3_6_St_9_Active_H1;
  int t2_6_St_9_Active_H1;
  int t1_6_St_9_Active_H1;
  int t3_6_St_9_Active_S;
  int t2_6_St_9_Active_S;
  int t1_6_St_9_Active_S;
  Main__version_type ck_24;
  int nr_7_St_9_Active;
  Main__st_9 ns_7_St_9_Active;
  int t3_6_St_9_Active;
  int t2_6_St_9_Active;
  int t1_6_St_9_Active;
  int act_6_St_9_Active;
  int nr_7_St_9_Idle;
  Main__st_9 ns_7_St_9_Idle;
  int t3_6_St_9_Idle;
  int t2_6_St_9_Idle;
  int t1_6_St_9_Idle;
  int act_6_St_9_Idle;
  Main__st_9 ck_23;
  int r_6_St_8_Active;
  Main__st_8 s_6_St_8_Active;
  int r_6_St_8_Idle;
  Main__st_8 s_6_St_8_Idle;
  int t3_7_St_8_Active_H3;
  int t2_7_St_8_Active_H3;
  int t1_7_St_8_Active_H3;
  int t3_7_St_8_Active_H2;
  int t2_7_St_8_Active_H2;
  int t1_7_St_8_Active_H2;
  int t3_7_St_8_Active_H1;
  int t2_7_St_8_Active_H1;
  int t1_7_St_8_Active_H1;
  int t3_7_St_8_Active_S;
  int t2_7_St_8_Active_S;
  int t1_7_St_8_Active_S;
  Main__version_type ck_27;
  int nr_6_St_8_Active;
  Main__st_8 ns_6_St_8_Active;
  int t3_7_St_8_Active;
  int t2_7_St_8_Active;
  int t1_7_St_8_Active;
  int act_7_St_8_Active;
  int nr_6_St_8_Idle;
  Main__st_8 ns_6_St_8_Idle;
  int t3_7_St_8_Idle;
  int t2_7_St_8_Idle;
  int t1_7_St_8_Idle;
  int act_7_St_8_Idle;
  Main__st_8 ck_26;
  int r_5_St_7_Active;
  Main__st_7 s_5_St_7_Active;
  int r_5_St_7_Idle;
  Main__st_7 s_5_St_7_Idle;
  int t3_8_St_7_Active_H3;
  int t2_8_St_7_Active_H3;
  int t1_8_St_7_Active_H3;
  int t3_8_St_7_Active_H2;
  int t2_8_St_7_Active_H2;
  int t1_8_St_7_Active_H2;
  int t3_8_St_7_Active_H1;
  int t2_8_St_7_Active_H1;
  int t1_8_St_7_Active_H1;
  int t3_8_St_7_Active_S;
  int t2_8_St_7_Active_S;
  int t1_8_St_7_Active_S;
  Main__version_type ck_30;
  int nr_5_St_7_Active;
  Main__st_7 ns_5_St_7_Active;
  int t3_8_St_7_Active;
  int t2_8_St_7_Active;
  int t1_8_St_7_Active;
  int act_8_St_7_Active;
  int nr_5_St_7_Idle;
  Main__st_7 ns_5_St_7_Idle;
  int t3_8_St_7_Idle;
  int t2_8_St_7_Idle;
  int t1_8_St_7_Idle;
  int act_8_St_7_Idle;
  Main__st_7 ck_29;
  int r_4_St_6_Active;
  Main__st_6 s_4_St_6_Active;
  int r_4_St_6_Idle;
  Main__st_6 s_4_St_6_Idle;
  int t3_9_St_6_Active_H3;
  int t2_9_St_6_Active_H3;
  int t1_9_St_6_Active_H3;
  int t3_9_St_6_Active_H2;
  int t2_9_St_6_Active_H2;
  int t1_9_St_6_Active_H2;
  int t3_9_St_6_Active_H1;
  int t2_9_St_6_Active_H1;
  int t1_9_St_6_Active_H1;
  int t3_9_St_6_Active_S;
  int t2_9_St_6_Active_S;
  int t1_9_St_6_Active_S;
  Main__version_type ck_33;
  int nr_4_St_6_Active;
  Main__st_6 ns_4_St_6_Active;
  int t3_9_St_6_Active;
  int t2_9_St_6_Active;
  int t1_9_St_6_Active;
  int act_9_St_6_Active;
  int nr_4_St_6_Idle;
  Main__st_6 ns_4_St_6_Idle;
  int t3_9_St_6_Idle;
  int t2_9_St_6_Idle;
  int t1_9_St_6_Idle;
  int act_9_St_6_Idle;
  Main__st_6 ck_32;
  int r_3_St_5_Active;
  Main__st_5 s_3_St_5_Active;
  int r_3_St_5_Idle;
  Main__st_5 s_3_St_5_Idle;
  int t3_10_St_5_Active_H3;
  int t2_10_St_5_Active_H3;
  int t1_10_St_5_Active_H3;
  int t3_10_St_5_Active_H2;
  int t2_10_St_5_Active_H2;
  int t1_10_St_5_Active_H2;
  int t3_10_St_5_Active_H1;
  int t2_10_St_5_Active_H1;
  int t1_10_St_5_Active_H1;
  int t3_10_St_5_Active_S;
  int t2_10_St_5_Active_S;
  int t1_10_St_5_Active_S;
  Main__version_type ck_36;
  int nr_3_St_5_Active;
  Main__st_5 ns_3_St_5_Active;
  int t3_10_St_5_Active;
  int t2_10_St_5_Active;
  int t1_10_St_5_Active;
  int act_10_St_5_Active;
  int nr_3_St_5_Idle;
  Main__st_5 ns_3_St_5_Idle;
  int t3_10_St_5_Idle;
  int t2_10_St_5_Idle;
  int t1_10_St_5_Idle;
  int act_10_St_5_Idle;
  Main__st_5 ck_35;
  int r_2_St_4_Fail;
  Main__st_4 s_2_St_4_Fail;
  int r_2_St_4_Free;
  Main__st_4 s_2_St_4_Free;
  int nr_2_St_4_Fail;
  Main__st_4 ns_2_St_4_Fail;
  int max_occ_St_4_Fail;
  int err_St_4_Fail;
  int nr_2_St_4_Free;
  Main__st_4 ns_2_St_4_Free;
  int max_occ_St_4_Free;
  int err_St_4_Free;
  Main__st_4 ck_38;
  int r_1_St_3_Fail;
  Main__st_3 s_1_St_3_Fail;
  int r_1_St_3_Free;
  Main__st_3 s_1_St_3_Free;
  int nr_1_St_3_Fail;
  Main__st_3 ns_1_St_3_Fail;
  int max_occ_1_St_3_Fail;
  int err_4_St_3_Fail;
  int nr_1_St_3_Free;
  Main__st_3 ns_1_St_3_Free;
  int max_occ_1_St_3_Free;
  int err_4_St_3_Free;
  Main__st_3 ck_40;
  int r_St_2_Fail;
  Main__st_2 s_St_2_Fail;
  int r_St_2_Free;
  Main__st_2 s_St_2_Free;
  int nr_St_2_Fail;
  Main__st_2 ns_St_2_Fail;
  int max_occ_2_St_2_Fail;
  int err_5_St_2_Fail;
  int nr_St_2_Free;
  Main__st_2 ns_St_2_Free;
  int max_occ_2_St_2_Free;
  int err_5_St_2_Free;
  Main__st_2 ck_42;
  int v_110;
  int v_109;
  int v_108;
  int v_107;
  int v_106;
  int v_105;
  int v_104;
  int v_103;
  int v_102;
  int v_101;
  int v_100;
  int v_99;
  int v_98;
  int v_97;
  int v_96;
  int v_95;
  int v_94;
  int v_93;
  int v_92;
  int v_91;
  int v_90;
  int v_89;
  int v_88;
  int v_87;
  int v_86;
  int v_85;
  int v_84;
  int v_83;
  int v_82;
  int v_81;
  int v_80;
  int v_79;
  int v_78;
  int v_77;
  int v_76;
  int v_75;
  int v_74;
  int v_73;
  int v_72;
  int v_71;
  int v_70;
  int v_69;
  int v_68;
  int v_67;
  int v_66;
  int v_65;
  int v_64;
  int v_63;
  int v_62;
  int v_61;
  int v_60;
  int v_59;
  int v_58;
  int v_57;
  int v_56;
  int v_55;
  int v_54;
  int v_53;
  int v_52;
  int v_51;
  int v_50;
  int v_49;
  int v_48;
  int v_47;
  int v_46;
  int v_45;
  int v_44;
  int v_43;
  int v_42;
  int v_41;
  int v_40;
  int v_39;
  int v_38;
  int v_37;
  int v_36;
  int v_35;
  int v_34;
  int v_33;
  int v_32;
  int v_31;
  int v_30;
  int v_29;
  int v_28;
  int v_27;
  int v_26;
  int v_25;
  int v_24;
  int v_23;
  int v_22;
  int v_21;
  int v_20;
  int v_19;
  int v_18;
  int v_17;
  int v_16;
  int v_15;
  int v_14;
  int v_13;
  int v_12;
  int v_11;
  int v_10;
  int v_9;
  int v_8;
  int v_7;
  int v_6;
  int v_5;
  int v_4;
  Main__st_15 s_13;
  Main__st_15 ns_13;
  int r_13;
  int nr_13;
  Main__st_14 s_12;
  Main__st_14 ns_12;
  int r_12;
  int nr_12;
  Main__st_13 s_11;
  Main__st_13 ns_11;
  int r_11;
  int nr_11;
  Main__st_12 s_10;
  Main__st_12 ns_10;
  int r_10;
  int nr_10;
  Main__st_11 s_9;
  Main__st_11 ns_9;
  int r_9;
  int nr_9;
  Main__st_10 s_8;
  Main__st_10 ns_8;
  int r_8;
  int nr_8;
  Main__st_9 s_7;
  Main__st_9 ns_7;
  int r_7;
  int nr_7;
  Main__st_8 s_6;
  Main__st_8 ns_6;
  int r_6;
  int nr_6;
  Main__st_7 s_5;
  Main__st_7 ns_5;
  int r_5;
  int nr_5;
  Main__st_6 s_4;
  Main__st_6 ns_4;
  int r_4;
  int nr_4;
  Main__st_5 s_3;
  Main__st_5 ns_3;
  int r_3;
  int nr_3;
  Main__st_4 s_2;
  Main__st_4 ns_2;
  int r_2;
  int nr_2;
  Main__st_3 s_1;
  Main__st_3 ns_1;
  int r_1;
  int nr_1;
  Main__st_2 s;
  Main__st_2 ns;
  int r;
  int nr;
  int pref_ver_task_assume_10;
  int pref_ver_task_guarantee_10;
  int pref_ver_task_assume_9;
  int pref_ver_task_guarantee_9;
  int pref_ver_task_assume_8;
  int pref_ver_task_guarantee_8;
  int pref_ver_task_assume_7;
  int pref_ver_task_guarantee_7;
  int pref_ver_task_assume_6;
  int pref_ver_task_guarantee_6;
  int pref_ver_task_assume_5;
  int pref_ver_task_guarantee_5;
  int pref_ver_task_assume_4;
  int pref_ver_task_guarantee_4;
  int pref_ver_task_assume_3;
  int pref_ver_task_guarantee_3;
  int pref_ver_task_assume_2;
  int pref_ver_task_guarantee_2;
  int pref_ver_task_assume_1;
  int pref_ver_task_guarantee_1;
  int pref_ver_task_assume;
  int pref_ver_task_guarantee;
  Main__version_type task_ver_22;
  int current_texec_10;
  int tmin_22;
  int tmax_22;
  Main__version_type pref_ver_22;
  int obj_22;
  Main__version_type task_ver_21;
  int current_texec_9;
  int tmin_21;
  int tmax_21;
  Main__version_type pref_ver_21;
  int obj_21;
  Main__version_type task_ver_20;
  int current_texec_8;
  int tmin_20;
  int tmax_20;
  Main__version_type pref_ver_20;
  int obj_20;
  Main__version_type task_ver_19;
  int current_texec_7;
  int tmin_19;
  int tmax_19;
  Main__version_type pref_ver_19;
  int obj_19;
  Main__version_type task_ver_18;
  int current_texec_6;
  int tmin_18;
  int tmax_18;
  Main__version_type pref_ver_18;
  int obj_18;
  Main__version_type task_ver_17;
  int current_texec_5;
  int tmin_17;
  int tmax_17;
  Main__version_type pref_ver_17;
  int obj_17;
  Main__version_type task_ver_16;
  int current_texec_4;
  int tmin_16;
  int tmax_16;
  Main__version_type pref_ver_16;
  int obj_16;
  Main__version_type task_ver_15;
  int current_texec_3;
  int tmin_15;
  int tmax_15;
  Main__version_type pref_ver_15;
  int obj_15;
  Main__version_type task_ver_14;
  int current_texec_2;
  int tmin_14;
  int tmax_14;
  Main__version_type pref_ver_14;
  int obj_14;
  Main__version_type task_ver_13;
  int current_texec_1;
  int tmin_13;
  int tmax_13;
  Main__version_type pref_ver_13;
  int obj_13;
  Main__version_type task_ver_12;
  int current_texec;
  int tmin_12;
  int tmax_12;
  Main__version_type pref_ver_12;
  int obj_12;
  int f_5;
  int rp_5;
  int err_5;
  int max_occ_2;
  int f_4;
  int rp_4;
  int err_4;
  int max_occ_1;
  int f;
  int rp;
  int err;
  int max_occ;
  int req_task_10;
  int end_task_10;
  Main__version_type task_ver_10;
  int texe_10;
  int tmin_10;
  int tmax_10;
  int obj_10;
  int act_10;
  int wt_10;
  int t1_10;
  int t2_10;
  int t3_10;
  Main__version_type pref_ver_10;
  int req_task_9;
  int end_task_9;
  Main__version_type task_ver_9;
  int texe_9;
  int tmin_9;
  int tmax_9;
  int obj_9;
  int act_9;
  int wt_9;
  int t1_9;
  int t2_9;
  int t3_9;
  Main__version_type pref_ver_9;
  int req_task_8;
  int end_task_8;
  Main__version_type task_ver_8;
  int texe_8;
  int tmin_8;
  int tmax_8;
  int obj_8;
  int act_8;
  int wt_8;
  int t1_8;
  int t2_8;
  int t3_8;
  Main__version_type pref_ver_8;
  int req_task_7;
  int end_task_7;
  Main__version_type task_ver_7;
  int texe_7;
  int tmin_7;
  int tmax_7;
  int obj_7;
  int act_7;
  int wt_7;
  int t1_7;
  int t2_7;
  int t3_7;
  Main__version_type pref_ver_7;
  int req_task_6;
  int end_task_6;
  Main__version_type task_ver_6;
  int texe_6;
  int tmin_6;
  int tmax_6;
  int obj_6;
  int act_6;
  int wt_6;
  int t1_6;
  int t2_6;
  int t3_6;
  Main__version_type pref_ver_6;
  int req_task_5;
  int end_task_5;
  Main__version_type task_ver_5;
  int texe_5;
  int tmin_5;
  int tmax_5;
  int obj_5;
  int act_5;
  int wt_5;
  int t1_5;
  int t2_5;
  int t3_5;
  Main__version_type pref_ver_5;
  int req_task_4;
  int end_task_4;
  Main__version_type task_ver_4;
  int texe_4;
  int tmin_4;
  int tmax_4;
  int obj_4;
  int act_4;
  int wt_4;
  int t1_4;
  int t2_4;
  int t3_4;
  Main__version_type pref_ver_4;
  int req_task_3;
  int end_task_3;
  Main__version_type task_ver_3;
  int texe_3;
  int tmin_3;
  int tmax_3;
  int obj_3;
  int act_3;
  int wt_3;
  int t1_3;
  int t2_3;
  int t3_3;
  Main__version_type pref_ver_3;
  int req_task_2;
  int end_task_2;
  Main__version_type task_ver_2;
  int texe_2;
  int tmin_2;
  int tmax_2;
  int obj_2;
  int act_2;
  int wt_2;
  int t1_2;
  int t2_2;
  int t3_2;
  Main__version_type pref_ver_2;
  int req_task_1;
  int end_task_1;
  Main__version_type task_ver_1;
  int texe_1;
  int tmin_1;
  int tmax_1;
  int obj_1;
  int act_1;
  int wt_1;
  int t1_1;
  int t2_1;
  int t3_1;
  Main__version_type pref_ver_1;
  int req_task;
  int end_task;
  Main__version_type task_ver;
  int texe;
  int tmin;
  int tmax;
  int obj;
  int act;
  int wt;
  int t1;
  int t2;
  int t3;
  Main__version_type pref_ver;
  int err_1;
  int err_2;
  int err_3;
  int c_img_t1;
  int c_img_t2;
  int c_img_t3;
  int me_imu_t1;
  int me_imu_t2;
  int me_imu_t3;
  int me_img_t1;
  int me_img_t2;
  int me_img_t3;
  int sl_t1;
  int sl_t2;
  int sl_t3;
  int oa_t1;
  int oa_t2;
  int oa_t3;
  int tl_t1;
  int tl_t2;
  int tl_t3;
  int rs_t1;
  int rs_t2;
  int rs_t3;
  int rb_t1;
  int rb_t2;
  int rb_t3;
  int dt_t1;
  int dt_t2;
  int dt_t3;
  int rpl_t1;
  int rpl_t2;
  int rpl_t3;
  int trk_t1;
  int trk_t2;
  int trk_t3;
  int occ_t1;
  int occ_t2;
  int occ_t3;
  int max_occ_t1;
  int max_occ_t2;
  int max_occ_t3;
  Main__version_type c_img_pref;
  Main__version_type me_imu_pref;
  Main__version_type me_img_pref;
  Main__version_type sl_pref;
  Main__version_type oa_pref;
  Main__version_type tl_pref;
  Main__version_type rs_pref;
  Main__version_type rb_pref;
  Main__version_type dt_pref;
  Main__version_type rpl_pref;
  Main__version_type trk_pref;
  pref_ver_task_assume_10 = true;
  v_4 = !(pref_ver_task_assume_10);
  Main_controller__main_ctrlr0_step(c_img_e, c_img_r, self->ck, self->ck_10,
                                    self->ck_13, self->ck_16, self->ck_19,
                                    self->ck_22, self->ck_25, self->ck_28,
                                    self->ck_31, self->ck_34, self->ck_37,
                                    self->ck_39, self->ck_41, self->ck_7,
                                    dt_e, dt_r, f_1, f_2, f_3, max1, max10,
                                    max11, max2, max3, max4, max5, max6,
                                    max7, max8, max9, me_img_e, me_img_r,
                                    me_imu_e, me_imu_r, min1, min10, min11,
                                    min2, min3, min4, min5, min6, min7, min8,
                                    min9, oa_e, oa_r, self->pnr, self->pnr_1,
                                    self->pnr_10, self->pnr_11, self->pnr_12,
                                    self->pnr_13, self->pnr_2, self->pnr_3,
                                    self->pnr_4, self->pnr_5, self->pnr_6,
                                    self->pnr_7, self->pnr_8, self->pnr_9,
                                    self->pref_ver_task_assume_10_1,
                                    self->pref_ver_task_assume_12,
                                    self->pref_ver_task_assume_1_1,
                                    self->pref_ver_task_assume_2_1,
                                    self->pref_ver_task_assume_3_1,
                                    self->pref_ver_task_assume_4_1,
                                    self->pref_ver_task_assume_5_1,
                                    self->pref_ver_task_assume_6_1,
                                    self->pref_ver_task_assume_7_1,
                                    self->pref_ver_task_assume_8_1,
                                    self->pref_ver_task_assume_9_1,
                                    self->pref_ver_task_guarantee_10_1,
                                    self->pref_ver_task_guarantee_12,
                                    self->pref_ver_task_guarantee_1_1,
                                    self->pref_ver_task_guarantee_2_1,
                                    self->pref_ver_task_guarantee_3_1,
                                    self->pref_ver_task_guarantee_4_1,
                                    self->pref_ver_task_guarantee_5_1,
                                    self->pref_ver_task_guarantee_6_1,
                                    self->pref_ver_task_guarantee_7_1,
                                    self->pref_ver_task_guarantee_8_1,
                                    self->pref_ver_task_guarantee_9_1, rb_e,
                                    rb_r, rp_1, rp_2, rp_3, rpl_e, rpl_r,
                                    rs_e, rs_r, sl_e, sl_r, texe1, texe10,
                                    texe11, texe2, texe3, texe4, texe5,
                                    texe6, texe7, texe8, texe9, tl_e, tl_r,
                                    trk_e, trk_r,
                                    &Main_controller__main_ctrlr0_out_st);
  c_c_img_ver = Main_controller__main_ctrlr0_out_st.c_c_img_ver;
  _out->c_img_ver = c_c_img_ver;
  task_ver_10 = _out->c_img_ver;
  task_ver_12 = task_ver_10;
  texe_10 = texe1;
  current_texec = texe_10;
  tmin_10 = min1;
  tmin_12 = tmin_10;
  tmax_10 = max1;
  tmax_12 = tmax_10;
  Main__pref_ver_task_step(task_ver_12, current_texec, tmin_12, tmax_12,
                           &Main__pref_ver_task_out_st, &self->pref_ver_task);
  pref_ver_12 = Main__pref_ver_task_out_st.pref_ver;
  obj_12 = Main__pref_ver_task_out_st.obj;
  Main_controller__main_ctrlr1_step(c_c_img_ver, c_img_e, c_img_r, self->ck,
                                    self->ck_10, self->ck_13, self->ck_16,
                                    self->ck_19, self->ck_22, self->ck_25,
                                    self->ck_28, self->ck_31, self->ck_34,
                                    self->ck_37, self->ck_39, self->ck_41,
                                    self->ck_7, dt_e, dt_r, f_1, f_2, f_3,
                                    max1, max10, max11, max2, max3, max4,
                                    max5, max6, max7, max8, max9, me_img_e,
                                    me_img_r, me_imu_e, me_imu_r, min1,
                                    min10, min11, min2, min3, min4, min5,
                                    min6, min7, min8, min9, oa_e, oa_r,
                                    obj_12, self->pnr, self->pnr_1,
                                    self->pnr_10, self->pnr_11, self->pnr_12,
                                    self->pnr_13, self->pnr_2, self->pnr_3,
                                    self->pnr_4, self->pnr_5, self->pnr_6,
                                    self->pnr_7, self->pnr_8, self->pnr_9,
                                    pref_ver_12,
                                    self->pref_ver_task_assume_10_1,
                                    self->pref_ver_task_assume_12,
                                    self->pref_ver_task_assume_1_1,
                                    self->pref_ver_task_assume_2_1,
                                    self->pref_ver_task_assume_3_1,
                                    self->pref_ver_task_assume_4_1,
                                    self->pref_ver_task_assume_5_1,
                                    self->pref_ver_task_assume_6_1,
                                    self->pref_ver_task_assume_7_1,
                                    self->pref_ver_task_assume_8_1,
                                    self->pref_ver_task_assume_9_1,
                                    self->pref_ver_task_guarantee_10_1,
                                    self->pref_ver_task_guarantee_12,
                                    self->pref_ver_task_guarantee_1_1,
                                    self->pref_ver_task_guarantee_2_1,
                                    self->pref_ver_task_guarantee_3_1,
                                    self->pref_ver_task_guarantee_4_1,
                                    self->pref_ver_task_guarantee_5_1,
                                    self->pref_ver_task_guarantee_6_1,
                                    self->pref_ver_task_guarantee_7_1,
                                    self->pref_ver_task_guarantee_8_1,
                                    self->pref_ver_task_guarantee_9_1, rb_e,
                                    rb_r, rp_1, rp_2, rp_3, rpl_e, rpl_r,
                                    rs_e, rs_r, sl_e, sl_r, texe1, texe10,
                                    texe11, texe2, texe3, texe4, texe5,
                                    texe6, texe7, texe8, texe9, tl_e, tl_r,
                                    trk_e, trk_r,
                                    &Main_controller__main_ctrlr1_out_st);
  c_me_imu_ver = Main_controller__main_ctrlr1_out_st.c_me_imu_ver;
  _out->me_imu_ver = c_me_imu_ver;
  task_ver_9 = _out->me_imu_ver;
  task_ver_13 = task_ver_9;
  texe_9 = texe2;
  current_texec_1 = texe_9;
  tmin_9 = min2;
  tmin_13 = tmin_9;
  tmax_9 = max2;
  tmax_13 = tmax_9;
  Main__pref_ver_task_step(task_ver_13, current_texec_1, tmin_13, tmax_13,
                           &Main__pref_ver_task_out_st,
                           &self->pref_ver_task_1);
  pref_ver_13 = Main__pref_ver_task_out_st.pref_ver;
  obj_13 = Main__pref_ver_task_out_st.obj;
  Main_controller__main_ctrlr2_step(c_c_img_ver, c_img_e, c_img_r,
                                    c_me_imu_ver, self->ck, self->ck_10,
                                    self->ck_13, self->ck_16, self->ck_19,
                                    self->ck_22, self->ck_25, self->ck_28,
                                    self->ck_31, self->ck_34, self->ck_37,
                                    self->ck_39, self->ck_41, self->ck_7,
                                    dt_e, dt_r, f_1, f_2, f_3, max1, max10,
                                    max11, max2, max3, max4, max5, max6,
                                    max7, max8, max9, me_img_e, me_img_r,
                                    me_imu_e, me_imu_r, min1, min10, min11,
                                    min2, min3, min4, min5, min6, min7, min8,
                                    min9, oa_e, oa_r, obj_12, obj_13,
                                    self->pnr, self->pnr_1, self->pnr_10,
                                    self->pnr_11, self->pnr_12, self->pnr_13,
                                    self->pnr_2, self->pnr_3, self->pnr_4,
                                    self->pnr_5, self->pnr_6, self->pnr_7,
                                    self->pnr_8, self->pnr_9, pref_ver_12,
                                    pref_ver_13,
                                    self->pref_ver_task_assume_10_1,
                                    self->pref_ver_task_assume_12,
                                    self->pref_ver_task_assume_1_1,
                                    self->pref_ver_task_assume_2_1,
                                    self->pref_ver_task_assume_3_1,
                                    self->pref_ver_task_assume_4_1,
                                    self->pref_ver_task_assume_5_1,
                                    self->pref_ver_task_assume_6_1,
                                    self->pref_ver_task_assume_7_1,
                                    self->pref_ver_task_assume_8_1,
                                    self->pref_ver_task_assume_9_1,
                                    self->pref_ver_task_guarantee_10_1,
                                    self->pref_ver_task_guarantee_12,
                                    self->pref_ver_task_guarantee_1_1,
                                    self->pref_ver_task_guarantee_2_1,
                                    self->pref_ver_task_guarantee_3_1,
                                    self->pref_ver_task_guarantee_4_1,
                                    self->pref_ver_task_guarantee_5_1,
                                    self->pref_ver_task_guarantee_6_1,
                                    self->pref_ver_task_guarantee_7_1,
                                    self->pref_ver_task_guarantee_8_1,
                                    self->pref_ver_task_guarantee_9_1, rb_e,
                                    rb_r, rp_1, rp_2, rp_3, rpl_e, rpl_r,
                                    rs_e, rs_r, sl_e, sl_r, texe1, texe10,
                                    texe11, texe2, texe3, texe4, texe5,
                                    texe6, texe7, texe8, texe9, tl_e, tl_r,
                                    trk_e, trk_r,
                                    &Main_controller__main_ctrlr2_out_st);
  c_me_img_ver = Main_controller__main_ctrlr2_out_st.c_me_img_ver;
  _out->me_img_ver = c_me_img_ver;
  task_ver_8 = _out->me_img_ver;
  task_ver_14 = task_ver_8;
  texe_8 = texe3;
  current_texec_2 = texe_8;
  tmin_8 = min3;
  tmin_14 = tmin_8;
  tmax_8 = max3;
  tmax_14 = tmax_8;
  Main__pref_ver_task_step(task_ver_14, current_texec_2, tmin_14, tmax_14,
                           &Main__pref_ver_task_out_st,
                           &self->pref_ver_task_2);
  pref_ver_14 = Main__pref_ver_task_out_st.pref_ver;
  obj_14 = Main__pref_ver_task_out_st.obj;
  Main_controller__main_ctrlr3_step(c_c_img_ver, c_img_e, c_img_r,
                                    c_me_img_ver, c_me_imu_ver, self->ck,
                                    self->ck_10, self->ck_13, self->ck_16,
                                    self->ck_19, self->ck_22, self->ck_25,
                                    self->ck_28, self->ck_31, self->ck_34,
                                    self->ck_37, self->ck_39, self->ck_41,
                                    self->ck_7, dt_e, dt_r, f_1, f_2, f_3,
                                    max1, max10, max11, max2, max3, max4,
                                    max5, max6, max7, max8, max9, me_img_e,
                                    me_img_r, me_imu_e, me_imu_r, min1,
                                    min10, min11, min2, min3, min4, min5,
                                    min6, min7, min8, min9, oa_e, oa_r,
                                    obj_12, obj_13, obj_14, self->pnr,
                                    self->pnr_1, self->pnr_10, self->pnr_11,
                                    self->pnr_12, self->pnr_13, self->pnr_2,
                                    self->pnr_3, self->pnr_4, self->pnr_5,
                                    self->pnr_6, self->pnr_7, self->pnr_8,
                                    self->pnr_9, pref_ver_12, pref_ver_13,
                                    pref_ver_14,
                                    self->pref_ver_task_assume_10_1,
                                    self->pref_ver_task_assume_12,
                                    self->pref_ver_task_assume_1_1,
                                    self->pref_ver_task_assume_2_1,
                                    self->pref_ver_task_assume_3_1,
                                    self->pref_ver_task_assume_4_1,
                                    self->pref_ver_task_assume_5_1,
                                    self->pref_ver_task_assume_6_1,
                                    self->pref_ver_task_assume_7_1,
                                    self->pref_ver_task_assume_8_1,
                                    self->pref_ver_task_assume_9_1,
                                    self->pref_ver_task_guarantee_10_1,
                                    self->pref_ver_task_guarantee_12,
                                    self->pref_ver_task_guarantee_1_1,
                                    self->pref_ver_task_guarantee_2_1,
                                    self->pref_ver_task_guarantee_3_1,
                                    self->pref_ver_task_guarantee_4_1,
                                    self->pref_ver_task_guarantee_5_1,
                                    self->pref_ver_task_guarantee_6_1,
                                    self->pref_ver_task_guarantee_7_1,
                                    self->pref_ver_task_guarantee_8_1,
                                    self->pref_ver_task_guarantee_9_1, rb_e,
                                    rb_r, rp_1, rp_2, rp_3, rpl_e, rpl_r,
                                    rs_e, rs_r, sl_e, sl_r, texe1, texe10,
                                    texe11, texe2, texe3, texe4, texe5,
                                    texe6, texe7, texe8, texe9, tl_e, tl_r,
                                    trk_e, trk_r,
                                    &Main_controller__main_ctrlr3_out_st);
  c_sl_ver = Main_controller__main_ctrlr3_out_st.c_sl_ver;
  _out->sl_ver = c_sl_ver;
  task_ver_7 = _out->sl_ver;
  task_ver_15 = task_ver_7;
  texe_7 = texe4;
  current_texec_3 = texe_7;
  tmin_7 = min4;
  tmin_15 = tmin_7;
  tmax_7 = max4;
  tmax_15 = tmax_7;
  Main__pref_ver_task_step(task_ver_15, current_texec_3, tmin_15, tmax_15,
                           &Main__pref_ver_task_out_st,
                           &self->pref_ver_task_3);
  pref_ver_15 = Main__pref_ver_task_out_st.pref_ver;
  obj_15 = Main__pref_ver_task_out_st.obj;
  Main_controller__main_ctrlr4_step(c_c_img_ver, c_img_e, c_img_r,
                                    c_me_img_ver, c_me_imu_ver, c_sl_ver,
                                    self->ck, self->ck_10, self->ck_13,
                                    self->ck_16, self->ck_19, self->ck_22,
                                    self->ck_25, self->ck_28, self->ck_31,
                                    self->ck_34, self->ck_37, self->ck_39,
                                    self->ck_41, self->ck_7, dt_e, dt_r, f_1,
                                    f_2, f_3, max1, max10, max11, max2, max3,
                                    max4, max5, max6, max7, max8, max9,
                                    me_img_e, me_img_r, me_imu_e, me_imu_r,
                                    min1, min10, min11, min2, min3, min4,
                                    min5, min6, min7, min8, min9, oa_e, oa_r,
                                    obj_12, obj_13, obj_14, obj_15,
                                    self->pnr, self->pnr_1, self->pnr_10,
                                    self->pnr_11, self->pnr_12, self->pnr_13,
                                    self->pnr_2, self->pnr_3, self->pnr_4,
                                    self->pnr_5, self->pnr_6, self->pnr_7,
                                    self->pnr_8, self->pnr_9, pref_ver_12,
                                    pref_ver_13, pref_ver_14, pref_ver_15,
                                    self->pref_ver_task_assume_10_1,
                                    self->pref_ver_task_assume_12,
                                    self->pref_ver_task_assume_1_1,
                                    self->pref_ver_task_assume_2_1,
                                    self->pref_ver_task_assume_3_1,
                                    self->pref_ver_task_assume_4_1,
                                    self->pref_ver_task_assume_5_1,
                                    self->pref_ver_task_assume_6_1,
                                    self->pref_ver_task_assume_7_1,
                                    self->pref_ver_task_assume_8_1,
                                    self->pref_ver_task_assume_9_1,
                                    self->pref_ver_task_guarantee_10_1,
                                    self->pref_ver_task_guarantee_12,
                                    self->pref_ver_task_guarantee_1_1,
                                    self->pref_ver_task_guarantee_2_1,
                                    self->pref_ver_task_guarantee_3_1,
                                    self->pref_ver_task_guarantee_4_1,
                                    self->pref_ver_task_guarantee_5_1,
                                    self->pref_ver_task_guarantee_6_1,
                                    self->pref_ver_task_guarantee_7_1,
                                    self->pref_ver_task_guarantee_8_1,
                                    self->pref_ver_task_guarantee_9_1, rb_e,
                                    rb_r, rp_1, rp_2, rp_3, rpl_e, rpl_r,
                                    rs_e, rs_r, sl_e, sl_r, texe1, texe10,
                                    texe11, texe2, texe3, texe4, texe5,
                                    texe6, texe7, texe8, texe9, tl_e, tl_r,
                                    trk_e, trk_r,
                                    &Main_controller__main_ctrlr4_out_st);
  c_oa_ver = Main_controller__main_ctrlr4_out_st.c_oa_ver;
  _out->oa_ver = c_oa_ver;
  task_ver_6 = _out->oa_ver;
  task_ver_16 = task_ver_6;
  texe_6 = texe5;
  current_texec_4 = texe_6;
  tmin_6 = min5;
  tmin_16 = tmin_6;
  tmax_6 = max5;
  tmax_16 = tmax_6;
  Main__pref_ver_task_step(task_ver_16, current_texec_4, tmin_16, tmax_16,
                           &Main__pref_ver_task_out_st,
                           &self->pref_ver_task_4);
  pref_ver_16 = Main__pref_ver_task_out_st.pref_ver;
  obj_16 = Main__pref_ver_task_out_st.obj;
  Main_controller__main_ctrlr5_step(c_c_img_ver, c_img_e, c_img_r,
                                    c_me_img_ver, c_me_imu_ver, c_oa_ver,
                                    c_sl_ver, self->ck, self->ck_10,
                                    self->ck_13, self->ck_16, self->ck_19,
                                    self->ck_22, self->ck_25, self->ck_28,
                                    self->ck_31, self->ck_34, self->ck_37,
                                    self->ck_39, self->ck_41, self->ck_7,
                                    dt_e, dt_r, f_1, f_2, f_3, max1, max10,
                                    max11, max2, max3, max4, max5, max6,
                                    max7, max8, max9, me_img_e, me_img_r,
                                    me_imu_e, me_imu_r, min1, min10, min11,
                                    min2, min3, min4, min5, min6, min7, min8,
                                    min9, oa_e, oa_r, obj_12, obj_13, obj_14,
                                    obj_15, obj_16, self->pnr, self->pnr_1,
                                    self->pnr_10, self->pnr_11, self->pnr_12,
                                    self->pnr_13, self->pnr_2, self->pnr_3,
                                    self->pnr_4, self->pnr_5, self->pnr_6,
                                    self->pnr_7, self->pnr_8, self->pnr_9,
                                    pref_ver_12, pref_ver_13, pref_ver_14,
                                    pref_ver_15, pref_ver_16,
                                    self->pref_ver_task_assume_10_1,
                                    self->pref_ver_task_assume_12,
                                    self->pref_ver_task_assume_1_1,
                                    self->pref_ver_task_assume_2_1,
                                    self->pref_ver_task_assume_3_1,
                                    self->pref_ver_task_assume_4_1,
                                    self->pref_ver_task_assume_5_1,
                                    self->pref_ver_task_assume_6_1,
                                    self->pref_ver_task_assume_7_1,
                                    self->pref_ver_task_assume_8_1,
                                    self->pref_ver_task_assume_9_1,
                                    self->pref_ver_task_guarantee_10_1,
                                    self->pref_ver_task_guarantee_12,
                                    self->pref_ver_task_guarantee_1_1,
                                    self->pref_ver_task_guarantee_2_1,
                                    self->pref_ver_task_guarantee_3_1,
                                    self->pref_ver_task_guarantee_4_1,
                                    self->pref_ver_task_guarantee_5_1,
                                    self->pref_ver_task_guarantee_6_1,
                                    self->pref_ver_task_guarantee_7_1,
                                    self->pref_ver_task_guarantee_8_1,
                                    self->pref_ver_task_guarantee_9_1, rb_e,
                                    rb_r, rp_1, rp_2, rp_3, rpl_e, rpl_r,
                                    rs_e, rs_r, sl_e, sl_r, texe1, texe10,
                                    texe11, texe2, texe3, texe4, texe5,
                                    texe6, texe7, texe8, texe9, tl_e, tl_r,
                                    trk_e, trk_r,
                                    &Main_controller__main_ctrlr5_out_st);
  c_tl_ver = Main_controller__main_ctrlr5_out_st.c_tl_ver;
  _out->tl_ver = c_tl_ver;
  task_ver_5 = _out->tl_ver;
  task_ver_17 = task_ver_5;
  texe_5 = texe6;
  current_texec_5 = texe_5;
  tmin_5 = min6;
  tmin_17 = tmin_5;
  tmax_5 = max6;
  tmax_17 = tmax_5;
  Main__pref_ver_task_step(task_ver_17, current_texec_5, tmin_17, tmax_17,
                           &Main__pref_ver_task_out_st,
                           &self->pref_ver_task_5);
  pref_ver_17 = Main__pref_ver_task_out_st.pref_ver;
  obj_17 = Main__pref_ver_task_out_st.obj;
  Main_controller__main_ctrlr6_step(c_c_img_ver, c_img_e, c_img_r,
                                    c_me_img_ver, c_me_imu_ver, c_oa_ver,
                                    c_sl_ver, c_tl_ver, self->ck,
                                    self->ck_10, self->ck_13, self->ck_16,
                                    self->ck_19, self->ck_22, self->ck_25,
                                    self->ck_28, self->ck_31, self->ck_34,
                                    self->ck_37, self->ck_39, self->ck_41,
                                    self->ck_7, dt_e, dt_r, f_1, f_2, f_3,
                                    max1, max10, max11, max2, max3, max4,
                                    max5, max6, max7, max8, max9, me_img_e,
                                    me_img_r, me_imu_e, me_imu_r, min1,
                                    min10, min11, min2, min3, min4, min5,
                                    min6, min7, min8, min9, oa_e, oa_r,
                                    obj_12, obj_13, obj_14, obj_15, obj_16,
                                    obj_17, self->pnr, self->pnr_1,
                                    self->pnr_10, self->pnr_11, self->pnr_12,
                                    self->pnr_13, self->pnr_2, self->pnr_3,
                                    self->pnr_4, self->pnr_5, self->pnr_6,
                                    self->pnr_7, self->pnr_8, self->pnr_9,
                                    pref_ver_12, pref_ver_13, pref_ver_14,
                                    pref_ver_15, pref_ver_16, pref_ver_17,
                                    self->pref_ver_task_assume_10_1,
                                    self->pref_ver_task_assume_12,
                                    self->pref_ver_task_assume_1_1,
                                    self->pref_ver_task_assume_2_1,
                                    self->pref_ver_task_assume_3_1,
                                    self->pref_ver_task_assume_4_1,
                                    self->pref_ver_task_assume_5_1,
                                    self->pref_ver_task_assume_6_1,
                                    self->pref_ver_task_assume_7_1,
                                    self->pref_ver_task_assume_8_1,
                                    self->pref_ver_task_assume_9_1,
                                    self->pref_ver_task_guarantee_10_1,
                                    self->pref_ver_task_guarantee_12,
                                    self->pref_ver_task_guarantee_1_1,
                                    self->pref_ver_task_guarantee_2_1,
                                    self->pref_ver_task_guarantee_3_1,
                                    self->pref_ver_task_guarantee_4_1,
                                    self->pref_ver_task_guarantee_5_1,
                                    self->pref_ver_task_guarantee_6_1,
                                    self->pref_ver_task_guarantee_7_1,
                                    self->pref_ver_task_guarantee_8_1,
                                    self->pref_ver_task_guarantee_9_1, rb_e,
                                    rb_r, rp_1, rp_2, rp_3, rpl_e, rpl_r,
                                    rs_e, rs_r, sl_e, sl_r, texe1, texe10,
                                    texe11, texe2, texe3, texe4, texe5,
                                    texe6, texe7, texe8, texe9, tl_e, tl_r,
                                    trk_e, trk_r,
                                    &Main_controller__main_ctrlr6_out_st);
  c_rs_ver = Main_controller__main_ctrlr6_out_st.c_rs_ver;
  _out->rs_ver = c_rs_ver;
  task_ver_4 = _out->rs_ver;
  task_ver_18 = task_ver_4;
  texe_4 = texe7;
  current_texec_6 = texe_4;
  tmin_4 = min7;
  tmin_18 = tmin_4;
  tmax_4 = max7;
  tmax_18 = tmax_4;
  Main__pref_ver_task_step(task_ver_18, current_texec_6, tmin_18, tmax_18,
                           &Main__pref_ver_task_out_st,
                           &self->pref_ver_task_6);
  pref_ver_18 = Main__pref_ver_task_out_st.pref_ver;
  obj_18 = Main__pref_ver_task_out_st.obj;
  Main_controller__main_ctrlr7_step(c_c_img_ver, c_img_e, c_img_r,
                                    c_me_img_ver, c_me_imu_ver, c_oa_ver,
                                    c_rs_ver, c_sl_ver, c_tl_ver, self->ck,
                                    self->ck_10, self->ck_13, self->ck_16,
                                    self->ck_19, self->ck_22, self->ck_25,
                                    self->ck_28, self->ck_31, self->ck_34,
                                    self->ck_37, self->ck_39, self->ck_41,
                                    self->ck_7, dt_e, dt_r, f_1, f_2, f_3,
                                    max1, max10, max11, max2, max3, max4,
                                    max5, max6, max7, max8, max9, me_img_e,
                                    me_img_r, me_imu_e, me_imu_r, min1,
                                    min10, min11, min2, min3, min4, min5,
                                    min6, min7, min8, min9, oa_e, oa_r,
                                    obj_12, obj_13, obj_14, obj_15, obj_16,
                                    obj_17, obj_18, self->pnr, self->pnr_1,
                                    self->pnr_10, self->pnr_11, self->pnr_12,
                                    self->pnr_13, self->pnr_2, self->pnr_3,
                                    self->pnr_4, self->pnr_5, self->pnr_6,
                                    self->pnr_7, self->pnr_8, self->pnr_9,
                                    pref_ver_12, pref_ver_13, pref_ver_14,
                                    pref_ver_15, pref_ver_16, pref_ver_17,
                                    pref_ver_18,
                                    self->pref_ver_task_assume_10_1,
                                    self->pref_ver_task_assume_12,
                                    self->pref_ver_task_assume_1_1,
                                    self->pref_ver_task_assume_2_1,
                                    self->pref_ver_task_assume_3_1,
                                    self->pref_ver_task_assume_4_1,
                                    self->pref_ver_task_assume_5_1,
                                    self->pref_ver_task_assume_6_1,
                                    self->pref_ver_task_assume_7_1,
                                    self->pref_ver_task_assume_8_1,
                                    self->pref_ver_task_assume_9_1,
                                    self->pref_ver_task_guarantee_10_1,
                                    self->pref_ver_task_guarantee_12,
                                    self->pref_ver_task_guarantee_1_1,
                                    self->pref_ver_task_guarantee_2_1,
                                    self->pref_ver_task_guarantee_3_1,
                                    self->pref_ver_task_guarantee_4_1,
                                    self->pref_ver_task_guarantee_5_1,
                                    self->pref_ver_task_guarantee_6_1,
                                    self->pref_ver_task_guarantee_7_1,
                                    self->pref_ver_task_guarantee_8_1,
                                    self->pref_ver_task_guarantee_9_1, rb_e,
                                    rb_r, rp_1, rp_2, rp_3, rpl_e, rpl_r,
                                    rs_e, rs_r, sl_e, sl_r, texe1, texe10,
                                    texe11, texe2, texe3, texe4, texe5,
                                    texe6, texe7, texe8, texe9, tl_e, tl_r,
                                    trk_e, trk_r,
                                    &Main_controller__main_ctrlr7_out_st);
  c_rb_ver = Main_controller__main_ctrlr7_out_st.c_rb_ver;
  _out->rb_ver = c_rb_ver;
  task_ver_3 = _out->rb_ver;
  task_ver_19 = task_ver_3;
  texe_3 = texe8;
  current_texec_7 = texe_3;
  tmin_3 = min8;
  tmin_19 = tmin_3;
  tmax_3 = max8;
  tmax_19 = tmax_3;
  Main__pref_ver_task_step(task_ver_19, current_texec_7, tmin_19, tmax_19,
                           &Main__pref_ver_task_out_st,
                           &self->pref_ver_task_7);
  pref_ver_19 = Main__pref_ver_task_out_st.pref_ver;
  obj_19 = Main__pref_ver_task_out_st.obj;
  Main_controller__main_ctrlr8_step(c_c_img_ver, c_img_e, c_img_r,
                                    c_me_img_ver, c_me_imu_ver, c_oa_ver,
                                    c_rb_ver, c_rs_ver, c_sl_ver, c_tl_ver,
                                    self->ck, self->ck_10, self->ck_13,
                                    self->ck_16, self->ck_19, self->ck_22,
                                    self->ck_25, self->ck_28, self->ck_31,
                                    self->ck_34, self->ck_37, self->ck_39,
                                    self->ck_41, self->ck_7, dt_e, dt_r, f_1,
                                    f_2, f_3, max1, max10, max11, max2, max3,
                                    max4, max5, max6, max7, max8, max9,
                                    me_img_e, me_img_r, me_imu_e, me_imu_r,
                                    min1, min10, min11, min2, min3, min4,
                                    min5, min6, min7, min8, min9, oa_e, oa_r,
                                    obj_12, obj_13, obj_14, obj_15, obj_16,
                                    obj_17, obj_18, obj_19, self->pnr,
                                    self->pnr_1, self->pnr_10, self->pnr_11,
                                    self->pnr_12, self->pnr_13, self->pnr_2,
                                    self->pnr_3, self->pnr_4, self->pnr_5,
                                    self->pnr_6, self->pnr_7, self->pnr_8,
                                    self->pnr_9, pref_ver_12, pref_ver_13,
                                    pref_ver_14, pref_ver_15, pref_ver_16,
                                    pref_ver_17, pref_ver_18, pref_ver_19,
                                    self->pref_ver_task_assume_10_1,
                                    self->pref_ver_task_assume_12,
                                    self->pref_ver_task_assume_1_1,
                                    self->pref_ver_task_assume_2_1,
                                    self->pref_ver_task_assume_3_1,
                                    self->pref_ver_task_assume_4_1,
                                    self->pref_ver_task_assume_5_1,
                                    self->pref_ver_task_assume_6_1,
                                    self->pref_ver_task_assume_7_1,
                                    self->pref_ver_task_assume_8_1,
                                    self->pref_ver_task_assume_9_1,
                                    self->pref_ver_task_guarantee_10_1,
                                    self->pref_ver_task_guarantee_12,
                                    self->pref_ver_task_guarantee_1_1,
                                    self->pref_ver_task_guarantee_2_1,
                                    self->pref_ver_task_guarantee_3_1,
                                    self->pref_ver_task_guarantee_4_1,
                                    self->pref_ver_task_guarantee_5_1,
                                    self->pref_ver_task_guarantee_6_1,
                                    self->pref_ver_task_guarantee_7_1,
                                    self->pref_ver_task_guarantee_8_1,
                                    self->pref_ver_task_guarantee_9_1, rb_e,
                                    rb_r, rp_1, rp_2, rp_3, rpl_e, rpl_r,
                                    rs_e, rs_r, sl_e, sl_r, texe1, texe10,
                                    texe11, texe2, texe3, texe4, texe5,
                                    texe6, texe7, texe8, texe9, tl_e, tl_r,
                                    trk_e, trk_r,
                                    &Main_controller__main_ctrlr8_out_st);
  c_rpl_ver = Main_controller__main_ctrlr8_out_st.c_rpl_ver;
  _out->rpl_ver = c_rpl_ver;
  task_ver_2 = _out->rpl_ver;
  task_ver_20 = task_ver_2;
  texe_2 = texe9;
  current_texec_8 = texe_2;
  tmin_2 = min9;
  tmin_20 = tmin_2;
  tmax_2 = max9;
  tmax_20 = tmax_2;
  Main__pref_ver_task_step(task_ver_20, current_texec_8, tmin_20, tmax_20,
                           &Main__pref_ver_task_out_st,
                           &self->pref_ver_task_8);
  pref_ver_20 = Main__pref_ver_task_out_st.pref_ver;
  obj_20 = Main__pref_ver_task_out_st.obj;
  Main_controller__main_ctrlr9_step(c_c_img_ver, c_img_e, c_img_r,
                                    c_me_img_ver, c_me_imu_ver, c_oa_ver,
                                    c_rb_ver, c_rpl_ver, c_rs_ver, c_sl_ver,
                                    c_tl_ver, self->ck, self->ck_10,
                                    self->ck_13, self->ck_16, self->ck_19,
                                    self->ck_22, self->ck_25, self->ck_28,
                                    self->ck_31, self->ck_34, self->ck_37,
                                    self->ck_39, self->ck_41, self->ck_7,
                                    dt_e, dt_r, f_1, f_2, f_3, max1, max10,
                                    max11, max2, max3, max4, max5, max6,
                                    max7, max8, max9, me_img_e, me_img_r,
                                    me_imu_e, me_imu_r, min1, min10, min11,
                                    min2, min3, min4, min5, min6, min7, min8,
                                    min9, oa_e, oa_r, obj_12, obj_13, obj_14,
                                    obj_15, obj_16, obj_17, obj_18, obj_19,
                                    obj_20, self->pnr, self->pnr_1,
                                    self->pnr_10, self->pnr_11, self->pnr_12,
                                    self->pnr_13, self->pnr_2, self->pnr_3,
                                    self->pnr_4, self->pnr_5, self->pnr_6,
                                    self->pnr_7, self->pnr_8, self->pnr_9,
                                    pref_ver_12, pref_ver_13, pref_ver_14,
                                    pref_ver_15, pref_ver_16, pref_ver_17,
                                    pref_ver_18, pref_ver_19, pref_ver_20,
                                    self->pref_ver_task_assume_10_1,
                                    self->pref_ver_task_assume_12,
                                    self->pref_ver_task_assume_1_1,
                                    self->pref_ver_task_assume_2_1,
                                    self->pref_ver_task_assume_3_1,
                                    self->pref_ver_task_assume_4_1,
                                    self->pref_ver_task_assume_5_1,
                                    self->pref_ver_task_assume_6_1,
                                    self->pref_ver_task_assume_7_1,
                                    self->pref_ver_task_assume_8_1,
                                    self->pref_ver_task_assume_9_1,
                                    self->pref_ver_task_guarantee_10_1,
                                    self->pref_ver_task_guarantee_12,
                                    self->pref_ver_task_guarantee_1_1,
                                    self->pref_ver_task_guarantee_2_1,
                                    self->pref_ver_task_guarantee_3_1,
                                    self->pref_ver_task_guarantee_4_1,
                                    self->pref_ver_task_guarantee_5_1,
                                    self->pref_ver_task_guarantee_6_1,
                                    self->pref_ver_task_guarantee_7_1,
                                    self->pref_ver_task_guarantee_8_1,
                                    self->pref_ver_task_guarantee_9_1, rb_e,
                                    rb_r, rp_1, rp_2, rp_3, rpl_e, rpl_r,
                                    rs_e, rs_r, sl_e, sl_r, texe1, texe10,
                                    texe11, texe2, texe3, texe4, texe5,
                                    texe6, texe7, texe8, texe9, tl_e, tl_r,
                                    trk_e, trk_r,
                                    &Main_controller__main_ctrlr9_out_st);
  c_dt_ver = Main_controller__main_ctrlr9_out_st.c_dt_ver;
  _out->dt_ver = c_dt_ver;
  task_ver_1 = _out->dt_ver;
  task_ver_21 = task_ver_1;
  texe_1 = texe10;
  current_texec_9 = texe_1;
  tmin_1 = min10;
  tmin_21 = tmin_1;
  tmax_1 = max10;
  tmax_21 = tmax_1;
  Main__pref_ver_task_step(task_ver_21, current_texec_9, tmin_21, tmax_21,
                           &Main__pref_ver_task_out_st,
                           &self->pref_ver_task_9);
  pref_ver_21 = Main__pref_ver_task_out_st.pref_ver;
  obj_21 = Main__pref_ver_task_out_st.obj;
  Main_controller__main_ctrlr10_step(c_c_img_ver, c_dt_ver, c_img_e, c_img_r,
                                     c_me_img_ver, c_me_imu_ver, c_oa_ver,
                                     c_rb_ver, c_rpl_ver, c_rs_ver, c_sl_ver,
                                     c_tl_ver, self->ck, self->ck_10,
                                     self->ck_13, self->ck_16, self->ck_19,
                                     self->ck_22, self->ck_25, self->ck_28,
                                     self->ck_31, self->ck_34, self->ck_37,
                                     self->ck_39, self->ck_41, self->ck_7,
                                     dt_e, dt_r, f_1, f_2, f_3, max1, max10,
                                     max11, max2, max3, max4, max5, max6,
                                     max7, max8, max9, me_img_e, me_img_r,
                                     me_imu_e, me_imu_r, min1, min10, min11,
                                     min2, min3, min4, min5, min6, min7,
                                     min8, min9, oa_e, oa_r, obj_12, obj_13,
                                     obj_14, obj_15, obj_16, obj_17, obj_18,
                                     obj_19, obj_20, obj_21, self->pnr,
                                     self->pnr_1, self->pnr_10, self->pnr_11,
                                     self->pnr_12, self->pnr_13, self->pnr_2,
                                     self->pnr_3, self->pnr_4, self->pnr_5,
                                     self->pnr_6, self->pnr_7, self->pnr_8,
                                     self->pnr_9, pref_ver_12, pref_ver_13,
                                     pref_ver_14, pref_ver_15, pref_ver_16,
                                     pref_ver_17, pref_ver_18, pref_ver_19,
                                     pref_ver_20, pref_ver_21,
                                     self->pref_ver_task_assume_10_1,
                                     self->pref_ver_task_assume_12,
                                     self->pref_ver_task_assume_1_1,
                                     self->pref_ver_task_assume_2_1,
                                     self->pref_ver_task_assume_3_1,
                                     self->pref_ver_task_assume_4_1,
                                     self->pref_ver_task_assume_5_1,
                                     self->pref_ver_task_assume_6_1,
                                     self->pref_ver_task_assume_7_1,
                                     self->pref_ver_task_assume_8_1,
                                     self->pref_ver_task_assume_9_1,
                                     self->pref_ver_task_guarantee_10_1,
                                     self->pref_ver_task_guarantee_12,
                                     self->pref_ver_task_guarantee_1_1,
                                     self->pref_ver_task_guarantee_2_1,
                                     self->pref_ver_task_guarantee_3_1,
                                     self->pref_ver_task_guarantee_4_1,
                                     self->pref_ver_task_guarantee_5_1,
                                     self->pref_ver_task_guarantee_6_1,
                                     self->pref_ver_task_guarantee_7_1,
                                     self->pref_ver_task_guarantee_8_1,
                                     self->pref_ver_task_guarantee_9_1, rb_e,
                                     rb_r, rp_1, rp_2, rp_3, rpl_e, rpl_r,
                                     rs_e, rs_r, sl_e, sl_r, texe1, texe10,
                                     texe11, texe2, texe3, texe4, texe5,
                                     texe6, texe7, texe8, texe9, tl_e, tl_r,
                                     trk_e, trk_r,
                                     &Main_controller__main_ctrlr10_out_st);
  c_trk_ver = Main_controller__main_ctrlr10_out_st.c_trk_ver;
  _out->trk_ver = c_trk_ver;
  task_ver = _out->trk_ver;
  task_ver_22 = task_ver;
  texe = texe11;
  current_texec_10 = texe;
  tmin = min11;
  tmin_22 = tmin;
  tmax = max11;
  tmax_22 = tmax;
  Main__pref_ver_task_step(task_ver_22, current_texec_10, tmin_22, tmax_22,
                           &Main__pref_ver_task_out_st,
                           &self->pref_ver_task_10);
  pref_ver_22 = Main__pref_ver_task_out_st.pref_ver;
  obj_22 = Main__pref_ver_task_out_st.obj;
  pref_ver_task_guarantee_10 = obj_22;
  v_5 = (v_4||pref_ver_task_guarantee_10);
  pref_ver_task_assume_9 = true;
  v_6 = !(pref_ver_task_assume_9);
  pref_ver_task_guarantee_9 = obj_21;
  v_7 = (v_6||pref_ver_task_guarantee_9);
  pref_ver_task_assume_8 = true;
  v_8 = !(pref_ver_task_assume_8);
  pref_ver_task_guarantee_8 = obj_20;
  v_9 = (v_8||pref_ver_task_guarantee_8);
  pref_ver_task_assume_7 = true;
  v_10 = !(pref_ver_task_assume_7);
  pref_ver_task_guarantee_7 = obj_19;
  v_11 = (v_10||pref_ver_task_guarantee_7);
  pref_ver_task_assume_6 = true;
  v_12 = !(pref_ver_task_assume_6);
  pref_ver_task_guarantee_6 = obj_18;
  v_13 = (v_12||pref_ver_task_guarantee_6);
  pref_ver_task_assume_5 = true;
  v_14 = !(pref_ver_task_assume_5);
  pref_ver_task_guarantee_5 = obj_17;
  v_15 = (v_14||pref_ver_task_guarantee_5);
  pref_ver_task_assume_4 = true;
  v_16 = !(pref_ver_task_assume_4);
  pref_ver_task_guarantee_4 = obj_16;
  v_17 = (v_16||pref_ver_task_guarantee_4);
  pref_ver_task_assume_3 = true;
  v_18 = !(pref_ver_task_assume_3);
  pref_ver_task_guarantee_3 = obj_15;
  v_19 = (v_18||pref_ver_task_guarantee_3);
  pref_ver_task_assume_2 = true;
  v_20 = !(pref_ver_task_assume_2);
  pref_ver_task_guarantee_2 = obj_14;
  v_21 = (v_20||pref_ver_task_guarantee_2);
  pref_ver_task_assume_1 = true;
  v_22 = !(pref_ver_task_assume_1);
  pref_ver_task_guarantee_1 = obj_13;
  v_23 = (v_22||pref_ver_task_guarantee_1);
  pref_ver_task_assume = true;
  v_24 = !(pref_ver_task_assume);
  pref_ver_task_guarantee = obj_12;
  v_25 = (v_24||pref_ver_task_guarantee);
  v_26 = (v_23&&v_25);
  v_27 = (v_21&&v_26);
  v_28 = (v_19&&v_27);
  v_29 = (v_17&&v_28);
  v_30 = (v_15&&v_29);
  v_31 = (v_13&&v_30);
  v_32 = (v_11&&v_31);
  v_33 = (v_9&&v_32);
  v_34 = (v_7&&v_33);
  v_35 = (v_5&&v_34);
  v_36 = (pref_ver_task_assume_1&&pref_ver_task_assume);
  v_37 = (pref_ver_task_assume_2&&v_36);
  v_38 = (pref_ver_task_assume_3&&v_37);
  v_39 = (pref_ver_task_assume_4&&v_38);
  v_40 = (pref_ver_task_assume_5&&v_39);
  v_41 = (pref_ver_task_assume_6&&v_40);
  v_42 = (pref_ver_task_assume_7&&v_41);
  v_43 = (pref_ver_task_assume_8&&v_42);
  v_44 = (pref_ver_task_assume_9&&v_43);
  v_45 = (pref_ver_task_assume_10&&v_44);
  rp_5 = rp_1;
  switch (self->ck_41) {
    case Main__St_2_Fail:
      if (rp_5) {
        s_St_2_Fail = Main__St_2_Free;
      } else {
        s_St_2_Fail = Main__St_2_Fail;
      };
      break;
    default:
      break;
  };
  f_5 = f_1;
  switch (self->ck_41) {
    case Main__St_2_Free:
      if (f_5) {
        s_St_2_Free = Main__St_2_Fail;
      } else {
        s_St_2_Free = Main__St_2_Free;
      };
      s = s_St_2_Free;
      break;
    case Main__St_2_Fail:
      s = s_St_2_Fail;
      break;
    default:
      break;
  };
  ck_42 = s;
  switch (ck_42) {
    case Main__St_2_Fail:
      err_5_St_2_Fail = true;
      err_5 = err_5_St_2_Fail;
      break;
    case Main__St_2_Free:
      err_5_St_2_Free = false;
      err_5 = err_5_St_2_Free;
      break;
    default:
      break;
  };
  err_1 = err_5;
  rp_4 = rp_2;
  switch (self->ck_39) {
    case Main__St_3_Fail:
      if (rp_4) {
        s_1_St_3_Fail = Main__St_3_Free;
      } else {
        s_1_St_3_Fail = Main__St_3_Fail;
      };
      break;
    default:
      break;
  };
  f_4 = f_2;
  switch (self->ck_39) {
    case Main__St_3_Free:
      if (f_4) {
        s_1_St_3_Free = Main__St_3_Fail;
      } else {
        s_1_St_3_Free = Main__St_3_Free;
      };
      s_1 = s_1_St_3_Free;
      break;
    case Main__St_3_Fail:
      s_1 = s_1_St_3_Fail;
      break;
    default:
      break;
  };
  ck_40 = s_1;
  switch (ck_40) {
    case Main__St_3_Fail:
      err_4_St_3_Fail = true;
      err_4 = err_4_St_3_Fail;
      break;
    case Main__St_3_Free:
      err_4_St_3_Free = false;
      err_4 = err_4_St_3_Free;
      break;
    default:
      break;
  };
  err_2 = err_4;
  rp = rp_3;
  switch (self->ck_37) {
    case Main__St_4_Fail:
      if (rp) {
        s_2_St_4_Fail = Main__St_4_Free;
      } else {
        s_2_St_4_Fail = Main__St_4_Fail;
      };
      break;
    default:
      break;
  };
  f = f_3;
  switch (self->ck_37) {
    case Main__St_4_Free:
      if (f) {
        s_2_St_4_Free = Main__St_4_Fail;
      } else {
        s_2_St_4_Free = Main__St_4_Free;
      };
      s_2 = s_2_St_4_Free;
      break;
    case Main__St_4_Fail:
      s_2 = s_2_St_4_Fail;
      break;
    default:
      break;
  };
  ck_38 = s_2;
  switch (ck_38) {
    case Main__St_4_Fail:
      err_St_4_Fail = true;
      err = err_St_4_Fail;
      break;
    case Main__St_4_Free:
      err_St_4_Free = false;
      err = err_St_4_Free;
      break;
    default:
      break;
  };
  err_3 = err;
  end_task_10 = c_img_e;
  switch (self->ck_34) {
    case Main__St_5_Active:
      if (end_task_10) {
        s_3_St_5_Active = Main__St_5_Idle;
      } else {
        s_3_St_5_Active = Main__St_5_Active;
      };
      break;
    default:
      break;
  };
  req_task_10 = c_img_r;
  switch (self->ck_34) {
    case Main__St_5_Idle:
      if (req_task_10) {
        s_3_St_5_Idle = Main__St_5_Active;
      } else {
        s_3_St_5_Idle = Main__St_5_Idle;
      };
      s_3 = s_3_St_5_Idle;
      break;
    case Main__St_5_Active:
      s_3 = s_3_St_5_Active;
      break;
    default:
      break;
  };
  ck_35 = s_3;
  switch (ck_35) {
    case Main__St_5_Active:
      act_10_St_5_Active = true;
      act_10 = act_10_St_5_Active;
      break;
    case Main__St_5_Idle:
      act_10_St_5_Idle = false;
      act_10 = act_10_St_5_Idle;
      break;
    default:
      break;
  };
  _out->c_img_act = act_10;
  wt_10 = false;
  _out->c_img_wt = wt_10;
  end_task_9 = me_imu_e;
  switch (self->ck_31) {
    case Main__St_6_Active:
      if (end_task_9) {
        s_4_St_6_Active = Main__St_6_Idle;
      } else {
        s_4_St_6_Active = Main__St_6_Active;
      };
      break;
    default:
      break;
  };
  req_task_9 = me_imu_r;
  switch (self->ck_31) {
    case Main__St_6_Idle:
      if (req_task_9) {
        s_4_St_6_Idle = Main__St_6_Active;
      } else {
        s_4_St_6_Idle = Main__St_6_Idle;
      };
      s_4 = s_4_St_6_Idle;
      break;
    case Main__St_6_Active:
      s_4 = s_4_St_6_Active;
      break;
    default:
      break;
  };
  ck_32 = s_4;
  switch (ck_32) {
    case Main__St_6_Active:
      act_9_St_6_Active = true;
      act_9 = act_9_St_6_Active;
      break;
    case Main__St_6_Idle:
      act_9_St_6_Idle = false;
      act_9 = act_9_St_6_Idle;
      break;
    default:
      break;
  };
  _out->me_imu_act = act_9;
  wt_9 = false;
  _out->me_imu_wt = wt_9;
  end_task_8 = me_img_e;
  switch (self->ck_28) {
    case Main__St_7_Active:
      if (end_task_8) {
        s_5_St_7_Active = Main__St_7_Idle;
      } else {
        s_5_St_7_Active = Main__St_7_Active;
      };
      break;
    default:
      break;
  };
  req_task_8 = me_img_r;
  switch (self->ck_28) {
    case Main__St_7_Idle:
      if (req_task_8) {
        s_5_St_7_Idle = Main__St_7_Active;
      } else {
        s_5_St_7_Idle = Main__St_7_Idle;
      };
      s_5 = s_5_St_7_Idle;
      break;
    case Main__St_7_Active:
      s_5 = s_5_St_7_Active;
      break;
    default:
      break;
  };
  ck_29 = s_5;
  switch (ck_29) {
    case Main__St_7_Active:
      act_8_St_7_Active = true;
      act_8 = act_8_St_7_Active;
      break;
    case Main__St_7_Idle:
      act_8_St_7_Idle = false;
      act_8 = act_8_St_7_Idle;
      break;
    default:
      break;
  };
  _out->me_img_act = act_8;
  wt_8 = false;
  _out->me_img_wt = wt_8;
  end_task_7 = sl_e;
  switch (self->ck_25) {
    case Main__St_8_Active:
      if (end_task_7) {
        s_6_St_8_Active = Main__St_8_Idle;
      } else {
        s_6_St_8_Active = Main__St_8_Active;
      };
      break;
    default:
      break;
  };
  req_task_7 = sl_r;
  switch (self->ck_25) {
    case Main__St_8_Idle:
      if (req_task_7) {
        s_6_St_8_Idle = Main__St_8_Active;
      } else {
        s_6_St_8_Idle = Main__St_8_Idle;
      };
      s_6 = s_6_St_8_Idle;
      break;
    case Main__St_8_Active:
      s_6 = s_6_St_8_Active;
      break;
    default:
      break;
  };
  ck_26 = s_6;
  switch (ck_26) {
    case Main__St_8_Active:
      act_7_St_8_Active = true;
      act_7 = act_7_St_8_Active;
      break;
    case Main__St_8_Idle:
      act_7_St_8_Idle = false;
      act_7 = act_7_St_8_Idle;
      break;
    default:
      break;
  };
  _out->sl_act = act_7;
  wt_7 = false;
  _out->sl_wt = wt_7;
  end_task_6 = oa_e;
  switch (self->ck_22) {
    case Main__St_9_Active:
      if (end_task_6) {
        s_7_St_9_Active = Main__St_9_Idle;
      } else {
        s_7_St_9_Active = Main__St_9_Active;
      };
      break;
    default:
      break;
  };
  req_task_6 = oa_r;
  switch (self->ck_22) {
    case Main__St_9_Idle:
      if (req_task_6) {
        s_7_St_9_Idle = Main__St_9_Active;
      } else {
        s_7_St_9_Idle = Main__St_9_Idle;
      };
      s_7 = s_7_St_9_Idle;
      break;
    case Main__St_9_Active:
      s_7 = s_7_St_9_Active;
      break;
    default:
      break;
  };
  ck_23 = s_7;
  switch (ck_23) {
    case Main__St_9_Active:
      act_6_St_9_Active = true;
      act_6 = act_6_St_9_Active;
      break;
    case Main__St_9_Idle:
      act_6_St_9_Idle = false;
      act_6 = act_6_St_9_Idle;
      break;
    default:
      break;
  };
  _out->oa_act = act_6;
  wt_6 = false;
  _out->oa_wt = wt_6;
  end_task_5 = tl_e;
  switch (self->ck_19) {
    case Main__St_10_Active:
      if (end_task_5) {
        s_8_St_10_Active = Main__St_10_Idle;
      } else {
        s_8_St_10_Active = Main__St_10_Active;
      };
      break;
    default:
      break;
  };
  req_task_5 = tl_r;
  switch (self->ck_19) {
    case Main__St_10_Idle:
      if (req_task_5) {
        s_8_St_10_Idle = Main__St_10_Active;
      } else {
        s_8_St_10_Idle = Main__St_10_Idle;
      };
      s_8 = s_8_St_10_Idle;
      break;
    case Main__St_10_Active:
      s_8 = s_8_St_10_Active;
      break;
    default:
      break;
  };
  ck_20 = s_8;
  switch (ck_20) {
    case Main__St_10_Active:
      act_5_St_10_Active = true;
      act_5 = act_5_St_10_Active;
      break;
    case Main__St_10_Idle:
      act_5_St_10_Idle = false;
      act_5 = act_5_St_10_Idle;
      break;
    default:
      break;
  };
  _out->tl_act = act_5;
  wt_5 = false;
  _out->tl_wt = wt_5;
  end_task_4 = rs_e;
  switch (self->ck_16) {
    case Main__St_11_Active:
      if (end_task_4) {
        s_9_St_11_Active = Main__St_11_Idle;
      } else {
        s_9_St_11_Active = Main__St_11_Active;
      };
      break;
    default:
      break;
  };
  req_task_4 = rs_r;
  switch (self->ck_16) {
    case Main__St_11_Idle:
      if (req_task_4) {
        s_9_St_11_Idle = Main__St_11_Active;
      } else {
        s_9_St_11_Idle = Main__St_11_Idle;
      };
      s_9 = s_9_St_11_Idle;
      break;
    case Main__St_11_Active:
      s_9 = s_9_St_11_Active;
      break;
    default:
      break;
  };
  ck_17 = s_9;
  switch (ck_17) {
    case Main__St_11_Active:
      act_4_St_11_Active = true;
      act_4 = act_4_St_11_Active;
      break;
    case Main__St_11_Idle:
      act_4_St_11_Idle = false;
      act_4 = act_4_St_11_Idle;
      break;
    default:
      break;
  };
  _out->rs_act = act_4;
  wt_4 = false;
  _out->rs_wt = wt_4;
  end_task_3 = rb_e;
  switch (self->ck_13) {
    case Main__St_12_Active:
      if (end_task_3) {
        s_10_St_12_Active = Main__St_12_Idle;
      } else {
        s_10_St_12_Active = Main__St_12_Active;
      };
      break;
    default:
      break;
  };
  req_task_3 = rb_r;
  switch (self->ck_13) {
    case Main__St_12_Idle:
      if (req_task_3) {
        s_10_St_12_Idle = Main__St_12_Active;
      } else {
        s_10_St_12_Idle = Main__St_12_Idle;
      };
      s_10 = s_10_St_12_Idle;
      break;
    case Main__St_12_Active:
      s_10 = s_10_St_12_Active;
      break;
    default:
      break;
  };
  ck_14 = s_10;
  switch (ck_14) {
    case Main__St_12_Active:
      act_3_St_12_Active = true;
      act_3 = act_3_St_12_Active;
      break;
    case Main__St_12_Idle:
      act_3_St_12_Idle = false;
      act_3 = act_3_St_12_Idle;
      break;
    default:
      break;
  };
  _out->rb_act = act_3;
  wt_3 = false;
  _out->rb_wt = wt_3;
  end_task_2 = rpl_e;
  switch (self->ck_10) {
    case Main__St_13_Active:
      if (end_task_2) {
        s_11_St_13_Active = Main__St_13_Idle;
      } else {
        s_11_St_13_Active = Main__St_13_Active;
      };
      break;
    default:
      break;
  };
  req_task_2 = rpl_r;
  switch (self->ck_10) {
    case Main__St_13_Idle:
      if (req_task_2) {
        s_11_St_13_Idle = Main__St_13_Active;
      } else {
        s_11_St_13_Idle = Main__St_13_Idle;
      };
      s_11 = s_11_St_13_Idle;
      break;
    case Main__St_13_Active:
      s_11 = s_11_St_13_Active;
      break;
    default:
      break;
  };
  ck_11 = s_11;
  switch (ck_11) {
    case Main__St_13_Active:
      act_2_St_13_Active = true;
      act_2 = act_2_St_13_Active;
      break;
    case Main__St_13_Idle:
      act_2_St_13_Idle = false;
      act_2 = act_2_St_13_Idle;
      break;
    default:
      break;
  };
  _out->rpl_act = act_2;
  wt_2 = false;
  _out->rpl_wt = wt_2;
  end_task_1 = dt_e;
  switch (self->ck_7) {
    case Main__St_14_Active:
      if (end_task_1) {
        s_12_St_14_Active = Main__St_14_Idle;
      } else {
        s_12_St_14_Active = Main__St_14_Active;
      };
      break;
    default:
      break;
  };
  req_task_1 = dt_r;
  switch (self->ck_7) {
    case Main__St_14_Idle:
      if (req_task_1) {
        s_12_St_14_Idle = Main__St_14_Active;
      } else {
        s_12_St_14_Idle = Main__St_14_Idle;
      };
      s_12 = s_12_St_14_Idle;
      break;
    case Main__St_14_Active:
      s_12 = s_12_St_14_Active;
      break;
    default:
      break;
  };
  ck_8 = s_12;
  switch (ck_8) {
    case Main__St_14_Active:
      act_1_St_14_Active = true;
      act_1 = act_1_St_14_Active;
      break;
    case Main__St_14_Idle:
      act_1_St_14_Idle = false;
      act_1 = act_1_St_14_Idle;
      break;
    default:
      break;
  };
  _out->dt_act = act_1;
  wt_1 = false;
  _out->dt_wt = wt_1;
  end_task = trk_e;
  switch (self->ck) {
    case Main__St_15_Active:
      if (end_task) {
        s_13_St_15_Active = Main__St_15_Idle;
      } else {
        s_13_St_15_Active = Main__St_15_Active;
      };
      break;
    default:
      break;
  };
  req_task = trk_r;
  switch (self->ck) {
    case Main__St_15_Idle:
      if (req_task) {
        s_13_St_15_Idle = Main__St_15_Active;
      } else {
        s_13_St_15_Idle = Main__St_15_Idle;
      };
      s_13 = s_13_St_15_Idle;
      break;
    case Main__St_15_Active:
      s_13 = s_13_St_15_Active;
      break;
    default:
      break;
  };
  ck_5 = s_13;
  switch (ck_5) {
    case Main__St_15_Active:
      act_St_15_Active = true;
      act = act_St_15_Active;
      break;
    case Main__St_15_Idle:
      act_St_15_Idle = false;
      act = act_St_15_Idle;
      break;
    default:
      break;
  };
  _out->trk_act = act;
  wt = false;
  _out->trk_wt = wt;
  switch (ck_35) {
    case Main__St_5_Active:
      ck_36 = task_ver_10;
      switch (ck_36) {
        case Main__H3:
          t1_10_St_5_Active_H3 = 0;
          t1_10_St_5_Active = t1_10_St_5_Active_H3;
          break;
        case Main__H2:
          t1_10_St_5_Active_H2 = 0;
          t1_10_St_5_Active = t1_10_St_5_Active_H2;
          break;
        case Main__H1:
          t1_10_St_5_Active_H1 = 1;
          t1_10_St_5_Active = t1_10_St_5_Active_H1;
          break;
        case Main__S:
          t1_10_St_5_Active_S = 0;
          t1_10_St_5_Active = t1_10_St_5_Active_S;
          break;
        default:
          break;
      };
      t1_10 = t1_10_St_5_Active;
      break;
    case Main__St_5_Idle:
      t1_10_St_5_Idle = 0;
      t1_10 = t1_10_St_5_Idle;
      break;
    default:
      break;
  };
  c_img_t1 = t1_10;
  switch (ck_32) {
    case Main__St_6_Active:
      ck_33 = task_ver_9;
      switch (ck_33) {
        case Main__H3:
          t1_9_St_6_Active_H3 = 0;
          t1_9_St_6_Active = t1_9_St_6_Active_H3;
          break;
        case Main__H2:
          t1_9_St_6_Active_H2 = 0;
          t1_9_St_6_Active = t1_9_St_6_Active_H2;
          break;
        case Main__H1:
          t1_9_St_6_Active_H1 = 0;
          t1_9_St_6_Active = t1_9_St_6_Active_H1;
          break;
        case Main__S:
          t1_9_St_6_Active_S = 0;
          t1_9_St_6_Active = t1_9_St_6_Active_S;
          break;
        default:
          break;
      };
      t1_9 = t1_9_St_6_Active;
      break;
    case Main__St_6_Idle:
      t1_9_St_6_Idle = 0;
      t1_9 = t1_9_St_6_Idle;
      break;
    default:
      break;
  };
  me_imu_t1 = t1_9;
  v_46 = (c_img_t1+me_imu_t1);
  switch (ck_29) {
    case Main__St_7_Active:
      ck_30 = task_ver_8;
      switch (ck_30) {
        case Main__H3:
          t1_8_St_7_Active_H3 = 0;
          t1_8_St_7_Active = t1_8_St_7_Active_H3;
          break;
        case Main__H2:
          t1_8_St_7_Active_H2 = 0;
          t1_8_St_7_Active = t1_8_St_7_Active_H2;
          break;
        case Main__H1:
          t1_8_St_7_Active_H1 = 1;
          t1_8_St_7_Active = t1_8_St_7_Active_H1;
          break;
        case Main__S:
          t1_8_St_7_Active_S = 0;
          t1_8_St_7_Active = t1_8_St_7_Active_S;
          break;
        default:
          break;
      };
      t1_8 = t1_8_St_7_Active;
      break;
    case Main__St_7_Idle:
      t1_8_St_7_Idle = 0;
      t1_8 = t1_8_St_7_Idle;
      break;
    default:
      break;
  };
  me_img_t1 = t1_8;
  v_47 = (v_46+me_img_t1);
  switch (ck_26) {
    case Main__St_8_Active:
      ck_27 = task_ver_7;
      switch (ck_27) {
        case Main__H3:
          t1_7_St_8_Active_H3 = 0;
          t1_7_St_8_Active = t1_7_St_8_Active_H3;
          break;
        case Main__H2:
          t1_7_St_8_Active_H2 = 0;
          t1_7_St_8_Active = t1_7_St_8_Active_H2;
          break;
        case Main__H1:
          t1_7_St_8_Active_H1 = 1;
          t1_7_St_8_Active = t1_7_St_8_Active_H1;
          break;
        case Main__S:
          t1_7_St_8_Active_S = 0;
          t1_7_St_8_Active = t1_7_St_8_Active_S;
          break;
        default:
          break;
      };
      t1_7 = t1_7_St_8_Active;
      break;
    case Main__St_8_Idle:
      t1_7_St_8_Idle = 0;
      t1_7 = t1_7_St_8_Idle;
      break;
    default:
      break;
  };
  sl_t1 = t1_7;
  v_48 = (v_47+sl_t1);
  switch (ck_23) {
    case Main__St_9_Active:
      ck_24 = task_ver_6;
      switch (ck_24) {
        case Main__H3:
          t1_6_St_9_Active_H3 = 0;
          t1_6_St_9_Active = t1_6_St_9_Active_H3;
          break;
        case Main__H2:
          t1_6_St_9_Active_H2 = 0;
          t1_6_St_9_Active = t1_6_St_9_Active_H2;
          break;
        case Main__H1:
          t1_6_St_9_Active_H1 = 1;
          t1_6_St_9_Active = t1_6_St_9_Active_H1;
          break;
        case Main__S:
          t1_6_St_9_Active_S = 0;
          t1_6_St_9_Active = t1_6_St_9_Active_S;
          break;
        default:
          break;
      };
      t1_6 = t1_6_St_9_Active;
      break;
    case Main__St_9_Idle:
      t1_6_St_9_Idle = 0;
      t1_6 = t1_6_St_9_Idle;
      break;
    default:
      break;
  };
  oa_t1 = t1_6;
  v_49 = (v_48+oa_t1);
  switch (ck_20) {
    case Main__St_10_Active:
      ck_21 = task_ver_5;
      switch (ck_21) {
        case Main__H3:
          t1_5_St_10_Active_H3 = 0;
          t1_5_St_10_Active = t1_5_St_10_Active_H3;
          break;
        case Main__H2:
          t1_5_St_10_Active_H2 = 0;
          t1_5_St_10_Active = t1_5_St_10_Active_H2;
          break;
        case Main__H1:
          t1_5_St_10_Active_H1 = 1;
          t1_5_St_10_Active = t1_5_St_10_Active_H1;
          break;
        case Main__S:
          t1_5_St_10_Active_S = 0;
          t1_5_St_10_Active = t1_5_St_10_Active_S;
          break;
        default:
          break;
      };
      t1_5 = t1_5_St_10_Active;
      break;
    case Main__St_10_Idle:
      t1_5_St_10_Idle = 0;
      t1_5 = t1_5_St_10_Idle;
      break;
    default:
      break;
  };
  tl_t1 = t1_5;
  v_50 = (v_49+tl_t1);
  switch (ck_17) {
    case Main__St_11_Active:
      ck_18 = task_ver_4;
      switch (ck_18) {
        case Main__H3:
          t1_4_St_11_Active_H3 = 0;
          t1_4_St_11_Active = t1_4_St_11_Active_H3;
          break;
        case Main__H2:
          t1_4_St_11_Active_H2 = 0;
          t1_4_St_11_Active = t1_4_St_11_Active_H2;
          break;
        case Main__H1:
          t1_4_St_11_Active_H1 = 1;
          t1_4_St_11_Active = t1_4_St_11_Active_H1;
          break;
        case Main__S:
          t1_4_St_11_Active_S = 0;
          t1_4_St_11_Active = t1_4_St_11_Active_S;
          break;
        default:
          break;
      };
      t1_4 = t1_4_St_11_Active;
      break;
    case Main__St_11_Idle:
      t1_4_St_11_Idle = 0;
      t1_4 = t1_4_St_11_Idle;
      break;
    default:
      break;
  };
  rs_t1 = t1_4;
  v_51 = (v_50+rs_t1);
  switch (ck_14) {
    case Main__St_12_Active:
      ck_15 = task_ver_3;
      switch (ck_15) {
        case Main__H3:
          t1_3_St_12_Active_H3 = 0;
          t1_3_St_12_Active = t1_3_St_12_Active_H3;
          break;
        case Main__H2:
          t1_3_St_12_Active_H2 = 0;
          t1_3_St_12_Active = t1_3_St_12_Active_H2;
          break;
        case Main__H1:
          t1_3_St_12_Active_H1 = 2;
          t1_3_St_12_Active = t1_3_St_12_Active_H1;
          break;
        case Main__S:
          t1_3_St_12_Active_S = 0;
          t1_3_St_12_Active = t1_3_St_12_Active_S;
          break;
        default:
          break;
      };
      t1_3 = t1_3_St_12_Active;
      break;
    case Main__St_12_Idle:
      t1_3_St_12_Idle = 0;
      t1_3 = t1_3_St_12_Idle;
      break;
    default:
      break;
  };
  rb_t1 = t1_3;
  v_52 = (v_51+rb_t1);
  switch (ck_8) {
    case Main__St_14_Active:
      ck_9 = task_ver_1;
      switch (ck_9) {
        case Main__H3:
          t1_1_St_14_Active_H3 = 0;
          t1_1_St_14_Active = t1_1_St_14_Active_H3;
          break;
        case Main__H2:
          t1_1_St_14_Active_H2 = 0;
          t1_1_St_14_Active = t1_1_St_14_Active_H2;
          break;
        case Main__H1:
          t1_1_St_14_Active_H1 = 2;
          t1_1_St_14_Active = t1_1_St_14_Active_H1;
          break;
        case Main__S:
          t1_1_St_14_Active_S = 0;
          t1_1_St_14_Active = t1_1_St_14_Active_S;
          break;
        default:
          break;
      };
      t1_1 = t1_1_St_14_Active;
      break;
    case Main__St_14_Idle:
      t1_1_St_14_Idle = 0;
      t1_1 = t1_1_St_14_Idle;
      break;
    default:
      break;
  };
  dt_t1 = t1_1;
  v_53 = (v_52+dt_t1);
  switch (ck_11) {
    case Main__St_13_Active:
      ck_12 = task_ver_2;
      switch (ck_12) {
        case Main__H3:
          t1_2_St_13_Active_H3 = 0;
          t1_2_St_13_Active = t1_2_St_13_Active_H3;
          break;
        case Main__H2:
          t1_2_St_13_Active_H2 = 0;
          t1_2_St_13_Active = t1_2_St_13_Active_H2;
          break;
        case Main__H1:
          t1_2_St_13_Active_H1 = 2;
          t1_2_St_13_Active = t1_2_St_13_Active_H1;
          break;
        case Main__S:
          t1_2_St_13_Active_S = 0;
          t1_2_St_13_Active = t1_2_St_13_Active_S;
          break;
        default:
          break;
      };
      t1_2 = t1_2_St_13_Active;
      break;
    case Main__St_13_Idle:
      t1_2_St_13_Idle = 0;
      t1_2 = t1_2_St_13_Idle;
      break;
    default:
      break;
  };
  rpl_t1 = t1_2;
  v_54 = (v_53+rpl_t1);
  switch (ck_5) {
    case Main__St_15_Active:
      ck_6 = task_ver;
      switch (ck_6) {
        case Main__H3:
          t1_St_15_Active_H3 = 0;
          t1_St_15_Active = t1_St_15_Active_H3;
          break;
        case Main__H2:
          t1_St_15_Active_H2 = 0;
          t1_St_15_Active = t1_St_15_Active_H2;
          break;
        case Main__H1:
          t1_St_15_Active_H1 = 2;
          t1_St_15_Active = t1_St_15_Active_H1;
          break;
        case Main__S:
          t1_St_15_Active_S = 0;
          t1_St_15_Active = t1_St_15_Active_S;
          break;
        default:
          break;
      };
      t1 = t1_St_15_Active;
      break;
    case Main__St_15_Idle:
      t1_St_15_Idle = 0;
      t1 = t1_St_15_Idle;
      break;
    default:
      break;
  };
  trk_t1 = t1;
  occ_t1 = (v_54+trk_t1);
  switch (ck_42) {
    case Main__St_2_Fail:
      max_occ_2_St_2_Fail = 0;
      max_occ_2 = max_occ_2_St_2_Fail;
      break;
    case Main__St_2_Free:
      max_occ_2_St_2_Free = 2;
      max_occ_2 = max_occ_2_St_2_Free;
      break;
    default:
      break;
  };
  max_occ_t1 = max_occ_2;
  v_73 = (occ_t1<=max_occ_t1);
  switch (ck_35) {
    case Main__St_5_Active:
      switch (ck_36) {
        case Main__H3:
          t2_10_St_5_Active_H3 = 0;
          t2_10_St_5_Active = t2_10_St_5_Active_H3;
          break;
        case Main__H2:
          t2_10_St_5_Active_H2 = 1;
          t2_10_St_5_Active = t2_10_St_5_Active_H2;
          break;
        case Main__H1:
          t2_10_St_5_Active_H1 = 0;
          t2_10_St_5_Active = t2_10_St_5_Active_H1;
          break;
        case Main__S:
          t2_10_St_5_Active_S = 0;
          t2_10_St_5_Active = t2_10_St_5_Active_S;
          break;
        default:
          break;
      };
      t2_10 = t2_10_St_5_Active;
      break;
    case Main__St_5_Idle:
      t2_10_St_5_Idle = 0;
      t2_10 = t2_10_St_5_Idle;
      break;
    default:
      break;
  };
  c_img_t2 = t2_10;
  switch (ck_32) {
    case Main__St_6_Active:
      switch (ck_33) {
        case Main__H3:
          t2_9_St_6_Active_H3 = 0;
          t2_9_St_6_Active = t2_9_St_6_Active_H3;
          break;
        case Main__H2:
          t2_9_St_6_Active_H2 = 0;
          t2_9_St_6_Active = t2_9_St_6_Active_H2;
          break;
        case Main__H1:
          t2_9_St_6_Active_H1 = 0;
          t2_9_St_6_Active = t2_9_St_6_Active_H1;
          break;
        case Main__S:
          t2_9_St_6_Active_S = 0;
          t2_9_St_6_Active = t2_9_St_6_Active_S;
          break;
        default:
          break;
      };
      t2_9 = t2_9_St_6_Active;
      break;
    case Main__St_6_Idle:
      t2_9_St_6_Idle = 0;
      t2_9 = t2_9_St_6_Idle;
      break;
    default:
      break;
  };
  me_imu_t2 = t2_9;
  v_55 = (c_img_t2+me_imu_t2);
  switch (ck_29) {
    case Main__St_7_Active:
      switch (ck_30) {
        case Main__H3:
          t2_8_St_7_Active_H3 = 0;
          t2_8_St_7_Active = t2_8_St_7_Active_H3;
          break;
        case Main__H2:
          t2_8_St_7_Active_H2 = 1;
          t2_8_St_7_Active = t2_8_St_7_Active_H2;
          break;
        case Main__H1:
          t2_8_St_7_Active_H1 = 0;
          t2_8_St_7_Active = t2_8_St_7_Active_H1;
          break;
        case Main__S:
          t2_8_St_7_Active_S = 0;
          t2_8_St_7_Active = t2_8_St_7_Active_S;
          break;
        default:
          break;
      };
      t2_8 = t2_8_St_7_Active;
      break;
    case Main__St_7_Idle:
      t2_8_St_7_Idle = 0;
      t2_8 = t2_8_St_7_Idle;
      break;
    default:
      break;
  };
  me_img_t2 = t2_8;
  v_56 = (v_55+me_img_t2);
  switch (ck_26) {
    case Main__St_8_Active:
      switch (ck_27) {
        case Main__H3:
          t2_7_St_8_Active_H3 = 0;
          t2_7_St_8_Active = t2_7_St_8_Active_H3;
          break;
        case Main__H2:
          t2_7_St_8_Active_H2 = 1;
          t2_7_St_8_Active = t2_7_St_8_Active_H2;
          break;
        case Main__H1:
          t2_7_St_8_Active_H1 = 0;
          t2_7_St_8_Active = t2_7_St_8_Active_H1;
          break;
        case Main__S:
          t2_7_St_8_Active_S = 0;
          t2_7_St_8_Active = t2_7_St_8_Active_S;
          break;
        default:
          break;
      };
      t2_7 = t2_7_St_8_Active;
      break;
    case Main__St_8_Idle:
      t2_7_St_8_Idle = 0;
      t2_7 = t2_7_St_8_Idle;
      break;
    default:
      break;
  };
  sl_t2 = t2_7;
  v_57 = (v_56+sl_t2);
  switch (ck_23) {
    case Main__St_9_Active:
      switch (ck_24) {
        case Main__H3:
          t2_6_St_9_Active_H3 = 0;
          t2_6_St_9_Active = t2_6_St_9_Active_H3;
          break;
        case Main__H2:
          t2_6_St_9_Active_H2 = 1;
          t2_6_St_9_Active = t2_6_St_9_Active_H2;
          break;
        case Main__H1:
          t2_6_St_9_Active_H1 = 0;
          t2_6_St_9_Active = t2_6_St_9_Active_H1;
          break;
        case Main__S:
          t2_6_St_9_Active_S = 0;
          t2_6_St_9_Active = t2_6_St_9_Active_S;
          break;
        default:
          break;
      };
      t2_6 = t2_6_St_9_Active;
      break;
    case Main__St_9_Idle:
      t2_6_St_9_Idle = 0;
      t2_6 = t2_6_St_9_Idle;
      break;
    default:
      break;
  };
  oa_t2 = t2_6;
  v_58 = (v_57+oa_t2);
  switch (ck_20) {
    case Main__St_10_Active:
      switch (ck_21) {
        case Main__H3:
          t2_5_St_10_Active_H3 = 0;
          t2_5_St_10_Active = t2_5_St_10_Active_H3;
          break;
        case Main__H2:
          t2_5_St_10_Active_H2 = 1;
          t2_5_St_10_Active = t2_5_St_10_Active_H2;
          break;
        case Main__H1:
          t2_5_St_10_Active_H1 = 0;
          t2_5_St_10_Active = t2_5_St_10_Active_H1;
          break;
        case Main__S:
          t2_5_St_10_Active_S = 0;
          t2_5_St_10_Active = t2_5_St_10_Active_S;
          break;
        default:
          break;
      };
      t2_5 = t2_5_St_10_Active;
      break;
    case Main__St_10_Idle:
      t2_5_St_10_Idle = 0;
      t2_5 = t2_5_St_10_Idle;
      break;
    default:
      break;
  };
  tl_t2 = t2_5;
  v_59 = (v_58+tl_t2);
  switch (ck_17) {
    case Main__St_11_Active:
      switch (ck_18) {
        case Main__H3:
          t2_4_St_11_Active_H3 = 0;
          t2_4_St_11_Active = t2_4_St_11_Active_H3;
          break;
        case Main__H2:
          t2_4_St_11_Active_H2 = 1;
          t2_4_St_11_Active = t2_4_St_11_Active_H2;
          break;
        case Main__H1:
          t2_4_St_11_Active_H1 = 0;
          t2_4_St_11_Active = t2_4_St_11_Active_H1;
          break;
        case Main__S:
          t2_4_St_11_Active_S = 0;
          t2_4_St_11_Active = t2_4_St_11_Active_S;
          break;
        default:
          break;
      };
      t2_4 = t2_4_St_11_Active;
      break;
    case Main__St_11_Idle:
      t2_4_St_11_Idle = 0;
      t2_4 = t2_4_St_11_Idle;
      break;
    default:
      break;
  };
  rs_t2 = t2_4;
  v_60 = (v_59+rs_t2);
  switch (ck_14) {
    case Main__St_12_Active:
      switch (ck_15) {
        case Main__H3:
          t2_3_St_12_Active_H3 = 0;
          t2_3_St_12_Active = t2_3_St_12_Active_H3;
          break;
        case Main__H2:
          t2_3_St_12_Active_H2 = 2;
          t2_3_St_12_Active = t2_3_St_12_Active_H2;
          break;
        case Main__H1:
          t2_3_St_12_Active_H1 = 0;
          t2_3_St_12_Active = t2_3_St_12_Active_H1;
          break;
        case Main__S:
          t2_3_St_12_Active_S = 0;
          t2_3_St_12_Active = t2_3_St_12_Active_S;
          break;
        default:
          break;
      };
      t2_3 = t2_3_St_12_Active;
      break;
    case Main__St_12_Idle:
      t2_3_St_12_Idle = 0;
      t2_3 = t2_3_St_12_Idle;
      break;
    default:
      break;
  };
  rb_t2 = t2_3;
  v_61 = (v_60+rb_t2);
  switch (ck_8) {
    case Main__St_14_Active:
      switch (ck_9) {
        case Main__H3:
          t2_1_St_14_Active_H3 = 0;
          t2_1_St_14_Active = t2_1_St_14_Active_H3;
          break;
        case Main__H2:
          t2_1_St_14_Active_H2 = 2;
          t2_1_St_14_Active = t2_1_St_14_Active_H2;
          break;
        case Main__H1:
          t2_1_St_14_Active_H1 = 0;
          t2_1_St_14_Active = t2_1_St_14_Active_H1;
          break;
        case Main__S:
          t2_1_St_14_Active_S = 0;
          t2_1_St_14_Active = t2_1_St_14_Active_S;
          break;
        default:
          break;
      };
      t2_1 = t2_1_St_14_Active;
      break;
    case Main__St_14_Idle:
      t2_1_St_14_Idle = 0;
      t2_1 = t2_1_St_14_Idle;
      break;
    default:
      break;
  };
  dt_t2 = t2_1;
  v_62 = (v_61+dt_t2);
  switch (ck_11) {
    case Main__St_13_Active:
      switch (ck_12) {
        case Main__H3:
          t2_2_St_13_Active_H3 = 0;
          t2_2_St_13_Active = t2_2_St_13_Active_H3;
          break;
        case Main__H2:
          t2_2_St_13_Active_H2 = 2;
          t2_2_St_13_Active = t2_2_St_13_Active_H2;
          break;
        case Main__H1:
          t2_2_St_13_Active_H1 = 0;
          t2_2_St_13_Active = t2_2_St_13_Active_H1;
          break;
        case Main__S:
          t2_2_St_13_Active_S = 0;
          t2_2_St_13_Active = t2_2_St_13_Active_S;
          break;
        default:
          break;
      };
      t2_2 = t2_2_St_13_Active;
      break;
    case Main__St_13_Idle:
      t2_2_St_13_Idle = 0;
      t2_2 = t2_2_St_13_Idle;
      break;
    default:
      break;
  };
  rpl_t2 = t2_2;
  v_63 = (v_62+rpl_t2);
  switch (ck_5) {
    case Main__St_15_Active:
      switch (ck_6) {
        case Main__H3:
          t2_St_15_Active_H3 = 0;
          t2_St_15_Active = t2_St_15_Active_H3;
          break;
        case Main__H2:
          t2_St_15_Active_H2 = 2;
          t2_St_15_Active = t2_St_15_Active_H2;
          break;
        case Main__H1:
          t2_St_15_Active_H1 = 0;
          t2_St_15_Active = t2_St_15_Active_H1;
          break;
        case Main__S:
          t2_St_15_Active_S = 0;
          t2_St_15_Active = t2_St_15_Active_S;
          break;
        default:
          break;
      };
      t2 = t2_St_15_Active;
      break;
    case Main__St_15_Idle:
      t2_St_15_Idle = 0;
      t2 = t2_St_15_Idle;
      break;
    default:
      break;
  };
  trk_t2 = t2;
  occ_t2 = (v_63+trk_t2);
  switch (ck_40) {
    case Main__St_3_Fail:
      max_occ_1_St_3_Fail = 0;
      max_occ_1 = max_occ_1_St_3_Fail;
      break;
    case Main__St_3_Free:
      max_occ_1_St_3_Free = 2;
      max_occ_1 = max_occ_1_St_3_Free;
      break;
    default:
      break;
  };
  max_occ_t2 = max_occ_1;
  v_74 = (occ_t2<=max_occ_t2);
  v_75 = (v_73&&v_74);
  switch (ck_35) {
    case Main__St_5_Active:
      switch (ck_36) {
        case Main__H3:
          t3_10_St_5_Active_H3 = 1;
          t3_10_St_5_Active = t3_10_St_5_Active_H3;
          break;
        case Main__H2:
          t3_10_St_5_Active_H2 = 0;
          t3_10_St_5_Active = t3_10_St_5_Active_H2;
          break;
        case Main__H1:
          t3_10_St_5_Active_H1 = 0;
          t3_10_St_5_Active = t3_10_St_5_Active_H1;
          break;
        case Main__S:
          t3_10_St_5_Active_S = 0;
          t3_10_St_5_Active = t3_10_St_5_Active_S;
          break;
        default:
          break;
      };
      t3_10 = t3_10_St_5_Active;
      break;
    case Main__St_5_Idle:
      t3_10_St_5_Idle = 0;
      t3_10 = t3_10_St_5_Idle;
      break;
    default:
      break;
  };
  c_img_t3 = t3_10;
  switch (ck_32) {
    case Main__St_6_Active:
      switch (ck_33) {
        case Main__H3:
          t3_9_St_6_Active_H3 = 0;
          t3_9_St_6_Active = t3_9_St_6_Active_H3;
          break;
        case Main__H2:
          t3_9_St_6_Active_H2 = 0;
          t3_9_St_6_Active = t3_9_St_6_Active_H2;
          break;
        case Main__H1:
          t3_9_St_6_Active_H1 = 0;
          t3_9_St_6_Active = t3_9_St_6_Active_H1;
          break;
        case Main__S:
          t3_9_St_6_Active_S = 0;
          t3_9_St_6_Active = t3_9_St_6_Active_S;
          break;
        default:
          break;
      };
      t3_9 = t3_9_St_6_Active;
      break;
    case Main__St_6_Idle:
      t3_9_St_6_Idle = 0;
      t3_9 = t3_9_St_6_Idle;
      break;
    default:
      break;
  };
  me_imu_t3 = t3_9;
  v_64 = (c_img_t3+me_imu_t3);
  switch (ck_29) {
    case Main__St_7_Active:
      switch (ck_30) {
        case Main__H3:
          t3_8_St_7_Active_H3 = 1;
          t3_8_St_7_Active = t3_8_St_7_Active_H3;
          break;
        case Main__H2:
          t3_8_St_7_Active_H2 = 0;
          t3_8_St_7_Active = t3_8_St_7_Active_H2;
          break;
        case Main__H1:
          t3_8_St_7_Active_H1 = 0;
          t3_8_St_7_Active = t3_8_St_7_Active_H1;
          break;
        case Main__S:
          t3_8_St_7_Active_S = 0;
          t3_8_St_7_Active = t3_8_St_7_Active_S;
          break;
        default:
          break;
      };
      t3_8 = t3_8_St_7_Active;
      break;
    case Main__St_7_Idle:
      t3_8_St_7_Idle = 0;
      t3_8 = t3_8_St_7_Idle;
      break;
    default:
      break;
  };
  me_img_t3 = t3_8;
  v_65 = (v_64+me_img_t3);
  switch (ck_26) {
    case Main__St_8_Active:
      switch (ck_27) {
        case Main__H3:
          t3_7_St_8_Active_H3 = 1;
          t3_7_St_8_Active = t3_7_St_8_Active_H3;
          break;
        case Main__H2:
          t3_7_St_8_Active_H2 = 0;
          t3_7_St_8_Active = t3_7_St_8_Active_H2;
          break;
        case Main__H1:
          t3_7_St_8_Active_H1 = 0;
          t3_7_St_8_Active = t3_7_St_8_Active_H1;
          break;
        case Main__S:
          t3_7_St_8_Active_S = 0;
          t3_7_St_8_Active = t3_7_St_8_Active_S;
          break;
        default:
          break;
      };
      t3_7 = t3_7_St_8_Active;
      break;
    case Main__St_8_Idle:
      t3_7_St_8_Idle = 0;
      t3_7 = t3_7_St_8_Idle;
      break;
    default:
      break;
  };
  sl_t3 = t3_7;
  v_66 = (v_65+sl_t3);
  switch (ck_23) {
    case Main__St_9_Active:
      switch (ck_24) {
        case Main__H3:
          t3_6_St_9_Active_H3 = 1;
          t3_6_St_9_Active = t3_6_St_9_Active_H3;
          break;
        case Main__H2:
          t3_6_St_9_Active_H2 = 0;
          t3_6_St_9_Active = t3_6_St_9_Active_H2;
          break;
        case Main__H1:
          t3_6_St_9_Active_H1 = 0;
          t3_6_St_9_Active = t3_6_St_9_Active_H1;
          break;
        case Main__S:
          t3_6_St_9_Active_S = 0;
          t3_6_St_9_Active = t3_6_St_9_Active_S;
          break;
        default:
          break;
      };
      t3_6 = t3_6_St_9_Active;
      break;
    case Main__St_9_Idle:
      t3_6_St_9_Idle = 0;
      t3_6 = t3_6_St_9_Idle;
      break;
    default:
      break;
  };
  oa_t3 = t3_6;
  v_67 = (v_66+oa_t3);
  switch (ck_20) {
    case Main__St_10_Active:
      switch (ck_21) {
        case Main__H3:
          t3_5_St_10_Active_H3 = 1;
          t3_5_St_10_Active = t3_5_St_10_Active_H3;
          break;
        case Main__H2:
          t3_5_St_10_Active_H2 = 0;
          t3_5_St_10_Active = t3_5_St_10_Active_H2;
          break;
        case Main__H1:
          t3_5_St_10_Active_H1 = 0;
          t3_5_St_10_Active = t3_5_St_10_Active_H1;
          break;
        case Main__S:
          t3_5_St_10_Active_S = 0;
          t3_5_St_10_Active = t3_5_St_10_Active_S;
          break;
        default:
          break;
      };
      t3_5 = t3_5_St_10_Active;
      break;
    case Main__St_10_Idle:
      t3_5_St_10_Idle = 0;
      t3_5 = t3_5_St_10_Idle;
      break;
    default:
      break;
  };
  tl_t3 = t3_5;
  v_68 = (v_67+tl_t3);
  switch (ck_17) {
    case Main__St_11_Active:
      switch (ck_18) {
        case Main__H3:
          t3_4_St_11_Active_H3 = 1;
          t3_4_St_11_Active = t3_4_St_11_Active_H3;
          break;
        case Main__H2:
          t3_4_St_11_Active_H2 = 0;
          t3_4_St_11_Active = t3_4_St_11_Active_H2;
          break;
        case Main__H1:
          t3_4_St_11_Active_H1 = 0;
          t3_4_St_11_Active = t3_4_St_11_Active_H1;
          break;
        case Main__S:
          t3_4_St_11_Active_S = 0;
          t3_4_St_11_Active = t3_4_St_11_Active_S;
          break;
        default:
          break;
      };
      t3_4 = t3_4_St_11_Active;
      break;
    case Main__St_11_Idle:
      t3_4_St_11_Idle = 0;
      t3_4 = t3_4_St_11_Idle;
      break;
    default:
      break;
  };
  rs_t3 = t3_4;
  v_69 = (v_68+rs_t3);
  switch (ck_14) {
    case Main__St_12_Active:
      switch (ck_15) {
        case Main__H3:
          t3_3_St_12_Active_H3 = 2;
          t3_3_St_12_Active = t3_3_St_12_Active_H3;
          break;
        case Main__H2:
          t3_3_St_12_Active_H2 = 0;
          t3_3_St_12_Active = t3_3_St_12_Active_H2;
          break;
        case Main__H1:
          t3_3_St_12_Active_H1 = 0;
          t3_3_St_12_Active = t3_3_St_12_Active_H1;
          break;
        case Main__S:
          t3_3_St_12_Active_S = 0;
          t3_3_St_12_Active = t3_3_St_12_Active_S;
          break;
        default:
          break;
      };
      t3_3 = t3_3_St_12_Active;
      break;
    case Main__St_12_Idle:
      t3_3_St_12_Idle = 0;
      t3_3 = t3_3_St_12_Idle;
      break;
    default:
      break;
  };
  rb_t3 = t3_3;
  v_70 = (v_69+rb_t3);
  switch (ck_8) {
    case Main__St_14_Active:
      switch (ck_9) {
        case Main__H3:
          t3_1_St_14_Active_H3 = 2;
          t3_1_St_14_Active = t3_1_St_14_Active_H3;
          break;
        case Main__H2:
          t3_1_St_14_Active_H2 = 0;
          t3_1_St_14_Active = t3_1_St_14_Active_H2;
          break;
        case Main__H1:
          t3_1_St_14_Active_H1 = 0;
          t3_1_St_14_Active = t3_1_St_14_Active_H1;
          break;
        case Main__S:
          t3_1_St_14_Active_S = 0;
          t3_1_St_14_Active = t3_1_St_14_Active_S;
          break;
        default:
          break;
      };
      t3_1 = t3_1_St_14_Active;
      break;
    case Main__St_14_Idle:
      t3_1_St_14_Idle = 0;
      t3_1 = t3_1_St_14_Idle;
      break;
    default:
      break;
  };
  dt_t3 = t3_1;
  v_71 = (v_70+dt_t3);
  switch (ck_11) {
    case Main__St_13_Active:
      switch (ck_12) {
        case Main__H3:
          t3_2_St_13_Active_H3 = 2;
          t3_2_St_13_Active = t3_2_St_13_Active_H3;
          break;
        case Main__H2:
          t3_2_St_13_Active_H2 = 0;
          t3_2_St_13_Active = t3_2_St_13_Active_H2;
          break;
        case Main__H1:
          t3_2_St_13_Active_H1 = 0;
          t3_2_St_13_Active = t3_2_St_13_Active_H1;
          break;
        case Main__S:
          t3_2_St_13_Active_S = 0;
          t3_2_St_13_Active = t3_2_St_13_Active_S;
          break;
        default:
          break;
      };
      t3_2 = t3_2_St_13_Active;
      break;
    case Main__St_13_Idle:
      t3_2_St_13_Idle = 0;
      t3_2 = t3_2_St_13_Idle;
      break;
    default:
      break;
  };
  rpl_t3 = t3_2;
  v_72 = (v_71+rpl_t3);
  switch (ck_5) {
    case Main__St_15_Active:
      switch (ck_6) {
        case Main__H3:
          t3_St_15_Active_H3 = 2;
          t3_St_15_Active = t3_St_15_Active_H3;
          break;
        case Main__H2:
          t3_St_15_Active_H2 = 0;
          t3_St_15_Active = t3_St_15_Active_H2;
          break;
        case Main__H1:
          t3_St_15_Active_H1 = 0;
          t3_St_15_Active = t3_St_15_Active_H1;
          break;
        case Main__S:
          t3_St_15_Active_S = 0;
          t3_St_15_Active = t3_St_15_Active_S;
          break;
        default:
          break;
      };
      t3 = t3_St_15_Active;
      break;
    case Main__St_15_Idle:
      t3_St_15_Idle = 0;
      t3 = t3_St_15_Idle;
      break;
    default:
      break;
  };
  trk_t3 = t3;
  occ_t3 = (v_72+trk_t3);
  switch (ck_38) {
    case Main__St_4_Fail:
      max_occ_St_4_Fail = 0;
      max_occ = max_occ_St_4_Fail;
      break;
    case Main__St_4_Free:
      max_occ_St_4_Free = 1;
      max_occ = max_occ_St_4_Free;
      break;
    default:
      break;
  };
  max_occ_t3 = max_occ;
  v_76 = (occ_t3<=max_occ_t3);
  _out->obj_occ = (v_75&&v_76);
  v_77 = (_out->me_imu_ver==Main__S);
  v_78 = (_out->rb_ver==Main__S);
  v_79 = !(v_78);
  _out->obj_tasks = (v_77&&v_79);
  Main_controller__main_ctrlr11_step(c_c_img_ver, c_dt_ver, c_img_e, c_img_r,
                                     c_me_img_ver, c_me_imu_ver, c_oa_ver,
                                     c_rb_ver, c_rpl_ver, c_rs_ver, c_sl_ver,
                                     c_tl_ver, c_trk_ver, self->ck,
                                     self->ck_10, self->ck_13, self->ck_16,
                                     self->ck_19, self->ck_22, self->ck_25,
                                     self->ck_28, self->ck_31, self->ck_34,
                                     self->ck_37, self->ck_39, self->ck_41,
                                     self->ck_7, dt_e, dt_r, f_1, f_2, f_3,
                                     max1, max10, max11, max2, max3, max4,
                                     max5, max6, max7, max8, max9, me_img_e,
                                     me_img_r, me_imu_e, me_imu_r, min1,
                                     min10, min11, min2, min3, min4, min5,
                                     min6, min7, min8, min9, oa_e, oa_r,
                                     obj_12, obj_13, obj_14, obj_15, obj_16,
                                     obj_17, obj_18, obj_19, obj_20, obj_21,
                                     obj_22, self->pnr, self->pnr_1,
                                     self->pnr_10, self->pnr_11,
                                     self->pnr_12, self->pnr_13, self->pnr_2,
                                     self->pnr_3, self->pnr_4, self->pnr_5,
                                     self->pnr_6, self->pnr_7, self->pnr_8,
                                     self->pnr_9, pref_ver_12, pref_ver_13,
                                     pref_ver_14, pref_ver_15, pref_ver_16,
                                     pref_ver_17, pref_ver_18, pref_ver_19,
                                     pref_ver_20, pref_ver_21, pref_ver_22,
                                     self->pref_ver_task_assume_10_1,
                                     self->pref_ver_task_assume_12,
                                     self->pref_ver_task_assume_1_1,
                                     self->pref_ver_task_assume_2_1,
                                     self->pref_ver_task_assume_3_1,
                                     self->pref_ver_task_assume_4_1,
                                     self->pref_ver_task_assume_5_1,
                                     self->pref_ver_task_assume_6_1,
                                     self->pref_ver_task_assume_7_1,
                                     self->pref_ver_task_assume_8_1,
                                     self->pref_ver_task_assume_9_1,
                                     self->pref_ver_task_guarantee_10_1,
                                     self->pref_ver_task_guarantee_12,
                                     self->pref_ver_task_guarantee_1_1,
                                     self->pref_ver_task_guarantee_2_1,
                                     self->pref_ver_task_guarantee_3_1,
                                     self->pref_ver_task_guarantee_4_1,
                                     self->pref_ver_task_guarantee_5_1,
                                     self->pref_ver_task_guarantee_6_1,
                                     self->pref_ver_task_guarantee_7_1,
                                     self->pref_ver_task_guarantee_8_1,
                                     self->pref_ver_task_guarantee_9_1, rb_e,
                                     rb_r, rp_1, rp_2, rp_3, rpl_e, rpl_r,
                                     rs_e, rs_r, sl_e, sl_r, texe1, texe10,
                                     texe11, texe2, texe3, texe4, texe5,
                                     texe6, texe7, texe8, texe9, tl_e, tl_r,
                                     trk_e, trk_r,
                                     &Main_controller__main_ctrlr11_out_st);
  c_c_img_pref = Main_controller__main_ctrlr11_out_st.c_c_img_pref;
  c_dt_pref = Main_controller__main_ctrlr11_out_st.c_dt_pref;
  c_me_img_pref = Main_controller__main_ctrlr11_out_st.c_me_img_pref;
  c_me_imu_pref = Main_controller__main_ctrlr11_out_st.c_me_imu_pref;
  c_oa_pref = Main_controller__main_ctrlr11_out_st.c_oa_pref;
  c_rb_pref = Main_controller__main_ctrlr11_out_st.c_rb_pref;
  c_rpl_pref = Main_controller__main_ctrlr11_out_st.c_rpl_pref;
  c_rs_pref = Main_controller__main_ctrlr11_out_st.c_rs_pref;
  c_sl_pref = Main_controller__main_ctrlr11_out_st.c_sl_pref;
  c_tl_pref = Main_controller__main_ctrlr11_out_st.c_tl_pref;
  c_trk_pref = Main_controller__main_ctrlr11_out_st.c_trk_pref;
  pref_ver_10 = pref_ver_12;
  c_img_pref = pref_ver_10;
  v_80 = (c_c_img_ver==c_img_pref);
  v_81 = (!(c_c_img_pref)||v_80);
  pref_ver_9 = pref_ver_13;
  me_imu_pref = pref_ver_9;
  v_82 = (c_me_imu_ver==me_imu_pref);
  v_83 = (!(c_me_imu_pref)||v_82);
  v_84 = (v_81&&v_83);
  pref_ver_8 = pref_ver_14;
  me_img_pref = pref_ver_8;
  v_85 = (c_me_img_ver==me_img_pref);
  v_86 = (!(c_me_img_pref)||v_85);
  v_87 = (v_84&&v_86);
  pref_ver_7 = pref_ver_15;
  sl_pref = pref_ver_7;
  v_88 = (c_sl_ver==sl_pref);
  v_89 = (!(c_sl_pref)||v_88);
  v_90 = (v_87&&v_89);
  pref_ver_6 = pref_ver_16;
  oa_pref = pref_ver_6;
  v_91 = (c_oa_ver==oa_pref);
  v_92 = (!(c_oa_pref)||v_91);
  v_93 = (v_90&&v_92);
  pref_ver_5 = pref_ver_17;
  tl_pref = pref_ver_5;
  v_94 = (c_tl_ver==tl_pref);
  v_95 = (!(c_tl_pref)||v_94);
  v_96 = (v_93&&v_95);
  pref_ver_4 = pref_ver_18;
  rs_pref = pref_ver_4;
  v_97 = (c_rs_ver==rs_pref);
  v_98 = (!(c_rs_pref)||v_97);
  v_99 = (v_96&&v_98);
  pref_ver_3 = pref_ver_19;
  rb_pref = pref_ver_3;
  v_100 = (c_rb_ver==rb_pref);
  v_101 = (!(c_rb_pref)||v_100);
  v_102 = (v_99&&v_101);
  pref_ver_1 = pref_ver_21;
  dt_pref = pref_ver_1;
  v_103 = (c_dt_ver==dt_pref);
  v_104 = (!(c_dt_pref)||v_103);
  v_105 = (v_102&&v_104);
  pref_ver_2 = pref_ver_20;
  rpl_pref = pref_ver_2;
  v_106 = (c_rpl_ver==rpl_pref);
  v_107 = (!(c_rpl_pref)||v_106);
  v_108 = (v_105&&v_107);
  pref_ver = pref_ver_22;
  trk_pref = pref_ver;
  v_109 = (c_trk_ver==trk_pref);
  v_110 = (!(c_trk_pref)||v_109);
  _out->obj_pref = (v_108&&v_110);
  obj = obj_22;
  switch (self->ck) {
    case Main__St_15_Active:
      if (end_task) {
        r_13_St_15_Active = true;
      } else {
        r_13_St_15_Active = self->pnr_13;
      };
      r_13 = r_13_St_15_Active;
      break;
    case Main__St_15_Idle:
      if (req_task) {
        r_13_St_15_Idle = true;
      } else {
        r_13_St_15_Idle = self->pnr_13;
      };
      r_13 = r_13_St_15_Idle;
      break;
    default:
      break;
  };
  switch (ck_5) {
    case Main__St_15_Active:
      ns_13_St_15_Active = Main__St_15_Active;
      ns_13 = ns_13_St_15_Active;
      break;
    case Main__St_15_Idle:
      ns_13_St_15_Idle = Main__St_15_Idle;
      ns_13 = ns_13_St_15_Idle;
      break;
    default:
      break;
  };
  self->ck = ns_13;
  switch (ck_5) {
    case Main__St_15_Active:
      nr_13_St_15_Active = false;
      nr_13 = nr_13_St_15_Active;
      break;
    case Main__St_15_Idle:
      nr_13_St_15_Idle = false;
      nr_13 = nr_13_St_15_Idle;
      break;
    default:
      break;
  };
  self->pnr_13 = nr_13;
  obj_1 = obj_21;
  switch (self->ck_7) {
    case Main__St_14_Active:
      if (end_task_1) {
        r_12_St_14_Active = true;
      } else {
        r_12_St_14_Active = self->pnr_12;
      };
      r_12 = r_12_St_14_Active;
      break;
    case Main__St_14_Idle:
      if (req_task_1) {
        r_12_St_14_Idle = true;
      } else {
        r_12_St_14_Idle = self->pnr_12;
      };
      r_12 = r_12_St_14_Idle;
      break;
    default:
      break;
  };
  switch (ck_8) {
    case Main__St_14_Active:
      ns_12_St_14_Active = Main__St_14_Active;
      ns_12 = ns_12_St_14_Active;
      break;
    case Main__St_14_Idle:
      ns_12_St_14_Idle = Main__St_14_Idle;
      ns_12 = ns_12_St_14_Idle;
      break;
    default:
      break;
  };
  self->ck_7 = ns_12;
  switch (ck_8) {
    case Main__St_14_Active:
      nr_12_St_14_Active = false;
      nr_12 = nr_12_St_14_Active;
      break;
    case Main__St_14_Idle:
      nr_12_St_14_Idle = false;
      nr_12 = nr_12_St_14_Idle;
      break;
    default:
      break;
  };
  self->pnr_12 = nr_12;
  obj_2 = obj_20;
  switch (self->ck_10) {
    case Main__St_13_Active:
      if (end_task_2) {
        r_11_St_13_Active = true;
      } else {
        r_11_St_13_Active = self->pnr_11;
      };
      r_11 = r_11_St_13_Active;
      break;
    case Main__St_13_Idle:
      if (req_task_2) {
        r_11_St_13_Idle = true;
      } else {
        r_11_St_13_Idle = self->pnr_11;
      };
      r_11 = r_11_St_13_Idle;
      break;
    default:
      break;
  };
  switch (ck_11) {
    case Main__St_13_Active:
      ns_11_St_13_Active = Main__St_13_Active;
      ns_11 = ns_11_St_13_Active;
      break;
    case Main__St_13_Idle:
      ns_11_St_13_Idle = Main__St_13_Idle;
      ns_11 = ns_11_St_13_Idle;
      break;
    default:
      break;
  };
  self->ck_10 = ns_11;
  switch (ck_11) {
    case Main__St_13_Active:
      nr_11_St_13_Active = false;
      nr_11 = nr_11_St_13_Active;
      break;
    case Main__St_13_Idle:
      nr_11_St_13_Idle = false;
      nr_11 = nr_11_St_13_Idle;
      break;
    default:
      break;
  };
  self->pnr_11 = nr_11;
  obj_3 = obj_19;
  switch (self->ck_13) {
    case Main__St_12_Active:
      if (end_task_3) {
        r_10_St_12_Active = true;
      } else {
        r_10_St_12_Active = self->pnr_10;
      };
      r_10 = r_10_St_12_Active;
      break;
    case Main__St_12_Idle:
      if (req_task_3) {
        r_10_St_12_Idle = true;
      } else {
        r_10_St_12_Idle = self->pnr_10;
      };
      r_10 = r_10_St_12_Idle;
      break;
    default:
      break;
  };
  switch (ck_14) {
    case Main__St_12_Active:
      ns_10_St_12_Active = Main__St_12_Active;
      ns_10 = ns_10_St_12_Active;
      break;
    case Main__St_12_Idle:
      ns_10_St_12_Idle = Main__St_12_Idle;
      ns_10 = ns_10_St_12_Idle;
      break;
    default:
      break;
  };
  self->ck_13 = ns_10;
  switch (ck_14) {
    case Main__St_12_Active:
      nr_10_St_12_Active = false;
      nr_10 = nr_10_St_12_Active;
      break;
    case Main__St_12_Idle:
      nr_10_St_12_Idle = false;
      nr_10 = nr_10_St_12_Idle;
      break;
    default:
      break;
  };
  self->pnr_10 = nr_10;
  obj_4 = obj_18;
  switch (self->ck_16) {
    case Main__St_11_Active:
      if (end_task_4) {
        r_9_St_11_Active = true;
      } else {
        r_9_St_11_Active = self->pnr_9;
      };
      r_9 = r_9_St_11_Active;
      break;
    case Main__St_11_Idle:
      if (req_task_4) {
        r_9_St_11_Idle = true;
      } else {
        r_9_St_11_Idle = self->pnr_9;
      };
      r_9 = r_9_St_11_Idle;
      break;
    default:
      break;
  };
  switch (ck_17) {
    case Main__St_11_Active:
      ns_9_St_11_Active = Main__St_11_Active;
      ns_9 = ns_9_St_11_Active;
      break;
    case Main__St_11_Idle:
      ns_9_St_11_Idle = Main__St_11_Idle;
      ns_9 = ns_9_St_11_Idle;
      break;
    default:
      break;
  };
  self->ck_16 = ns_9;
  switch (ck_17) {
    case Main__St_11_Active:
      nr_9_St_11_Active = false;
      nr_9 = nr_9_St_11_Active;
      break;
    case Main__St_11_Idle:
      nr_9_St_11_Idle = false;
      nr_9 = nr_9_St_11_Idle;
      break;
    default:
      break;
  };
  self->pnr_9 = nr_9;
  obj_5 = obj_17;
  switch (self->ck_19) {
    case Main__St_10_Active:
      if (end_task_5) {
        r_8_St_10_Active = true;
      } else {
        r_8_St_10_Active = self->pnr_8;
      };
      r_8 = r_8_St_10_Active;
      break;
    case Main__St_10_Idle:
      if (req_task_5) {
        r_8_St_10_Idle = true;
      } else {
        r_8_St_10_Idle = self->pnr_8;
      };
      r_8 = r_8_St_10_Idle;
      break;
    default:
      break;
  };
  switch (ck_20) {
    case Main__St_10_Active:
      ns_8_St_10_Active = Main__St_10_Active;
      ns_8 = ns_8_St_10_Active;
      break;
    case Main__St_10_Idle:
      ns_8_St_10_Idle = Main__St_10_Idle;
      ns_8 = ns_8_St_10_Idle;
      break;
    default:
      break;
  };
  self->ck_19 = ns_8;
  switch (ck_20) {
    case Main__St_10_Active:
      nr_8_St_10_Active = false;
      nr_8 = nr_8_St_10_Active;
      break;
    case Main__St_10_Idle:
      nr_8_St_10_Idle = false;
      nr_8 = nr_8_St_10_Idle;
      break;
    default:
      break;
  };
  self->pnr_8 = nr_8;
  obj_6 = obj_16;
  switch (self->ck_22) {
    case Main__St_9_Active:
      if (end_task_6) {
        r_7_St_9_Active = true;
      } else {
        r_7_St_9_Active = self->pnr_7;
      };
      r_7 = r_7_St_9_Active;
      break;
    case Main__St_9_Idle:
      if (req_task_6) {
        r_7_St_9_Idle = true;
      } else {
        r_7_St_9_Idle = self->pnr_7;
      };
      r_7 = r_7_St_9_Idle;
      break;
    default:
      break;
  };
  switch (ck_23) {
    case Main__St_9_Active:
      ns_7_St_9_Active = Main__St_9_Active;
      ns_7 = ns_7_St_9_Active;
      break;
    case Main__St_9_Idle:
      ns_7_St_9_Idle = Main__St_9_Idle;
      ns_7 = ns_7_St_9_Idle;
      break;
    default:
      break;
  };
  self->ck_22 = ns_7;
  switch (ck_23) {
    case Main__St_9_Active:
      nr_7_St_9_Active = false;
      nr_7 = nr_7_St_9_Active;
      break;
    case Main__St_9_Idle:
      nr_7_St_9_Idle = false;
      nr_7 = nr_7_St_9_Idle;
      break;
    default:
      break;
  };
  self->pnr_7 = nr_7;
  obj_7 = obj_15;
  switch (self->ck_25) {
    case Main__St_8_Active:
      if (end_task_7) {
        r_6_St_8_Active = true;
      } else {
        r_6_St_8_Active = self->pnr_6;
      };
      r_6 = r_6_St_8_Active;
      break;
    case Main__St_8_Idle:
      if (req_task_7) {
        r_6_St_8_Idle = true;
      } else {
        r_6_St_8_Idle = self->pnr_6;
      };
      r_6 = r_6_St_8_Idle;
      break;
    default:
      break;
  };
  switch (ck_26) {
    case Main__St_8_Active:
      ns_6_St_8_Active = Main__St_8_Active;
      ns_6 = ns_6_St_8_Active;
      break;
    case Main__St_8_Idle:
      ns_6_St_8_Idle = Main__St_8_Idle;
      ns_6 = ns_6_St_8_Idle;
      break;
    default:
      break;
  };
  self->ck_25 = ns_6;
  switch (ck_26) {
    case Main__St_8_Active:
      nr_6_St_8_Active = false;
      nr_6 = nr_6_St_8_Active;
      break;
    case Main__St_8_Idle:
      nr_6_St_8_Idle = false;
      nr_6 = nr_6_St_8_Idle;
      break;
    default:
      break;
  };
  self->pnr_6 = nr_6;
  obj_8 = obj_14;
  switch (self->ck_28) {
    case Main__St_7_Active:
      if (end_task_8) {
        r_5_St_7_Active = true;
      } else {
        r_5_St_7_Active = self->pnr_5;
      };
      r_5 = r_5_St_7_Active;
      break;
    case Main__St_7_Idle:
      if (req_task_8) {
        r_5_St_7_Idle = true;
      } else {
        r_5_St_7_Idle = self->pnr_5;
      };
      r_5 = r_5_St_7_Idle;
      break;
    default:
      break;
  };
  switch (ck_29) {
    case Main__St_7_Active:
      ns_5_St_7_Active = Main__St_7_Active;
      ns_5 = ns_5_St_7_Active;
      break;
    case Main__St_7_Idle:
      ns_5_St_7_Idle = Main__St_7_Idle;
      ns_5 = ns_5_St_7_Idle;
      break;
    default:
      break;
  };
  self->ck_28 = ns_5;
  switch (ck_29) {
    case Main__St_7_Active:
      nr_5_St_7_Active = false;
      nr_5 = nr_5_St_7_Active;
      break;
    case Main__St_7_Idle:
      nr_5_St_7_Idle = false;
      nr_5 = nr_5_St_7_Idle;
      break;
    default:
      break;
  };
  self->pnr_5 = nr_5;
  obj_9 = obj_13;
  switch (self->ck_31) {
    case Main__St_6_Active:
      if (end_task_9) {
        r_4_St_6_Active = true;
      } else {
        r_4_St_6_Active = self->pnr_4;
      };
      r_4 = r_4_St_6_Active;
      break;
    case Main__St_6_Idle:
      if (req_task_9) {
        r_4_St_6_Idle = true;
      } else {
        r_4_St_6_Idle = self->pnr_4;
      };
      r_4 = r_4_St_6_Idle;
      break;
    default:
      break;
  };
  switch (ck_32) {
    case Main__St_6_Active:
      ns_4_St_6_Active = Main__St_6_Active;
      ns_4 = ns_4_St_6_Active;
      break;
    case Main__St_6_Idle:
      ns_4_St_6_Idle = Main__St_6_Idle;
      ns_4 = ns_4_St_6_Idle;
      break;
    default:
      break;
  };
  self->ck_31 = ns_4;
  switch (ck_32) {
    case Main__St_6_Active:
      nr_4_St_6_Active = false;
      nr_4 = nr_4_St_6_Active;
      break;
    case Main__St_6_Idle:
      nr_4_St_6_Idle = false;
      nr_4 = nr_4_St_6_Idle;
      break;
    default:
      break;
  };
  self->pnr_4 = nr_4;
  obj_10 = obj_12;
  switch (self->ck_34) {
    case Main__St_5_Active:
      if (end_task_10) {
        r_3_St_5_Active = true;
      } else {
        r_3_St_5_Active = self->pnr_3;
      };
      r_3 = r_3_St_5_Active;
      break;
    case Main__St_5_Idle:
      if (req_task_10) {
        r_3_St_5_Idle = true;
      } else {
        r_3_St_5_Idle = self->pnr_3;
      };
      r_3 = r_3_St_5_Idle;
      break;
    default:
      break;
  };
  switch (ck_35) {
    case Main__St_5_Active:
      ns_3_St_5_Active = Main__St_5_Active;
      ns_3 = ns_3_St_5_Active;
      break;
    case Main__St_5_Idle:
      ns_3_St_5_Idle = Main__St_5_Idle;
      ns_3 = ns_3_St_5_Idle;
      break;
    default:
      break;
  };
  self->ck_34 = ns_3;
  switch (ck_35) {
    case Main__St_5_Active:
      nr_3_St_5_Active = false;
      nr_3 = nr_3_St_5_Active;
      break;
    case Main__St_5_Idle:
      nr_3_St_5_Idle = false;
      nr_3 = nr_3_St_5_Idle;
      break;
    default:
      break;
  };
  self->pnr_3 = nr_3;
  switch (self->ck_37) {
    case Main__St_4_Fail:
      if (rp) {
        r_2_St_4_Fail = true;
      } else {
        r_2_St_4_Fail = self->pnr_2;
      };
      r_2 = r_2_St_4_Fail;
      break;
    case Main__St_4_Free:
      if (f) {
        r_2_St_4_Free = true;
      } else {
        r_2_St_4_Free = self->pnr_2;
      };
      r_2 = r_2_St_4_Free;
      break;
    default:
      break;
  };
  switch (ck_38) {
    case Main__St_4_Fail:
      ns_2_St_4_Fail = Main__St_4_Fail;
      ns_2 = ns_2_St_4_Fail;
      break;
    case Main__St_4_Free:
      ns_2_St_4_Free = Main__St_4_Free;
      ns_2 = ns_2_St_4_Free;
      break;
    default:
      break;
  };
  self->ck_37 = ns_2;
  switch (ck_38) {
    case Main__St_4_Fail:
      nr_2_St_4_Fail = false;
      nr_2 = nr_2_St_4_Fail;
      break;
    case Main__St_4_Free:
      nr_2_St_4_Free = false;
      nr_2 = nr_2_St_4_Free;
      break;
    default:
      break;
  };
  self->pnr_2 = nr_2;
  switch (self->ck_39) {
    case Main__St_3_Fail:
      if (rp_4) {
        r_1_St_3_Fail = true;
      } else {
        r_1_St_3_Fail = self->pnr_1;
      };
      r_1 = r_1_St_3_Fail;
      break;
    case Main__St_3_Free:
      if (f_4) {
        r_1_St_3_Free = true;
      } else {
        r_1_St_3_Free = self->pnr_1;
      };
      r_1 = r_1_St_3_Free;
      break;
    default:
      break;
  };
  switch (ck_40) {
    case Main__St_3_Fail:
      ns_1_St_3_Fail = Main__St_3_Fail;
      ns_1 = ns_1_St_3_Fail;
      break;
    case Main__St_3_Free:
      ns_1_St_3_Free = Main__St_3_Free;
      ns_1 = ns_1_St_3_Free;
      break;
    default:
      break;
  };
  self->ck_39 = ns_1;
  switch (ck_40) {
    case Main__St_3_Fail:
      nr_1_St_3_Fail = false;
      nr_1 = nr_1_St_3_Fail;
      break;
    case Main__St_3_Free:
      nr_1_St_3_Free = false;
      nr_1 = nr_1_St_3_Free;
      break;
    default:
      break;
  };
  self->pnr_1 = nr_1;
  switch (self->ck_41) {
    case Main__St_2_Fail:
      if (rp_5) {
        r_St_2_Fail = true;
      } else {
        r_St_2_Fail = self->pnr;
      };
      r = r_St_2_Fail;
      break;
    case Main__St_2_Free:
      if (f_5) {
        r_St_2_Free = true;
      } else {
        r_St_2_Free = self->pnr;
      };
      r = r_St_2_Free;
      break;
    default:
      break;
  };
  switch (ck_42) {
    case Main__St_2_Fail:
      ns_St_2_Fail = Main__St_2_Fail;
      ns = ns_St_2_Fail;
      break;
    case Main__St_2_Free:
      ns_St_2_Free = Main__St_2_Free;
      ns = ns_St_2_Free;
      break;
    default:
      break;
  };
  self->ck_41 = ns;
  switch (ck_42) {
    case Main__St_2_Fail:
      nr_St_2_Fail = false;
      nr = nr_St_2_Fail;
      break;
    case Main__St_2_Free:
      nr_St_2_Free = false;
      nr = nr_St_2_Free;
      break;
    default:
      break;
  };
  self->pnr = nr;
  self->pref_ver_task_assume_10_1 = pref_ver_task_assume_10;
  self->pref_ver_task_guarantee_10_1 = pref_ver_task_guarantee_10;
  self->pref_ver_task_assume_9_1 = pref_ver_task_assume_9;
  self->pref_ver_task_guarantee_9_1 = pref_ver_task_guarantee_9;
  self->pref_ver_task_assume_8_1 = pref_ver_task_assume_8;
  self->pref_ver_task_guarantee_8_1 = pref_ver_task_guarantee_8;
  self->pref_ver_task_assume_7_1 = pref_ver_task_assume_7;
  self->pref_ver_task_guarantee_7_1 = pref_ver_task_guarantee_7;
  self->pref_ver_task_assume_6_1 = pref_ver_task_assume_6;
  self->pref_ver_task_guarantee_6_1 = pref_ver_task_guarantee_6;
  self->pref_ver_task_assume_5_1 = pref_ver_task_assume_5;
  self->pref_ver_task_guarantee_5_1 = pref_ver_task_guarantee_5;
  self->pref_ver_task_assume_4_1 = pref_ver_task_assume_4;
  self->pref_ver_task_guarantee_4_1 = pref_ver_task_guarantee_4;
  self->pref_ver_task_assume_3_1 = pref_ver_task_assume_3;
  self->pref_ver_task_guarantee_3_1 = pref_ver_task_guarantee_3;
  self->pref_ver_task_assume_2_1 = pref_ver_task_assume_2;
  self->pref_ver_task_guarantee_2_1 = pref_ver_task_guarantee_2;
  self->pref_ver_task_assume_1_1 = pref_ver_task_assume_1;
  self->pref_ver_task_guarantee_1_1 = pref_ver_task_guarantee_1;
  self->pref_ver_task_assume_12 = pref_ver_task_assume;
  self->pref_ver_task_guarantee_12 = pref_ver_task_guarantee;
  v = (_out->obj_occ&&_out->obj_pref);;
}

