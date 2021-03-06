/*
  OS-Unix.h -- Unix specific functions
  Copyright (C) 2020 Dieter Baron and Thomas Klausner

  This file is part of nihtest, regression tests for command line utilities.
  The authors can be contacted at <nihtest@nih.at>

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
  3. The names of the authors may not be used to endorse or promote
     products derived from this software without specific prior
     written permission.

  THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS
  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
  IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
  IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "OS.h"

#include <sys/stat.h>
#include <sys/utsname.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>

#include "Exception.h"

const std::string OS::path_separator = "/";

const std::unordered_map<std::string, std::string> OS::standard_environment = {
    { "LC_CTYPE", "C"},
    { "POSIXLY_CORRECT", "1" },
    { "TZ", "UTC" }
};


std::string OS::append_path_component(const std::string &directory, const std::string &name) {
    if (directory.empty() || directory == ".") {
        return name;
    }
    else {
        return directory + path_separator + name;
    }
}


void OS::change_directory(const std::string &directory) {
    if (chdir(directory.c_str()) < 0) {
	throw Exception("can't change into directory '" + directory + "'", true);
    }
}


void OS::create_directory(const std::string &directory) {
    if (mkdir(directory.c_str(), 0777) < 0) {
        throw Exception("can't create directory '" + directory + "'", true);
    }
}


bool OS::file_exists(const std::string &file_name) {
    struct stat st;
    
    if (stat(file_name.c_str(), &st) < 0) {
        return false;
    }
    
    return S_ISREG(st.st_mode);
}


bool OS::directory_exists(const std::string &name) {
    struct stat st;
    
    if (stat(name.c_str(), &st) < 0) {
        return false;
    }
    
    return S_ISDIR(st.st_mode);
}


std::string OS::get_error_string() {
    return strerror(errno);
}


bool OS::is_absolute(const std::string &file_name) {
    if (file_name.empty()) {
        return false;
    }
    return file_name[0] == '/';
}


static bool is_directory(const std::string &directory) {
    struct stat st;
    
    if (stat(directory.c_str(), &st) < 0) {
        throw Exception("can't stat '" + directory + "'", true);
    }
    return S_ISDIR(st.st_mode);
}


static void list_files_recurse(const std::string &directory, std::vector<std::string> *all_files) {
    DIR *dir = opendir(directory.c_str());
    if (dir == NULL) {
        throw Exception("can't list directory '" + directory + "'", true);
    }
    
    std::vector<std::string> files;
    
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        files.push_back(entry->d_name);
    }
    closedir(dir);
    
    std::sort(files.begin(), files.end());
    
    for (const auto &file : files) {
        auto name = OS::append_path_component(directory, file);
        if (is_directory(name)) {
            list_files_recurse(name, all_files);
        }
        else {
            all_files->push_back(name);
        }
    }
}


std::vector<std::string> OS::list_files(const std::string &directory) {
    std::vector<std::string> files;

    list_files_recurse(directory, &files);
    
    return files;
}


std::string OS::make_temp_directory(const std::string &directory, const std::string &name) {
    auto directory_template = append_path_component(directory, name + ".XXXXXXXX");

    char *temp_directory = strdup(directory_template.c_str());
    if (temp_directory == NULL) {
	throw Exception("out of memory");
    }
    if (mkdtemp(temp_directory) == NULL) {
	throw Exception("can't create temporary directory '" + name + "'", true);
    }
    return temp_directory;
}

void OS::remove_directory(const std::string &directory) {
    // TODO: proper implementation
    auto command = "rm -r " + directory;
    system(command.c_str());
}

std::string OS::operating_system() {
    struct utsname name;
    
    if (uname(&name) < 0) {
        throw Exception("can't get system information", true);
    }
    
    return name.sysname;
}
