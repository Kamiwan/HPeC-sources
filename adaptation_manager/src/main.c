/* --- Generated the 20/6/2018 at 0:20 --- */
/* --- heptagon compiler, version 1.04.00 (compiled mon. jun. 18 20:41:8 CET 2018) --- */
/* --- Command line: /home/gueyes/.opam/4.03.0/bin/heptc -hepts -s main -target c -target ctrln main.ept --- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

void Main__priorityInt_step(Main__priority_type val1,
                            Main__priorityInt_out* _out) {
  
  int val2_K;
  int val2_J;
  int val2_I;
  int val2_H;
  int val2_G;
  int val2_F;
  int val2_E;
  int val2_D;
  int val2_C;
  int val2_B;
  int val2_A;
  Main__priority_type ck;
  ck = val1;
  switch (ck) {
    case Main__K:
      val2_K = 11;
      _out->val2 = val2_K;
      break;
    case Main__J:
      val2_J = 10;
      _out->val2 = val2_J;
      break;
    case Main__I:
      val2_I = 9;
      _out->val2 = val2_I;
      break;
    case Main__H:
      val2_H = 8;
      _out->val2 = val2_H;
      break;
    case Main__G:
      val2_G = 7;
      _out->val2 = val2_G;
      break;
    case Main__F:
      val2_F = 6;
      _out->val2 = val2_F;
      break;
    case Main__E:
      val2_E = 5;
      _out->val2 = val2_E;
      break;
    case Main__D:
      val2_D = 4;
      _out->val2 = val2_D;
      break;
    case Main__C:
      val2_C = 3;
      _out->val2 = val2_C;
      break;
    case Main__B:
      val2_B = 2;
      _out->val2 = val2_B;
      break;
    case Main__A:
      val2_A = 1;
      _out->val2 = val2_A;
      break;
    default:
      break;
  };;
}

void Main__tile_reset(Main__tile_mem* self) {
  self->ck = Main__St_Free;
  self->pnr = false;
}

void Main__tile_step(int cr, int f, int rp, Main__tile_out* _out,
                     Main__tile_mem* self) {
  
  int v_2;
  Main__st v_1;
  int v;
  int v_4;
  Main__st v_3;
  int r_St_Fail;
  Main__st s_St_Fail;
  int r_St_Busy;
  Main__st s_St_Busy;
  int r_St_Free;
  Main__st s_St_Free;
  int nr_St_Fail;
  Main__st ns_St_Fail;
  int err_St_Fail;
  int act_St_Fail;
  int nr_St_Busy;
  Main__st ns_St_Busy;
  int err_St_Busy;
  int act_St_Busy;
  int nr_St_Free;
  Main__st ns_St_Free;
  int err_St_Free;
  int act_St_Free;
  Main__st ck_1;
  Main__st s;
  Main__st ns;
  int r;
  int nr;
  switch (self->ck) {
    case Main__St_Fail:
      if (rp) {
        s_St_Fail = Main__St_Free;
        r_St_Fail = true;
      } else {
        s_St_Fail = Main__St_Fail;
        r_St_Fail = self->pnr;
      };
      s = s_St_Fail;
      r = r_St_Fail;
      break;
    case Main__St_Busy:
      v = !(cr);
      if (v) {
        v_1 = Main__St_Free;
      } else {
        v_1 = Main__St_Busy;
      };
      if (f) {
        s_St_Busy = Main__St_Fail;
      } else {
        s_St_Busy = v_1;
      };
      if (v) {
        v_2 = true;
      } else {
        v_2 = self->pnr;
      };
      if (f) {
        r_St_Busy = true;
      } else {
        r_St_Busy = v_2;
      };
      s = s_St_Busy;
      r = r_St_Busy;
      break;
    case Main__St_Free:
      if (cr) {
        v_3 = Main__St_Busy;
      } else {
        v_3 = Main__St_Free;
      };
      if (f) {
        s_St_Free = Main__St_Fail;
      } else {
        s_St_Free = v_3;
      };
      s = s_St_Free;
      if (cr) {
        v_4 = true;
      } else {
        v_4 = self->pnr;
      };
      if (f) {
        r_St_Free = true;
      } else {
        r_St_Free = v_4;
      };
      r = r_St_Free;
      break;
    default:
      break;
  };
  ck_1 = s;
  switch (ck_1) {
    case Main__St_Fail:
      ns_St_Fail = Main__St_Fail;
      nr_St_Fail = false;
      act_St_Fail = false;
      err_St_Fail = true;
      ns = ns_St_Fail;
      nr = nr_St_Fail;
      _out->act = act_St_Fail;
      _out->err = err_St_Fail;
      break;
    case Main__St_Busy:
      ns_St_Busy = Main__St_Busy;
      nr_St_Busy = false;
      act_St_Busy = true;
      err_St_Busy = false;
      ns = ns_St_Busy;
      nr = nr_St_Busy;
      _out->act = act_St_Busy;
      _out->err = err_St_Busy;
      break;
    case Main__St_Free:
      ns_St_Free = Main__St_Free;
      ns = ns_St_Free;
      nr_St_Free = false;
      nr = nr_St_Free;
      act_St_Free = false;
      _out->act = act_St_Free;
      err_St_Free = false;
      _out->err = err_St_Free;
      break;
    default:
      break;
  };
  self->ck = ns;
  self->pnr = nr;;
}

void Main__contrast_img_reset(Main__contrast_img_mem* self) {
  self->ck = Main__St_1_Inactive;
  self->pnr = false;
}

void Main__contrast_img_step(int r, int c1, int c2, int c3, int c4, int e,
                             Main__contrast_img_out* _out,
                             Main__contrast_img_mem* self) {
  
  int v_11;
  Main__st_1 v_10;
  int v_9;
  Main__st_1 v_8;
  int v_7;
  Main__st_1 v_6;
  int v_5;
  Main__st_1 v;
  int v_32;
  Main__st_1 v_31;
  int v_30;
  Main__st_1 v_29;
  int v_28;
  Main__st_1 v_27;
  int v_26;
  Main__st_1 v_25;
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
  int v_53;
  Main__st_1 v_52;
  int v_51;
  Main__st_1 v_50;
  int v_49;
  Main__st_1 v_48;
  int v_47;
  Main__st_1 v_46;
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
  int v_74;
  Main__st_1 v_73;
  int v_72;
  Main__st_1 v_71;
  int v_70;
  Main__st_1 v_69;
  int v_68;
  Main__st_1 v_67;
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
  int v_95;
  Main__st_1 v_94;
  int v_93;
  Main__st_1 v_92;
  int v_91;
  Main__st_1 v_90;
  int v_89;
  Main__st_1 v_88;
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
  int v_115;
  Main__st_1 v_114;
  int v_113;
  Main__st_1 v_112;
  int v_111;
  Main__st_1 v_110;
  int v_109;
  Main__st_1 v_108;
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
  int r_11_St_1_Wait;
  Main__st_1 s_St_1_Wait;
  int r_11_St_1_Hw3;
  Main__st_1 s_St_1_Hw3;
  int r_11_St_1_Hw2;
  Main__st_1 s_St_1_Hw2;
  int r_11_St_1_Hw1;
  Main__st_1 s_St_1_Hw1;
  int r_11_St_1_Sw;
  Main__st_1 s_St_1_Sw;
  int r_11_St_1_Inactive;
  Main__st_1 s_St_1_Inactive;
  int nr_St_1_Wait;
  Main__st_1 ns_St_1_Wait;
  Main__version_type ver_St_1_Wait;
  int wt_St_1_Wait;
  int act_St_1_Wait;
  int size_St_1_Wait;
  int nr_St_1_Hw3;
  Main__st_1 ns_St_1_Hw3;
  Main__version_type ver_St_1_Hw3;
  int wt_St_1_Hw3;
  int act_St_1_Hw3;
  int size_St_1_Hw3;
  int nr_St_1_Hw2;
  Main__st_1 ns_St_1_Hw2;
  Main__version_type ver_St_1_Hw2;
  int wt_St_1_Hw2;
  int act_St_1_Hw2;
  int size_St_1_Hw2;
  int nr_St_1_Hw1;
  Main__st_1 ns_St_1_Hw1;
  Main__version_type ver_St_1_Hw1;
  int wt_St_1_Hw1;
  int act_St_1_Hw1;
  int size_St_1_Hw1;
  int nr_St_1_Sw;
  Main__st_1 ns_St_1_Sw;
  Main__version_type ver_St_1_Sw;
  int wt_St_1_Sw;
  int act_St_1_Sw;
  int size_St_1_Sw;
  int nr_St_1_Inactive;
  Main__st_1 ns_St_1_Inactive;
  Main__version_type ver_St_1_Inactive;
  int wt_St_1_Inactive;
  int act_St_1_Inactive;
  int size_St_1_Inactive;
  Main__st_1 ck_2;
  Main__st_1 s;
  Main__st_1 ns;
  int r_11;
  int nr;
  switch (self->ck) {
    case Main__St_1_Wait:
      if (c4) {
        v = Main__St_1_Sw;
      } else {
        v = Main__St_1_Wait;
      };
      if (c3) {
        v_6 = Main__St_1_Hw3;
      } else {
        v_6 = v;
      };
      if (c2) {
        v_8 = Main__St_1_Hw2;
      } else {
        v_8 = v_6;
      };
      if (c1) {
        v_10 = Main__St_1_Hw1;
      } else {
        v_10 = v_8;
      };
      if (e) {
        s_St_1_Wait = Main__St_1_Inactive;
      } else {
        s_St_1_Wait = v_10;
      };
      if (c4) {
        v_5 = true;
      } else {
        v_5 = self->pnr;
      };
      if (c3) {
        v_7 = true;
      } else {
        v_7 = v_5;
      };
      if (c2) {
        v_9 = true;
      } else {
        v_9 = v_7;
      };
      if (c1) {
        v_11 = true;
      } else {
        v_11 = v_9;
      };
      if (e) {
        r_11_St_1_Wait = true;
      } else {
        r_11_St_1_Wait = v_11;
      };
      s = s_St_1_Wait;
      r_11 = r_11_St_1_Wait;
      break;
    case Main__St_1_Hw3:
      v_12 = !(c3);
      v_13 = (v_12&&c1);
      v_14 = !(c3);
      v_15 = (v_14&&c2);
      v_16 = !(c3);
      v_17 = (v_16&&c4);
      v_18 = !(c3);
      v_19 = !(c2);
      v_20 = (v_18&&v_19);
      v_21 = !(c1);
      v_22 = (v_20&&v_21);
      v_23 = !(c4);
      v_24 = (v_22&&v_23);
      if (v_24) {
        v_25 = Main__St_1_Wait;
      } else {
        v_25 = Main__St_1_Hw3;
      };
      if (v_17) {
        v_27 = Main__St_1_Sw;
      } else {
        v_27 = v_25;
      };
      if (v_15) {
        v_29 = Main__St_1_Hw2;
      } else {
        v_29 = v_27;
      };
      if (v_13) {
        v_31 = Main__St_1_Hw1;
      } else {
        v_31 = v_29;
      };
      if (e) {
        s_St_1_Hw3 = Main__St_1_Inactive;
      } else {
        s_St_1_Hw3 = v_31;
      };
      if (v_24) {
        v_26 = true;
      } else {
        v_26 = self->pnr;
      };
      if (v_17) {
        v_28 = true;
      } else {
        v_28 = v_26;
      };
      if (v_15) {
        v_30 = true;
      } else {
        v_30 = v_28;
      };
      if (v_13) {
        v_32 = true;
      } else {
        v_32 = v_30;
      };
      if (e) {
        r_11_St_1_Hw3 = true;
      } else {
        r_11_St_1_Hw3 = v_32;
      };
      s = s_St_1_Hw3;
      r_11 = r_11_St_1_Hw3;
      break;
    case Main__St_1_Hw2:
      v_33 = !(c2);
      v_34 = (v_33&&c1);
      v_35 = !(c2);
      v_36 = (v_35&&c3);
      v_37 = !(c2);
      v_38 = (v_37&&c4);
      v_39 = !(c3);
      v_40 = !(c2);
      v_41 = (v_39&&v_40);
      v_42 = !(c1);
      v_43 = (v_41&&v_42);
      v_44 = !(c4);
      v_45 = (v_43&&v_44);
      if (v_45) {
        v_46 = Main__St_1_Wait;
      } else {
        v_46 = Main__St_1_Hw2;
      };
      if (v_38) {
        v_48 = Main__St_1_Sw;
      } else {
        v_48 = v_46;
      };
      if (v_36) {
        v_50 = Main__St_1_Hw3;
      } else {
        v_50 = v_48;
      };
      if (v_34) {
        v_52 = Main__St_1_Hw1;
      } else {
        v_52 = v_50;
      };
      if (e) {
        s_St_1_Hw2 = Main__St_1_Inactive;
      } else {
        s_St_1_Hw2 = v_52;
      };
      if (v_45) {
        v_47 = true;
      } else {
        v_47 = self->pnr;
      };
      if (v_38) {
        v_49 = true;
      } else {
        v_49 = v_47;
      };
      if (v_36) {
        v_51 = true;
      } else {
        v_51 = v_49;
      };
      if (v_34) {
        v_53 = true;
      } else {
        v_53 = v_51;
      };
      if (e) {
        r_11_St_1_Hw2 = true;
      } else {
        r_11_St_1_Hw2 = v_53;
      };
      s = s_St_1_Hw2;
      r_11 = r_11_St_1_Hw2;
      break;
    case Main__St_1_Hw1:
      v_54 = !(c1);
      v_55 = (v_54&&c2);
      v_56 = !(c1);
      v_57 = (v_56&&c3);
      v_58 = !(c1);
      v_59 = (v_58&&c4);
      v_60 = !(c3);
      v_61 = !(c2);
      v_62 = (v_60&&v_61);
      v_63 = !(c1);
      v_64 = (v_62&&v_63);
      v_65 = !(c4);
      v_66 = (v_64&&v_65);
      if (v_66) {
        v_67 = Main__St_1_Wait;
      } else {
        v_67 = Main__St_1_Hw1;
      };
      if (v_59) {
        v_69 = Main__St_1_Sw;
      } else {
        v_69 = v_67;
      };
      if (v_57) {
        v_71 = Main__St_1_Hw3;
      } else {
        v_71 = v_69;
      };
      if (v_55) {
        v_73 = Main__St_1_Hw2;
      } else {
        v_73 = v_71;
      };
      if (e) {
        s_St_1_Hw1 = Main__St_1_Inactive;
      } else {
        s_St_1_Hw1 = v_73;
      };
      if (v_66) {
        v_68 = true;
      } else {
        v_68 = self->pnr;
      };
      if (v_59) {
        v_70 = true;
      } else {
        v_70 = v_68;
      };
      if (v_57) {
        v_72 = true;
      } else {
        v_72 = v_70;
      };
      if (v_55) {
        v_74 = true;
      } else {
        v_74 = v_72;
      };
      if (e) {
        r_11_St_1_Hw1 = true;
      } else {
        r_11_St_1_Hw1 = v_74;
      };
      s = s_St_1_Hw1;
      r_11 = r_11_St_1_Hw1;
      break;
    case Main__St_1_Sw:
      v_75 = !(c4);
      v_76 = (v_75&&c1);
      v_77 = !(c4);
      v_78 = (v_77&&c2);
      v_79 = !(c4);
      v_80 = (v_79&&c3);
      v_81 = !(c3);
      v_82 = !(c2);
      v_83 = (v_81&&v_82);
      v_84 = !(c1);
      v_85 = (v_83&&v_84);
      v_86 = !(c4);
      v_87 = (v_85&&v_86);
      if (v_87) {
        v_88 = Main__St_1_Wait;
      } else {
        v_88 = Main__St_1_Sw;
      };
      if (v_80) {
        v_90 = Main__St_1_Hw3;
      } else {
        v_90 = v_88;
      };
      if (v_78) {
        v_92 = Main__St_1_Hw2;
      } else {
        v_92 = v_90;
      };
      if (v_76) {
        v_94 = Main__St_1_Hw1;
      } else {
        v_94 = v_92;
      };
      if (e) {
        s_St_1_Sw = Main__St_1_Inactive;
      } else {
        s_St_1_Sw = v_94;
      };
      if (v_87) {
        v_89 = true;
      } else {
        v_89 = self->pnr;
      };
      if (v_80) {
        v_91 = true;
      } else {
        v_91 = v_89;
      };
      if (v_78) {
        v_93 = true;
      } else {
        v_93 = v_91;
      };
      if (v_76) {
        v_95 = true;
      } else {
        v_95 = v_93;
      };
      if (e) {
        r_11_St_1_Sw = true;
      } else {
        r_11_St_1_Sw = v_95;
      };
      s = s_St_1_Sw;
      r_11 = r_11_St_1_Sw;
      break;
    case Main__St_1_Inactive:
      v_96 = (r&&c1);
      v_97 = (r&&c2);
      v_98 = (r&&c3);
      v_99 = (r&&c4);
      v_100 = !(c3);
      v_101 = (r&&v_100);
      v_102 = !(c2);
      v_103 = (v_101&&v_102);
      v_104 = !(c1);
      v_105 = (v_103&&v_104);
      v_106 = !(c4);
      v_107 = (v_105&&v_106);
      if (v_107) {
        v_108 = Main__St_1_Wait;
      } else {
        v_108 = Main__St_1_Inactive;
      };
      if (v_99) {
        v_110 = Main__St_1_Sw;
      } else {
        v_110 = v_108;
      };
      if (v_98) {
        v_112 = Main__St_1_Hw3;
      } else {
        v_112 = v_110;
      };
      if (v_97) {
        v_114 = Main__St_1_Hw2;
      } else {
        v_114 = v_112;
      };
      if (v_96) {
        s_St_1_Inactive = Main__St_1_Hw1;
      } else {
        s_St_1_Inactive = v_114;
      };
      s = s_St_1_Inactive;
      if (v_107) {
        v_109 = true;
      } else {
        v_109 = self->pnr;
      };
      if (v_99) {
        v_111 = true;
      } else {
        v_111 = v_109;
      };
      if (v_98) {
        v_113 = true;
      } else {
        v_113 = v_111;
      };
      if (v_97) {
        v_115 = true;
      } else {
        v_115 = v_113;
      };
      if (v_96) {
        r_11_St_1_Inactive = true;
      } else {
        r_11_St_1_Inactive = v_115;
      };
      r_11 = r_11_St_1_Inactive;
      break;
    default:
      break;
  };
  ck_2 = s;
  switch (ck_2) {
    case Main__St_1_Wait:
      ns_St_1_Wait = Main__St_1_Wait;
      nr_St_1_Wait = false;
      size_St_1_Wait = 0;
      act_St_1_Wait = false;
      wt_St_1_Wait = true;
      ver_St_1_Wait = Main__N;
      ns = ns_St_1_Wait;
      nr = nr_St_1_Wait;
      _out->size = size_St_1_Wait;
      _out->act = act_St_1_Wait;
      _out->wt = wt_St_1_Wait;
      _out->ver = ver_St_1_Wait;
      break;
    case Main__St_1_Hw3:
      ns_St_1_Hw3 = Main__St_1_Hw3;
      nr_St_1_Hw3 = false;
      size_St_1_Hw3 = 1;
      act_St_1_Hw3 = true;
      wt_St_1_Hw3 = false;
      ver_St_1_Hw3 = Main__H3;
      ns = ns_St_1_Hw3;
      nr = nr_St_1_Hw3;
      _out->size = size_St_1_Hw3;
      _out->act = act_St_1_Hw3;
      _out->wt = wt_St_1_Hw3;
      _out->ver = ver_St_1_Hw3;
      break;
    case Main__St_1_Hw2:
      ns_St_1_Hw2 = Main__St_1_Hw2;
      nr_St_1_Hw2 = false;
      size_St_1_Hw2 = 1;
      act_St_1_Hw2 = true;
      wt_St_1_Hw2 = false;
      ver_St_1_Hw2 = Main__H2;
      ns = ns_St_1_Hw2;
      nr = nr_St_1_Hw2;
      _out->size = size_St_1_Hw2;
      _out->act = act_St_1_Hw2;
      _out->wt = wt_St_1_Hw2;
      _out->ver = ver_St_1_Hw2;
      break;
    case Main__St_1_Hw1:
      ns_St_1_Hw1 = Main__St_1_Hw1;
      nr_St_1_Hw1 = false;
      size_St_1_Hw1 = 1;
      act_St_1_Hw1 = true;
      wt_St_1_Hw1 = false;
      ver_St_1_Hw1 = Main__H1;
      ns = ns_St_1_Hw1;
      nr = nr_St_1_Hw1;
      _out->size = size_St_1_Hw1;
      _out->act = act_St_1_Hw1;
      _out->wt = wt_St_1_Hw1;
      _out->ver = ver_St_1_Hw1;
      break;
    case Main__St_1_Sw:
      ns_St_1_Sw = Main__St_1_Sw;
      nr_St_1_Sw = false;
      size_St_1_Sw = 0;
      act_St_1_Sw = true;
      wt_St_1_Sw = false;
      ver_St_1_Sw = Main__S;
      ns = ns_St_1_Sw;
      nr = nr_St_1_Sw;
      _out->size = size_St_1_Sw;
      _out->act = act_St_1_Sw;
      _out->wt = wt_St_1_Sw;
      _out->ver = ver_St_1_Sw;
      break;
    case Main__St_1_Inactive:
      ns_St_1_Inactive = Main__St_1_Inactive;
      ns = ns_St_1_Inactive;
      nr_St_1_Inactive = false;
      nr = nr_St_1_Inactive;
      size_St_1_Inactive = 0;
      _out->size = size_St_1_Inactive;
      act_St_1_Inactive = false;
      _out->act = act_St_1_Inactive;
      wt_St_1_Inactive = false;
      _out->wt = wt_St_1_Inactive;
      ver_St_1_Inactive = Main__N;
      _out->ver = ver_St_1_Inactive;
      break;
    default:
      break;
  };
  self->ck = ns;
  self->pnr = nr;;
}

void Main__motion_estim_imu_reset(Main__motion_estim_imu_mem* self) {
  self->ck = Main__St_2_Inactive;
  self->pnr = false;
}

void Main__motion_estim_imu_step(int r, int c, int e,
                                 Main__motion_estim_imu_out* _out,
                                 Main__motion_estim_imu_mem* self) {
  
  int v_116;
  Main__st_2 v;
  int v_119;
  Main__st_2 v_118;
  int v_117;
  int v_124;
  Main__st_2 v_123;
  int v_122;
  int v_121;
  int v_120;
  int r_12_St_2_Wait;
  Main__st_2 s_St_2_Wait;
  int r_12_St_2_Active;
  Main__st_2 s_St_2_Active;
  int r_12_St_2_Inactive;
  Main__st_2 s_St_2_Inactive;
  int nr_St_2_Wait;
  Main__st_2 ns_St_2_Wait;
  Main__version_type ver_St_2_Wait;
  int wt_St_2_Wait;
  int act_St_2_Wait;
  int size_St_2_Wait;
  int nr_St_2_Active;
  Main__st_2 ns_St_2_Active;
  Main__version_type ver_St_2_Active;
  int wt_St_2_Active;
  int act_St_2_Active;
  int size_St_2_Active;
  int nr_St_2_Inactive;
  Main__st_2 ns_St_2_Inactive;
  Main__version_type ver_St_2_Inactive;
  int wt_St_2_Inactive;
  int act_St_2_Inactive;
  int size_St_2_Inactive;
  Main__st_2 ck_3;
  Main__st_2 s;
  Main__st_2 ns;
  int r_12;
  int nr;
  switch (self->ck) {
    case Main__St_2_Wait:
      if (c) {
        v = Main__St_2_Active;
      } else {
        v = Main__St_2_Wait;
      };
      if (e) {
        s_St_2_Wait = Main__St_2_Inactive;
      } else {
        s_St_2_Wait = v;
      };
      if (c) {
        v_116 = true;
      } else {
        v_116 = self->pnr;
      };
      if (e) {
        r_12_St_2_Wait = true;
      } else {
        r_12_St_2_Wait = v_116;
      };
      s = s_St_2_Wait;
      r_12 = r_12_St_2_Wait;
      break;
    case Main__St_2_Active:
      v_117 = !(c);
      if (v_117) {
        v_118 = Main__St_2_Wait;
      } else {
        v_118 = Main__St_2_Active;
      };
      if (e) {
        s_St_2_Active = Main__St_2_Inactive;
      } else {
        s_St_2_Active = v_118;
      };
      if (v_117) {
        v_119 = true;
      } else {
        v_119 = self->pnr;
      };
      if (e) {
        r_12_St_2_Active = true;
      } else {
        r_12_St_2_Active = v_119;
      };
      s = s_St_2_Active;
      r_12 = r_12_St_2_Active;
      break;
    case Main__St_2_Inactive:
      v_120 = (r&&c);
      v_121 = !(c);
      v_122 = (r&&v_121);
      if (v_122) {
        v_123 = Main__St_2_Wait;
      } else {
        v_123 = Main__St_2_Inactive;
      };
      if (v_120) {
        s_St_2_Inactive = Main__St_2_Active;
      } else {
        s_St_2_Inactive = v_123;
      };
      s = s_St_2_Inactive;
      if (v_122) {
        v_124 = true;
      } else {
        v_124 = self->pnr;
      };
      if (v_120) {
        r_12_St_2_Inactive = true;
      } else {
        r_12_St_2_Inactive = v_124;
      };
      r_12 = r_12_St_2_Inactive;
      break;
    default:
      break;
  };
  ck_3 = s;
  switch (ck_3) {
    case Main__St_2_Wait:
      ns_St_2_Wait = Main__St_2_Wait;
      nr_St_2_Wait = false;
      size_St_2_Wait = 0;
      act_St_2_Wait = false;
      wt_St_2_Wait = true;
      ver_St_2_Wait = Main__N;
      ns = ns_St_2_Wait;
      nr = nr_St_2_Wait;
      _out->size = size_St_2_Wait;
      _out->act = act_St_2_Wait;
      _out->wt = wt_St_2_Wait;
      _out->ver = ver_St_2_Wait;
      break;
    case Main__St_2_Active:
      ns_St_2_Active = Main__St_2_Active;
      nr_St_2_Active = false;
      size_St_2_Active = 0;
      act_St_2_Active = true;
      wt_St_2_Active = false;
      ver_St_2_Active = Main__S;
      ns = ns_St_2_Active;
      nr = nr_St_2_Active;
      _out->size = size_St_2_Active;
      _out->act = act_St_2_Active;
      _out->wt = wt_St_2_Active;
      _out->ver = ver_St_2_Active;
      break;
    case Main__St_2_Inactive:
      ns_St_2_Inactive = Main__St_2_Inactive;
      ns = ns_St_2_Inactive;
      nr_St_2_Inactive = false;
      nr = nr_St_2_Inactive;
      size_St_2_Inactive = 0;
      _out->size = size_St_2_Inactive;
      act_St_2_Inactive = false;
      _out->act = act_St_2_Inactive;
      wt_St_2_Inactive = false;
      _out->wt = wt_St_2_Inactive;
      ver_St_2_Inactive = Main__N;
      _out->ver = ver_St_2_Inactive;
      break;
    default:
      break;
  };
  self->ck = ns;
  self->pnr = nr;;
}

void Main__motion_estim_img_reset(Main__motion_estim_img_mem* self) {
  self->ck = Main__St_3_Inactive;
  self->pnr = false;
}

void Main__motion_estim_img_step(int r, int c1, int c2, int c3, int c4,
                                 int e, Main__motion_estim_img_out* _out,
                                 Main__motion_estim_img_mem* self) {
  
  int v_131;
  Main__st_3 v_130;
  int v_129;
  Main__st_3 v_128;
  int v_127;
  Main__st_3 v_126;
  int v_125;
  Main__st_3 v;
  int v_152;
  Main__st_3 v_151;
  int v_150;
  Main__st_3 v_149;
  int v_148;
  Main__st_3 v_147;
  int v_146;
  Main__st_3 v_145;
  int v_144;
  int v_143;
  int v_142;
  int v_141;
  int v_140;
  int v_139;
  int v_138;
  int v_137;
  int v_136;
  int v_135;
  int v_134;
  int v_133;
  int v_132;
  int v_173;
  Main__st_3 v_172;
  int v_171;
  Main__st_3 v_170;
  int v_169;
  Main__st_3 v_168;
  int v_167;
  Main__st_3 v_166;
  int v_165;
  int v_164;
  int v_163;
  int v_162;
  int v_161;
  int v_160;
  int v_159;
  int v_158;
  int v_157;
  int v_156;
  int v_155;
  int v_154;
  int v_153;
  int v_194;
  Main__st_3 v_193;
  int v_192;
  Main__st_3 v_191;
  int v_190;
  Main__st_3 v_189;
  int v_188;
  Main__st_3 v_187;
  int v_186;
  int v_185;
  int v_184;
  int v_183;
  int v_182;
  int v_181;
  int v_180;
  int v_179;
  int v_178;
  int v_177;
  int v_176;
  int v_175;
  int v_174;
  int v_215;
  Main__st_3 v_214;
  int v_213;
  Main__st_3 v_212;
  int v_211;
  Main__st_3 v_210;
  int v_209;
  Main__st_3 v_208;
  int v_207;
  int v_206;
  int v_205;
  int v_204;
  int v_203;
  int v_202;
  int v_201;
  int v_200;
  int v_199;
  int v_198;
  int v_197;
  int v_196;
  int v_195;
  int v_235;
  Main__st_3 v_234;
  int v_233;
  Main__st_3 v_232;
  int v_231;
  Main__st_3 v_230;
  int v_229;
  Main__st_3 v_228;
  int v_227;
  int v_226;
  int v_225;
  int v_224;
  int v_223;
  int v_222;
  int v_221;
  int v_220;
  int v_219;
  int v_218;
  int v_217;
  int v_216;
  int r_13_St_3_Wait;
  Main__st_3 s_St_3_Wait;
  int r_13_St_3_Hw3;
  Main__st_3 s_St_3_Hw3;
  int r_13_St_3_Hw2;
  Main__st_3 s_St_3_Hw2;
  int r_13_St_3_Hw1;
  Main__st_3 s_St_3_Hw1;
  int r_13_St_3_Sw;
  Main__st_3 s_St_3_Sw;
  int r_13_St_3_Inactive;
  Main__st_3 s_St_3_Inactive;
  int nr_St_3_Wait;
  Main__st_3 ns_St_3_Wait;
  Main__version_type ver_St_3_Wait;
  int wt_St_3_Wait;
  int act_St_3_Wait;
  int size_St_3_Wait;
  int nr_St_3_Hw3;
  Main__st_3 ns_St_3_Hw3;
  Main__version_type ver_St_3_Hw3;
  int wt_St_3_Hw3;
  int act_St_3_Hw3;
  int size_St_3_Hw3;
  int nr_St_3_Hw2;
  Main__st_3 ns_St_3_Hw2;
  Main__version_type ver_St_3_Hw2;
  int wt_St_3_Hw2;
  int act_St_3_Hw2;
  int size_St_3_Hw2;
  int nr_St_3_Hw1;
  Main__st_3 ns_St_3_Hw1;
  Main__version_type ver_St_3_Hw1;
  int wt_St_3_Hw1;
  int act_St_3_Hw1;
  int size_St_3_Hw1;
  int nr_St_3_Sw;
  Main__st_3 ns_St_3_Sw;
  Main__version_type ver_St_3_Sw;
  int wt_St_3_Sw;
  int act_St_3_Sw;
  int size_St_3_Sw;
  int nr_St_3_Inactive;
  Main__st_3 ns_St_3_Inactive;
  Main__version_type ver_St_3_Inactive;
  int wt_St_3_Inactive;
  int act_St_3_Inactive;
  int size_St_3_Inactive;
  Main__st_3 ck_4;
  Main__st_3 s;
  Main__st_3 ns;
  int r_13;
  int nr;
  switch (self->ck) {
    case Main__St_3_Wait:
      if (c4) {
        v = Main__St_3_Sw;
      } else {
        v = Main__St_3_Wait;
      };
      if (c3) {
        v_126 = Main__St_3_Hw3;
      } else {
        v_126 = v;
      };
      if (c2) {
        v_128 = Main__St_3_Hw2;
      } else {
        v_128 = v_126;
      };
      if (c1) {
        v_130 = Main__St_3_Hw1;
      } else {
        v_130 = v_128;
      };
      if (e) {
        s_St_3_Wait = Main__St_3_Inactive;
      } else {
        s_St_3_Wait = v_130;
      };
      if (c4) {
        v_125 = true;
      } else {
        v_125 = self->pnr;
      };
      if (c3) {
        v_127 = true;
      } else {
        v_127 = v_125;
      };
      if (c2) {
        v_129 = true;
      } else {
        v_129 = v_127;
      };
      if (c1) {
        v_131 = true;
      } else {
        v_131 = v_129;
      };
      if (e) {
        r_13_St_3_Wait = true;
      } else {
        r_13_St_3_Wait = v_131;
      };
      s = s_St_3_Wait;
      r_13 = r_13_St_3_Wait;
      break;
    case Main__St_3_Hw3:
      v_132 = !(c3);
      v_133 = (v_132&&c1);
      v_134 = !(c3);
      v_135 = (v_134&&c2);
      v_136 = !(c3);
      v_137 = (v_136&&c4);
      v_138 = !(c3);
      v_139 = !(c2);
      v_140 = (v_138&&v_139);
      v_141 = !(c1);
      v_142 = (v_140&&v_141);
      v_143 = !(c4);
      v_144 = (v_142&&v_143);
      if (v_144) {
        v_145 = Main__St_3_Wait;
      } else {
        v_145 = Main__St_3_Hw3;
      };
      if (v_137) {
        v_147 = Main__St_3_Sw;
      } else {
        v_147 = v_145;
      };
      if (v_135) {
        v_149 = Main__St_3_Hw2;
      } else {
        v_149 = v_147;
      };
      if (v_133) {
        v_151 = Main__St_3_Hw1;
      } else {
        v_151 = v_149;
      };
      if (e) {
        s_St_3_Hw3 = Main__St_3_Inactive;
      } else {
        s_St_3_Hw3 = v_151;
      };
      if (v_144) {
        v_146 = true;
      } else {
        v_146 = self->pnr;
      };
      if (v_137) {
        v_148 = true;
      } else {
        v_148 = v_146;
      };
      if (v_135) {
        v_150 = true;
      } else {
        v_150 = v_148;
      };
      if (v_133) {
        v_152 = true;
      } else {
        v_152 = v_150;
      };
      if (e) {
        r_13_St_3_Hw3 = true;
      } else {
        r_13_St_3_Hw3 = v_152;
      };
      s = s_St_3_Hw3;
      r_13 = r_13_St_3_Hw3;
      break;
    case Main__St_3_Hw2:
      v_153 = !(c2);
      v_154 = (v_153&&c1);
      v_155 = !(c2);
      v_156 = (v_155&&c3);
      v_157 = !(c2);
      v_158 = (v_157&&c4);
      v_159 = !(c3);
      v_160 = !(c2);
      v_161 = (v_159&&v_160);
      v_162 = !(c1);
      v_163 = (v_161&&v_162);
      v_164 = !(c4);
      v_165 = (v_163&&v_164);
      if (v_165) {
        v_166 = Main__St_3_Wait;
      } else {
        v_166 = Main__St_3_Hw2;
      };
      if (v_158) {
        v_168 = Main__St_3_Sw;
      } else {
        v_168 = v_166;
      };
      if (v_156) {
        v_170 = Main__St_3_Hw3;
      } else {
        v_170 = v_168;
      };
      if (v_154) {
        v_172 = Main__St_3_Hw1;
      } else {
        v_172 = v_170;
      };
      if (e) {
        s_St_3_Hw2 = Main__St_3_Inactive;
      } else {
        s_St_3_Hw2 = v_172;
      };
      if (v_165) {
        v_167 = true;
      } else {
        v_167 = self->pnr;
      };
      if (v_158) {
        v_169 = true;
      } else {
        v_169 = v_167;
      };
      if (v_156) {
        v_171 = true;
      } else {
        v_171 = v_169;
      };
      if (v_154) {
        v_173 = true;
      } else {
        v_173 = v_171;
      };
      if (e) {
        r_13_St_3_Hw2 = true;
      } else {
        r_13_St_3_Hw2 = v_173;
      };
      s = s_St_3_Hw2;
      r_13 = r_13_St_3_Hw2;
      break;
    case Main__St_3_Hw1:
      v_174 = !(c1);
      v_175 = (v_174&&c2);
      v_176 = !(c1);
      v_177 = (v_176&&c3);
      v_178 = !(c1);
      v_179 = (v_178&&c4);
      v_180 = !(c3);
      v_181 = !(c2);
      v_182 = (v_180&&v_181);
      v_183 = !(c1);
      v_184 = (v_182&&v_183);
      v_185 = !(c4);
      v_186 = (v_184&&v_185);
      if (v_186) {
        v_187 = Main__St_3_Wait;
      } else {
        v_187 = Main__St_3_Hw1;
      };
      if (v_179) {
        v_189 = Main__St_3_Sw;
      } else {
        v_189 = v_187;
      };
      if (v_177) {
        v_191 = Main__St_3_Hw3;
      } else {
        v_191 = v_189;
      };
      if (v_175) {
        v_193 = Main__St_3_Hw2;
      } else {
        v_193 = v_191;
      };
      if (e) {
        s_St_3_Hw1 = Main__St_3_Inactive;
      } else {
        s_St_3_Hw1 = v_193;
      };
      if (v_186) {
        v_188 = true;
      } else {
        v_188 = self->pnr;
      };
      if (v_179) {
        v_190 = true;
      } else {
        v_190 = v_188;
      };
      if (v_177) {
        v_192 = true;
      } else {
        v_192 = v_190;
      };
      if (v_175) {
        v_194 = true;
      } else {
        v_194 = v_192;
      };
      if (e) {
        r_13_St_3_Hw1 = true;
      } else {
        r_13_St_3_Hw1 = v_194;
      };
      s = s_St_3_Hw1;
      r_13 = r_13_St_3_Hw1;
      break;
    case Main__St_3_Sw:
      v_195 = !(c4);
      v_196 = (v_195&&c1);
      v_197 = !(c4);
      v_198 = (v_197&&c2);
      v_199 = !(c4);
      v_200 = (v_199&&c3);
      v_201 = !(c3);
      v_202 = !(c2);
      v_203 = (v_201&&v_202);
      v_204 = !(c1);
      v_205 = (v_203&&v_204);
      v_206 = !(c4);
      v_207 = (v_205&&v_206);
      if (v_207) {
        v_208 = Main__St_3_Wait;
      } else {
        v_208 = Main__St_3_Sw;
      };
      if (v_200) {
        v_210 = Main__St_3_Hw3;
      } else {
        v_210 = v_208;
      };
      if (v_198) {
        v_212 = Main__St_3_Hw2;
      } else {
        v_212 = v_210;
      };
      if (v_196) {
        v_214 = Main__St_3_Hw1;
      } else {
        v_214 = v_212;
      };
      if (e) {
        s_St_3_Sw = Main__St_3_Inactive;
      } else {
        s_St_3_Sw = v_214;
      };
      if (v_207) {
        v_209 = true;
      } else {
        v_209 = self->pnr;
      };
      if (v_200) {
        v_211 = true;
      } else {
        v_211 = v_209;
      };
      if (v_198) {
        v_213 = true;
      } else {
        v_213 = v_211;
      };
      if (v_196) {
        v_215 = true;
      } else {
        v_215 = v_213;
      };
      if (e) {
        r_13_St_3_Sw = true;
      } else {
        r_13_St_3_Sw = v_215;
      };
      s = s_St_3_Sw;
      r_13 = r_13_St_3_Sw;
      break;
    case Main__St_3_Inactive:
      v_216 = (r&&c1);
      v_217 = (r&&c2);
      v_218 = (r&&c3);
      v_219 = (r&&c4);
      v_220 = !(c3);
      v_221 = (r&&v_220);
      v_222 = !(c2);
      v_223 = (v_221&&v_222);
      v_224 = !(c1);
      v_225 = (v_223&&v_224);
      v_226 = !(c4);
      v_227 = (v_225&&v_226);
      if (v_227) {
        v_228 = Main__St_3_Wait;
      } else {
        v_228 = Main__St_3_Inactive;
      };
      if (v_219) {
        v_230 = Main__St_3_Sw;
      } else {
        v_230 = v_228;
      };
      if (v_218) {
        v_232 = Main__St_3_Hw3;
      } else {
        v_232 = v_230;
      };
      if (v_217) {
        v_234 = Main__St_3_Hw2;
      } else {
        v_234 = v_232;
      };
      if (v_216) {
        s_St_3_Inactive = Main__St_3_Hw1;
      } else {
        s_St_3_Inactive = v_234;
      };
      s = s_St_3_Inactive;
      if (v_227) {
        v_229 = true;
      } else {
        v_229 = self->pnr;
      };
      if (v_219) {
        v_231 = true;
      } else {
        v_231 = v_229;
      };
      if (v_218) {
        v_233 = true;
      } else {
        v_233 = v_231;
      };
      if (v_217) {
        v_235 = true;
      } else {
        v_235 = v_233;
      };
      if (v_216) {
        r_13_St_3_Inactive = true;
      } else {
        r_13_St_3_Inactive = v_235;
      };
      r_13 = r_13_St_3_Inactive;
      break;
    default:
      break;
  };
  ck_4 = s;
  switch (ck_4) {
    case Main__St_3_Wait:
      ns_St_3_Wait = Main__St_3_Wait;
      nr_St_3_Wait = false;
      size_St_3_Wait = 0;
      act_St_3_Wait = false;
      wt_St_3_Wait = true;
      ver_St_3_Wait = Main__N;
      ns = ns_St_3_Wait;
      nr = nr_St_3_Wait;
      _out->size = size_St_3_Wait;
      _out->act = act_St_3_Wait;
      _out->wt = wt_St_3_Wait;
      _out->ver = ver_St_3_Wait;
      break;
    case Main__St_3_Hw3:
      ns_St_3_Hw3 = Main__St_3_Hw3;
      nr_St_3_Hw3 = false;
      size_St_3_Hw3 = 1;
      act_St_3_Hw3 = true;
      wt_St_3_Hw3 = false;
      ver_St_3_Hw3 = Main__H3;
      ns = ns_St_3_Hw3;
      nr = nr_St_3_Hw3;
      _out->size = size_St_3_Hw3;
      _out->act = act_St_3_Hw3;
      _out->wt = wt_St_3_Hw3;
      _out->ver = ver_St_3_Hw3;
      break;
    case Main__St_3_Hw2:
      ns_St_3_Hw2 = Main__St_3_Hw2;
      nr_St_3_Hw2 = false;
      size_St_3_Hw2 = 1;
      act_St_3_Hw2 = true;
      wt_St_3_Hw2 = false;
      ver_St_3_Hw2 = Main__H2;
      ns = ns_St_3_Hw2;
      nr = nr_St_3_Hw2;
      _out->size = size_St_3_Hw2;
      _out->act = act_St_3_Hw2;
      _out->wt = wt_St_3_Hw2;
      _out->ver = ver_St_3_Hw2;
      break;
    case Main__St_3_Hw1:
      ns_St_3_Hw1 = Main__St_3_Hw1;
      nr_St_3_Hw1 = false;
      size_St_3_Hw1 = 1;
      act_St_3_Hw1 = true;
      wt_St_3_Hw1 = false;
      ver_St_3_Hw1 = Main__H1;
      ns = ns_St_3_Hw1;
      nr = nr_St_3_Hw1;
      _out->size = size_St_3_Hw1;
      _out->act = act_St_3_Hw1;
      _out->wt = wt_St_3_Hw1;
      _out->ver = ver_St_3_Hw1;
      break;
    case Main__St_3_Sw:
      ns_St_3_Sw = Main__St_3_Sw;
      nr_St_3_Sw = false;
      size_St_3_Sw = 0;
      act_St_3_Sw = true;
      wt_St_3_Sw = false;
      ver_St_3_Sw = Main__S;
      ns = ns_St_3_Sw;
      nr = nr_St_3_Sw;
      _out->size = size_St_3_Sw;
      _out->act = act_St_3_Sw;
      _out->wt = wt_St_3_Sw;
      _out->ver = ver_St_3_Sw;
      break;
    case Main__St_3_Inactive:
      ns_St_3_Inactive = Main__St_3_Inactive;
      ns = ns_St_3_Inactive;
      nr_St_3_Inactive = false;
      nr = nr_St_3_Inactive;
      size_St_3_Inactive = 0;
      _out->size = size_St_3_Inactive;
      act_St_3_Inactive = false;
      _out->act = act_St_3_Inactive;
      wt_St_3_Inactive = false;
      _out->wt = wt_St_3_Inactive;
      ver_St_3_Inactive = Main__N;
      _out->ver = ver_St_3_Inactive;
      break;
    default:
      break;
  };
  self->ck = ns;
  self->pnr = nr;;
}

void Main__search_landing_reset(Main__search_landing_mem* self) {
  self->ck = Main__St_4_Inactive;
  self->pnr = false;
}

void Main__search_landing_step(int r, int c1, int c2, int c3, int c4, int e,
                               Main__search_landing_out* _out,
                               Main__search_landing_mem* self) {
  
  int v_242;
  Main__st_4 v_241;
  int v_240;
  Main__st_4 v_239;
  int v_238;
  Main__st_4 v_237;
  int v_236;
  Main__st_4 v;
  int v_263;
  Main__st_4 v_262;
  int v_261;
  Main__st_4 v_260;
  int v_259;
  Main__st_4 v_258;
  int v_257;
  Main__st_4 v_256;
  int v_255;
  int v_254;
  int v_253;
  int v_252;
  int v_251;
  int v_250;
  int v_249;
  int v_248;
  int v_247;
  int v_246;
  int v_245;
  int v_244;
  int v_243;
  int v_284;
  Main__st_4 v_283;
  int v_282;
  Main__st_4 v_281;
  int v_280;
  Main__st_4 v_279;
  int v_278;
  Main__st_4 v_277;
  int v_276;
  int v_275;
  int v_274;
  int v_273;
  int v_272;
  int v_271;
  int v_270;
  int v_269;
  int v_268;
  int v_267;
  int v_266;
  int v_265;
  int v_264;
  int v_305;
  Main__st_4 v_304;
  int v_303;
  Main__st_4 v_302;
  int v_301;
  Main__st_4 v_300;
  int v_299;
  Main__st_4 v_298;
  int v_297;
  int v_296;
  int v_295;
  int v_294;
  int v_293;
  int v_292;
  int v_291;
  int v_290;
  int v_289;
  int v_288;
  int v_287;
  int v_286;
  int v_285;
  int v_326;
  Main__st_4 v_325;
  int v_324;
  Main__st_4 v_323;
  int v_322;
  Main__st_4 v_321;
  int v_320;
  Main__st_4 v_319;
  int v_318;
  int v_317;
  int v_316;
  int v_315;
  int v_314;
  int v_313;
  int v_312;
  int v_311;
  int v_310;
  int v_309;
  int v_308;
  int v_307;
  int v_306;
  int v_346;
  Main__st_4 v_345;
  int v_344;
  Main__st_4 v_343;
  int v_342;
  Main__st_4 v_341;
  int v_340;
  Main__st_4 v_339;
  int v_338;
  int v_337;
  int v_336;
  int v_335;
  int v_334;
  int v_333;
  int v_332;
  int v_331;
  int v_330;
  int v_329;
  int v_328;
  int v_327;
  int r_14_St_4_Wait;
  Main__st_4 s_St_4_Wait;
  int r_14_St_4_Hw3;
  Main__st_4 s_St_4_Hw3;
  int r_14_St_4_Hw2;
  Main__st_4 s_St_4_Hw2;
  int r_14_St_4_Hw1;
  Main__st_4 s_St_4_Hw1;
  int r_14_St_4_Sw;
  Main__st_4 s_St_4_Sw;
  int r_14_St_4_Inactive;
  Main__st_4 s_St_4_Inactive;
  int nr_St_4_Wait;
  Main__st_4 ns_St_4_Wait;
  Main__version_type ver_St_4_Wait;
  int wt_St_4_Wait;
  int act_St_4_Wait;
  int size_St_4_Wait;
  int nr_St_4_Hw3;
  Main__st_4 ns_St_4_Hw3;
  Main__version_type ver_St_4_Hw3;
  int wt_St_4_Hw3;
  int act_St_4_Hw3;
  int size_St_4_Hw3;
  int nr_St_4_Hw2;
  Main__st_4 ns_St_4_Hw2;
  Main__version_type ver_St_4_Hw2;
  int wt_St_4_Hw2;
  int act_St_4_Hw2;
  int size_St_4_Hw2;
  int nr_St_4_Hw1;
  Main__st_4 ns_St_4_Hw1;
  Main__version_type ver_St_4_Hw1;
  int wt_St_4_Hw1;
  int act_St_4_Hw1;
  int size_St_4_Hw1;
  int nr_St_4_Sw;
  Main__st_4 ns_St_4_Sw;
  Main__version_type ver_St_4_Sw;
  int wt_St_4_Sw;
  int act_St_4_Sw;
  int size_St_4_Sw;
  int nr_St_4_Inactive;
  Main__st_4 ns_St_4_Inactive;
  Main__version_type ver_St_4_Inactive;
  int wt_St_4_Inactive;
  int act_St_4_Inactive;
  int size_St_4_Inactive;
  Main__st_4 ck_5;
  Main__st_4 s;
  Main__st_4 ns;
  int r_14;
  int nr;
  switch (self->ck) {
    case Main__St_4_Wait:
      if (c4) {
        v = Main__St_4_Sw;
      } else {
        v = Main__St_4_Wait;
      };
      if (c3) {
        v_237 = Main__St_4_Hw3;
      } else {
        v_237 = v;
      };
      if (c2) {
        v_239 = Main__St_4_Hw2;
      } else {
        v_239 = v_237;
      };
      if (c1) {
        v_241 = Main__St_4_Hw1;
      } else {
        v_241 = v_239;
      };
      if (e) {
        s_St_4_Wait = Main__St_4_Inactive;
      } else {
        s_St_4_Wait = v_241;
      };
      if (c4) {
        v_236 = true;
      } else {
        v_236 = self->pnr;
      };
      if (c3) {
        v_238 = true;
      } else {
        v_238 = v_236;
      };
      if (c2) {
        v_240 = true;
      } else {
        v_240 = v_238;
      };
      if (c1) {
        v_242 = true;
      } else {
        v_242 = v_240;
      };
      if (e) {
        r_14_St_4_Wait = true;
      } else {
        r_14_St_4_Wait = v_242;
      };
      s = s_St_4_Wait;
      r_14 = r_14_St_4_Wait;
      break;
    case Main__St_4_Hw3:
      v_243 = !(c3);
      v_244 = (v_243&&c1);
      v_245 = !(c3);
      v_246 = (v_245&&c2);
      v_247 = !(c3);
      v_248 = (v_247&&c4);
      v_249 = !(c3);
      v_250 = !(c2);
      v_251 = (v_249&&v_250);
      v_252 = !(c1);
      v_253 = (v_251&&v_252);
      v_254 = !(c4);
      v_255 = (v_253&&v_254);
      if (v_255) {
        v_256 = Main__St_4_Wait;
      } else {
        v_256 = Main__St_4_Hw3;
      };
      if (v_248) {
        v_258 = Main__St_4_Sw;
      } else {
        v_258 = v_256;
      };
      if (v_246) {
        v_260 = Main__St_4_Hw2;
      } else {
        v_260 = v_258;
      };
      if (v_244) {
        v_262 = Main__St_4_Hw1;
      } else {
        v_262 = v_260;
      };
      if (e) {
        s_St_4_Hw3 = Main__St_4_Inactive;
      } else {
        s_St_4_Hw3 = v_262;
      };
      if (v_255) {
        v_257 = true;
      } else {
        v_257 = self->pnr;
      };
      if (v_248) {
        v_259 = true;
      } else {
        v_259 = v_257;
      };
      if (v_246) {
        v_261 = true;
      } else {
        v_261 = v_259;
      };
      if (v_244) {
        v_263 = true;
      } else {
        v_263 = v_261;
      };
      if (e) {
        r_14_St_4_Hw3 = true;
      } else {
        r_14_St_4_Hw3 = v_263;
      };
      s = s_St_4_Hw3;
      r_14 = r_14_St_4_Hw3;
      break;
    case Main__St_4_Hw2:
      v_264 = !(c2);
      v_265 = (v_264&&c1);
      v_266 = !(c2);
      v_267 = (v_266&&c3);
      v_268 = !(c2);
      v_269 = (v_268&&c4);
      v_270 = !(c3);
      v_271 = !(c2);
      v_272 = (v_270&&v_271);
      v_273 = !(c1);
      v_274 = (v_272&&v_273);
      v_275 = !(c4);
      v_276 = (v_274&&v_275);
      if (v_276) {
        v_277 = Main__St_4_Wait;
      } else {
        v_277 = Main__St_4_Hw2;
      };
      if (v_269) {
        v_279 = Main__St_4_Sw;
      } else {
        v_279 = v_277;
      };
      if (v_267) {
        v_281 = Main__St_4_Hw3;
      } else {
        v_281 = v_279;
      };
      if (v_265) {
        v_283 = Main__St_4_Hw1;
      } else {
        v_283 = v_281;
      };
      if (e) {
        s_St_4_Hw2 = Main__St_4_Inactive;
      } else {
        s_St_4_Hw2 = v_283;
      };
      if (v_276) {
        v_278 = true;
      } else {
        v_278 = self->pnr;
      };
      if (v_269) {
        v_280 = true;
      } else {
        v_280 = v_278;
      };
      if (v_267) {
        v_282 = true;
      } else {
        v_282 = v_280;
      };
      if (v_265) {
        v_284 = true;
      } else {
        v_284 = v_282;
      };
      if (e) {
        r_14_St_4_Hw2 = true;
      } else {
        r_14_St_4_Hw2 = v_284;
      };
      s = s_St_4_Hw2;
      r_14 = r_14_St_4_Hw2;
      break;
    case Main__St_4_Hw1:
      v_285 = !(c1);
      v_286 = (v_285&&c2);
      v_287 = !(c1);
      v_288 = (v_287&&c3);
      v_289 = !(c1);
      v_290 = (v_289&&c4);
      v_291 = !(c3);
      v_292 = !(c2);
      v_293 = (v_291&&v_292);
      v_294 = !(c1);
      v_295 = (v_293&&v_294);
      v_296 = !(c4);
      v_297 = (v_295&&v_296);
      if (v_297) {
        v_298 = Main__St_4_Wait;
      } else {
        v_298 = Main__St_4_Hw1;
      };
      if (v_290) {
        v_300 = Main__St_4_Sw;
      } else {
        v_300 = v_298;
      };
      if (v_288) {
        v_302 = Main__St_4_Hw3;
      } else {
        v_302 = v_300;
      };
      if (v_286) {
        v_304 = Main__St_4_Hw2;
      } else {
        v_304 = v_302;
      };
      if (e) {
        s_St_4_Hw1 = Main__St_4_Inactive;
      } else {
        s_St_4_Hw1 = v_304;
      };
      if (v_297) {
        v_299 = true;
      } else {
        v_299 = self->pnr;
      };
      if (v_290) {
        v_301 = true;
      } else {
        v_301 = v_299;
      };
      if (v_288) {
        v_303 = true;
      } else {
        v_303 = v_301;
      };
      if (v_286) {
        v_305 = true;
      } else {
        v_305 = v_303;
      };
      if (e) {
        r_14_St_4_Hw1 = true;
      } else {
        r_14_St_4_Hw1 = v_305;
      };
      s = s_St_4_Hw1;
      r_14 = r_14_St_4_Hw1;
      break;
    case Main__St_4_Sw:
      v_306 = !(c4);
      v_307 = (v_306&&c1);
      v_308 = !(c4);
      v_309 = (v_308&&c2);
      v_310 = !(c4);
      v_311 = (v_310&&c3);
      v_312 = !(c3);
      v_313 = !(c2);
      v_314 = (v_312&&v_313);
      v_315 = !(c1);
      v_316 = (v_314&&v_315);
      v_317 = !(c4);
      v_318 = (v_316&&v_317);
      if (v_318) {
        v_319 = Main__St_4_Wait;
      } else {
        v_319 = Main__St_4_Sw;
      };
      if (v_311) {
        v_321 = Main__St_4_Hw3;
      } else {
        v_321 = v_319;
      };
      if (v_309) {
        v_323 = Main__St_4_Hw2;
      } else {
        v_323 = v_321;
      };
      if (v_307) {
        v_325 = Main__St_4_Hw1;
      } else {
        v_325 = v_323;
      };
      if (e) {
        s_St_4_Sw = Main__St_4_Inactive;
      } else {
        s_St_4_Sw = v_325;
      };
      if (v_318) {
        v_320 = true;
      } else {
        v_320 = self->pnr;
      };
      if (v_311) {
        v_322 = true;
      } else {
        v_322 = v_320;
      };
      if (v_309) {
        v_324 = true;
      } else {
        v_324 = v_322;
      };
      if (v_307) {
        v_326 = true;
      } else {
        v_326 = v_324;
      };
      if (e) {
        r_14_St_4_Sw = true;
      } else {
        r_14_St_4_Sw = v_326;
      };
      s = s_St_4_Sw;
      r_14 = r_14_St_4_Sw;
      break;
    case Main__St_4_Inactive:
      v_327 = (r&&c1);
      v_328 = (r&&c2);
      v_329 = (r&&c3);
      v_330 = (r&&c4);
      v_331 = !(c3);
      v_332 = (r&&v_331);
      v_333 = !(c2);
      v_334 = (v_332&&v_333);
      v_335 = !(c1);
      v_336 = (v_334&&v_335);
      v_337 = !(c4);
      v_338 = (v_336&&v_337);
      if (v_338) {
        v_339 = Main__St_4_Wait;
      } else {
        v_339 = Main__St_4_Inactive;
      };
      if (v_330) {
        v_341 = Main__St_4_Sw;
      } else {
        v_341 = v_339;
      };
      if (v_329) {
        v_343 = Main__St_4_Hw3;
      } else {
        v_343 = v_341;
      };
      if (v_328) {
        v_345 = Main__St_4_Hw2;
      } else {
        v_345 = v_343;
      };
      if (v_327) {
        s_St_4_Inactive = Main__St_4_Hw1;
      } else {
        s_St_4_Inactive = v_345;
      };
      s = s_St_4_Inactive;
      if (v_338) {
        v_340 = true;
      } else {
        v_340 = self->pnr;
      };
      if (v_330) {
        v_342 = true;
      } else {
        v_342 = v_340;
      };
      if (v_329) {
        v_344 = true;
      } else {
        v_344 = v_342;
      };
      if (v_328) {
        v_346 = true;
      } else {
        v_346 = v_344;
      };
      if (v_327) {
        r_14_St_4_Inactive = true;
      } else {
        r_14_St_4_Inactive = v_346;
      };
      r_14 = r_14_St_4_Inactive;
      break;
    default:
      break;
  };
  ck_5 = s;
  switch (ck_5) {
    case Main__St_4_Wait:
      ns_St_4_Wait = Main__St_4_Wait;
      nr_St_4_Wait = false;
      size_St_4_Wait = 0;
      act_St_4_Wait = false;
      wt_St_4_Wait = true;
      ver_St_4_Wait = Main__N;
      ns = ns_St_4_Wait;
      nr = nr_St_4_Wait;
      _out->size = size_St_4_Wait;
      _out->act = act_St_4_Wait;
      _out->wt = wt_St_4_Wait;
      _out->ver = ver_St_4_Wait;
      break;
    case Main__St_4_Hw3:
      ns_St_4_Hw3 = Main__St_4_Hw3;
      nr_St_4_Hw3 = false;
      size_St_4_Hw3 = 1;
      act_St_4_Hw3 = true;
      wt_St_4_Hw3 = false;
      ver_St_4_Hw3 = Main__H3;
      ns = ns_St_4_Hw3;
      nr = nr_St_4_Hw3;
      _out->size = size_St_4_Hw3;
      _out->act = act_St_4_Hw3;
      _out->wt = wt_St_4_Hw3;
      _out->ver = ver_St_4_Hw3;
      break;
    case Main__St_4_Hw2:
      ns_St_4_Hw2 = Main__St_4_Hw2;
      nr_St_4_Hw2 = false;
      size_St_4_Hw2 = 1;
      act_St_4_Hw2 = true;
      wt_St_4_Hw2 = false;
      ver_St_4_Hw2 = Main__H2;
      ns = ns_St_4_Hw2;
      nr = nr_St_4_Hw2;
      _out->size = size_St_4_Hw2;
      _out->act = act_St_4_Hw2;
      _out->wt = wt_St_4_Hw2;
      _out->ver = ver_St_4_Hw2;
      break;
    case Main__St_4_Hw1:
      ns_St_4_Hw1 = Main__St_4_Hw1;
      nr_St_4_Hw1 = false;
      size_St_4_Hw1 = 1;
      act_St_4_Hw1 = true;
      wt_St_4_Hw1 = false;
      ver_St_4_Hw1 = Main__H1;
      ns = ns_St_4_Hw1;
      nr = nr_St_4_Hw1;
      _out->size = size_St_4_Hw1;
      _out->act = act_St_4_Hw1;
      _out->wt = wt_St_4_Hw1;
      _out->ver = ver_St_4_Hw1;
      break;
    case Main__St_4_Sw:
      ns_St_4_Sw = Main__St_4_Sw;
      nr_St_4_Sw = false;
      size_St_4_Sw = 0;
      act_St_4_Sw = true;
      wt_St_4_Sw = false;
      ver_St_4_Sw = Main__S;
      ns = ns_St_4_Sw;
      nr = nr_St_4_Sw;
      _out->size = size_St_4_Sw;
      _out->act = act_St_4_Sw;
      _out->wt = wt_St_4_Sw;
      _out->ver = ver_St_4_Sw;
      break;
    case Main__St_4_Inactive:
      ns_St_4_Inactive = Main__St_4_Inactive;
      ns = ns_St_4_Inactive;
      nr_St_4_Inactive = false;
      nr = nr_St_4_Inactive;
      size_St_4_Inactive = 0;
      _out->size = size_St_4_Inactive;
      act_St_4_Inactive = false;
      _out->act = act_St_4_Inactive;
      wt_St_4_Inactive = false;
      _out->wt = wt_St_4_Inactive;
      ver_St_4_Inactive = Main__N;
      _out->ver = ver_St_4_Inactive;
      break;
    default:
      break;
  };
  self->ck = ns;
  self->pnr = nr;;
}

void Main__obstacle_avoidance_reset(Main__obstacle_avoidance_mem* self) {
  self->ck = Main__St_5_Inactive;
  self->pnr = false;
}

void Main__obstacle_avoidance_step(int r, int c1, int c2, int c3, int c4,
                                   int e, Main__obstacle_avoidance_out* _out,
                                   Main__obstacle_avoidance_mem* self) {
  
  int v_353;
  Main__st_5 v_352;
  int v_351;
  Main__st_5 v_350;
  int v_349;
  Main__st_5 v_348;
  int v_347;
  Main__st_5 v;
  int v_374;
  Main__st_5 v_373;
  int v_372;
  Main__st_5 v_371;
  int v_370;
  Main__st_5 v_369;
  int v_368;
  Main__st_5 v_367;
  int v_366;
  int v_365;
  int v_364;
  int v_363;
  int v_362;
  int v_361;
  int v_360;
  int v_359;
  int v_358;
  int v_357;
  int v_356;
  int v_355;
  int v_354;
  int v_395;
  Main__st_5 v_394;
  int v_393;
  Main__st_5 v_392;
  int v_391;
  Main__st_5 v_390;
  int v_389;
  Main__st_5 v_388;
  int v_387;
  int v_386;
  int v_385;
  int v_384;
  int v_383;
  int v_382;
  int v_381;
  int v_380;
  int v_379;
  int v_378;
  int v_377;
  int v_376;
  int v_375;
  int v_416;
  Main__st_5 v_415;
  int v_414;
  Main__st_5 v_413;
  int v_412;
  Main__st_5 v_411;
  int v_410;
  Main__st_5 v_409;
  int v_408;
  int v_407;
  int v_406;
  int v_405;
  int v_404;
  int v_403;
  int v_402;
  int v_401;
  int v_400;
  int v_399;
  int v_398;
  int v_397;
  int v_396;
  int v_437;
  Main__st_5 v_436;
  int v_435;
  Main__st_5 v_434;
  int v_433;
  Main__st_5 v_432;
  int v_431;
  Main__st_5 v_430;
  int v_429;
  int v_428;
  int v_427;
  int v_426;
  int v_425;
  int v_424;
  int v_423;
  int v_422;
  int v_421;
  int v_420;
  int v_419;
  int v_418;
  int v_417;
  int v_457;
  Main__st_5 v_456;
  int v_455;
  Main__st_5 v_454;
  int v_453;
  Main__st_5 v_452;
  int v_451;
  Main__st_5 v_450;
  int v_449;
  int v_448;
  int v_447;
  int v_446;
  int v_445;
  int v_444;
  int v_443;
  int v_442;
  int v_441;
  int v_440;
  int v_439;
  int v_438;
  int r_15_St_5_Wait;
  Main__st_5 s_St_5_Wait;
  int r_15_St_5_Hw3;
  Main__st_5 s_St_5_Hw3;
  int r_15_St_5_Hw2;
  Main__st_5 s_St_5_Hw2;
  int r_15_St_5_Hw1;
  Main__st_5 s_St_5_Hw1;
  int r_15_St_5_Sw;
  Main__st_5 s_St_5_Sw;
  int r_15_St_5_Inactive;
  Main__st_5 s_St_5_Inactive;
  int nr_St_5_Wait;
  Main__st_5 ns_St_5_Wait;
  Main__version_type ver_St_5_Wait;
  int wt_St_5_Wait;
  int act_St_5_Wait;
  int size_St_5_Wait;
  int nr_St_5_Hw3;
  Main__st_5 ns_St_5_Hw3;
  Main__version_type ver_St_5_Hw3;
  int wt_St_5_Hw3;
  int act_St_5_Hw3;
  int size_St_5_Hw3;
  int nr_St_5_Hw2;
  Main__st_5 ns_St_5_Hw2;
  Main__version_type ver_St_5_Hw2;
  int wt_St_5_Hw2;
  int act_St_5_Hw2;
  int size_St_5_Hw2;
  int nr_St_5_Hw1;
  Main__st_5 ns_St_5_Hw1;
  Main__version_type ver_St_5_Hw1;
  int wt_St_5_Hw1;
  int act_St_5_Hw1;
  int size_St_5_Hw1;
  int nr_St_5_Sw;
  Main__st_5 ns_St_5_Sw;
  Main__version_type ver_St_5_Sw;
  int wt_St_5_Sw;
  int act_St_5_Sw;
  int size_St_5_Sw;
  int nr_St_5_Inactive;
  Main__st_5 ns_St_5_Inactive;
  Main__version_type ver_St_5_Inactive;
  int wt_St_5_Inactive;
  int act_St_5_Inactive;
  int size_St_5_Inactive;
  Main__st_5 ck_6;
  Main__st_5 s;
  Main__st_5 ns;
  int r_15;
  int nr;
  switch (self->ck) {
    case Main__St_5_Wait:
      if (c4) {
        v = Main__St_5_Sw;
      } else {
        v = Main__St_5_Wait;
      };
      if (c3) {
        v_348 = Main__St_5_Hw3;
      } else {
        v_348 = v;
      };
      if (c2) {
        v_350 = Main__St_5_Hw2;
      } else {
        v_350 = v_348;
      };
      if (c1) {
        v_352 = Main__St_5_Hw1;
      } else {
        v_352 = v_350;
      };
      if (e) {
        s_St_5_Wait = Main__St_5_Inactive;
      } else {
        s_St_5_Wait = v_352;
      };
      if (c4) {
        v_347 = true;
      } else {
        v_347 = self->pnr;
      };
      if (c3) {
        v_349 = true;
      } else {
        v_349 = v_347;
      };
      if (c2) {
        v_351 = true;
      } else {
        v_351 = v_349;
      };
      if (c1) {
        v_353 = true;
      } else {
        v_353 = v_351;
      };
      if (e) {
        r_15_St_5_Wait = true;
      } else {
        r_15_St_5_Wait = v_353;
      };
      s = s_St_5_Wait;
      r_15 = r_15_St_5_Wait;
      break;
    case Main__St_5_Hw3:
      v_354 = !(c3);
      v_355 = (v_354&&c1);
      v_356 = !(c3);
      v_357 = (v_356&&c2);
      v_358 = !(c3);
      v_359 = (v_358&&c4);
      v_360 = !(c3);
      v_361 = !(c2);
      v_362 = (v_360&&v_361);
      v_363 = !(c1);
      v_364 = (v_362&&v_363);
      v_365 = !(c4);
      v_366 = (v_364&&v_365);
      if (v_366) {
        v_367 = Main__St_5_Wait;
      } else {
        v_367 = Main__St_5_Hw3;
      };
      if (v_359) {
        v_369 = Main__St_5_Sw;
      } else {
        v_369 = v_367;
      };
      if (v_357) {
        v_371 = Main__St_5_Hw2;
      } else {
        v_371 = v_369;
      };
      if (v_355) {
        v_373 = Main__St_5_Hw1;
      } else {
        v_373 = v_371;
      };
      if (e) {
        s_St_5_Hw3 = Main__St_5_Inactive;
      } else {
        s_St_5_Hw3 = v_373;
      };
      if (v_366) {
        v_368 = true;
      } else {
        v_368 = self->pnr;
      };
      if (v_359) {
        v_370 = true;
      } else {
        v_370 = v_368;
      };
      if (v_357) {
        v_372 = true;
      } else {
        v_372 = v_370;
      };
      if (v_355) {
        v_374 = true;
      } else {
        v_374 = v_372;
      };
      if (e) {
        r_15_St_5_Hw3 = true;
      } else {
        r_15_St_5_Hw3 = v_374;
      };
      s = s_St_5_Hw3;
      r_15 = r_15_St_5_Hw3;
      break;
    case Main__St_5_Hw2:
      v_375 = !(c2);
      v_376 = (v_375&&c1);
      v_377 = !(c2);
      v_378 = (v_377&&c3);
      v_379 = !(c2);
      v_380 = (v_379&&c4);
      v_381 = !(c3);
      v_382 = !(c2);
      v_383 = (v_381&&v_382);
      v_384 = !(c1);
      v_385 = (v_383&&v_384);
      v_386 = !(c4);
      v_387 = (v_385&&v_386);
      if (v_387) {
        v_388 = Main__St_5_Wait;
      } else {
        v_388 = Main__St_5_Hw2;
      };
      if (v_380) {
        v_390 = Main__St_5_Sw;
      } else {
        v_390 = v_388;
      };
      if (v_378) {
        v_392 = Main__St_5_Hw3;
      } else {
        v_392 = v_390;
      };
      if (v_376) {
        v_394 = Main__St_5_Hw1;
      } else {
        v_394 = v_392;
      };
      if (e) {
        s_St_5_Hw2 = Main__St_5_Inactive;
      } else {
        s_St_5_Hw2 = v_394;
      };
      if (v_387) {
        v_389 = true;
      } else {
        v_389 = self->pnr;
      };
      if (v_380) {
        v_391 = true;
      } else {
        v_391 = v_389;
      };
      if (v_378) {
        v_393 = true;
      } else {
        v_393 = v_391;
      };
      if (v_376) {
        v_395 = true;
      } else {
        v_395 = v_393;
      };
      if (e) {
        r_15_St_5_Hw2 = true;
      } else {
        r_15_St_5_Hw2 = v_395;
      };
      s = s_St_5_Hw2;
      r_15 = r_15_St_5_Hw2;
      break;
    case Main__St_5_Hw1:
      v_396 = !(c1);
      v_397 = (v_396&&c2);
      v_398 = !(c1);
      v_399 = (v_398&&c3);
      v_400 = !(c1);
      v_401 = (v_400&&c4);
      v_402 = !(c3);
      v_403 = !(c2);
      v_404 = (v_402&&v_403);
      v_405 = !(c1);
      v_406 = (v_404&&v_405);
      v_407 = !(c4);
      v_408 = (v_406&&v_407);
      if (v_408) {
        v_409 = Main__St_5_Wait;
      } else {
        v_409 = Main__St_5_Hw1;
      };
      if (v_401) {
        v_411 = Main__St_5_Sw;
      } else {
        v_411 = v_409;
      };
      if (v_399) {
        v_413 = Main__St_5_Hw3;
      } else {
        v_413 = v_411;
      };
      if (v_397) {
        v_415 = Main__St_5_Hw2;
      } else {
        v_415 = v_413;
      };
      if (e) {
        s_St_5_Hw1 = Main__St_5_Inactive;
      } else {
        s_St_5_Hw1 = v_415;
      };
      if (v_408) {
        v_410 = true;
      } else {
        v_410 = self->pnr;
      };
      if (v_401) {
        v_412 = true;
      } else {
        v_412 = v_410;
      };
      if (v_399) {
        v_414 = true;
      } else {
        v_414 = v_412;
      };
      if (v_397) {
        v_416 = true;
      } else {
        v_416 = v_414;
      };
      if (e) {
        r_15_St_5_Hw1 = true;
      } else {
        r_15_St_5_Hw1 = v_416;
      };
      s = s_St_5_Hw1;
      r_15 = r_15_St_5_Hw1;
      break;
    case Main__St_5_Sw:
      v_417 = !(c4);
      v_418 = (v_417&&c1);
      v_419 = !(c4);
      v_420 = (v_419&&c2);
      v_421 = !(c4);
      v_422 = (v_421&&c3);
      v_423 = !(c3);
      v_424 = !(c2);
      v_425 = (v_423&&v_424);
      v_426 = !(c1);
      v_427 = (v_425&&v_426);
      v_428 = !(c4);
      v_429 = (v_427&&v_428);
      if (v_429) {
        v_430 = Main__St_5_Wait;
      } else {
        v_430 = Main__St_5_Sw;
      };
      if (v_422) {
        v_432 = Main__St_5_Hw3;
      } else {
        v_432 = v_430;
      };
      if (v_420) {
        v_434 = Main__St_5_Hw2;
      } else {
        v_434 = v_432;
      };
      if (v_418) {
        v_436 = Main__St_5_Hw1;
      } else {
        v_436 = v_434;
      };
      if (e) {
        s_St_5_Sw = Main__St_5_Inactive;
      } else {
        s_St_5_Sw = v_436;
      };
      if (v_429) {
        v_431 = true;
      } else {
        v_431 = self->pnr;
      };
      if (v_422) {
        v_433 = true;
      } else {
        v_433 = v_431;
      };
      if (v_420) {
        v_435 = true;
      } else {
        v_435 = v_433;
      };
      if (v_418) {
        v_437 = true;
      } else {
        v_437 = v_435;
      };
      if (e) {
        r_15_St_5_Sw = true;
      } else {
        r_15_St_5_Sw = v_437;
      };
      s = s_St_5_Sw;
      r_15 = r_15_St_5_Sw;
      break;
    case Main__St_5_Inactive:
      v_438 = (r&&c1);
      v_439 = (r&&c2);
      v_440 = (r&&c3);
      v_441 = (r&&c4);
      v_442 = !(c3);
      v_443 = (r&&v_442);
      v_444 = !(c2);
      v_445 = (v_443&&v_444);
      v_446 = !(c1);
      v_447 = (v_445&&v_446);
      v_448 = !(c4);
      v_449 = (v_447&&v_448);
      if (v_449) {
        v_450 = Main__St_5_Wait;
      } else {
        v_450 = Main__St_5_Inactive;
      };
      if (v_441) {
        v_452 = Main__St_5_Sw;
      } else {
        v_452 = v_450;
      };
      if (v_440) {
        v_454 = Main__St_5_Hw3;
      } else {
        v_454 = v_452;
      };
      if (v_439) {
        v_456 = Main__St_5_Hw2;
      } else {
        v_456 = v_454;
      };
      if (v_438) {
        s_St_5_Inactive = Main__St_5_Hw1;
      } else {
        s_St_5_Inactive = v_456;
      };
      s = s_St_5_Inactive;
      if (v_449) {
        v_451 = true;
      } else {
        v_451 = self->pnr;
      };
      if (v_441) {
        v_453 = true;
      } else {
        v_453 = v_451;
      };
      if (v_440) {
        v_455 = true;
      } else {
        v_455 = v_453;
      };
      if (v_439) {
        v_457 = true;
      } else {
        v_457 = v_455;
      };
      if (v_438) {
        r_15_St_5_Inactive = true;
      } else {
        r_15_St_5_Inactive = v_457;
      };
      r_15 = r_15_St_5_Inactive;
      break;
    default:
      break;
  };
  ck_6 = s;
  switch (ck_6) {
    case Main__St_5_Wait:
      ns_St_5_Wait = Main__St_5_Wait;
      nr_St_5_Wait = false;
      size_St_5_Wait = 0;
      act_St_5_Wait = false;
      wt_St_5_Wait = true;
      ver_St_5_Wait = Main__N;
      ns = ns_St_5_Wait;
      nr = nr_St_5_Wait;
      _out->size = size_St_5_Wait;
      _out->act = act_St_5_Wait;
      _out->wt = wt_St_5_Wait;
      _out->ver = ver_St_5_Wait;
      break;
    case Main__St_5_Hw3:
      ns_St_5_Hw3 = Main__St_5_Hw3;
      nr_St_5_Hw3 = false;
      size_St_5_Hw3 = 1;
      act_St_5_Hw3 = true;
      wt_St_5_Hw3 = false;
      ver_St_5_Hw3 = Main__H3;
      ns = ns_St_5_Hw3;
      nr = nr_St_5_Hw3;
      _out->size = size_St_5_Hw3;
      _out->act = act_St_5_Hw3;
      _out->wt = wt_St_5_Hw3;
      _out->ver = ver_St_5_Hw3;
      break;
    case Main__St_5_Hw2:
      ns_St_5_Hw2 = Main__St_5_Hw2;
      nr_St_5_Hw2 = false;
      size_St_5_Hw2 = 1;
      act_St_5_Hw2 = true;
      wt_St_5_Hw2 = false;
      ver_St_5_Hw2 = Main__H2;
      ns = ns_St_5_Hw2;
      nr = nr_St_5_Hw2;
      _out->size = size_St_5_Hw2;
      _out->act = act_St_5_Hw2;
      _out->wt = wt_St_5_Hw2;
      _out->ver = ver_St_5_Hw2;
      break;
    case Main__St_5_Hw1:
      ns_St_5_Hw1 = Main__St_5_Hw1;
      nr_St_5_Hw1 = false;
      size_St_5_Hw1 = 1;
      act_St_5_Hw1 = true;
      wt_St_5_Hw1 = false;
      ver_St_5_Hw1 = Main__H1;
      ns = ns_St_5_Hw1;
      nr = nr_St_5_Hw1;
      _out->size = size_St_5_Hw1;
      _out->act = act_St_5_Hw1;
      _out->wt = wt_St_5_Hw1;
      _out->ver = ver_St_5_Hw1;
      break;
    case Main__St_5_Sw:
      ns_St_5_Sw = Main__St_5_Sw;
      nr_St_5_Sw = false;
      size_St_5_Sw = 0;
      act_St_5_Sw = true;
      wt_St_5_Sw = false;
      ver_St_5_Sw = Main__S;
      ns = ns_St_5_Sw;
      nr = nr_St_5_Sw;
      _out->size = size_St_5_Sw;
      _out->act = act_St_5_Sw;
      _out->wt = wt_St_5_Sw;
      _out->ver = ver_St_5_Sw;
      break;
    case Main__St_5_Inactive:
      ns_St_5_Inactive = Main__St_5_Inactive;
      ns = ns_St_5_Inactive;
      nr_St_5_Inactive = false;
      nr = nr_St_5_Inactive;
      size_St_5_Inactive = 0;
      _out->size = size_St_5_Inactive;
      act_St_5_Inactive = false;
      _out->act = act_St_5_Inactive;
      wt_St_5_Inactive = false;
      _out->wt = wt_St_5_Inactive;
      ver_St_5_Inactive = Main__N;
      _out->ver = ver_St_5_Inactive;
      break;
    default:
      break;
  };
  self->ck = ns;
  self->pnr = nr;;
}

void Main__t_landing_reset(Main__t_landing_mem* self) {
  self->ck = Main__St_6_Inactive;
  self->pnr = false;
}

void Main__t_landing_step(int r, int c1, int c2, int c3, int c4, int e,
                          Main__t_landing_out* _out,
                          Main__t_landing_mem* self) {
  
  int v_464;
  Main__st_6 v_463;
  int v_462;
  Main__st_6 v_461;
  int v_460;
  Main__st_6 v_459;
  int v_458;
  Main__st_6 v;
  int v_485;
  Main__st_6 v_484;
  int v_483;
  Main__st_6 v_482;
  int v_481;
  Main__st_6 v_480;
  int v_479;
  Main__st_6 v_478;
  int v_477;
  int v_476;
  int v_475;
  int v_474;
  int v_473;
  int v_472;
  int v_471;
  int v_470;
  int v_469;
  int v_468;
  int v_467;
  int v_466;
  int v_465;
  int v_506;
  Main__st_6 v_505;
  int v_504;
  Main__st_6 v_503;
  int v_502;
  Main__st_6 v_501;
  int v_500;
  Main__st_6 v_499;
  int v_498;
  int v_497;
  int v_496;
  int v_495;
  int v_494;
  int v_493;
  int v_492;
  int v_491;
  int v_490;
  int v_489;
  int v_488;
  int v_487;
  int v_486;
  int v_527;
  Main__st_6 v_526;
  int v_525;
  Main__st_6 v_524;
  int v_523;
  Main__st_6 v_522;
  int v_521;
  Main__st_6 v_520;
  int v_519;
  int v_518;
  int v_517;
  int v_516;
  int v_515;
  int v_514;
  int v_513;
  int v_512;
  int v_511;
  int v_510;
  int v_509;
  int v_508;
  int v_507;
  int v_548;
  Main__st_6 v_547;
  int v_546;
  Main__st_6 v_545;
  int v_544;
  Main__st_6 v_543;
  int v_542;
  Main__st_6 v_541;
  int v_540;
  int v_539;
  int v_538;
  int v_537;
  int v_536;
  int v_535;
  int v_534;
  int v_533;
  int v_532;
  int v_531;
  int v_530;
  int v_529;
  int v_528;
  int v_568;
  Main__st_6 v_567;
  int v_566;
  Main__st_6 v_565;
  int v_564;
  Main__st_6 v_563;
  int v_562;
  Main__st_6 v_561;
  int v_560;
  int v_559;
  int v_558;
  int v_557;
  int v_556;
  int v_555;
  int v_554;
  int v_553;
  int v_552;
  int v_551;
  int v_550;
  int v_549;
  int r_16_St_6_Wait;
  Main__st_6 s_St_6_Wait;
  int r_16_St_6_Hw3;
  Main__st_6 s_St_6_Hw3;
  int r_16_St_6_Hw2;
  Main__st_6 s_St_6_Hw2;
  int r_16_St_6_Hw1;
  Main__st_6 s_St_6_Hw1;
  int r_16_St_6_Sw;
  Main__st_6 s_St_6_Sw;
  int r_16_St_6_Inactive;
  Main__st_6 s_St_6_Inactive;
  int nr_St_6_Wait;
  Main__st_6 ns_St_6_Wait;
  Main__version_type ver_St_6_Wait;
  int wt_St_6_Wait;
  int act_St_6_Wait;
  int size_St_6_Wait;
  int nr_St_6_Hw3;
  Main__st_6 ns_St_6_Hw3;
  Main__version_type ver_St_6_Hw3;
  int wt_St_6_Hw3;
  int act_St_6_Hw3;
  int size_St_6_Hw3;
  int nr_St_6_Hw2;
  Main__st_6 ns_St_6_Hw2;
  Main__version_type ver_St_6_Hw2;
  int wt_St_6_Hw2;
  int act_St_6_Hw2;
  int size_St_6_Hw2;
  int nr_St_6_Hw1;
  Main__st_6 ns_St_6_Hw1;
  Main__version_type ver_St_6_Hw1;
  int wt_St_6_Hw1;
  int act_St_6_Hw1;
  int size_St_6_Hw1;
  int nr_St_6_Sw;
  Main__st_6 ns_St_6_Sw;
  Main__version_type ver_St_6_Sw;
  int wt_St_6_Sw;
  int act_St_6_Sw;
  int size_St_6_Sw;
  int nr_St_6_Inactive;
  Main__st_6 ns_St_6_Inactive;
  Main__version_type ver_St_6_Inactive;
  int wt_St_6_Inactive;
  int act_St_6_Inactive;
  int size_St_6_Inactive;
  Main__st_6 ck_7;
  Main__st_6 s;
  Main__st_6 ns;
  int r_16;
  int nr;
  switch (self->ck) {
    case Main__St_6_Wait:
      if (c4) {
        v = Main__St_6_Sw;
      } else {
        v = Main__St_6_Wait;
      };
      if (c3) {
        v_459 = Main__St_6_Hw3;
      } else {
        v_459 = v;
      };
      if (c2) {
        v_461 = Main__St_6_Hw2;
      } else {
        v_461 = v_459;
      };
      if (c1) {
        v_463 = Main__St_6_Hw1;
      } else {
        v_463 = v_461;
      };
      if (e) {
        s_St_6_Wait = Main__St_6_Inactive;
      } else {
        s_St_6_Wait = v_463;
      };
      if (c4) {
        v_458 = true;
      } else {
        v_458 = self->pnr;
      };
      if (c3) {
        v_460 = true;
      } else {
        v_460 = v_458;
      };
      if (c2) {
        v_462 = true;
      } else {
        v_462 = v_460;
      };
      if (c1) {
        v_464 = true;
      } else {
        v_464 = v_462;
      };
      if (e) {
        r_16_St_6_Wait = true;
      } else {
        r_16_St_6_Wait = v_464;
      };
      s = s_St_6_Wait;
      r_16 = r_16_St_6_Wait;
      break;
    case Main__St_6_Hw3:
      v_465 = !(c3);
      v_466 = (v_465&&c1);
      v_467 = !(c3);
      v_468 = (v_467&&c2);
      v_469 = !(c3);
      v_470 = (v_469&&c4);
      v_471 = !(c3);
      v_472 = !(c2);
      v_473 = (v_471&&v_472);
      v_474 = !(c1);
      v_475 = (v_473&&v_474);
      v_476 = !(c4);
      v_477 = (v_475&&v_476);
      if (v_477) {
        v_478 = Main__St_6_Wait;
      } else {
        v_478 = Main__St_6_Hw3;
      };
      if (v_470) {
        v_480 = Main__St_6_Sw;
      } else {
        v_480 = v_478;
      };
      if (v_468) {
        v_482 = Main__St_6_Hw2;
      } else {
        v_482 = v_480;
      };
      if (v_466) {
        v_484 = Main__St_6_Hw1;
      } else {
        v_484 = v_482;
      };
      if (e) {
        s_St_6_Hw3 = Main__St_6_Inactive;
      } else {
        s_St_6_Hw3 = v_484;
      };
      if (v_477) {
        v_479 = true;
      } else {
        v_479 = self->pnr;
      };
      if (v_470) {
        v_481 = true;
      } else {
        v_481 = v_479;
      };
      if (v_468) {
        v_483 = true;
      } else {
        v_483 = v_481;
      };
      if (v_466) {
        v_485 = true;
      } else {
        v_485 = v_483;
      };
      if (e) {
        r_16_St_6_Hw3 = true;
      } else {
        r_16_St_6_Hw3 = v_485;
      };
      s = s_St_6_Hw3;
      r_16 = r_16_St_6_Hw3;
      break;
    case Main__St_6_Hw2:
      v_486 = !(c2);
      v_487 = (v_486&&c1);
      v_488 = !(c2);
      v_489 = (v_488&&c3);
      v_490 = !(c2);
      v_491 = (v_490&&c4);
      v_492 = !(c3);
      v_493 = !(c2);
      v_494 = (v_492&&v_493);
      v_495 = !(c1);
      v_496 = (v_494&&v_495);
      v_497 = !(c4);
      v_498 = (v_496&&v_497);
      if (v_498) {
        v_499 = Main__St_6_Wait;
      } else {
        v_499 = Main__St_6_Hw2;
      };
      if (v_491) {
        v_501 = Main__St_6_Sw;
      } else {
        v_501 = v_499;
      };
      if (v_489) {
        v_503 = Main__St_6_Hw3;
      } else {
        v_503 = v_501;
      };
      if (v_487) {
        v_505 = Main__St_6_Hw1;
      } else {
        v_505 = v_503;
      };
      if (e) {
        s_St_6_Hw2 = Main__St_6_Inactive;
      } else {
        s_St_6_Hw2 = v_505;
      };
      if (v_498) {
        v_500 = true;
      } else {
        v_500 = self->pnr;
      };
      if (v_491) {
        v_502 = true;
      } else {
        v_502 = v_500;
      };
      if (v_489) {
        v_504 = true;
      } else {
        v_504 = v_502;
      };
      if (v_487) {
        v_506 = true;
      } else {
        v_506 = v_504;
      };
      if (e) {
        r_16_St_6_Hw2 = true;
      } else {
        r_16_St_6_Hw2 = v_506;
      };
      s = s_St_6_Hw2;
      r_16 = r_16_St_6_Hw2;
      break;
    case Main__St_6_Hw1:
      v_507 = !(c1);
      v_508 = (v_507&&c2);
      v_509 = !(c1);
      v_510 = (v_509&&c3);
      v_511 = !(c1);
      v_512 = (v_511&&c4);
      v_513 = !(c3);
      v_514 = !(c2);
      v_515 = (v_513&&v_514);
      v_516 = !(c1);
      v_517 = (v_515&&v_516);
      v_518 = !(c4);
      v_519 = (v_517&&v_518);
      if (v_519) {
        v_520 = Main__St_6_Wait;
      } else {
        v_520 = Main__St_6_Hw1;
      };
      if (v_512) {
        v_522 = Main__St_6_Sw;
      } else {
        v_522 = v_520;
      };
      if (v_510) {
        v_524 = Main__St_6_Hw3;
      } else {
        v_524 = v_522;
      };
      if (v_508) {
        v_526 = Main__St_6_Hw2;
      } else {
        v_526 = v_524;
      };
      if (e) {
        s_St_6_Hw1 = Main__St_6_Inactive;
      } else {
        s_St_6_Hw1 = v_526;
      };
      if (v_519) {
        v_521 = true;
      } else {
        v_521 = self->pnr;
      };
      if (v_512) {
        v_523 = true;
      } else {
        v_523 = v_521;
      };
      if (v_510) {
        v_525 = true;
      } else {
        v_525 = v_523;
      };
      if (v_508) {
        v_527 = true;
      } else {
        v_527 = v_525;
      };
      if (e) {
        r_16_St_6_Hw1 = true;
      } else {
        r_16_St_6_Hw1 = v_527;
      };
      s = s_St_6_Hw1;
      r_16 = r_16_St_6_Hw1;
      break;
    case Main__St_6_Sw:
      v_528 = !(c4);
      v_529 = (v_528&&c1);
      v_530 = !(c4);
      v_531 = (v_530&&c2);
      v_532 = !(c4);
      v_533 = (v_532&&c3);
      v_534 = !(c3);
      v_535 = !(c2);
      v_536 = (v_534&&v_535);
      v_537 = !(c1);
      v_538 = (v_536&&v_537);
      v_539 = !(c4);
      v_540 = (v_538&&v_539);
      if (v_540) {
        v_541 = Main__St_6_Wait;
      } else {
        v_541 = Main__St_6_Sw;
      };
      if (v_533) {
        v_543 = Main__St_6_Hw3;
      } else {
        v_543 = v_541;
      };
      if (v_531) {
        v_545 = Main__St_6_Hw2;
      } else {
        v_545 = v_543;
      };
      if (v_529) {
        v_547 = Main__St_6_Hw1;
      } else {
        v_547 = v_545;
      };
      if (e) {
        s_St_6_Sw = Main__St_6_Inactive;
      } else {
        s_St_6_Sw = v_547;
      };
      if (v_540) {
        v_542 = true;
      } else {
        v_542 = self->pnr;
      };
      if (v_533) {
        v_544 = true;
      } else {
        v_544 = v_542;
      };
      if (v_531) {
        v_546 = true;
      } else {
        v_546 = v_544;
      };
      if (v_529) {
        v_548 = true;
      } else {
        v_548 = v_546;
      };
      if (e) {
        r_16_St_6_Sw = true;
      } else {
        r_16_St_6_Sw = v_548;
      };
      s = s_St_6_Sw;
      r_16 = r_16_St_6_Sw;
      break;
    case Main__St_6_Inactive:
      v_549 = (r&&c1);
      v_550 = (r&&c2);
      v_551 = (r&&c3);
      v_552 = (r&&c4);
      v_553 = !(c3);
      v_554 = (r&&v_553);
      v_555 = !(c2);
      v_556 = (v_554&&v_555);
      v_557 = !(c1);
      v_558 = (v_556&&v_557);
      v_559 = !(c4);
      v_560 = (v_558&&v_559);
      if (v_560) {
        v_561 = Main__St_6_Wait;
      } else {
        v_561 = Main__St_6_Inactive;
      };
      if (v_552) {
        v_563 = Main__St_6_Sw;
      } else {
        v_563 = v_561;
      };
      if (v_551) {
        v_565 = Main__St_6_Hw3;
      } else {
        v_565 = v_563;
      };
      if (v_550) {
        v_567 = Main__St_6_Hw2;
      } else {
        v_567 = v_565;
      };
      if (v_549) {
        s_St_6_Inactive = Main__St_6_Hw1;
      } else {
        s_St_6_Inactive = v_567;
      };
      s = s_St_6_Inactive;
      if (v_560) {
        v_562 = true;
      } else {
        v_562 = self->pnr;
      };
      if (v_552) {
        v_564 = true;
      } else {
        v_564 = v_562;
      };
      if (v_551) {
        v_566 = true;
      } else {
        v_566 = v_564;
      };
      if (v_550) {
        v_568 = true;
      } else {
        v_568 = v_566;
      };
      if (v_549) {
        r_16_St_6_Inactive = true;
      } else {
        r_16_St_6_Inactive = v_568;
      };
      r_16 = r_16_St_6_Inactive;
      break;
    default:
      break;
  };
  ck_7 = s;
  switch (ck_7) {
    case Main__St_6_Wait:
      ns_St_6_Wait = Main__St_6_Wait;
      nr_St_6_Wait = false;
      size_St_6_Wait = 0;
      act_St_6_Wait = false;
      wt_St_6_Wait = true;
      ver_St_6_Wait = Main__N;
      ns = ns_St_6_Wait;
      nr = nr_St_6_Wait;
      _out->size = size_St_6_Wait;
      _out->act = act_St_6_Wait;
      _out->wt = wt_St_6_Wait;
      _out->ver = ver_St_6_Wait;
      break;
    case Main__St_6_Hw3:
      ns_St_6_Hw3 = Main__St_6_Hw3;
      nr_St_6_Hw3 = false;
      size_St_6_Hw3 = 1;
      act_St_6_Hw3 = true;
      wt_St_6_Hw3 = false;
      ver_St_6_Hw3 = Main__H3;
      ns = ns_St_6_Hw3;
      nr = nr_St_6_Hw3;
      _out->size = size_St_6_Hw3;
      _out->act = act_St_6_Hw3;
      _out->wt = wt_St_6_Hw3;
      _out->ver = ver_St_6_Hw3;
      break;
    case Main__St_6_Hw2:
      ns_St_6_Hw2 = Main__St_6_Hw2;
      nr_St_6_Hw2 = false;
      size_St_6_Hw2 = 1;
      act_St_6_Hw2 = true;
      wt_St_6_Hw2 = false;
      ver_St_6_Hw2 = Main__H2;
      ns = ns_St_6_Hw2;
      nr = nr_St_6_Hw2;
      _out->size = size_St_6_Hw2;
      _out->act = act_St_6_Hw2;
      _out->wt = wt_St_6_Hw2;
      _out->ver = ver_St_6_Hw2;
      break;
    case Main__St_6_Hw1:
      ns_St_6_Hw1 = Main__St_6_Hw1;
      nr_St_6_Hw1 = false;
      size_St_6_Hw1 = 1;
      act_St_6_Hw1 = true;
      wt_St_6_Hw1 = false;
      ver_St_6_Hw1 = Main__H1;
      ns = ns_St_6_Hw1;
      nr = nr_St_6_Hw1;
      _out->size = size_St_6_Hw1;
      _out->act = act_St_6_Hw1;
      _out->wt = wt_St_6_Hw1;
      _out->ver = ver_St_6_Hw1;
      break;
    case Main__St_6_Sw:
      ns_St_6_Sw = Main__St_6_Sw;
      nr_St_6_Sw = false;
      size_St_6_Sw = 0;
      act_St_6_Sw = true;
      wt_St_6_Sw = false;
      ver_St_6_Sw = Main__S;
      ns = ns_St_6_Sw;
      nr = nr_St_6_Sw;
      _out->size = size_St_6_Sw;
      _out->act = act_St_6_Sw;
      _out->wt = wt_St_6_Sw;
      _out->ver = ver_St_6_Sw;
      break;
    case Main__St_6_Inactive:
      ns_St_6_Inactive = Main__St_6_Inactive;
      ns = ns_St_6_Inactive;
      nr_St_6_Inactive = false;
      nr = nr_St_6_Inactive;
      size_St_6_Inactive = 0;
      _out->size = size_St_6_Inactive;
      act_St_6_Inactive = false;
      _out->act = act_St_6_Inactive;
      wt_St_6_Inactive = false;
      _out->wt = wt_St_6_Inactive;
      ver_St_6_Inactive = Main__N;
      _out->ver = ver_St_6_Inactive;
      break;
    default:
      break;
  };
  self->ck = ns;
  self->pnr = nr;;
}

void Main__rotoz_s_reset(Main__rotoz_s_mem* self) {
  self->ck = Main__St_7_Inactive;
  self->pnr = false;
}

void Main__rotoz_s_step(int r, int c1, int c2, int c3, int c4, int e,
                        Main__rotoz_s_out* _out, Main__rotoz_s_mem* self) {
  
  int v_575;
  Main__st_7 v_574;
  int v_573;
  Main__st_7 v_572;
  int v_571;
  Main__st_7 v_570;
  int v_569;
  Main__st_7 v;
  int v_596;
  Main__st_7 v_595;
  int v_594;
  Main__st_7 v_593;
  int v_592;
  Main__st_7 v_591;
  int v_590;
  Main__st_7 v_589;
  int v_588;
  int v_587;
  int v_586;
  int v_585;
  int v_584;
  int v_583;
  int v_582;
  int v_581;
  int v_580;
  int v_579;
  int v_578;
  int v_577;
  int v_576;
  int v_617;
  Main__st_7 v_616;
  int v_615;
  Main__st_7 v_614;
  int v_613;
  Main__st_7 v_612;
  int v_611;
  Main__st_7 v_610;
  int v_609;
  int v_608;
  int v_607;
  int v_606;
  int v_605;
  int v_604;
  int v_603;
  int v_602;
  int v_601;
  int v_600;
  int v_599;
  int v_598;
  int v_597;
  int v_638;
  Main__st_7 v_637;
  int v_636;
  Main__st_7 v_635;
  int v_634;
  Main__st_7 v_633;
  int v_632;
  Main__st_7 v_631;
  int v_630;
  int v_629;
  int v_628;
  int v_627;
  int v_626;
  int v_625;
  int v_624;
  int v_623;
  int v_622;
  int v_621;
  int v_620;
  int v_619;
  int v_618;
  int v_659;
  Main__st_7 v_658;
  int v_657;
  Main__st_7 v_656;
  int v_655;
  Main__st_7 v_654;
  int v_653;
  Main__st_7 v_652;
  int v_651;
  int v_650;
  int v_649;
  int v_648;
  int v_647;
  int v_646;
  int v_645;
  int v_644;
  int v_643;
  int v_642;
  int v_641;
  int v_640;
  int v_639;
  int v_679;
  Main__st_7 v_678;
  int v_677;
  Main__st_7 v_676;
  int v_675;
  Main__st_7 v_674;
  int v_673;
  Main__st_7 v_672;
  int v_671;
  int v_670;
  int v_669;
  int v_668;
  int v_667;
  int v_666;
  int v_665;
  int v_664;
  int v_663;
  int v_662;
  int v_661;
  int v_660;
  int r_17_St_7_Wait;
  Main__st_7 s_St_7_Wait;
  int r_17_St_7_Hw3;
  Main__st_7 s_St_7_Hw3;
  int r_17_St_7_Hw2;
  Main__st_7 s_St_7_Hw2;
  int r_17_St_7_Hw1;
  Main__st_7 s_St_7_Hw1;
  int r_17_St_7_Sw;
  Main__st_7 s_St_7_Sw;
  int r_17_St_7_Inactive;
  Main__st_7 s_St_7_Inactive;
  int nr_St_7_Wait;
  Main__st_7 ns_St_7_Wait;
  Main__version_type ver_St_7_Wait;
  int wt_St_7_Wait;
  int act_St_7_Wait;
  int size_St_7_Wait;
  int nr_St_7_Hw3;
  Main__st_7 ns_St_7_Hw3;
  Main__version_type ver_St_7_Hw3;
  int wt_St_7_Hw3;
  int act_St_7_Hw3;
  int size_St_7_Hw3;
  int nr_St_7_Hw2;
  Main__st_7 ns_St_7_Hw2;
  Main__version_type ver_St_7_Hw2;
  int wt_St_7_Hw2;
  int act_St_7_Hw2;
  int size_St_7_Hw2;
  int nr_St_7_Hw1;
  Main__st_7 ns_St_7_Hw1;
  Main__version_type ver_St_7_Hw1;
  int wt_St_7_Hw1;
  int act_St_7_Hw1;
  int size_St_7_Hw1;
  int nr_St_7_Sw;
  Main__st_7 ns_St_7_Sw;
  Main__version_type ver_St_7_Sw;
  int wt_St_7_Sw;
  int act_St_7_Sw;
  int size_St_7_Sw;
  int nr_St_7_Inactive;
  Main__st_7 ns_St_7_Inactive;
  Main__version_type ver_St_7_Inactive;
  int wt_St_7_Inactive;
  int act_St_7_Inactive;
  int size_St_7_Inactive;
  Main__st_7 ck_8;
  Main__st_7 s;
  Main__st_7 ns;
  int r_17;
  int nr;
  switch (self->ck) {
    case Main__St_7_Wait:
      if (c4) {
        v = Main__St_7_Sw;
      } else {
        v = Main__St_7_Wait;
      };
      if (c3) {
        v_570 = Main__St_7_Hw3;
      } else {
        v_570 = v;
      };
      if (c2) {
        v_572 = Main__St_7_Hw2;
      } else {
        v_572 = v_570;
      };
      if (c1) {
        v_574 = Main__St_7_Hw1;
      } else {
        v_574 = v_572;
      };
      if (e) {
        s_St_7_Wait = Main__St_7_Inactive;
      } else {
        s_St_7_Wait = v_574;
      };
      if (c4) {
        v_569 = true;
      } else {
        v_569 = self->pnr;
      };
      if (c3) {
        v_571 = true;
      } else {
        v_571 = v_569;
      };
      if (c2) {
        v_573 = true;
      } else {
        v_573 = v_571;
      };
      if (c1) {
        v_575 = true;
      } else {
        v_575 = v_573;
      };
      if (e) {
        r_17_St_7_Wait = true;
      } else {
        r_17_St_7_Wait = v_575;
      };
      s = s_St_7_Wait;
      r_17 = r_17_St_7_Wait;
      break;
    case Main__St_7_Hw3:
      v_576 = !(c3);
      v_577 = (v_576&&c1);
      v_578 = !(c3);
      v_579 = (v_578&&c2);
      v_580 = !(c3);
      v_581 = (v_580&&c4);
      v_582 = !(c3);
      v_583 = !(c2);
      v_584 = (v_582&&v_583);
      v_585 = !(c1);
      v_586 = (v_584&&v_585);
      v_587 = !(c4);
      v_588 = (v_586&&v_587);
      if (v_588) {
        v_589 = Main__St_7_Wait;
      } else {
        v_589 = Main__St_7_Hw3;
      };
      if (v_581) {
        v_591 = Main__St_7_Sw;
      } else {
        v_591 = v_589;
      };
      if (v_579) {
        v_593 = Main__St_7_Hw2;
      } else {
        v_593 = v_591;
      };
      if (v_577) {
        v_595 = Main__St_7_Hw1;
      } else {
        v_595 = v_593;
      };
      if (e) {
        s_St_7_Hw3 = Main__St_7_Inactive;
      } else {
        s_St_7_Hw3 = v_595;
      };
      if (v_588) {
        v_590 = true;
      } else {
        v_590 = self->pnr;
      };
      if (v_581) {
        v_592 = true;
      } else {
        v_592 = v_590;
      };
      if (v_579) {
        v_594 = true;
      } else {
        v_594 = v_592;
      };
      if (v_577) {
        v_596 = true;
      } else {
        v_596 = v_594;
      };
      if (e) {
        r_17_St_7_Hw3 = true;
      } else {
        r_17_St_7_Hw3 = v_596;
      };
      s = s_St_7_Hw3;
      r_17 = r_17_St_7_Hw3;
      break;
    case Main__St_7_Hw2:
      v_597 = !(c2);
      v_598 = (v_597&&c1);
      v_599 = !(c2);
      v_600 = (v_599&&c3);
      v_601 = !(c2);
      v_602 = (v_601&&c4);
      v_603 = !(c3);
      v_604 = !(c2);
      v_605 = (v_603&&v_604);
      v_606 = !(c1);
      v_607 = (v_605&&v_606);
      v_608 = !(c4);
      v_609 = (v_607&&v_608);
      if (v_609) {
        v_610 = Main__St_7_Wait;
      } else {
        v_610 = Main__St_7_Hw2;
      };
      if (v_602) {
        v_612 = Main__St_7_Sw;
      } else {
        v_612 = v_610;
      };
      if (v_600) {
        v_614 = Main__St_7_Hw3;
      } else {
        v_614 = v_612;
      };
      if (v_598) {
        v_616 = Main__St_7_Hw1;
      } else {
        v_616 = v_614;
      };
      if (e) {
        s_St_7_Hw2 = Main__St_7_Inactive;
      } else {
        s_St_7_Hw2 = v_616;
      };
      if (v_609) {
        v_611 = true;
      } else {
        v_611 = self->pnr;
      };
      if (v_602) {
        v_613 = true;
      } else {
        v_613 = v_611;
      };
      if (v_600) {
        v_615 = true;
      } else {
        v_615 = v_613;
      };
      if (v_598) {
        v_617 = true;
      } else {
        v_617 = v_615;
      };
      if (e) {
        r_17_St_7_Hw2 = true;
      } else {
        r_17_St_7_Hw2 = v_617;
      };
      s = s_St_7_Hw2;
      r_17 = r_17_St_7_Hw2;
      break;
    case Main__St_7_Hw1:
      v_618 = !(c1);
      v_619 = (v_618&&c2);
      v_620 = !(c1);
      v_621 = (v_620&&c3);
      v_622 = !(c1);
      v_623 = (v_622&&c4);
      v_624 = !(c3);
      v_625 = !(c2);
      v_626 = (v_624&&v_625);
      v_627 = !(c1);
      v_628 = (v_626&&v_627);
      v_629 = !(c4);
      v_630 = (v_628&&v_629);
      if (v_630) {
        v_631 = Main__St_7_Wait;
      } else {
        v_631 = Main__St_7_Hw1;
      };
      if (v_623) {
        v_633 = Main__St_7_Sw;
      } else {
        v_633 = v_631;
      };
      if (v_621) {
        v_635 = Main__St_7_Hw3;
      } else {
        v_635 = v_633;
      };
      if (v_619) {
        v_637 = Main__St_7_Hw2;
      } else {
        v_637 = v_635;
      };
      if (e) {
        s_St_7_Hw1 = Main__St_7_Inactive;
      } else {
        s_St_7_Hw1 = v_637;
      };
      if (v_630) {
        v_632 = true;
      } else {
        v_632 = self->pnr;
      };
      if (v_623) {
        v_634 = true;
      } else {
        v_634 = v_632;
      };
      if (v_621) {
        v_636 = true;
      } else {
        v_636 = v_634;
      };
      if (v_619) {
        v_638 = true;
      } else {
        v_638 = v_636;
      };
      if (e) {
        r_17_St_7_Hw1 = true;
      } else {
        r_17_St_7_Hw1 = v_638;
      };
      s = s_St_7_Hw1;
      r_17 = r_17_St_7_Hw1;
      break;
    case Main__St_7_Sw:
      v_639 = !(c4);
      v_640 = (v_639&&c1);
      v_641 = !(c4);
      v_642 = (v_641&&c2);
      v_643 = !(c4);
      v_644 = (v_643&&c3);
      v_645 = !(c3);
      v_646 = !(c2);
      v_647 = (v_645&&v_646);
      v_648 = !(c1);
      v_649 = (v_647&&v_648);
      v_650 = !(c4);
      v_651 = (v_649&&v_650);
      if (v_651) {
        v_652 = Main__St_7_Wait;
      } else {
        v_652 = Main__St_7_Sw;
      };
      if (v_644) {
        v_654 = Main__St_7_Hw3;
      } else {
        v_654 = v_652;
      };
      if (v_642) {
        v_656 = Main__St_7_Hw2;
      } else {
        v_656 = v_654;
      };
      if (v_640) {
        v_658 = Main__St_7_Hw1;
      } else {
        v_658 = v_656;
      };
      if (e) {
        s_St_7_Sw = Main__St_7_Inactive;
      } else {
        s_St_7_Sw = v_658;
      };
      if (v_651) {
        v_653 = true;
      } else {
        v_653 = self->pnr;
      };
      if (v_644) {
        v_655 = true;
      } else {
        v_655 = v_653;
      };
      if (v_642) {
        v_657 = true;
      } else {
        v_657 = v_655;
      };
      if (v_640) {
        v_659 = true;
      } else {
        v_659 = v_657;
      };
      if (e) {
        r_17_St_7_Sw = true;
      } else {
        r_17_St_7_Sw = v_659;
      };
      s = s_St_7_Sw;
      r_17 = r_17_St_7_Sw;
      break;
    case Main__St_7_Inactive:
      v_660 = (r&&c1);
      v_661 = (r&&c2);
      v_662 = (r&&c3);
      v_663 = (r&&c4);
      v_664 = !(c3);
      v_665 = (r&&v_664);
      v_666 = !(c2);
      v_667 = (v_665&&v_666);
      v_668 = !(c1);
      v_669 = (v_667&&v_668);
      v_670 = !(c4);
      v_671 = (v_669&&v_670);
      if (v_671) {
        v_672 = Main__St_7_Wait;
      } else {
        v_672 = Main__St_7_Inactive;
      };
      if (v_663) {
        v_674 = Main__St_7_Sw;
      } else {
        v_674 = v_672;
      };
      if (v_662) {
        v_676 = Main__St_7_Hw3;
      } else {
        v_676 = v_674;
      };
      if (v_661) {
        v_678 = Main__St_7_Hw2;
      } else {
        v_678 = v_676;
      };
      if (v_660) {
        s_St_7_Inactive = Main__St_7_Hw1;
      } else {
        s_St_7_Inactive = v_678;
      };
      s = s_St_7_Inactive;
      if (v_671) {
        v_673 = true;
      } else {
        v_673 = self->pnr;
      };
      if (v_663) {
        v_675 = true;
      } else {
        v_675 = v_673;
      };
      if (v_662) {
        v_677 = true;
      } else {
        v_677 = v_675;
      };
      if (v_661) {
        v_679 = true;
      } else {
        v_679 = v_677;
      };
      if (v_660) {
        r_17_St_7_Inactive = true;
      } else {
        r_17_St_7_Inactive = v_679;
      };
      r_17 = r_17_St_7_Inactive;
      break;
    default:
      break;
  };
  ck_8 = s;
  switch (ck_8) {
    case Main__St_7_Wait:
      ns_St_7_Wait = Main__St_7_Wait;
      nr_St_7_Wait = false;
      size_St_7_Wait = 0;
      act_St_7_Wait = false;
      wt_St_7_Wait = true;
      ver_St_7_Wait = Main__N;
      ns = ns_St_7_Wait;
      nr = nr_St_7_Wait;
      _out->size = size_St_7_Wait;
      _out->act = act_St_7_Wait;
      _out->wt = wt_St_7_Wait;
      _out->ver = ver_St_7_Wait;
      break;
    case Main__St_7_Hw3:
      ns_St_7_Hw3 = Main__St_7_Hw3;
      nr_St_7_Hw3 = false;
      size_St_7_Hw3 = 1;
      act_St_7_Hw3 = true;
      wt_St_7_Hw3 = false;
      ver_St_7_Hw3 = Main__H3;
      ns = ns_St_7_Hw3;
      nr = nr_St_7_Hw3;
      _out->size = size_St_7_Hw3;
      _out->act = act_St_7_Hw3;
      _out->wt = wt_St_7_Hw3;
      _out->ver = ver_St_7_Hw3;
      break;
    case Main__St_7_Hw2:
      ns_St_7_Hw2 = Main__St_7_Hw2;
      nr_St_7_Hw2 = false;
      size_St_7_Hw2 = 1;
      act_St_7_Hw2 = true;
      wt_St_7_Hw2 = false;
      ver_St_7_Hw2 = Main__H2;
      ns = ns_St_7_Hw2;
      nr = nr_St_7_Hw2;
      _out->size = size_St_7_Hw2;
      _out->act = act_St_7_Hw2;
      _out->wt = wt_St_7_Hw2;
      _out->ver = ver_St_7_Hw2;
      break;
    case Main__St_7_Hw1:
      ns_St_7_Hw1 = Main__St_7_Hw1;
      nr_St_7_Hw1 = false;
      size_St_7_Hw1 = 1;
      act_St_7_Hw1 = true;
      wt_St_7_Hw1 = false;
      ver_St_7_Hw1 = Main__H1;
      ns = ns_St_7_Hw1;
      nr = nr_St_7_Hw1;
      _out->size = size_St_7_Hw1;
      _out->act = act_St_7_Hw1;
      _out->wt = wt_St_7_Hw1;
      _out->ver = ver_St_7_Hw1;
      break;
    case Main__St_7_Sw:
      ns_St_7_Sw = Main__St_7_Sw;
      nr_St_7_Sw = false;
      size_St_7_Sw = 0;
      act_St_7_Sw = true;
      wt_St_7_Sw = false;
      ver_St_7_Sw = Main__S;
      ns = ns_St_7_Sw;
      nr = nr_St_7_Sw;
      _out->size = size_St_7_Sw;
      _out->act = act_St_7_Sw;
      _out->wt = wt_St_7_Sw;
      _out->ver = ver_St_7_Sw;
      break;
    case Main__St_7_Inactive:
      ns_St_7_Inactive = Main__St_7_Inactive;
      ns = ns_St_7_Inactive;
      nr_St_7_Inactive = false;
      nr = nr_St_7_Inactive;
      size_St_7_Inactive = 0;
      _out->size = size_St_7_Inactive;
      act_St_7_Inactive = false;
      _out->act = act_St_7_Inactive;
      wt_St_7_Inactive = false;
      _out->wt = wt_St_7_Inactive;
      ver_St_7_Inactive = Main__N;
      _out->ver = ver_St_7_Inactive;
      break;
    default:
      break;
  };
  self->ck = ns;
  self->pnr = nr;;
}

void Main__rotoz_b_reset(Main__rotoz_b_mem* self) {
  self->ck = Main__St_8_Inactive;
  self->pnr = false;
}

void Main__rotoz_b_step(int r, int c1, int c2, int e,
                        Main__rotoz_b_out* _out, Main__rotoz_b_mem* self) {
  
  int v_682;
  Main__st_8 v_681;
  int v_680;
  Main__st_8 v;
  int v_691;
  Main__st_8 v_690;
  int v_689;
  Main__st_8 v_688;
  int v_687;
  int v_686;
  int v_685;
  int v_684;
  int v_683;
  int v_700;
  Main__st_8 v_699;
  int v_698;
  Main__st_8 v_697;
  int v_696;
  int v_695;
  int v_694;
  int v_693;
  int v_692;
  int v_710;
  Main__st_8 v_709;
  int v_708;
  Main__st_8 v_707;
  int v_706;
  int v_705;
  int v_704;
  int v_703;
  int v_702;
  int v_701;
  int r_18_St_8_Wait;
  Main__st_8 s_St_8_Wait;
  int r_18_St_8_Hw2;
  Main__st_8 s_St_8_Hw2;
  int r_18_St_8_Hw1;
  Main__st_8 s_St_8_Hw1;
  int r_18_St_8_Inactive;
  Main__st_8 s_St_8_Inactive;
  int nr_St_8_Wait;
  Main__st_8 ns_St_8_Wait;
  Main__version_type ver_St_8_Wait;
  int wt_St_8_Wait;
  int act_St_8_Wait;
  int size_St_8_Wait;
  int nr_St_8_Hw2;
  Main__st_8 ns_St_8_Hw2;
  Main__version_type ver_St_8_Hw2;
  int wt_St_8_Hw2;
  int act_St_8_Hw2;
  int size_St_8_Hw2;
  int nr_St_8_Hw1;
  Main__st_8 ns_St_8_Hw1;
  Main__version_type ver_St_8_Hw1;
  int wt_St_8_Hw1;
  int act_St_8_Hw1;
  int size_St_8_Hw1;
  int nr_St_8_Inactive;
  Main__st_8 ns_St_8_Inactive;
  Main__version_type ver_St_8_Inactive;
  int wt_St_8_Inactive;
  int act_St_8_Inactive;
  int size_St_8_Inactive;
  Main__st_8 ck_9;
  Main__st_8 s;
  Main__st_8 ns;
  int r_18;
  int nr;
  switch (self->ck) {
    case Main__St_8_Wait:
      if (c2) {
        v = Main__St_8_Hw2;
      } else {
        v = Main__St_8_Wait;
      };
      if (c1) {
        v_681 = Main__St_8_Hw1;
      } else {
        v_681 = v;
      };
      if (e) {
        s_St_8_Wait = Main__St_8_Inactive;
      } else {
        s_St_8_Wait = v_681;
      };
      if (c2) {
        v_680 = true;
      } else {
        v_680 = self->pnr;
      };
      if (c1) {
        v_682 = true;
      } else {
        v_682 = v_680;
      };
      if (e) {
        r_18_St_8_Wait = true;
      } else {
        r_18_St_8_Wait = v_682;
      };
      s = s_St_8_Wait;
      r_18 = r_18_St_8_Wait;
      break;
    case Main__St_8_Hw2:
      v_683 = !(c2);
      v_684 = (v_683&&c1);
      v_685 = !(c2);
      v_686 = !(c1);
      v_687 = (v_685&&v_686);
      if (v_687) {
        v_688 = Main__St_8_Wait;
      } else {
        v_688 = Main__St_8_Hw2;
      };
      if (v_684) {
        v_690 = Main__St_8_Hw1;
      } else {
        v_690 = v_688;
      };
      if (e) {
        s_St_8_Hw2 = Main__St_8_Inactive;
      } else {
        s_St_8_Hw2 = v_690;
      };
      if (v_687) {
        v_689 = true;
      } else {
        v_689 = self->pnr;
      };
      if (v_684) {
        v_691 = true;
      } else {
        v_691 = v_689;
      };
      if (e) {
        r_18_St_8_Hw2 = true;
      } else {
        r_18_St_8_Hw2 = v_691;
      };
      s = s_St_8_Hw2;
      r_18 = r_18_St_8_Hw2;
      break;
    case Main__St_8_Hw1:
      v_692 = !(c1);
      v_693 = (v_692&&c2);
      v_694 = !(c2);
      v_695 = !(c1);
      v_696 = (v_694&&v_695);
      if (v_696) {
        v_697 = Main__St_8_Wait;
      } else {
        v_697 = Main__St_8_Hw1;
      };
      if (v_693) {
        v_699 = Main__St_8_Hw2;
      } else {
        v_699 = v_697;
      };
      if (e) {
        s_St_8_Hw1 = Main__St_8_Inactive;
      } else {
        s_St_8_Hw1 = v_699;
      };
      if (v_696) {
        v_698 = true;
      } else {
        v_698 = self->pnr;
      };
      if (v_693) {
        v_700 = true;
      } else {
        v_700 = v_698;
      };
      if (e) {
        r_18_St_8_Hw1 = true;
      } else {
        r_18_St_8_Hw1 = v_700;
      };
      s = s_St_8_Hw1;
      r_18 = r_18_St_8_Hw1;
      break;
    case Main__St_8_Inactive:
      v_701 = (r&&c1);
      v_702 = (r&&c2);
      v_703 = !(c2);
      v_704 = (r&&v_703);
      v_705 = !(c1);
      v_706 = (v_704&&v_705);
      if (v_706) {
        v_707 = Main__St_8_Wait;
      } else {
        v_707 = Main__St_8_Inactive;
      };
      if (v_702) {
        v_709 = Main__St_8_Hw2;
      } else {
        v_709 = v_707;
      };
      if (v_701) {
        s_St_8_Inactive = Main__St_8_Hw1;
      } else {
        s_St_8_Inactive = v_709;
      };
      s = s_St_8_Inactive;
      if (v_706) {
        v_708 = true;
      } else {
        v_708 = self->pnr;
      };
      if (v_702) {
        v_710 = true;
      } else {
        v_710 = v_708;
      };
      if (v_701) {
        r_18_St_8_Inactive = true;
      } else {
        r_18_St_8_Inactive = v_710;
      };
      r_18 = r_18_St_8_Inactive;
      break;
    default:
      break;
  };
  ck_9 = s;
  switch (ck_9) {
    case Main__St_8_Wait:
      ns_St_8_Wait = Main__St_8_Wait;
      nr_St_8_Wait = false;
      size_St_8_Wait = 0;
      act_St_8_Wait = false;
      wt_St_8_Wait = true;
      ver_St_8_Wait = Main__N;
      ns = ns_St_8_Wait;
      nr = nr_St_8_Wait;
      _out->size = size_St_8_Wait;
      _out->act = act_St_8_Wait;
      _out->wt = wt_St_8_Wait;
      _out->ver = ver_St_8_Wait;
      break;
    case Main__St_8_Hw2:
      ns_St_8_Hw2 = Main__St_8_Hw2;
      nr_St_8_Hw2 = false;
      size_St_8_Hw2 = 2;
      act_St_8_Hw2 = true;
      wt_St_8_Hw2 = false;
      ver_St_8_Hw2 = Main__H2;
      ns = ns_St_8_Hw2;
      nr = nr_St_8_Hw2;
      _out->size = size_St_8_Hw2;
      _out->act = act_St_8_Hw2;
      _out->wt = wt_St_8_Hw2;
      _out->ver = ver_St_8_Hw2;
      break;
    case Main__St_8_Hw1:
      ns_St_8_Hw1 = Main__St_8_Hw1;
      nr_St_8_Hw1 = false;
      size_St_8_Hw1 = 2;
      act_St_8_Hw1 = true;
      wt_St_8_Hw1 = false;
      ver_St_8_Hw1 = Main__H1;
      ns = ns_St_8_Hw1;
      nr = nr_St_8_Hw1;
      _out->size = size_St_8_Hw1;
      _out->act = act_St_8_Hw1;
      _out->wt = wt_St_8_Hw1;
      _out->ver = ver_St_8_Hw1;
      break;
    case Main__St_8_Inactive:
      ns_St_8_Inactive = Main__St_8_Inactive;
      ns = ns_St_8_Inactive;
      nr_St_8_Inactive = false;
      nr = nr_St_8_Inactive;
      size_St_8_Inactive = 0;
      _out->size = size_St_8_Inactive;
      act_St_8_Inactive = false;
      _out->act = act_St_8_Inactive;
      wt_St_8_Inactive = false;
      _out->wt = wt_St_8_Inactive;
      ver_St_8_Inactive = Main__N;
      _out->ver = ver_St_8_Inactive;
      break;
    default:
      break;
  };
  self->ck = ns;
  self->pnr = nr;;
}

void Main__replanning_reset(Main__replanning_mem* self) {
  self->ck = Main__St_9_Inactive;
  self->pnr = false;
}

void Main__replanning_step(int r, int c1, int c2, int c3, int e,
                           Main__replanning_out* _out,
                           Main__replanning_mem* self) {
  
  int v_715;
  Main__st_9 v_714;
  int v_713;
  Main__st_9 v_712;
  int v_711;
  Main__st_9 v;
  int v_730;
  Main__st_9 v_729;
  int v_728;
  Main__st_9 v_727;
  int v_726;
  Main__st_9 v_725;
  int v_724;
  int v_723;
  int v_722;
  int v_721;
  int v_720;
  int v_719;
  int v_718;
  int v_717;
  int v_716;
  int v_745;
  Main__st_9 v_744;
  int v_743;
  Main__st_9 v_742;
  int v_741;
  Main__st_9 v_740;
  int v_739;
  int v_738;
  int v_737;
  int v_736;
  int v_735;
  int v_734;
  int v_733;
  int v_732;
  int v_731;
  int v_760;
  Main__st_9 v_759;
  int v_758;
  Main__st_9 v_757;
  int v_756;
  Main__st_9 v_755;
  int v_754;
  int v_753;
  int v_752;
  int v_751;
  int v_750;
  int v_749;
  int v_748;
  int v_747;
  int v_746;
  int v_775;
  Main__st_9 v_774;
  int v_773;
  Main__st_9 v_772;
  int v_771;
  Main__st_9 v_770;
  int v_769;
  int v_768;
  int v_767;
  int v_766;
  int v_765;
  int v_764;
  int v_763;
  int v_762;
  int v_761;
  int r_19_St_9_Wait;
  Main__st_9 s_St_9_Wait;
  int r_19_St_9_Hw2;
  Main__st_9 s_St_9_Hw2;
  int r_19_St_9_Hw1;
  Main__st_9 s_St_9_Hw1;
  int r_19_St_9_Sw;
  Main__st_9 s_St_9_Sw;
  int r_19_St_9_Inactive;
  Main__st_9 s_St_9_Inactive;
  int nr_St_9_Wait;
  Main__st_9 ns_St_9_Wait;
  Main__version_type ver_St_9_Wait;
  int wt_St_9_Wait;
  int act_St_9_Wait;
  int size_St_9_Wait;
  int nr_St_9_Hw2;
  Main__st_9 ns_St_9_Hw2;
  Main__version_type ver_St_9_Hw2;
  int wt_St_9_Hw2;
  int act_St_9_Hw2;
  int size_St_9_Hw2;
  int nr_St_9_Hw1;
  Main__st_9 ns_St_9_Hw1;
  Main__version_type ver_St_9_Hw1;
  int wt_St_9_Hw1;
  int act_St_9_Hw1;
  int size_St_9_Hw1;
  int nr_St_9_Sw;
  Main__st_9 ns_St_9_Sw;
  Main__version_type ver_St_9_Sw;
  int wt_St_9_Sw;
  int act_St_9_Sw;
  int size_St_9_Sw;
  int nr_St_9_Inactive;
  Main__st_9 ns_St_9_Inactive;
  Main__version_type ver_St_9_Inactive;
  int wt_St_9_Inactive;
  int act_St_9_Inactive;
  int size_St_9_Inactive;
  Main__st_9 ck_10;
  Main__st_9 s;
  Main__st_9 ns;
  int r_19;
  int nr;
  switch (self->ck) {
    case Main__St_9_Wait:
      if (c3) {
        v = Main__St_9_Sw;
      } else {
        v = Main__St_9_Wait;
      };
      if (c2) {
        v_712 = Main__St_9_Hw2;
      } else {
        v_712 = v;
      };
      if (c1) {
        v_714 = Main__St_9_Hw1;
      } else {
        v_714 = v_712;
      };
      if (e) {
        s_St_9_Wait = Main__St_9_Inactive;
      } else {
        s_St_9_Wait = v_714;
      };
      if (c3) {
        v_711 = true;
      } else {
        v_711 = self->pnr;
      };
      if (c2) {
        v_713 = true;
      } else {
        v_713 = v_711;
      };
      if (c1) {
        v_715 = true;
      } else {
        v_715 = v_713;
      };
      if (e) {
        r_19_St_9_Wait = true;
      } else {
        r_19_St_9_Wait = v_715;
      };
      s = s_St_9_Wait;
      r_19 = r_19_St_9_Wait;
      break;
    case Main__St_9_Hw2:
      v_716 = !(c2);
      v_717 = (v_716&&c1);
      v_718 = !(c2);
      v_719 = (v_718&&c3);
      v_720 = !(c2);
      v_721 = !(c1);
      v_722 = (v_720&&v_721);
      v_723 = !(c3);
      v_724 = (v_722&&v_723);
      if (v_724) {
        v_725 = Main__St_9_Wait;
      } else {
        v_725 = Main__St_9_Hw2;
      };
      if (v_719) {
        v_727 = Main__St_9_Sw;
      } else {
        v_727 = v_725;
      };
      if (v_717) {
        v_729 = Main__St_9_Hw1;
      } else {
        v_729 = v_727;
      };
      if (e) {
        s_St_9_Hw2 = Main__St_9_Inactive;
      } else {
        s_St_9_Hw2 = v_729;
      };
      if (v_724) {
        v_726 = true;
      } else {
        v_726 = self->pnr;
      };
      if (v_719) {
        v_728 = true;
      } else {
        v_728 = v_726;
      };
      if (v_717) {
        v_730 = true;
      } else {
        v_730 = v_728;
      };
      if (e) {
        r_19_St_9_Hw2 = true;
      } else {
        r_19_St_9_Hw2 = v_730;
      };
      s = s_St_9_Hw2;
      r_19 = r_19_St_9_Hw2;
      break;
    case Main__St_9_Hw1:
      v_731 = !(c1);
      v_732 = (v_731&&c2);
      v_733 = !(c2);
      v_734 = (v_733&&c3);
      v_735 = !(c2);
      v_736 = !(c1);
      v_737 = (v_735&&v_736);
      v_738 = !(c3);
      v_739 = (v_737&&v_738);
      if (v_739) {
        v_740 = Main__St_9_Wait;
      } else {
        v_740 = Main__St_9_Hw1;
      };
      if (v_734) {
        v_742 = Main__St_9_Sw;
      } else {
        v_742 = v_740;
      };
      if (v_732) {
        v_744 = Main__St_9_Hw2;
      } else {
        v_744 = v_742;
      };
      if (e) {
        s_St_9_Hw1 = Main__St_9_Inactive;
      } else {
        s_St_9_Hw1 = v_744;
      };
      if (v_739) {
        v_741 = true;
      } else {
        v_741 = self->pnr;
      };
      if (v_734) {
        v_743 = true;
      } else {
        v_743 = v_741;
      };
      if (v_732) {
        v_745 = true;
      } else {
        v_745 = v_743;
      };
      if (e) {
        r_19_St_9_Hw1 = true;
      } else {
        r_19_St_9_Hw1 = v_745;
      };
      s = s_St_9_Hw1;
      r_19 = r_19_St_9_Hw1;
      break;
    case Main__St_9_Sw:
      v_746 = !(c3);
      v_747 = (v_746&&c1);
      v_748 = !(c3);
      v_749 = (v_748&&c2);
      v_750 = !(c3);
      v_751 = !(c2);
      v_752 = (v_750&&v_751);
      v_753 = !(c1);
      v_754 = (v_752&&v_753);
      if (v_754) {
        v_755 = Main__St_9_Wait;
      } else {
        v_755 = Main__St_9_Sw;
      };
      if (v_749) {
        v_757 = Main__St_9_Hw2;
      } else {
        v_757 = v_755;
      };
      if (v_747) {
        v_759 = Main__St_9_Hw1;
      } else {
        v_759 = v_757;
      };
      if (e) {
        s_St_9_Sw = Main__St_9_Inactive;
      } else {
        s_St_9_Sw = v_759;
      };
      if (v_754) {
        v_756 = true;
      } else {
        v_756 = self->pnr;
      };
      if (v_749) {
        v_758 = true;
      } else {
        v_758 = v_756;
      };
      if (v_747) {
        v_760 = true;
      } else {
        v_760 = v_758;
      };
      if (e) {
        r_19_St_9_Sw = true;
      } else {
        r_19_St_9_Sw = v_760;
      };
      s = s_St_9_Sw;
      r_19 = r_19_St_9_Sw;
      break;
    case Main__St_9_Inactive:
      v_761 = (r&&c1);
      v_762 = (r&&c2);
      v_763 = (r&&c3);
      v_764 = !(c2);
      v_765 = (r&&v_764);
      v_766 = !(c1);
      v_767 = (v_765&&v_766);
      v_768 = !(c3);
      v_769 = (v_767&&v_768);
      if (v_769) {
        v_770 = Main__St_9_Wait;
      } else {
        v_770 = Main__St_9_Inactive;
      };
      if (v_763) {
        v_772 = Main__St_9_Sw;
      } else {
        v_772 = v_770;
      };
      if (v_762) {
        v_774 = Main__St_9_Hw2;
      } else {
        v_774 = v_772;
      };
      if (v_761) {
        s_St_9_Inactive = Main__St_9_Hw1;
      } else {
        s_St_9_Inactive = v_774;
      };
      s = s_St_9_Inactive;
      if (v_769) {
        v_771 = true;
      } else {
        v_771 = self->pnr;
      };
      if (v_763) {
        v_773 = true;
      } else {
        v_773 = v_771;
      };
      if (v_762) {
        v_775 = true;
      } else {
        v_775 = v_773;
      };
      if (v_761) {
        r_19_St_9_Inactive = true;
      } else {
        r_19_St_9_Inactive = v_775;
      };
      r_19 = r_19_St_9_Inactive;
      break;
    default:
      break;
  };
  ck_10 = s;
  switch (ck_10) {
    case Main__St_9_Wait:
      ns_St_9_Wait = Main__St_9_Wait;
      nr_St_9_Wait = false;
      size_St_9_Wait = 0;
      act_St_9_Wait = false;
      wt_St_9_Wait = true;
      ver_St_9_Wait = Main__N;
      ns = ns_St_9_Wait;
      nr = nr_St_9_Wait;
      _out->size = size_St_9_Wait;
      _out->act = act_St_9_Wait;
      _out->wt = wt_St_9_Wait;
      _out->ver = ver_St_9_Wait;
      break;
    case Main__St_9_Hw2:
      ns_St_9_Hw2 = Main__St_9_Hw2;
      nr_St_9_Hw2 = false;
      size_St_9_Hw2 = 2;
      act_St_9_Hw2 = true;
      wt_St_9_Hw2 = false;
      ver_St_9_Hw2 = Main__H2;
      ns = ns_St_9_Hw2;
      nr = nr_St_9_Hw2;
      _out->size = size_St_9_Hw2;
      _out->act = act_St_9_Hw2;
      _out->wt = wt_St_9_Hw2;
      _out->ver = ver_St_9_Hw2;
      break;
    case Main__St_9_Hw1:
      ns_St_9_Hw1 = Main__St_9_Hw1;
      nr_St_9_Hw1 = false;
      size_St_9_Hw1 = 2;
      act_St_9_Hw1 = true;
      wt_St_9_Hw1 = false;
      ver_St_9_Hw1 = Main__H1;
      ns = ns_St_9_Hw1;
      nr = nr_St_9_Hw1;
      _out->size = size_St_9_Hw1;
      _out->act = act_St_9_Hw1;
      _out->wt = wt_St_9_Hw1;
      _out->ver = ver_St_9_Hw1;
      break;
    case Main__St_9_Sw:
      ns_St_9_Sw = Main__St_9_Sw;
      nr_St_9_Sw = false;
      size_St_9_Sw = 0;
      act_St_9_Sw = true;
      wt_St_9_Sw = false;
      ver_St_9_Sw = Main__S;
      ns = ns_St_9_Sw;
      nr = nr_St_9_Sw;
      _out->size = size_St_9_Sw;
      _out->act = act_St_9_Sw;
      _out->wt = wt_St_9_Sw;
      _out->ver = ver_St_9_Sw;
      break;
    case Main__St_9_Inactive:
      ns_St_9_Inactive = Main__St_9_Inactive;
      ns = ns_St_9_Inactive;
      nr_St_9_Inactive = false;
      nr = nr_St_9_Inactive;
      size_St_9_Inactive = 0;
      _out->size = size_St_9_Inactive;
      act_St_9_Inactive = false;
      _out->act = act_St_9_Inactive;
      wt_St_9_Inactive = false;
      _out->wt = wt_St_9_Inactive;
      ver_St_9_Inactive = Main__N;
      _out->ver = ver_St_9_Inactive;
      break;
    default:
      break;
  };
  self->ck = ns;
  self->pnr = nr;;
}

void Main__detection_reset(Main__detection_mem* self) {
  self->ck = Main__St_10_Inactive;
  self->pnr = false;
}

void Main__detection_step(int r, int c1, int c2, int c3, int e,
                          Main__detection_out* _out,
                          Main__detection_mem* self) {
  
  int v_780;
  Main__st_10 v_779;
  int v_778;
  Main__st_10 v_777;
  int v_776;
  Main__st_10 v;
  int v_795;
  Main__st_10 v_794;
  int v_793;
  Main__st_10 v_792;
  int v_791;
  Main__st_10 v_790;
  int v_789;
  int v_788;
  int v_787;
  int v_786;
  int v_785;
  int v_784;
  int v_783;
  int v_782;
  int v_781;
  int v_810;
  Main__st_10 v_809;
  int v_808;
  Main__st_10 v_807;
  int v_806;
  Main__st_10 v_805;
  int v_804;
  int v_803;
  int v_802;
  int v_801;
  int v_800;
  int v_799;
  int v_798;
  int v_797;
  int v_796;
  int v_825;
  Main__st_10 v_824;
  int v_823;
  Main__st_10 v_822;
  int v_821;
  Main__st_10 v_820;
  int v_819;
  int v_818;
  int v_817;
  int v_816;
  int v_815;
  int v_814;
  int v_813;
  int v_812;
  int v_811;
  int v_840;
  Main__st_10 v_839;
  int v_838;
  Main__st_10 v_837;
  int v_836;
  Main__st_10 v_835;
  int v_834;
  int v_833;
  int v_832;
  int v_831;
  int v_830;
  int v_829;
  int v_828;
  int v_827;
  int v_826;
  int r_20_St_10_Wait;
  Main__st_10 s_St_10_Wait;
  int r_20_St_10_Hw2;
  Main__st_10 s_St_10_Hw2;
  int r_20_St_10_Hw1;
  Main__st_10 s_St_10_Hw1;
  int r_20_St_10_Sw;
  Main__st_10 s_St_10_Sw;
  int r_20_St_10_Inactive;
  Main__st_10 s_St_10_Inactive;
  int nr_St_10_Wait;
  Main__st_10 ns_St_10_Wait;
  Main__version_type ver_St_10_Wait;
  int wt_St_10_Wait;
  int act_St_10_Wait;
  int size_St_10_Wait;
  int nr_St_10_Hw2;
  Main__st_10 ns_St_10_Hw2;
  Main__version_type ver_St_10_Hw2;
  int wt_St_10_Hw2;
  int act_St_10_Hw2;
  int size_St_10_Hw2;
  int nr_St_10_Hw1;
  Main__st_10 ns_St_10_Hw1;
  Main__version_type ver_St_10_Hw1;
  int wt_St_10_Hw1;
  int act_St_10_Hw1;
  int size_St_10_Hw1;
  int nr_St_10_Sw;
  Main__st_10 ns_St_10_Sw;
  Main__version_type ver_St_10_Sw;
  int wt_St_10_Sw;
  int act_St_10_Sw;
  int size_St_10_Sw;
  int nr_St_10_Inactive;
  Main__st_10 ns_St_10_Inactive;
  Main__version_type ver_St_10_Inactive;
  int wt_St_10_Inactive;
  int act_St_10_Inactive;
  int size_St_10_Inactive;
  Main__st_10 ck_11;
  Main__st_10 s;
  Main__st_10 ns;
  int r_20;
  int nr;
  switch (self->ck) {
    case Main__St_10_Wait:
      if (c3) {
        v = Main__St_10_Sw;
      } else {
        v = Main__St_10_Wait;
      };
      if (c2) {
        v_777 = Main__St_10_Hw2;
      } else {
        v_777 = v;
      };
      if (c1) {
        v_779 = Main__St_10_Hw1;
      } else {
        v_779 = v_777;
      };
      if (e) {
        s_St_10_Wait = Main__St_10_Inactive;
      } else {
        s_St_10_Wait = v_779;
      };
      if (c3) {
        v_776 = true;
      } else {
        v_776 = self->pnr;
      };
      if (c2) {
        v_778 = true;
      } else {
        v_778 = v_776;
      };
      if (c1) {
        v_780 = true;
      } else {
        v_780 = v_778;
      };
      if (e) {
        r_20_St_10_Wait = true;
      } else {
        r_20_St_10_Wait = v_780;
      };
      s = s_St_10_Wait;
      r_20 = r_20_St_10_Wait;
      break;
    case Main__St_10_Hw2:
      v_781 = !(c2);
      v_782 = (v_781&&c1);
      v_783 = !(c2);
      v_784 = (v_783&&c3);
      v_785 = !(c2);
      v_786 = !(c1);
      v_787 = (v_785&&v_786);
      v_788 = !(c3);
      v_789 = (v_787&&v_788);
      if (v_789) {
        v_790 = Main__St_10_Wait;
      } else {
        v_790 = Main__St_10_Hw2;
      };
      if (v_784) {
        v_792 = Main__St_10_Sw;
      } else {
        v_792 = v_790;
      };
      if (v_782) {
        v_794 = Main__St_10_Hw1;
      } else {
        v_794 = v_792;
      };
      if (e) {
        s_St_10_Hw2 = Main__St_10_Inactive;
      } else {
        s_St_10_Hw2 = v_794;
      };
      if (v_789) {
        v_791 = true;
      } else {
        v_791 = self->pnr;
      };
      if (v_784) {
        v_793 = true;
      } else {
        v_793 = v_791;
      };
      if (v_782) {
        v_795 = true;
      } else {
        v_795 = v_793;
      };
      if (e) {
        r_20_St_10_Hw2 = true;
      } else {
        r_20_St_10_Hw2 = v_795;
      };
      s = s_St_10_Hw2;
      r_20 = r_20_St_10_Hw2;
      break;
    case Main__St_10_Hw1:
      v_796 = !(c1);
      v_797 = (v_796&&c2);
      v_798 = !(c2);
      v_799 = (v_798&&c3);
      v_800 = !(c2);
      v_801 = !(c1);
      v_802 = (v_800&&v_801);
      v_803 = !(c3);
      v_804 = (v_802&&v_803);
      if (v_804) {
        v_805 = Main__St_10_Wait;
      } else {
        v_805 = Main__St_10_Hw1;
      };
      if (v_799) {
        v_807 = Main__St_10_Sw;
      } else {
        v_807 = v_805;
      };
      if (v_797) {
        v_809 = Main__St_10_Hw2;
      } else {
        v_809 = v_807;
      };
      if (e) {
        s_St_10_Hw1 = Main__St_10_Inactive;
      } else {
        s_St_10_Hw1 = v_809;
      };
      if (v_804) {
        v_806 = true;
      } else {
        v_806 = self->pnr;
      };
      if (v_799) {
        v_808 = true;
      } else {
        v_808 = v_806;
      };
      if (v_797) {
        v_810 = true;
      } else {
        v_810 = v_808;
      };
      if (e) {
        r_20_St_10_Hw1 = true;
      } else {
        r_20_St_10_Hw1 = v_810;
      };
      s = s_St_10_Hw1;
      r_20 = r_20_St_10_Hw1;
      break;
    case Main__St_10_Sw:
      v_811 = !(c3);
      v_812 = (v_811&&c1);
      v_813 = !(c3);
      v_814 = (v_813&&c2);
      v_815 = !(c3);
      v_816 = !(c2);
      v_817 = (v_815&&v_816);
      v_818 = !(c1);
      v_819 = (v_817&&v_818);
      if (v_819) {
        v_820 = Main__St_10_Wait;
      } else {
        v_820 = Main__St_10_Sw;
      };
      if (v_814) {
        v_822 = Main__St_10_Hw2;
      } else {
        v_822 = v_820;
      };
      if (v_812) {
        v_824 = Main__St_10_Hw1;
      } else {
        v_824 = v_822;
      };
      if (e) {
        s_St_10_Sw = Main__St_10_Inactive;
      } else {
        s_St_10_Sw = v_824;
      };
      if (v_819) {
        v_821 = true;
      } else {
        v_821 = self->pnr;
      };
      if (v_814) {
        v_823 = true;
      } else {
        v_823 = v_821;
      };
      if (v_812) {
        v_825 = true;
      } else {
        v_825 = v_823;
      };
      if (e) {
        r_20_St_10_Sw = true;
      } else {
        r_20_St_10_Sw = v_825;
      };
      s = s_St_10_Sw;
      r_20 = r_20_St_10_Sw;
      break;
    case Main__St_10_Inactive:
      v_826 = (r&&c1);
      v_827 = (r&&c2);
      v_828 = (r&&c3);
      v_829 = !(c2);
      v_830 = (r&&v_829);
      v_831 = !(c1);
      v_832 = (v_830&&v_831);
      v_833 = !(c3);
      v_834 = (v_832&&v_833);
      if (v_834) {
        v_835 = Main__St_10_Wait;
      } else {
        v_835 = Main__St_10_Inactive;
      };
      if (v_828) {
        v_837 = Main__St_10_Sw;
      } else {
        v_837 = v_835;
      };
      if (v_827) {
        v_839 = Main__St_10_Hw2;
      } else {
        v_839 = v_837;
      };
      if (v_826) {
        s_St_10_Inactive = Main__St_10_Hw1;
      } else {
        s_St_10_Inactive = v_839;
      };
      s = s_St_10_Inactive;
      if (v_834) {
        v_836 = true;
      } else {
        v_836 = self->pnr;
      };
      if (v_828) {
        v_838 = true;
      } else {
        v_838 = v_836;
      };
      if (v_827) {
        v_840 = true;
      } else {
        v_840 = v_838;
      };
      if (v_826) {
        r_20_St_10_Inactive = true;
      } else {
        r_20_St_10_Inactive = v_840;
      };
      r_20 = r_20_St_10_Inactive;
      break;
    default:
      break;
  };
  ck_11 = s;
  switch (ck_11) {
    case Main__St_10_Wait:
      ns_St_10_Wait = Main__St_10_Wait;
      nr_St_10_Wait = false;
      size_St_10_Wait = 0;
      act_St_10_Wait = false;
      wt_St_10_Wait = true;
      ver_St_10_Wait = Main__N;
      ns = ns_St_10_Wait;
      nr = nr_St_10_Wait;
      _out->size = size_St_10_Wait;
      _out->act = act_St_10_Wait;
      _out->wt = wt_St_10_Wait;
      _out->ver = ver_St_10_Wait;
      break;
    case Main__St_10_Hw2:
      ns_St_10_Hw2 = Main__St_10_Hw2;
      nr_St_10_Hw2 = false;
      size_St_10_Hw2 = 2;
      act_St_10_Hw2 = true;
      wt_St_10_Hw2 = false;
      ver_St_10_Hw2 = Main__H2;
      ns = ns_St_10_Hw2;
      nr = nr_St_10_Hw2;
      _out->size = size_St_10_Hw2;
      _out->act = act_St_10_Hw2;
      _out->wt = wt_St_10_Hw2;
      _out->ver = ver_St_10_Hw2;
      break;
    case Main__St_10_Hw1:
      ns_St_10_Hw1 = Main__St_10_Hw1;
      nr_St_10_Hw1 = false;
      size_St_10_Hw1 = 2;
      act_St_10_Hw1 = true;
      wt_St_10_Hw1 = false;
      ver_St_10_Hw1 = Main__H1;
      ns = ns_St_10_Hw1;
      nr = nr_St_10_Hw1;
      _out->size = size_St_10_Hw1;
      _out->act = act_St_10_Hw1;
      _out->wt = wt_St_10_Hw1;
      _out->ver = ver_St_10_Hw1;
      break;
    case Main__St_10_Sw:
      ns_St_10_Sw = Main__St_10_Sw;
      nr_St_10_Sw = false;
      size_St_10_Sw = 0;
      act_St_10_Sw = true;
      wt_St_10_Sw = false;
      ver_St_10_Sw = Main__S;
      ns = ns_St_10_Sw;
      nr = nr_St_10_Sw;
      _out->size = size_St_10_Sw;
      _out->act = act_St_10_Sw;
      _out->wt = wt_St_10_Sw;
      _out->ver = ver_St_10_Sw;
      break;
    case Main__St_10_Inactive:
      ns_St_10_Inactive = Main__St_10_Inactive;
      ns = ns_St_10_Inactive;
      nr_St_10_Inactive = false;
      nr = nr_St_10_Inactive;
      size_St_10_Inactive = 0;
      _out->size = size_St_10_Inactive;
      act_St_10_Inactive = false;
      _out->act = act_St_10_Inactive;
      wt_St_10_Inactive = false;
      _out->wt = wt_St_10_Inactive;
      ver_St_10_Inactive = Main__N;
      _out->ver = ver_St_10_Inactive;
      break;
    default:
      break;
  };
  self->ck = ns;
  self->pnr = nr;;
}

void Main__tracking_reset(Main__tracking_mem* self) {
  self->ck = Main__St_11_Inactive;
  self->pnr = false;
}

void Main__tracking_step(int r, int c1, int c2, int c3, int e,
                         Main__tracking_out* _out, Main__tracking_mem* self) {
  
  int v_845;
  Main__st_11 v_844;
  int v_843;
  Main__st_11 v_842;
  int v_841;
  Main__st_11 v;
  int v_860;
  Main__st_11 v_859;
  int v_858;
  Main__st_11 v_857;
  int v_856;
  Main__st_11 v_855;
  int v_854;
  int v_853;
  int v_852;
  int v_851;
  int v_850;
  int v_849;
  int v_848;
  int v_847;
  int v_846;
  int v_875;
  Main__st_11 v_874;
  int v_873;
  Main__st_11 v_872;
  int v_871;
  Main__st_11 v_870;
  int v_869;
  int v_868;
  int v_867;
  int v_866;
  int v_865;
  int v_864;
  int v_863;
  int v_862;
  int v_861;
  int v_890;
  Main__st_11 v_889;
  int v_888;
  Main__st_11 v_887;
  int v_886;
  Main__st_11 v_885;
  int v_884;
  int v_883;
  int v_882;
  int v_881;
  int v_880;
  int v_879;
  int v_878;
  int v_877;
  int v_876;
  int v_905;
  Main__st_11 v_904;
  int v_903;
  Main__st_11 v_902;
  int v_901;
  Main__st_11 v_900;
  int v_899;
  int v_898;
  int v_897;
  int v_896;
  int v_895;
  int v_894;
  int v_893;
  int v_892;
  int v_891;
  int r_21_St_11_Wait;
  Main__st_11 s_St_11_Wait;
  int r_21_St_11_Hw2;
  Main__st_11 s_St_11_Hw2;
  int r_21_St_11_Hw1;
  Main__st_11 s_St_11_Hw1;
  int r_21_St_11_Sw;
  Main__st_11 s_St_11_Sw;
  int r_21_St_11_Inactive;
  Main__st_11 s_St_11_Inactive;
  int nr_St_11_Wait;
  Main__st_11 ns_St_11_Wait;
  Main__version_type ver_St_11_Wait;
  int wt_St_11_Wait;
  int act_St_11_Wait;
  int size_St_11_Wait;
  int nr_St_11_Hw2;
  Main__st_11 ns_St_11_Hw2;
  Main__version_type ver_St_11_Hw2;
  int wt_St_11_Hw2;
  int act_St_11_Hw2;
  int size_St_11_Hw2;
  int nr_St_11_Hw1;
  Main__st_11 ns_St_11_Hw1;
  Main__version_type ver_St_11_Hw1;
  int wt_St_11_Hw1;
  int act_St_11_Hw1;
  int size_St_11_Hw1;
  int nr_St_11_Sw;
  Main__st_11 ns_St_11_Sw;
  Main__version_type ver_St_11_Sw;
  int wt_St_11_Sw;
  int act_St_11_Sw;
  int size_St_11_Sw;
  int nr_St_11_Inactive;
  Main__st_11 ns_St_11_Inactive;
  Main__version_type ver_St_11_Inactive;
  int wt_St_11_Inactive;
  int act_St_11_Inactive;
  int size_St_11_Inactive;
  Main__st_11 ck_12;
  Main__st_11 s;
  Main__st_11 ns;
  int r_21;
  int nr;
  switch (self->ck) {
    case Main__St_11_Wait:
      if (c3) {
        v = Main__St_11_Sw;
      } else {
        v = Main__St_11_Wait;
      };
      if (c2) {
        v_842 = Main__St_11_Hw2;
      } else {
        v_842 = v;
      };
      if (c1) {
        v_844 = Main__St_11_Hw1;
      } else {
        v_844 = v_842;
      };
      if (e) {
        s_St_11_Wait = Main__St_11_Inactive;
      } else {
        s_St_11_Wait = v_844;
      };
      if (c3) {
        v_841 = true;
      } else {
        v_841 = self->pnr;
      };
      if (c2) {
        v_843 = true;
      } else {
        v_843 = v_841;
      };
      if (c1) {
        v_845 = true;
      } else {
        v_845 = v_843;
      };
      if (e) {
        r_21_St_11_Wait = true;
      } else {
        r_21_St_11_Wait = v_845;
      };
      s = s_St_11_Wait;
      r_21 = r_21_St_11_Wait;
      break;
    case Main__St_11_Hw2:
      v_846 = !(c2);
      v_847 = (v_846&&c1);
      v_848 = !(c2);
      v_849 = (v_848&&c3);
      v_850 = !(c2);
      v_851 = !(c1);
      v_852 = (v_850&&v_851);
      v_853 = !(c3);
      v_854 = (v_852&&v_853);
      if (v_854) {
        v_855 = Main__St_11_Wait;
      } else {
        v_855 = Main__St_11_Hw2;
      };
      if (v_849) {
        v_857 = Main__St_11_Sw;
      } else {
        v_857 = v_855;
      };
      if (v_847) {
        v_859 = Main__St_11_Hw1;
      } else {
        v_859 = v_857;
      };
      if (e) {
        s_St_11_Hw2 = Main__St_11_Inactive;
      } else {
        s_St_11_Hw2 = v_859;
      };
      if (v_854) {
        v_856 = true;
      } else {
        v_856 = self->pnr;
      };
      if (v_849) {
        v_858 = true;
      } else {
        v_858 = v_856;
      };
      if (v_847) {
        v_860 = true;
      } else {
        v_860 = v_858;
      };
      if (e) {
        r_21_St_11_Hw2 = true;
      } else {
        r_21_St_11_Hw2 = v_860;
      };
      s = s_St_11_Hw2;
      r_21 = r_21_St_11_Hw2;
      break;
    case Main__St_11_Hw1:
      v_861 = !(c1);
      v_862 = (v_861&&c2);
      v_863 = !(c2);
      v_864 = (v_863&&c3);
      v_865 = !(c2);
      v_866 = !(c1);
      v_867 = (v_865&&v_866);
      v_868 = !(c3);
      v_869 = (v_867&&v_868);
      if (v_869) {
        v_870 = Main__St_11_Wait;
      } else {
        v_870 = Main__St_11_Hw1;
      };
      if (v_864) {
        v_872 = Main__St_11_Sw;
      } else {
        v_872 = v_870;
      };
      if (v_862) {
        v_874 = Main__St_11_Hw2;
      } else {
        v_874 = v_872;
      };
      if (e) {
        s_St_11_Hw1 = Main__St_11_Inactive;
      } else {
        s_St_11_Hw1 = v_874;
      };
      if (v_869) {
        v_871 = true;
      } else {
        v_871 = self->pnr;
      };
      if (v_864) {
        v_873 = true;
      } else {
        v_873 = v_871;
      };
      if (v_862) {
        v_875 = true;
      } else {
        v_875 = v_873;
      };
      if (e) {
        r_21_St_11_Hw1 = true;
      } else {
        r_21_St_11_Hw1 = v_875;
      };
      s = s_St_11_Hw1;
      r_21 = r_21_St_11_Hw1;
      break;
    case Main__St_11_Sw:
      v_876 = !(c3);
      v_877 = (v_876&&c1);
      v_878 = !(c3);
      v_879 = (v_878&&c2);
      v_880 = !(c3);
      v_881 = !(c2);
      v_882 = (v_880&&v_881);
      v_883 = !(c1);
      v_884 = (v_882&&v_883);
      if (v_884) {
        v_885 = Main__St_11_Wait;
      } else {
        v_885 = Main__St_11_Sw;
      };
      if (v_879) {
        v_887 = Main__St_11_Hw2;
      } else {
        v_887 = v_885;
      };
      if (v_877) {
        v_889 = Main__St_11_Hw1;
      } else {
        v_889 = v_887;
      };
      if (e) {
        s_St_11_Sw = Main__St_11_Inactive;
      } else {
        s_St_11_Sw = v_889;
      };
      if (v_884) {
        v_886 = true;
      } else {
        v_886 = self->pnr;
      };
      if (v_879) {
        v_888 = true;
      } else {
        v_888 = v_886;
      };
      if (v_877) {
        v_890 = true;
      } else {
        v_890 = v_888;
      };
      if (e) {
        r_21_St_11_Sw = true;
      } else {
        r_21_St_11_Sw = v_890;
      };
      s = s_St_11_Sw;
      r_21 = r_21_St_11_Sw;
      break;
    case Main__St_11_Inactive:
      v_891 = (r&&c1);
      v_892 = (r&&c2);
      v_893 = (r&&c3);
      v_894 = !(c2);
      v_895 = (r&&v_894);
      v_896 = !(c1);
      v_897 = (v_895&&v_896);
      v_898 = !(c3);
      v_899 = (v_897&&v_898);
      if (v_899) {
        v_900 = Main__St_11_Wait;
      } else {
        v_900 = Main__St_11_Inactive;
      };
      if (v_893) {
        v_902 = Main__St_11_Sw;
      } else {
        v_902 = v_900;
      };
      if (v_892) {
        v_904 = Main__St_11_Hw2;
      } else {
        v_904 = v_902;
      };
      if (v_891) {
        s_St_11_Inactive = Main__St_11_Hw1;
      } else {
        s_St_11_Inactive = v_904;
      };
      s = s_St_11_Inactive;
      if (v_899) {
        v_901 = true;
      } else {
        v_901 = self->pnr;
      };
      if (v_893) {
        v_903 = true;
      } else {
        v_903 = v_901;
      };
      if (v_892) {
        v_905 = true;
      } else {
        v_905 = v_903;
      };
      if (v_891) {
        r_21_St_11_Inactive = true;
      } else {
        r_21_St_11_Inactive = v_905;
      };
      r_21 = r_21_St_11_Inactive;
      break;
    default:
      break;
  };
  ck_12 = s;
  switch (ck_12) {
    case Main__St_11_Wait:
      ns_St_11_Wait = Main__St_11_Wait;
      nr_St_11_Wait = false;
      size_St_11_Wait = 0;
      act_St_11_Wait = false;
      wt_St_11_Wait = true;
      ver_St_11_Wait = Main__N;
      ns = ns_St_11_Wait;
      nr = nr_St_11_Wait;
      _out->size = size_St_11_Wait;
      _out->act = act_St_11_Wait;
      _out->wt = wt_St_11_Wait;
      _out->ver = ver_St_11_Wait;
      break;
    case Main__St_11_Hw2:
      ns_St_11_Hw2 = Main__St_11_Hw2;
      nr_St_11_Hw2 = false;
      size_St_11_Hw2 = 2;
      act_St_11_Hw2 = true;
      wt_St_11_Hw2 = false;
      ver_St_11_Hw2 = Main__H2;
      ns = ns_St_11_Hw2;
      nr = nr_St_11_Hw2;
      _out->size = size_St_11_Hw2;
      _out->act = act_St_11_Hw2;
      _out->wt = wt_St_11_Hw2;
      _out->ver = ver_St_11_Hw2;
      break;
    case Main__St_11_Hw1:
      ns_St_11_Hw1 = Main__St_11_Hw1;
      nr_St_11_Hw1 = false;
      size_St_11_Hw1 = 2;
      act_St_11_Hw1 = true;
      wt_St_11_Hw1 = false;
      ver_St_11_Hw1 = Main__H1;
      ns = ns_St_11_Hw1;
      nr = nr_St_11_Hw1;
      _out->size = size_St_11_Hw1;
      _out->act = act_St_11_Hw1;
      _out->wt = wt_St_11_Hw1;
      _out->ver = ver_St_11_Hw1;
      break;
    case Main__St_11_Sw:
      ns_St_11_Sw = Main__St_11_Sw;
      nr_St_11_Sw = false;
      size_St_11_Sw = 0;
      act_St_11_Sw = true;
      wt_St_11_Sw = false;
      ver_St_11_Sw = Main__S;
      ns = ns_St_11_Sw;
      nr = nr_St_11_Sw;
      _out->size = size_St_11_Sw;
      _out->act = act_St_11_Sw;
      _out->wt = wt_St_11_Sw;
      _out->ver = ver_St_11_Sw;
      break;
    case Main__St_11_Inactive:
      ns_St_11_Inactive = Main__St_11_Inactive;
      ns = ns_St_11_Inactive;
      nr_St_11_Inactive = false;
      nr = nr_St_11_Inactive;
      size_St_11_Inactive = 0;
      _out->size = size_St_11_Inactive;
      act_St_11_Inactive = false;
      _out->act = act_St_11_Inactive;
      wt_St_11_Inactive = false;
      _out->wt = wt_St_11_Inactive;
      ver_St_11_Inactive = Main__N;
      _out->ver = ver_St_11_Inactive;
      break;
    default:
      break;
  };
  self->ck = ns;
  self->pnr = nr;;
}

void Main__main_reset(Main__main_mem* self) {
  self->v_973 = 0;
  self->v_980 = 0;
  self->v_991 = 0;
  self->v_1000 = 0;
  self->v_1007 = 0;
  self->v_1018 = 0;
  self->v_1027 = 0;
  self->v_1034 = 0;
  self->v_1045 = 0;
  self->ck_23 = Main__St_25_Inactive;
  self->pnr_13 = false;
  self->ck_25 = Main__St_24_Inactive;
  self->pnr_12 = false;
  self->ck_27 = Main__St_23_Inactive;
  self->pnr_11 = false;
  self->ck_29 = Main__St_22_Inactive;
  self->pnr_10 = false;
  self->ck_31 = Main__St_21_Inactive;
  self->pnr_9 = false;
  self->ck_33 = Main__St_20_Inactive;
  self->pnr_8 = false;
  self->ck_35 = Main__St_19_Inactive;
  self->pnr_7 = false;
  self->ck_37 = Main__St_18_Inactive;
  self->pnr_6 = false;
  self->ck_39 = Main__St_17_Inactive;
  self->pnr_5 = false;
  self->ck_41 = Main__St_16_Inactive;
  self->pnr_4 = false;
  self->ck_43 = Main__St_15_Inactive;
  self->pnr_3 = false;
  self->ck_45 = Main__St_14_Free;
  self->pnr_2 = false;
  self->ck_47 = Main__St_13_Free;
  self->pnr_1 = false;
  self->ck_49 = Main__St_12_Free;
  self->pnr = false;
}

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
                     Main__main_out* _out, Main__main_mem* self) {
  Main_controller__main_ctrlr0_out Main_controller__main_ctrlr0_out_st;
  
  int c;
  int cUp1;
  int cKp1;
  int cDw1;
  int cUp2;
  int cKp2;
  int cDw2;
  int cUp3;
  int cKp3;
  int cDw3;
  int ct4;
  int cUp4;
  int cKp4;
  int cDw4;
  int ct5;
  int cUp5;
  int cKp5;
  int cDw5;
  int ct6;
  int cUp6;
  int cKp6;
  int cDw6;
  int cUp7;
  int cKp7;
  int cDw7;
  int cUp8;
  int cKp8;
  int cDw8;
  int cUp9;
  int cKp9;
  int cDw9;
  int ct10;
  int cUp10;
  int cKp10;
  int cDw10;
  int ct11;
  int cUp11;
  int cKp11;
  int cDw11;
  int c_img_c1;
  int c_img_c2;
  int c_img_c3;
  int c_img_c4;
  int me_imu_c;
  int me_img_c1;
  int me_img_c2;
  int me_img_c3;
  int me_img_c4;
  int sl_c1;
  int sl_c2;
  int sl_c3;
  int sl_c4;
  int oa_c1;
  int oa_c2;
  int oa_c3;
  int oa_c4;
  int tl_c1;
  int tl_c2;
  int tl_c3;
  int tl_c4;
  int rs_c1;
  int rs_c2;
  int rs_c3;
  int rs_c4;
  int rb_c1;
  int rb_c2;
  int rpl_c1;
  int rpl_c2;
  int rpl_c3;
  int dt_c1;
  int dt_c2;
  int dt_c3;
  int trk_c1;
  int trk_c2;
  int trk_c3;
  int cr1;
  int cr2;
  int cr3;
  int wcet1_N;
  int wcet1_H3;
  int wcet1_H2;
  int wcet1_H1;
  int wcet1_S;
  Main__version_type ck;
  int wcet2_N;
  int wcet2_H3;
  int wcet2_H2;
  int wcet2_H1;
  int wcet2_S;
  Main__version_type ck_13;
  int wcet3_N;
  int wcet3_H3;
  int wcet3_H2;
  int wcet3_H1;
  int wcet3_S;
  Main__version_type ck_14;
  int wcet4_N;
  int wcet4_H3;
  int wcet4_H2;
  int wcet4_H1;
  int wcet4_S;
  Main__version_type ck_15;
  int wcet5_N;
  int wcet5_H3;
  int wcet5_H2;
  int wcet5_H1;
  int wcet5_S;
  Main__version_type ck_16;
  int wcet6_N;
  int wcet6_H3;
  int wcet6_H2;
  int wcet6_H1;
  int wcet6_S;
  Main__version_type ck_17;
  int wcet7_N;
  int wcet7_H3;
  int wcet7_H2;
  int wcet7_H1;
  int wcet7_S;
  Main__version_type ck_18;
  int wcet8_N;
  int wcet8_H3;
  int wcet8_H2;
  int wcet8_H1;
  int wcet8_S;
  Main__version_type ck_19;
  int wcet9_N;
  int wcet9_H3;
  int wcet9_H2;
  int wcet9_H1;
  int wcet9_S;
  Main__version_type ck_20;
  int wcet10_N;
  int wcet10_H3;
  int wcet10_H2;
  int wcet10_H1;
  int wcet10_S;
  Main__version_type ck_21;
  int wcet11_N;
  int wcet11_H3;
  int wcet11_H2;
  int wcet11_H1;
  int wcet11_S;
  Main__version_type ck_22;
  int v_1054;
  Main__st_25 v_1053;
  int v_1052;
  Main__st_25 v_1051;
  int v_1050;
  Main__st_25 v_1049;
  int v_1069;
  Main__st_25 v_1068;
  int v_1067;
  Main__st_25 v_1066;
  int v_1065;
  Main__st_25 v_1064;
  int v_1063;
  int v_1062;
  int v_1061;
  int v_1060;
  int v_1059;
  int v_1058;
  int v_1057;
  int v_1056;
  int v_1055;
  int v_1084;
  Main__st_25 v_1083;
  int v_1082;
  Main__st_25 v_1081;
  int v_1080;
  Main__st_25 v_1079;
  int v_1078;
  int v_1077;
  int v_1076;
  int v_1075;
  int v_1074;
  int v_1073;
  int v_1072;
  int v_1071;
  int v_1070;
  int v_1099;
  Main__st_25 v_1098;
  int v_1097;
  Main__st_25 v_1096;
  int v_1095;
  Main__st_25 v_1094;
  int v_1093;
  int v_1092;
  int v_1091;
  int v_1090;
  int v_1089;
  int v_1088;
  int v_1087;
  int v_1086;
  int v_1085;
  int v_1114;
  Main__st_25 v_1113;
  int v_1112;
  Main__st_25 v_1111;
  int v_1110;
  Main__st_25 v_1109;
  int v_1108;
  int v_1107;
  int v_1106;
  int v_1105;
  int v_1104;
  int v_1103;
  int v_1102;
  int v_1101;
  int v_1100;
  int r_35_St_25_Wait;
  Main__st_25 s_13_St_25_Wait;
  int r_35_St_25_Hw2;
  Main__st_25 s_13_St_25_Hw2;
  int r_35_St_25_Hw1;
  Main__st_25 s_13_St_25_Hw1;
  int r_35_St_25_Sw;
  Main__st_25 s_13_St_25_Sw;
  int r_35_St_25_Inactive;
  Main__st_25 s_13_St_25_Inactive;
  int nr_13_St_25_Wait;
  Main__st_25 ns_13_St_25_Wait;
  Main__version_type ver_St_25_Wait;
  int wt_St_25_Wait;
  int act_St_25_Wait;
  int size_St_25_Wait;
  int nr_13_St_25_Hw2;
  Main__st_25 ns_13_St_25_Hw2;
  Main__version_type ver_St_25_Hw2;
  int wt_St_25_Hw2;
  int act_St_25_Hw2;
  int size_St_25_Hw2;
  int nr_13_St_25_Hw1;
  Main__st_25 ns_13_St_25_Hw1;
  Main__version_type ver_St_25_Hw1;
  int wt_St_25_Hw1;
  int act_St_25_Hw1;
  int size_St_25_Hw1;
  int nr_13_St_25_Sw;
  Main__st_25 ns_13_St_25_Sw;
  Main__version_type ver_St_25_Sw;
  int wt_St_25_Sw;
  int act_St_25_Sw;
  int size_St_25_Sw;
  int nr_13_St_25_Inactive;
  Main__st_25 ns_13_St_25_Inactive;
  Main__version_type ver_St_25_Inactive;
  int wt_St_25_Inactive;
  int act_St_25_Inactive;
  int size_St_25_Inactive;
  Main__st_25 ck_24;
  int v_1120;
  Main__st_24 v_1119;
  int v_1118;
  Main__st_24 v_1117;
  int v_1116;
  Main__st_24 v_1115;
  int v_1135;
  Main__st_24 v_1134;
  int v_1133;
  Main__st_24 v_1132;
  int v_1131;
  Main__st_24 v_1130;
  int v_1129;
  int v_1128;
  int v_1127;
  int v_1126;
  int v_1125;
  int v_1124;
  int v_1123;
  int v_1122;
  int v_1121;
  int v_1150;
  Main__st_24 v_1149;
  int v_1148;
  Main__st_24 v_1147;
  int v_1146;
  Main__st_24 v_1145;
  int v_1144;
  int v_1143;
  int v_1142;
  int v_1141;
  int v_1140;
  int v_1139;
  int v_1138;
  int v_1137;
  int v_1136;
  int v_1165;
  Main__st_24 v_1164;
  int v_1163;
  Main__st_24 v_1162;
  int v_1161;
  Main__st_24 v_1160;
  int v_1159;
  int v_1158;
  int v_1157;
  int v_1156;
  int v_1155;
  int v_1154;
  int v_1153;
  int v_1152;
  int v_1151;
  int v_1180;
  Main__st_24 v_1179;
  int v_1178;
  Main__st_24 v_1177;
  int v_1176;
  Main__st_24 v_1175;
  int v_1174;
  int v_1173;
  int v_1172;
  int v_1171;
  int v_1170;
  int v_1169;
  int v_1168;
  int v_1167;
  int v_1166;
  int r_34_St_24_Wait;
  Main__st_24 s_12_St_24_Wait;
  int r_34_St_24_Hw2;
  Main__st_24 s_12_St_24_Hw2;
  int r_34_St_24_Hw1;
  Main__st_24 s_12_St_24_Hw1;
  int r_34_St_24_Sw;
  Main__st_24 s_12_St_24_Sw;
  int r_34_St_24_Inactive;
  Main__st_24 s_12_St_24_Inactive;
  int nr_12_St_24_Wait;
  Main__st_24 ns_12_St_24_Wait;
  Main__version_type ver_1_St_24_Wait;
  int wt_1_St_24_Wait;
  int act_4_St_24_Wait;
  int size_1_St_24_Wait;
  int nr_12_St_24_Hw2;
  Main__st_24 ns_12_St_24_Hw2;
  Main__version_type ver_1_St_24_Hw2;
  int wt_1_St_24_Hw2;
  int act_4_St_24_Hw2;
  int size_1_St_24_Hw2;
  int nr_12_St_24_Hw1;
  Main__st_24 ns_12_St_24_Hw1;
  Main__version_type ver_1_St_24_Hw1;
  int wt_1_St_24_Hw1;
  int act_4_St_24_Hw1;
  int size_1_St_24_Hw1;
  int nr_12_St_24_Sw;
  Main__st_24 ns_12_St_24_Sw;
  Main__version_type ver_1_St_24_Sw;
  int wt_1_St_24_Sw;
  int act_4_St_24_Sw;
  int size_1_St_24_Sw;
  int nr_12_St_24_Inactive;
  Main__st_24 ns_12_St_24_Inactive;
  Main__version_type ver_1_St_24_Inactive;
  int wt_1_St_24_Inactive;
  int act_4_St_24_Inactive;
  int size_1_St_24_Inactive;
  Main__st_24 ck_26;
  int v_1186;
  Main__st_23 v_1185;
  int v_1184;
  Main__st_23 v_1183;
  int v_1182;
  Main__st_23 v_1181;
  int v_1201;
  Main__st_23 v_1200;
  int v_1199;
  Main__st_23 v_1198;
  int v_1197;
  Main__st_23 v_1196;
  int v_1195;
  int v_1194;
  int v_1193;
  int v_1192;
  int v_1191;
  int v_1190;
  int v_1189;
  int v_1188;
  int v_1187;
  int v_1216;
  Main__st_23 v_1215;
  int v_1214;
  Main__st_23 v_1213;
  int v_1212;
  Main__st_23 v_1211;
  int v_1210;
  int v_1209;
  int v_1208;
  int v_1207;
  int v_1206;
  int v_1205;
  int v_1204;
  int v_1203;
  int v_1202;
  int v_1231;
  Main__st_23 v_1230;
  int v_1229;
  Main__st_23 v_1228;
  int v_1227;
  Main__st_23 v_1226;
  int v_1225;
  int v_1224;
  int v_1223;
  int v_1222;
  int v_1221;
  int v_1220;
  int v_1219;
  int v_1218;
  int v_1217;
  int v_1246;
  Main__st_23 v_1245;
  int v_1244;
  Main__st_23 v_1243;
  int v_1242;
  Main__st_23 v_1241;
  int v_1240;
  int v_1239;
  int v_1238;
  int v_1237;
  int v_1236;
  int v_1235;
  int v_1234;
  int v_1233;
  int v_1232;
  int r_33_St_23_Wait;
  Main__st_23 s_11_St_23_Wait;
  int r_33_St_23_Hw2;
  Main__st_23 s_11_St_23_Hw2;
  int r_33_St_23_Hw1;
  Main__st_23 s_11_St_23_Hw1;
  int r_33_St_23_Sw;
  Main__st_23 s_11_St_23_Sw;
  int r_33_St_23_Inactive;
  Main__st_23 s_11_St_23_Inactive;
  int nr_11_St_23_Wait;
  Main__st_23 ns_11_St_23_Wait;
  Main__version_type ver_2_St_23_Wait;
  int wt_2_St_23_Wait;
  int act_5_St_23_Wait;
  int size_2_St_23_Wait;
  int nr_11_St_23_Hw2;
  Main__st_23 ns_11_St_23_Hw2;
  Main__version_type ver_2_St_23_Hw2;
  int wt_2_St_23_Hw2;
  int act_5_St_23_Hw2;
  int size_2_St_23_Hw2;
  int nr_11_St_23_Hw1;
  Main__st_23 ns_11_St_23_Hw1;
  Main__version_type ver_2_St_23_Hw1;
  int wt_2_St_23_Hw1;
  int act_5_St_23_Hw1;
  int size_2_St_23_Hw1;
  int nr_11_St_23_Sw;
  Main__st_23 ns_11_St_23_Sw;
  Main__version_type ver_2_St_23_Sw;
  int wt_2_St_23_Sw;
  int act_5_St_23_Sw;
  int size_2_St_23_Sw;
  int nr_11_St_23_Inactive;
  Main__st_23 ns_11_St_23_Inactive;
  Main__version_type ver_2_St_23_Inactive;
  int wt_2_St_23_Inactive;
  int act_5_St_23_Inactive;
  int size_2_St_23_Inactive;
  Main__st_23 ck_28;
  int v_1250;
  Main__st_22 v_1249;
  int v_1248;
  Main__st_22 v_1247;
  int v_1259;
  Main__st_22 v_1258;
  int v_1257;
  Main__st_22 v_1256;
  int v_1255;
  int v_1254;
  int v_1253;
  int v_1252;
  int v_1251;
  int v_1268;
  Main__st_22 v_1267;
  int v_1266;
  Main__st_22 v_1265;
  int v_1264;
  int v_1263;
  int v_1262;
  int v_1261;
  int v_1260;
  int v_1278;
  Main__st_22 v_1277;
  int v_1276;
  Main__st_22 v_1275;
  int v_1274;
  int v_1273;
  int v_1272;
  int v_1271;
  int v_1270;
  int v_1269;
  int r_32_St_22_Wait;
  Main__st_22 s_10_St_22_Wait;
  int r_32_St_22_Hw2;
  Main__st_22 s_10_St_22_Hw2;
  int r_32_St_22_Hw1;
  Main__st_22 s_10_St_22_Hw1;
  int r_32_St_22_Inactive;
  Main__st_22 s_10_St_22_Inactive;
  int nr_10_St_22_Wait;
  Main__st_22 ns_10_St_22_Wait;
  Main__version_type ver_3_St_22_Wait;
  int wt_3_St_22_Wait;
  int act_6_St_22_Wait;
  int size_3_St_22_Wait;
  int nr_10_St_22_Hw2;
  Main__st_22 ns_10_St_22_Hw2;
  Main__version_type ver_3_St_22_Hw2;
  int wt_3_St_22_Hw2;
  int act_6_St_22_Hw2;
  int size_3_St_22_Hw2;
  int nr_10_St_22_Hw1;
  Main__st_22 ns_10_St_22_Hw1;
  Main__version_type ver_3_St_22_Hw1;
  int wt_3_St_22_Hw1;
  int act_6_St_22_Hw1;
  int size_3_St_22_Hw1;
  int nr_10_St_22_Inactive;
  Main__st_22 ns_10_St_22_Inactive;
  Main__version_type ver_3_St_22_Inactive;
  int wt_3_St_22_Inactive;
  int act_6_St_22_Inactive;
  int size_3_St_22_Inactive;
  Main__st_22 ck_30;
  int v_1286;
  Main__st_21 v_1285;
  int v_1284;
  Main__st_21 v_1283;
  int v_1282;
  Main__st_21 v_1281;
  int v_1280;
  Main__st_21 v_1279;
  int v_1307;
  Main__st_21 v_1306;
  int v_1305;
  Main__st_21 v_1304;
  int v_1303;
  Main__st_21 v_1302;
  int v_1301;
  Main__st_21 v_1300;
  int v_1299;
  int v_1298;
  int v_1297;
  int v_1296;
  int v_1295;
  int v_1294;
  int v_1293;
  int v_1292;
  int v_1291;
  int v_1290;
  int v_1289;
  int v_1288;
  int v_1287;
  int v_1328;
  Main__st_21 v_1327;
  int v_1326;
  Main__st_21 v_1325;
  int v_1324;
  Main__st_21 v_1323;
  int v_1322;
  Main__st_21 v_1321;
  int v_1320;
  int v_1319;
  int v_1318;
  int v_1317;
  int v_1316;
  int v_1315;
  int v_1314;
  int v_1313;
  int v_1312;
  int v_1311;
  int v_1310;
  int v_1309;
  int v_1308;
  int v_1349;
  Main__st_21 v_1348;
  int v_1347;
  Main__st_21 v_1346;
  int v_1345;
  Main__st_21 v_1344;
  int v_1343;
  Main__st_21 v_1342;
  int v_1341;
  int v_1340;
  int v_1339;
  int v_1338;
  int v_1337;
  int v_1336;
  int v_1335;
  int v_1334;
  int v_1333;
  int v_1332;
  int v_1331;
  int v_1330;
  int v_1329;
  int v_1370;
  Main__st_21 v_1369;
  int v_1368;
  Main__st_21 v_1367;
  int v_1366;
  Main__st_21 v_1365;
  int v_1364;
  Main__st_21 v_1363;
  int v_1362;
  int v_1361;
  int v_1360;
  int v_1359;
  int v_1358;
  int v_1357;
  int v_1356;
  int v_1355;
  int v_1354;
  int v_1353;
  int v_1352;
  int v_1351;
  int v_1350;
  int v_1390;
  Main__st_21 v_1389;
  int v_1388;
  Main__st_21 v_1387;
  int v_1386;
  Main__st_21 v_1385;
  int v_1384;
  Main__st_21 v_1383;
  int v_1382;
  int v_1381;
  int v_1380;
  int v_1379;
  int v_1378;
  int v_1377;
  int v_1376;
  int v_1375;
  int v_1374;
  int v_1373;
  int v_1372;
  int v_1371;
  int r_31_St_21_Wait;
  Main__st_21 s_9_St_21_Wait;
  int r_31_St_21_Hw3;
  Main__st_21 s_9_St_21_Hw3;
  int r_31_St_21_Hw2;
  Main__st_21 s_9_St_21_Hw2;
  int r_31_St_21_Hw1;
  Main__st_21 s_9_St_21_Hw1;
  int r_31_St_21_Sw;
  Main__st_21 s_9_St_21_Sw;
  int r_31_St_21_Inactive;
  Main__st_21 s_9_St_21_Inactive;
  int nr_9_St_21_Wait;
  Main__st_21 ns_9_St_21_Wait;
  Main__version_type ver_4_St_21_Wait;
  int wt_4_St_21_Wait;
  int act_7_St_21_Wait;
  int size_4_St_21_Wait;
  int nr_9_St_21_Hw3;
  Main__st_21 ns_9_St_21_Hw3;
  Main__version_type ver_4_St_21_Hw3;
  int wt_4_St_21_Hw3;
  int act_7_St_21_Hw3;
  int size_4_St_21_Hw3;
  int nr_9_St_21_Hw2;
  Main__st_21 ns_9_St_21_Hw2;
  Main__version_type ver_4_St_21_Hw2;
  int wt_4_St_21_Hw2;
  int act_7_St_21_Hw2;
  int size_4_St_21_Hw2;
  int nr_9_St_21_Hw1;
  Main__st_21 ns_9_St_21_Hw1;
  Main__version_type ver_4_St_21_Hw1;
  int wt_4_St_21_Hw1;
  int act_7_St_21_Hw1;
  int size_4_St_21_Hw1;
  int nr_9_St_21_Sw;
  Main__st_21 ns_9_St_21_Sw;
  Main__version_type ver_4_St_21_Sw;
  int wt_4_St_21_Sw;
  int act_7_St_21_Sw;
  int size_4_St_21_Sw;
  int nr_9_St_21_Inactive;
  Main__st_21 ns_9_St_21_Inactive;
  Main__version_type ver_4_St_21_Inactive;
  int wt_4_St_21_Inactive;
  int act_7_St_21_Inactive;
  int size_4_St_21_Inactive;
  Main__st_21 ck_32;
  int v_1398;
  Main__st_20 v_1397;
  int v_1396;
  Main__st_20 v_1395;
  int v_1394;
  Main__st_20 v_1393;
  int v_1392;
  Main__st_20 v_1391;
  int v_1419;
  Main__st_20 v_1418;
  int v_1417;
  Main__st_20 v_1416;
  int v_1415;
  Main__st_20 v_1414;
  int v_1413;
  Main__st_20 v_1412;
  int v_1411;
  int v_1410;
  int v_1409;
  int v_1408;
  int v_1407;
  int v_1406;
  int v_1405;
  int v_1404;
  int v_1403;
  int v_1402;
  int v_1401;
  int v_1400;
  int v_1399;
  int v_1440;
  Main__st_20 v_1439;
  int v_1438;
  Main__st_20 v_1437;
  int v_1436;
  Main__st_20 v_1435;
  int v_1434;
  Main__st_20 v_1433;
  int v_1432;
  int v_1431;
  int v_1430;
  int v_1429;
  int v_1428;
  int v_1427;
  int v_1426;
  int v_1425;
  int v_1424;
  int v_1423;
  int v_1422;
  int v_1421;
  int v_1420;
  int v_1461;
  Main__st_20 v_1460;
  int v_1459;
  Main__st_20 v_1458;
  int v_1457;
  Main__st_20 v_1456;
  int v_1455;
  Main__st_20 v_1454;
  int v_1453;
  int v_1452;
  int v_1451;
  int v_1450;
  int v_1449;
  int v_1448;
  int v_1447;
  int v_1446;
  int v_1445;
  int v_1444;
  int v_1443;
  int v_1442;
  int v_1441;
  int v_1482;
  Main__st_20 v_1481;
  int v_1480;
  Main__st_20 v_1479;
  int v_1478;
  Main__st_20 v_1477;
  int v_1476;
  Main__st_20 v_1475;
  int v_1474;
  int v_1473;
  int v_1472;
  int v_1471;
  int v_1470;
  int v_1469;
  int v_1468;
  int v_1467;
  int v_1466;
  int v_1465;
  int v_1464;
  int v_1463;
  int v_1462;
  int v_1502;
  Main__st_20 v_1501;
  int v_1500;
  Main__st_20 v_1499;
  int v_1498;
  Main__st_20 v_1497;
  int v_1496;
  Main__st_20 v_1495;
  int v_1494;
  int v_1493;
  int v_1492;
  int v_1491;
  int v_1490;
  int v_1489;
  int v_1488;
  int v_1487;
  int v_1486;
  int v_1485;
  int v_1484;
  int v_1483;
  int r_30_St_20_Wait;
  Main__st_20 s_8_St_20_Wait;
  int r_30_St_20_Hw3;
  Main__st_20 s_8_St_20_Hw3;
  int r_30_St_20_Hw2;
  Main__st_20 s_8_St_20_Hw2;
  int r_30_St_20_Hw1;
  Main__st_20 s_8_St_20_Hw1;
  int r_30_St_20_Sw;
  Main__st_20 s_8_St_20_Sw;
  int r_30_St_20_Inactive;
  Main__st_20 s_8_St_20_Inactive;
  int nr_8_St_20_Wait;
  Main__st_20 ns_8_St_20_Wait;
  Main__version_type ver_5_St_20_Wait;
  int wt_5_St_20_Wait;
  int act_8_St_20_Wait;
  int size_5_St_20_Wait;
  int nr_8_St_20_Hw3;
  Main__st_20 ns_8_St_20_Hw3;
  Main__version_type ver_5_St_20_Hw3;
  int wt_5_St_20_Hw3;
  int act_8_St_20_Hw3;
  int size_5_St_20_Hw3;
  int nr_8_St_20_Hw2;
  Main__st_20 ns_8_St_20_Hw2;
  Main__version_type ver_5_St_20_Hw2;
  int wt_5_St_20_Hw2;
  int act_8_St_20_Hw2;
  int size_5_St_20_Hw2;
  int nr_8_St_20_Hw1;
  Main__st_20 ns_8_St_20_Hw1;
  Main__version_type ver_5_St_20_Hw1;
  int wt_5_St_20_Hw1;
  int act_8_St_20_Hw1;
  int size_5_St_20_Hw1;
  int nr_8_St_20_Sw;
  Main__st_20 ns_8_St_20_Sw;
  Main__version_type ver_5_St_20_Sw;
  int wt_5_St_20_Sw;
  int act_8_St_20_Sw;
  int size_5_St_20_Sw;
  int nr_8_St_20_Inactive;
  Main__st_20 ns_8_St_20_Inactive;
  Main__version_type ver_5_St_20_Inactive;
  int wt_5_St_20_Inactive;
  int act_8_St_20_Inactive;
  int size_5_St_20_Inactive;
  Main__st_20 ck_34;
  int v_1510;
  Main__st_19 v_1509;
  int v_1508;
  Main__st_19 v_1507;
  int v_1506;
  Main__st_19 v_1505;
  int v_1504;
  Main__st_19 v_1503;
  int v_1531;
  Main__st_19 v_1530;
  int v_1529;
  Main__st_19 v_1528;
  int v_1527;
  Main__st_19 v_1526;
  int v_1525;
  Main__st_19 v_1524;
  int v_1523;
  int v_1522;
  int v_1521;
  int v_1520;
  int v_1519;
  int v_1518;
  int v_1517;
  int v_1516;
  int v_1515;
  int v_1514;
  int v_1513;
  int v_1512;
  int v_1511;
  int v_1552;
  Main__st_19 v_1551;
  int v_1550;
  Main__st_19 v_1549;
  int v_1548;
  Main__st_19 v_1547;
  int v_1546;
  Main__st_19 v_1545;
  int v_1544;
  int v_1543;
  int v_1542;
  int v_1541;
  int v_1540;
  int v_1539;
  int v_1538;
  int v_1537;
  int v_1536;
  int v_1535;
  int v_1534;
  int v_1533;
  int v_1532;
  int v_1573;
  Main__st_19 v_1572;
  int v_1571;
  Main__st_19 v_1570;
  int v_1569;
  Main__st_19 v_1568;
  int v_1567;
  Main__st_19 v_1566;
  int v_1565;
  int v_1564;
  int v_1563;
  int v_1562;
  int v_1561;
  int v_1560;
  int v_1559;
  int v_1558;
  int v_1557;
  int v_1556;
  int v_1555;
  int v_1554;
  int v_1553;
  int v_1594;
  Main__st_19 v_1593;
  int v_1592;
  Main__st_19 v_1591;
  int v_1590;
  Main__st_19 v_1589;
  int v_1588;
  Main__st_19 v_1587;
  int v_1586;
  int v_1585;
  int v_1584;
  int v_1583;
  int v_1582;
  int v_1581;
  int v_1580;
  int v_1579;
  int v_1578;
  int v_1577;
  int v_1576;
  int v_1575;
  int v_1574;
  int v_1614;
  Main__st_19 v_1613;
  int v_1612;
  Main__st_19 v_1611;
  int v_1610;
  Main__st_19 v_1609;
  int v_1608;
  Main__st_19 v_1607;
  int v_1606;
  int v_1605;
  int v_1604;
  int v_1603;
  int v_1602;
  int v_1601;
  int v_1600;
  int v_1599;
  int v_1598;
  int v_1597;
  int v_1596;
  int v_1595;
  int r_29_St_19_Wait;
  Main__st_19 s_7_St_19_Wait;
  int r_29_St_19_Hw3;
  Main__st_19 s_7_St_19_Hw3;
  int r_29_St_19_Hw2;
  Main__st_19 s_7_St_19_Hw2;
  int r_29_St_19_Hw1;
  Main__st_19 s_7_St_19_Hw1;
  int r_29_St_19_Sw;
  Main__st_19 s_7_St_19_Sw;
  int r_29_St_19_Inactive;
  Main__st_19 s_7_St_19_Inactive;
  int nr_7_St_19_Wait;
  Main__st_19 ns_7_St_19_Wait;
  Main__version_type ver_6_St_19_Wait;
  int wt_6_St_19_Wait;
  int act_9_St_19_Wait;
  int size_6_St_19_Wait;
  int nr_7_St_19_Hw3;
  Main__st_19 ns_7_St_19_Hw3;
  Main__version_type ver_6_St_19_Hw3;
  int wt_6_St_19_Hw3;
  int act_9_St_19_Hw3;
  int size_6_St_19_Hw3;
  int nr_7_St_19_Hw2;
  Main__st_19 ns_7_St_19_Hw2;
  Main__version_type ver_6_St_19_Hw2;
  int wt_6_St_19_Hw2;
  int act_9_St_19_Hw2;
  int size_6_St_19_Hw2;
  int nr_7_St_19_Hw1;
  Main__st_19 ns_7_St_19_Hw1;
  Main__version_type ver_6_St_19_Hw1;
  int wt_6_St_19_Hw1;
  int act_9_St_19_Hw1;
  int size_6_St_19_Hw1;
  int nr_7_St_19_Sw;
  Main__st_19 ns_7_St_19_Sw;
  Main__version_type ver_6_St_19_Sw;
  int wt_6_St_19_Sw;
  int act_9_St_19_Sw;
  int size_6_St_19_Sw;
  int nr_7_St_19_Inactive;
  Main__st_19 ns_7_St_19_Inactive;
  Main__version_type ver_6_St_19_Inactive;
  int wt_6_St_19_Inactive;
  int act_9_St_19_Inactive;
  int size_6_St_19_Inactive;
  Main__st_19 ck_36;
  int v_1622;
  Main__st_18 v_1621;
  int v_1620;
  Main__st_18 v_1619;
  int v_1618;
  Main__st_18 v_1617;
  int v_1616;
  Main__st_18 v_1615;
  int v_1643;
  Main__st_18 v_1642;
  int v_1641;
  Main__st_18 v_1640;
  int v_1639;
  Main__st_18 v_1638;
  int v_1637;
  Main__st_18 v_1636;
  int v_1635;
  int v_1634;
  int v_1633;
  int v_1632;
  int v_1631;
  int v_1630;
  int v_1629;
  int v_1628;
  int v_1627;
  int v_1626;
  int v_1625;
  int v_1624;
  int v_1623;
  int v_1664;
  Main__st_18 v_1663;
  int v_1662;
  Main__st_18 v_1661;
  int v_1660;
  Main__st_18 v_1659;
  int v_1658;
  Main__st_18 v_1657;
  int v_1656;
  int v_1655;
  int v_1654;
  int v_1653;
  int v_1652;
  int v_1651;
  int v_1650;
  int v_1649;
  int v_1648;
  int v_1647;
  int v_1646;
  int v_1645;
  int v_1644;
  int v_1685;
  Main__st_18 v_1684;
  int v_1683;
  Main__st_18 v_1682;
  int v_1681;
  Main__st_18 v_1680;
  int v_1679;
  Main__st_18 v_1678;
  int v_1677;
  int v_1676;
  int v_1675;
  int v_1674;
  int v_1673;
  int v_1672;
  int v_1671;
  int v_1670;
  int v_1669;
  int v_1668;
  int v_1667;
  int v_1666;
  int v_1665;
  int v_1706;
  Main__st_18 v_1705;
  int v_1704;
  Main__st_18 v_1703;
  int v_1702;
  Main__st_18 v_1701;
  int v_1700;
  Main__st_18 v_1699;
  int v_1698;
  int v_1697;
  int v_1696;
  int v_1695;
  int v_1694;
  int v_1693;
  int v_1692;
  int v_1691;
  int v_1690;
  int v_1689;
  int v_1688;
  int v_1687;
  int v_1686;
  int v_1726;
  Main__st_18 v_1725;
  int v_1724;
  Main__st_18 v_1723;
  int v_1722;
  Main__st_18 v_1721;
  int v_1720;
  Main__st_18 v_1719;
  int v_1718;
  int v_1717;
  int v_1716;
  int v_1715;
  int v_1714;
  int v_1713;
  int v_1712;
  int v_1711;
  int v_1710;
  int v_1709;
  int v_1708;
  int v_1707;
  int r_28_St_18_Wait;
  Main__st_18 s_6_St_18_Wait;
  int r_28_St_18_Hw3;
  Main__st_18 s_6_St_18_Hw3;
  int r_28_St_18_Hw2;
  Main__st_18 s_6_St_18_Hw2;
  int r_28_St_18_Hw1;
  Main__st_18 s_6_St_18_Hw1;
  int r_28_St_18_Sw;
  Main__st_18 s_6_St_18_Sw;
  int r_28_St_18_Inactive;
  Main__st_18 s_6_St_18_Inactive;
  int nr_6_St_18_Wait;
  Main__st_18 ns_6_St_18_Wait;
  Main__version_type ver_7_St_18_Wait;
  int wt_7_St_18_Wait;
  int act_10_St_18_Wait;
  int size_7_St_18_Wait;
  int nr_6_St_18_Hw3;
  Main__st_18 ns_6_St_18_Hw3;
  Main__version_type ver_7_St_18_Hw3;
  int wt_7_St_18_Hw3;
  int act_10_St_18_Hw3;
  int size_7_St_18_Hw3;
  int nr_6_St_18_Hw2;
  Main__st_18 ns_6_St_18_Hw2;
  Main__version_type ver_7_St_18_Hw2;
  int wt_7_St_18_Hw2;
  int act_10_St_18_Hw2;
  int size_7_St_18_Hw2;
  int nr_6_St_18_Hw1;
  Main__st_18 ns_6_St_18_Hw1;
  Main__version_type ver_7_St_18_Hw1;
  int wt_7_St_18_Hw1;
  int act_10_St_18_Hw1;
  int size_7_St_18_Hw1;
  int nr_6_St_18_Sw;
  Main__st_18 ns_6_St_18_Sw;
  Main__version_type ver_7_St_18_Sw;
  int wt_7_St_18_Sw;
  int act_10_St_18_Sw;
  int size_7_St_18_Sw;
  int nr_6_St_18_Inactive;
  Main__st_18 ns_6_St_18_Inactive;
  Main__version_type ver_7_St_18_Inactive;
  int wt_7_St_18_Inactive;
  int act_10_St_18_Inactive;
  int size_7_St_18_Inactive;
  Main__st_18 ck_38;
  int v_1734;
  Main__st_17 v_1733;
  int v_1732;
  Main__st_17 v_1731;
  int v_1730;
  Main__st_17 v_1729;
  int v_1728;
  Main__st_17 v_1727;
  int v_1755;
  Main__st_17 v_1754;
  int v_1753;
  Main__st_17 v_1752;
  int v_1751;
  Main__st_17 v_1750;
  int v_1749;
  Main__st_17 v_1748;
  int v_1747;
  int v_1746;
  int v_1745;
  int v_1744;
  int v_1743;
  int v_1742;
  int v_1741;
  int v_1740;
  int v_1739;
  int v_1738;
  int v_1737;
  int v_1736;
  int v_1735;
  int v_1776;
  Main__st_17 v_1775;
  int v_1774;
  Main__st_17 v_1773;
  int v_1772;
  Main__st_17 v_1771;
  int v_1770;
  Main__st_17 v_1769;
  int v_1768;
  int v_1767;
  int v_1766;
  int v_1765;
  int v_1764;
  int v_1763;
  int v_1762;
  int v_1761;
  int v_1760;
  int v_1759;
  int v_1758;
  int v_1757;
  int v_1756;
  int v_1797;
  Main__st_17 v_1796;
  int v_1795;
  Main__st_17 v_1794;
  int v_1793;
  Main__st_17 v_1792;
  int v_1791;
  Main__st_17 v_1790;
  int v_1789;
  int v_1788;
  int v_1787;
  int v_1786;
  int v_1785;
  int v_1784;
  int v_1783;
  int v_1782;
  int v_1781;
  int v_1780;
  int v_1779;
  int v_1778;
  int v_1777;
  int v_1818;
  Main__st_17 v_1817;
  int v_1816;
  Main__st_17 v_1815;
  int v_1814;
  Main__st_17 v_1813;
  int v_1812;
  Main__st_17 v_1811;
  int v_1810;
  int v_1809;
  int v_1808;
  int v_1807;
  int v_1806;
  int v_1805;
  int v_1804;
  int v_1803;
  int v_1802;
  int v_1801;
  int v_1800;
  int v_1799;
  int v_1798;
  int v_1838;
  Main__st_17 v_1837;
  int v_1836;
  Main__st_17 v_1835;
  int v_1834;
  Main__st_17 v_1833;
  int v_1832;
  Main__st_17 v_1831;
  int v_1830;
  int v_1829;
  int v_1828;
  int v_1827;
  int v_1826;
  int v_1825;
  int v_1824;
  int v_1823;
  int v_1822;
  int v_1821;
  int v_1820;
  int v_1819;
  int r_27_St_17_Wait;
  Main__st_17 s_5_St_17_Wait;
  int r_27_St_17_Hw3;
  Main__st_17 s_5_St_17_Hw3;
  int r_27_St_17_Hw2;
  Main__st_17 s_5_St_17_Hw2;
  int r_27_St_17_Hw1;
  Main__st_17 s_5_St_17_Hw1;
  int r_27_St_17_Sw;
  Main__st_17 s_5_St_17_Sw;
  int r_27_St_17_Inactive;
  Main__st_17 s_5_St_17_Inactive;
  int nr_5_St_17_Wait;
  Main__st_17 ns_5_St_17_Wait;
  Main__version_type ver_8_St_17_Wait;
  int wt_8_St_17_Wait;
  int act_11_St_17_Wait;
  int size_8_St_17_Wait;
  int nr_5_St_17_Hw3;
  Main__st_17 ns_5_St_17_Hw3;
  Main__version_type ver_8_St_17_Hw3;
  int wt_8_St_17_Hw3;
  int act_11_St_17_Hw3;
  int size_8_St_17_Hw3;
  int nr_5_St_17_Hw2;
  Main__st_17 ns_5_St_17_Hw2;
  Main__version_type ver_8_St_17_Hw2;
  int wt_8_St_17_Hw2;
  int act_11_St_17_Hw2;
  int size_8_St_17_Hw2;
  int nr_5_St_17_Hw1;
  Main__st_17 ns_5_St_17_Hw1;
  Main__version_type ver_8_St_17_Hw1;
  int wt_8_St_17_Hw1;
  int act_11_St_17_Hw1;
  int size_8_St_17_Hw1;
  int nr_5_St_17_Sw;
  Main__st_17 ns_5_St_17_Sw;
  Main__version_type ver_8_St_17_Sw;
  int wt_8_St_17_Sw;
  int act_11_St_17_Sw;
  int size_8_St_17_Sw;
  int nr_5_St_17_Inactive;
  Main__st_17 ns_5_St_17_Inactive;
  Main__version_type ver_8_St_17_Inactive;
  int wt_8_St_17_Inactive;
  int act_11_St_17_Inactive;
  int size_8_St_17_Inactive;
  Main__st_17 ck_40;
  int v_1840;
  Main__st_16 v_1839;
  int v_1843;
  Main__st_16 v_1842;
  int v_1841;
  int v_1848;
  Main__st_16 v_1847;
  int v_1846;
  int v_1845;
  int v_1844;
  int r_26_St_16_Wait;
  Main__st_16 s_4_St_16_Wait;
  int r_26_St_16_Active;
  Main__st_16 s_4_St_16_Active;
  int r_26_St_16_Inactive;
  Main__st_16 s_4_St_16_Inactive;
  int nr_4_St_16_Wait;
  Main__st_16 ns_4_St_16_Wait;
  Main__version_type ver_9_St_16_Wait;
  int wt_9_St_16_Wait;
  int act_12_St_16_Wait;
  int size_9_St_16_Wait;
  int nr_4_St_16_Active;
  Main__st_16 ns_4_St_16_Active;
  Main__version_type ver_9_St_16_Active;
  int wt_9_St_16_Active;
  int act_12_St_16_Active;
  int size_9_St_16_Active;
  int nr_4_St_16_Inactive;
  Main__st_16 ns_4_St_16_Inactive;
  Main__version_type ver_9_St_16_Inactive;
  int wt_9_St_16_Inactive;
  int act_12_St_16_Inactive;
  int size_9_St_16_Inactive;
  Main__st_16 ck_42;
  int v_1856;
  Main__st_15 v_1855;
  int v_1854;
  Main__st_15 v_1853;
  int v_1852;
  Main__st_15 v_1851;
  int v_1850;
  Main__st_15 v_1849;
  int v_1877;
  Main__st_15 v_1876;
  int v_1875;
  Main__st_15 v_1874;
  int v_1873;
  Main__st_15 v_1872;
  int v_1871;
  Main__st_15 v_1870;
  int v_1869;
  int v_1868;
  int v_1867;
  int v_1866;
  int v_1865;
  int v_1864;
  int v_1863;
  int v_1862;
  int v_1861;
  int v_1860;
  int v_1859;
  int v_1858;
  int v_1857;
  int v_1898;
  Main__st_15 v_1897;
  int v_1896;
  Main__st_15 v_1895;
  int v_1894;
  Main__st_15 v_1893;
  int v_1892;
  Main__st_15 v_1891;
  int v_1890;
  int v_1889;
  int v_1888;
  int v_1887;
  int v_1886;
  int v_1885;
  int v_1884;
  int v_1883;
  int v_1882;
  int v_1881;
  int v_1880;
  int v_1879;
  int v_1878;
  int v_1919;
  Main__st_15 v_1918;
  int v_1917;
  Main__st_15 v_1916;
  int v_1915;
  Main__st_15 v_1914;
  int v_1913;
  Main__st_15 v_1912;
  int v_1911;
  int v_1910;
  int v_1909;
  int v_1908;
  int v_1907;
  int v_1906;
  int v_1905;
  int v_1904;
  int v_1903;
  int v_1902;
  int v_1901;
  int v_1900;
  int v_1899;
  int v_1940;
  Main__st_15 v_1939;
  int v_1938;
  Main__st_15 v_1937;
  int v_1936;
  Main__st_15 v_1935;
  int v_1934;
  Main__st_15 v_1933;
  int v_1932;
  int v_1931;
  int v_1930;
  int v_1929;
  int v_1928;
  int v_1927;
  int v_1926;
  int v_1925;
  int v_1924;
  int v_1923;
  int v_1922;
  int v_1921;
  int v_1920;
  int v_1960;
  Main__st_15 v_1959;
  int v_1958;
  Main__st_15 v_1957;
  int v_1956;
  Main__st_15 v_1955;
  int v_1954;
  Main__st_15 v_1953;
  int v_1952;
  int v_1951;
  int v_1950;
  int v_1949;
  int v_1948;
  int v_1947;
  int v_1946;
  int v_1945;
  int v_1944;
  int v_1943;
  int v_1942;
  int v_1941;
  int r_25_St_15_Wait;
  Main__st_15 s_3_St_15_Wait;
  int r_25_St_15_Hw3;
  Main__st_15 s_3_St_15_Hw3;
  int r_25_St_15_Hw2;
  Main__st_15 s_3_St_15_Hw2;
  int r_25_St_15_Hw1;
  Main__st_15 s_3_St_15_Hw1;
  int r_25_St_15_Sw;
  Main__st_15 s_3_St_15_Sw;
  int r_25_St_15_Inactive;
  Main__st_15 s_3_St_15_Inactive;
  int nr_3_St_15_Wait;
  Main__st_15 ns_3_St_15_Wait;
  Main__version_type ver_10_St_15_Wait;
  int wt_10_St_15_Wait;
  int act_13_St_15_Wait;
  int size_10_St_15_Wait;
  int nr_3_St_15_Hw3;
  Main__st_15 ns_3_St_15_Hw3;
  Main__version_type ver_10_St_15_Hw3;
  int wt_10_St_15_Hw3;
  int act_13_St_15_Hw3;
  int size_10_St_15_Hw3;
  int nr_3_St_15_Hw2;
  Main__st_15 ns_3_St_15_Hw2;
  Main__version_type ver_10_St_15_Hw2;
  int wt_10_St_15_Hw2;
  int act_13_St_15_Hw2;
  int size_10_St_15_Hw2;
  int nr_3_St_15_Hw1;
  Main__st_15 ns_3_St_15_Hw1;
  Main__version_type ver_10_St_15_Hw1;
  int wt_10_St_15_Hw1;
  int act_13_St_15_Hw1;
  int size_10_St_15_Hw1;
  int nr_3_St_15_Sw;
  Main__st_15 ns_3_St_15_Sw;
  Main__version_type ver_10_St_15_Sw;
  int wt_10_St_15_Sw;
  int act_13_St_15_Sw;
  int size_10_St_15_Sw;
  int nr_3_St_15_Inactive;
  Main__st_15 ns_3_St_15_Inactive;
  Main__version_type ver_10_St_15_Inactive;
  int wt_10_St_15_Inactive;
  int act_13_St_15_Inactive;
  int size_10_St_15_Inactive;
  Main__st_15 ck_44;
  int v_1963;
  Main__st_14 v_1962;
  int v_1961;
  int v_1965;
  Main__st_14 v_1964;
  int r_24_St_14_Fail;
  Main__st_14 s_2_St_14_Fail;
  int r_24_St_14_Busy;
  Main__st_14 s_2_St_14_Busy;
  int r_24_St_14_Free;
  Main__st_14 s_2_St_14_Free;
  int nr_2_St_14_Fail;
  Main__st_14 ns_2_St_14_Fail;
  int err_St_14_Fail;
  int act_14_St_14_Fail;
  int nr_2_St_14_Busy;
  Main__st_14 ns_2_St_14_Busy;
  int err_St_14_Busy;
  int act_14_St_14_Busy;
  int nr_2_St_14_Free;
  Main__st_14 ns_2_St_14_Free;
  int err_St_14_Free;
  int act_14_St_14_Free;
  Main__st_14 ck_46;
  int v_1968;
  Main__st_13 v_1967;
  int v_1966;
  int v_1970;
  Main__st_13 v_1969;
  int r_23_St_13_Fail;
  Main__st_13 s_1_St_13_Fail;
  int r_23_St_13_Busy;
  Main__st_13 s_1_St_13_Busy;
  int r_23_St_13_Free;
  Main__st_13 s_1_St_13_Free;
  int nr_1_St_13_Fail;
  Main__st_13 ns_1_St_13_Fail;
  int err_4_St_13_Fail;
  int act_15_St_13_Fail;
  int nr_1_St_13_Busy;
  Main__st_13 ns_1_St_13_Busy;
  int err_4_St_13_Busy;
  int act_15_St_13_Busy;
  int nr_1_St_13_Free;
  Main__st_13 ns_1_St_13_Free;
  int err_4_St_13_Free;
  int act_15_St_13_Free;
  Main__st_13 ck_48;
  int v_1973;
  Main__st_12 v_1972;
  int v_1971;
  int v_1975;
  Main__st_12 v_1974;
  int r_22_St_12_Fail;
  Main__st_12 s_St_12_Fail;
  int r_22_St_12_Busy;
  Main__st_12 s_St_12_Busy;
  int r_22_St_12_Free;
  Main__st_12 s_St_12_Free;
  int nr_St_12_Fail;
  Main__st_12 ns_St_12_Fail;
  int err_5_St_12_Fail;
  int act_16_St_12_Fail;
  int nr_St_12_Busy;
  Main__st_12 ns_St_12_Busy;
  int err_5_St_12_Busy;
  int act_16_St_12_Busy;
  int nr_St_12_Free;
  Main__st_12 ns_St_12_Free;
  int err_5_St_12_Free;
  int act_16_St_12_Free;
  Main__st_12 ck_50;
  int v_1048;
  int v_1047;
  int v_1046;
  int v_1044;
  int v_1043;
  int v_1042;
  int v_1041;
  int v_1040;
  int v_1039;
  int v_1038;
  int v_1037;
  int v_1036;
  int v_1035;
  int v_1033;
  int v_1032;
  int v_1031;
  int v_1030;
  int v_1029;
  int v_1028;
  int v_1026;
  int v_1025;
  int v_1024;
  int v_1023;
  int v_1022;
  int v_1021;
  int v_1020;
  int v_1019;
  int v_1017;
  int v_1016;
  int v_1015;
  int v_1014;
  int v_1013;
  int v_1012;
  int v_1011;
  int v_1010;
  int v_1009;
  int v_1008;
  int v_1006;
  int v_1005;
  int v_1004;
  int v_1003;
  int v_1002;
  int v_1001;
  int v_999;
  int v_998;
  int v_997;
  int v_996;
  int v_995;
  int v_994;
  int v_993;
  int v_992;
  int v_990;
  int v_989;
  int v_988;
  int v_987;
  int v_986;
  int v_985;
  int v_984;
  int v_983;
  int v_982;
  int v_981;
  int v_979;
  int v_978;
  int v_977;
  int v_976;
  int v_975;
  int v_974;
  int v_972;
  int v_971;
  int v_970;
  int v_969;
  int v_968;
  int v_967;
  int v_966;
  int v_965;
  int v_964;
  int v_963;
  int v_962;
  int v_961;
  int v_960;
  int v_959;
  int v_958;
  int v_957;
  int v_956;
  int v_955;
  int v_954;
  int v_953;
  int v_952;
  int v_951;
  int v_950;
  int v_949;
  int v_948;
  int v_947;
  int v_946;
  int v_945;
  int v_944;
  int v_943;
  int v_942;
  int v_941;
  int v_940;
  int v_939;
  int v_938;
  int v_937;
  int v_936;
  int v_935;
  int v_934;
  int v_933;
  int v_932;
  int v_931;
  int v_930;
  int v_929;
  int v_928;
  int v_927;
  int v_926;
  int v_925;
  int v_924;
  int v_923;
  int v_922;
  int v_921;
  int v_920;
  int v_919;
  int v_918;
  int v_917;
  int v_916;
  int v_915;
  int v_914;
  int v_913;
  int v_912;
  int v_911;
  int v_910;
  int v_909;
  int v_908;
  int v_907;
  int v_906;
  int v;
  Main__st_25 s_13;
  Main__st_25 ns_13;
  int r_35;
  int nr_13;
  Main__st_24 s_12;
  Main__st_24 ns_12;
  int r_34;
  int nr_12;
  Main__st_23 s_11;
  Main__st_23 ns_11;
  int r_33;
  int nr_11;
  Main__st_22 s_10;
  Main__st_22 ns_10;
  int r_32;
  int nr_10;
  Main__st_21 s_9;
  Main__st_21 ns_9;
  int r_31;
  int nr_9;
  Main__st_20 s_8;
  Main__st_20 ns_8;
  int r_30;
  int nr_8;
  Main__st_19 s_7;
  Main__st_19 ns_7;
  int r_29;
  int nr_7;
  Main__st_18 s_6;
  Main__st_18 ns_6;
  int r_28;
  int nr_6;
  Main__st_17 s_5;
  Main__st_17 ns_5;
  int r_27;
  int nr_5;
  Main__st_16 s_4;
  Main__st_16 ns_4;
  int r_26;
  int nr_4;
  Main__st_15 s_3;
  Main__st_15 ns_3;
  int r_25;
  int nr_3;
  Main__st_14 s_2;
  Main__st_14 ns_2;
  int r_24;
  int nr_2;
  Main__st_13 s_1;
  Main__st_13 ns_1;
  int r_23;
  int nr_1;
  Main__st_12 s;
  Main__st_12 ns;
  int r_22;
  int nr;
  int cr_2;
  int f_5;
  int rp_5;
  int act_16;
  int err_5;
  int cr_1;
  int f_4;
  int rp_4;
  int act_15;
  int err_4;
  int cr;
  int f;
  int rp;
  int act_14;
  int err;
  int r_10;
  int c1_9;
  int c2_9;
  int c3_8;
  int c4_5;
  int e_10;
  int size_10;
  int act_13;
  int wt_10;
  Main__version_type ver_10;
  int r_9;
  int c_1;
  int e_9;
  int size_9;
  int act_12;
  int wt_9;
  Main__version_type ver_9;
  int r_8;
  int c1_8;
  int c2_8;
  int c3_7;
  int c4_4;
  int e_8;
  int size_8;
  int act_11;
  int wt_8;
  Main__version_type ver_8;
  int r_7;
  int c1_7;
  int c2_7;
  int c3_6;
  int c4_3;
  int e_7;
  int size_7;
  int act_10;
  int wt_7;
  Main__version_type ver_7;
  int r_6;
  int c1_6;
  int c2_6;
  int c3_5;
  int c4_2;
  int e_6;
  int size_6;
  int act_9;
  int wt_6;
  Main__version_type ver_6;
  int r_5;
  int c1_5;
  int c2_5;
  int c3_4;
  int c4_1;
  int e_5;
  int size_5;
  int act_8;
  int wt_5;
  Main__version_type ver_5;
  int r_4;
  int c1_4;
  int c2_4;
  int c3_3;
  int c4;
  int e_4;
  int size_4;
  int act_7;
  int wt_4;
  Main__version_type ver_4;
  int r_3;
  int c1_3;
  int c2_3;
  int e_3;
  int size_3;
  int act_6;
  int wt_3;
  Main__version_type ver_3;
  int r_2;
  int c1_2;
  int c2_2;
  int c3_2;
  int e_2;
  int size_2;
  int act_5;
  int wt_2;
  Main__version_type ver_2;
  int r_1;
  int c1_1;
  int c2_1;
  int c3_1;
  int e_1;
  int size_1;
  int act_4;
  int wt_1;
  Main__version_type ver_1;
  int r;
  int c1;
  int c2;
  int c3;
  int e;
  int size;
  int act;
  int wt;
  Main__version_type ver;
  v_969 = (texe4>-1);
  v_971 = (maxtexe4<texe4);
  v_978 = (texe4>-1);
  v_987 = (texe4>-1);
  v_989 = (texe4<mintexe4);
  v_996 = (texe10>-1);
  v_998 = (maxtexe10<texe10);
  v_1005 = (texe10>-1);
  v_1014 = (texe10>-1);
  v_1016 = (texe10<mintexe10);
  v_1023 = (texe11>-1);
  v_1025 = (maxtexe11<texe11);
  v_1032 = (texe11>-1);
  v_1041 = (texe11>-1);
  v_1043 = (texe11<mintexe11);
  _out->wcet1_sw = 6;
  _out->wcet1_hw1 = 5;
  _out->wcet1_hw2 = 5;
  _out->wcet1_hw3 = 5;
  _out->wcet2_sw = 0;
  _out->wcet2_hw1 = 0;
  _out->wcet2_hw2 = 0;
  _out->wcet2_hw3 = 0;
  _out->wcet3_sw = 1000;
  _out->wcet3_hw1 = 25;
  _out->wcet3_hw2 = 25;
  _out->wcet3_hw3 = 25;
  _out->wcet4_sw = 255;
  _out->wcet4_hw1 = 25;
  _out->wcet4_hw2 = 25;
  _out->wcet4_hw3 = 25;
  _out->wcet5_sw = 0;
  _out->wcet5_hw1 = 0;
  _out->wcet5_hw2 = 0;
  _out->wcet5_hw3 = 0;
  _out->wcet6_sw = 394;
  _out->wcet6_hw1 = 25;
  _out->wcet6_hw2 = 25;
  _out->wcet6_hw3 = 25;
  _out->wcet7_sw = 10;
  _out->wcet7_hw1 = 6;
  _out->wcet7_hw2 = 6;
  _out->wcet7_hw3 = 6;
  _out->wcet8_sw = 0;
  _out->wcet8_hw1 = 0;
  _out->wcet8_hw2 = 0;
  _out->wcet8_hw3 = 0;
  _out->wcet9_sw = 0;
  _out->wcet9_hw1 = 0;
  _out->wcet9_hw2 = 0;
  _out->wcet9_hw3 = 0;
  _out->wcet10_sw = 285;
  _out->wcet10_hw1 = 8;
  _out->wcet10_hw2 = 8;
  _out->wcet10_hw3 = 8;
  _out->wcet11_sw = 285;
  _out->wcet11_hw1 = 7;
  _out->wcet11_hw2 = 7;
  _out->wcet11_hw3 = 7;
  e = trk_e;
  r = trk_r;
  e_1 = dt_e;
  r_1 = dt_r;
  e_2 = rpl_e;
  r_2 = rpl_r;
  e_3 = rb_e;
  r_3 = rb_r;
  e_4 = rs_e;
  r_4 = rs_r;
  e_5 = tl_e;
  r_5 = tl_r;
  e_6 = oa_e;
  r_6 = oa_r;
  e_7 = sl_e;
  r_7 = sl_r;
  e_8 = me_img_e;
  r_8 = me_img_r;
  e_9 = me_imu_e;
  r_9 = me_imu_r;
  e_10 = c_img_e;
  r_10 = c_img_r;
  rp = rp_3;
  f = f_3;
  rp_4 = rp_2;
  f_4 = f_2;
  rp_5 = rp_1;
  f_5 = f_1;
  switch (self->ck_45) {
    case Main__St_14_Fail:
      if (rp) {
        s_2_St_14_Fail = Main__St_14_Free;
        r_24_St_14_Fail = true;
      } else {
        s_2_St_14_Fail = Main__St_14_Fail;
        r_24_St_14_Fail = self->pnr_2;
      };
      break;
    default:
      break;
  };
  switch (self->ck_47) {
    case Main__St_13_Fail:
      if (rp_4) {
        s_1_St_13_Fail = Main__St_13_Free;
        r_23_St_13_Fail = true;
      } else {
        s_1_St_13_Fail = Main__St_13_Fail;
        r_23_St_13_Fail = self->pnr_1;
      };
      break;
    default:
      break;
  };
  switch (self->ck_49) {
    case Main__St_12_Fail:
      if (rp_5) {
        s_St_12_Fail = Main__St_12_Free;
        r_22_St_12_Fail = true;
      } else {
        s_St_12_Fail = Main__St_12_Fail;
        r_22_St_12_Fail = self->pnr;
      };
      break;
    default:
      break;
  };
  Main_controller__main_ctrlr0_step(c_img_e, c_img_r, self->ck_23,
                                    self->ck_25, self->ck_27, self->ck_29,
                                    self->ck_31, self->ck_33, self->ck_35,
                                    self->ck_37, self->ck_39, self->ck_41,
                                    self->ck_43, self->ck_45, self->ck_47,
                                    self->ck_49, dt_e, dt_r, f_1, f_2, f_3,
                                    maxtexe1, maxtexe10, maxtexe11, maxtexe2,
                                    maxtexe3, maxtexe4, maxtexe5, maxtexe6,
                                    maxtexe7, maxtexe8, maxtexe9, me_img_e,
                                    me_img_r, me_imu_e, me_imu_r, mintexe1,
                                    mintexe10, mintexe11, mintexe2, mintexe3,
                                    mintexe4, mintexe5, mintexe6, mintexe7,
                                    mintexe8, mintexe9, oa_e, oa_r,
                                    self->pnr, self->pnr_1, self->pnr_10,
                                    self->pnr_11, self->pnr_12, self->pnr_13,
                                    self->pnr_2, self->pnr_3, self->pnr_4,
                                    self->pnr_5, self->pnr_6, self->pnr_7,
                                    self->pnr_8, self->pnr_9, rb_e, rb_r,
                                    rp_1, rp_2, rp_3, rpl_e, rpl_r, rs_e,
                                    rs_r, sl_e, sl_r, texe1, texe10, texe11,
                                    texe2, texe3, texe4, texe5, texe6, texe7,
                                    texe8, texe9, tl_e, tl_r, trk_e, trk_r,
                                    self->v_1000, self->v_1007, self->v_1018,
                                    self->v_1027, self->v_1034, self->v_1045,
                                    self->v_973, self->v_980, self->v_991,
                                    &Main_controller__main_ctrlr0_out_st);
  c = Main_controller__main_ctrlr0_out_st.c;
  cDw1 = Main_controller__main_ctrlr0_out_st.cDw1;
  cDw10 = Main_controller__main_ctrlr0_out_st.cDw10;
  cDw11 = Main_controller__main_ctrlr0_out_st.cDw11;
  cDw2 = Main_controller__main_ctrlr0_out_st.cDw2;
  cDw3 = Main_controller__main_ctrlr0_out_st.cDw3;
  cDw4 = Main_controller__main_ctrlr0_out_st.cDw4;
  cDw5 = Main_controller__main_ctrlr0_out_st.cDw5;
  cDw6 = Main_controller__main_ctrlr0_out_st.cDw6;
  cDw7 = Main_controller__main_ctrlr0_out_st.cDw7;
  cDw8 = Main_controller__main_ctrlr0_out_st.cDw8;
  cDw9 = Main_controller__main_ctrlr0_out_st.cDw9;
  cKp1 = Main_controller__main_ctrlr0_out_st.cKp1;
  cKp10 = Main_controller__main_ctrlr0_out_st.cKp10;
  cKp11 = Main_controller__main_ctrlr0_out_st.cKp11;
  cKp2 = Main_controller__main_ctrlr0_out_st.cKp2;
  cKp3 = Main_controller__main_ctrlr0_out_st.cKp3;
  cKp4 = Main_controller__main_ctrlr0_out_st.cKp4;
  cKp5 = Main_controller__main_ctrlr0_out_st.cKp5;
  cKp6 = Main_controller__main_ctrlr0_out_st.cKp6;
  cKp7 = Main_controller__main_ctrlr0_out_st.cKp7;
  cKp8 = Main_controller__main_ctrlr0_out_st.cKp8;
  cKp9 = Main_controller__main_ctrlr0_out_st.cKp9;
  cUp1 = Main_controller__main_ctrlr0_out_st.cUp1;
  cUp10 = Main_controller__main_ctrlr0_out_st.cUp10;
  cUp11 = Main_controller__main_ctrlr0_out_st.cUp11;
  cUp2 = Main_controller__main_ctrlr0_out_st.cUp2;
  cUp3 = Main_controller__main_ctrlr0_out_st.cUp3;
  cUp4 = Main_controller__main_ctrlr0_out_st.cUp4;
  cUp5 = Main_controller__main_ctrlr0_out_st.cUp5;
  cUp6 = Main_controller__main_ctrlr0_out_st.cUp6;
  cUp7 = Main_controller__main_ctrlr0_out_st.cUp7;
  cUp8 = Main_controller__main_ctrlr0_out_st.cUp8;
  cUp9 = Main_controller__main_ctrlr0_out_st.cUp9;
  c_img_c1 = Main_controller__main_ctrlr0_out_st.c_img_c1;
  c_img_c2 = Main_controller__main_ctrlr0_out_st.c_img_c2;
  c_img_c3 = Main_controller__main_ctrlr0_out_st.c_img_c3;
  c_img_c4 = Main_controller__main_ctrlr0_out_st.c_img_c4;
  cr1 = Main_controller__main_ctrlr0_out_st.cr1;
  cr2 = Main_controller__main_ctrlr0_out_st.cr2;
  cr3 = Main_controller__main_ctrlr0_out_st.cr3;
  ct10 = Main_controller__main_ctrlr0_out_st.ct10;
  ct11 = Main_controller__main_ctrlr0_out_st.ct11;
  ct4 = Main_controller__main_ctrlr0_out_st.ct4;
  ct5 = Main_controller__main_ctrlr0_out_st.ct5;
  ct6 = Main_controller__main_ctrlr0_out_st.ct6;
  dt_c1 = Main_controller__main_ctrlr0_out_st.dt_c1;
  dt_c2 = Main_controller__main_ctrlr0_out_st.dt_c2;
  dt_c3 = Main_controller__main_ctrlr0_out_st.dt_c3;
  me_img_c1 = Main_controller__main_ctrlr0_out_st.me_img_c1;
  me_img_c2 = Main_controller__main_ctrlr0_out_st.me_img_c2;
  me_img_c3 = Main_controller__main_ctrlr0_out_st.me_img_c3;
  me_img_c4 = Main_controller__main_ctrlr0_out_st.me_img_c4;
  me_imu_c = Main_controller__main_ctrlr0_out_st.me_imu_c;
  oa_c1 = Main_controller__main_ctrlr0_out_st.oa_c1;
  oa_c2 = Main_controller__main_ctrlr0_out_st.oa_c2;
  oa_c3 = Main_controller__main_ctrlr0_out_st.oa_c3;
  oa_c4 = Main_controller__main_ctrlr0_out_st.oa_c4;
  rb_c1 = Main_controller__main_ctrlr0_out_st.rb_c1;
  rb_c2 = Main_controller__main_ctrlr0_out_st.rb_c2;
  rpl_c1 = Main_controller__main_ctrlr0_out_st.rpl_c1;
  rpl_c2 = Main_controller__main_ctrlr0_out_st.rpl_c2;
  rpl_c3 = Main_controller__main_ctrlr0_out_st.rpl_c3;
  rs_c1 = Main_controller__main_ctrlr0_out_st.rs_c1;
  rs_c2 = Main_controller__main_ctrlr0_out_st.rs_c2;
  rs_c3 = Main_controller__main_ctrlr0_out_st.rs_c3;
  rs_c4 = Main_controller__main_ctrlr0_out_st.rs_c4;
  sl_c1 = Main_controller__main_ctrlr0_out_st.sl_c1;
  sl_c2 = Main_controller__main_ctrlr0_out_st.sl_c2;
  sl_c3 = Main_controller__main_ctrlr0_out_st.sl_c3;
  sl_c4 = Main_controller__main_ctrlr0_out_st.sl_c4;
  tl_c1 = Main_controller__main_ctrlr0_out_st.tl_c1;
  tl_c2 = Main_controller__main_ctrlr0_out_st.tl_c2;
  tl_c3 = Main_controller__main_ctrlr0_out_st.tl_c3;
  tl_c4 = Main_controller__main_ctrlr0_out_st.tl_c4;
  trk_c1 = Main_controller__main_ctrlr0_out_st.trk_c1;
  trk_c2 = Main_controller__main_ctrlr0_out_st.trk_c2;
  trk_c3 = Main_controller__main_ctrlr0_out_st.trk_c3;
  c3 = trk_c3;
  c2 = trk_c2;
  c1 = trk_c1;
  c3_1 = dt_c3;
  c2_1 = dt_c2;
  c1_1 = dt_c1;
  c3_2 = rpl_c3;
  c2_2 = rpl_c2;
  c1_2 = rpl_c1;
  c2_3 = rb_c2;
  c1_3 = rb_c1;
  c4 = rs_c4;
  c3_3 = rs_c3;
  c2_4 = rs_c2;
  c1_4 = rs_c1;
  c4_1 = tl_c4;
  c3_4 = tl_c3;
  c2_5 = tl_c2;
  c1_5 = tl_c1;
  c4_2 = oa_c4;
  c3_5 = oa_c3;
  c2_6 = oa_c2;
  c1_6 = oa_c1;
  c4_3 = sl_c4;
  c3_6 = sl_c3;
  c2_7 = sl_c2;
  c1_7 = sl_c1;
  c4_4 = me_img_c4;
  c3_7 = me_img_c3;
  c2_8 = me_img_c2;
  c1_8 = me_img_c1;
  c_1 = me_imu_c;
  c4_5 = c_img_c4;
  c3_8 = c_img_c3;
  c2_9 = c_img_c2;
  c1_9 = c_img_c1;
  cr = cr3;
  cr_1 = cr2;
  cr_2 = cr1;
  switch (self->ck_23) {
    case Main__St_25_Wait:
      if (c3) {
        v_1049 = Main__St_25_Sw;
      } else {
        v_1049 = Main__St_25_Wait;
      };
      if (c2) {
        v_1051 = Main__St_25_Hw2;
      } else {
        v_1051 = v_1049;
      };
      if (c1) {
        v_1053 = Main__St_25_Hw1;
      } else {
        v_1053 = v_1051;
      };
      if (e) {
        s_13_St_25_Wait = Main__St_25_Inactive;
      } else {
        s_13_St_25_Wait = v_1053;
      };
      if (c3) {
        v_1050 = true;
      } else {
        v_1050 = self->pnr_13;
      };
      if (c2) {
        v_1052 = true;
      } else {
        v_1052 = v_1050;
      };
      if (c1) {
        v_1054 = true;
      } else {
        v_1054 = v_1052;
      };
      if (e) {
        r_35_St_25_Wait = true;
      } else {
        r_35_St_25_Wait = v_1054;
      };
      s_13 = s_13_St_25_Wait;
      r_35 = r_35_St_25_Wait;
      break;
    case Main__St_25_Hw2:
      v_1055 = !(c2);
      v_1056 = (v_1055&&c1);
      v_1057 = !(c2);
      v_1058 = (v_1057&&c3);
      v_1059 = !(c2);
      v_1060 = !(c1);
      v_1061 = (v_1059&&v_1060);
      v_1062 = !(c3);
      v_1063 = (v_1061&&v_1062);
      if (v_1063) {
        v_1064 = Main__St_25_Wait;
      } else {
        v_1064 = Main__St_25_Hw2;
      };
      if (v_1058) {
        v_1066 = Main__St_25_Sw;
      } else {
        v_1066 = v_1064;
      };
      if (v_1056) {
        v_1068 = Main__St_25_Hw1;
      } else {
        v_1068 = v_1066;
      };
      if (e) {
        s_13_St_25_Hw2 = Main__St_25_Inactive;
      } else {
        s_13_St_25_Hw2 = v_1068;
      };
      if (v_1063) {
        v_1065 = true;
      } else {
        v_1065 = self->pnr_13;
      };
      if (v_1058) {
        v_1067 = true;
      } else {
        v_1067 = v_1065;
      };
      if (v_1056) {
        v_1069 = true;
      } else {
        v_1069 = v_1067;
      };
      if (e) {
        r_35_St_25_Hw2 = true;
      } else {
        r_35_St_25_Hw2 = v_1069;
      };
      s_13 = s_13_St_25_Hw2;
      r_35 = r_35_St_25_Hw2;
      break;
    case Main__St_25_Hw1:
      v_1070 = !(c1);
      v_1071 = (v_1070&&c2);
      v_1072 = !(c2);
      v_1073 = (v_1072&&c3);
      v_1074 = !(c2);
      v_1075 = !(c1);
      v_1076 = (v_1074&&v_1075);
      v_1077 = !(c3);
      v_1078 = (v_1076&&v_1077);
      if (v_1078) {
        v_1079 = Main__St_25_Wait;
      } else {
        v_1079 = Main__St_25_Hw1;
      };
      if (v_1073) {
        v_1081 = Main__St_25_Sw;
      } else {
        v_1081 = v_1079;
      };
      if (v_1071) {
        v_1083 = Main__St_25_Hw2;
      } else {
        v_1083 = v_1081;
      };
      if (e) {
        s_13_St_25_Hw1 = Main__St_25_Inactive;
      } else {
        s_13_St_25_Hw1 = v_1083;
      };
      if (v_1078) {
        v_1080 = true;
      } else {
        v_1080 = self->pnr_13;
      };
      if (v_1073) {
        v_1082 = true;
      } else {
        v_1082 = v_1080;
      };
      if (v_1071) {
        v_1084 = true;
      } else {
        v_1084 = v_1082;
      };
      if (e) {
        r_35_St_25_Hw1 = true;
      } else {
        r_35_St_25_Hw1 = v_1084;
      };
      s_13 = s_13_St_25_Hw1;
      r_35 = r_35_St_25_Hw1;
      break;
    case Main__St_25_Sw:
      v_1085 = !(c3);
      v_1086 = (v_1085&&c1);
      v_1087 = !(c3);
      v_1088 = (v_1087&&c2);
      v_1089 = !(c3);
      v_1090 = !(c2);
      v_1091 = (v_1089&&v_1090);
      v_1092 = !(c1);
      v_1093 = (v_1091&&v_1092);
      if (v_1093) {
        v_1094 = Main__St_25_Wait;
      } else {
        v_1094 = Main__St_25_Sw;
      };
      if (v_1088) {
        v_1096 = Main__St_25_Hw2;
      } else {
        v_1096 = v_1094;
      };
      if (v_1086) {
        v_1098 = Main__St_25_Hw1;
      } else {
        v_1098 = v_1096;
      };
      if (e) {
        s_13_St_25_Sw = Main__St_25_Inactive;
      } else {
        s_13_St_25_Sw = v_1098;
      };
      if (v_1093) {
        v_1095 = true;
      } else {
        v_1095 = self->pnr_13;
      };
      if (v_1088) {
        v_1097 = true;
      } else {
        v_1097 = v_1095;
      };
      if (v_1086) {
        v_1099 = true;
      } else {
        v_1099 = v_1097;
      };
      if (e) {
        r_35_St_25_Sw = true;
      } else {
        r_35_St_25_Sw = v_1099;
      };
      s_13 = s_13_St_25_Sw;
      r_35 = r_35_St_25_Sw;
      break;
    case Main__St_25_Inactive:
      v_1100 = (r&&c1);
      v_1101 = (r&&c2);
      v_1102 = (r&&c3);
      v_1103 = !(c2);
      v_1104 = (r&&v_1103);
      v_1105 = !(c1);
      v_1106 = (v_1104&&v_1105);
      v_1107 = !(c3);
      v_1108 = (v_1106&&v_1107);
      if (v_1108) {
        v_1109 = Main__St_25_Wait;
      } else {
        v_1109 = Main__St_25_Inactive;
      };
      if (v_1102) {
        v_1111 = Main__St_25_Sw;
      } else {
        v_1111 = v_1109;
      };
      if (v_1101) {
        v_1113 = Main__St_25_Hw2;
      } else {
        v_1113 = v_1111;
      };
      if (v_1100) {
        s_13_St_25_Inactive = Main__St_25_Hw1;
      } else {
        s_13_St_25_Inactive = v_1113;
      };
      s_13 = s_13_St_25_Inactive;
      if (v_1108) {
        v_1110 = true;
      } else {
        v_1110 = self->pnr_13;
      };
      if (v_1102) {
        v_1112 = true;
      } else {
        v_1112 = v_1110;
      };
      if (v_1101) {
        v_1114 = true;
      } else {
        v_1114 = v_1112;
      };
      if (v_1100) {
        r_35_St_25_Inactive = true;
      } else {
        r_35_St_25_Inactive = v_1114;
      };
      r_35 = r_35_St_25_Inactive;
      break;
    default:
      break;
  };
  ck_24 = s_13;
  switch (ck_24) {
    case Main__St_25_Wait:
      ns_13_St_25_Wait = Main__St_25_Wait;
      nr_13_St_25_Wait = false;
      size_St_25_Wait = 0;
      act_St_25_Wait = false;
      wt_St_25_Wait = true;
      ver_St_25_Wait = Main__N;
      ns_13 = ns_13_St_25_Wait;
      nr_13 = nr_13_St_25_Wait;
      size = size_St_25_Wait;
      act = act_St_25_Wait;
      wt = wt_St_25_Wait;
      ver = ver_St_25_Wait;
      break;
    case Main__St_25_Hw2:
      ns_13_St_25_Hw2 = Main__St_25_Hw2;
      nr_13_St_25_Hw2 = false;
      size_St_25_Hw2 = 2;
      act_St_25_Hw2 = true;
      wt_St_25_Hw2 = false;
      ver_St_25_Hw2 = Main__H2;
      ns_13 = ns_13_St_25_Hw2;
      nr_13 = nr_13_St_25_Hw2;
      size = size_St_25_Hw2;
      act = act_St_25_Hw2;
      wt = wt_St_25_Hw2;
      ver = ver_St_25_Hw2;
      break;
    case Main__St_25_Hw1:
      ns_13_St_25_Hw1 = Main__St_25_Hw1;
      nr_13_St_25_Hw1 = false;
      size_St_25_Hw1 = 2;
      act_St_25_Hw1 = true;
      wt_St_25_Hw1 = false;
      ver_St_25_Hw1 = Main__H1;
      ns_13 = ns_13_St_25_Hw1;
      nr_13 = nr_13_St_25_Hw1;
      size = size_St_25_Hw1;
      act = act_St_25_Hw1;
      wt = wt_St_25_Hw1;
      ver = ver_St_25_Hw1;
      break;
    case Main__St_25_Sw:
      ns_13_St_25_Sw = Main__St_25_Sw;
      nr_13_St_25_Sw = false;
      size_St_25_Sw = 0;
      act_St_25_Sw = true;
      wt_St_25_Sw = false;
      ver_St_25_Sw = Main__S;
      ns_13 = ns_13_St_25_Sw;
      nr_13 = nr_13_St_25_Sw;
      size = size_St_25_Sw;
      act = act_St_25_Sw;
      wt = wt_St_25_Sw;
      ver = ver_St_25_Sw;
      break;
    case Main__St_25_Inactive:
      ns_13_St_25_Inactive = Main__St_25_Inactive;
      ns_13 = ns_13_St_25_Inactive;
      nr_13_St_25_Inactive = false;
      nr_13 = nr_13_St_25_Inactive;
      size_St_25_Inactive = 0;
      size = size_St_25_Inactive;
      act_St_25_Inactive = false;
      act = act_St_25_Inactive;
      wt_St_25_Inactive = false;
      wt = wt_St_25_Inactive;
      ver_St_25_Inactive = Main__N;
      ver = ver_St_25_Inactive;
      break;
    default:
      break;
  };
  _out->trk_size = size;
  _out->trk_act = act;
  v_1024 = (_out->trk_act&&v_1023);
  v_1026 = (v_1024&&v_1025);
  v_1033 = (_out->trk_act&&v_1032);
  v_1042 = (_out->trk_act&&v_1041);
  v_1044 = (v_1042&&v_1043);
  _out->trk_wt = wt;
  v_966 = !(_out->trk_wt);
  v_967 = (!(ct11)||v_966);
  _out->trk_ver = ver;
  v_912 = (_out->trk_ver==Main__S);
  v_913 = (_out->trk_ver==Main__N);
  v_914 = (v_912||v_913);
  v_915 = !(v_914);
  v_946 = (_out->trk_ver==Main__S);
  v_949 = (_out->trk_ver==Main__S);
  ck_22 = _out->trk_ver;
  switch (ck_22) {
    case Main__N:
      wcet11_N = -1;
      _out->wcet11 = wcet11_N;
      break;
    case Main__H3:
      wcet11_H3 = _out->wcet11_hw3;
      _out->wcet11 = wcet11_H3;
      break;
    case Main__H2:
      wcet11_H2 = _out->wcet11_hw2;
      _out->wcet11 = wcet11_H2;
      break;
    case Main__H1:
      wcet11_H1 = _out->wcet11_hw1;
      _out->wcet11 = wcet11_H1;
      break;
    case Main__S:
      wcet11_S = _out->wcet11_sw;
      _out->wcet11 = wcet11_S;
      break;
    default:
      break;
  };
  v_1028 = (self->v_1027>_out->wcet11);
  v_1029 = (!(v_1026)||v_1028);
  v_1030 = (!(cUp11)||v_1029);
  v_1035 = (self->v_1034<_out->wcet11);
  v_1036 = (v_1033&&v_1035);
  v_1037 = (_out->wcet11<maxtexe11);
  v_1038 = (!(v_1036)||v_1037);
  v_1039 = (!(cKp11)||v_1038);
  v_1046 = (self->v_1045<_out->wcet11);
  v_1047 = (!(v_1044)||v_1046);
  v_1048 = (!(cDw11)||v_1047);
  switch (self->ck_25) {
    case Main__St_24_Wait:
      if (c3_1) {
        v_1115 = Main__St_24_Sw;
      } else {
        v_1115 = Main__St_24_Wait;
      };
      if (c2_1) {
        v_1117 = Main__St_24_Hw2;
      } else {
        v_1117 = v_1115;
      };
      if (c1_1) {
        v_1119 = Main__St_24_Hw1;
      } else {
        v_1119 = v_1117;
      };
      if (e_1) {
        s_12_St_24_Wait = Main__St_24_Inactive;
      } else {
        s_12_St_24_Wait = v_1119;
      };
      if (c3_1) {
        v_1116 = true;
      } else {
        v_1116 = self->pnr_12;
      };
      if (c2_1) {
        v_1118 = true;
      } else {
        v_1118 = v_1116;
      };
      if (c1_1) {
        v_1120 = true;
      } else {
        v_1120 = v_1118;
      };
      if (e_1) {
        r_34_St_24_Wait = true;
      } else {
        r_34_St_24_Wait = v_1120;
      };
      s_12 = s_12_St_24_Wait;
      r_34 = r_34_St_24_Wait;
      break;
    case Main__St_24_Hw2:
      v_1121 = !(c2_1);
      v_1122 = (v_1121&&c1_1);
      v_1123 = !(c2_1);
      v_1124 = (v_1123&&c3_1);
      v_1125 = !(c2_1);
      v_1126 = !(c1_1);
      v_1127 = (v_1125&&v_1126);
      v_1128 = !(c3_1);
      v_1129 = (v_1127&&v_1128);
      if (v_1129) {
        v_1130 = Main__St_24_Wait;
      } else {
        v_1130 = Main__St_24_Hw2;
      };
      if (v_1124) {
        v_1132 = Main__St_24_Sw;
      } else {
        v_1132 = v_1130;
      };
      if (v_1122) {
        v_1134 = Main__St_24_Hw1;
      } else {
        v_1134 = v_1132;
      };
      if (e_1) {
        s_12_St_24_Hw2 = Main__St_24_Inactive;
      } else {
        s_12_St_24_Hw2 = v_1134;
      };
      if (v_1129) {
        v_1131 = true;
      } else {
        v_1131 = self->pnr_12;
      };
      if (v_1124) {
        v_1133 = true;
      } else {
        v_1133 = v_1131;
      };
      if (v_1122) {
        v_1135 = true;
      } else {
        v_1135 = v_1133;
      };
      if (e_1) {
        r_34_St_24_Hw2 = true;
      } else {
        r_34_St_24_Hw2 = v_1135;
      };
      s_12 = s_12_St_24_Hw2;
      r_34 = r_34_St_24_Hw2;
      break;
    case Main__St_24_Hw1:
      v_1136 = !(c1_1);
      v_1137 = (v_1136&&c2_1);
      v_1138 = !(c2_1);
      v_1139 = (v_1138&&c3_1);
      v_1140 = !(c2_1);
      v_1141 = !(c1_1);
      v_1142 = (v_1140&&v_1141);
      v_1143 = !(c3_1);
      v_1144 = (v_1142&&v_1143);
      if (v_1144) {
        v_1145 = Main__St_24_Wait;
      } else {
        v_1145 = Main__St_24_Hw1;
      };
      if (v_1139) {
        v_1147 = Main__St_24_Sw;
      } else {
        v_1147 = v_1145;
      };
      if (v_1137) {
        v_1149 = Main__St_24_Hw2;
      } else {
        v_1149 = v_1147;
      };
      if (e_1) {
        s_12_St_24_Hw1 = Main__St_24_Inactive;
      } else {
        s_12_St_24_Hw1 = v_1149;
      };
      if (v_1144) {
        v_1146 = true;
      } else {
        v_1146 = self->pnr_12;
      };
      if (v_1139) {
        v_1148 = true;
      } else {
        v_1148 = v_1146;
      };
      if (v_1137) {
        v_1150 = true;
      } else {
        v_1150 = v_1148;
      };
      if (e_1) {
        r_34_St_24_Hw1 = true;
      } else {
        r_34_St_24_Hw1 = v_1150;
      };
      s_12 = s_12_St_24_Hw1;
      r_34 = r_34_St_24_Hw1;
      break;
    case Main__St_24_Sw:
      v_1151 = !(c3_1);
      v_1152 = (v_1151&&c1_1);
      v_1153 = !(c3_1);
      v_1154 = (v_1153&&c2_1);
      v_1155 = !(c3_1);
      v_1156 = !(c2_1);
      v_1157 = (v_1155&&v_1156);
      v_1158 = !(c1_1);
      v_1159 = (v_1157&&v_1158);
      if (v_1159) {
        v_1160 = Main__St_24_Wait;
      } else {
        v_1160 = Main__St_24_Sw;
      };
      if (v_1154) {
        v_1162 = Main__St_24_Hw2;
      } else {
        v_1162 = v_1160;
      };
      if (v_1152) {
        v_1164 = Main__St_24_Hw1;
      } else {
        v_1164 = v_1162;
      };
      if (e_1) {
        s_12_St_24_Sw = Main__St_24_Inactive;
      } else {
        s_12_St_24_Sw = v_1164;
      };
      if (v_1159) {
        v_1161 = true;
      } else {
        v_1161 = self->pnr_12;
      };
      if (v_1154) {
        v_1163 = true;
      } else {
        v_1163 = v_1161;
      };
      if (v_1152) {
        v_1165 = true;
      } else {
        v_1165 = v_1163;
      };
      if (e_1) {
        r_34_St_24_Sw = true;
      } else {
        r_34_St_24_Sw = v_1165;
      };
      s_12 = s_12_St_24_Sw;
      r_34 = r_34_St_24_Sw;
      break;
    case Main__St_24_Inactive:
      v_1166 = (r_1&&c1_1);
      v_1167 = (r_1&&c2_1);
      v_1168 = (r_1&&c3_1);
      v_1169 = !(c2_1);
      v_1170 = (r_1&&v_1169);
      v_1171 = !(c1_1);
      v_1172 = (v_1170&&v_1171);
      v_1173 = !(c3_1);
      v_1174 = (v_1172&&v_1173);
      if (v_1174) {
        v_1175 = Main__St_24_Wait;
      } else {
        v_1175 = Main__St_24_Inactive;
      };
      if (v_1168) {
        v_1177 = Main__St_24_Sw;
      } else {
        v_1177 = v_1175;
      };
      if (v_1167) {
        v_1179 = Main__St_24_Hw2;
      } else {
        v_1179 = v_1177;
      };
      if (v_1166) {
        s_12_St_24_Inactive = Main__St_24_Hw1;
      } else {
        s_12_St_24_Inactive = v_1179;
      };
      s_12 = s_12_St_24_Inactive;
      if (v_1174) {
        v_1176 = true;
      } else {
        v_1176 = self->pnr_12;
      };
      if (v_1168) {
        v_1178 = true;
      } else {
        v_1178 = v_1176;
      };
      if (v_1167) {
        v_1180 = true;
      } else {
        v_1180 = v_1178;
      };
      if (v_1166) {
        r_34_St_24_Inactive = true;
      } else {
        r_34_St_24_Inactive = v_1180;
      };
      r_34 = r_34_St_24_Inactive;
      break;
    default:
      break;
  };
  ck_26 = s_12;
  switch (ck_26) {
    case Main__St_24_Wait:
      ns_12_St_24_Wait = Main__St_24_Wait;
      nr_12_St_24_Wait = false;
      size_1_St_24_Wait = 0;
      act_4_St_24_Wait = false;
      wt_1_St_24_Wait = true;
      ver_1_St_24_Wait = Main__N;
      ns_12 = ns_12_St_24_Wait;
      nr_12 = nr_12_St_24_Wait;
      size_1 = size_1_St_24_Wait;
      act_4 = act_4_St_24_Wait;
      wt_1 = wt_1_St_24_Wait;
      ver_1 = ver_1_St_24_Wait;
      break;
    case Main__St_24_Hw2:
      ns_12_St_24_Hw2 = Main__St_24_Hw2;
      nr_12_St_24_Hw2 = false;
      size_1_St_24_Hw2 = 2;
      act_4_St_24_Hw2 = true;
      wt_1_St_24_Hw2 = false;
      ver_1_St_24_Hw2 = Main__H2;
      ns_12 = ns_12_St_24_Hw2;
      nr_12 = nr_12_St_24_Hw2;
      size_1 = size_1_St_24_Hw2;
      act_4 = act_4_St_24_Hw2;
      wt_1 = wt_1_St_24_Hw2;
      ver_1 = ver_1_St_24_Hw2;
      break;
    case Main__St_24_Hw1:
      ns_12_St_24_Hw1 = Main__St_24_Hw1;
      nr_12_St_24_Hw1 = false;
      size_1_St_24_Hw1 = 2;
      act_4_St_24_Hw1 = true;
      wt_1_St_24_Hw1 = false;
      ver_1_St_24_Hw1 = Main__H1;
      ns_12 = ns_12_St_24_Hw1;
      nr_12 = nr_12_St_24_Hw1;
      size_1 = size_1_St_24_Hw1;
      act_4 = act_4_St_24_Hw1;
      wt_1 = wt_1_St_24_Hw1;
      ver_1 = ver_1_St_24_Hw1;
      break;
    case Main__St_24_Sw:
      ns_12_St_24_Sw = Main__St_24_Sw;
      nr_12_St_24_Sw = false;
      size_1_St_24_Sw = 0;
      act_4_St_24_Sw = true;
      wt_1_St_24_Sw = false;
      ver_1_St_24_Sw = Main__S;
      ns_12 = ns_12_St_24_Sw;
      nr_12 = nr_12_St_24_Sw;
      size_1 = size_1_St_24_Sw;
      act_4 = act_4_St_24_Sw;
      wt_1 = wt_1_St_24_Sw;
      ver_1 = ver_1_St_24_Sw;
      break;
    case Main__St_24_Inactive:
      ns_12_St_24_Inactive = Main__St_24_Inactive;
      ns_12 = ns_12_St_24_Inactive;
      nr_12_St_24_Inactive = false;
      nr_12 = nr_12_St_24_Inactive;
      size_1_St_24_Inactive = 0;
      size_1 = size_1_St_24_Inactive;
      act_4_St_24_Inactive = false;
      act_4 = act_4_St_24_Inactive;
      wt_1_St_24_Inactive = false;
      wt_1 = wt_1_St_24_Inactive;
      ver_1_St_24_Inactive = Main__N;
      ver_1 = ver_1_St_24_Inactive;
      break;
    default:
      break;
  };
  _out->dt_size = size_1;
  _out->dt_act = act_4;
  v_950 = (v_949&&_out->dt_act);
  v_997 = (_out->dt_act&&v_996);
  v_999 = (v_997&&v_998);
  v_1006 = (_out->dt_act&&v_1005);
  v_1015 = (_out->dt_act&&v_1014);
  v_1017 = (v_1015&&v_1016);
  _out->dt_wt = wt_1;
  v_963 = !(_out->dt_wt);
  v_964 = (!(ct10)||v_963);
  _out->dt_ver = ver_1;
  v_916 = (_out->trk_ver==_out->dt_ver);
  v_917 = !(v_916);
  v_926 = (_out->trk_ver==_out->dt_ver);
  v_927 = !(v_926);
  v_922 = (_out->dt_ver==Main__S);
  v_923 = (_out->dt_ver==Main__N);
  v_924 = (v_922||v_923);
  v_925 = !(v_924);
  v_944 = (_out->dt_ver==Main__S);
  v_945 = (v_944&&_out->trk_act);
  v_947 = (!(v_945)||v_946);
  v_951 = (_out->dt_ver==Main__S);
  v_952 = (!(v_950)||v_951);
  ck_21 = _out->dt_ver;
  switch (ck_21) {
    case Main__N:
      wcet10_N = -1;
      _out->wcet10 = wcet10_N;
      break;
    case Main__H3:
      wcet10_H3 = _out->wcet10_hw3;
      _out->wcet10 = wcet10_H3;
      break;
    case Main__H2:
      wcet10_H2 = _out->wcet10_hw2;
      _out->wcet10 = wcet10_H2;
      break;
    case Main__H1:
      wcet10_H1 = _out->wcet10_hw1;
      _out->wcet10 = wcet10_H1;
      break;
    case Main__S:
      wcet10_S = _out->wcet10_sw;
      _out->wcet10 = wcet10_S;
      break;
    default:
      break;
  };
  v_1001 = (self->v_1000>_out->wcet10);
  v_1002 = (!(v_999)||v_1001);
  v_1003 = (!(cUp10)||v_1002);
  v_1008 = (self->v_1007<_out->wcet10);
  v_1009 = (v_1006&&v_1008);
  v_1010 = (_out->wcet10<maxtexe10);
  v_1011 = (!(v_1009)||v_1010);
  v_1012 = (!(cKp10)||v_1011);
  v_1019 = (self->v_1018<_out->wcet10);
  v_1020 = (!(v_1017)||v_1019);
  v_1021 = (!(cDw10)||v_1020);
  switch (self->ck_27) {
    case Main__St_23_Wait:
      if (c3_2) {
        v_1181 = Main__St_23_Sw;
      } else {
        v_1181 = Main__St_23_Wait;
      };
      if (c2_2) {
        v_1183 = Main__St_23_Hw2;
      } else {
        v_1183 = v_1181;
      };
      if (c1_2) {
        v_1185 = Main__St_23_Hw1;
      } else {
        v_1185 = v_1183;
      };
      if (e_2) {
        s_11_St_23_Wait = Main__St_23_Inactive;
      } else {
        s_11_St_23_Wait = v_1185;
      };
      if (c3_2) {
        v_1182 = true;
      } else {
        v_1182 = self->pnr_11;
      };
      if (c2_2) {
        v_1184 = true;
      } else {
        v_1184 = v_1182;
      };
      if (c1_2) {
        v_1186 = true;
      } else {
        v_1186 = v_1184;
      };
      if (e_2) {
        r_33_St_23_Wait = true;
      } else {
        r_33_St_23_Wait = v_1186;
      };
      s_11 = s_11_St_23_Wait;
      r_33 = r_33_St_23_Wait;
      break;
    case Main__St_23_Hw2:
      v_1187 = !(c2_2);
      v_1188 = (v_1187&&c1_2);
      v_1189 = !(c2_2);
      v_1190 = (v_1189&&c3_2);
      v_1191 = !(c2_2);
      v_1192 = !(c1_2);
      v_1193 = (v_1191&&v_1192);
      v_1194 = !(c3_2);
      v_1195 = (v_1193&&v_1194);
      if (v_1195) {
        v_1196 = Main__St_23_Wait;
      } else {
        v_1196 = Main__St_23_Hw2;
      };
      if (v_1190) {
        v_1198 = Main__St_23_Sw;
      } else {
        v_1198 = v_1196;
      };
      if (v_1188) {
        v_1200 = Main__St_23_Hw1;
      } else {
        v_1200 = v_1198;
      };
      if (e_2) {
        s_11_St_23_Hw2 = Main__St_23_Inactive;
      } else {
        s_11_St_23_Hw2 = v_1200;
      };
      if (v_1195) {
        v_1197 = true;
      } else {
        v_1197 = self->pnr_11;
      };
      if (v_1190) {
        v_1199 = true;
      } else {
        v_1199 = v_1197;
      };
      if (v_1188) {
        v_1201 = true;
      } else {
        v_1201 = v_1199;
      };
      if (e_2) {
        r_33_St_23_Hw2 = true;
      } else {
        r_33_St_23_Hw2 = v_1201;
      };
      s_11 = s_11_St_23_Hw2;
      r_33 = r_33_St_23_Hw2;
      break;
    case Main__St_23_Hw1:
      v_1202 = !(c1_2);
      v_1203 = (v_1202&&c2_2);
      v_1204 = !(c2_2);
      v_1205 = (v_1204&&c3_2);
      v_1206 = !(c2_2);
      v_1207 = !(c1_2);
      v_1208 = (v_1206&&v_1207);
      v_1209 = !(c3_2);
      v_1210 = (v_1208&&v_1209);
      if (v_1210) {
        v_1211 = Main__St_23_Wait;
      } else {
        v_1211 = Main__St_23_Hw1;
      };
      if (v_1205) {
        v_1213 = Main__St_23_Sw;
      } else {
        v_1213 = v_1211;
      };
      if (v_1203) {
        v_1215 = Main__St_23_Hw2;
      } else {
        v_1215 = v_1213;
      };
      if (e_2) {
        s_11_St_23_Hw1 = Main__St_23_Inactive;
      } else {
        s_11_St_23_Hw1 = v_1215;
      };
      if (v_1210) {
        v_1212 = true;
      } else {
        v_1212 = self->pnr_11;
      };
      if (v_1205) {
        v_1214 = true;
      } else {
        v_1214 = v_1212;
      };
      if (v_1203) {
        v_1216 = true;
      } else {
        v_1216 = v_1214;
      };
      if (e_2) {
        r_33_St_23_Hw1 = true;
      } else {
        r_33_St_23_Hw1 = v_1216;
      };
      s_11 = s_11_St_23_Hw1;
      r_33 = r_33_St_23_Hw1;
      break;
    case Main__St_23_Sw:
      v_1217 = !(c3_2);
      v_1218 = (v_1217&&c1_2);
      v_1219 = !(c3_2);
      v_1220 = (v_1219&&c2_2);
      v_1221 = !(c3_2);
      v_1222 = !(c2_2);
      v_1223 = (v_1221&&v_1222);
      v_1224 = !(c1_2);
      v_1225 = (v_1223&&v_1224);
      if (v_1225) {
        v_1226 = Main__St_23_Wait;
      } else {
        v_1226 = Main__St_23_Sw;
      };
      if (v_1220) {
        v_1228 = Main__St_23_Hw2;
      } else {
        v_1228 = v_1226;
      };
      if (v_1218) {
        v_1230 = Main__St_23_Hw1;
      } else {
        v_1230 = v_1228;
      };
      if (e_2) {
        s_11_St_23_Sw = Main__St_23_Inactive;
      } else {
        s_11_St_23_Sw = v_1230;
      };
      if (v_1225) {
        v_1227 = true;
      } else {
        v_1227 = self->pnr_11;
      };
      if (v_1220) {
        v_1229 = true;
      } else {
        v_1229 = v_1227;
      };
      if (v_1218) {
        v_1231 = true;
      } else {
        v_1231 = v_1229;
      };
      if (e_2) {
        r_33_St_23_Sw = true;
      } else {
        r_33_St_23_Sw = v_1231;
      };
      s_11 = s_11_St_23_Sw;
      r_33 = r_33_St_23_Sw;
      break;
    case Main__St_23_Inactive:
      v_1232 = (r_2&&c1_2);
      v_1233 = (r_2&&c2_2);
      v_1234 = (r_2&&c3_2);
      v_1235 = !(c2_2);
      v_1236 = (r_2&&v_1235);
      v_1237 = !(c1_2);
      v_1238 = (v_1236&&v_1237);
      v_1239 = !(c3_2);
      v_1240 = (v_1238&&v_1239);
      if (v_1240) {
        v_1241 = Main__St_23_Wait;
      } else {
        v_1241 = Main__St_23_Inactive;
      };
      if (v_1234) {
        v_1243 = Main__St_23_Sw;
      } else {
        v_1243 = v_1241;
      };
      if (v_1233) {
        v_1245 = Main__St_23_Hw2;
      } else {
        v_1245 = v_1243;
      };
      if (v_1232) {
        s_11_St_23_Inactive = Main__St_23_Hw1;
      } else {
        s_11_St_23_Inactive = v_1245;
      };
      s_11 = s_11_St_23_Inactive;
      if (v_1240) {
        v_1242 = true;
      } else {
        v_1242 = self->pnr_11;
      };
      if (v_1234) {
        v_1244 = true;
      } else {
        v_1244 = v_1242;
      };
      if (v_1233) {
        v_1246 = true;
      } else {
        v_1246 = v_1244;
      };
      if (v_1232) {
        r_33_St_23_Inactive = true;
      } else {
        r_33_St_23_Inactive = v_1246;
      };
      r_33 = r_33_St_23_Inactive;
      break;
    default:
      break;
  };
  ck_28 = s_11;
  switch (ck_28) {
    case Main__St_23_Wait:
      ns_11_St_23_Wait = Main__St_23_Wait;
      nr_11_St_23_Wait = false;
      size_2_St_23_Wait = 0;
      act_5_St_23_Wait = false;
      wt_2_St_23_Wait = true;
      ver_2_St_23_Wait = Main__N;
      ns_11 = ns_11_St_23_Wait;
      nr_11 = nr_11_St_23_Wait;
      size_2 = size_2_St_23_Wait;
      act_5 = act_5_St_23_Wait;
      wt_2 = wt_2_St_23_Wait;
      ver_2 = ver_2_St_23_Wait;
      break;
    case Main__St_23_Hw2:
      ns_11_St_23_Hw2 = Main__St_23_Hw2;
      nr_11_St_23_Hw2 = false;
      size_2_St_23_Hw2 = 2;
      act_5_St_23_Hw2 = true;
      wt_2_St_23_Hw2 = false;
      ver_2_St_23_Hw2 = Main__H2;
      ns_11 = ns_11_St_23_Hw2;
      nr_11 = nr_11_St_23_Hw2;
      size_2 = size_2_St_23_Hw2;
      act_5 = act_5_St_23_Hw2;
      wt_2 = wt_2_St_23_Hw2;
      ver_2 = ver_2_St_23_Hw2;
      break;
    case Main__St_23_Hw1:
      ns_11_St_23_Hw1 = Main__St_23_Hw1;
      nr_11_St_23_Hw1 = false;
      size_2_St_23_Hw1 = 2;
      act_5_St_23_Hw1 = true;
      wt_2_St_23_Hw1 = false;
      ver_2_St_23_Hw1 = Main__H1;
      ns_11 = ns_11_St_23_Hw1;
      nr_11 = nr_11_St_23_Hw1;
      size_2 = size_2_St_23_Hw1;
      act_5 = act_5_St_23_Hw1;
      wt_2 = wt_2_St_23_Hw1;
      ver_2 = ver_2_St_23_Hw1;
      break;
    case Main__St_23_Sw:
      ns_11_St_23_Sw = Main__St_23_Sw;
      nr_11_St_23_Sw = false;
      size_2_St_23_Sw = 0;
      act_5_St_23_Sw = true;
      wt_2_St_23_Sw = false;
      ver_2_St_23_Sw = Main__S;
      ns_11 = ns_11_St_23_Sw;
      nr_11 = nr_11_St_23_Sw;
      size_2 = size_2_St_23_Sw;
      act_5 = act_5_St_23_Sw;
      wt_2 = wt_2_St_23_Sw;
      ver_2 = ver_2_St_23_Sw;
      break;
    case Main__St_23_Inactive:
      ns_11_St_23_Inactive = Main__St_23_Inactive;
      ns_11 = ns_11_St_23_Inactive;
      nr_11_St_23_Inactive = false;
      nr_11 = nr_11_St_23_Inactive;
      size_2_St_23_Inactive = 0;
      size_2 = size_2_St_23_Inactive;
      act_5_St_23_Inactive = false;
      act_5 = act_5_St_23_Inactive;
      wt_2_St_23_Inactive = false;
      wt_2 = wt_2_St_23_Inactive;
      ver_2_St_23_Inactive = Main__N;
      ver_2 = ver_2_St_23_Inactive;
      break;
    default:
      break;
  };
  _out->rpl_size = size_2;
  _out->rpl_act = act_5;
  _out->rpl_wt = wt_2;
  _out->rpl_ver = ver_2;
  ck_20 = _out->rpl_ver;
  switch (ck_20) {
    case Main__N:
      wcet9_N = -1;
      _out->wcet9 = wcet9_N;
      break;
    case Main__H3:
      wcet9_H3 = _out->wcet9_hw3;
      _out->wcet9 = wcet9_H3;
      break;
    case Main__H2:
      wcet9_H2 = _out->wcet9_hw2;
      _out->wcet9 = wcet9_H2;
      break;
    case Main__H1:
      wcet9_H1 = _out->wcet9_hw1;
      _out->wcet9 = wcet9_H1;
      break;
    case Main__S:
      wcet9_S = _out->wcet9_sw;
      _out->wcet9 = wcet9_S;
      break;
    default:
      break;
  };
  switch (self->ck_29) {
    case Main__St_22_Wait:
      if (c2_3) {
        v_1247 = Main__St_22_Hw2;
      } else {
        v_1247 = Main__St_22_Wait;
      };
      if (c1_3) {
        v_1249 = Main__St_22_Hw1;
      } else {
        v_1249 = v_1247;
      };
      if (e_3) {
        s_10_St_22_Wait = Main__St_22_Inactive;
      } else {
        s_10_St_22_Wait = v_1249;
      };
      if (c2_3) {
        v_1248 = true;
      } else {
        v_1248 = self->pnr_10;
      };
      if (c1_3) {
        v_1250 = true;
      } else {
        v_1250 = v_1248;
      };
      if (e_3) {
        r_32_St_22_Wait = true;
      } else {
        r_32_St_22_Wait = v_1250;
      };
      s_10 = s_10_St_22_Wait;
      r_32 = r_32_St_22_Wait;
      break;
    case Main__St_22_Hw2:
      v_1251 = !(c2_3);
      v_1252 = (v_1251&&c1_3);
      v_1253 = !(c2_3);
      v_1254 = !(c1_3);
      v_1255 = (v_1253&&v_1254);
      if (v_1255) {
        v_1256 = Main__St_22_Wait;
      } else {
        v_1256 = Main__St_22_Hw2;
      };
      if (v_1252) {
        v_1258 = Main__St_22_Hw1;
      } else {
        v_1258 = v_1256;
      };
      if (e_3) {
        s_10_St_22_Hw2 = Main__St_22_Inactive;
      } else {
        s_10_St_22_Hw2 = v_1258;
      };
      if (v_1255) {
        v_1257 = true;
      } else {
        v_1257 = self->pnr_10;
      };
      if (v_1252) {
        v_1259 = true;
      } else {
        v_1259 = v_1257;
      };
      if (e_3) {
        r_32_St_22_Hw2 = true;
      } else {
        r_32_St_22_Hw2 = v_1259;
      };
      s_10 = s_10_St_22_Hw2;
      r_32 = r_32_St_22_Hw2;
      break;
    case Main__St_22_Hw1:
      v_1260 = !(c1_3);
      v_1261 = (v_1260&&c2_3);
      v_1262 = !(c2_3);
      v_1263 = !(c1_3);
      v_1264 = (v_1262&&v_1263);
      if (v_1264) {
        v_1265 = Main__St_22_Wait;
      } else {
        v_1265 = Main__St_22_Hw1;
      };
      if (v_1261) {
        v_1267 = Main__St_22_Hw2;
      } else {
        v_1267 = v_1265;
      };
      if (e_3) {
        s_10_St_22_Hw1 = Main__St_22_Inactive;
      } else {
        s_10_St_22_Hw1 = v_1267;
      };
      if (v_1264) {
        v_1266 = true;
      } else {
        v_1266 = self->pnr_10;
      };
      if (v_1261) {
        v_1268 = true;
      } else {
        v_1268 = v_1266;
      };
      if (e_3) {
        r_32_St_22_Hw1 = true;
      } else {
        r_32_St_22_Hw1 = v_1268;
      };
      s_10 = s_10_St_22_Hw1;
      r_32 = r_32_St_22_Hw1;
      break;
    case Main__St_22_Inactive:
      v_1269 = (r_3&&c1_3);
      v_1270 = (r_3&&c2_3);
      v_1271 = !(c2_3);
      v_1272 = (r_3&&v_1271);
      v_1273 = !(c1_3);
      v_1274 = (v_1272&&v_1273);
      if (v_1274) {
        v_1275 = Main__St_22_Wait;
      } else {
        v_1275 = Main__St_22_Inactive;
      };
      if (v_1270) {
        v_1277 = Main__St_22_Hw2;
      } else {
        v_1277 = v_1275;
      };
      if (v_1269) {
        s_10_St_22_Inactive = Main__St_22_Hw1;
      } else {
        s_10_St_22_Inactive = v_1277;
      };
      s_10 = s_10_St_22_Inactive;
      if (v_1274) {
        v_1276 = true;
      } else {
        v_1276 = self->pnr_10;
      };
      if (v_1270) {
        v_1278 = true;
      } else {
        v_1278 = v_1276;
      };
      if (v_1269) {
        r_32_St_22_Inactive = true;
      } else {
        r_32_St_22_Inactive = v_1278;
      };
      r_32 = r_32_St_22_Inactive;
      break;
    default:
      break;
  };
  ck_30 = s_10;
  switch (ck_30) {
    case Main__St_22_Wait:
      ns_10_St_22_Wait = Main__St_22_Wait;
      nr_10_St_22_Wait = false;
      size_3_St_22_Wait = 0;
      act_6_St_22_Wait = false;
      wt_3_St_22_Wait = true;
      ver_3_St_22_Wait = Main__N;
      ns_10 = ns_10_St_22_Wait;
      nr_10 = nr_10_St_22_Wait;
      size_3 = size_3_St_22_Wait;
      act_6 = act_6_St_22_Wait;
      wt_3 = wt_3_St_22_Wait;
      ver_3 = ver_3_St_22_Wait;
      break;
    case Main__St_22_Hw2:
      ns_10_St_22_Hw2 = Main__St_22_Hw2;
      nr_10_St_22_Hw2 = false;
      size_3_St_22_Hw2 = 2;
      act_6_St_22_Hw2 = true;
      wt_3_St_22_Hw2 = false;
      ver_3_St_22_Hw2 = Main__H2;
      ns_10 = ns_10_St_22_Hw2;
      nr_10 = nr_10_St_22_Hw2;
      size_3 = size_3_St_22_Hw2;
      act_6 = act_6_St_22_Hw2;
      wt_3 = wt_3_St_22_Hw2;
      ver_3 = ver_3_St_22_Hw2;
      break;
    case Main__St_22_Hw1:
      ns_10_St_22_Hw1 = Main__St_22_Hw1;
      nr_10_St_22_Hw1 = false;
      size_3_St_22_Hw1 = 2;
      act_6_St_22_Hw1 = true;
      wt_3_St_22_Hw1 = false;
      ver_3_St_22_Hw1 = Main__H1;
      ns_10 = ns_10_St_22_Hw1;
      nr_10 = nr_10_St_22_Hw1;
      size_3 = size_3_St_22_Hw1;
      act_6 = act_6_St_22_Hw1;
      wt_3 = wt_3_St_22_Hw1;
      ver_3 = ver_3_St_22_Hw1;
      break;
    case Main__St_22_Inactive:
      ns_10_St_22_Inactive = Main__St_22_Inactive;
      ns_10 = ns_10_St_22_Inactive;
      nr_10_St_22_Inactive = false;
      nr_10 = nr_10_St_22_Inactive;
      size_3_St_22_Inactive = 0;
      size_3 = size_3_St_22_Inactive;
      act_6_St_22_Inactive = false;
      act_6 = act_6_St_22_Inactive;
      wt_3_St_22_Inactive = false;
      wt_3 = wt_3_St_22_Inactive;
      ver_3_St_22_Inactive = Main__N;
      ver_3 = ver_3_St_22_Inactive;
      break;
    default:
      break;
  };
  _out->rb_size = size_3;
  _out->rb_act = act_6;
  _out->rb_wt = wt_3;
  _out->rb_ver = ver_3;
  ck_19 = _out->rb_ver;
  switch (ck_19) {
    case Main__N:
      wcet8_N = -1;
      _out->wcet8 = wcet8_N;
      break;
    case Main__H3:
      wcet8_H3 = _out->wcet8_hw3;
      _out->wcet8 = wcet8_H3;
      break;
    case Main__H2:
      wcet8_H2 = _out->wcet8_hw2;
      _out->wcet8 = wcet8_H2;
      break;
    case Main__H1:
      wcet8_H1 = _out->wcet8_hw1;
      _out->wcet8 = wcet8_H1;
      break;
    case Main__S:
      wcet8_S = _out->wcet8_sw;
      _out->wcet8 = wcet8_S;
      break;
    default:
      break;
  };
  switch (self->ck_31) {
    case Main__St_21_Wait:
      if (c4) {
        v_1279 = Main__St_21_Sw;
      } else {
        v_1279 = Main__St_21_Wait;
      };
      if (c3_3) {
        v_1281 = Main__St_21_Hw3;
      } else {
        v_1281 = v_1279;
      };
      if (c2_4) {
        v_1283 = Main__St_21_Hw2;
      } else {
        v_1283 = v_1281;
      };
      if (c1_4) {
        v_1285 = Main__St_21_Hw1;
      } else {
        v_1285 = v_1283;
      };
      if (e_4) {
        s_9_St_21_Wait = Main__St_21_Inactive;
      } else {
        s_9_St_21_Wait = v_1285;
      };
      if (c4) {
        v_1280 = true;
      } else {
        v_1280 = self->pnr_9;
      };
      if (c3_3) {
        v_1282 = true;
      } else {
        v_1282 = v_1280;
      };
      if (c2_4) {
        v_1284 = true;
      } else {
        v_1284 = v_1282;
      };
      if (c1_4) {
        v_1286 = true;
      } else {
        v_1286 = v_1284;
      };
      if (e_4) {
        r_31_St_21_Wait = true;
      } else {
        r_31_St_21_Wait = v_1286;
      };
      s_9 = s_9_St_21_Wait;
      r_31 = r_31_St_21_Wait;
      break;
    case Main__St_21_Hw3:
      v_1287 = !(c3_3);
      v_1288 = (v_1287&&c1_4);
      v_1289 = !(c3_3);
      v_1290 = (v_1289&&c2_4);
      v_1291 = !(c3_3);
      v_1292 = (v_1291&&c4);
      v_1293 = !(c3_3);
      v_1294 = !(c2_4);
      v_1295 = (v_1293&&v_1294);
      v_1296 = !(c1_4);
      v_1297 = (v_1295&&v_1296);
      v_1298 = !(c4);
      v_1299 = (v_1297&&v_1298);
      if (v_1299) {
        v_1300 = Main__St_21_Wait;
      } else {
        v_1300 = Main__St_21_Hw3;
      };
      if (v_1292) {
        v_1302 = Main__St_21_Sw;
      } else {
        v_1302 = v_1300;
      };
      if (v_1290) {
        v_1304 = Main__St_21_Hw2;
      } else {
        v_1304 = v_1302;
      };
      if (v_1288) {
        v_1306 = Main__St_21_Hw1;
      } else {
        v_1306 = v_1304;
      };
      if (e_4) {
        s_9_St_21_Hw3 = Main__St_21_Inactive;
      } else {
        s_9_St_21_Hw3 = v_1306;
      };
      if (v_1299) {
        v_1301 = true;
      } else {
        v_1301 = self->pnr_9;
      };
      if (v_1292) {
        v_1303 = true;
      } else {
        v_1303 = v_1301;
      };
      if (v_1290) {
        v_1305 = true;
      } else {
        v_1305 = v_1303;
      };
      if (v_1288) {
        v_1307 = true;
      } else {
        v_1307 = v_1305;
      };
      if (e_4) {
        r_31_St_21_Hw3 = true;
      } else {
        r_31_St_21_Hw3 = v_1307;
      };
      s_9 = s_9_St_21_Hw3;
      r_31 = r_31_St_21_Hw3;
      break;
    case Main__St_21_Hw2:
      v_1308 = !(c2_4);
      v_1309 = (v_1308&&c1_4);
      v_1310 = !(c2_4);
      v_1311 = (v_1310&&c3_3);
      v_1312 = !(c2_4);
      v_1313 = (v_1312&&c4);
      v_1314 = !(c3_3);
      v_1315 = !(c2_4);
      v_1316 = (v_1314&&v_1315);
      v_1317 = !(c1_4);
      v_1318 = (v_1316&&v_1317);
      v_1319 = !(c4);
      v_1320 = (v_1318&&v_1319);
      if (v_1320) {
        v_1321 = Main__St_21_Wait;
      } else {
        v_1321 = Main__St_21_Hw2;
      };
      if (v_1313) {
        v_1323 = Main__St_21_Sw;
      } else {
        v_1323 = v_1321;
      };
      if (v_1311) {
        v_1325 = Main__St_21_Hw3;
      } else {
        v_1325 = v_1323;
      };
      if (v_1309) {
        v_1327 = Main__St_21_Hw1;
      } else {
        v_1327 = v_1325;
      };
      if (e_4) {
        s_9_St_21_Hw2 = Main__St_21_Inactive;
      } else {
        s_9_St_21_Hw2 = v_1327;
      };
      if (v_1320) {
        v_1322 = true;
      } else {
        v_1322 = self->pnr_9;
      };
      if (v_1313) {
        v_1324 = true;
      } else {
        v_1324 = v_1322;
      };
      if (v_1311) {
        v_1326 = true;
      } else {
        v_1326 = v_1324;
      };
      if (v_1309) {
        v_1328 = true;
      } else {
        v_1328 = v_1326;
      };
      if (e_4) {
        r_31_St_21_Hw2 = true;
      } else {
        r_31_St_21_Hw2 = v_1328;
      };
      s_9 = s_9_St_21_Hw2;
      r_31 = r_31_St_21_Hw2;
      break;
    case Main__St_21_Hw1:
      v_1329 = !(c1_4);
      v_1330 = (v_1329&&c2_4);
      v_1331 = !(c1_4);
      v_1332 = (v_1331&&c3_3);
      v_1333 = !(c1_4);
      v_1334 = (v_1333&&c4);
      v_1335 = !(c3_3);
      v_1336 = !(c2_4);
      v_1337 = (v_1335&&v_1336);
      v_1338 = !(c1_4);
      v_1339 = (v_1337&&v_1338);
      v_1340 = !(c4);
      v_1341 = (v_1339&&v_1340);
      if (v_1341) {
        v_1342 = Main__St_21_Wait;
      } else {
        v_1342 = Main__St_21_Hw1;
      };
      if (v_1334) {
        v_1344 = Main__St_21_Sw;
      } else {
        v_1344 = v_1342;
      };
      if (v_1332) {
        v_1346 = Main__St_21_Hw3;
      } else {
        v_1346 = v_1344;
      };
      if (v_1330) {
        v_1348 = Main__St_21_Hw2;
      } else {
        v_1348 = v_1346;
      };
      if (e_4) {
        s_9_St_21_Hw1 = Main__St_21_Inactive;
      } else {
        s_9_St_21_Hw1 = v_1348;
      };
      if (v_1341) {
        v_1343 = true;
      } else {
        v_1343 = self->pnr_9;
      };
      if (v_1334) {
        v_1345 = true;
      } else {
        v_1345 = v_1343;
      };
      if (v_1332) {
        v_1347 = true;
      } else {
        v_1347 = v_1345;
      };
      if (v_1330) {
        v_1349 = true;
      } else {
        v_1349 = v_1347;
      };
      if (e_4) {
        r_31_St_21_Hw1 = true;
      } else {
        r_31_St_21_Hw1 = v_1349;
      };
      s_9 = s_9_St_21_Hw1;
      r_31 = r_31_St_21_Hw1;
      break;
    case Main__St_21_Sw:
      v_1350 = !(c4);
      v_1351 = (v_1350&&c1_4);
      v_1352 = !(c4);
      v_1353 = (v_1352&&c2_4);
      v_1354 = !(c4);
      v_1355 = (v_1354&&c3_3);
      v_1356 = !(c3_3);
      v_1357 = !(c2_4);
      v_1358 = (v_1356&&v_1357);
      v_1359 = !(c1_4);
      v_1360 = (v_1358&&v_1359);
      v_1361 = !(c4);
      v_1362 = (v_1360&&v_1361);
      if (v_1362) {
        v_1363 = Main__St_21_Wait;
      } else {
        v_1363 = Main__St_21_Sw;
      };
      if (v_1355) {
        v_1365 = Main__St_21_Hw3;
      } else {
        v_1365 = v_1363;
      };
      if (v_1353) {
        v_1367 = Main__St_21_Hw2;
      } else {
        v_1367 = v_1365;
      };
      if (v_1351) {
        v_1369 = Main__St_21_Hw1;
      } else {
        v_1369 = v_1367;
      };
      if (e_4) {
        s_9_St_21_Sw = Main__St_21_Inactive;
      } else {
        s_9_St_21_Sw = v_1369;
      };
      if (v_1362) {
        v_1364 = true;
      } else {
        v_1364 = self->pnr_9;
      };
      if (v_1355) {
        v_1366 = true;
      } else {
        v_1366 = v_1364;
      };
      if (v_1353) {
        v_1368 = true;
      } else {
        v_1368 = v_1366;
      };
      if (v_1351) {
        v_1370 = true;
      } else {
        v_1370 = v_1368;
      };
      if (e_4) {
        r_31_St_21_Sw = true;
      } else {
        r_31_St_21_Sw = v_1370;
      };
      s_9 = s_9_St_21_Sw;
      r_31 = r_31_St_21_Sw;
      break;
    case Main__St_21_Inactive:
      v_1371 = (r_4&&c1_4);
      v_1372 = (r_4&&c2_4);
      v_1373 = (r_4&&c3_3);
      v_1374 = (r_4&&c4);
      v_1375 = !(c3_3);
      v_1376 = (r_4&&v_1375);
      v_1377 = !(c2_4);
      v_1378 = (v_1376&&v_1377);
      v_1379 = !(c1_4);
      v_1380 = (v_1378&&v_1379);
      v_1381 = !(c4);
      v_1382 = (v_1380&&v_1381);
      if (v_1382) {
        v_1383 = Main__St_21_Wait;
      } else {
        v_1383 = Main__St_21_Inactive;
      };
      if (v_1374) {
        v_1385 = Main__St_21_Sw;
      } else {
        v_1385 = v_1383;
      };
      if (v_1373) {
        v_1387 = Main__St_21_Hw3;
      } else {
        v_1387 = v_1385;
      };
      if (v_1372) {
        v_1389 = Main__St_21_Hw2;
      } else {
        v_1389 = v_1387;
      };
      if (v_1371) {
        s_9_St_21_Inactive = Main__St_21_Hw1;
      } else {
        s_9_St_21_Inactive = v_1389;
      };
      s_9 = s_9_St_21_Inactive;
      if (v_1382) {
        v_1384 = true;
      } else {
        v_1384 = self->pnr_9;
      };
      if (v_1374) {
        v_1386 = true;
      } else {
        v_1386 = v_1384;
      };
      if (v_1373) {
        v_1388 = true;
      } else {
        v_1388 = v_1386;
      };
      if (v_1372) {
        v_1390 = true;
      } else {
        v_1390 = v_1388;
      };
      if (v_1371) {
        r_31_St_21_Inactive = true;
      } else {
        r_31_St_21_Inactive = v_1390;
      };
      r_31 = r_31_St_21_Inactive;
      break;
    default:
      break;
  };
  ck_32 = s_9;
  switch (ck_32) {
    case Main__St_21_Wait:
      ns_9_St_21_Wait = Main__St_21_Wait;
      nr_9_St_21_Wait = false;
      size_4_St_21_Wait = 0;
      act_7_St_21_Wait = false;
      wt_4_St_21_Wait = true;
      ver_4_St_21_Wait = Main__N;
      ns_9 = ns_9_St_21_Wait;
      nr_9 = nr_9_St_21_Wait;
      size_4 = size_4_St_21_Wait;
      act_7 = act_7_St_21_Wait;
      wt_4 = wt_4_St_21_Wait;
      ver_4 = ver_4_St_21_Wait;
      break;
    case Main__St_21_Hw3:
      ns_9_St_21_Hw3 = Main__St_21_Hw3;
      nr_9_St_21_Hw3 = false;
      size_4_St_21_Hw3 = 1;
      act_7_St_21_Hw3 = true;
      wt_4_St_21_Hw3 = false;
      ver_4_St_21_Hw3 = Main__H3;
      ns_9 = ns_9_St_21_Hw3;
      nr_9 = nr_9_St_21_Hw3;
      size_4 = size_4_St_21_Hw3;
      act_7 = act_7_St_21_Hw3;
      wt_4 = wt_4_St_21_Hw3;
      ver_4 = ver_4_St_21_Hw3;
      break;
    case Main__St_21_Hw2:
      ns_9_St_21_Hw2 = Main__St_21_Hw2;
      nr_9_St_21_Hw2 = false;
      size_4_St_21_Hw2 = 1;
      act_7_St_21_Hw2 = true;
      wt_4_St_21_Hw2 = false;
      ver_4_St_21_Hw2 = Main__H2;
      ns_9 = ns_9_St_21_Hw2;
      nr_9 = nr_9_St_21_Hw2;
      size_4 = size_4_St_21_Hw2;
      act_7 = act_7_St_21_Hw2;
      wt_4 = wt_4_St_21_Hw2;
      ver_4 = ver_4_St_21_Hw2;
      break;
    case Main__St_21_Hw1:
      ns_9_St_21_Hw1 = Main__St_21_Hw1;
      nr_9_St_21_Hw1 = false;
      size_4_St_21_Hw1 = 1;
      act_7_St_21_Hw1 = true;
      wt_4_St_21_Hw1 = false;
      ver_4_St_21_Hw1 = Main__H1;
      ns_9 = ns_9_St_21_Hw1;
      nr_9 = nr_9_St_21_Hw1;
      size_4 = size_4_St_21_Hw1;
      act_7 = act_7_St_21_Hw1;
      wt_4 = wt_4_St_21_Hw1;
      ver_4 = ver_4_St_21_Hw1;
      break;
    case Main__St_21_Sw:
      ns_9_St_21_Sw = Main__St_21_Sw;
      nr_9_St_21_Sw = false;
      size_4_St_21_Sw = 0;
      act_7_St_21_Sw = true;
      wt_4_St_21_Sw = false;
      ver_4_St_21_Sw = Main__S;
      ns_9 = ns_9_St_21_Sw;
      nr_9 = nr_9_St_21_Sw;
      size_4 = size_4_St_21_Sw;
      act_7 = act_7_St_21_Sw;
      wt_4 = wt_4_St_21_Sw;
      ver_4 = ver_4_St_21_Sw;
      break;
    case Main__St_21_Inactive:
      ns_9_St_21_Inactive = Main__St_21_Inactive;
      ns_9 = ns_9_St_21_Inactive;
      nr_9_St_21_Inactive = false;
      nr_9 = nr_9_St_21_Inactive;
      size_4_St_21_Inactive = 0;
      size_4 = size_4_St_21_Inactive;
      act_7_St_21_Inactive = false;
      act_7 = act_7_St_21_Inactive;
      wt_4_St_21_Inactive = false;
      wt_4 = wt_4_St_21_Inactive;
      ver_4_St_21_Inactive = Main__N;
      ver_4 = ver_4_St_21_Inactive;
      break;
    default:
      break;
  };
  _out->rs_size = size_4;
  _out->rs_act = act_7;
  _out->rs_wt = wt_4;
  _out->rs_ver = ver_4;
  ck_18 = _out->rs_ver;
  switch (ck_18) {
    case Main__N:
      wcet7_N = -1;
      _out->wcet7 = wcet7_N;
      break;
    case Main__H3:
      wcet7_H3 = _out->wcet7_hw3;
      _out->wcet7 = wcet7_H3;
      break;
    case Main__H2:
      wcet7_H2 = _out->wcet7_hw2;
      _out->wcet7 = wcet7_H2;
      break;
    case Main__H1:
      wcet7_H1 = _out->wcet7_hw1;
      _out->wcet7 = wcet7_H1;
      break;
    case Main__S:
      wcet7_S = _out->wcet7_sw;
      _out->wcet7 = wcet7_S;
      break;
    default:
      break;
  };
  switch (self->ck_33) {
    case Main__St_20_Wait:
      if (c4_1) {
        v_1391 = Main__St_20_Sw;
      } else {
        v_1391 = Main__St_20_Wait;
      };
      if (c3_4) {
        v_1393 = Main__St_20_Hw3;
      } else {
        v_1393 = v_1391;
      };
      if (c2_5) {
        v_1395 = Main__St_20_Hw2;
      } else {
        v_1395 = v_1393;
      };
      if (c1_5) {
        v_1397 = Main__St_20_Hw1;
      } else {
        v_1397 = v_1395;
      };
      if (e_5) {
        s_8_St_20_Wait = Main__St_20_Inactive;
      } else {
        s_8_St_20_Wait = v_1397;
      };
      if (c4_1) {
        v_1392 = true;
      } else {
        v_1392 = self->pnr_8;
      };
      if (c3_4) {
        v_1394 = true;
      } else {
        v_1394 = v_1392;
      };
      if (c2_5) {
        v_1396 = true;
      } else {
        v_1396 = v_1394;
      };
      if (c1_5) {
        v_1398 = true;
      } else {
        v_1398 = v_1396;
      };
      if (e_5) {
        r_30_St_20_Wait = true;
      } else {
        r_30_St_20_Wait = v_1398;
      };
      s_8 = s_8_St_20_Wait;
      r_30 = r_30_St_20_Wait;
      break;
    case Main__St_20_Hw3:
      v_1399 = !(c3_4);
      v_1400 = (v_1399&&c1_5);
      v_1401 = !(c3_4);
      v_1402 = (v_1401&&c2_5);
      v_1403 = !(c3_4);
      v_1404 = (v_1403&&c4_1);
      v_1405 = !(c3_4);
      v_1406 = !(c2_5);
      v_1407 = (v_1405&&v_1406);
      v_1408 = !(c1_5);
      v_1409 = (v_1407&&v_1408);
      v_1410 = !(c4_1);
      v_1411 = (v_1409&&v_1410);
      if (v_1411) {
        v_1412 = Main__St_20_Wait;
      } else {
        v_1412 = Main__St_20_Hw3;
      };
      if (v_1404) {
        v_1414 = Main__St_20_Sw;
      } else {
        v_1414 = v_1412;
      };
      if (v_1402) {
        v_1416 = Main__St_20_Hw2;
      } else {
        v_1416 = v_1414;
      };
      if (v_1400) {
        v_1418 = Main__St_20_Hw1;
      } else {
        v_1418 = v_1416;
      };
      if (e_5) {
        s_8_St_20_Hw3 = Main__St_20_Inactive;
      } else {
        s_8_St_20_Hw3 = v_1418;
      };
      if (v_1411) {
        v_1413 = true;
      } else {
        v_1413 = self->pnr_8;
      };
      if (v_1404) {
        v_1415 = true;
      } else {
        v_1415 = v_1413;
      };
      if (v_1402) {
        v_1417 = true;
      } else {
        v_1417 = v_1415;
      };
      if (v_1400) {
        v_1419 = true;
      } else {
        v_1419 = v_1417;
      };
      if (e_5) {
        r_30_St_20_Hw3 = true;
      } else {
        r_30_St_20_Hw3 = v_1419;
      };
      s_8 = s_8_St_20_Hw3;
      r_30 = r_30_St_20_Hw3;
      break;
    case Main__St_20_Hw2:
      v_1420 = !(c2_5);
      v_1421 = (v_1420&&c1_5);
      v_1422 = !(c2_5);
      v_1423 = (v_1422&&c3_4);
      v_1424 = !(c2_5);
      v_1425 = (v_1424&&c4_1);
      v_1426 = !(c3_4);
      v_1427 = !(c2_5);
      v_1428 = (v_1426&&v_1427);
      v_1429 = !(c1_5);
      v_1430 = (v_1428&&v_1429);
      v_1431 = !(c4_1);
      v_1432 = (v_1430&&v_1431);
      if (v_1432) {
        v_1433 = Main__St_20_Wait;
      } else {
        v_1433 = Main__St_20_Hw2;
      };
      if (v_1425) {
        v_1435 = Main__St_20_Sw;
      } else {
        v_1435 = v_1433;
      };
      if (v_1423) {
        v_1437 = Main__St_20_Hw3;
      } else {
        v_1437 = v_1435;
      };
      if (v_1421) {
        v_1439 = Main__St_20_Hw1;
      } else {
        v_1439 = v_1437;
      };
      if (e_5) {
        s_8_St_20_Hw2 = Main__St_20_Inactive;
      } else {
        s_8_St_20_Hw2 = v_1439;
      };
      if (v_1432) {
        v_1434 = true;
      } else {
        v_1434 = self->pnr_8;
      };
      if (v_1425) {
        v_1436 = true;
      } else {
        v_1436 = v_1434;
      };
      if (v_1423) {
        v_1438 = true;
      } else {
        v_1438 = v_1436;
      };
      if (v_1421) {
        v_1440 = true;
      } else {
        v_1440 = v_1438;
      };
      if (e_5) {
        r_30_St_20_Hw2 = true;
      } else {
        r_30_St_20_Hw2 = v_1440;
      };
      s_8 = s_8_St_20_Hw2;
      r_30 = r_30_St_20_Hw2;
      break;
    case Main__St_20_Hw1:
      v_1441 = !(c1_5);
      v_1442 = (v_1441&&c2_5);
      v_1443 = !(c1_5);
      v_1444 = (v_1443&&c3_4);
      v_1445 = !(c1_5);
      v_1446 = (v_1445&&c4_1);
      v_1447 = !(c3_4);
      v_1448 = !(c2_5);
      v_1449 = (v_1447&&v_1448);
      v_1450 = !(c1_5);
      v_1451 = (v_1449&&v_1450);
      v_1452 = !(c4_1);
      v_1453 = (v_1451&&v_1452);
      if (v_1453) {
        v_1454 = Main__St_20_Wait;
      } else {
        v_1454 = Main__St_20_Hw1;
      };
      if (v_1446) {
        v_1456 = Main__St_20_Sw;
      } else {
        v_1456 = v_1454;
      };
      if (v_1444) {
        v_1458 = Main__St_20_Hw3;
      } else {
        v_1458 = v_1456;
      };
      if (v_1442) {
        v_1460 = Main__St_20_Hw2;
      } else {
        v_1460 = v_1458;
      };
      if (e_5) {
        s_8_St_20_Hw1 = Main__St_20_Inactive;
      } else {
        s_8_St_20_Hw1 = v_1460;
      };
      if (v_1453) {
        v_1455 = true;
      } else {
        v_1455 = self->pnr_8;
      };
      if (v_1446) {
        v_1457 = true;
      } else {
        v_1457 = v_1455;
      };
      if (v_1444) {
        v_1459 = true;
      } else {
        v_1459 = v_1457;
      };
      if (v_1442) {
        v_1461 = true;
      } else {
        v_1461 = v_1459;
      };
      if (e_5) {
        r_30_St_20_Hw1 = true;
      } else {
        r_30_St_20_Hw1 = v_1461;
      };
      s_8 = s_8_St_20_Hw1;
      r_30 = r_30_St_20_Hw1;
      break;
    case Main__St_20_Sw:
      v_1462 = !(c4_1);
      v_1463 = (v_1462&&c1_5);
      v_1464 = !(c4_1);
      v_1465 = (v_1464&&c2_5);
      v_1466 = !(c4_1);
      v_1467 = (v_1466&&c3_4);
      v_1468 = !(c3_4);
      v_1469 = !(c2_5);
      v_1470 = (v_1468&&v_1469);
      v_1471 = !(c1_5);
      v_1472 = (v_1470&&v_1471);
      v_1473 = !(c4_1);
      v_1474 = (v_1472&&v_1473);
      if (v_1474) {
        v_1475 = Main__St_20_Wait;
      } else {
        v_1475 = Main__St_20_Sw;
      };
      if (v_1467) {
        v_1477 = Main__St_20_Hw3;
      } else {
        v_1477 = v_1475;
      };
      if (v_1465) {
        v_1479 = Main__St_20_Hw2;
      } else {
        v_1479 = v_1477;
      };
      if (v_1463) {
        v_1481 = Main__St_20_Hw1;
      } else {
        v_1481 = v_1479;
      };
      if (e_5) {
        s_8_St_20_Sw = Main__St_20_Inactive;
      } else {
        s_8_St_20_Sw = v_1481;
      };
      if (v_1474) {
        v_1476 = true;
      } else {
        v_1476 = self->pnr_8;
      };
      if (v_1467) {
        v_1478 = true;
      } else {
        v_1478 = v_1476;
      };
      if (v_1465) {
        v_1480 = true;
      } else {
        v_1480 = v_1478;
      };
      if (v_1463) {
        v_1482 = true;
      } else {
        v_1482 = v_1480;
      };
      if (e_5) {
        r_30_St_20_Sw = true;
      } else {
        r_30_St_20_Sw = v_1482;
      };
      s_8 = s_8_St_20_Sw;
      r_30 = r_30_St_20_Sw;
      break;
    case Main__St_20_Inactive:
      v_1483 = (r_5&&c1_5);
      v_1484 = (r_5&&c2_5);
      v_1485 = (r_5&&c3_4);
      v_1486 = (r_5&&c4_1);
      v_1487 = !(c3_4);
      v_1488 = (r_5&&v_1487);
      v_1489 = !(c2_5);
      v_1490 = (v_1488&&v_1489);
      v_1491 = !(c1_5);
      v_1492 = (v_1490&&v_1491);
      v_1493 = !(c4_1);
      v_1494 = (v_1492&&v_1493);
      if (v_1494) {
        v_1495 = Main__St_20_Wait;
      } else {
        v_1495 = Main__St_20_Inactive;
      };
      if (v_1486) {
        v_1497 = Main__St_20_Sw;
      } else {
        v_1497 = v_1495;
      };
      if (v_1485) {
        v_1499 = Main__St_20_Hw3;
      } else {
        v_1499 = v_1497;
      };
      if (v_1484) {
        v_1501 = Main__St_20_Hw2;
      } else {
        v_1501 = v_1499;
      };
      if (v_1483) {
        s_8_St_20_Inactive = Main__St_20_Hw1;
      } else {
        s_8_St_20_Inactive = v_1501;
      };
      s_8 = s_8_St_20_Inactive;
      if (v_1494) {
        v_1496 = true;
      } else {
        v_1496 = self->pnr_8;
      };
      if (v_1486) {
        v_1498 = true;
      } else {
        v_1498 = v_1496;
      };
      if (v_1485) {
        v_1500 = true;
      } else {
        v_1500 = v_1498;
      };
      if (v_1484) {
        v_1502 = true;
      } else {
        v_1502 = v_1500;
      };
      if (v_1483) {
        r_30_St_20_Inactive = true;
      } else {
        r_30_St_20_Inactive = v_1502;
      };
      r_30 = r_30_St_20_Inactive;
      break;
    default:
      break;
  };
  ck_34 = s_8;
  switch (ck_34) {
    case Main__St_20_Wait:
      ns_8_St_20_Wait = Main__St_20_Wait;
      nr_8_St_20_Wait = false;
      size_5_St_20_Wait = 0;
      act_8_St_20_Wait = false;
      wt_5_St_20_Wait = true;
      ver_5_St_20_Wait = Main__N;
      ns_8 = ns_8_St_20_Wait;
      nr_8 = nr_8_St_20_Wait;
      size_5 = size_5_St_20_Wait;
      act_8 = act_8_St_20_Wait;
      wt_5 = wt_5_St_20_Wait;
      ver_5 = ver_5_St_20_Wait;
      break;
    case Main__St_20_Hw3:
      ns_8_St_20_Hw3 = Main__St_20_Hw3;
      nr_8_St_20_Hw3 = false;
      size_5_St_20_Hw3 = 1;
      act_8_St_20_Hw3 = true;
      wt_5_St_20_Hw3 = false;
      ver_5_St_20_Hw3 = Main__H3;
      ns_8 = ns_8_St_20_Hw3;
      nr_8 = nr_8_St_20_Hw3;
      size_5 = size_5_St_20_Hw3;
      act_8 = act_8_St_20_Hw3;
      wt_5 = wt_5_St_20_Hw3;
      ver_5 = ver_5_St_20_Hw3;
      break;
    case Main__St_20_Hw2:
      ns_8_St_20_Hw2 = Main__St_20_Hw2;
      nr_8_St_20_Hw2 = false;
      size_5_St_20_Hw2 = 1;
      act_8_St_20_Hw2 = true;
      wt_5_St_20_Hw2 = false;
      ver_5_St_20_Hw2 = Main__H2;
      ns_8 = ns_8_St_20_Hw2;
      nr_8 = nr_8_St_20_Hw2;
      size_5 = size_5_St_20_Hw2;
      act_8 = act_8_St_20_Hw2;
      wt_5 = wt_5_St_20_Hw2;
      ver_5 = ver_5_St_20_Hw2;
      break;
    case Main__St_20_Hw1:
      ns_8_St_20_Hw1 = Main__St_20_Hw1;
      nr_8_St_20_Hw1 = false;
      size_5_St_20_Hw1 = 1;
      act_8_St_20_Hw1 = true;
      wt_5_St_20_Hw1 = false;
      ver_5_St_20_Hw1 = Main__H1;
      ns_8 = ns_8_St_20_Hw1;
      nr_8 = nr_8_St_20_Hw1;
      size_5 = size_5_St_20_Hw1;
      act_8 = act_8_St_20_Hw1;
      wt_5 = wt_5_St_20_Hw1;
      ver_5 = ver_5_St_20_Hw1;
      break;
    case Main__St_20_Sw:
      ns_8_St_20_Sw = Main__St_20_Sw;
      nr_8_St_20_Sw = false;
      size_5_St_20_Sw = 0;
      act_8_St_20_Sw = true;
      wt_5_St_20_Sw = false;
      ver_5_St_20_Sw = Main__S;
      ns_8 = ns_8_St_20_Sw;
      nr_8 = nr_8_St_20_Sw;
      size_5 = size_5_St_20_Sw;
      act_8 = act_8_St_20_Sw;
      wt_5 = wt_5_St_20_Sw;
      ver_5 = ver_5_St_20_Sw;
      break;
    case Main__St_20_Inactive:
      ns_8_St_20_Inactive = Main__St_20_Inactive;
      ns_8 = ns_8_St_20_Inactive;
      nr_8_St_20_Inactive = false;
      nr_8 = nr_8_St_20_Inactive;
      size_5_St_20_Inactive = 0;
      size_5 = size_5_St_20_Inactive;
      act_8_St_20_Inactive = false;
      act_8 = act_8_St_20_Inactive;
      wt_5_St_20_Inactive = false;
      wt_5 = wt_5_St_20_Inactive;
      ver_5_St_20_Inactive = Main__N;
      ver_5 = ver_5_St_20_Inactive;
      break;
    default:
      break;
  };
  _out->tl_size = size_5;
  _out->tl_act = act_8;
  _out->tl_wt = wt_5;
  v_960 = !(_out->tl_wt);
  v_961 = (!(ct6)||v_960);
  _out->tl_ver = ver_5;
  ck_17 = _out->tl_ver;
  switch (ck_17) {
    case Main__N:
      wcet6_N = -1;
      _out->wcet6 = wcet6_N;
      break;
    case Main__H3:
      wcet6_H3 = _out->wcet6_hw3;
      _out->wcet6 = wcet6_H3;
      break;
    case Main__H2:
      wcet6_H2 = _out->wcet6_hw2;
      _out->wcet6 = wcet6_H2;
      break;
    case Main__H1:
      wcet6_H1 = _out->wcet6_hw1;
      _out->wcet6 = wcet6_H1;
      break;
    case Main__S:
      wcet6_S = _out->wcet6_sw;
      _out->wcet6 = wcet6_S;
      break;
    default:
      break;
  };
  switch (self->ck_35) {
    case Main__St_19_Wait:
      if (c4_2) {
        v_1503 = Main__St_19_Sw;
      } else {
        v_1503 = Main__St_19_Wait;
      };
      if (c3_5) {
        v_1505 = Main__St_19_Hw3;
      } else {
        v_1505 = v_1503;
      };
      if (c2_6) {
        v_1507 = Main__St_19_Hw2;
      } else {
        v_1507 = v_1505;
      };
      if (c1_6) {
        v_1509 = Main__St_19_Hw1;
      } else {
        v_1509 = v_1507;
      };
      if (e_6) {
        s_7_St_19_Wait = Main__St_19_Inactive;
      } else {
        s_7_St_19_Wait = v_1509;
      };
      if (c4_2) {
        v_1504 = true;
      } else {
        v_1504 = self->pnr_7;
      };
      if (c3_5) {
        v_1506 = true;
      } else {
        v_1506 = v_1504;
      };
      if (c2_6) {
        v_1508 = true;
      } else {
        v_1508 = v_1506;
      };
      if (c1_6) {
        v_1510 = true;
      } else {
        v_1510 = v_1508;
      };
      if (e_6) {
        r_29_St_19_Wait = true;
      } else {
        r_29_St_19_Wait = v_1510;
      };
      s_7 = s_7_St_19_Wait;
      r_29 = r_29_St_19_Wait;
      break;
    case Main__St_19_Hw3:
      v_1511 = !(c3_5);
      v_1512 = (v_1511&&c1_6);
      v_1513 = !(c3_5);
      v_1514 = (v_1513&&c2_6);
      v_1515 = !(c3_5);
      v_1516 = (v_1515&&c4_2);
      v_1517 = !(c3_5);
      v_1518 = !(c2_6);
      v_1519 = (v_1517&&v_1518);
      v_1520 = !(c1_6);
      v_1521 = (v_1519&&v_1520);
      v_1522 = !(c4_2);
      v_1523 = (v_1521&&v_1522);
      if (v_1523) {
        v_1524 = Main__St_19_Wait;
      } else {
        v_1524 = Main__St_19_Hw3;
      };
      if (v_1516) {
        v_1526 = Main__St_19_Sw;
      } else {
        v_1526 = v_1524;
      };
      if (v_1514) {
        v_1528 = Main__St_19_Hw2;
      } else {
        v_1528 = v_1526;
      };
      if (v_1512) {
        v_1530 = Main__St_19_Hw1;
      } else {
        v_1530 = v_1528;
      };
      if (e_6) {
        s_7_St_19_Hw3 = Main__St_19_Inactive;
      } else {
        s_7_St_19_Hw3 = v_1530;
      };
      if (v_1523) {
        v_1525 = true;
      } else {
        v_1525 = self->pnr_7;
      };
      if (v_1516) {
        v_1527 = true;
      } else {
        v_1527 = v_1525;
      };
      if (v_1514) {
        v_1529 = true;
      } else {
        v_1529 = v_1527;
      };
      if (v_1512) {
        v_1531 = true;
      } else {
        v_1531 = v_1529;
      };
      if (e_6) {
        r_29_St_19_Hw3 = true;
      } else {
        r_29_St_19_Hw3 = v_1531;
      };
      s_7 = s_7_St_19_Hw3;
      r_29 = r_29_St_19_Hw3;
      break;
    case Main__St_19_Hw2:
      v_1532 = !(c2_6);
      v_1533 = (v_1532&&c1_6);
      v_1534 = !(c2_6);
      v_1535 = (v_1534&&c3_5);
      v_1536 = !(c2_6);
      v_1537 = (v_1536&&c4_2);
      v_1538 = !(c3_5);
      v_1539 = !(c2_6);
      v_1540 = (v_1538&&v_1539);
      v_1541 = !(c1_6);
      v_1542 = (v_1540&&v_1541);
      v_1543 = !(c4_2);
      v_1544 = (v_1542&&v_1543);
      if (v_1544) {
        v_1545 = Main__St_19_Wait;
      } else {
        v_1545 = Main__St_19_Hw2;
      };
      if (v_1537) {
        v_1547 = Main__St_19_Sw;
      } else {
        v_1547 = v_1545;
      };
      if (v_1535) {
        v_1549 = Main__St_19_Hw3;
      } else {
        v_1549 = v_1547;
      };
      if (v_1533) {
        v_1551 = Main__St_19_Hw1;
      } else {
        v_1551 = v_1549;
      };
      if (e_6) {
        s_7_St_19_Hw2 = Main__St_19_Inactive;
      } else {
        s_7_St_19_Hw2 = v_1551;
      };
      if (v_1544) {
        v_1546 = true;
      } else {
        v_1546 = self->pnr_7;
      };
      if (v_1537) {
        v_1548 = true;
      } else {
        v_1548 = v_1546;
      };
      if (v_1535) {
        v_1550 = true;
      } else {
        v_1550 = v_1548;
      };
      if (v_1533) {
        v_1552 = true;
      } else {
        v_1552 = v_1550;
      };
      if (e_6) {
        r_29_St_19_Hw2 = true;
      } else {
        r_29_St_19_Hw2 = v_1552;
      };
      s_7 = s_7_St_19_Hw2;
      r_29 = r_29_St_19_Hw2;
      break;
    case Main__St_19_Hw1:
      v_1553 = !(c1_6);
      v_1554 = (v_1553&&c2_6);
      v_1555 = !(c1_6);
      v_1556 = (v_1555&&c3_5);
      v_1557 = !(c1_6);
      v_1558 = (v_1557&&c4_2);
      v_1559 = !(c3_5);
      v_1560 = !(c2_6);
      v_1561 = (v_1559&&v_1560);
      v_1562 = !(c1_6);
      v_1563 = (v_1561&&v_1562);
      v_1564 = !(c4_2);
      v_1565 = (v_1563&&v_1564);
      if (v_1565) {
        v_1566 = Main__St_19_Wait;
      } else {
        v_1566 = Main__St_19_Hw1;
      };
      if (v_1558) {
        v_1568 = Main__St_19_Sw;
      } else {
        v_1568 = v_1566;
      };
      if (v_1556) {
        v_1570 = Main__St_19_Hw3;
      } else {
        v_1570 = v_1568;
      };
      if (v_1554) {
        v_1572 = Main__St_19_Hw2;
      } else {
        v_1572 = v_1570;
      };
      if (e_6) {
        s_7_St_19_Hw1 = Main__St_19_Inactive;
      } else {
        s_7_St_19_Hw1 = v_1572;
      };
      if (v_1565) {
        v_1567 = true;
      } else {
        v_1567 = self->pnr_7;
      };
      if (v_1558) {
        v_1569 = true;
      } else {
        v_1569 = v_1567;
      };
      if (v_1556) {
        v_1571 = true;
      } else {
        v_1571 = v_1569;
      };
      if (v_1554) {
        v_1573 = true;
      } else {
        v_1573 = v_1571;
      };
      if (e_6) {
        r_29_St_19_Hw1 = true;
      } else {
        r_29_St_19_Hw1 = v_1573;
      };
      s_7 = s_7_St_19_Hw1;
      r_29 = r_29_St_19_Hw1;
      break;
    case Main__St_19_Sw:
      v_1574 = !(c4_2);
      v_1575 = (v_1574&&c1_6);
      v_1576 = !(c4_2);
      v_1577 = (v_1576&&c2_6);
      v_1578 = !(c4_2);
      v_1579 = (v_1578&&c3_5);
      v_1580 = !(c3_5);
      v_1581 = !(c2_6);
      v_1582 = (v_1580&&v_1581);
      v_1583 = !(c1_6);
      v_1584 = (v_1582&&v_1583);
      v_1585 = !(c4_2);
      v_1586 = (v_1584&&v_1585);
      if (v_1586) {
        v_1587 = Main__St_19_Wait;
      } else {
        v_1587 = Main__St_19_Sw;
      };
      if (v_1579) {
        v_1589 = Main__St_19_Hw3;
      } else {
        v_1589 = v_1587;
      };
      if (v_1577) {
        v_1591 = Main__St_19_Hw2;
      } else {
        v_1591 = v_1589;
      };
      if (v_1575) {
        v_1593 = Main__St_19_Hw1;
      } else {
        v_1593 = v_1591;
      };
      if (e_6) {
        s_7_St_19_Sw = Main__St_19_Inactive;
      } else {
        s_7_St_19_Sw = v_1593;
      };
      if (v_1586) {
        v_1588 = true;
      } else {
        v_1588 = self->pnr_7;
      };
      if (v_1579) {
        v_1590 = true;
      } else {
        v_1590 = v_1588;
      };
      if (v_1577) {
        v_1592 = true;
      } else {
        v_1592 = v_1590;
      };
      if (v_1575) {
        v_1594 = true;
      } else {
        v_1594 = v_1592;
      };
      if (e_6) {
        r_29_St_19_Sw = true;
      } else {
        r_29_St_19_Sw = v_1594;
      };
      s_7 = s_7_St_19_Sw;
      r_29 = r_29_St_19_Sw;
      break;
    case Main__St_19_Inactive:
      v_1595 = (r_6&&c1_6);
      v_1596 = (r_6&&c2_6);
      v_1597 = (r_6&&c3_5);
      v_1598 = (r_6&&c4_2);
      v_1599 = !(c3_5);
      v_1600 = (r_6&&v_1599);
      v_1601 = !(c2_6);
      v_1602 = (v_1600&&v_1601);
      v_1603 = !(c1_6);
      v_1604 = (v_1602&&v_1603);
      v_1605 = !(c4_2);
      v_1606 = (v_1604&&v_1605);
      if (v_1606) {
        v_1607 = Main__St_19_Wait;
      } else {
        v_1607 = Main__St_19_Inactive;
      };
      if (v_1598) {
        v_1609 = Main__St_19_Sw;
      } else {
        v_1609 = v_1607;
      };
      if (v_1597) {
        v_1611 = Main__St_19_Hw3;
      } else {
        v_1611 = v_1609;
      };
      if (v_1596) {
        v_1613 = Main__St_19_Hw2;
      } else {
        v_1613 = v_1611;
      };
      if (v_1595) {
        s_7_St_19_Inactive = Main__St_19_Hw1;
      } else {
        s_7_St_19_Inactive = v_1613;
      };
      s_7 = s_7_St_19_Inactive;
      if (v_1606) {
        v_1608 = true;
      } else {
        v_1608 = self->pnr_7;
      };
      if (v_1598) {
        v_1610 = true;
      } else {
        v_1610 = v_1608;
      };
      if (v_1597) {
        v_1612 = true;
      } else {
        v_1612 = v_1610;
      };
      if (v_1596) {
        v_1614 = true;
      } else {
        v_1614 = v_1612;
      };
      if (v_1595) {
        r_29_St_19_Inactive = true;
      } else {
        r_29_St_19_Inactive = v_1614;
      };
      r_29 = r_29_St_19_Inactive;
      break;
    default:
      break;
  };
  ck_36 = s_7;
  switch (ck_36) {
    case Main__St_19_Wait:
      ns_7_St_19_Wait = Main__St_19_Wait;
      nr_7_St_19_Wait = false;
      size_6_St_19_Wait = 0;
      act_9_St_19_Wait = false;
      wt_6_St_19_Wait = true;
      ver_6_St_19_Wait = Main__N;
      ns_7 = ns_7_St_19_Wait;
      nr_7 = nr_7_St_19_Wait;
      size_6 = size_6_St_19_Wait;
      act_9 = act_9_St_19_Wait;
      wt_6 = wt_6_St_19_Wait;
      ver_6 = ver_6_St_19_Wait;
      break;
    case Main__St_19_Hw3:
      ns_7_St_19_Hw3 = Main__St_19_Hw3;
      nr_7_St_19_Hw3 = false;
      size_6_St_19_Hw3 = 1;
      act_9_St_19_Hw3 = true;
      wt_6_St_19_Hw3 = false;
      ver_6_St_19_Hw3 = Main__H3;
      ns_7 = ns_7_St_19_Hw3;
      nr_7 = nr_7_St_19_Hw3;
      size_6 = size_6_St_19_Hw3;
      act_9 = act_9_St_19_Hw3;
      wt_6 = wt_6_St_19_Hw3;
      ver_6 = ver_6_St_19_Hw3;
      break;
    case Main__St_19_Hw2:
      ns_7_St_19_Hw2 = Main__St_19_Hw2;
      nr_7_St_19_Hw2 = false;
      size_6_St_19_Hw2 = 1;
      act_9_St_19_Hw2 = true;
      wt_6_St_19_Hw2 = false;
      ver_6_St_19_Hw2 = Main__H2;
      ns_7 = ns_7_St_19_Hw2;
      nr_7 = nr_7_St_19_Hw2;
      size_6 = size_6_St_19_Hw2;
      act_9 = act_9_St_19_Hw2;
      wt_6 = wt_6_St_19_Hw2;
      ver_6 = ver_6_St_19_Hw2;
      break;
    case Main__St_19_Hw1:
      ns_7_St_19_Hw1 = Main__St_19_Hw1;
      nr_7_St_19_Hw1 = false;
      size_6_St_19_Hw1 = 1;
      act_9_St_19_Hw1 = true;
      wt_6_St_19_Hw1 = false;
      ver_6_St_19_Hw1 = Main__H1;
      ns_7 = ns_7_St_19_Hw1;
      nr_7 = nr_7_St_19_Hw1;
      size_6 = size_6_St_19_Hw1;
      act_9 = act_9_St_19_Hw1;
      wt_6 = wt_6_St_19_Hw1;
      ver_6 = ver_6_St_19_Hw1;
      break;
    case Main__St_19_Sw:
      ns_7_St_19_Sw = Main__St_19_Sw;
      nr_7_St_19_Sw = false;
      size_6_St_19_Sw = 0;
      act_9_St_19_Sw = true;
      wt_6_St_19_Sw = false;
      ver_6_St_19_Sw = Main__S;
      ns_7 = ns_7_St_19_Sw;
      nr_7 = nr_7_St_19_Sw;
      size_6 = size_6_St_19_Sw;
      act_9 = act_9_St_19_Sw;
      wt_6 = wt_6_St_19_Sw;
      ver_6 = ver_6_St_19_Sw;
      break;
    case Main__St_19_Inactive:
      ns_7_St_19_Inactive = Main__St_19_Inactive;
      ns_7 = ns_7_St_19_Inactive;
      nr_7_St_19_Inactive = false;
      nr_7 = nr_7_St_19_Inactive;
      size_6_St_19_Inactive = 0;
      size_6 = size_6_St_19_Inactive;
      act_9_St_19_Inactive = false;
      act_9 = act_9_St_19_Inactive;
      wt_6_St_19_Inactive = false;
      wt_6 = wt_6_St_19_Inactive;
      ver_6_St_19_Inactive = Main__N;
      ver_6 = ver_6_St_19_Inactive;
      break;
    default:
      break;
  };
  _out->oa_size = size_6;
  _out->oa_act = act_9;
  _out->oa_wt = wt_6;
  v_957 = !(_out->oa_wt);
  v_958 = (!(ct5)||v_957);
  _out->oa_ver = ver_6;
  ck_16 = _out->oa_ver;
  switch (ck_16) {
    case Main__N:
      wcet5_N = -1;
      _out->wcet5 = wcet5_N;
      break;
    case Main__H3:
      wcet5_H3 = _out->wcet5_hw3;
      _out->wcet5 = wcet5_H3;
      break;
    case Main__H2:
      wcet5_H2 = _out->wcet5_hw2;
      _out->wcet5 = wcet5_H2;
      break;
    case Main__H1:
      wcet5_H1 = _out->wcet5_hw1;
      _out->wcet5 = wcet5_H1;
      break;
    case Main__S:
      wcet5_S = _out->wcet5_sw;
      _out->wcet5 = wcet5_S;
      break;
    default:
      break;
  };
  switch (self->ck_37) {
    case Main__St_18_Wait:
      if (c4_3) {
        v_1615 = Main__St_18_Sw;
      } else {
        v_1615 = Main__St_18_Wait;
      };
      if (c3_6) {
        v_1617 = Main__St_18_Hw3;
      } else {
        v_1617 = v_1615;
      };
      if (c2_7) {
        v_1619 = Main__St_18_Hw2;
      } else {
        v_1619 = v_1617;
      };
      if (c1_7) {
        v_1621 = Main__St_18_Hw1;
      } else {
        v_1621 = v_1619;
      };
      if (e_7) {
        s_6_St_18_Wait = Main__St_18_Inactive;
      } else {
        s_6_St_18_Wait = v_1621;
      };
      if (c4_3) {
        v_1616 = true;
      } else {
        v_1616 = self->pnr_6;
      };
      if (c3_6) {
        v_1618 = true;
      } else {
        v_1618 = v_1616;
      };
      if (c2_7) {
        v_1620 = true;
      } else {
        v_1620 = v_1618;
      };
      if (c1_7) {
        v_1622 = true;
      } else {
        v_1622 = v_1620;
      };
      if (e_7) {
        r_28_St_18_Wait = true;
      } else {
        r_28_St_18_Wait = v_1622;
      };
      s_6 = s_6_St_18_Wait;
      r_28 = r_28_St_18_Wait;
      break;
    case Main__St_18_Hw3:
      v_1623 = !(c3_6);
      v_1624 = (v_1623&&c1_7);
      v_1625 = !(c3_6);
      v_1626 = (v_1625&&c2_7);
      v_1627 = !(c3_6);
      v_1628 = (v_1627&&c4_3);
      v_1629 = !(c3_6);
      v_1630 = !(c2_7);
      v_1631 = (v_1629&&v_1630);
      v_1632 = !(c1_7);
      v_1633 = (v_1631&&v_1632);
      v_1634 = !(c4_3);
      v_1635 = (v_1633&&v_1634);
      if (v_1635) {
        v_1636 = Main__St_18_Wait;
      } else {
        v_1636 = Main__St_18_Hw3;
      };
      if (v_1628) {
        v_1638 = Main__St_18_Sw;
      } else {
        v_1638 = v_1636;
      };
      if (v_1626) {
        v_1640 = Main__St_18_Hw2;
      } else {
        v_1640 = v_1638;
      };
      if (v_1624) {
        v_1642 = Main__St_18_Hw1;
      } else {
        v_1642 = v_1640;
      };
      if (e_7) {
        s_6_St_18_Hw3 = Main__St_18_Inactive;
      } else {
        s_6_St_18_Hw3 = v_1642;
      };
      if (v_1635) {
        v_1637 = true;
      } else {
        v_1637 = self->pnr_6;
      };
      if (v_1628) {
        v_1639 = true;
      } else {
        v_1639 = v_1637;
      };
      if (v_1626) {
        v_1641 = true;
      } else {
        v_1641 = v_1639;
      };
      if (v_1624) {
        v_1643 = true;
      } else {
        v_1643 = v_1641;
      };
      if (e_7) {
        r_28_St_18_Hw3 = true;
      } else {
        r_28_St_18_Hw3 = v_1643;
      };
      s_6 = s_6_St_18_Hw3;
      r_28 = r_28_St_18_Hw3;
      break;
    case Main__St_18_Hw2:
      v_1644 = !(c2_7);
      v_1645 = (v_1644&&c1_7);
      v_1646 = !(c2_7);
      v_1647 = (v_1646&&c3_6);
      v_1648 = !(c2_7);
      v_1649 = (v_1648&&c4_3);
      v_1650 = !(c3_6);
      v_1651 = !(c2_7);
      v_1652 = (v_1650&&v_1651);
      v_1653 = !(c1_7);
      v_1654 = (v_1652&&v_1653);
      v_1655 = !(c4_3);
      v_1656 = (v_1654&&v_1655);
      if (v_1656) {
        v_1657 = Main__St_18_Wait;
      } else {
        v_1657 = Main__St_18_Hw2;
      };
      if (v_1649) {
        v_1659 = Main__St_18_Sw;
      } else {
        v_1659 = v_1657;
      };
      if (v_1647) {
        v_1661 = Main__St_18_Hw3;
      } else {
        v_1661 = v_1659;
      };
      if (v_1645) {
        v_1663 = Main__St_18_Hw1;
      } else {
        v_1663 = v_1661;
      };
      if (e_7) {
        s_6_St_18_Hw2 = Main__St_18_Inactive;
      } else {
        s_6_St_18_Hw2 = v_1663;
      };
      if (v_1656) {
        v_1658 = true;
      } else {
        v_1658 = self->pnr_6;
      };
      if (v_1649) {
        v_1660 = true;
      } else {
        v_1660 = v_1658;
      };
      if (v_1647) {
        v_1662 = true;
      } else {
        v_1662 = v_1660;
      };
      if (v_1645) {
        v_1664 = true;
      } else {
        v_1664 = v_1662;
      };
      if (e_7) {
        r_28_St_18_Hw2 = true;
      } else {
        r_28_St_18_Hw2 = v_1664;
      };
      s_6 = s_6_St_18_Hw2;
      r_28 = r_28_St_18_Hw2;
      break;
    case Main__St_18_Hw1:
      v_1665 = !(c1_7);
      v_1666 = (v_1665&&c2_7);
      v_1667 = !(c1_7);
      v_1668 = (v_1667&&c3_6);
      v_1669 = !(c1_7);
      v_1670 = (v_1669&&c4_3);
      v_1671 = !(c3_6);
      v_1672 = !(c2_7);
      v_1673 = (v_1671&&v_1672);
      v_1674 = !(c1_7);
      v_1675 = (v_1673&&v_1674);
      v_1676 = !(c4_3);
      v_1677 = (v_1675&&v_1676);
      if (v_1677) {
        v_1678 = Main__St_18_Wait;
      } else {
        v_1678 = Main__St_18_Hw1;
      };
      if (v_1670) {
        v_1680 = Main__St_18_Sw;
      } else {
        v_1680 = v_1678;
      };
      if (v_1668) {
        v_1682 = Main__St_18_Hw3;
      } else {
        v_1682 = v_1680;
      };
      if (v_1666) {
        v_1684 = Main__St_18_Hw2;
      } else {
        v_1684 = v_1682;
      };
      if (e_7) {
        s_6_St_18_Hw1 = Main__St_18_Inactive;
      } else {
        s_6_St_18_Hw1 = v_1684;
      };
      if (v_1677) {
        v_1679 = true;
      } else {
        v_1679 = self->pnr_6;
      };
      if (v_1670) {
        v_1681 = true;
      } else {
        v_1681 = v_1679;
      };
      if (v_1668) {
        v_1683 = true;
      } else {
        v_1683 = v_1681;
      };
      if (v_1666) {
        v_1685 = true;
      } else {
        v_1685 = v_1683;
      };
      if (e_7) {
        r_28_St_18_Hw1 = true;
      } else {
        r_28_St_18_Hw1 = v_1685;
      };
      s_6 = s_6_St_18_Hw1;
      r_28 = r_28_St_18_Hw1;
      break;
    case Main__St_18_Sw:
      v_1686 = !(c4_3);
      v_1687 = (v_1686&&c1_7);
      v_1688 = !(c4_3);
      v_1689 = (v_1688&&c2_7);
      v_1690 = !(c4_3);
      v_1691 = (v_1690&&c3_6);
      v_1692 = !(c3_6);
      v_1693 = !(c2_7);
      v_1694 = (v_1692&&v_1693);
      v_1695 = !(c1_7);
      v_1696 = (v_1694&&v_1695);
      v_1697 = !(c4_3);
      v_1698 = (v_1696&&v_1697);
      if (v_1698) {
        v_1699 = Main__St_18_Wait;
      } else {
        v_1699 = Main__St_18_Sw;
      };
      if (v_1691) {
        v_1701 = Main__St_18_Hw3;
      } else {
        v_1701 = v_1699;
      };
      if (v_1689) {
        v_1703 = Main__St_18_Hw2;
      } else {
        v_1703 = v_1701;
      };
      if (v_1687) {
        v_1705 = Main__St_18_Hw1;
      } else {
        v_1705 = v_1703;
      };
      if (e_7) {
        s_6_St_18_Sw = Main__St_18_Inactive;
      } else {
        s_6_St_18_Sw = v_1705;
      };
      if (v_1698) {
        v_1700 = true;
      } else {
        v_1700 = self->pnr_6;
      };
      if (v_1691) {
        v_1702 = true;
      } else {
        v_1702 = v_1700;
      };
      if (v_1689) {
        v_1704 = true;
      } else {
        v_1704 = v_1702;
      };
      if (v_1687) {
        v_1706 = true;
      } else {
        v_1706 = v_1704;
      };
      if (e_7) {
        r_28_St_18_Sw = true;
      } else {
        r_28_St_18_Sw = v_1706;
      };
      s_6 = s_6_St_18_Sw;
      r_28 = r_28_St_18_Sw;
      break;
    case Main__St_18_Inactive:
      v_1707 = (r_7&&c1_7);
      v_1708 = (r_7&&c2_7);
      v_1709 = (r_7&&c3_6);
      v_1710 = (r_7&&c4_3);
      v_1711 = !(c3_6);
      v_1712 = (r_7&&v_1711);
      v_1713 = !(c2_7);
      v_1714 = (v_1712&&v_1713);
      v_1715 = !(c1_7);
      v_1716 = (v_1714&&v_1715);
      v_1717 = !(c4_3);
      v_1718 = (v_1716&&v_1717);
      if (v_1718) {
        v_1719 = Main__St_18_Wait;
      } else {
        v_1719 = Main__St_18_Inactive;
      };
      if (v_1710) {
        v_1721 = Main__St_18_Sw;
      } else {
        v_1721 = v_1719;
      };
      if (v_1709) {
        v_1723 = Main__St_18_Hw3;
      } else {
        v_1723 = v_1721;
      };
      if (v_1708) {
        v_1725 = Main__St_18_Hw2;
      } else {
        v_1725 = v_1723;
      };
      if (v_1707) {
        s_6_St_18_Inactive = Main__St_18_Hw1;
      } else {
        s_6_St_18_Inactive = v_1725;
      };
      s_6 = s_6_St_18_Inactive;
      if (v_1718) {
        v_1720 = true;
      } else {
        v_1720 = self->pnr_6;
      };
      if (v_1710) {
        v_1722 = true;
      } else {
        v_1722 = v_1720;
      };
      if (v_1709) {
        v_1724 = true;
      } else {
        v_1724 = v_1722;
      };
      if (v_1708) {
        v_1726 = true;
      } else {
        v_1726 = v_1724;
      };
      if (v_1707) {
        r_28_St_18_Inactive = true;
      } else {
        r_28_St_18_Inactive = v_1726;
      };
      r_28 = r_28_St_18_Inactive;
      break;
    default:
      break;
  };
  ck_38 = s_6;
  switch (ck_38) {
    case Main__St_18_Wait:
      ns_6_St_18_Wait = Main__St_18_Wait;
      nr_6_St_18_Wait = false;
      size_7_St_18_Wait = 0;
      act_10_St_18_Wait = false;
      wt_7_St_18_Wait = true;
      ver_7_St_18_Wait = Main__N;
      ns_6 = ns_6_St_18_Wait;
      nr_6 = nr_6_St_18_Wait;
      size_7 = size_7_St_18_Wait;
      act_10 = act_10_St_18_Wait;
      wt_7 = wt_7_St_18_Wait;
      ver_7 = ver_7_St_18_Wait;
      break;
    case Main__St_18_Hw3:
      ns_6_St_18_Hw3 = Main__St_18_Hw3;
      nr_6_St_18_Hw3 = false;
      size_7_St_18_Hw3 = 1;
      act_10_St_18_Hw3 = true;
      wt_7_St_18_Hw3 = false;
      ver_7_St_18_Hw3 = Main__H3;
      ns_6 = ns_6_St_18_Hw3;
      nr_6 = nr_6_St_18_Hw3;
      size_7 = size_7_St_18_Hw3;
      act_10 = act_10_St_18_Hw3;
      wt_7 = wt_7_St_18_Hw3;
      ver_7 = ver_7_St_18_Hw3;
      break;
    case Main__St_18_Hw2:
      ns_6_St_18_Hw2 = Main__St_18_Hw2;
      nr_6_St_18_Hw2 = false;
      size_7_St_18_Hw2 = 1;
      act_10_St_18_Hw2 = true;
      wt_7_St_18_Hw2 = false;
      ver_7_St_18_Hw2 = Main__H2;
      ns_6 = ns_6_St_18_Hw2;
      nr_6 = nr_6_St_18_Hw2;
      size_7 = size_7_St_18_Hw2;
      act_10 = act_10_St_18_Hw2;
      wt_7 = wt_7_St_18_Hw2;
      ver_7 = ver_7_St_18_Hw2;
      break;
    case Main__St_18_Hw1:
      ns_6_St_18_Hw1 = Main__St_18_Hw1;
      nr_6_St_18_Hw1 = false;
      size_7_St_18_Hw1 = 1;
      act_10_St_18_Hw1 = true;
      wt_7_St_18_Hw1 = false;
      ver_7_St_18_Hw1 = Main__H1;
      ns_6 = ns_6_St_18_Hw1;
      nr_6 = nr_6_St_18_Hw1;
      size_7 = size_7_St_18_Hw1;
      act_10 = act_10_St_18_Hw1;
      wt_7 = wt_7_St_18_Hw1;
      ver_7 = ver_7_St_18_Hw1;
      break;
    case Main__St_18_Sw:
      ns_6_St_18_Sw = Main__St_18_Sw;
      nr_6_St_18_Sw = false;
      size_7_St_18_Sw = 0;
      act_10_St_18_Sw = true;
      wt_7_St_18_Sw = false;
      ver_7_St_18_Sw = Main__S;
      ns_6 = ns_6_St_18_Sw;
      nr_6 = nr_6_St_18_Sw;
      size_7 = size_7_St_18_Sw;
      act_10 = act_10_St_18_Sw;
      wt_7 = wt_7_St_18_Sw;
      ver_7 = ver_7_St_18_Sw;
      break;
    case Main__St_18_Inactive:
      ns_6_St_18_Inactive = Main__St_18_Inactive;
      ns_6 = ns_6_St_18_Inactive;
      nr_6_St_18_Inactive = false;
      nr_6 = nr_6_St_18_Inactive;
      size_7_St_18_Inactive = 0;
      size_7 = size_7_St_18_Inactive;
      act_10_St_18_Inactive = false;
      act_10 = act_10_St_18_Inactive;
      wt_7_St_18_Inactive = false;
      wt_7 = wt_7_St_18_Inactive;
      ver_7_St_18_Inactive = Main__N;
      ver_7 = ver_7_St_18_Inactive;
      break;
    default:
      break;
  };
  _out->sl_size = size_7;
  _out->sl_act = act_10;
  v_970 = (_out->sl_act&&v_969);
  v_972 = (v_970&&v_971);
  v_979 = (_out->sl_act&&v_978);
  v_988 = (_out->sl_act&&v_987);
  v_990 = (v_988&&v_989);
  _out->sl_wt = wt_7;
  v_954 = !(_out->sl_wt);
  v_955 = (!(ct4)||v_954);
  _out->sl_ver = ver_7;
  v_918 = (_out->trk_ver==_out->sl_ver);
  v_919 = !(v_918);
  v_920 = (v_917&&v_919);
  v_921 = (!(v_915)||v_920);
  v_937 = (_out->trk_ver==_out->sl_ver);
  v_938 = !(v_937);
  v_928 = (_out->dt_ver==_out->sl_ver);
  v_929 = !(v_928);
  v_930 = (v_927&&v_929);
  v_931 = (!(v_925)||v_930);
  v_932 = (v_921&&v_931);
  v_939 = (_out->sl_ver==_out->dt_ver);
  v_940 = !(v_939);
  v_941 = (v_938&&v_940);
  v_933 = (_out->sl_ver==Main__S);
  v_934 = (_out->sl_ver==Main__N);
  v_935 = (v_933||v_934);
  v_936 = !(v_935);
  v_942 = (!(v_936)||v_941);
  v_943 = (v_932&&v_942);
  v_948 = (v_943&&v_947);
  v_953 = (v_948&&v_952);
  v_956 = (v_953&&v_955);
  v_959 = (v_956&&v_958);
  v_962 = (v_959&&v_961);
  v_965 = (v_962&&v_964);
  v_968 = (v_965&&v_967);
  ck_15 = _out->sl_ver;
  switch (ck_15) {
    case Main__N:
      wcet4_N = -1;
      _out->wcet4 = wcet4_N;
      break;
    case Main__H3:
      wcet4_H3 = _out->wcet4_hw3;
      _out->wcet4 = wcet4_H3;
      break;
    case Main__H2:
      wcet4_H2 = _out->wcet4_hw2;
      _out->wcet4 = wcet4_H2;
      break;
    case Main__H1:
      wcet4_H1 = _out->wcet4_hw1;
      _out->wcet4 = wcet4_H1;
      break;
    case Main__S:
      wcet4_S = _out->wcet4_sw;
      _out->wcet4 = wcet4_S;
      break;
    default:
      break;
  };
  v_974 = (self->v_973>_out->wcet4);
  v_975 = (!(v_972)||v_974);
  v_976 = (!(cUp4)||v_975);
  v_977 = (v_968&&v_976);
  v_981 = (self->v_980<_out->wcet4);
  v_982 = (v_979&&v_981);
  v_983 = (_out->wcet4<maxtexe4);
  v_984 = (!(v_982)||v_983);
  v_985 = (!(cKp4)||v_984);
  v_986 = (v_977&&v_985);
  v_992 = (self->v_991<_out->wcet4);
  v_993 = (!(v_990)||v_992);
  v_994 = (!(cDw4)||v_993);
  v_995 = (v_986&&v_994);
  v_1004 = (v_995&&v_1003);
  v_1013 = (v_1004&&v_1012);
  v_1022 = (v_1013&&v_1021);
  v_1031 = (v_1022&&v_1030);
  v_1040 = (v_1031&&v_1039);
  _out->obj_task = (v_1040&&v_1048);
  switch (self->ck_39) {
    case Main__St_17_Wait:
      if (c4_4) {
        v_1727 = Main__St_17_Sw;
      } else {
        v_1727 = Main__St_17_Wait;
      };
      if (c3_7) {
        v_1729 = Main__St_17_Hw3;
      } else {
        v_1729 = v_1727;
      };
      if (c2_8) {
        v_1731 = Main__St_17_Hw2;
      } else {
        v_1731 = v_1729;
      };
      if (c1_8) {
        v_1733 = Main__St_17_Hw1;
      } else {
        v_1733 = v_1731;
      };
      if (e_8) {
        s_5_St_17_Wait = Main__St_17_Inactive;
      } else {
        s_5_St_17_Wait = v_1733;
      };
      if (c4_4) {
        v_1728 = true;
      } else {
        v_1728 = self->pnr_5;
      };
      if (c3_7) {
        v_1730 = true;
      } else {
        v_1730 = v_1728;
      };
      if (c2_8) {
        v_1732 = true;
      } else {
        v_1732 = v_1730;
      };
      if (c1_8) {
        v_1734 = true;
      } else {
        v_1734 = v_1732;
      };
      if (e_8) {
        r_27_St_17_Wait = true;
      } else {
        r_27_St_17_Wait = v_1734;
      };
      s_5 = s_5_St_17_Wait;
      r_27 = r_27_St_17_Wait;
      break;
    case Main__St_17_Hw3:
      v_1735 = !(c3_7);
      v_1736 = (v_1735&&c1_8);
      v_1737 = !(c3_7);
      v_1738 = (v_1737&&c2_8);
      v_1739 = !(c3_7);
      v_1740 = (v_1739&&c4_4);
      v_1741 = !(c3_7);
      v_1742 = !(c2_8);
      v_1743 = (v_1741&&v_1742);
      v_1744 = !(c1_8);
      v_1745 = (v_1743&&v_1744);
      v_1746 = !(c4_4);
      v_1747 = (v_1745&&v_1746);
      if (v_1747) {
        v_1748 = Main__St_17_Wait;
      } else {
        v_1748 = Main__St_17_Hw3;
      };
      if (v_1740) {
        v_1750 = Main__St_17_Sw;
      } else {
        v_1750 = v_1748;
      };
      if (v_1738) {
        v_1752 = Main__St_17_Hw2;
      } else {
        v_1752 = v_1750;
      };
      if (v_1736) {
        v_1754 = Main__St_17_Hw1;
      } else {
        v_1754 = v_1752;
      };
      if (e_8) {
        s_5_St_17_Hw3 = Main__St_17_Inactive;
      } else {
        s_5_St_17_Hw3 = v_1754;
      };
      if (v_1747) {
        v_1749 = true;
      } else {
        v_1749 = self->pnr_5;
      };
      if (v_1740) {
        v_1751 = true;
      } else {
        v_1751 = v_1749;
      };
      if (v_1738) {
        v_1753 = true;
      } else {
        v_1753 = v_1751;
      };
      if (v_1736) {
        v_1755 = true;
      } else {
        v_1755 = v_1753;
      };
      if (e_8) {
        r_27_St_17_Hw3 = true;
      } else {
        r_27_St_17_Hw3 = v_1755;
      };
      s_5 = s_5_St_17_Hw3;
      r_27 = r_27_St_17_Hw3;
      break;
    case Main__St_17_Hw2:
      v_1756 = !(c2_8);
      v_1757 = (v_1756&&c1_8);
      v_1758 = !(c2_8);
      v_1759 = (v_1758&&c3_7);
      v_1760 = !(c2_8);
      v_1761 = (v_1760&&c4_4);
      v_1762 = !(c3_7);
      v_1763 = !(c2_8);
      v_1764 = (v_1762&&v_1763);
      v_1765 = !(c1_8);
      v_1766 = (v_1764&&v_1765);
      v_1767 = !(c4_4);
      v_1768 = (v_1766&&v_1767);
      if (v_1768) {
        v_1769 = Main__St_17_Wait;
      } else {
        v_1769 = Main__St_17_Hw2;
      };
      if (v_1761) {
        v_1771 = Main__St_17_Sw;
      } else {
        v_1771 = v_1769;
      };
      if (v_1759) {
        v_1773 = Main__St_17_Hw3;
      } else {
        v_1773 = v_1771;
      };
      if (v_1757) {
        v_1775 = Main__St_17_Hw1;
      } else {
        v_1775 = v_1773;
      };
      if (e_8) {
        s_5_St_17_Hw2 = Main__St_17_Inactive;
      } else {
        s_5_St_17_Hw2 = v_1775;
      };
      if (v_1768) {
        v_1770 = true;
      } else {
        v_1770 = self->pnr_5;
      };
      if (v_1761) {
        v_1772 = true;
      } else {
        v_1772 = v_1770;
      };
      if (v_1759) {
        v_1774 = true;
      } else {
        v_1774 = v_1772;
      };
      if (v_1757) {
        v_1776 = true;
      } else {
        v_1776 = v_1774;
      };
      if (e_8) {
        r_27_St_17_Hw2 = true;
      } else {
        r_27_St_17_Hw2 = v_1776;
      };
      s_5 = s_5_St_17_Hw2;
      r_27 = r_27_St_17_Hw2;
      break;
    case Main__St_17_Hw1:
      v_1777 = !(c1_8);
      v_1778 = (v_1777&&c2_8);
      v_1779 = !(c1_8);
      v_1780 = (v_1779&&c3_7);
      v_1781 = !(c1_8);
      v_1782 = (v_1781&&c4_4);
      v_1783 = !(c3_7);
      v_1784 = !(c2_8);
      v_1785 = (v_1783&&v_1784);
      v_1786 = !(c1_8);
      v_1787 = (v_1785&&v_1786);
      v_1788 = !(c4_4);
      v_1789 = (v_1787&&v_1788);
      if (v_1789) {
        v_1790 = Main__St_17_Wait;
      } else {
        v_1790 = Main__St_17_Hw1;
      };
      if (v_1782) {
        v_1792 = Main__St_17_Sw;
      } else {
        v_1792 = v_1790;
      };
      if (v_1780) {
        v_1794 = Main__St_17_Hw3;
      } else {
        v_1794 = v_1792;
      };
      if (v_1778) {
        v_1796 = Main__St_17_Hw2;
      } else {
        v_1796 = v_1794;
      };
      if (e_8) {
        s_5_St_17_Hw1 = Main__St_17_Inactive;
      } else {
        s_5_St_17_Hw1 = v_1796;
      };
      if (v_1789) {
        v_1791 = true;
      } else {
        v_1791 = self->pnr_5;
      };
      if (v_1782) {
        v_1793 = true;
      } else {
        v_1793 = v_1791;
      };
      if (v_1780) {
        v_1795 = true;
      } else {
        v_1795 = v_1793;
      };
      if (v_1778) {
        v_1797 = true;
      } else {
        v_1797 = v_1795;
      };
      if (e_8) {
        r_27_St_17_Hw1 = true;
      } else {
        r_27_St_17_Hw1 = v_1797;
      };
      s_5 = s_5_St_17_Hw1;
      r_27 = r_27_St_17_Hw1;
      break;
    case Main__St_17_Sw:
      v_1798 = !(c4_4);
      v_1799 = (v_1798&&c1_8);
      v_1800 = !(c4_4);
      v_1801 = (v_1800&&c2_8);
      v_1802 = !(c4_4);
      v_1803 = (v_1802&&c3_7);
      v_1804 = !(c3_7);
      v_1805 = !(c2_8);
      v_1806 = (v_1804&&v_1805);
      v_1807 = !(c1_8);
      v_1808 = (v_1806&&v_1807);
      v_1809 = !(c4_4);
      v_1810 = (v_1808&&v_1809);
      if (v_1810) {
        v_1811 = Main__St_17_Wait;
      } else {
        v_1811 = Main__St_17_Sw;
      };
      if (v_1803) {
        v_1813 = Main__St_17_Hw3;
      } else {
        v_1813 = v_1811;
      };
      if (v_1801) {
        v_1815 = Main__St_17_Hw2;
      } else {
        v_1815 = v_1813;
      };
      if (v_1799) {
        v_1817 = Main__St_17_Hw1;
      } else {
        v_1817 = v_1815;
      };
      if (e_8) {
        s_5_St_17_Sw = Main__St_17_Inactive;
      } else {
        s_5_St_17_Sw = v_1817;
      };
      if (v_1810) {
        v_1812 = true;
      } else {
        v_1812 = self->pnr_5;
      };
      if (v_1803) {
        v_1814 = true;
      } else {
        v_1814 = v_1812;
      };
      if (v_1801) {
        v_1816 = true;
      } else {
        v_1816 = v_1814;
      };
      if (v_1799) {
        v_1818 = true;
      } else {
        v_1818 = v_1816;
      };
      if (e_8) {
        r_27_St_17_Sw = true;
      } else {
        r_27_St_17_Sw = v_1818;
      };
      s_5 = s_5_St_17_Sw;
      r_27 = r_27_St_17_Sw;
      break;
    case Main__St_17_Inactive:
      v_1819 = (r_8&&c1_8);
      v_1820 = (r_8&&c2_8);
      v_1821 = (r_8&&c3_7);
      v_1822 = (r_8&&c4_4);
      v_1823 = !(c3_7);
      v_1824 = (r_8&&v_1823);
      v_1825 = !(c2_8);
      v_1826 = (v_1824&&v_1825);
      v_1827 = !(c1_8);
      v_1828 = (v_1826&&v_1827);
      v_1829 = !(c4_4);
      v_1830 = (v_1828&&v_1829);
      if (v_1830) {
        v_1831 = Main__St_17_Wait;
      } else {
        v_1831 = Main__St_17_Inactive;
      };
      if (v_1822) {
        v_1833 = Main__St_17_Sw;
      } else {
        v_1833 = v_1831;
      };
      if (v_1821) {
        v_1835 = Main__St_17_Hw3;
      } else {
        v_1835 = v_1833;
      };
      if (v_1820) {
        v_1837 = Main__St_17_Hw2;
      } else {
        v_1837 = v_1835;
      };
      if (v_1819) {
        s_5_St_17_Inactive = Main__St_17_Hw1;
      } else {
        s_5_St_17_Inactive = v_1837;
      };
      s_5 = s_5_St_17_Inactive;
      if (v_1830) {
        v_1832 = true;
      } else {
        v_1832 = self->pnr_5;
      };
      if (v_1822) {
        v_1834 = true;
      } else {
        v_1834 = v_1832;
      };
      if (v_1821) {
        v_1836 = true;
      } else {
        v_1836 = v_1834;
      };
      if (v_1820) {
        v_1838 = true;
      } else {
        v_1838 = v_1836;
      };
      if (v_1819) {
        r_27_St_17_Inactive = true;
      } else {
        r_27_St_17_Inactive = v_1838;
      };
      r_27 = r_27_St_17_Inactive;
      break;
    default:
      break;
  };
  ck_40 = s_5;
  switch (ck_40) {
    case Main__St_17_Wait:
      ns_5_St_17_Wait = Main__St_17_Wait;
      nr_5_St_17_Wait = false;
      size_8_St_17_Wait = 0;
      act_11_St_17_Wait = false;
      wt_8_St_17_Wait = true;
      ver_8_St_17_Wait = Main__N;
      ns_5 = ns_5_St_17_Wait;
      nr_5 = nr_5_St_17_Wait;
      size_8 = size_8_St_17_Wait;
      act_11 = act_11_St_17_Wait;
      wt_8 = wt_8_St_17_Wait;
      ver_8 = ver_8_St_17_Wait;
      break;
    case Main__St_17_Hw3:
      ns_5_St_17_Hw3 = Main__St_17_Hw3;
      nr_5_St_17_Hw3 = false;
      size_8_St_17_Hw3 = 1;
      act_11_St_17_Hw3 = true;
      wt_8_St_17_Hw3 = false;
      ver_8_St_17_Hw3 = Main__H3;
      ns_5 = ns_5_St_17_Hw3;
      nr_5 = nr_5_St_17_Hw3;
      size_8 = size_8_St_17_Hw3;
      act_11 = act_11_St_17_Hw3;
      wt_8 = wt_8_St_17_Hw3;
      ver_8 = ver_8_St_17_Hw3;
      break;
    case Main__St_17_Hw2:
      ns_5_St_17_Hw2 = Main__St_17_Hw2;
      nr_5_St_17_Hw2 = false;
      size_8_St_17_Hw2 = 1;
      act_11_St_17_Hw2 = true;
      wt_8_St_17_Hw2 = false;
      ver_8_St_17_Hw2 = Main__H2;
      ns_5 = ns_5_St_17_Hw2;
      nr_5 = nr_5_St_17_Hw2;
      size_8 = size_8_St_17_Hw2;
      act_11 = act_11_St_17_Hw2;
      wt_8 = wt_8_St_17_Hw2;
      ver_8 = ver_8_St_17_Hw2;
      break;
    case Main__St_17_Hw1:
      ns_5_St_17_Hw1 = Main__St_17_Hw1;
      nr_5_St_17_Hw1 = false;
      size_8_St_17_Hw1 = 1;
      act_11_St_17_Hw1 = true;
      wt_8_St_17_Hw1 = false;
      ver_8_St_17_Hw1 = Main__H1;
      ns_5 = ns_5_St_17_Hw1;
      nr_5 = nr_5_St_17_Hw1;
      size_8 = size_8_St_17_Hw1;
      act_11 = act_11_St_17_Hw1;
      wt_8 = wt_8_St_17_Hw1;
      ver_8 = ver_8_St_17_Hw1;
      break;
    case Main__St_17_Sw:
      ns_5_St_17_Sw = Main__St_17_Sw;
      nr_5_St_17_Sw = false;
      size_8_St_17_Sw = 0;
      act_11_St_17_Sw = true;
      wt_8_St_17_Sw = false;
      ver_8_St_17_Sw = Main__S;
      ns_5 = ns_5_St_17_Sw;
      nr_5 = nr_5_St_17_Sw;
      size_8 = size_8_St_17_Sw;
      act_11 = act_11_St_17_Sw;
      wt_8 = wt_8_St_17_Sw;
      ver_8 = ver_8_St_17_Sw;
      break;
    case Main__St_17_Inactive:
      ns_5_St_17_Inactive = Main__St_17_Inactive;
      ns_5 = ns_5_St_17_Inactive;
      nr_5_St_17_Inactive = false;
      nr_5 = nr_5_St_17_Inactive;
      size_8_St_17_Inactive = 0;
      size_8 = size_8_St_17_Inactive;
      act_11_St_17_Inactive = false;
      act_11 = act_11_St_17_Inactive;
      wt_8_St_17_Inactive = false;
      wt_8 = wt_8_St_17_Inactive;
      ver_8_St_17_Inactive = Main__N;
      ver_8 = ver_8_St_17_Inactive;
      break;
    default:
      break;
  };
  _out->me_img_size = size_8;
  _out->me_img_act = act_11;
  _out->me_img_wt = wt_8;
  _out->me_img_ver = ver_8;
  ck_14 = _out->me_img_ver;
  switch (ck_14) {
    case Main__N:
      wcet3_N = -1;
      _out->wcet3 = wcet3_N;
      break;
    case Main__H3:
      wcet3_H3 = _out->wcet3_hw3;
      _out->wcet3 = wcet3_H3;
      break;
    case Main__H2:
      wcet3_H2 = _out->wcet3_hw2;
      _out->wcet3 = wcet3_H2;
      break;
    case Main__H1:
      wcet3_H1 = _out->wcet3_hw1;
      _out->wcet3 = wcet3_H1;
      break;
    case Main__S:
      wcet3_S = _out->wcet3_sw;
      _out->wcet3 = wcet3_S;
      break;
    default:
      break;
  };
  switch (self->ck_41) {
    case Main__St_16_Wait:
      if (c_1) {
        v_1839 = Main__St_16_Active;
      } else {
        v_1839 = Main__St_16_Wait;
      };
      if (e_9) {
        s_4_St_16_Wait = Main__St_16_Inactive;
      } else {
        s_4_St_16_Wait = v_1839;
      };
      if (c_1) {
        v_1840 = true;
      } else {
        v_1840 = self->pnr_4;
      };
      if (e_9) {
        r_26_St_16_Wait = true;
      } else {
        r_26_St_16_Wait = v_1840;
      };
      s_4 = s_4_St_16_Wait;
      r_26 = r_26_St_16_Wait;
      break;
    case Main__St_16_Active:
      v_1841 = !(c_1);
      if (v_1841) {
        v_1842 = Main__St_16_Wait;
      } else {
        v_1842 = Main__St_16_Active;
      };
      if (e_9) {
        s_4_St_16_Active = Main__St_16_Inactive;
      } else {
        s_4_St_16_Active = v_1842;
      };
      if (v_1841) {
        v_1843 = true;
      } else {
        v_1843 = self->pnr_4;
      };
      if (e_9) {
        r_26_St_16_Active = true;
      } else {
        r_26_St_16_Active = v_1843;
      };
      s_4 = s_4_St_16_Active;
      r_26 = r_26_St_16_Active;
      break;
    case Main__St_16_Inactive:
      v_1844 = (r_9&&c_1);
      v_1845 = !(c_1);
      v_1846 = (r_9&&v_1845);
      if (v_1846) {
        v_1847 = Main__St_16_Wait;
      } else {
        v_1847 = Main__St_16_Inactive;
      };
      if (v_1844) {
        s_4_St_16_Inactive = Main__St_16_Active;
      } else {
        s_4_St_16_Inactive = v_1847;
      };
      s_4 = s_4_St_16_Inactive;
      if (v_1846) {
        v_1848 = true;
      } else {
        v_1848 = self->pnr_4;
      };
      if (v_1844) {
        r_26_St_16_Inactive = true;
      } else {
        r_26_St_16_Inactive = v_1848;
      };
      r_26 = r_26_St_16_Inactive;
      break;
    default:
      break;
  };
  ck_42 = s_4;
  switch (ck_42) {
    case Main__St_16_Wait:
      ns_4_St_16_Wait = Main__St_16_Wait;
      nr_4_St_16_Wait = false;
      size_9_St_16_Wait = 0;
      act_12_St_16_Wait = false;
      wt_9_St_16_Wait = true;
      ver_9_St_16_Wait = Main__N;
      ns_4 = ns_4_St_16_Wait;
      nr_4 = nr_4_St_16_Wait;
      size_9 = size_9_St_16_Wait;
      act_12 = act_12_St_16_Wait;
      wt_9 = wt_9_St_16_Wait;
      ver_9 = ver_9_St_16_Wait;
      break;
    case Main__St_16_Active:
      ns_4_St_16_Active = Main__St_16_Active;
      nr_4_St_16_Active = false;
      size_9_St_16_Active = 0;
      act_12_St_16_Active = true;
      wt_9_St_16_Active = false;
      ver_9_St_16_Active = Main__S;
      ns_4 = ns_4_St_16_Active;
      nr_4 = nr_4_St_16_Active;
      size_9 = size_9_St_16_Active;
      act_12 = act_12_St_16_Active;
      wt_9 = wt_9_St_16_Active;
      ver_9 = ver_9_St_16_Active;
      break;
    case Main__St_16_Inactive:
      ns_4_St_16_Inactive = Main__St_16_Inactive;
      ns_4 = ns_4_St_16_Inactive;
      nr_4_St_16_Inactive = false;
      nr_4 = nr_4_St_16_Inactive;
      size_9_St_16_Inactive = 0;
      size_9 = size_9_St_16_Inactive;
      act_12_St_16_Inactive = false;
      act_12 = act_12_St_16_Inactive;
      wt_9_St_16_Inactive = false;
      wt_9 = wt_9_St_16_Inactive;
      ver_9_St_16_Inactive = Main__N;
      ver_9 = ver_9_St_16_Inactive;
      break;
    default:
      break;
  };
  _out->me_imu_size = size_9;
  _out->me_imu_act = act_12;
  _out->me_imu_wt = wt_9;
  _out->me_imu_ver = ver_9;
  ck_13 = _out->me_imu_ver;
  switch (ck_13) {
    case Main__N:
      wcet2_N = -1;
      _out->wcet2 = wcet2_N;
      break;
    case Main__H3:
      wcet2_H3 = _out->wcet2_hw3;
      _out->wcet2 = wcet2_H3;
      break;
    case Main__H2:
      wcet2_H2 = _out->wcet2_hw2;
      _out->wcet2 = wcet2_H2;
      break;
    case Main__H1:
      wcet2_H1 = _out->wcet2_hw1;
      _out->wcet2 = wcet2_H1;
      break;
    case Main__S:
      wcet2_S = _out->wcet2_sw;
      _out->wcet2 = wcet2_S;
      break;
    default:
      break;
  };
  switch (self->ck_43) {
    case Main__St_15_Wait:
      if (c4_5) {
        v_1849 = Main__St_15_Sw;
      } else {
        v_1849 = Main__St_15_Wait;
      };
      if (c3_8) {
        v_1851 = Main__St_15_Hw3;
      } else {
        v_1851 = v_1849;
      };
      if (c2_9) {
        v_1853 = Main__St_15_Hw2;
      } else {
        v_1853 = v_1851;
      };
      if (c1_9) {
        v_1855 = Main__St_15_Hw1;
      } else {
        v_1855 = v_1853;
      };
      if (e_10) {
        s_3_St_15_Wait = Main__St_15_Inactive;
      } else {
        s_3_St_15_Wait = v_1855;
      };
      if (c4_5) {
        v_1850 = true;
      } else {
        v_1850 = self->pnr_3;
      };
      if (c3_8) {
        v_1852 = true;
      } else {
        v_1852 = v_1850;
      };
      if (c2_9) {
        v_1854 = true;
      } else {
        v_1854 = v_1852;
      };
      if (c1_9) {
        v_1856 = true;
      } else {
        v_1856 = v_1854;
      };
      if (e_10) {
        r_25_St_15_Wait = true;
      } else {
        r_25_St_15_Wait = v_1856;
      };
      s_3 = s_3_St_15_Wait;
      r_25 = r_25_St_15_Wait;
      break;
    case Main__St_15_Hw3:
      v_1857 = !(c3_8);
      v_1858 = (v_1857&&c1_9);
      v_1859 = !(c3_8);
      v_1860 = (v_1859&&c2_9);
      v_1861 = !(c3_8);
      v_1862 = (v_1861&&c4_5);
      v_1863 = !(c3_8);
      v_1864 = !(c2_9);
      v_1865 = (v_1863&&v_1864);
      v_1866 = !(c1_9);
      v_1867 = (v_1865&&v_1866);
      v_1868 = !(c4_5);
      v_1869 = (v_1867&&v_1868);
      if (v_1869) {
        v_1870 = Main__St_15_Wait;
      } else {
        v_1870 = Main__St_15_Hw3;
      };
      if (v_1862) {
        v_1872 = Main__St_15_Sw;
      } else {
        v_1872 = v_1870;
      };
      if (v_1860) {
        v_1874 = Main__St_15_Hw2;
      } else {
        v_1874 = v_1872;
      };
      if (v_1858) {
        v_1876 = Main__St_15_Hw1;
      } else {
        v_1876 = v_1874;
      };
      if (e_10) {
        s_3_St_15_Hw3 = Main__St_15_Inactive;
      } else {
        s_3_St_15_Hw3 = v_1876;
      };
      if (v_1869) {
        v_1871 = true;
      } else {
        v_1871 = self->pnr_3;
      };
      if (v_1862) {
        v_1873 = true;
      } else {
        v_1873 = v_1871;
      };
      if (v_1860) {
        v_1875 = true;
      } else {
        v_1875 = v_1873;
      };
      if (v_1858) {
        v_1877 = true;
      } else {
        v_1877 = v_1875;
      };
      if (e_10) {
        r_25_St_15_Hw3 = true;
      } else {
        r_25_St_15_Hw3 = v_1877;
      };
      s_3 = s_3_St_15_Hw3;
      r_25 = r_25_St_15_Hw3;
      break;
    case Main__St_15_Hw2:
      v_1878 = !(c2_9);
      v_1879 = (v_1878&&c1_9);
      v_1880 = !(c2_9);
      v_1881 = (v_1880&&c3_8);
      v_1882 = !(c2_9);
      v_1883 = (v_1882&&c4_5);
      v_1884 = !(c3_8);
      v_1885 = !(c2_9);
      v_1886 = (v_1884&&v_1885);
      v_1887 = !(c1_9);
      v_1888 = (v_1886&&v_1887);
      v_1889 = !(c4_5);
      v_1890 = (v_1888&&v_1889);
      if (v_1890) {
        v_1891 = Main__St_15_Wait;
      } else {
        v_1891 = Main__St_15_Hw2;
      };
      if (v_1883) {
        v_1893 = Main__St_15_Sw;
      } else {
        v_1893 = v_1891;
      };
      if (v_1881) {
        v_1895 = Main__St_15_Hw3;
      } else {
        v_1895 = v_1893;
      };
      if (v_1879) {
        v_1897 = Main__St_15_Hw1;
      } else {
        v_1897 = v_1895;
      };
      if (e_10) {
        s_3_St_15_Hw2 = Main__St_15_Inactive;
      } else {
        s_3_St_15_Hw2 = v_1897;
      };
      if (v_1890) {
        v_1892 = true;
      } else {
        v_1892 = self->pnr_3;
      };
      if (v_1883) {
        v_1894 = true;
      } else {
        v_1894 = v_1892;
      };
      if (v_1881) {
        v_1896 = true;
      } else {
        v_1896 = v_1894;
      };
      if (v_1879) {
        v_1898 = true;
      } else {
        v_1898 = v_1896;
      };
      if (e_10) {
        r_25_St_15_Hw2 = true;
      } else {
        r_25_St_15_Hw2 = v_1898;
      };
      s_3 = s_3_St_15_Hw2;
      r_25 = r_25_St_15_Hw2;
      break;
    case Main__St_15_Hw1:
      v_1899 = !(c1_9);
      v_1900 = (v_1899&&c2_9);
      v_1901 = !(c1_9);
      v_1902 = (v_1901&&c3_8);
      v_1903 = !(c1_9);
      v_1904 = (v_1903&&c4_5);
      v_1905 = !(c3_8);
      v_1906 = !(c2_9);
      v_1907 = (v_1905&&v_1906);
      v_1908 = !(c1_9);
      v_1909 = (v_1907&&v_1908);
      v_1910 = !(c4_5);
      v_1911 = (v_1909&&v_1910);
      if (v_1911) {
        v_1912 = Main__St_15_Wait;
      } else {
        v_1912 = Main__St_15_Hw1;
      };
      if (v_1904) {
        v_1914 = Main__St_15_Sw;
      } else {
        v_1914 = v_1912;
      };
      if (v_1902) {
        v_1916 = Main__St_15_Hw3;
      } else {
        v_1916 = v_1914;
      };
      if (v_1900) {
        v_1918 = Main__St_15_Hw2;
      } else {
        v_1918 = v_1916;
      };
      if (e_10) {
        s_3_St_15_Hw1 = Main__St_15_Inactive;
      } else {
        s_3_St_15_Hw1 = v_1918;
      };
      if (v_1911) {
        v_1913 = true;
      } else {
        v_1913 = self->pnr_3;
      };
      if (v_1904) {
        v_1915 = true;
      } else {
        v_1915 = v_1913;
      };
      if (v_1902) {
        v_1917 = true;
      } else {
        v_1917 = v_1915;
      };
      if (v_1900) {
        v_1919 = true;
      } else {
        v_1919 = v_1917;
      };
      if (e_10) {
        r_25_St_15_Hw1 = true;
      } else {
        r_25_St_15_Hw1 = v_1919;
      };
      s_3 = s_3_St_15_Hw1;
      r_25 = r_25_St_15_Hw1;
      break;
    case Main__St_15_Sw:
      v_1920 = !(c4_5);
      v_1921 = (v_1920&&c1_9);
      v_1922 = !(c4_5);
      v_1923 = (v_1922&&c2_9);
      v_1924 = !(c4_5);
      v_1925 = (v_1924&&c3_8);
      v_1926 = !(c3_8);
      v_1927 = !(c2_9);
      v_1928 = (v_1926&&v_1927);
      v_1929 = !(c1_9);
      v_1930 = (v_1928&&v_1929);
      v_1931 = !(c4_5);
      v_1932 = (v_1930&&v_1931);
      if (v_1932) {
        v_1933 = Main__St_15_Wait;
      } else {
        v_1933 = Main__St_15_Sw;
      };
      if (v_1925) {
        v_1935 = Main__St_15_Hw3;
      } else {
        v_1935 = v_1933;
      };
      if (v_1923) {
        v_1937 = Main__St_15_Hw2;
      } else {
        v_1937 = v_1935;
      };
      if (v_1921) {
        v_1939 = Main__St_15_Hw1;
      } else {
        v_1939 = v_1937;
      };
      if (e_10) {
        s_3_St_15_Sw = Main__St_15_Inactive;
      } else {
        s_3_St_15_Sw = v_1939;
      };
      if (v_1932) {
        v_1934 = true;
      } else {
        v_1934 = self->pnr_3;
      };
      if (v_1925) {
        v_1936 = true;
      } else {
        v_1936 = v_1934;
      };
      if (v_1923) {
        v_1938 = true;
      } else {
        v_1938 = v_1936;
      };
      if (v_1921) {
        v_1940 = true;
      } else {
        v_1940 = v_1938;
      };
      if (e_10) {
        r_25_St_15_Sw = true;
      } else {
        r_25_St_15_Sw = v_1940;
      };
      s_3 = s_3_St_15_Sw;
      r_25 = r_25_St_15_Sw;
      break;
    case Main__St_15_Inactive:
      v_1941 = (r_10&&c1_9);
      v_1942 = (r_10&&c2_9);
      v_1943 = (r_10&&c3_8);
      v_1944 = (r_10&&c4_5);
      v_1945 = !(c3_8);
      v_1946 = (r_10&&v_1945);
      v_1947 = !(c2_9);
      v_1948 = (v_1946&&v_1947);
      v_1949 = !(c1_9);
      v_1950 = (v_1948&&v_1949);
      v_1951 = !(c4_5);
      v_1952 = (v_1950&&v_1951);
      if (v_1952) {
        v_1953 = Main__St_15_Wait;
      } else {
        v_1953 = Main__St_15_Inactive;
      };
      if (v_1944) {
        v_1955 = Main__St_15_Sw;
      } else {
        v_1955 = v_1953;
      };
      if (v_1943) {
        v_1957 = Main__St_15_Hw3;
      } else {
        v_1957 = v_1955;
      };
      if (v_1942) {
        v_1959 = Main__St_15_Hw2;
      } else {
        v_1959 = v_1957;
      };
      if (v_1941) {
        s_3_St_15_Inactive = Main__St_15_Hw1;
      } else {
        s_3_St_15_Inactive = v_1959;
      };
      s_3 = s_3_St_15_Inactive;
      if (v_1952) {
        v_1954 = true;
      } else {
        v_1954 = self->pnr_3;
      };
      if (v_1944) {
        v_1956 = true;
      } else {
        v_1956 = v_1954;
      };
      if (v_1943) {
        v_1958 = true;
      } else {
        v_1958 = v_1956;
      };
      if (v_1942) {
        v_1960 = true;
      } else {
        v_1960 = v_1958;
      };
      if (v_1941) {
        r_25_St_15_Inactive = true;
      } else {
        r_25_St_15_Inactive = v_1960;
      };
      r_25 = r_25_St_15_Inactive;
      break;
    default:
      break;
  };
  ck_44 = s_3;
  switch (ck_44) {
    case Main__St_15_Wait:
      ns_3_St_15_Wait = Main__St_15_Wait;
      nr_3_St_15_Wait = false;
      size_10_St_15_Wait = 0;
      act_13_St_15_Wait = false;
      wt_10_St_15_Wait = true;
      ver_10_St_15_Wait = Main__N;
      ns_3 = ns_3_St_15_Wait;
      nr_3 = nr_3_St_15_Wait;
      size_10 = size_10_St_15_Wait;
      act_13 = act_13_St_15_Wait;
      wt_10 = wt_10_St_15_Wait;
      ver_10 = ver_10_St_15_Wait;
      break;
    case Main__St_15_Hw3:
      ns_3_St_15_Hw3 = Main__St_15_Hw3;
      nr_3_St_15_Hw3 = false;
      size_10_St_15_Hw3 = 1;
      act_13_St_15_Hw3 = true;
      wt_10_St_15_Hw3 = false;
      ver_10_St_15_Hw3 = Main__H3;
      ns_3 = ns_3_St_15_Hw3;
      nr_3 = nr_3_St_15_Hw3;
      size_10 = size_10_St_15_Hw3;
      act_13 = act_13_St_15_Hw3;
      wt_10 = wt_10_St_15_Hw3;
      ver_10 = ver_10_St_15_Hw3;
      break;
    case Main__St_15_Hw2:
      ns_3_St_15_Hw2 = Main__St_15_Hw2;
      nr_3_St_15_Hw2 = false;
      size_10_St_15_Hw2 = 1;
      act_13_St_15_Hw2 = true;
      wt_10_St_15_Hw2 = false;
      ver_10_St_15_Hw2 = Main__H2;
      ns_3 = ns_3_St_15_Hw2;
      nr_3 = nr_3_St_15_Hw2;
      size_10 = size_10_St_15_Hw2;
      act_13 = act_13_St_15_Hw2;
      wt_10 = wt_10_St_15_Hw2;
      ver_10 = ver_10_St_15_Hw2;
      break;
    case Main__St_15_Hw1:
      ns_3_St_15_Hw1 = Main__St_15_Hw1;
      nr_3_St_15_Hw1 = false;
      size_10_St_15_Hw1 = 1;
      act_13_St_15_Hw1 = true;
      wt_10_St_15_Hw1 = false;
      ver_10_St_15_Hw1 = Main__H1;
      ns_3 = ns_3_St_15_Hw1;
      nr_3 = nr_3_St_15_Hw1;
      size_10 = size_10_St_15_Hw1;
      act_13 = act_13_St_15_Hw1;
      wt_10 = wt_10_St_15_Hw1;
      ver_10 = ver_10_St_15_Hw1;
      break;
    case Main__St_15_Sw:
      ns_3_St_15_Sw = Main__St_15_Sw;
      nr_3_St_15_Sw = false;
      size_10_St_15_Sw = 0;
      act_13_St_15_Sw = true;
      wt_10_St_15_Sw = false;
      ver_10_St_15_Sw = Main__S;
      ns_3 = ns_3_St_15_Sw;
      nr_3 = nr_3_St_15_Sw;
      size_10 = size_10_St_15_Sw;
      act_13 = act_13_St_15_Sw;
      wt_10 = wt_10_St_15_Sw;
      ver_10 = ver_10_St_15_Sw;
      break;
    case Main__St_15_Inactive:
      ns_3_St_15_Inactive = Main__St_15_Inactive;
      ns_3 = ns_3_St_15_Inactive;
      nr_3_St_15_Inactive = false;
      nr_3 = nr_3_St_15_Inactive;
      size_10_St_15_Inactive = 0;
      size_10 = size_10_St_15_Inactive;
      act_13_St_15_Inactive = false;
      act_13 = act_13_St_15_Inactive;
      wt_10_St_15_Inactive = false;
      wt_10 = wt_10_St_15_Inactive;
      ver_10_St_15_Inactive = Main__N;
      ver_10 = ver_10_St_15_Inactive;
      break;
    default:
      break;
  };
  _out->c_img_size = size_10;
  _out->c_img_act = act_13;
  _out->c_img_wt = wt_10;
  _out->c_img_ver = ver_10;
  ck = _out->c_img_ver;
  switch (ck) {
    case Main__N:
      wcet1_N = -1;
      _out->wcet1 = wcet1_N;
      break;
    case Main__H3:
      wcet1_H3 = _out->wcet1_hw3;
      _out->wcet1 = wcet1_H3;
      break;
    case Main__H2:
      wcet1_H2 = _out->wcet1_hw2;
      _out->wcet1 = wcet1_H2;
      break;
    case Main__H1:
      wcet1_H1 = _out->wcet1_hw1;
      _out->wcet1 = wcet1_H1;
      break;
    case Main__S:
      wcet1_S = _out->wcet1_sw;
      _out->wcet1 = wcet1_S;
      break;
    default:
      break;
  };
  switch (self->ck_45) {
    case Main__St_14_Busy:
      v_1961 = !(cr);
      if (v_1961) {
        v_1962 = Main__St_14_Free;
      } else {
        v_1962 = Main__St_14_Busy;
      };
      if (f) {
        s_2_St_14_Busy = Main__St_14_Fail;
      } else {
        s_2_St_14_Busy = v_1962;
      };
      if (v_1961) {
        v_1963 = true;
      } else {
        v_1963 = self->pnr_2;
      };
      if (f) {
        r_24_St_14_Busy = true;
      } else {
        r_24_St_14_Busy = v_1963;
      };
      s_2 = s_2_St_14_Busy;
      r_24 = r_24_St_14_Busy;
      break;
    case Main__St_14_Free:
      if (cr) {
        v_1964 = Main__St_14_Busy;
      } else {
        v_1964 = Main__St_14_Free;
      };
      if (f) {
        s_2_St_14_Free = Main__St_14_Fail;
      } else {
        s_2_St_14_Free = v_1964;
      };
      s_2 = s_2_St_14_Free;
      if (cr) {
        v_1965 = true;
      } else {
        v_1965 = self->pnr_2;
      };
      if (f) {
        r_24_St_14_Free = true;
      } else {
        r_24_St_14_Free = v_1965;
      };
      r_24 = r_24_St_14_Free;
      break;
    case Main__St_14_Fail:
      s_2 = s_2_St_14_Fail;
      r_24 = r_24_St_14_Fail;
      break;
    default:
      break;
  };
  ck_46 = s_2;
  switch (ck_46) {
    case Main__St_14_Fail:
      ns_2_St_14_Fail = Main__St_14_Fail;
      nr_2_St_14_Fail = false;
      act_14_St_14_Fail = false;
      err_St_14_Fail = true;
      ns_2 = ns_2_St_14_Fail;
      nr_2 = nr_2_St_14_Fail;
      act_14 = act_14_St_14_Fail;
      err = err_St_14_Fail;
      break;
    case Main__St_14_Busy:
      ns_2_St_14_Busy = Main__St_14_Busy;
      nr_2_St_14_Busy = false;
      act_14_St_14_Busy = true;
      err_St_14_Busy = false;
      ns_2 = ns_2_St_14_Busy;
      nr_2 = nr_2_St_14_Busy;
      act_14 = act_14_St_14_Busy;
      err = err_St_14_Busy;
      break;
    case Main__St_14_Free:
      ns_2_St_14_Free = Main__St_14_Free;
      ns_2 = ns_2_St_14_Free;
      nr_2_St_14_Free = false;
      nr_2 = nr_2_St_14_Free;
      act_14_St_14_Free = false;
      act_14 = act_14_St_14_Free;
      err_St_14_Free = false;
      err = err_St_14_Free;
      break;
    default:
      break;
  };
  _out->act_3 = act_14;
  v_910 = !(_out->act_3);
  _out->err_3 = err;
  v_911 = (!(_out->err_3)||v_910);
  switch (self->ck_47) {
    case Main__St_13_Busy:
      v_1966 = !(cr_1);
      if (v_1966) {
        v_1967 = Main__St_13_Free;
      } else {
        v_1967 = Main__St_13_Busy;
      };
      if (f_4) {
        s_1_St_13_Busy = Main__St_13_Fail;
      } else {
        s_1_St_13_Busy = v_1967;
      };
      if (v_1966) {
        v_1968 = true;
      } else {
        v_1968 = self->pnr_1;
      };
      if (f_4) {
        r_23_St_13_Busy = true;
      } else {
        r_23_St_13_Busy = v_1968;
      };
      s_1 = s_1_St_13_Busy;
      r_23 = r_23_St_13_Busy;
      break;
    case Main__St_13_Free:
      if (cr_1) {
        v_1969 = Main__St_13_Busy;
      } else {
        v_1969 = Main__St_13_Free;
      };
      if (f_4) {
        s_1_St_13_Free = Main__St_13_Fail;
      } else {
        s_1_St_13_Free = v_1969;
      };
      s_1 = s_1_St_13_Free;
      if (cr_1) {
        v_1970 = true;
      } else {
        v_1970 = self->pnr_1;
      };
      if (f_4) {
        r_23_St_13_Free = true;
      } else {
        r_23_St_13_Free = v_1970;
      };
      r_23 = r_23_St_13_Free;
      break;
    case Main__St_13_Fail:
      s_1 = s_1_St_13_Fail;
      r_23 = r_23_St_13_Fail;
      break;
    default:
      break;
  };
  ck_48 = s_1;
  switch (ck_48) {
    case Main__St_13_Fail:
      ns_1_St_13_Fail = Main__St_13_Fail;
      nr_1_St_13_Fail = false;
      act_15_St_13_Fail = false;
      err_4_St_13_Fail = true;
      ns_1 = ns_1_St_13_Fail;
      nr_1 = nr_1_St_13_Fail;
      act_15 = act_15_St_13_Fail;
      err_4 = err_4_St_13_Fail;
      break;
    case Main__St_13_Busy:
      ns_1_St_13_Busy = Main__St_13_Busy;
      nr_1_St_13_Busy = false;
      act_15_St_13_Busy = true;
      err_4_St_13_Busy = false;
      ns_1 = ns_1_St_13_Busy;
      nr_1 = nr_1_St_13_Busy;
      act_15 = act_15_St_13_Busy;
      err_4 = err_4_St_13_Busy;
      break;
    case Main__St_13_Free:
      ns_1_St_13_Free = Main__St_13_Free;
      ns_1 = ns_1_St_13_Free;
      nr_1_St_13_Free = false;
      nr_1 = nr_1_St_13_Free;
      act_15_St_13_Free = false;
      act_15 = act_15_St_13_Free;
      err_4_St_13_Free = false;
      err_4 = err_4_St_13_Free;
      break;
    default:
      break;
  };
  _out->act_2 = act_15;
  v_907 = !(_out->act_2);
  _out->err_2 = err_4;
  v_908 = (!(_out->err_2)||v_907);
  switch (self->ck_49) {
    case Main__St_12_Busy:
      v_1971 = !(cr_2);
      if (v_1971) {
        v_1972 = Main__St_12_Free;
      } else {
        v_1972 = Main__St_12_Busy;
      };
      if (f_5) {
        s_St_12_Busy = Main__St_12_Fail;
      } else {
        s_St_12_Busy = v_1972;
      };
      if (v_1971) {
        v_1973 = true;
      } else {
        v_1973 = self->pnr;
      };
      if (f_5) {
        r_22_St_12_Busy = true;
      } else {
        r_22_St_12_Busy = v_1973;
      };
      s = s_St_12_Busy;
      r_22 = r_22_St_12_Busy;
      break;
    case Main__St_12_Free:
      if (cr_2) {
        v_1974 = Main__St_12_Busy;
      } else {
        v_1974 = Main__St_12_Free;
      };
      if (f_5) {
        s_St_12_Free = Main__St_12_Fail;
      } else {
        s_St_12_Free = v_1974;
      };
      s = s_St_12_Free;
      if (cr_2) {
        v_1975 = true;
      } else {
        v_1975 = self->pnr;
      };
      if (f_5) {
        r_22_St_12_Free = true;
      } else {
        r_22_St_12_Free = v_1975;
      };
      r_22 = r_22_St_12_Free;
      break;
    case Main__St_12_Fail:
      s = s_St_12_Fail;
      r_22 = r_22_St_12_Fail;
      break;
    default:
      break;
  };
  ck_50 = s;
  switch (ck_50) {
    case Main__St_12_Fail:
      ns_St_12_Fail = Main__St_12_Fail;
      nr_St_12_Fail = false;
      act_16_St_12_Fail = false;
      err_5_St_12_Fail = true;
      ns = ns_St_12_Fail;
      nr = nr_St_12_Fail;
      act_16 = act_16_St_12_Fail;
      err_5 = err_5_St_12_Fail;
      break;
    case Main__St_12_Busy:
      ns_St_12_Busy = Main__St_12_Busy;
      nr_St_12_Busy = false;
      act_16_St_12_Busy = true;
      err_5_St_12_Busy = false;
      ns = ns_St_12_Busy;
      nr = nr_St_12_Busy;
      act_16 = act_16_St_12_Busy;
      err_5 = err_5_St_12_Busy;
      break;
    case Main__St_12_Free:
      ns_St_12_Free = Main__St_12_Free;
      ns = ns_St_12_Free;
      nr_St_12_Free = false;
      nr = nr_St_12_Free;
      act_16_St_12_Free = false;
      act_16 = act_16_St_12_Free;
      err_5_St_12_Free = false;
      err_5 = err_5_St_12_Free;
      break;
    default:
      break;
  };
  _out->act_1 = act_16;
  v = !(_out->act_1);
  _out->err_1 = err_5;
  v_906 = (!(_out->err_1)||v);
  v_909 = (v_906&&v_908);
  _out->obj_tiles = (v_909&&v_911);
  _out->obj = (_out->obj_tiles&&_out->obj_task);
  self->v_973 = _out->wcet4;
  self->v_980 = _out->wcet4;
  self->v_991 = _out->wcet4;
  self->v_1000 = _out->wcet10;
  self->v_1007 = _out->wcet10;
  self->v_1018 = _out->wcet10;
  self->v_1027 = _out->wcet11;
  self->v_1034 = _out->wcet11;
  self->v_1045 = _out->wcet11;
  self->ck_23 = ns_13;
  self->pnr_13 = nr_13;
  self->ck_25 = ns_12;
  self->pnr_12 = nr_12;
  self->ck_27 = ns_11;
  self->pnr_11 = nr_11;
  self->ck_29 = ns_10;
  self->pnr_10 = nr_10;
  self->ck_31 = ns_9;
  self->pnr_9 = nr_9;
  self->ck_33 = ns_8;
  self->pnr_8 = nr_8;
  self->ck_35 = ns_7;
  self->pnr_7 = nr_7;
  self->ck_37 = ns_6;
  self->pnr_6 = nr_6;
  self->ck_39 = ns_5;
  self->pnr_5 = nr_5;
  self->ck_41 = ns_4;
  self->pnr_4 = nr_4;
  self->ck_43 = ns_3;
  self->pnr_3 = nr_3;
  self->ck_45 = ns_2;
  self->pnr_2 = nr_2;
  self->ck_47 = ns_1;
  self->pnr_1 = nr_1;
  self->ck_49 = ns;
  self->pnr = nr;;
}

