/*
  CompareFiles.h -- compare files
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

#ifndef HAD_COMPARE_FILES_H
#define HAD_COMPARE_FILES_H

#include <string>
#include <vector>

#include "Configuration.h"
#include "Test.h"

class CompareFiles {
public:
    CompareFiles(const std::vector<Test::File> &expected_, const std::vector<std::string> &got_, Test *test_, bool verbose_) : expected(expected_), got(got_), test(test_), comparators(&test_->configuration.file_compare), verbose(verbose_), ok(true) { }

    bool compare();
    
private:
    void compare_files(const std::vector<std::string> &argv, const std::string &got, const std::string &expected);
    void print_header();
    void print_line(char indicator, const std::string &line);
    
    const std::vector<Test::File> &expected;
    const std::vector<std::string> &got;
    FileComparators *comparators;
    Test *test;
    bool verbose;
    
    bool ok;
};

#endif // HAD_COMPARE_FILES_H
