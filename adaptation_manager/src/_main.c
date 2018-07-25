/* --- Generated the 20/6/2018 at 0:20 --- */
/* --- heptagon compiler, version 1.04.00 (compiled mon. jun. 18 20:41:8 CET 2018) --- */
/* --- Command line: /home/gueyes/.opam/4.03.0/bin/heptc -hepts -s main -target c -target ctrln main.ept --- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "_main.h"

Main__main_mem mem;
int main(int argc, char** argv) {
  int step_c;
  int step_max;
  int texe1;
  int texe2;
  int texe3;
  int texe4;
  int texe5;
  int texe6;
  int texe7;
  int texe8;
  int texe9;
  int texe10;
  int texe11;
  int mintexe1;
  int mintexe2;
  int mintexe3;
  int mintexe4;
  int mintexe5;
  int mintexe6;
  int mintexe7;
  int mintexe8;
  int mintexe9;
  int mintexe10;
  int mintexe11;
  int maxtexe1;
  int maxtexe2;
  int maxtexe3;
  int maxtexe4;
  int maxtexe5;
  int maxtexe6;
  int maxtexe7;
  int maxtexe8;
  int maxtexe9;
  int maxtexe10;
  int maxtexe11;
  int c_img_r;
  int c_img_e;
  int me_imu_r;
  int me_imu_e;
  int me_img_r;
  int me_img_e;
  int sl_r;
  int sl_e;
  int oa_r;
  int oa_e;
  int tl_r;
  int tl_e;
  int rs_r;
  int rs_e;
  int rb_r;
  int rb_e;
  int rpl_r;
  int rpl_e;
  int dt_r;
  int dt_e;
  int trk_r;
  int trk_e;
  int f_1;
  int rp_1;
  int f_2;
  int rp_2;
  int f_3;
  int rp_3;
  Main__main_out _res;
  char buf_3[20];
  char buf_7[20];
  char buf_11[20];
  char buf_15[20];
  char buf_19[20];
  char buf_23[20];
  char buf_27[20];
  char buf_31[20];
  char buf_35[20];
  char buf_39[20];
  char buf_43[20];
  step_c = 0;
  step_max = 0;
  if ((argc==2)) {
    step_max = atoi(argv[1]);
  };
  Main__main_reset(&mem);
  while ((!(step_max)||(step_c<step_max))) {
    step_c = (step_c+1);
    
    if ((scanf("%d", &texe1)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &texe2)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &texe3)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &texe4)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &texe5)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &texe6)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &texe7)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &texe8)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &texe9)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &texe10)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &texe11)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &mintexe1)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &mintexe2)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &mintexe3)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &mintexe4)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &mintexe5)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &mintexe6)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &mintexe7)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &mintexe8)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &mintexe9)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &mintexe10)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &mintexe11)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &maxtexe1)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &maxtexe2)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &maxtexe3)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &maxtexe4)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &maxtexe5)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &maxtexe6)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &maxtexe7)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &maxtexe8)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &maxtexe9)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &maxtexe10)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &maxtexe11)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &c_img_r)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &c_img_e)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &me_imu_r)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &me_imu_e)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &me_img_r)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &me_img_e)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &sl_r)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &sl_e)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &oa_r)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &oa_e)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &tl_r)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &tl_e)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &rs_r)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &rs_e)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &rb_r)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &rb_e)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &rpl_r)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &rpl_e)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &dt_r)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &dt_e)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &trk_r)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &trk_e)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &f_1)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &rp_1)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &f_2)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &rp_2)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &f_3)==EOF)) {
      return 0;
    };;
    
    if ((scanf("%d", &rp_3)==EOF)) {
      return 0;
    };;
    Main__main_step(texe1, texe2, texe3, texe4, texe5, texe6, texe7, texe8,
                    texe9, texe10, texe11, mintexe1, mintexe2, mintexe3,
                    mintexe4, mintexe5, mintexe6, mintexe7, mintexe8,
                    mintexe9, mintexe10, mintexe11, maxtexe1, maxtexe2,
                    maxtexe3, maxtexe4, maxtexe5, maxtexe6, maxtexe7,
                    maxtexe8, maxtexe9, maxtexe10, maxtexe11, c_img_r,
                    c_img_e, me_imu_r, me_imu_e, me_img_r, me_img_e, sl_r,
                    sl_e, oa_r, oa_e, tl_r, tl_e, rs_r, rs_e, rb_r, rb_e,
                    rpl_r, rpl_e, dt_r, dt_e, trk_r, trk_e, f_1, rp_1, f_2,
                    rp_2, f_3, rp_3, &_res, &mem);
    printf("%d\n", _res.c_img_size);
    printf("%d\n", _res.c_img_act);
    printf("%d\n", _res.c_img_wt);
    printf("%s\n", string_of_Main__version_type(_res.c_img_ver, buf_3));
    printf("%d\n", _res.me_imu_size);
    printf("%d\n", _res.me_imu_act);
    printf("%d\n", _res.me_imu_wt);
    printf("%s\n", string_of_Main__version_type(_res.me_imu_ver, buf_7));
    printf("%d\n", _res.me_img_size);
    printf("%d\n", _res.me_img_act);
    printf("%d\n", _res.me_img_wt);
    printf("%s\n", string_of_Main__version_type(_res.me_img_ver, buf_11));
    printf("%d\n", _res.sl_size);
    printf("%d\n", _res.sl_act);
    printf("%d\n", _res.sl_wt);
    printf("%s\n", string_of_Main__version_type(_res.sl_ver, buf_15));
    printf("%d\n", _res.oa_size);
    printf("%d\n", _res.oa_act);
    printf("%d\n", _res.oa_wt);
    printf("%s\n", string_of_Main__version_type(_res.oa_ver, buf_19));
    printf("%d\n", _res.tl_size);
    printf("%d\n", _res.tl_act);
    printf("%d\n", _res.tl_wt);
    printf("%s\n", string_of_Main__version_type(_res.tl_ver, buf_23));
    printf("%d\n", _res.rs_size);
    printf("%d\n", _res.rs_act);
    printf("%d\n", _res.rs_wt);
    printf("%s\n", string_of_Main__version_type(_res.rs_ver, buf_27));
    printf("%d\n", _res.rb_size);
    printf("%d\n", _res.rb_act);
    printf("%d\n", _res.rb_wt);
    printf("%s\n", string_of_Main__version_type(_res.rb_ver, buf_31));
    printf("%d\n", _res.rpl_size);
    printf("%d\n", _res.rpl_act);
    printf("%d\n", _res.rpl_wt);
    printf("%s\n", string_of_Main__version_type(_res.rpl_ver, buf_35));
    printf("%d\n", _res.dt_size);
    printf("%d\n", _res.dt_act);
    printf("%d\n", _res.dt_wt);
    printf("%s\n", string_of_Main__version_type(_res.dt_ver, buf_39));
    printf("%d\n", _res.trk_size);
    printf("%d\n", _res.trk_act);
    printf("%d\n", _res.trk_wt);
    printf("%s\n", string_of_Main__version_type(_res.trk_ver, buf_43));
    printf("%d\n", _res.act_1);
    printf("%d\n", _res.err_1);
    printf("%d\n", _res.act_2);
    printf("%d\n", _res.err_2);
    printf("%d\n", _res.act_3);
    printf("%d\n", _res.err_3);
    printf("%d\n", _res.obj);
    printf("%d\n", _res.obj_task);
    printf("%d\n", _res.obj_tiles);
    printf("%d\n", _res.wcet1);
    printf("%d\n", _res.wcet2);
    printf("%d\n", _res.wcet3);
    printf("%d\n", _res.wcet4);
    printf("%d\n", _res.wcet5);
    printf("%d\n", _res.wcet6);
    printf("%d\n", _res.wcet7);
    printf("%d\n", _res.wcet8);
    printf("%d\n", _res.wcet9);
    printf("%d\n", _res.wcet10);
    printf("%d\n", _res.wcet11);
    printf("%d\n", _res.wcet1_sw);
    printf("%d\n", _res.wcet1_hw1);
    printf("%d\n", _res.wcet1_hw2);
    printf("%d\n", _res.wcet1_hw3);
    printf("%d\n", _res.wcet2_sw);
    printf("%d\n", _res.wcet2_hw1);
    printf("%d\n", _res.wcet2_hw2);
    printf("%d\n", _res.wcet2_hw3);
    printf("%d\n", _res.wcet3_sw);
    printf("%d\n", _res.wcet3_hw1);
    printf("%d\n", _res.wcet3_hw2);
    printf("%d\n", _res.wcet3_hw3);
    printf("%d\n", _res.wcet4_sw);
    printf("%d\n", _res.wcet4_hw1);
    printf("%d\n", _res.wcet4_hw2);
    printf("%d\n", _res.wcet4_hw3);
    printf("%d\n", _res.wcet5_sw);
    printf("%d\n", _res.wcet5_hw1);
    printf("%d\n", _res.wcet5_hw2);
    printf("%d\n", _res.wcet5_hw3);
    printf("%d\n", _res.wcet6_sw);
    printf("%d\n", _res.wcet6_hw1);
    printf("%d\n", _res.wcet6_hw2);
    printf("%d\n", _res.wcet6_hw3);
    printf("%d\n", _res.wcet7_sw);
    printf("%d\n", _res.wcet7_hw1);
    printf("%d\n", _res.wcet7_hw2);
    printf("%d\n", _res.wcet7_hw3);
    printf("%d\n", _res.wcet8_sw);
    printf("%d\n", _res.wcet8_hw1);
    printf("%d\n", _res.wcet8_hw2);
    printf("%d\n", _res.wcet8_hw3);
    printf("%d\n", _res.wcet9_sw);
    printf("%d\n", _res.wcet9_hw1);
    printf("%d\n", _res.wcet9_hw2);
    printf("%d\n", _res.wcet9_hw3);
    printf("%d\n", _res.wcet10_sw);
    printf("%d\n", _res.wcet10_hw1);
    printf("%d\n", _res.wcet10_hw2);
    printf("%d\n", _res.wcet10_hw3);
    printf("%d\n", _res.wcet11_sw);
    printf("%d\n", _res.wcet11_hw1);
    printf("%d\n", _res.wcet11_hw2);
    printf("%d\n", _res.wcet11_hw3);
    fflush(stdout);
  };
  return 0;
}

