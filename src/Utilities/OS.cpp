#include "OS.h"

#include "../Log.h"
#include "../FileSystem/FileManager.h"

using namespace RNS;
using namespace RNS::Utilities;

/*static*/ void OS::setup() {
	RNS::FileSystem::FileManager::setup();
}

/*static*/ bool OS::file_exists(const char* file_path) {
	return RNS::FileSystem::FileManager::file_exists(file_path);
}

/*static*/ const Bytes OS::read_file(const char* file_path) {
	return RNS::FileSystem::FileManager::read_file(file_path);
}

/*static*/ bool OS::write_file(const Bytes& data, const char* file_path) {
	return RNS::FileSystem::FileManager::write_file(data, file_path);
}

/*static*/ bool OS::remove_file(const char* file_path) {
	return RNS::FileSystem::FileManager::remove_file(file_path);
}

/*static*/ bool OS::rename_file(const char* from_file_path, const char* to_file_path) {
	return RNS::FileSystem::FileManager::rename_file(from_file_path, to_file_path);
}

/*static*/ bool OS::create_directory(const char* directory_path) {
	return RNS::FileSystem::FileManager::create_directory(directory_path);
}
