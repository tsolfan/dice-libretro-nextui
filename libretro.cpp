#include <sstream>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "libretro_core_options.h"

#include <stdio.h>
#if defined(_WIN32) && !defined(_XBOX)
#include <windows.h>
#endif
#include "libretro.h"
#include "dice.h"
#ifdef MANYMOUSE
#include "manymouse/manymouse.h"
#endif

static uint8_t *frame_buf;
static uint8_t *retro_frame_buf;
static struct retro_log_callback logging;
retro_log_printf_t log_cb;
static bool use_audio_cb;
static float last_aspect;
static float last_sample_rate;
char retro_base_directory[4096];
char retro_game_path[4096];
static bool screen_horizontal = true;
static uint32_t max_height;

static dice_libretro::DICE dice;

static void fallback_log(enum retro_log_level level, const char *fmt, ...)
{
   (void)level;
   va_list va;
   va_start(va, fmt);
   vfprintf(stderr, fmt, va);
   va_end(va);
}


//static
retro_environment_t environ_cb;

static unsigned color;  // TODO (kmitton)

void retro_init(void)
{
   max_height = VIDEO_HEIGHT;  // Until we load the video description.

   // Screen height jitters a little in some games, leave some extra buffer space.
   int safety_factor = 2;
   frame_buf = (uint8_t*)malloc(VIDEO_PIXELS * VIDEO_BYTES_PER_PIXEL * safety_factor);
   retro_frame_buf = (uint8_t*)malloc(VIDEO_PIXELS * VIDEO_BYTES_PER_PIXEL * safety_factor);

   const char *dir = NULL;
   if (environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &dir) && dir)
   {
      snprintf(retro_base_directory, sizeof(retro_base_directory), "%s", dir);
   }
#ifdef MANYMOUSE
   int available_mice = ManyMouse_Init();

   if (available_mice < 0)
      log_cb(RETRO_LOG_ERROR, "ManyMouse failed to initialize!\n");
   else if (available_mice == 0)
      log_cb(RETRO_LOG_INFO, "No mice detected!\n");
   else if (available_mice > 1)
   {
      // Extra mice, list them.
      // (Or maybe just Wayland on Linux):
      // https://github.com/icculus/manymouse/issues/10
      char buffer[255];
      struct retro_message message;
      message.msg = buffer;
      message.frames = 120;

      snprintf(buffer, sizeof(buffer), "ManyMouse driver: %s\n", ManyMouse_DriverName());
      environ_cb(RETRO_ENVIRONMENT_SET_MESSAGE, &message);
      log_cb(RETRO_LOG_INFO, buffer);

      for (int i = 0; i < available_mice; i++)
      {
         snprintf(buffer, sizeof(buffer), "Mouse #%d: %s\n", i, ManyMouse_DeviceName(i));
         environ_cb(RETRO_ENVIRONMENT_SET_MESSAGE, &message);
         log_cb(RETRO_LOG_INFO, buffer);
      }
   }
#endif

   color = 0;

}

void retro_deinit(void)
{
   free(frame_buf);
   frame_buf = NULL;

   free(retro_frame_buf);
   retro_frame_buf = NULL;

#ifdef MANYMOUSE
   // TODO (mittonk): Is this too early?  Or too late?
   ManyMouse_Quit();
#endif
}

unsigned retro_api_version(void)
{
   return RETRO_API_VERSION;
}

void retro_set_controller_port_device(unsigned port, unsigned device)
{
   log_cb(RETRO_LOG_INFO, "Plugging device %u into port %u.\n", device, port);
}

void retro_get_system_info(struct retro_system_info *info)
{
   memset(info, 0, sizeof(*info));
   info->library_name     = "dice";
   info->library_version  = "0.4.2";
   info->need_fullpath    = true;
   // dmy are launcher files for rom-less games like pong.
   // zip means we support zipped files (maybe not needed here?)
   // The others are extensions on various rom dumps, which aren't standardized
   // at all.
   info->valid_extensions = "zip|dmy|k1|a1|6c|c6|d2|s1|f4|a4|1da|da1|C4|4c|4d|d7|d4";
   info->block_extract = true;
}

retro_video_refresh_t video_cb;
retro_audio_sample_t audio_cb;
static retro_audio_sample_batch_t audio_batch_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;

void retro_get_system_av_info(struct retro_system_av_info *info)
{
   // Some games use vertical monitors, won't know till we load it.
   //float aspect = screen_horizontal ? 4.0f / 3.0f : 3.0f / 4.0f;
   float aspect = 4.0f / 3.0f;
   float sampling_rate         = 30000.0f;

   info->timing = (struct retro_system_timing) {
      .fps = 60.0,
         .sample_rate = 0.0,
   };
   info->geometry.base_width   = VIDEO_WIDTH;
   info->geometry.base_height  = max_height;
   info->geometry.max_width    = VIDEO_WIDTH;
   log_cb(RETRO_LOG_DEBUG, "max_height in av_info %d\n", max_height);
   info->geometry.max_height   = VIDEO_HEIGHT;
   info->geometry.aspect_ratio = aspect;

   last_aspect                 = aspect;
   last_sample_rate            = sampling_rate;
}


void retro_set_environment(retro_environment_t cb)
{
   bool option_cats_supported = false;

   environ_cb = cb;

   if (cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &logging))
      log_cb = logging.log;
   else
      log_cb = fallback_log;


   libretro_set_core_options(environ_cb,
         &option_cats_supported);
}

/*
   void retro_set_environment(retro_environment_t cb)
   {
   environ_cb = cb;

   if (cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &logging))
   log_cb = logging.log;
   else
   log_cb = fallback_log;

   static const struct retro_controller_description controllers[] = {
   { "Arcade stick", RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_ANALOG, 0) },
   };

   static const struct retro_controller_info ports[] = {
   { controllers, 1 },
   { NULL, 0 },
   };

   cb(RETRO_ENVIRONMENT_SET_CONTROLLER_INFO, (void*)ports);

   unsigned core_options_version = 0;
   if (!environ_cb(RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION, &core_options_version))
   core_options_version = 0;

   if (core_options_version >= 2)
   {
   } else {
   static const struct retro_variable vars[] = {
   { "dice_use_mouse_pointer_for_paddle_1", "Use mouse pointer for paddle 1; false|true" },

#ifdef MANYMOUSE
{ "dice_manymouse_paddle0", "Mouse-Paddle 1; false|true" },
{ "dice_manymouse_paddle0_x", "Mouse-Paddle 1 x; 0x|0x|0y|1x|1y|2x|2y|3x|3y" },
{ "dice_manymouse_paddle0_y", "Mouse-Paddle 1 y; 0y|0x|0y|1x|1y|2x|2y|3x|3y" },

{ "dice_manymouse_paddle1", "Mouse-Paddle 2; false|true" },
{ "dice_manymouse_paddle1_x", "Mouse-Paddle 2 x; 1x|0x|0y|1x|1y|2x|2y|3x|3y" },
{ "dice_manymouse_paddle1_y", "Mouse-Paddle 2 y; 1y|0x|0y|1x|1y|2x|2y|3x|3y" },

{ "dice_manymouse_paddle2", "Mouse-Paddle 3; false|true" },
{ "dice_manymouse_paddle2_x", "Mouse-Paddle 3 x; 2x|0x|0y|1x|1y|2x|2y|3x|3y" },
{ "dice_manymouse_paddle2_y", "Mouse-Paddle 3 y; 2y|0x|0y|1x|1y|2x|2y|3x|3y" },

{ "dice_manymouse_paddle3", "Mouse-Paddle 4; false|true" },
{ "dice_manymouse_paddle3_x", "Mouse-Paddle 4 x; 3x|0x|0y|1x|1y|2x|2y|3x|3y" },
{ "dice_manymouse_paddle3_y", "Mouse-Paddle 4 y; 3y|0x|0y|1x|1y|2x|2y|3x|3y" },
#endif

{ "dice_paddle_joystick_absolute", "Paddle joystick absolute; false|true" },
{ "dice_paddle_keyboard_sensitivity", "Paddle D-pad sensitivity; 250|125|375|500" },
{ "dice_paddle_joystick_sensitivity", "Paddle analog stick sensitivity; 500|125|250|375" },
{ "dice_wheel_keyjoy_sensitivity", "Wheel sensitivity; 500|125|250|375" },
{ "dice_throttle_keyjoy_sensitivity", "Throttle sensitivity; 250|125|375|500" },

{ "dice_dipswitch_1", "Dipswitch 1; -1|0|1" },
{ "dice_dipswitch_2", "Dipswitch 2; -1|0|1" },
{ "dice_dipswitch_3", "Dipswitch 3; -1|0|1" },

{ "dice_dipswitch16_1", "Dipswitch16 1; -1|0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15" },
{ "dice_dipswitch16_2", "Dipswitch16 2; -1|0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15" },

{ NULL, NULL },
};

cb(RETRO_ENVIRONMENT_SET_VARIABLES, (void*)vars);
}
}
*/

void retro_set_audio_sample(retro_audio_sample_t cb)
{
   audio_cb = cb;
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb)
{
   audio_batch_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb)
{
   input_poll_cb = cb;
}

void retro_set_input_state(retro_input_state_t cb)
{
   input_state_cb = cb;
}

void retro_set_video_refresh(retro_video_refresh_t cb)
{
   video_cb = cb;
}

void retro_reset(void)
{
   dice.reset();
}

static void update_input(void)
{
   int32_t input_bitmask[NUM_CONTROLLERS];
   int32_t input_analog_left_x[NUM_CONTROLLERS];
   int32_t input_analog_left_y[NUM_CONTROLLERS];
   int32_t input_pointer_x[NUM_CONTROLLERS];
   int32_t input_pointer_y[NUM_CONTROLLERS];
   int32_t input_mouse_x[NUM_CONTROLLERS];
   int32_t input_mouse_y[NUM_CONTROLLERS];

   for (unsigned pad = 0; pad < NUM_CONTROLLERS; pad++)
   {
      input_bitmask[(pad)] = 0;
      for (int i = 0; i <= RETRO_DEVICE_ID_JOYPAD_R3; i++) \
         input_bitmask[(pad)] |= input_state_cb((pad), RETRO_DEVICE_JOYPAD, 0, i) ? (1 << i) : 0 ;

      input_analog_left_x[pad] = input_state_cb( (pad), RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT,
            RETRO_DEVICE_ID_ANALOG_X);

      input_analog_left_y[pad] = input_state_cb( (pad), RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT,
            RETRO_DEVICE_ID_ANALOG_Y);

      input_mouse_x[pad] = input_state_cb( (pad), RETRO_DEVICE_MOUSE, pad, RETRO_DEVICE_ID_MOUSE_X);
      input_mouse_y[pad] = input_state_cb( (pad), RETRO_DEVICE_MOUSE, pad, RETRO_DEVICE_ID_MOUSE_Y);
      /* bool mouse_pressed = input_state_cb((pad), RETRO_DEVICE_MOUSE, pad, RETRO_DEVICE_ID_MOUSE_LEFT);
         if (mouse_pressed)
         log_cb(RETRO_LOG_INFO, "Mouse #: %d    : (%6d, %6d).\n", pad, input_mouse_x[pad], input_mouse_y[pad]);
         */
   }

   unsigned pad = 0;
   bool pointer_pressed = input_state_cb(pad, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_PRESSED);
   int16_t pointer_x = input_state_cb(pad, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_X);
   int16_t pointer_y = input_state_cb(pad, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_Y);
   //if (pointer_pressed)
   //log_cb(RETRO_LOG_INFO, "Pointer Pressed #: %d    : (%6d, %6d).\n", pad, pointer_x, pointer_y);

   //log_cb(RETRO_LOG_INFO, "Pointer #: %d    : (%6d, %6d).\n", pad, pointer_x, pointer_y);
   input_pointer_x[pad] = pointer_x;
   input_pointer_y[pad] = pointer_y;

   dice.update_input(input_bitmask, input_analog_left_x, input_analog_left_y, input_pointer_x, input_pointer_y, input_mouse_x, input_mouse_y);
}

static void check_variables(void)
{
   struct retro_variable var = {0};

   var.key = "dice_use_mouse_pointer_for_paddle_1";
   var.value = NULL;
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      bool use_mouse_pointer_for_paddle_1 = !strcmp(var.value, "enabled") ? true : false;
      dice.set_use_mouse_pointer_for_paddle_1(use_mouse_pointer_for_paddle_1);
      log_cb(RETRO_LOG_INFO, "Key -> Val: %s -> %s.\n", var.key, var.value);
   }

   char buffer[50];

   for (unsigned paddle=0; paddle < 4; paddle++)
   {
      snprintf(buffer, sizeof(buffer), "dice_retromouse_paddle%d", paddle);
      var.key = buffer;
      var.value = NULL;
      if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
      {
         bool val = !strcmp(var.value, "enabled") ? true : false;
         dice.set_retromouse_enabled(paddle, val);
         log_cb(RETRO_LOG_INFO, "Key -> Val: %s -> %s.\n", var.key, var.value);
      }

      snprintf(buffer, sizeof(buffer), "dice_retromouse_paddle%d_x", paddle);
      var.key = buffer;
      var.value = NULL;
      if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
      {
         dice.set_retromouse_axis(paddle, 0, var.value);
         log_cb(RETRO_LOG_INFO, "Key -> Val: %s -> %s.\n", var.key, var.value);
      }

      snprintf(buffer, sizeof(buffer), "dice_retromouse_paddle%d_y", paddle);
      var.key = buffer;
      var.value = NULL;
      if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
      {
         dice.set_retromouse_axis(paddle, 1, var.value);
         log_cb(RETRO_LOG_INFO, "Key -> Val: %s -> %s.\n", var.key, var.value);
      }
   }

#ifdef MANYMOUSE
   for (unsigned paddle=0; paddle < 4; paddle++)
   {
      snprintf(buffer, sizeof(buffer), "dice_manymouse_paddle%d", paddle);
      var.key = buffer;
      var.value = NULL;
      if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
      {
         bool val = !strcmp(var.value, "enabled") ? true : false;
         dice.set_manymouse_enabled(paddle, val);
         log_cb(RETRO_LOG_INFO, "Key -> Val: %s -> %s.\n", var.key, var.value);
      }

      snprintf(buffer, sizeof(buffer), "dice_manymouse_paddle%d_x", paddle);
      var.key = buffer;
      var.value = NULL;
      if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
      {
         dice.set_manymouse_axis(paddle, 0, var.value);
         log_cb(RETRO_LOG_INFO, "Key -> Val: %s -> %s.\n", var.key, var.value);
      }

      snprintf(buffer, sizeof(buffer), "dice_manymouse_paddle%d_y", paddle);
      var.key = buffer;
      var.value = NULL;
      if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
      {
         dice.set_manymouse_axis(paddle, 1, var.value);
         log_cb(RETRO_LOG_INFO, "Key -> Val: %s -> %s.\n", var.key, var.value);
      }
   }
#endif

   var.key = "dice_paddle_joystick_absolute";
   var.value = NULL;
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (!strcmp(var.value, "disabled"))
         dice.set_paddle_joystick_absolute(false);
      else
         dice.set_paddle_joystick_absolute(true);
   }

   var.key = "dice_paddle_keyboard_sensitivity";
   var.value = NULL;
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      dice.set_paddle_keyboard_sensitivity(atoi(var.value));
   }

   var.key = "dice_retromouse_paddle_sensitivity";
   var.value = NULL;
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      dice.set_paddle_retromouse_sensitivity(atoi(var.value));
   }

   var.key = "dice_paddle_joystick_sensitivity";
   var.value = NULL;
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      dice.set_paddle_joystick_sensitivity(atoi(var.value));
   }

   var.key = "dice_wheel_keyjoy_sensitivity";
   var.value = NULL;
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      dice.set_wheel_keyjoy_sensitivity(atoi(var.value));
   }

   var.key = "dice_throttle_keyjoy_sensitivity";
   var.value = NULL;
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      dice.set_throttle_keyjoy_sensitivity(atoi(var.value));
   }

   for (unsigned dipswitch=1; dipswitch<=3; dipswitch++)
   {
      snprintf(buffer, sizeof(buffer), "dice_dipswitch_%d", dipswitch);
      var.key = buffer;
      var.value = NULL;
      if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
      {
         dice.set_dipswitch(var.key, atoi(var.value));
      }
   }

   for (unsigned dipswitch=1; dipswitch<=2; dipswitch++)
   {
      snprintf(buffer, sizeof(buffer), "dice_dipswitch16_%d", dipswitch);
      var.key = buffer;
      var.value = NULL;
      if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
      {
         dice.set_dipswitch(var.key, atoi(var.value));
      }
   }

}

static void audio_callback(void)
{
}

static void audio_set_state(bool enable)
{
   (void)enable;
}

typedef uint16_t pixel_t;

void retro_run(void)
{
   update_input();

   dice.run();
   //dice.render_frame();

   bool updated = false;
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated)
      check_variables();
}

bool retro_load_game(const struct retro_game_info *info)
{
   struct retro_input_descriptor desc[] = {
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "Left" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "Up" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "Down" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Right" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Coin" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START,  "Start" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,  "Button 2" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,  "Button 1" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,  "Button 3" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,  "Dollar" },

      { 0, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT, RETRO_DEVICE_ID_ANALOG_X, "Left Analog X" },
      { 0, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT, RETRO_DEVICE_ID_ANALOG_Y, "Left Analog Y" },
      { 0 },
   };

   environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, desc);

   enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_RGB565;
   if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt))
   {
      log_cb(RETRO_LOG_INFO, "RGB565 is not supported.\n");
      return false;
   }

   snprintf(retro_game_path, sizeof(retro_game_path), "%s", info->path);
   struct retro_audio_callback audio_cb = { audio_callback, audio_set_state };
   use_audio_cb = environ_cb(RETRO_ENVIRONMENT_SET_AUDIO_CALLBACK, &audio_cb);

   check_variables();

   uint16_t *pixel_buf = reinterpret_cast<uint16_t *>(frame_buf);
   uint16_t *retro_pixel_buf = reinterpret_cast<uint16_t *>(retro_frame_buf);
   dice.load_game(info->path, pixel_buf, retro_pixel_buf);

   // If game uses a vertical monitor, tell libretro and adjust aspect ratio.
   screen_horizontal = dice.game_video_rotation == ROTATE_0 || dice.game_video_rotation == ROTATE_180;
   environ_cb(RETRO_ENVIRONMENT_SET_ROTATION, &dice.game_video_rotation);

   // Set current screen size based on circuit description; the maximum stays at VIDEO_HEIGHT,
   // which is what the video chip actually pushes every frame.
   max_height = dice.max_height;
   struct retro_system_av_info avinfo;
   retro_get_system_av_info(&avinfo);
   environ_cb(RETRO_ENVIRONMENT_SET_GEOMETRY, &avinfo);
   // Now that we've got a circuit up, configure input sensitivity.
   check_variables();

   (void)info;

   return true;
}

void retro_unload_game(void)
{
}

unsigned retro_get_region(void)
{
   return RETRO_REGION_NTSC;
}

bool retro_load_game_special(unsigned type, const struct retro_game_info *info, size_t num)
{
   return false;
}

size_t retro_serialize_size(void)
{
   return 0;
}

bool retro_serialize(void *data_, size_t size)
{
   return false;
}

bool retro_unserialize(const void *data_, size_t size)
{
   return false;
}

void *retro_get_memory_data(unsigned id)
{
   (void)id;
   return NULL;
}

size_t retro_get_memory_size(unsigned id)
{
   (void)id;
   return 0;
}

void retro_cheat_reset(void)
{}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{
   (void)index;
   (void)enabled;
   (void)code;
}

