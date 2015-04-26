/**
 * Our first Pebble Watchface!
 *
 * Written by ACM@UTEP
 * Saturday, March 28th, 2015
 * 
 * (Based on Pebble SDK Tutorial)
 **/
 
#include <pebble.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
	
	
/** Our main display for the user */
static Window *main_window;
/** The layer of text which contains the time */
static TextLayer *lpTextLayer;


static TextLayer *plus;
static TextLayer *minus;
static TextLayer *amount;

static int change = 0;
static int value = 0;
static int count = 0;


char * itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
		value = value + change;
	  char * buffer = "000000000000000";
	  buffer = itoa(value, buffer);
	  text_layer_set_text(lpTextLayer, buffer);
	  text_layer_set_text_color(lpTextLayer, GColorBlack);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {

	if(count == 0){
		change = 50;
	  text_layer_set_text(amount, "50");
	}
	else if(count == 1){
		change = 100;
	  text_layer_set_text(amount, "100");
	}
	else if(count == 2){
		change = 500;
	  text_layer_set_text(amount, "500");
	}
	else if(count == 3){
		change = 1000;
	  text_layer_set_text(amount, "1000");
	}
	count = (count+1) % 4;
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
		value = value - change;
	  char * buffer = "000000000000000";
	  buffer  = itoa(value, buffer);
	  text_layer_set_text(lpTextLayer, buffer);
	  text_layer_set_text_color(lpTextLayer, GColorBlack);

}

static void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}



/**
 * Main Window Load - When the window is loaded, what happens?
 */ 
static void main_window_load(Window *window) {
	change = 50;
	value = 8000;
  plus = text_layer_create(GRect(115,0,50,50));
  text_layer_set_text_color(plus, GColorBlack);
  text_layer_set_text(plus, "+");
  text_layer_set_font(plus, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));

	
	minus = text_layer_create(GRect(120,100,50,50));
  text_layer_set_text_color(minus, GColorBlack);
  text_layer_set_text(minus, "-");
  text_layer_set_font(minus, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
	
	amount = text_layer_create(GRect(0, 100, 120, 50));
  //text_layer_set_background_color(lpTextLayer, GColorBlack);
  text_layer_set_text_color(amount, GColorBlack);
  text_layer_set_text(amount, "50");
  text_layer_set_font(amount, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
	text_layer_set_text_alignment(lpTextLayer, GTextAlignmentLeft);
	
  lpTextLayer = text_layer_create(GRect(0, 50, 144, 50));
  //text_layer_set_background_color(lpTextLayer, GColorBlack);
  text_layer_set_text_color(lpTextLayer, GColorBlack);
  text_layer_set_text(lpTextLayer, "8000");
  text_layer_set_font(lpTextLayer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(lpTextLayer, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(lpTextLayer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(plus));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(minus));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(amount));
	
	window_set_click_config_provider(main_window, click_config_provider);
}
 

/**
 * Main Window Unload - When the window is unloaded, what happens?
 */ 
static void main_window_unload(Window *window) {}
 
/** 
 * Initialize - Where we set everything up!
 */
static void init() {
  // Creates the main window element
  main_window = window_create();
  
  // Set handler to manage the elements inside the window
  window_set_window_handlers(main_window, (WindowHandlers) {
  .load = main_window_load,
  .unload = main_window_unload
  });
  
  // Show the window
  window_stack_push(main_window, true);
}
 
/**
 * Deinitialize - Where we clean up after ourselves
 */
static void deinit() {
  text_layer_destroy(lpTextLayer); // You aren't needed anymore!
  window_destroy(main_window); // Bye-bye :(
}
 
/**
 * Main - The Pebble OS will call this first!
 */
int main(void) { 
  init(); // Setup the watchface!
  app_event_loop(); // Run the watchface!
  deinit(); // Clean up everything we used in the watchface!
  return 0; // Everything is okay!
}