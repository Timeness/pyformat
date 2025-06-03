#include "formatter.h"
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <cctype>

std::string Formatter::format_text(const std::string& text) {
    if (text.empty()) return text;
    
    if (text.find('|') != std::string::npos && 
        (text.substr(0, 2) == "l|" || text.substr(0, 5) == "link|" || text.substr(0, 2) == "a|")) {
        return process_link(text);
    }
    
    std::string format_part, content;
    size_t pipe_pos = text.find('|');
    size_t space_pos = text.find(' ');
    
    if (pipe_pos != std::string::npos) {
        format_part = text.substr(0, pipe_pos);
        content = text.substr(pipe_pos + 1);
    } else if (space_pos != std::string::npos) {
        format_part = text.substr(0, space_pos);
        content = text.substr(space_pos + 1);
    } else {
        return text;
    }
    
    std::vector<std::string> format_list = parse_formats(format_part);
    std::vector<std::string> valid_formats = get_valid_formats(format_list);
    
    if (valid_formats.empty()) {
        return text;
    }
    
    return build_tags(valid_formats, content);
}

std::string Formatter::process_link(const std::string& text) {
    std::vector<std::string> parts = split_string(text, '|');
    
    if (parts.size() >= 3) {
        std::string link_text = trim(parts[1]);
        std::string url = trim(parts[2]);
        return "<a href=\"" + url + "\">" + link_text + "</a>";
    } else if (parts.size() == 2) {
        std::string url = trim(parts[1]);
        return "<a href=\"" + url + "\">" + url + "</a>";
    }
    
    return text;
}

std::vector<std::string> Formatter::parse_formats(const std::string& format_part) {
    std::vector<std::string> format_list;
    
    if (format_part.find('-') != std::string::npos) {
        format_list = split_string(format_part, '-');
    } else if (format_part.find('+') != std::string::npos) {
        format_list = split_string(format_part, '+');
    } else if (format_part.find('.') != std::string::npos) {
        format_list = split_string(format_part, '.');
    } else if (format_part.find(',') != std::string::npos) {
        format_list = split_string(format_part, ',');
    } else if (format_part.find('_') != std::string::npos) {
        format_list = split_string(format_part, '_');
    } else {
        format_list.push_back(format_part);
    }
    
    return format_list;
}

std::vector<std::string> Formatter::get_valid_formats(const std::vector<std::string>& format_list) {
    static std::unordered_map<std::string, std::string> format_map = {
        {"b", "b"}, {"bold", "b"},
        {"i", "i"}, {"italic", "i"},
        {"u", "u"}, {"underline", "u"},
        {"s", "s"}, {"strike", "s"}, {"del", "s"},
        {"c", "code"}, {"code", "code"},
        {"p", "pre"}, {"pre", "pre"},
        {"sp", "tg-spoiler"}, {"spoiler", "tg-spoiler"},
        {"mono", "code"}, {"m", "code"},
        {"em", "i"}, {"emphasis", "i"},
        {"strong", "b"}, {"str", "b"},
        {"mark", "mark"}, {"highlight", "mark"},
        {"small", "small"}, {"sm", "small"},
        {"big", "big"}, {"large", "big"},
        {"sub", "sub"}, {"subscript", "sub"},
        {"sup", "sup"}, {"superscript", "sup"},
        {"ins", "ins"}, {"insert", "ins"},
        {"kbd", "kbd"}, {"key", "kbd"},
        {"var", "var"}, {"variable", "var"},
        {"samp", "samp"}, {"sample", "samp"},
        {"tt", "tt"}, {"teletype", "tt"},
        {"cite", "cite"}, {"citation", "cite"},
        {"dfn", "dfn"}, {"definition", "dfn"},
        {"abbr", "abbr"}, {"abbreviation", "abbr"},
        {"q", "q"}, {"quote", "q"}
    };
    
    std::vector<std::string> valid_formats;
    for (const auto& fmt : format_list) {
        std::string lower_fmt = to_lower(trim(fmt));
        if (format_map.find(lower_fmt) != format_map.end()) {
            valid_formats.push_back(format_map[lower_fmt]);
        }
    }
    
    return valid_formats;
}

std::string Formatter::build_tags(const std::vector<std::string>& valid_formats, const std::string& content) {
    std::string opening_tags, closing_tags;
    
    for (const auto& fmt : valid_formats) {
        if (fmt == "tg-spoiler") {
            opening_tags += "<span class=\"tg-spoiler\">";
            closing_tags = "</span>" + closing_tags;
        } else {
            opening_tags += "<" + fmt + ">";
            closing_tags = "</" + fmt + ">" + closing_tags;
        }
    }
    
    return opening_tags + content + closing_tags;
}

std::vector<std::string> Formatter::split_string(const std::string& str, char delim) {
    std::vector<std::string> result;
    std::istringstream iss(str);
    std::string token;
    while (std::getline(iss, token, delim)) {
        result.push_back(token);
    }
    return result;
}

std::string Formatter::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t");
    return str.substr(start, end - start + 1);
}

std::string Formatter::to_lower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}
