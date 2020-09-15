# AUTOGENERATED FILE; DO NOT MODIFY (use `build.py` instead)
CFLAGS+=-DPROJECT_VERSION=0.2.0-34 -DSOKOL_GLCORE33 -D_DEBUG -D_POSIX_C_SOURCE=200809L -Og -Wall -g -pedantic -std=c99
CXXFLAGS+=-DPROJECT_VERSION=0.2.0-34 -DSOKOL_GLCORE33 -D_DEBUG -D_POSIX_C_SOURCE=200809L -Og -Wall -g -pedantic -std=c++11
LDFLAGS+=-lX11 -lXi -lXcursor -lGL -ldl -lstdc++ -lm -pthread
INCLUDES+=-Ilib/ -Ilib/cimgui/

.PHONY: default all Zany80
default: Zany80
	./build/Zany80

all: sokol cimgui stb rapidxml TextEditor scas z80e Zany80

HEADERS=lib/TextEditor/TextEditor.h lib/cimgui/cimgui.h lib/cimgui/imconfig.h lib/cimgui/imgui.h lib/cimgui/imgui_internal.h lib/cimgui/imstb_rectpack.h lib/cimgui/imstb_textedit.h lib/cimgui/imstb_truetype.h lib/rapidxml/rapidxml.hpp lib/rapidxml/rapidxml_iterators.hpp lib/rapidxml/rapidxml_print.hpp lib/rapidxml/rapidxml_utils.hpp lib/scas/8xp.h lib/scas/assembler.h lib/scas/bin.h lib/scas/directives.h lib/scas/enums.h lib/scas/errors.h lib/scas/expression.h lib/scas/format.h lib/scas/functions.h lib/scas/hashtable.h lib/scas/instructions.h lib/scas/linker.h lib/scas/list.h lib/scas/log.h lib/scas/match.h lib/scas/md5.h lib/scas/merge.h lib/scas/objects.h lib/scas/operators.h lib/scas/privatize.h lib/scas/readline.h lib/scas/runtime.h lib/scas/stack.h lib/scas/stringop.h lib/sokol/sokol_app.h lib/sokol/sokol_args.h lib/sokol/sokol_audio.h lib/sokol/sokol_fetch.h lib/sokol/sokol_gfx.h lib/sokol/sokol_glue.h lib/sokol/sokol_time.h lib/sokol/util/sokol_debugtext.h lib/sokol/util/sokol_fontstash.h lib/sokol/util/sokol_gfx_imgui.h lib/sokol/util/sokol_gl.h lib/sokol/util/sokol_imgui.h lib/sokol/util/sokol_memtrack.h lib/stb/stb_ds.h lib/z80e/cpu.h lib/z80e/debugger/commands.h lib/z80e/debugger/debugger.h lib/z80e/debugger/hooks.h lib/z80e/debugger/keys.h lib/z80e/disassembler/disassemble.h lib/z80e/log/log.h lib/z80e/registers.h lib/z80e/runloop/runloop.h lib/z80e/ti/asic.h lib/z80e/ti/hardware/flash.h lib/z80e/ti/hardware/interrupts.h lib/z80e/ti/hardware/keyboard.h lib/z80e/ti/hardware/link.h lib/z80e/ti/hardware/memorymapping.h lib/z80e/ti/hardware/speed.h lib/z80e/ti/hardware/status.h lib/z80e/ti/hardware/t6a04.h lib/z80e/ti/hardware/timers.h lib/z80e/ti/memory.h lib/z80e/ti/ti.h
build/obj/%.o: %.c $(HEADERS)
	$(CC) $< $(CFLAGS) $(INCLUDES) -c -o $@
build/obj/%.o: %.cpp $(HEADERS)
	$(CXX) $< $(CXXFLAGS) $(INCLUDES) -c -o $@

### TARGET: TextEditor

TextEditor_OBJECTS = build/obj/lib/TextEditor/TextEditor.o

TextEditor_HEADERS = lib/TextEditor/TextEditor.h


### TARGET: cimgui

cimgui_OBJECTS = build/obj/lib/cimgui/cimgui.o build/obj/lib/cimgui/imgui.o build/obj/lib/cimgui/imgui_demo.o build/obj/lib/cimgui/imgui_draw.o build/obj/lib/cimgui/imgui_widgets.o

cimgui_HEADERS = lib/cimgui/cimgui.h lib/cimgui/imconfig.h lib/cimgui/imgui.h lib/cimgui/imgui_internal.h lib/cimgui/imstb_rectpack.h lib/cimgui/imstb_textedit.h lib/cimgui/imstb_truetype.h


### TARGET: rapidxml

rapidxml_OBJECTS = 

rapidxml_HEADERS = lib/rapidxml/rapidxml.hpp lib/rapidxml/rapidxml_iterators.hpp lib/rapidxml/rapidxml_print.hpp lib/rapidxml/rapidxml_utils.hpp


### TARGET: scas

scas_OBJECTS = build/obj/lib/scas/8xp.o build/obj/lib/scas/assembler.o build/obj/lib/scas/bin.o build/obj/lib/scas/directives.o build/obj/lib/scas/errors.o build/obj/lib/scas/expression.o build/obj/lib/scas/format.o build/obj/lib/scas/functions.o build/obj/lib/scas/hashtable.o build/obj/lib/scas/instructions.o build/obj/lib/scas/linker.o build/obj/lib/scas/list.o build/obj/lib/scas/log.o build/obj/lib/scas/match.o build/obj/lib/scas/md5.o build/obj/lib/scas/merge.o build/obj/lib/scas/objects.o build/obj/lib/scas/operators.o build/obj/lib/scas/privatize.o build/obj/lib/scas/readline.o build/obj/lib/scas/runtime.o build/obj/lib/scas/stack.o build/obj/lib/scas/stringop.o

scas_HEADERS = lib/scas/8xp.h lib/scas/assembler.h lib/scas/bin.h lib/scas/directives.h lib/scas/enums.h lib/scas/errors.h lib/scas/expression.h lib/scas/format.h lib/scas/functions.h lib/scas/hashtable.h lib/scas/instructions.h lib/scas/linker.h lib/scas/list.h lib/scas/log.h lib/scas/match.h lib/scas/md5.h lib/scas/merge.h lib/scas/objects.h lib/scas/operators.h lib/scas/privatize.h lib/scas/readline.h lib/scas/runtime.h lib/scas/stack.h lib/scas/stringop.h


### TARGET: sokol

sokol_OBJECTS = build/obj/lib/sokol/sokol.o build/obj/lib/sokol/sokol_cpp.o

sokol_HEADERS = lib/sokol/sokol_app.h lib/sokol/sokol_args.h lib/sokol/sokol_audio.h lib/sokol/sokol_fetch.h lib/sokol/sokol_gfx.h lib/sokol/sokol_glue.h lib/sokol/sokol_time.h lib/sokol/util/sokol_debugtext.h lib/sokol/util/sokol_fontstash.h lib/sokol/util/sokol_gfx_imgui.h lib/sokol/util/sokol_gl.h lib/sokol/util/sokol_imgui.h lib/sokol/util/sokol_memtrack.h


### TARGET: stb

stb_OBJECTS = build/obj/lib/stb/stb_ds.o

stb_HEADERS = lib/stb/stb_ds.h


### TARGET: z80e

z80e_OBJECTS = build/obj/lib/z80e/cpu.o build/obj/lib/z80e/debugger/commands/commands.o build/obj/lib/z80e/debugger/commands/disassemble.o build/obj/lib/z80e/debugger/commands/dump_lcd.o build/obj/lib/z80e/debugger/commands/hexdump.o build/obj/lib/z80e/debugger/commands/link.o build/obj/lib/z80e/debugger/commands/load_register.o build/obj/lib/z80e/debugger/commands/on.o build/obj/lib/z80e/debugger/commands/ports.o build/obj/lib/z80e/debugger/commands/press_key.o build/obj/lib/z80e/debugger/commands/print_expression.o build/obj/lib/z80e/debugger/commands/print_mappings.o build/obj/lib/z80e/debugger/commands/print_registers.o build/obj/lib/z80e/debugger/commands/release_key.o build/obj/lib/z80e/debugger/commands/run.o build/obj/lib/z80e/debugger/commands/stack.o build/obj/lib/z80e/debugger/commands/stop.o build/obj/lib/z80e/debugger/commands/tap_key.o build/obj/lib/z80e/debugger/commands/timers.o build/obj/lib/z80e/debugger/commands/turn_on.o build/obj/lib/z80e/debugger/commands/unhalt.o build/obj/lib/z80e/debugger/debugger.o build/obj/lib/z80e/debugger/hooks.o build/obj/lib/z80e/disassembler/disassemble.o build/obj/lib/z80e/log/log.o build/obj/lib/z80e/registers.o build/obj/lib/z80e/runloop/runloop.o build/obj/lib/z80e/ti/asic.o build/obj/lib/z80e/ti/hardware/flash.o build/obj/lib/z80e/ti/hardware/interrupts.o build/obj/lib/z80e/ti/hardware/keyboard.o build/obj/lib/z80e/ti/hardware/link.o build/obj/lib/z80e/ti/hardware/memorymapping.o build/obj/lib/z80e/ti/hardware/speed.o build/obj/lib/z80e/ti/hardware/status.o build/obj/lib/z80e/ti/hardware/t6a04.o build/obj/lib/z80e/ti/hardware/timers.o build/obj/lib/z80e/ti/memory.o

z80e_HEADERS = lib/z80e/cpu.h lib/z80e/debugger/commands.h lib/z80e/debugger/debugger.h lib/z80e/debugger/hooks.h lib/z80e/debugger/keys.h lib/z80e/disassembler/disassemble.h lib/z80e/log/log.h lib/z80e/registers.h lib/z80e/runloop/runloop.h lib/z80e/ti/asic.h lib/z80e/ti/hardware/flash.h lib/z80e/ti/hardware/interrupts.h lib/z80e/ti/hardware/keyboard.h lib/z80e/ti/hardware/link.h lib/z80e/ti/hardware/memorymapping.h lib/z80e/ti/hardware/speed.h lib/z80e/ti/hardware/status.h lib/z80e/ti/hardware/t6a04.h lib/z80e/ti/hardware/timers.h lib/z80e/ti/memory.h lib/z80e/ti/ti.h


### TARGET: Zany80

Zany80: build/Zany80

Zany80_OBJECTS = build/obj/src/XML.o build/obj/src/graphics.o build/obj/src/graphics_legacy.o build/obj/src/license.o build/obj/src/main.o build/obj/src/ring_buffer.o build/obj/src/serial.o build/obj/src/z80.o build/obj/src/zexall.o

Zany80_HEADERS = 

build/Zany80: $(Zany80_OBJECTS)  $(TextEditor_OBJECTS) $(cimgui_OBJECTS) $(rapidxml_OBJECTS) $(scas_OBJECTS) $(sokol_OBJECTS) $(stb_OBJECTS) $(z80e_OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
