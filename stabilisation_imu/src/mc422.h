#ifndef MC422_H_
#define MC422_H_

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
typedef signed char  int8_t;
typedef signed short int16_t;
typedef signed int   int32_t;

#include <string.h>
//void *memset(void *dest, int c, int n);

#define IORD(a,o) (((volatile uint32_t *)a)[o])
#define IOWR(a,o,v) (((volatile uint32_t *)a)[o] = v)

#define BILINEAR_COEFF_FRACT_PART_BIT 16
#define MC_COEF(f) (int)(0.5 + (f) * (1 << BILINEAR_COEFF_FRACT_PART_BIT))

#define MC_STATUS_ACTIVE_MASK     1
#define MC_STATUS_UNDERFLOW_MASK  2
#define MC_STATUS_OVERFLOW_MASK   4

#define MC_EVENT_END_IMAGE_MASK   1

#define MC_Control_Register       0
#define MC_Input_image_Register   1
#define MC_Output_image_Register  2
#define MC_Step_x_on_x_Register   3
#define MC_Step_x_on_y_Register   4
#define MC_Step_y_on_x_Register   5
#define MC_Step_y_on_y_Register   6
#define MC_Input_x0_Register      7
#define MC_Input_y0_Register      8
#define MC_Input_y_Register       9
#define MC_Input_uv_Register      10
#define MC_Status_Register        11
#define MC_Step_x_on_xy_Register  18
#define MC_Step_x_on_xx_Register  19
#define MC_Step_x_on_yy_Register  20
#define MC_Step_y_on_xy_Register  21
#define MC_Step_y_on_xx_Register  22
#define MC_Step_y_on_yy_Register  23

#define MC_OUTPUT_FIELD_MASK      1

#define MC_OUTPUT_PROGRESSIVE            0x0
#define MC_OUTPUT_INTERLEAVED_TOP        0x8
#define MC_OUTPUT_INTERLEAVED_BOTTOM     0xC

#define MC_OUTPUT_DEINTERLACING_BOTTOM   0x0    // We must deinterlace the sequence BOTTOM then TOP (bottom field first)
#define MC_OUTPUT_DEINTERLACING_TOP      0x1    // We must deinterlace the sequence TOP then BOTTOM (top field first)
#define MC_OUTPUT_DEINTERLACING_UNKNOWN  0x1    // We don't know how to deinterlace
#define MC_OUTPUT_DEINTERLACING_DONTCARE 0x3    // We don't need to take care of fields ordering so as to deinterlace

#define MC_OUTPUT_START_MC               0x10   //MC start only when this bit is 1. This bit is put to 0 at init


typedef volatile struct {
  uint32_t control_status;              //  0
  uint32_t input_image;                 //  1
  uint32_t output_image;                //  2
  uint32_t step_x_on_x;                 //  3
  uint32_t step_x_on_y;                 //  4
  uint32_t step_y_on_x;                 //  5
  uint32_t step_y_on_y;                 //  6
  uint32_t input_x0;                    //  7
  uint32_t input_y0;                    //  8
  uint32_t input_y;                     //  9
  uint32_t input_uv;                    // 10
  uint32_t dummy_0;                     // 11
  uint32_t dummy_9;        // 12&13
  uint32_t dummy_10;    // 14&15
  uint32_t UpLeft;                      // 16
  uint32_t DownRight;                   // 17
  uint32_t step_x_on_xy;                // 18
  uint32_t step_x_on_xx;                // 19
  uint32_t step_x_on_yy;                // 20
  uint32_t step_y_on_xy;                // 21
  uint32_t step_y_on_xx;                // 22
  uint32_t step_y_on_yy;                // 23
  uint32_t dummy_1;                     // 24
  uint32_t dummy_2;                     // 25
  uint32_t dummy_3;                     // 26
  uint32_t dummy_4;                     // 27
  uint32_t dummy_5;                     // 28
  uint32_t dummy_6;                     // 29
  uint32_t dummy_7;                     // 30
  uint32_t dummy_8;                     // 31
} mc422_hw_regs_t;

typedef struct {
  mc422_hw_regs_t *base;

  uint16_t        input_width;
  uint16_t        input_height;
  uint16_t        output_width;
  uint16_t        output_height;

  uint8_t         deinterlacing_mode;
  uint8_t         field_mode;
} mc422_device_t;

// MC422 initialisation
void mc422_init( mc422_device_t *device,       // Structure for MC422 module
                 uint32_t base,                // Base address
                 uint16_t input_image_nb_col,  // Number of columns of input  image
                 uint16_t input_image_nb_row,  // Number of lines   of input  image
                 uint16_t output_image_nb_col, // Number of columns of output image
                 uint16_t output_image_nb_row, // Number of lines   of output image
                 uint8_t deinterlacing_mode,   // Deinterlacing mode:    MC_OUTPUT_DEINTERLACING_BOTTOM
                                               //                     or MC_OUTPUT_DEINTERLACING_TOP
                                               //                     or MC_OUTPUT_DEINTERLACING_UNKNOWN
                                               //                     or MC_OUTPUT_DEINTERLACING_DONTCARE
                 uint8_t init_parity           // Parity of first field/frame:    MC_OUTPUT_INTERLEAVED_TOP
                                               //                              or MC_OUTPUT_INTERLEAVED_BOTTOM
                                               //                              or MC_OUTPUT_PROGRESSIVE
);

// Usefull Cosinus/Sinus computation
void compute_cos_sin(float theta, float *c, float *s, int is_interlaced);

// MC422 parameters programming
void mc422_set_params( mc422_device_t *device,    // Structure for MC422 module
                       uint8_t use_compensation,  // Do we use compensation?
                       float theta,
                       float x,
                       float y,
                       float zoom_x,
                       float zoom_y,
                       uint32_t picture_y,        // Address luminance
                       uint32_t picture_uv        // Address chrominance
);

// MC422 status
uint32_t mc422_get_status(mc422_device_t *device);

// MC422 ready to send new frame/field?
uint32_t mc422_ready_to_send(mc422_device_t *device);

// MC422 start (MC422 send new frame/field)
void mc422_start(mc422_device_t *device);
#endif
