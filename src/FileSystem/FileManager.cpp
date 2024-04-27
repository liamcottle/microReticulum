#include "FileManager.h"
#include "../Bytes.h"
#include "../Log.h"

using namespace RNS;
using namespace RNS::FileSystem;

#if USE_FS_TYPE == FS_TYPE_NONE

/*static*/ void FileManager::setup() {
	error("FileManager::setup() not implemented");
}

/*static*/ bool FileManager::file_exists(const char* file_path) {
	error("FileManager::file_exists() not implemented");
	return false;
}

/*static*/ const RNS::Bytes FileManager::read_file(const char* file_path) {
	error("FileManager::read_file() not implemented");
	throw "FileManager::read_file() not implemented";
}

/*static*/ bool FileManager::write_file(const Bytes& data, const char* file_path) {
    error("FileManager::write_file() not implemented");
    return false;
}

/*static*/ bool FileManager::remove_file(const char* file_path) {
	error("FileManager::remove_file() not implemented");
	return false;
}

/*static*/ bool FileManager::rename_file(const char* from_file_path, const char* to_file_path) {
	error("FileManager::rename_file() not implemented");
	return false;
}

/*static*/ bool FileManager::create_directory(const char* directory_path) {
	error("FileManager::create_directory() not implemented");
	return false;
}

#elif USE_FS_TYPE == FS_TYPE_SPIFFS

#include <Arduino.h>
#include <SPIFFS.h>

void listDir(const char* dir){
  Serial.print("DIR: ");
  Serial.println(dir);
  File root = SPIFFS.open(dir);
  File file = root.openNextFile();
  while(file){
	Serial.print(F("  FILE: "));
	Serial.print(file.name()); 
	Serial.print(F(" ("));
	Serial.print(file.size());
	Serial.print(F(" bytes)\n"));
	file = root.openNextFile();
  }
}

/*static*/ void FileManager::setup() {
	
	// init file system
	if(!SPIFFS.begin(true, "")){
		error("SPIFFS filesystem mount failed");
		return;
	}

	// log file system stats
	uint32_t size = SPIFFS.totalBytes() / (1024 * 1024);
	uint32_t used = SPIFFS.usedBytes() / (1024 * 1024);
	info("SPIFFS filesystem usage: " + std::to_string(used) + "MB / " + std::to_string(size) + "MB");
	listDir("/");

	// ensure filesystem is writable and format if not
	Bytes test;
	if(!FileManager::write_file(test, "/test")){
		info("SPIFFS filesystem is being formatted, please wait...");
		SPIFFS.format();
	}

	// remove test file if it exists
	FileManager::remove_file("/test");

	// file system should be ready
	debug("SPIFFS filesystem is ready");

}

/*static*/ bool FileManager::file_exists(const char* file_path) {

	// check if file exists
	File file = SPIFFS.open(file_path, FILE_READ);
	if(file){
		file.close();
		return true;
	}
	
	// file does not exist
	error("file_exists: failed to open file " + std::string(file_path));
	return false;

}

/*static*/ const RNS::Bytes FileManager::read_file(const char* file_path) {
	
	Bytes data;
	File file = SPIFFS.open(file_path, FILE_READ);
	if(file){
		size_t size = file.size();
		size_t read = file.readBytes((char*)data.writable(size), size);
		extreme("read_file: read " + std::to_string(read) + " bytes from file " + std::string(file_path));
		if(read != size){
			error("read_file: failed to read file " + std::string(file_path));
            data.clear();
		}
		file.close();
	} else {
		error("read_file: failed to open input file " + std::string(file_path));
	}

    return data;

}

/*static*/ bool FileManager::write_file(const Bytes& data, const char* file_path) {
    
	bool success = false;
	File file = SPIFFS.open(file_path, FILE_WRITE);
	if(file){
		size_t wrote = file.write(data.data(), data.size());
        extreme("write_file: wrote " + std::to_string(wrote) + " bytes to file " + std::string(file_path));
        if(wrote == data.size()){
            success = true;
        } else {
			error("write_file: failed to write file " + std::string(file_path));
		}
		file.close();
	} else {
		error("write_file: failed to open output file " + std::string(file_path));
	}

    return success;

}

/*static*/ bool FileManager::remove_file(const char* file_path) {
	return SPIFFS.remove(file_path);
}

/*static*/ bool FileManager::rename_file(const char* from_file_path, const char* to_file_path) {
	return SPIFFS.rename(from_file_path, to_file_path);
}

/*static*/ bool FileManager::create_directory(const char* directory_path) {
	if(SPIFFS.mkdir(directory_path)){
		return true;
	} else {
		error("create_directory: failed to create directorty " + std::string(directory_path));
		return false;
	}
}

#elif USE_FS_TYPE == FS_TYPE_INTERNALFS

#include "InternalFileSystem.h"
using namespace Adafruit_LittleFS_Namespace;

void listDir(const char* dir){
  Serial.print("DIR: ");
  Serial.println(dir);
  File root = InternalFS.open(dir);
  File file = root.openNextFile();
  while(file){
	Serial.print(F("  FILE: "));
	Serial.print(file.name()); 
	Serial.print(F(" ("));
	Serial.print(file.size());
	Serial.print(F(" bytes)\n"));
	file = root.openNextFile();
  }
}

/*static*/ void FileManager::setup() {
	
	// init file system (automatically reformats if flash fails to mount)
	if(!InternalFS.begin()){
		error("InternalFS filesystem mount failed");
		return;
	}

	// TODO: log file system stats
	listDir("/");

	// ensure filesystem is writable and format if not
	Bytes test;
	if(!FileManager::write_file(test, "/test")){
		info("InternalFS filesystem is being formatted, please wait...");
		InternalFS.format();
	}

	// remove test file if it exists
	FileManager::remove_file("/test");

	// file system should be ready
	debug("InternalFS filesystem is ready");

}

/*static*/ bool FileManager::file_exists(const char* file_path) {

	// check if file exists
	bool exists = InternalFS.exists(file_path);
	if(exists){
		return true;
	}
	
	// file does not exist
	error("file_exists: failed to open file " + std::string(file_path));
	return false;

}

/*static*/ const RNS::Bytes FileManager::read_file(const char* file_path) {

	Bytes data;
	File file = InternalFS.open(file_path, FILE_O_READ);
	if(file){
		size_t size = file.size();
		size_t read = file.readBytes((char*)data.writable(size), size);
		extreme("read_file: read " + std::to_string(read) + " bytes from file " + std::string(file_path));
		if(read != size){
			error("read_file: failed to read file " + std::string(file_path));
            data.clear();
		}
		file.close();
	} else {
		error("read_file: failed to open input file " + std::string(file_path));
	}

    return data;

}

/*static*/ bool FileManager::write_file(const Bytes& data, const char* file_path) {
    
	bool success = false;
	File file = InternalFS.open(file_path, FILE_O_WRITE);
	if(file){
		size_t wrote = file.write(data.data(), data.size());
        extreme("write_file: wrote " + std::to_string(wrote) + " bytes to file " + std::string(file_path));
        if(wrote == data.size()){
            success = true;
        } else {
			error("write_file: failed to write file " + std::string(file_path));
		}
		file.close();
	} else {
		error("write_file: failed to open output file " + std::string(file_path));
	}

    return success;

}

/*static*/ bool FileManager::remove_file(const char* file_path) {
	return InternalFS.remove(file_path);
}

/*static*/ bool FileManager::rename_file(const char* from_file_path, const char* to_file_path) {
	return InternalFS.rename(from_file_path, to_file_path);
}

/*static*/ bool FileManager::create_directory(const char* directory_path) {
	if(InternalFS.mkdir(directory_path)){
		return true;
	} else {
		error("create_directory: failed to create directorty " + std::string(directory_path));
		return false;
	}
}

#endif
