#pragma once

// file system types
#define FS_TYPE_NONE 0

// set default fs type if none configured
#ifndef USE_FS_TYPE
#define USE_FS_TYPE FS_TYPE_NONE
#endif

#include "../Bytes.h"

namespace RNS { namespace FileSystem {

	class FileManager {

	public:
		static void setup();
		static bool file_exists(const char* file_path);
		static const RNS::Bytes read_file(const char* file_path);
		static bool write_file(const RNS::Bytes& data, const char* file_path);
		static bool remove_file(const char* file_path);
		static bool rename_file(const char* from_file_path, const char* to_file_path);
		static bool create_directory(const char* directory_path);
    };

} }
