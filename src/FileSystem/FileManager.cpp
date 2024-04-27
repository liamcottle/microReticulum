#include "FileManager.h"
#include "../Bytes.h"
#include "../Log.h"

#if USE_FS_TYPE == FS_TYPE_NONE

/*static*/ void RNS::FileSystem::FileManager::setup() {
	error("FileManager::setup() not implemented");
}

/*static*/ bool RNS::FileSystem::FileManager::file_exists(const char* file_path) {
	error("FileManager::file_exists() not implemented");
}

/*static*/ const RNS::Bytes RNS::FileSystem::FileManager::read_file(const char* file_path) {
	error("FileManager::read_file() not implemented");
    Bytes data;
    return data;
}

/*static*/ bool RNS::FileSystem::FileManager::write_file(const Bytes& data, const char* file_path) {
    error("FileManager::write_file() not implemented");
    return false;
}

/*static*/ bool RNS::FileSystem::FileManager::remove_file(const char* file_path) {
	error("FileManager::remove_file() not implemented");
	return false;
}

/*static*/ bool RNS::FileSystem::FileManager::rename_file(const char* from_file_path, const char* to_file_path) {
	error("FileManager::rename_file() not implemented");
	return false;
}

/*static*/ bool RNS::FileSystem::FileManager::create_directory(const char* directory_path) {
	error("FileManager::create_directory() not implemented");
	return false;
}

#endif
