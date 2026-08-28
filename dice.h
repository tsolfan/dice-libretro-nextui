#ifndef DICE_H__
#define DICE_H__

#include <vector>

#include "circuit.h"
#include "circuit_desc.h"
#include "game_list.h"

using std::vector;
/*
   using phoenix::VerticalLayout;
   using phoenix::Viewport; */

namespace dice_libretro {

#define VIDEO_WIDTH 640 // TODO (mittonk): Draw from dice.
                        //#define VIDEO_HEIGHT 480
                        //#define VIDEO_HEIGHT 246
#define VIDEO_HEIGHT 268
#define VIDEO_BYTES_PER_PIXEL 2
#define VIDEO_PIXELS VIDEO_WIDTH * VIDEO_HEIGHT
#define VIDEO_PITCH VIDEO_WIDTH * VIDEO_BYTES_PER_PIXEL

#define NUM_CONTROLLERS 6

   // RetroArch mask_id is a bit position, so do some shifting.
   inline bool input_mask(int32_t input_bitmask, unsigned mask_id)
   {
      return bool(input_bitmask & (1 << mask_id));
   }


   // Encapsulate the DICE emulator, for use in the libretro core.
   class DICE
   {
      private:
         Settings settings;
         Input* input;
         Video* video;
         Circuit* circuit;
         RealTimeClock real_time;
         GameDesc* game_desc;

         //VerticalLayout layout;
         //Viewport* viewport;

      public:
         VideoOrientation game_video_rotation = ROTATE_0;
         uint32_t max_height;

         void load_game(const char *path, uint16_t *pixel_buf, uint16_t *retro_pixel_buf);
         void update_input(int32_t *input_state, int32_t *input_analog_left_x, int32_t *input_analog_left_y,
               int32_t *input_pointer_x, int32_t *input_pointer_y,
               int32_t *input_mouse_x, int32_t *input_mouse_y);
         void run(void);
         void render_frame(void);
         void reset(void);

         void set_use_mouse_pointer_for_paddle_1(bool val);
         void set_retromouse_enabled(unsigned paddle, bool val);
         void set_retromouse_axis(unsigned paddle, unsigned axis, const char* axis_name);
#ifdef MANYMOUSE
         void set_manymouse_enabled(unsigned paddle, bool val);
         void set_manymouse_axis(unsigned paddle, unsigned axis, const char* axis_name);
#endif

         void set_paddle_joystick_absolute(bool paddle_joystick_absolute);
         void set_paddle_joystick_paddle_1_paddle_2_combo(bool paddle_joystick_paddle_1_paddle_2_combo);
         bool get_paddle_joystick_paddle_1_paddle_2_combo() const;
         void set_paddle_keyboard_sensitivity(int val);
         void set_paddle_joystick_sensitivity(int val);
         void set_paddle_retromouse_sensitivity(int val);
         void set_wheel_keyjoy_sensitivity(int val);
         void set_throttle_keyjoy_sensitivity(int val);
         void set_dipswitch(const char* retro_dipswitch_key, int val);

   };


} // namespace
#endif
