#ifndef FORMATTER_H
#define FORMATTER_H

#include <string>
#include <vector>

class Formatter {
public:
    static std::string format_text(const std::string& text);
    
private:
    static std::vector<std::string> split_string(const std::string& str, char delim);
    static std::string trim(const std::string& str);
    static std::string to_lower(const std::string& str);
    static std::string process_link(const std::string& text);
    static std::vector<std::string> parse_formats(const std::string& format_part);
    static std::vector<std::string> get_valid_formats(const std::vector<std::string>& format_list);
    static std::string build_tags(const std::vector<std::string>& valid_formats, const std::string& content);
};

#endif
