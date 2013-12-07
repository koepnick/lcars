#include <pebble.h>

static Window *window;


// Text Layers
// Date
static TextLayer *text_date_layer;
// Time
static TextLayer *text_time_layer;
// AM/PM
static TextLayer *text_ampm_layer;

// Bitmaps
static BitmapLayer *background_layer;
static GBitmap *background_image;



void handle_minute_tick( struct tm* tick_time, TimeUnits units_changed )
{
	static char date_text[] = "Xxx 00.00";
	static char time_text[] = "00:00";
	static char	ampm_text[] = "  ";
	char *time_format;

	strftime( date_text, sizeof( date_text ), "%a %m.%d", tick_time );

	text_layer_set_text( text_date_layer, date_text );
	if( clock_is_24h_style() ) {
		time_format = "%R";
	}
	else
	{
		time_format = "%I:%M";
	}
	strftime( time_text, sizeof( time_text ), time_format, tick_time );
	text_layer_set_text( text_time_layer, time_text );
	if( !clock_is_24h_style() )
	{
		strftime( ampm_text, sizeof( ampm_text), "%p", tick_time );
APP_LOG(APP_LOG_LEVEL_DEBUG, ampm_text, window);
		if( ampm_text[0] == 'A' )
		{
			strncpy( ampm_text, "am", sizeof( "am" ) );
		} else {
			strncpy( ampm_text, "pm", sizeof( "pm" ) );
		}
	}

	text_layer_set_text( text_ampm_layer, ampm_text );

}



static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
	background_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
	background_layer = bitmap_layer_create(layer_get_frame(window_layer));
	bitmap_layer_set_bitmap(background_layer, background_image);
	layer_add_child(window_layer, bitmap_layer_get_layer(background_layer));


	text_date_layer = text_layer_create( GRect( 78, 5, 144-78, 168-5) );
	text_layer_set_text_color( text_date_layer, GColorWhite );
	text_layer_set_background_color( text_date_layer, GColorClear );
	text_layer_set_font( text_date_layer, 
		fonts_load_custom_font( resource_get_handle( RESOURCE_ID_FONT_LCARS_BOLD_17 ) ) );


	text_time_layer = text_layer_create( GRect( 40, 28, 144-40, 168-48) );
	text_layer_set_text_color( text_time_layer, GColorWhite );
	text_layer_set_background_color( text_time_layer, GColorClear );
	text_layer_set_font( text_time_layer, 
		fonts_load_custom_font( resource_get_handle( RESOURCE_ID_FONT_LCARS_60 ) ) );

	text_ampm_layer = text_layer_create( GRect( 40, 85, 144-40, 168-105) );
	text_layer_set_text_color( text_ampm_layer, GColorWhite );
	text_layer_set_background_color( text_ampm_layer, GColorClear );
	text_layer_set_font( text_ampm_layer, 
		fonts_load_custom_font( resource_get_handle( RESOURCE_ID_FONT_LCARS_60 ) ) );

	tick_timer_service_subscribe( SECOND_UNIT, handle_minute_tick );

	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_date_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_time_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_ampm_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_date_layer);
  text_layer_destroy(text_time_layer);
  text_layer_destroy(text_ampm_layer);
  bitmap_layer_destroy(background_layer);
  gbitmap_destroy(background_image);
}

static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  app_event_loop();
  deinit();
}
