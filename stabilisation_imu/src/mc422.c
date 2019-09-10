#include "mc422.h"

#include <stdio.h>


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
)
{
  memset(device,0,sizeof(*device));
  device->base = (mc422_hw_regs_t *)base;

  // Init MC for correct field parity for first start
  mc422_hw_regs_t *hw_regs = device->base;
  IOWR(&hw_regs->control_status, 0, deinterlacing_mode | init_parity);
  device->deinterlacing_mode = deinterlacing_mode;
  device->field_mode         = init_parity;

  device->input_width   = input_image_nb_col;
  device->input_height  = input_image_nb_row;
  device->output_width  = output_image_nb_col;
  device->output_height = output_image_nb_row;
  IOWR(&hw_regs->input_image,  0, (input_image_nb_row  << 16) | input_image_nb_col);
  IOWR(&hw_regs->output_image, 0, (output_image_nb_row << 16) | output_image_nb_col);
}



// Usefull Cosinus/Sinus computation
void compute_cos_sin(float theta, float *c, float *s, int is_interlaced)
{
  float a = theta;
  float a_n;

  if (is_interlaced) {
    if (a > 0.5)
      a = 0.5;
    else if (a < -0.5)
      a = -0.5;
  } else {
    if (a > 0.4)
      a = 0.4;
    else if (a < -0.4)
      a = -0.4;
  }

  *s = a;
  a_n = a * a / 2.0;
  *c = 1.0 - a_n;
  a_n = a_n * a  / 3.0;
  *s -= a_n;
  a_n = a_n * a  / 4.0;
  *c += a_n;
  a_n = a_n * a  / 5.0;
  *s += a_n;

}



// MC422 parameters programming
void mc422_set_params( mc422_device_t *device,    // Structure for MC422 module
                       uint8_t use_compensation,  // Do we use compensation?
                       float theta,               // Angle Theta to compensate
                       float x,                   // Translation X to compensate
                       float y,                   // Translation Y to compensate
                       float zoom_x,              // Zoom X to apply
                       float zoom_y,              // Zoom Y to apply
                       uint32_t picture_y,        // Address luminance
                       uint32_t picture_uv        // Address chrominance
)
{
  uint32_t step_x_on_x;
  uint32_t step_x_on_y;
  uint32_t step_y_on_x;
  uint32_t step_y_on_y;
  uint32_t shift_x;
  uint32_t shift_y;

  float s;
  float c;

  if (device->field_mode==MC_OUTPUT_PROGRESSIVE) {
    compute_cos_sin(theta, &c, &s, 0);
  } else {
    compute_cos_sin(theta, &c, &s, 1);
  }
  step_x_on_x = (int32_t) MC_COEF(c * zoom_x);
  step_y_on_y = (int32_t) MC_COEF(c * zoom_y);

  int xy_zoom , yx_zoom;
   if (device->input_width == 720) {
  	if (device->input_height == 288) {
  		/* PAL */
      yx_zoom = MC_COEF(16.0 / zoom_x / 15.0 / 2.0);
      xy_zoom = MC_COEF(2 * 15.0 / zoom_y / 16.0);
  	} else {
  		/* NTSC */
      yx_zoom = MC_COEF(8.0 / zoom_x / 9.0 / 2.0);
      xy_zoom = MC_COEF(2 * 9.0 / zoom_y / 8.0);
  	}
  } else {
  	if (device->field_mode==MC_OUTPUT_PROGRESSIVE) {
      yx_zoom = MC_COEF(1.0 / zoom_x);
      xy_zoom = MC_COEF(1.0 / zoom_y);
  	} else {
      yx_zoom = MC_COEF(1.0 / zoom_x / 2);
      xy_zoom = MC_COEF(1.0 / zoom_y * 2);
  	}
  }

  step_x_on_y = ((int32_t) MC_COEF(s) * xy_zoom)  >> BILINEAR_COEFF_FRACT_PART_BIT;
  step_y_on_x = -(((int32_t)MC_COEF(s) * yx_zoom) >> BILINEAR_COEFF_FRACT_PART_BIT);

  shift_x = MC_COEF(x) ;
  if (device->field_mode==MC_OUTPUT_PROGRESSIVE) {
    shift_y = MC_COEF(y);
  } else {
    shift_y = MC_COEF(0.5*y);
  }

  mc422_hw_regs_t *hw_regs = device->base;
  if (use_compensation==0) {
    step_x_on_x = MC_COEF(1.0);
    step_x_on_y = 0;
    step_y_on_x = 0;
    step_y_on_y = MC_COEF(1.0);
    shift_x = 0;
    shift_y = 0;
  }

  IOWR(&hw_regs->step_x_on_x, 0, step_x_on_x);
  IOWR(&hw_regs-> step_x_on_y, 0, step_x_on_y);
  IOWR(&hw_regs-> step_x_on_xy, 0, 0);
  IOWR(&hw_regs-> step_x_on_xx, 0, 0);
  IOWR(&hw_regs-> step_x_on_yy, 0, 0);
  IOWR(&hw_regs->step_y_on_x, 0, step_y_on_x);
  IOWR(&hw_regs->step_y_on_y, 0, step_y_on_y);
  IOWR(&hw_regs-> step_y_on_xy, 0, 0);
  IOWR(&hw_regs-> step_y_on_xx, 0, 0);
  IOWR(&hw_regs-> step_y_on_yy, 0, 0);

  IOWR(&hw_regs->input_x0, 0, shift_x);
  IOWR(&hw_regs->input_y0, 0, shift_y);

  IOWR(&hw_regs->input_y, 0, picture_y);
  IOWR(&hw_regs->input_uv, 0, picture_uv);

  //DEBUG MODE
  /*
  printf("\n");
  printf("step_x_on_x %x \n",step_x_on_x);
  printf("step_x_on_y %x \n",step_x_on_y);
  printf("step_x_on_xy %x \n",hw_regs->step_x_on_xy);
  printf("step_x_on_xx %x \n",hw_regs->step_x_on_xx);
  printf("step_x_on_yy %x \n",hw_regs->step_x_on_yy);
  printf("step_y_on_x %x \n",step_y_on_x);
  printf("step_y_on_y %x \n",step_y_on_y);
  printf("step_y_on_xy %x \n", hw_regs->step_y_on_xy);
  printf("step_y_on_xx %x \n", hw_regs->step_y_on_xx);
  printf("step_y_on_yy %x \n", hw_regs->step_y_on_yy);
  printf("shift_x %x \n", shift_x);
  printf("shift_y %x \n", shift_y);
  printf("\n");
  */
  /*
  IOWR(&hw_regs->step_x_on_x, 0, 0x00010000);
  IOWR(&hw_regs->step_x_on_y, 0,  0 );
  IOWR(&hw_regs-> step_x_on_xy, 0, 0);
  IOWR(&hw_regs-> step_x_on_xx, 0, 0);
  IOWR(&hw_regs-> step_x_on_yy, 0, 0);
  IOWR(&hw_regs->step_y_on_x, 0, 0 );
  IOWR(&hw_regs->step_y_on_y, 0, 0x00010000);
  IOWR(&hw_regs-> step_y_on_xy, 0, 0);
  IOWR(&hw_regs-> step_y_on_xx, 0, 0);
  IOWR(&hw_regs-> step_y_on_yy, 0, 0);
  IOWR(&hw_regs->input_x0, 0, 0);
  IOWR(&hw_regs->input_y0, 0, 0);

  IOWR(&hw_regs->input_y, 0, picture_y);
  IOWR(&hw_regs->input_uv, 0, picture_uv);
  */

}



// MC422 status
uint32_t mc422_get_status(mc422_device_t *device)
{
  mc422_hw_regs_t *hw_regs = device->base;
  return IORD(&hw_regs->control_status, 0);
}



// MC422 ready to send new frame/field?
uint32_t mc422_ready_to_send(mc422_device_t *device)
{
  uint32_t status;
  status = ( (mc422_get_status(device) & MC_STATUS_ACTIVE_MASK) == 0 );
  return (status);
}



// MC422 start (MC422 send new frame/field)
void mc422_start(mc422_device_t *device)
{
  mc422_hw_regs_t *hw_regs = device->base;

  if        (device->deinterlacing_mode==MC_OUTPUT_DEINTERLACING_TOP) {
    // We must deinterlace the sequence Top then Bottom (top field first)
    if (device->field_mode==MC_OUTPUT_INTERLEAVED_TOP) {
      // Previous field was a Top, send a Bottom
      IOWR(&hw_regs->control_status, 0, MC_OUTPUT_START_MC | MC_OUTPUT_INTERLEAVED_BOTTOM | MC_OUTPUT_DEINTERLACING_TOP);
      device->field_mode = MC_OUTPUT_INTERLEAVED_BOTTOM;
    } else {
      // Previous field was a Bottom, send a Top
      IOWR(&hw_regs->control_status, 0, MC_OUTPUT_START_MC | MC_OUTPUT_INTERLEAVED_TOP | MC_OUTPUT_DEINTERLACING_TOP);
      device->field_mode = MC_OUTPUT_INTERLEAVED_TOP;
    }
  } else if (device->deinterlacing_mode==MC_OUTPUT_DEINTERLACING_BOTTOM) {
    // We must deinterlace the sequence Bottom then Top (bottom field first)
    if (device->field_mode==MC_OUTPUT_INTERLEAVED_TOP) {
      // Previous field was a Top, send a Bottom
      IOWR(&hw_regs->control_status, 0, MC_OUTPUT_START_MC | MC_OUTPUT_INTERLEAVED_BOTTOM | MC_OUTPUT_DEINTERLACING_BOTTOM);
      device->field_mode = MC_OUTPUT_INTERLEAVED_BOTTOM;
    } else {
      // Previous field was a Bottom, send a Top
      IOWR(&hw_regs->control_status, 0, MC_OUTPUT_START_MC | MC_OUTPUT_INTERLEAVED_TOP | MC_OUTPUT_DEINTERLACING_BOTTOM);
      device->field_mode = MC_OUTPUT_INTERLEAVED_TOP;
    }
  } else {
    if (device->field_mode==MC_OUTPUT_INTERLEAVED_TOP) {
      // Previous field was a Top, send a Bottom
      IOWR(&hw_regs->control_status, 0, MC_OUTPUT_START_MC | MC_OUTPUT_INTERLEAVED_BOTTOM | device->deinterlacing_mode);
      device->field_mode = MC_OUTPUT_INTERLEAVED_BOTTOM;
    } else if (device->field_mode==MC_OUTPUT_INTERLEAVED_BOTTOM) {
      // Previous field was a Bottom, send a Top
      IOWR(&hw_regs->control_status, 0, MC_OUTPUT_START_MC | MC_OUTPUT_INTERLEAVED_TOP | device->deinterlacing_mode);
      device->field_mode = MC_OUTPUT_INTERLEAVED_TOP;
    } else {
      IOWR(&hw_regs->control_status, 0, MC_OUTPUT_START_MC | device->field_mode | device->deinterlacing_mode);
    }
  }
}


 
 /*
  void main(void)
  {
    mc422_init(device, .., .., ..);
    while(1)
    {
      while(mc422_ready_to_send(device) ==0 );
      mc422_set_params();
      mc422_start();
    }
  }
*/
