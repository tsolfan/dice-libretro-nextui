#ifndef LIBRETRO_CORE_OPTIONS_H__
#define LIBRETRO_CORE_OPTIONS_H__

#include <stdlib.h>
#include <string.h>

#include "libretro.h"
#include "retro_inline.h"

#ifndef HAVE_NO_LANGEXTRA
#include "libretro_core_options_intl.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct retro_core_option_v2_category option_cats_us[] = {
   {
      "input",
      "Input",
      "Configure core-specific input settings."
   },
   {
      "retromouse",
      "RetroMouse",
      "Configure libretro multiple-mouse settings."
   },
#ifdef MANYMOUSE
   {
      "manymouse",
      "ManyMouse",
      "Configure ManyMouse multiple-mouse settings."
   },
#endif
   {
      "dipswitch",
      "DIP Switch",
      "Configure DIP switch and potentiometer settings."
   },
   { NULL, NULL, NULL },
};

struct retro_core_option_v2_definition option_defs_us[] = {
   {
      "dice_use_mouse_pointer_for_paddle_1",
      "Input > Use Mouse Pointer For Paddle 1",
      "Use Mouse Pointer For Paddle 1",
      "Use the system mouse pointer as paddle controller 1.  Does not let you choose a specific mouse if you have several.",
      NULL,
      "input",
      {
         {"disabled", NULL},
         {"enabled", NULL},
         {NULL, NULL},
      },
      "disabled"
   },
   {
      "dice_paddle_joystick_absolute",
      "Input > Paddle Joystick Uses Absolute Motion",
      "Paddle Joystick Uses Absolute Motion",
      "Directly map joystick movement range to paddle range, rather than relative motions.",
      NULL,
      "input",
      {
         {"disabled", NULL},
         {"enabled", NULL},
         {NULL, NULL},
      },
      "disabled"
   },
   {
      "dice_paddle_joystick_paddle_1_paddle_2_combo",
      "Input > Use Face Buttons for Paddle 2 Controls",
      "Use Face Buttons for Paddle 2 Controls",
      "Allows for multiplayer on a single device.",
      NULL,
      "input",
      {
         {"disabled", NULL},
         {"enabled", NULL},
         {NULL, NULL},
      },
      "disabled"
   },
   {
      // D-pad but not necessarily keyboard under libretro,
      // that name comes from the upstream code.
      "dice_paddle_keyboard_sensitivity",
      "Input > Paddle D-pad Sensitivity",
      "Paddle D-pad Sensitivity",
      "Sensitivity when using D-pad for a paddle.",
      NULL,
      "input",
      {
         {"25", NULL},
         {"50", NULL},
         {"75", NULL},
         {"125", NULL},
         {"250", NULL},
         {"375", NULL},
         {"500", NULL},
         {NULL, NULL},
      },
      "250"
   },
   {
      "dice_paddle_joystick_sensitivity",
      "Input > Paddle Analog Stick Sensitivity",
      "Paddle Analog Stick Sensitivity",
      "Sensitivity when using analog stick for a paddle.",
      NULL,
      "input",
      {
         {"25", NULL},
         {"50", NULL},
         {"75", NULL},
         {"125", NULL},
         {"250", NULL},
         {"375", NULL},
         {"500", NULL},
         {NULL, NULL},
      },
      "500"
   },
   {
      "dice_retromouse_paddle_sensitivity",
      "Input > Paddle RetroMouse Sensitivity",
      "Paddle RetroMouse Sensitivity",
      "Sensitivity when using RetroMouse for a paddle.",
      NULL,
      "input",
      {
         {"25", NULL},
         {"50", NULL},
         {"75", NULL},
         {"100", NULL},
         {"125", NULL},
         {"250", NULL},
         {"375", NULL},
         {"500", NULL},
         {NULL, NULL},
      },
      "125"
   },
   {
      "dice_wheel_keyjoy_sensitivity",
      "Input > Wheel Sensitivity",
      "Wheel Sensitivity",
      "Sensitivity when using D-pad or analog stick for a wheel.",
      NULL,
      "input",
      {
         {"125", NULL},
         {"250", NULL},
         {"375", NULL},
         {"500", NULL},
         {NULL, NULL},
      },
      "500"
   },
   {
      "dice_throttle_keyjoy_sensitivity",
      "Input > Throttle Sensitivity",
      "Throttle Sensitivity",
      "Sensitivity when using D-pad or analog stick for a throttle.",
      NULL,
      "input",
      {
         {"125", NULL},
         {"250", NULL},
         {"375", NULL},
         {"500", NULL},
         {NULL, NULL},
      },
      "250"
   },

   {
      "dice_retromouse_paddle0",
      "RetroMouse > Use Mouse For Paddle 1",
      "Use Mouse For Paddle 1",
      "Use a specific mouse for Paddle 1, chosen by Port 1's Mouse Index.",
      NULL,
      "retromouse",
      {
         {"disabled", NULL},
         {"enabled", NULL},
         {NULL, NULL},
      },
      "disabled"
   },
   {
      "dice_retromouse_paddle1",
      "RetroMouse > Use Mouse For Paddle 2",
      "Use Mouse For Paddle 2",
      "Use a specific mouse for Paddle 2, chosen by Port 2's Mouse Index.",
      NULL,
      "retromouse",
      {
         {"disabled", NULL},
         {"enabled", NULL},
         {NULL, NULL},
      },
      "disabled"
   },
   {
      "dice_retromouse_paddle2",
      "RetroMouse > Use Mouse For Paddle 3",
      "Use Mouse For Paddle 3",
      "Use a specific mouse for Paddle 3, chosen by Port 3's Mouse Index.",
      NULL,
      "retromouse",
      {
         {"disabled", NULL},
         {"enabled", NULL},
         {NULL, NULL},
      },
      "disabled"
   },
   {
      "dice_retromouse_paddle3",
      "RetroMouse > Use Mouse For Paddle 4",
      "Use Mouse For Paddle 4",
      "Use a specific mouse for Paddle 4, chosen by Port 4's Mouse Index.",
      NULL,
      "retromouse",
      {
         {"disabled", NULL},
         {"enabled", NULL},
         {NULL, NULL},
      },
      "disabled"
   },
   {
      "dice_retromouse_paddle0_x",
      "RetroMouse > Mouse Axis For Paddle 1 Horizontal",
      "Mouse Axis For Paddle 1 Horizontal",
      "Mouse axis for player 1, horizontal screen motion.",
      NULL,
      "retromouse",
      {
         {"x", NULL},
         {"y", NULL},
         {NULL, NULL},
      },
      "x"
   },
   {
      "dice_retromouse_paddle0_y",
      "RetroMouse > Mouse Axis For Paddle 1 Vertical",
      "Mouse Axis For Paddle 1 Vertical",
      "Mouse axis for player 1, vertical screen motion.",
      NULL,
      "retromouse",
      {
         {"x", NULL},
         {"y", NULL},
         {NULL, NULL},
      },
      "y"
   },
   {
      "dice_retromouse_paddle1_x",
      "RetroMouse > Mouse Axis For Paddle 2 Horizontal",
      "Mouse Axis For Paddle 2 Horizontal",
      "Mouse axis for player 2, horizontal screen motion.",
      NULL,
      "retromouse",
      {
         {"x", NULL},
         {"y", NULL},
         {NULL, NULL},
      },
      "x"
   },
   {
      "dice_retromouse_paddle1_y",
      "RetroMouse > Mouse Axis For Paddle 2 Vertical",
      "Mouse Axis For Paddle 2 Vertical",
      "Mouse axis for player 2, vertical screen motion.",
      NULL,
      "retromouse",
      {
         {"x", NULL},
         {"y", NULL},
         {NULL, NULL},
      },
      "y"
   },
   {
      "dice_retromouse_paddle2_x",
      "RetroMouse > Mouse Axis For Paddle 3 Horizontal",
      "Mouse Axis For Paddle 3 Horizontal",
      "Mouse axis for player 3, horizontal screen motion.",
      NULL,
      "retromouse",
      {
         {"x", NULL},
         {"y", NULL},
         {NULL, NULL},
      },
      "x"
   },
   {
      "dice_retromouse_paddle2_y",
      "RetroMouse > Mouse Axis For Paddle 3 Vertical",
      "Mouse Axis For Paddle 3 Vertical",
      "Mouse axis for player 3, vertical screen motion.",
      NULL,
      "retromouse",
      {
         {"x", NULL},
         {"y", NULL},
         {NULL, NULL},
      },
      "y"
   },
   {
      "dice_retromouse_paddle3_x",
      "RetroMouse > Mouse Axis For Paddle 4 Horizontal",
      "Mouse Axis For Paddle 4 Horizontal",
      "Mouse axis for player 4, horizontal screen motion.",
      NULL,
      "retromouse",
      {
         {"x", NULL},
         {"y", NULL},
         {NULL, NULL},
      },
      "x"
   },
   {
      "dice_retromouse_paddle3_y",
      "RetroMouse > Mouse Axis For Paddle 4 Vertical",
      "Mouse Axis For Paddle 4 Vertical",
      "Mouse axis for player 4, vertical screen motion.",
      NULL,
      "retromouse",
      {
         {"x", NULL},
         {"y", NULL},
         {NULL, NULL},
      },
      "y"
   },

#ifdef MANYMOUSE
   {
      "dice_manymouse_paddle0",
      "ManyMouse > Use Mouse For Paddle 1",
      "Use Mouse For Paddle 1",
      "Use a mouse for Paddle 1.",
      NULL,
      "manymouse",
      {
         {"disabled", NULL},
         {"enabled", NULL},
         {NULL, NULL},
      },
      "disabled"
   },
   {
      "dice_manymouse_paddle0_x",
      "ManyMouse > Mouse-Paddle 1 x",
      "Mouse-Paddle 1 x",
      "Mouse axis for player 1, horizontal screen motion.",
      NULL,
      "manymouse",
      {
         {"0x", NULL},
         {"0y", NULL},
         {"1x", NULL},
         {"1y", NULL},
         {"2x", NULL},
         {"2y", NULL},
         {"3x", NULL},
         {"3y", NULL},
         {NULL, NULL},
      },
      "0x"
   },
   {
      "dice_manymouse_paddle0_y",
      "ManyMouse > Mouse-Paddle 1 y",
      "Mouse-Paddle 1 y",
      "Mouse axis for player 1, vertical screen motion.",
      NULL,
      "manymouse",
      {
         {"0x", NULL},
         {"0y", NULL},
         {"1x", NULL},
         {"1y", NULL},
         {"2x", NULL},
         {"2y", NULL},
         {"3x", NULL},
         {"3y", NULL},
         {NULL, NULL},
      },
      "0y"
   },
   {
      "dice_manymouse_paddle1",
      "ManyMouse > Use Mouse For Paddle 2",
      "Use Mouse For Paddle 2",
      "Use a mouse for Paddle 2.",
      NULL,
      "manymouse",
      {
         {"disabled", NULL},
         {"enabled", NULL},
         {NULL, NULL},
      },
      "disabled"
   },
   {
      "dice_manymouse_paddle1_x",
      "ManyMouse > Mouse-Paddle 2 x",
      "Mouse-Paddle 2 x",
      "Mouse axis for player 2, horizontal screen motion.",
      NULL,
      "manymouse",
      {
         {"0x", NULL},
         {"0y", NULL},
         {"1x", NULL},
         {"1y", NULL},
         {"2x", NULL},
         {"2y", NULL},
         {"3x", NULL},
         {"3y", NULL},
         {NULL, NULL},
      },
      "1x"
   },
   {
      "dice_manymouse_paddle1_y",
      "ManyMouse > Mouse-Paddle 2 y",
      "Mouse-Paddle 2 y",
      "Mouse axis for player 2, vertical screen motion.",
      NULL,
      "manymouse",
      {
         {"0x", NULL},
         {"0y", NULL},
         {"1x", NULL},
         {"1y", NULL},
         {"2x", NULL},
         {"2y", NULL},
         {"3x", NULL},
         {"3y", NULL},
         {NULL, NULL},
      },
      "1y"
   },
   {
      "dice_manymouse_paddle2",
      "ManyMouse > Use Mouse For Paddle 3",
      "Use Mouse For Paddle 3",
      "Use a mouse for Paddle 3.",
      NULL,
      "manymouse",
      {
         {"disabled", NULL},
         {"enabled", NULL},
         {NULL, NULL},
      },
      "disabled"
   },
   {
      "dice_manymouse_paddle2_x",
      "ManyMouse > Mouse-Paddle 3 x",
      "Mouse-Paddle 3 x",
      "Mouse axis for player 3, horizontal screen motion.",
      NULL,
      "manymouse",
      {
         {"0x", NULL},
         {"0y", NULL},
         {"1x", NULL},
         {"1y", NULL},
         {"2x", NULL},
         {"2y", NULL},
         {"3x", NULL},
         {"3y", NULL},
         {NULL, NULL},
      },
      "2x"
   },
   {
      "dice_manymouse_paddle2_y",
      "ManyMouse > Mouse-Paddle 3 y",
      "Mouse-Paddle 3 y",
      "Mouse axis for player 3, vertical screen motion.",
      NULL,
      "manymouse",
      {
         {"0x", NULL},
         {"0y", NULL},
         {"1x", NULL},
         {"1y", NULL},
         {"2x", NULL},
         {"2y", NULL},
         {"3x", NULL},
         {"3y", NULL},
         {NULL, NULL},
      },
      "2y"
   },
   {
      "dice_manymouse_paddle3",
      "ManyMouse > Use Mouse For Paddle 4",
      "Use Mouse For Paddle 4",
      "Use a mouse for Paddle 4.",
      NULL,
      "manymouse",
      {
         {"disabled", NULL},
         {"enabled", NULL},
         {NULL, NULL},
      },
      "disabled"
   },
   {
      "dice_manymouse_paddle3_x",
      "ManyMouse > Mouse-Paddle 4 x",
      "Mouse-Paddle 4 x",
      "Mouse axis for player 4, horizontal screen motion.",
      NULL,
      "manymouse",
      {
         {"0x", NULL},
         {"0y", NULL},
         {"1x", NULL},
         {"1y", NULL},
         {"2x", NULL},
         {"2y", NULL},
         {"3x", NULL},
         {"3y", NULL},
         {NULL, NULL},
      },
      "3x"
   },
   {
      "dice_manymouse_paddle3_y",
      "ManyMouse > Mouse-Paddle 4 y",
      "Mouse-Paddle 4 y",
      "Mouse axis for player 4, vertical screen motion.",
      NULL,
      "manymouse",
      {
         {"0x", NULL},
         {"0y", NULL},
         {"1x", NULL},
         {"1y", NULL},
         {"2x", NULL},
         {"2y", NULL},
         {"3x", NULL},
         {"3y", NULL},
         {NULL, NULL},
      },
      "3y"
   },
#endif

   {
      "dice_dipswitch_1",
      "DIP Switch > DIP Switch 1",
      "DIP Switch 1",
      "Setting for DIP switch 1.",
      NULL,
      "dipswitch",
      {
         {"-1", "default"},
         {"0", NULL},
         {"1", NULL},
         {NULL, NULL},
      },
      "-1"
   },
   {
      "dice_dipswitch_2",
      "DIP Switch > DIP Switch 2",
      "DIP Switch 2",
      "Setting for DIP switch 2.",
      NULL,
      "dipswitch",
      {
         {"-1", "default"},
         {"0", NULL},
         {"1", NULL},
         {NULL, NULL},
      },
      "-1"
   },
   {
      "dice_dipswitch_3",
      "DIP Switch > DIP Switch 3",
      "DIP Switch 3",
      "Setting for DIP switch 3.",
      NULL,
      "dipswitch",
      {
         {"-1", "default"},
         {"0", NULL},
         {"1", NULL},
         {NULL, NULL},
      },
      "-1"
   },
   {
      "dice_dipswitch16_1",
      "DIP Switch > DIP Switch Hex 1",
      "DIP Switch Hex 1",
      "Setting for 16-position DIP switch number 1.",
      NULL,
      "dipswitch",
      {
         {"-1", "default"},
         {"0", NULL},
         {"1", NULL},
         {"2", NULL},
         {"3", NULL},
         {"4", NULL},
         {"5", NULL},
         {"6", NULL},
         {"7", NULL},
         {"8", NULL},
         {"9", NULL},
         {"10", NULL},
         {"11", NULL},
         {"12", NULL},
         {"13", NULL},
         {"14", NULL},
         {"15", NULL},
         {NULL, NULL},
      },
      "-1"
   },
   {
      "dice_dipswitch16_2",
      "DIP Switch > DIP Switch Hex 2",
      "DIP Switch Hex 2",
      "Setting for 16-position DIP switch number 2.",
      NULL,
      "dipswitch",
      {
         {"-1", "default"},
         {"0", NULL},
         {"1", NULL},
         {"2", NULL},
         {"3", NULL},
         {"4", NULL},
         {"5", NULL},
         {"6", NULL},
         {"7", NULL},
         {"8", NULL},
         {"9", NULL},
         {"10", NULL},
         {"11", NULL},
         {"12", NULL},
         {"13", NULL},
         {"14", NULL},
         {"15", NULL},
         {NULL, NULL},
      },
      "-1"
   },

   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};

struct retro_core_options_v2 options_us = {
  option_cats_us,
  option_defs_us
};

/*
 ********************************
 * Language Mapping
 ********************************
*/

#ifndef HAVE_NO_LANGEXTRA
struct retro_core_options_v2 *options_intl[RETRO_LANGUAGE_LAST] = {
   &options_us,    /* RETRO_LANGUAGE_ENGLISH */
   &options_ja,    /* RETRO_LANGUAGE_JAPANESE */
   &options_fr,    /* RETRO_LANGUAGE_FRENCH */
   &options_es,    /* RETRO_LANGUAGE_SPANISH */
   &options_de,    /* RETRO_LANGUAGE_GERMAN */
   &options_it,    /* RETRO_LANGUAGE_ITALIAN */
   &options_nl,    /* RETRO_LANGUAGE_DUTCH */
   &options_pt_br, /* RETRO_LANGUAGE_PORTUGUESE_BRAZIL */
   &options_pt_pt, /* RETRO_LANGUAGE_PORTUGUESE_PORTUGAL */
   &options_ru,    /* RETRO_LANGUAGE_RUSSIAN */
   &options_ko,    /* RETRO_LANGUAGE_KOREAN */
   &options_cht,   /* RETRO_LANGUAGE_CHINESE_TRADITIONAL */
   &options_chs,   /* RETRO_LANGUAGE_CHINESE_SIMPLIFIED */
   &options_eo,    /* RETRO_LANGUAGE_ESPERANTO */
   &options_pl,    /* RETRO_LANGUAGE_POLISH */
   &options_vn,    /* RETRO_LANGUAGE_VIETNAMESE */
   &options_ar,    /* RETRO_LANGUAGE_ARABIC */
   &options_el,    /* RETRO_LANGUAGE_GREEK */
   &options_tr,    /* RETRO_LANGUAGE_TURKISH */
   &options_sk,    /* RETRO_LANGUAGE_SLOVAK */
   &options_fa,    /* RETRO_LANGUAGE_PERSIAN */
   &options_he,    /* RETRO_LANGUAGE_HEBREW */
   &options_ast,   /* RETRO_LANGUAGE_ASTURIAN */
   &options_fi,    /* RETRO_LANGUAGE_FINNISH */
   &options_id,    /* RETRO_LANGUAGE_INDONESIAN */
   &options_sv,    /* RETRO_LANGUAGE_SWEDISH */
   &options_uk,    /* RETRO_LANGUAGE_UKRAINIAN */
   &options_cs,    /* RETRO_LANGUAGE_CZECH */
   &options_val,   /* RETRO_LANGUAGE_CATALAN_VALENCIA */
   &options_ca,    /* RETRO_LANGUAGE_CATALAN */
   &options_en,    /* RETRO_LANGUAGE_BRITISH_ENGLISH */
   &options_hu,    /* RETRO_LANGUAGE_HUNGARIAN */
};
#endif

/*
 ********************************
 * Functions
 ********************************
*/

/* Handles configuration/setting of core options.
 * Should be called as early as possible - ideally inside
 * retro_set_environment(), and no later than retro_load_game()
 * > We place the function body in the header to avoid the
 *   necessity of adding more .c files (i.e. want this to
 *   be as painless as possible for core devs)
 */

static INLINE void libretro_set_core_options(retro_environment_t environ_cb,
      bool *categories_supported)
{
   unsigned version  = 0;
#ifndef HAVE_NO_LANGEXTRA
   unsigned language = 0;
#endif

   if (!environ_cb || !categories_supported)
      return;

   *categories_supported = false;

   if (!environ_cb(RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION, &version))
      version = 0;

   if (version >= 2)
   {
#ifndef HAVE_NO_LANGEXTRA
      struct retro_core_options_v2_intl core_options_intl;

      core_options_intl.us    = &options_us;
      core_options_intl.local = NULL;

      if (environ_cb(RETRO_ENVIRONMENT_GET_LANGUAGE, &language) &&
          (language < RETRO_LANGUAGE_LAST) && (language != RETRO_LANGUAGE_ENGLISH))
         core_options_intl.local = options_intl[language];

      *categories_supported = environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_V2_INTL,
            &core_options_intl);
#else
      *categories_supported = environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_V2,
            &options_us);
#endif
   }
   else
   {
      size_t i, j;
      size_t option_index              = 0;
      size_t num_options               = 0;
      struct retro_core_option_definition
            *option_v1_defs_us         = NULL;
#ifndef HAVE_NO_LANGEXTRA
      size_t num_options_intl          = 0;
      struct retro_core_option_v2_definition
            *option_defs_intl          = NULL;
      struct retro_core_option_definition
            *option_v1_defs_intl       = NULL;
      struct retro_core_options_intl
            core_options_v1_intl;
#endif
      struct retro_variable *variables = NULL;
      char **values_buf                = NULL;

      /* Determine total number of options */
      while (true)
      {
         if (option_defs_us[num_options].key)
            num_options++;
         else
            break;
      }

      if (version >= 1)
      {
         /* Allocate US array */
         option_v1_defs_us = (struct retro_core_option_definition *)
               calloc(num_options + 1, sizeof(struct retro_core_option_definition));

         /* Copy parameters from option_defs_us array */
         for (i = 0; i < num_options; i++)
         {
            struct retro_core_option_v2_definition *option_def_us = &option_defs_us[i];
            struct retro_core_option_value *option_values         = option_def_us->values;
            struct retro_core_option_definition *option_v1_def_us = &option_v1_defs_us[i];
            struct retro_core_option_value *option_v1_values      = option_v1_def_us->values;

            option_v1_def_us->key           = option_def_us->key;
            option_v1_def_us->desc          = option_def_us->desc;
            option_v1_def_us->info          = option_def_us->info;
            option_v1_def_us->default_value = option_def_us->default_value;

            /* Values must be copied individually... */
            while (option_values->value)
            {
               option_v1_values->value = option_values->value;
               option_v1_values->label = option_values->label;

               option_values++;
               option_v1_values++;
            }
         }

#ifndef HAVE_NO_LANGEXTRA
         if (environ_cb(RETRO_ENVIRONMENT_GET_LANGUAGE, &language) &&
             (language < RETRO_LANGUAGE_LAST) && (language != RETRO_LANGUAGE_ENGLISH) &&
             options_intl[language])
            option_defs_intl = options_intl[language]->definitions;

         if (option_defs_intl)
         {
            /* Determine number of intl options */
            while (true)
            {
               if (option_defs_intl[num_options_intl].key)
                  num_options_intl++;
               else
                  break;
            }

            /* Allocate intl array */
            option_v1_defs_intl = (struct retro_core_option_definition *)
                  calloc(num_options_intl + 1, sizeof(struct retro_core_option_definition));

            /* Copy parameters from option_defs_intl array */
            for (i = 0; i < num_options_intl; i++)
            {
               struct retro_core_option_v2_definition *option_def_intl = &option_defs_intl[i];
               struct retro_core_option_value *option_values           = option_def_intl->values;
               struct retro_core_option_definition *option_v1_def_intl = &option_v1_defs_intl[i];
               struct retro_core_option_value *option_v1_values        = option_v1_def_intl->values;

               option_v1_def_intl->key           = option_def_intl->key;
               option_v1_def_intl->desc          = option_def_intl->desc;
               option_v1_def_intl->info          = option_def_intl->info;
               option_v1_def_intl->default_value = option_def_intl->default_value;

               /* Values must be copied individually... */
               while (option_values->value)
               {
                  option_v1_values->value = option_values->value;
                  option_v1_values->label = option_values->label;

                  option_values++;
                  option_v1_values++;
               }
            }
         }

         core_options_v1_intl.us    = option_v1_defs_us;
         core_options_v1_intl.local = option_v1_defs_intl;

         environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_INTL, &core_options_v1_intl);
#else
         environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS, option_v1_defs_us);
#endif
      }
      else
      {
         /* Allocate arrays */
         variables  = (struct retro_variable *)calloc(num_options + 1,
               sizeof(struct retro_variable));
         values_buf = (char **)calloc(num_options, sizeof(char *));

         if (!variables || !values_buf)
            goto error;

         /* Copy parameters from option_defs_us array */
         for (i = 0; i < num_options; i++)
         {
            const char *key                        = option_defs_us[i].key;
            const char *desc                       = option_defs_us[i].desc;
            const char *default_value              = option_defs_us[i].default_value;
            struct retro_core_option_value *values = option_defs_us[i].values;
            size_t buf_len                         = 3;
            size_t default_index                   = 0;

            values_buf[i] = NULL;

            if (desc)
            {
               size_t num_values = 0;

               /* Determine number of values */
               while (true)
               {
                  if (values[num_values].value)
                  {
                     /* Check if this is the default value */
                     if (default_value)
                        if (strcmp(values[num_values].value, default_value) == 0)
                           default_index = num_values;

                     buf_len += strlen(values[num_values].value);
                     num_values++;
                  }
                  else
                     break;
               }

               /* Build values string */
               if (num_values > 0)
               {
                  buf_len += num_values - 1;
                  buf_len += strlen(desc);

                  values_buf[i] = (char *)calloc(buf_len, sizeof(char));
                  if (!values_buf[i])
                     goto error;

                  strcpy(values_buf[i], desc);
                  strcat(values_buf[i], "; ");

                  /* Default value goes first */
                  strcat(values_buf[i], values[default_index].value);

                  /* Add remaining values */
                  for (j = 0; j < num_values; j++)
                  {
                     if (j != default_index)
                     {
                        strcat(values_buf[i], "|");
                        strcat(values_buf[i], values[j].value);
                     }
                  }
               }
            }

            variables[option_index].key   = key;
            variables[option_index].value = values_buf[i];
            option_index++;
         }

         /* Set variables */
         environ_cb(RETRO_ENVIRONMENT_SET_VARIABLES, variables);
      }

error:
      /* Clean up */

      if (option_v1_defs_us)
      {
         free(option_v1_defs_us);
         option_v1_defs_us = NULL;
      }

#ifndef HAVE_NO_LANGEXTRA
      if (option_v1_defs_intl)
      {
         free(option_v1_defs_intl);
         option_v1_defs_intl = NULL;
      }
#endif

      if (values_buf)
      {
         for (i = 0; i < num_options; i++)
         {
            if (values_buf[i])
            {
               free(values_buf[i]);
               values_buf[i] = NULL;
            }
         }

         free(values_buf);
         values_buf = NULL;
      }

      if (variables)
      {
         free(variables);
         variables = NULL;
      }
   }
}

#ifdef __cplusplus
}
#endif

#endif
