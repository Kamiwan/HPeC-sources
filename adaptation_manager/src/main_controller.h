/* --- Generated the 20/6/2018 at 0:39 --- */
/* --- heptagon compiler, version 1.04.00 (compiled mon. jun. 18 20:41:8 CET 2018) --- */
/* --- Command line: /home/gueyes/.opam/4.03.0/bin/heptc -target c main_controller.ept --- */

#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

#include "main_controller_types.h"
#include "main.h"
typedef struct Main_controller__main_ctrlr0_out {
  int c;
  int cDw1;
  int cDw10;
  int cDw11;
  int cDw2;
  int cDw3;
  int cDw4;
  int cDw5;
  int cDw6;
  int cDw7;
  int cDw8;
  int cDw9;
  int cKp1;
  int cKp10;
  int cKp11;
  int cKp2;
  int cKp3;
  int cKp4;
  int cKp5;
  int cKp6;
  int cKp7;
  int cKp8;
  int cKp9;
  int cUp1;
  int cUp10;
  int cUp11;
  int cUp2;
  int cUp3;
  int cUp4;
  int cUp5;
  int cUp6;
  int cUp7;
  int cUp8;
  int cUp9;
  int c_img_c1;
  int c_img_c2;
  int c_img_c3;
  int c_img_c4;
  int cr1;
  int cr2;
  int cr3;
  int ct10;
  int ct11;
  int ct4;
  int ct5;
  int ct6;
  int dt_c1;
  int dt_c2;
  int dt_c3;
  int me_img_c1;
  int me_img_c2;
  int me_img_c3;
  int me_img_c4;
  int me_imu_c;
  int oa_c1;
  int oa_c2;
  int oa_c3;
  int oa_c4;
  int rb_c1;
  int rb_c2;
  int rpl_c1;
  int rpl_c2;
  int rpl_c3;
  int rs_c1;
  int rs_c2;
  int rs_c3;
  int rs_c4;
  int sl_c1;
  int sl_c2;
  int sl_c3;
  int sl_c4;
  int tl_c1;
  int tl_c2;
  int tl_c3;
  int tl_c4;
  int trk_c1;
  int trk_c2;
  int trk_c3;
} Main_controller__main_ctrlr0_out;

void Main_controller__main_ctrlr0_step(int c_img_e, int c_img_r,
                                       Main__st_25 ck_23, Main__st_24 ck_25,
                                       Main__st_23 ck_27, Main__st_22 ck_29,
                                       Main__st_21 ck_31, Main__st_20 ck_33,
                                       Main__st_19 ck_35, Main__st_18 ck_37,
                                       Main__st_17 ck_39, Main__st_16 ck_41,
                                       Main__st_15 ck_43, Main__st_14 ck_45,
                                       Main__st_13 ck_47, Main__st_12 ck_49,
                                       int dt_e, int dt_r, int f_1, int f_2,
                                       int f_3, int maxtexe1, int maxtexe10,
                                       int maxtexe11, int maxtexe2,
                                       int maxtexe3, int maxtexe4,
                                       int maxtexe5, int maxtexe6,
                                       int maxtexe7, int maxtexe8,
                                       int maxtexe9, int me_img_e,
                                       int me_img_r, int me_imu_e,
                                       int me_imu_r, int mintexe1,
                                       int mintexe10, int mintexe11,
                                       int mintexe2, int mintexe3,
                                       int mintexe4, int mintexe5,
                                       int mintexe6, int mintexe7,
                                       int mintexe8, int mintexe9, int oa_e,
                                       int oa_r, int pnr, int pnr_1,
                                       int pnr_10, int pnr_11, int pnr_12,
                                       int pnr_13, int pnr_2, int pnr_3,
                                       int pnr_4, int pnr_5, int pnr_6,
                                       int pnr_7, int pnr_8, int pnr_9,
                                       int rb_e, int rb_r, int rp_1,
                                       int rp_2, int rp_3, int rpl_e,
                                       int rpl_r, int rs_e, int rs_r,
                                       int sl_e, int sl_r, int texe1,
                                       int texe10, int texe11, int texe2,
                                       int texe3, int texe4, int texe5,
                                       int texe6, int texe7, int texe8,
                                       int texe9, int tl_e, int tl_r,
                                       int trk_e, int trk_r, int v_1000,
                                       int v_1007, int v_1018, int v_1027,
                                       int v_1034, int v_1045, int v_973,
                                       int v_980, int v_991,
                                       Main_controller__main_ctrlr0_out* _out);

#endif // MAIN_CONTROLLER_H
