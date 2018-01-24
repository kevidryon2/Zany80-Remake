#include <plugin_manager.hpp>
#include <iostream>
#include <Plugins.hpp>
#include <cstring>

extern std::string folder;

void init(liblib::Library *pm) {
	plugin_manager = pm;
	offloaded = new std::map <std::string,liblib::Library*>;
	plugins = new std::map <std::string,liblib::Library*>;
	std::cout << "[Plugin Manager] Path: "<<folder<<"plugins/\n";
	bool runnerFound = false;
	for (std::string s : *enumerate_plugins()) {
		std::cout << "[Plugin Manager] "<<"Loading \""<<s<<"\"...\n";
		liblib::Library * library;
		if (attemptLoad(s,&library)) {
				std::cout <<"[Plugin Manager] "<< s << " loaded successfully...\n";
				(*plugins)[s]=library;
		}
		else {
			try {
				offloaded->at(s);
				std::cerr << "[Plugin Manager] Missing prereqs for plugin: "<<s<<", offloading...\n";
			}
			catch (std::out_of_range) {
				std::cerr << "[Plugin Manager] Invalid plugin: "<<s<<"\n";
			}
		}
	}
	finishLoading();
}

std::vector<std::string> *enumerate_plugins() {
	//TODO: either use a file to track installed plugins or query all valid files in the folder recursively.
	if (plugin_paths == nullptr) {
		plugin_paths = new std::vector <std::string>;
		plugin_paths->push_back("plugins/rom_runner");
		plugin_paths->push_back("plugins/cpu/z80");
		plugin_paths->push_back("plugins/RAM16_8");
	}
	return plugin_paths;
}

void cleanup() {
	if (plugin_paths != nullptr) {
		plugin_paths->clear();
		delete plugin_paths;
		plugin_paths = nullptr;
	}
	if (plugins != nullptr) {
		int plength;
		do {
			plength = plugins->size();
			std::cout << "[Plugin Manager] "<<plength<<" plugins remaining for cleanup...\n";
			for (auto pair : *plugins) {
				try {
					if (unneeded(pair.second)) {
						std::cout << "[Plugin Manager] Cleaning up "<<pair.first<<"\n";
						try {
							(*pair.second)["cleanup"]();
						} catch(...){}
						delete pair.second;
						plugins->erase(pair.first);
						break;
					}
					else {
						std::cout << "[Plugin Manager] Plugin "<<pair.first<<" is still needed by other plugins; delaying cleanup...\n";
					}
				}
				catch (std::exception) {
					
				}
			}
		} while (plugins->size() != plength && plugins->size() > 0);
		delete plugins;
		plugins = nullptr;
	}
}

bool unneeded(liblib::Library *prereq) {
	for (auto pair : *plugins) {
		liblib::Library *library = pair.second;
		try {
			std::string needed = (const char *)(*library)["neededPlugins"]();
			if (needed == "") {
				continue;
			}
			needed += ';';
			while (needed.find(';') != std::string::npos) {
				std::string curr = needed.substr(0,needed.find(';'));
				needed = needed.substr(needed.find(';')+1,needed.length());
				if (curr.length() == 0 || curr.find('/') == std::string::npos)
					continue;
				std::string category = curr.substr(0,curr.find('/'));
				std::string id = curr.substr(curr.find('/')+1,curr.length());
				if (category == "CPU") {
					try {
						if (*(PluginType*)((*prereq)["getType"]()) == Hardware) {
							if (*(HardwareType*)((*prereq)["getHardwareType"]()) == CPU) {
								std::string signature = (const char *)(*prereq)["getSignature"]();
								if (signature == id) {
									// The prereq is needed by another plugin
									return false;
								}
							}
						}
					}
					catch (std::exception e){}
				}
				else if (category == "RAM") {
					try {
						if (*(PluginType*)((*prereq)["getType"]()) == Hardware) {
							if (*(HardwareType*)((*prereq)["getHardwareType"]()) == RAM) {
								std::string signature = (const char *)(*prereq)["getSignature"]();
								if (signature == id) {
									// The prereq is needed by another plugin
									return false;
								}
							}
						}
					}
					catch (std::exception e){}
				}
			}
		}
		catch (std::exception) {
			return false;
		}
	}
	return true;
}

bool prereqsLoaded(liblib::Library **library) {
	try {
		std::string needed = (const char *)(**library)["neededPlugins"]();
		if (needed == "") {
			std::cout << "[Plugin Manager] No prereqs\n";
			return true;
		}
		needed += ';';
		std::cout << "[Plugin Manager] Prereqs: "<<needed<<"\n";
		bool all_found = true;
		while (needed.find(';') != std::string::npos) {
			std::string curr = needed.substr(0,needed.find(';'));
			needed = needed.substr(needed.find(';')+1,needed.length());
			if (curr.length() == 0 || curr.find('/') == std::string::npos)
				continue;
			std::string category = curr.substr(0,curr.find('/'));
			std::string id = curr.substr(curr.find('/')+1,curr.length());
			bool cur_found = false;
			if (category == "CPU") {
				if (getCPUs() != nullptr) {
					// Check if any of the loaded CPUs match the signature specified
					for (liblib::Library *cpu : *CPUs) {
						try {
							std::string signature = (const char *)(*cpu)["getSignature"]();
							if (signature == id) {
								cur_found = true;
								break;
							}
						}
						// Invalid CPU plugin, will be destroyed later
						catch (std::exception) {}
					}
				}
			}
			else if (category == "RAM") {
				if (getHardware() != nullptr) {
					for (liblib::Library *ram : *hardware) {
						try {
							if (*((HardwareType*)((*ram)["getHardwareType"]())) == RAM) {
								std::string signature = (const char *)(*ram)["getSignature"]();
								if (signature == id) {
									cur_found = true;
									break;
								}
							}
						}
						catch (std::exception) {}
					}
				}
			}
			if (!cur_found) {
				all_found = false;
				break;
			}
		}
		return all_found;
	}
	catch (std::exception) {
		return false;
	}
}

bool attemptLoad(std::string name, liblib::Library **library) {
	try {
		*library = new liblib::Library(folder + name);
		// Before initializing it, verify that all needed plugins are loaded
		if (prereqsLoaded(library)) {
			((init_t)(**library)["init"])(plugin_manager);
			return true;
		}
		else {
			(*offloaded)[name]=(*library);
			return false;
		}
	}
	catch (std::exception &e) {
		return false;
	}
}

void gatherPlugins(PluginType type, std::vector<liblib::Library*> *vector) {
	vector->clear();
	for (auto pair : *plugins) {
		liblib::Library *library = pair.second;
		if (*((PluginType*)(*library)["getType"]()) == type) {
			vector->push_back(library);
		}
	}
}

void gatherPlugins(HardwareType type, std::vector<liblib::Library*> *vector) {
	vector->clear();
	for (liblib::Library *library : *hardware) {
		if (*((HardwareType*)(*library)["getHardwareType"]()) == type) {
			vector->push_back(library);
		}
	}
}

std::vector<liblib::Library*> *getRunners() {
	if (runners == nullptr) {
		runners = new std::vector <liblib::Library*>;
	}
	gatherPlugins(Runner,runners);
	if (!runners->empty()) {
		return runners;
	}
	return nullptr;
}

std::vector<liblib::Library*> *getHardware() {
	if (hardware == nullptr) {
		hardware = new std::vector <liblib::Library*>;
	}
	gatherPlugins(Hardware,hardware);
	if (!hardware->empty()) {
		return hardware;
	}
	return nullptr;
}

std::vector<liblib::Library*> *getCPUs() {
	if (CPUs == nullptr) {
		CPUs = new std::vector <liblib::Library*>;
	}
	getHardware();
	gatherPlugins(CPU,CPUs);
	if (!CPUs->empty()) {
		return CPUs;
	}
	return nullptr;
}

liblib::Library *getDefaultRunner() {
	getRunners();
	if (runners->empty()) {
		return nullptr;
	}
	for (liblib::Library *library : *runners) {
		if (*(RunnerType*)((*library)["getRunnerType"]()) == Shell) {
			return library;
		}
	}
	// No shell, but there *are* other runners. Just go with the first one.
	return (*runners)[0];
}

liblib::Library *getCPU(const char *signature) {
	if(getCPUs() != nullptr) {
		for (liblib::Library *cpu : *CPUs) {
			const char *sig = (const char *)((*cpu)["getSignature"]());
			if (strcmp(signature,sig) == 0) {
				return cpu;
			}
		}
	}
	return nullptr;
}

liblib::Library *getRAM(const char *signature) {
	if(getHardware() != nullptr) {
		for (liblib::Library *ram : *hardware) {
			const char *sig = (const char *)((*ram)["getSignature"]());
			if (strcmp(signature,sig) == 0) {
				return ram;
			}
		}
	}
	return nullptr;
}

void finishLoading() {
	if (offloaded == nullptr)
		return;
	int plength;
	do {
		plength = offloaded->size();
		std::cout << "[Plugin Manager] "<<plength<<" offloaded plugins remaining...\n";
		for (auto pair : *offloaded) {
			try {
				liblib::Library *library = pair.second;
				std::cout << "[Plugin Manager] Attempting late-load of " << pair.first << "\n";
				if (prereqsLoaded(&library)) {
					(*plugins)[pair.first] = library;
					((init_t)(*library)["init"])(plugin_manager);
					offloaded->erase(pair.first);
					std::cout << "[Plugin Manager] Late-load of " << pair.first << " successful!\n";
					break;
				}
				else {
					std::cerr << "[Plugin Manager] Late-load of " << pair.first << " unsuccessful.\n";
				}
			}
			catch (std::exception &e) {
				std::cerr << "[Plugin Manager] Late-load of " << pair.first << " unsuccessful.\n";
			}
		}
	}
	while (plength != offloaded->size() && !offloaded->empty());
	delete offloaded;
	offloaded = nullptr;
}

liblib::Library *plugin_manager;

std::vector<std::string> *plugin_paths;
std::map <std::string, liblib::Library*> *plugins;
std::map <std::string,liblib::Library*> * offloaded;
std::vector <liblib::Library*> *runners;
std::vector <liblib::Library*> *hardware;
std::vector <liblib::Library*> *CPUs;