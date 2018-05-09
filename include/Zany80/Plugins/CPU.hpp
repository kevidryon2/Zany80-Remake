#pragma once

#ifndef NEEDED_PLUGINS
#error Must define NEEDED_PLUGINS !
#endif

#define _num_to_type(x) uint ## x ## _t
#define num_to_type(x) _num_to_type(x)

#ifdef ADDRESS_BUS_SIZE
#define ADDRESS_BUS_SIZE_T num_to_type(ADDRESS_BUS_SIZE)
#else
#error ADDRESS_BUS_SIZE must be defined!
#endif

#ifdef DATA_BUS_SIZE
#define DATA_BUS_SIZE_T num_to_type(DATA_BUS_SIZE)
#else
#error DATA_BUS_SIZE must be defined!
#endif

#include <Zany80/Plugins.hpp>
#include <liblib/liblib.hpp>

typedef void(*write_t)(ADDRESS_BUS_SIZE_T,DATA_BUS_SIZE_T);
typedef DATA_BUS_SIZE_T(*read_t)(ADDRESS_BUS_SIZE_T);

extern "C" {
	PluginType *getType();
	HardwareType *getHardwareType();
	void postMessage(PluginMessage m);
	bool isSignatureCompatible(const char *sig);
	bool isCategory(const char *cat);
	bool isType(const char *type);
	void init(liblib::Library *plugin_manager);
	void cleanup();
	const char *neededPlugins();
	void emulate(uint64_t cycles);
	void cycle();
	uint64_t getCycles();
	uint8_t getAddressBusSize();
	uint8_t getDataBusSize();
	void setRAM(liblib::Library *RAM);
}

PluginType type = Hardware;
HardwareType hw_type = CPU;
extern const char *signature;

write_t writeRAM;
read_t readRAM;

PluginType *getType(){
	return &type;
}

HardwareType *getHardwareType() {
	return &hw_type;
}

#ifndef OVERRIDE_RAM

void setRAM(liblib::Library *RAM) {
	// Deliberately *don't* handle the exception - let the main program take care of it
	// Caches the functions instead of fetching them every time for performance reasons
	writeRAM = (write_t)(*RAM)["write"];
	readRAM = (read_t)(*RAM)["read"];
}

#endif

uint8_t getAddressBusSize() {
	return ADDRESS_BUS_SIZE;
}
uint8_t getDataBusSize() {
	return DATA_BUS_SIZE;
}

#ifndef OVERRIDE_SIG

#include <string.h>

bool isSignatureCompatible(const char *sig) {
	return strcmp(sig,signature) == 0;
}

#endif

#ifndef OVERRIDE_EMULATE

void emulate(uint64_t cycles) {
	for (uint64_t i = 0; i < cycles; i++) {
		cycle();
	}
}

#endif

const char *neededPlugins() {
	return NEEDED_PLUGINS;
}

#undef num_to_type
#undef _num_to_type