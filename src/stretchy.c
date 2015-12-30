#include <pebble.h>
#include <poses.h>

static Window *window;
static TextLayer *header_layer;
static TextLayer *title_layer;
static TextLayer *description_layer;
static TextLayer *footer_layer;

// Track the current pose
unsigned char current_pose = 255;
bool show_description = 0;
bool start = 0;
bool title_page = 1;

// Track time between poses. Defaults to update every 60 seconds.
static int last_change = 0;
static int interval = 60;
static char seconds_buffer[20];

static void refresh_layers() {
	if (!layer_get_hidden((Layer*)title_layer)) {
		layer_set_hidden((Layer*)title_layer, true);
	}
	
	if (layer_get_hidden((Layer*)header_layer)) {
		layer_set_hidden((Layer*)header_layer, false);
	}
	if (layer_get_hidden((Layer*)description_layer)) {
		layer_set_hidden((Layer*)description_layer, false);
	}
	
	text_layer_set_text(header_layer, poses[current_pose].fullname);
	text_layer_set_text(description_layer, poses[current_pose].description);
}

static void change_pose(bool increment) {
	if (increment) {
		// Change to the next pose.
		current_pose++;

		if (current_pose >= NUM_POSES) {
			current_pose = 0;
		}
	}
	else {
		 // Change to the previous pose.
		if (current_pose == 0) {
			current_pose = NUM_POSES - 1;
		}
		else {
			current_pose--;
		}
	}
	
	last_change = 0;
	
	refresh_layers();
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  // Start/stop the poses
	if (title_page) {
		title_page = 0;
		change_pose(true);
	}
	start = !start;
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
	if (!title_page) {
		change_pose(true);
	}
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
	if (!title_page) {
		change_pose(false);
	}
}

static void click_config_provider(void *context) {
	// Assigns handlers to button clicks
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

	// Title layer displays text in the very center of screen
	title_layer = text_layer_create((GRect) { .origin = { 0, 70 }, .size = { bounds.size.w, 22 } });
	text_layer_set_text(title_layer, "Press select to start.");
	text_layer_set_text_alignment(title_layer, GTextAlignmentCenter);
	text_layer_set_font(title_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
	
	// Header layer displays text in the top center of the screen
	header_layer = text_layer_create((GRect) { .origin = { 0, 0 }, .size = { bounds.size.w, 20 } });
	text_layer_set_text_alignment(header_layer, GTextAlignmentCenter);
	text_layer_set_font(header_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
	text_layer_set_text_color(header_layer, GColorWhite);
	text_layer_set_background_color(header_layer, GColorOxfordBlue);
	
	// Description layer displays a large block of text on the screen
	description_layer = text_layer_create((GRect) { .origin = { 0, 20 }, .size = { bounds.size.w, bounds.size.h - 20 } });
	text_layer_set_text_alignment(description_layer, GTextAlignmentCenter);
	text_layer_set_overflow_mode(description_layer, GTextOverflowModeWordWrap);
	
	// Footer layer displays information about the active mode
	footer_layer = text_layer_create((GRect) { .origin = { 0, bounds.size.h - 20 }, .size = { bounds.size.w, 20 } });
	text_layer_set_text_alignment(footer_layer, GTextAlignmentCenter);
	text_layer_set_text_color(footer_layer, GColorWhite);
	text_layer_set_background_color(footer_layer, GColorOxfordBlue);
	
	layer_add_child(window_layer, text_layer_get_layer(header_layer));
	layer_add_child(window_layer, text_layer_get_layer(description_layer));
	layer_add_child(window_layer, text_layer_get_layer(title_layer));
	layer_add_child(window_layer, text_layer_get_layer(footer_layer));
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	if (start) {
  // Increment pose
		last_change++;
		if (last_change >= interval) {
			// Change the pose and add a short vibration
			change_pose(true);
			vibes_short_pulse();
		}
		
		snprintf(seconds_buffer, sizeof(seconds_buffer), "%d seconds left", interval - last_change);
	}
	else {
		snprintf(seconds_buffer, sizeof(seconds_buffer), "Paused");
	}
	
	text_layer_set_text(footer_layer, seconds_buffer);
}

static void window_unload(Window *window) {
  text_layer_destroy(header_layer);
	text_layer_destroy(description_layer);
	text_layer_destroy(title_layer);
	text_layer_destroy(footer_layer);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
	.load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
	
	// Subscribe to TickTimerService
	tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}