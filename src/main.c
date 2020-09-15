#include <stdio.h>
#include <stdlib.h>

#include "sokol/sokol_app.h"
#include "sokol/sokol_gfx.h"
#include "sokol/sokol_glue.h"
#include "sokol/util/sokol_imgui.h"
#include "sokol/sokol_time.h"
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui/cimgui.h"

#include "serial.h"
#include "editor.h"
#include "z80.h"
#include "license.h"
#include "global.h"
#include "graphics.h"

#define STR_(x) #x
#define STR(x) STR_(x)

extern uint64_t executed;
static bool license_shown = false;
static window_t *license;
static bool in_pass = false;
menu_t *global_menu;

static void toggle_license() {
	if (license_shown) {
		window_register(license);
	}
	else {
		window_unregister(license);
	}
}

void init(void) {
	printf("Zany80 version " STR(PROJECT_VERSION) "\n");
	sg_setup(&(sg_desc){
		.context = sapp_sgcontext(),
	});
	simgui_setup(&(simgui_desc_t){.sample_count = 4});
	stm_setup();
	window_t *root = get_root();
	window_register(root);
	global_menu = menu_create("Global");
	window_append_menu(root, global_menu);
	menu_append(global_menu, checkbox_create("Dock serial port", &serial_is_docked, &serial_toggle_root));
	menu_append(global_menu, checkbox_create("Show license", &license_shown, &toggle_license));
	// Start the serial port docked in the root window.
	serial_init(true);
	const char msg[] = "Welcome to Zany80 version " STR(PROJECT_VERSION) "!\n"
		"\n"
		"This is the serial port's output. This message was generated by Zany80, and "
		"was not actually produced by an emulator.\n"
		"\n"
		"To clear this message, hit Clear in the Serial menu on the top. To pop the "
		"serial port into its own window, uncheck 'Dock serial port' in the Global "
		"menu.\n"
		"\n"
		"When the serial port has its own window, the clear button is present on the "
		"menu bar.\n"
		"\n"
		"To view the license, hit License in the Global menu.\n"
		"\n"
		;
	serial_write_all(msg, sizeof(msg) - 1);
	license = window_create("License");
	window_auto_size(license, true);
	window_append(license, label_set_wrapped(label_create(LICENSE), false));
	z80_init();
	editor_init();
}

void frame(void) {
	int width = sapp_width();
	int height = sapp_height();
	const sg_pass_action action = (sg_pass_action) { 0 };
	sg_begin_default_pass(&action, width, height);
	in_pass = true;
	static uint64_t last_time;
	double delta = stm_sec(stm_laptime(&last_time));
	static double total_time = 0;
	if (executed != 0) {
		total_time += delta;
		double HZ = executed / total_time;
		printf("\rMHz: %.04f", HZ / 1000000);
		fflush(stdout);
	}
	simgui_new_frame(width, height, delta);
	render_windows();
	simgui_render();
	sg_end_pass();
	in_pass = false;
	sg_commit();
}

void deinit(void) {
	if (in_pass) {
		sg_end_pass();
		in_pass = false;
		puts("Was in pass during deinit!");
	}
	editor_deinit();
	z80_deinit();
	window_clear(license, true);
	window_destroy(license);
	serial_deinit();
	window_t *root = get_root();
	window_clear(root, true);
	simgui_shutdown();
	sg_shutdown();
}

void event(const sapp_event *event) {
	simgui_handle_event(event);
}

sapp_desc sokol_main(int argc, char **argv) {
	(void)argv;
	(void)argc;
	return (sapp_desc) {
		.width = 640,
		.height = 480,
		.window_title = "Zany80 v" STR(PROJECT_VERSION),
		.init_cb = init,
		.frame_cb = frame,
		.cleanup_cb = deinit,
		.event_cb = event,
		.sample_count = 4,
		.gl_force_gles2 = false,
		.fullscreen = true,
		.alpha = true,
	};
}
