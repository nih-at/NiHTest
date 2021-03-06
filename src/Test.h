/*
  Test.h -- main class for a test case
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

#ifndef HAD_TEST_H
#define HAD_TEST_H

#include <string>
#include <regex>
#include <unordered_map>
#include <vector>

#include "Configuration.h"
#include "Parser.h"

class Test : ParserConsumer {
public:
    enum Result {
        PASSED = 0,
        FAILED = 1,
        SKIPPED = 77,
        ERROR = 99
    };
    
    struct File {
        std::string name;
        std::string input;
        std::string output;
        
        File(const std::string &name_, const std::string &input_, const std::string &output_) : name(name_), input(input_), output(output_) { }
        
        bool operator<(File other) const { return name < other.name; }
    };
    
    struct Replace {
        std::regex pattern;
        std::string replacement;
        
        Replace(const std::regex &pattern_, const std::string &replacement_) : pattern(pattern_), replacement(replacement_) { }
    };

    Test(const std::string &test_case, Configuration configuration_);
    
    Result run();

    std::string find_file(const std::string &name) const;
    virtual void process_directive(const Parser::Directive *directive, const std::vector<std::string> &args);

    Configuration configuration;
    std::string name;
    bool run_test;
    
    std::vector<std::string> arguments;
    std::unordered_map<std::string, int> directories;
    std::unordered_map<std::string, std::string> environment;
    std::vector<std::string> error_output;
    std::vector<Replace> error_output_replace;
    std::string exit_code;
    std::vector<File> files;
    std::vector<std::string> input;
    std::unordered_map<char, int> limits;
    std::vector<std::string> output;
    std::string input_file;
    std::vector<std::string> precheck_command;
    std::string preload_library;
    std::string program;
    std::vector<std::string> required_features;
    std::unordered_map<std::string, time_t> touch_files;
    
private:
    static const std::vector<Parser::Directive> directives;

    void compare_arrays(const std::vector<std::string> &expected, const std::vector<std::string> &got, const std::string &what);
    void compare_files();
    void enter_sandbox();
    Result execute_test();
    int get_int(const std::string &string);
    bool has_feature(const std::string &name);
    void leave_sandbox(bool keep);
    std::string make_filename(const std::string &directory, const std::string name) const;
    void print_result(Result result) const;
    void read_features();
    void rewrite_lines(const std::vector<Replace> &replacements, std::vector<std::string> *lines);
    
    bool in_sandbox;
    std::string sandbox_name;
    std::vector<std::string> failed;

    std::unordered_set<std::string> features;
    bool features_read;
};

#endif // HAD_TEST_H
