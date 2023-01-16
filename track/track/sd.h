#ifndef __sd_h__
#define __sd_h__

#include <SPI.h>
#include <SdFat.h>



extern SdFat sd;

String get_log_filename(const String &dir, const String &date);
bool create_log_dir(const String &dirname);
bool create_log_file(SdFile &file, String &filename);


#endif
