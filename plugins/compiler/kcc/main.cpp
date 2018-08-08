#define NEEDED_PLUGINS ""
#include <Zany80/Plugins/Generic.hpp>
#include <Zany80/Zany80.hpp>

#include <cstring>
#include <fstream>
#include <iostream>

liblib::Library *plugin_manager;

extern "C" {

	bool isCategory(const char *cat) {
		return (!strcmp(cat, "CCompiler")) || (!strcmp(cat, "Development"));
	}

	bool isType(const char *type) {
		return (!strcmp(type, "z80"));
	}
	
}

void messageShell(const char *message) {
	((message_t)(*plugin_manager)["message"])({
		0, "history", (int)strlen("history"), "CCompiler/z80", message
	}, "Runner/Shell");
}

void postMessage(PluginMessage m) {
	if (!strcmp(m.data,"init")) {
		plugin_manager = (liblib::Library*)m.context;
	}
	else if (!strcmp(m.data,"cleanup")) {
		
	}
	else if (!strcmp(m.data, "invoke")) {
		std::string command = "\"" + true_folder + "/plugins/binaries/kcc\" ";
		for (std::string s : *((std::vector<std::string>*)m.context)) {
			command += s + " ";
		}
		command += "2>kcc_error.log";
		std::cout << '\n' << command << '\n';
		if (system(command.c_str()) == 0) {
			messageShell("Compiled successfully!");
		}
		else {
			std::ifstream error("kcc_error.log");
			if (error.is_open()) {
				std::string buf;
				while (std::getline(error, buf)) {
					messageShell(buf.c_str());
				}
				error.close();
			}
			else {
				messageShell("Error!");
			}
		}
		remove("kcc_error.log");
	}
}
