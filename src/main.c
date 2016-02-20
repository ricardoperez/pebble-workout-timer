#include <pebble.h>

#define TIMER_INTERVAL_MS 1000

Window *window;
TextLayer *text_layer;
int *counter;
int teipo = 30;
int repeat = 3;
int wait = 2; 

void update_timer_text(void){
  static char s_buffer[4];
  snprintf(s_buffer, sizeof(s_buffer), "%d", teipo);
	text_layer_set_text(text_layer, s_buffer);
}

void repeat_timer(void){
  
}

void wait_for(void){
  
}

void timer(void *context) {
  teipo--;  
  update_timer_text();
  if (teipo > 0 ){
    app_timer_register(1000, timer, NULL);
  }else{
    vibes_long_pulse();
  }
}

void handle_init(void) {
	// Create a window and text layer
	window = window_create();
	
  text_layer = text_layer_create(GRect(0, 30, 144, 154));
	update_timer_text();
	// Set the text, font, and text alignment
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  
  app_timer_register(1000, timer, NULL);
	// Add the text layer to the window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));

	// Push the window
	window_stack_push(window, true);
	
	// App Logging!
}

void handle_deinit(void) {
	// Destroy the text layer
	text_layer_destroy(text_layer);
	
	// Destroy the window
	window_destroy(window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
