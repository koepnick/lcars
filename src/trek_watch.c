#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define MY_UUID { 0x3F, 0x88, 0xA0, 0xD5, 0x83, 0x1E, 0x4F, 0x15, 0x87, 0x42, 0x13, 0xBC, 0x42, 0x53, 0x69, 0x0C }
PBL_APP_INFO(MY_UUID,
             "Trekkie", "Zach Bruggeman",
             1, 0, /* App version */
             RESOURCE_ID_IMAGE_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;
BmpContainer background_image;

TextLayer text_date_layer;
TextLayer text_stardate_label_layer;
TextLayer text_stardate_disp_layer;
TextLayer text_time_layer;
TextLayer text_ampm_layer;

void update_display(PblTm *tick_time) {
  static char date_text[] = "Xxx 00.00";
  static char new_date_text[] = "Xxx 00.00";
  static char time_text[] = "00:00";
  static char ampm_text[] = "xm";
  static char stardate_label_text[] = "stardate";
  static char stardate_disp_text[] = "0000.000";
  char *time_format;

  // Draw the stardate label
  text_layer_set_text(&text_stardate_label_layer, stardate_label_text);

  // Figure and draw stardate
  string_format_time(stardate_disp_text, sizeof(stardate_disp_text),
                     "%Y.%j", tick_time);
  text_layer_set_text(&text_stardate_disp_layer, stardate_disp_text);
  
  // Draws only if changed, saves your precious battery!
  // Get new_date_text
  string_format_time(new_date_text, sizeof(date_text), "%a %m.%d", tick_time);
  new_date_text[0] += 32; // lowercase the weekday
  // Replace leading '0' in month and day with ' ' 
  if (new_date_text[4] == '0') new_date_text[4] = ' ';
  if (new_date_text[7] == '0') new_date_text[7] = ' ';
  // Set text layer if changed
  if (strncmp(new_date_text, date_text, sizeof(date_text)) != 0) {
      strncpy(date_text, new_date_text, sizeof(date_text));
      text_layer_set_text(&text_date_layer, date_text);
  }

  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%I:%M";
  }

  string_format_time(time_text, sizeof(time_text), time_format, tick_time);
  // Kludge to handle lack of non-padded hour format string
  // for twelve hour clock.
  if (!clock_is_24h_style() && (time_text[0] == '0')) {
    memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }
  text_layer_set_text(&text_time_layer, time_text);

  // Write the AM or PM or nothing for 24h clock
  if (!clock_is_24h_style()) {
    string_format_time(ampm_text, sizeof(ampm_text), "%p", tick_time);
    if (ampm_text[0] == 'A') {
      strncpy(ampm_text, "am", sizeof(ampm_text));
    } else {
      strncpy(ampm_text, "pm", sizeof(ampm_text));
    }
  } else {
    strncpy(ampm_text, "  ", sizeof(ampm_text));
  }
  text_layer_set_text(&text_ampm_layer, ampm_text);  
}


void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)ctx;

  update_display(t->tick_time);
}

void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Trekkie");
  window_stack_push(&window, true /* Animated */);

  resource_init_current_app(&APP_RESOURCES);

  bmp_init_container(RESOURCE_ID_IMAGE_BACKGROUND, &background_image);
  layer_add_child(&window.layer, &background_image.layer.layer);

  // Date Layer
  text_layer_init(&text_date_layer, window.layer.frame);
  text_layer_set_text_color(&text_date_layer, GColorWhite);
  text_layer_set_background_color(&text_date_layer, GColorClear);
  layer_set_frame(&text_date_layer.layer, GRect(77, 5, 60, 20));
  text_layer_set_font(&text_date_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LCARS_BOLD_17)));
  layer_add_child(&window.layer, &text_date_layer.layer);

  // Stardate Label Layer
  text_layer_init(&text_stardate_label_layer, window.layer.frame);
  text_layer_set_text_color(&text_stardate_label_layer, GColorWhite);
  text_layer_set_background_color(&text_stardate_label_layer, GColorClear);
  layer_set_frame(&text_stardate_label_layer.layer,
                  GRect(35, 26, 144-40, 168-15));
  text_layer_set_font(&text_stardate_label_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LCARS_BOLD_17)));
  layer_add_child(&window.layer, &text_stardate_label_layer.layer);

  // Stardate Display Layer
  text_layer_init(&text_stardate_disp_layer, window.layer.frame);
  text_layer_set_text_color(&text_stardate_disp_layer, GColorWhite);
  text_layer_set_background_color(&text_stardate_disp_layer, GColorClear);
  layer_set_frame(&text_stardate_disp_layer.layer,
                  GRect(35, 36, 144-40, 168-48));
  text_layer_set_font(&text_stardate_disp_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LCARS_40_NUMBERS)));
  layer_add_child(&window.layer, &text_stardate_disp_layer.layer);

  // Time Layer
  text_layer_init(&text_time_layer, window.layer.frame);
  text_layer_set_text_color(&text_time_layer, GColorWhite);
  text_layer_set_background_color(&text_time_layer, GColorClear);
  layer_set_frame(&text_time_layer.layer, GRect(40, 48+25, 144-40, 168-48));
  text_layer_set_font(&text_time_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LCARS_60_NUMBERS)));
  layer_add_child(&window.layer, &text_time_layer.layer);

  // AMPM Layer
  text_layer_init(&text_ampm_layer, window.layer.frame);
  text_layer_set_text_color(&text_ampm_layer, GColorWhite);
  text_layer_set_background_color(&text_ampm_layer, GColorClear);
  layer_set_frame(&text_ampm_layer.layer, GRect(40, 105+25, 144-40, 168-105));
  text_layer_set_font(&text_ampm_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LCARS_BOLD_17)));
  layer_add_child(&window.layer, &text_ampm_layer.layer);

  // No blank screen on init.
  PblTm tick_time;
  get_time(&tick_time);
  update_display(&tick_time);
}

void handle_deinit(AppContextRef ctx) {
  (void)ctx;

  bmp_deinit_container(&background_image);
}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .deinit_handler = &handle_deinit,

    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = MINUTE_UNIT
    }
  };
  app_event_loop(params, &handlers);
}
