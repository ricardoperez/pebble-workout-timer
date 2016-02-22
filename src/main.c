#include <pebble.h>

#define TIMER_INTERVAL_MS 1000

Window *window;
TextLayer *text_layer;
int *counter;

static void timer(void *context);

int original_tempo = 15;
int original_wait = 4;
int original_repeat = 5;
int tempo;
int wait; 
int repeat;

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
    repeat--; // DRY THIS
    vibes_double_pulse();
    tempo = original_tempo;
    wait = original_wait;
    update_timer_text();
    app_timer_register(1000, timer, NULL);
  }
}

void cabo(void){
  vibes_long_pulse();
  text_layer_set_text(text_layer, "DONE!!");
}

static void timer(void *context) {
  tempo--;  
  update_timer_text();
  if (tempo > 0 ){
    app_timer_register(1000, timer, NULL);
  }else{
    if (repeat > 0){ 
      vibes_short_pulse();
      wait_for(NULL);
    }else{ 
      cabo(); 
    }
  }
}

static void start_timer(){
  tempo = original_tempo;
  wait = original_wait;
  repeat = original_repeat;
  update_timer_text();
  app_timer_register(1000, timer, NULL);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  start_timer();
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler); 
}

void handle_init(void) {
  // Create a window and text layer
  window = window_create();
  text_layer = text_layer_create(GRect(0, 60, 144, 154));
  // Set the text, font, and text alignment
  text_layer_set_text(text_layer, "START --->");
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
