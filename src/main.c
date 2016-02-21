#include <pebble.h>

#define TIMER_INTERVAL_MS 1000

Window *window;
TextLayer *text_layer;
int *counter;

static void timer(void *context);

int tempo = 15;
int repeat = 2;
int wait = 3; 
int original_tempo;
int original_wait;

void update_timer_text(void){
  static char s_buffer[4];
  snprintf(s_buffer, sizeof(s_buffer), "%d", tempo);
	text_layer_set_text(text_layer, s_buffer);
}

void wait_for(void *context){
  wait--;
	text_layer_set_text(text_layer, "WAIT MATE");
  if (wait > 0){
    app_timer_register(1000, wait_for, NULL);
  }else{
    repeat--;
    tempo = original_tempo;
    wait = original_wait;
    vibes_short_pulse();
    app_timer_register(1000, timer, NULL);
  }
}

void cabo(void){
  vibes_double_pulse();
  text_layer_set_text(text_layer, "DONE!!");
}

static void timer(void *context) {
  tempo--;  
  update_timer_text();
  if (tempo > 0 ){
    app_timer_register(1000, timer, NULL);
  }else{
    vibes_long_pulse();
    (repeat > 0) ? wait_for(NULL) : cabo();
  }
}

static void start_timer(){
  app_timer_register(1000, timer, NULL);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  start_timer();
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler); 
}

void handle_init(void) {
  original_tempo = tempo;
  original_wait = wait;
	// Create a window and text layer
	window = window_create();
  text_layer = text_layer_create(GRect(0, 30, 144, 154));
	update_timer_text();
	// Set the text, font, and text alignment
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);

	// Add the text layer to the window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
	// Push the window
  window_set_click_config_provider(window, click_config_provider);
	window_stack_push(window, true);
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
