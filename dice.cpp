#include <iostream>
#include <fstream>
#include <regex>
#include <stdexcept>
#include <string>

//#include <phoenix.hpp>
#include "dice.h"

#include "circuit.h"
#include "circuit_desc.h"

#include "game_list.h"
#include "chips/rom.h"
#include "filename.h"
#include "libretro.h"

/*
   using phoenix::VerticalLayout;
   using phoenix::Viewport;
   */

extern retro_environment_t environ_cb;
extern retro_log_printf_t log_cb;


namespace dice_libretro {

   void DICE::load_game(const char *path, uint16_t *pixel_buf, uint16_t *retro_pixel_buf)
   {
      /*
         input = new Input();
         video = Video::createDefault(layout, viewport);
         */
      input = new Input();
      video = new Video();
      video->pixel_buf = pixel_buf;
      video->retro_pixel_buf = retro_pixel_buf;

      video->video_init(VIDEO_WIDTH, VIDEO_HEIGHT);

      log_cb(RETRO_LOG_DEBUG, "Filename from libretro: %s\n", path);

      // Some RetroArch playlists supply entries like
      // "game.zip#inner.rom".  We want just "game.zip".
      std::string ziphash (".zip#");
      std::string path_no_inner (path);
      std::size_t found = path_no_inner.rfind(ziphash);
      log_cb(RETRO_LOG_DEBUG, "Any '.zip#', found at position: %d\n", found);
      if (found != std::string::npos)
         path_no_inner.replace(found, std::string::npos, ".zip");
      else if (found == std::string::npos)
      {
         found = path_no_inner.rfind(".ZIP#");  // Cover simple uppercase.
         log_cb(RETRO_LOG_DEBUG, "Any '.ZIP#', found at position: %d\n", found);
         if (found != std::string::npos)
            path_no_inner.replace(found, std::string::npos, ".ZIP");
      }
      log_cb(RETRO_LOG_DEBUG, "Filename with any '.zip#...' removed: %s\n", path_no_inner.c_str());
      //const string extension = nall::extension(path_no_inner);
      //log_cb(RETRO_LOG_DEBUG, "File extension: %s\n", extension.c_str());

      const string basename = nall::basename(nall::notdir(path_no_inner));
      log_cb(RETRO_LOG_INFO, "File basename, for game matching: %s\n", basename.c_str());

      RomDesc::set_zip_filename(path_no_inner.c_str());
      int i = 0;
      for (i = 0; i < game_list_size; i++) {
         if (game_list[i].command_line == basename)
         {
            break;
         }
      }
      if (i == game_list_size)
      {
         log_cb(RETRO_LOG_ERROR, "Fatal: No matching game for that filename.  Filenames matter.");
         exit(1);  // TODO (mittonk): Seems rude, what's cleaner?
      }

      game_desc = &game_list[i];

      circuit = new Circuit(settings,
            *input,
            *video,
            game_desc->desc, game_desc->command_line);

      // Export monitor orientation towards libretro.
      if (circuit) game_video_rotation = circuit->video.desc->orientation;
      // Export monitor height towards libretro.
      if (circuit) max_height = circuit->video.desc->retro_v_size;

   }

   void DICE::run(void)
   {
      // Run until the video frame is finished.
      if(circuit)
      {
         // Run until we've got a full video frame, but not much more.
         while (!circuit->video.frame_done && 
               // Bail if it's taken 2+ wall clock seconds since the last input check / video
               // frame.
               circuit->rtc.get_usecs() < circuit->last_input_update_timestamp + RETRO_WATCHDOG_USECS) {
            circuit->run(1.0e-3 / Circuit::timescale); // Run 1 ms
         }
         circuit->video.frame_done = false;
      }
   }

   void DICE::render_frame(void)
   {
   }

   void DICE::update_input(int32_t input_state[], int32_t input_analog_left_x[], int32_t input_analog_left_y[],
         int32_t input_pointer_x[], int32_t input_pointer_y[],
         int32_t input_mouse_x[], int32_t input_mouse_y[])
   {
#ifdef MANYMOUSE
      input->poll_input(); // Mice handled by manymouse
#endif

      if (circuit)
      {
         for (unsigned i=0; i<NUM_CONTROLLERS; i++)
         {
            circuit->input.input_state[i] = input_state[i];
            circuit->input.input_analog_left_x[i] = input_analog_left_x[i];
            circuit->input.input_analog_left_y[i] = input_analog_left_y[i];
            circuit->input.input_pointer_x[i] = input_pointer_x[i];
            circuit->input.input_pointer_y[i] = input_pointer_y[i];
            circuit->input.input_mouse_x[i] = input_mouse_x[i];
            circuit->input.input_mouse_y[i] = input_mouse_y[i];
         }

         circuit->last_input_update_timestamp = circuit->rtc.get_usecs();
      }
   }

   void DICE::reset(void)
   {
      if(circuit) delete circuit;
      circuit = new Circuit(settings, *input, *video, game_desc->desc, game_desc->command_line);
   }

   void DICE::set_use_mouse_pointer_for_paddle_1(bool val)
   {
      if (circuit) circuit->input.use_mouse_pointer_for_paddle_1 = val;
   }

   void DICE::set_retromouse_enabled(unsigned paddle, bool val)
   {
      if (circuit) circuit->input.retromouse_enabled[paddle] = val;
   }

   void DICE::set_retromouse_axis(unsigned paddle, unsigned axis, const char* axis_name)
   {
      unsigned mouse_axis = 0;
      switch (axis_name[0]) {
         case 'x': mouse_axis = 0; break;
         case 'y': mouse_axis = 1; break;
      }

      if (circuit)
      {
         if (axis == 0)
         {
            circuit->input.retromouse_settings[paddle].settings_x_axis_axis = mouse_axis;
         } else {
            circuit->input.retromouse_settings[paddle].settings_y_axis_axis = mouse_axis;
         }
      }
   }

#ifdef MANYMOUSE
   void DICE::set_manymouse_enabled(unsigned paddle, bool val)
   {
      if (circuit) circuit->input.manymouse_enabled[paddle] = val;
   }

   void DICE::set_manymouse_axis(unsigned paddle, unsigned axis, const char* axis_name)
   {
      unsigned mouse_idx = 0;
      unsigned mouse_axis = 0;
      switch (axis_name[0]) {
         case '0': mouse_idx = 0; break;
         case '1': mouse_idx = 1; break;
         case '2': mouse_idx = 2; break;
         case '3': mouse_idx = 3; break;
      }
      switch (axis_name[1]) {
         case 'x': mouse_axis = 0; break;
         case 'y': mouse_axis = 1; break;
      }

      if (circuit)
      {
         if (axis == 0)
         {
            circuit->input.manymouse_settings[paddle].settings_x_axis_mouse = mouse_idx;
            circuit->input.manymouse_settings[paddle].settings_x_axis_axis = mouse_axis;
         } else {
            circuit->input.manymouse_settings[paddle].settings_y_axis_mouse = mouse_idx;
            circuit->input.manymouse_settings[paddle].settings_y_axis_axis = mouse_axis;
         }
      }
   }
#endif

   void DICE::set_paddle_joystick_absolute(bool paddle_joystick_absolute)
   {
      if (circuit) circuit->input.paddle_joystick_absolute = paddle_joystick_absolute;
   }

   void DICE::set_paddle_joystick_paddle_1_paddle_2_combo(bool paddle_joystick_paddle_1_paddle_2_combo)
   {
      if (circuit) circuit->input.paddle_joystick_paddle_1_paddle_2_combo = paddle_joystick_paddle_1_paddle_2_combo;
   }

   bool DICE::get_paddle_joystick_paddle_1_paddle_2_combo() const
   {
      return circuit && circuit->input.paddle_joystick_paddle_1_paddle_2_combo;
   }

   void DICE::set_paddle_keyboard_sensitivity(int val)
   {
      if (circuit) circuit->input.paddle_keyboard_sensitivity = val;
   }

   void DICE::set_paddle_joystick_sensitivity(int val)
   {
      if (circuit) circuit->input.paddle_joystick_sensitivity = val;
   }

   void DICE::set_paddle_retromouse_sensitivity(int val)
   {
      if (circuit) circuit->input.paddle_retromouse_sensitivity = val;
   }

   void DICE::set_wheel_keyjoy_sensitivity(int val)
   {
      if (circuit) circuit->input.wheel_keyjoy_sensitivity = val;
   }

   void DICE::set_throttle_keyjoy_sensitivity(int val)
   {
      if (circuit) circuit->input.throttle_keyjoy_sensitivity = val;
   }

   void DICE::set_dipswitch(const char* retro_dipswitch_key, int val)
   {
      // Brute force, but only done on game load and manual config
      // changes, so maybe good enough.
      if (circuit)
      {

         const std::string key(retro_dipswitch_key);
         DipswitchBase *desc = circuit->game_config.dipswitch_map[key];

         if (desc)
         {
            if (val == -1)
            {
               log_cb(RETRO_LOG_DEBUG, "Setting dipswitch %s to %d meaning default value %d\n", retro_dipswitch_key, val, desc->retro_default_state);
               desc->state = desc->retro_default_state;
            } else {
               log_cb(RETRO_LOG_DEBUG, "Setting dipswitch %s to %d\n", retro_dipswitch_key, val);
               desc->state = val;
            }
         }
      }
   }

} // namespace
