#include "file_io.h"
#include <fstream>
#include <iostream>
#include <sys/stat.h>

#ifdef _WIN32
    #include <io.h>
    #include <fcntl.h>
#endif

// ... остальной код без изменений
