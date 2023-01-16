#include "sd.h"



SdFat sd;


bool create_log_dir(const String &dirname) {
    if (sd.exists(dirname))
        return true;

    return sd.mkdir(dirname);
}


String get_log_filename(const String &dirname, const String &date) {
    char buf[5];
    int idx = 0;
    while (true) {
        idx++;
        if (idx == 1000)
            return "";

        sprintf(buf, "%03d", idx);
        String filename = dirname + "/" + date + "-" + buf + ".log";
        if (!sd.exists(filename))
            return filename;
    }
}


bool create_log_file(SdFile &file, String &filename) {
    return file.open(filename.c_str(), O_WRITE | O_CREAT | O_TRUNC);
}
