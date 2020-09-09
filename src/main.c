#include <stdio.h>
#include <stdlib.h>

#include "sokol/sokol_app.h"
#include "sokol/sokol_gfx.h"
#include "sokol/util/sokol_imgui.h"
#include "sokol/sokol_time.h"
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui/cimgui.h"
#include "graphics.h"

#include "serial.h"
#include "license.h"

#define STR_(x) #x
#define STR(x) STR_(x)

static bool license_shown = false;
static window_t *license;
static bool in_pass = false;

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
		.mtl_device = sapp_metal_get_device(),
		.mtl_renderpass_descriptor_cb = sapp_metal_get_renderpass_descriptor,
		.mtl_drawable_cb = sapp_metal_get_drawable,
		.d3d11_device = sapp_d3d11_get_device(),
		.d3d11_device_context = sapp_d3d11_get_device_context(),
		.d3d11_render_target_view_cb = sapp_d3d11_get_render_target_view,
		.d3d11_depth_stencil_view_cb = sapp_d3d11_get_depth_stencil_view
	});
	simgui_setup(&(simgui_desc_t){.sample_count = 2});
	stm_setup();
	window_t *root = get_root();
	window_register(root);
	menu_t *menu = menu_create("Global");
	window_append_menu(root, menu);
	menu_append(menu, checkbox_create("Dock serial port", &serial_is_docked, &serial_toggle_root));
	menu_append(menu, checkbox_create("Show license", &license_shown, &toggle_license));
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
}

void frame(void) {
	int width = sapp_width();
	int height = sapp_height();
	const sg_pass_action action = (sg_pass_action) { 0 };
	sg_begin_default_pass(&action, width, height);
	in_pass = true;
        static uint64_t last_time;
	double delta = stm_sec(stm_laptime(&last_time));
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
		.fullscreen = true,
		.alpha = true,
	};
}
