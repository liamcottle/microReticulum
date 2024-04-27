#pragma once

// file system types
#define FS_TYPE_NONE 0
#define FS_TYPE_SPIFFS 1
#define FS_TYPE_INTERNALFS 2

// configure fs type for heltec v3
#ifdef HELTEC_WIFI_LORA_32_V3
#define USE_FS_TYPE FS_TYPE_SPIFFS
#endif

// configure fs type for rak4631
#ifdef RAK4631
#define USE_FS_TYPE FS_TYPE_INTERNALFS
#endif

// set default fs type
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
