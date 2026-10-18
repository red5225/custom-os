#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace {
struct Value { std::string text; bool number = false; };
std::string trim(std::string s) {
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) s.erase(s.begin());
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) s.pop_back();
    return s;
}
bool starts(const std::string& s, const std::string& p) { return s.rfind(p, 0) == 0; }
bool quoted(const std::string& s, std::string& out) {
    if (s.size() < 2 || s.front() != '"' || s.back() != '"') return false;
    out.clear();
    for (std::size_t i = 1; i + 1 < s.size(); ++i) {
        if (s[i] == '\\' && i + 2 < s.size()) {
            const char c = s[++i];
            out += c == 'n' ? '\n' : c == 't' ? '\t' : c;
        } else out += s[i];
    }
    return true;
}
bool valueOf(const std::string& raw, Value& v) {
    const std::string s = trim(raw);
    if (quoted(s, v.text)) { v.number = false; return true; }
    char* end = nullptr;
    std::strtoll(s.c_str(), &end, 10);
    if (!s.empty() && end == s.c_str() + s.size()) { v.text = s; v.number = true; return true; }
    return false;
}
bool truth(const std::string& expression, const std::map<std::string, Value>& vars, bool& result) {
    const auto op = expression.find("==");
    if (op == std::string::npos) return false;
    const std::string name = trim(expression.substr(0, op));
    Value rhs;
    if (!valueOf(expression.substr(op + 2), rhs)) return false;
    const auto it = vars.find(name);
    if (it == vars.end()) { result = false; return true; }
    result = it->second.text == rhs.text;
    return true;
}
std::size_t matching(const std::vector<std::string>& lines, std::size_t from, bool wantElse) {
    int depth = 0;
    for (std::size_t i = from + 1; i < lines.size(); ++i) {
        const std::string s = trim(lines[i]);
        if (starts(s, "if ")) ++depth;
        else if (s == "end") {
            if (depth == 0) return i;
            --depth;
        } else if (wantElse && s == "else" && depth == 0) return i;
    }
    return lines.size();
}
int run(const std::string& path) {
    std::ifstream in(path);
    if (!in) { std::cerr << "xc: cannot open " << path << '\n'; return 2; }
    std::vector<std::string> lines;
    for (std::string line; std::getline(in, line);) lines.push_back(line);
    std::map<std::string, Value> vars;
    for (std::size_t i = 0; i < lines.size(); ++i) {
        std::string s = trim(lines[i]);
        if (s.empty() || s[0] == '#') continue;
        if (starts(s, "program ")) { Value name; if (!valueOf(s.substr(8), name) || name.number) goto syntax; continue; }
        if (starts(s, "say ")) { Value v; if (!valueOf(s.substr(4), v) || v.number) goto syntax; std::cout << v.text << '\n'; continue; }
        if (starts(s, "let ")) {
            const auto eq = s.find('=');
            if (eq == std::string::npos) goto syntax;
            const std::string name = trim(s.substr(4, eq - 4));
            if (name.empty() || !std::isalpha(static_cast<unsigned char>(name[0]))) goto syntax;
            for (char c : name) if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_') goto syntax;
            Value v; if (!valueOf(s.substr(eq + 1), v)) goto syntax;
            vars[name] = v; continue;
        }
        if (starts(s, "add ")) {
            std::istringstream ss(s.substr(4)); std::string name, amount, extra;
            if (!(ss >> name >> amount) || (ss >> extra)) goto syntax;
            const auto it = vars.find(name);
            if (it == vars.end() || !it->second.number) goto syntax;
            char* end = nullptr; const long long n = std::strtoll(amount.c_str(), &end, 10);
            if (end == amount.c_str() || *end) goto syntax;
            const long long old = std::strtoll(it->second.text.c_str(), nullptr, 10);
            it->second.text = std::to_string(old + n); continue;
        }
        if (starts(s, "show ")) {
            const std::string name = trim(s.substr(5)); const auto it = vars.find(name);
            if (it == vars.end()) goto syntax;
            std::cout << it->second.text << '\n'; continue;
        }
        if (starts(s, "ask ")) {
            const std::string name = trim(s.substr(4));
            std::string input; if (!std::getline(std::cin, input)) input.clear();
            vars[name] = Value{input, false}; continue;
        }
        if (starts(s, "if ")) {
            bool ok = false;
            if (!truth(s.substr(3), vars, ok)) goto syntax;
            const std::size_t e = matching(lines, i, true);
            const std::size_t end = matching(lines, i, false);
            if (end == lines.size()) goto syntax;
            if (!ok) i = e == lines.size() ? end : e;
            continue;
        }
        if (s == "else") {
            const std::size_t end = matching(lines, i, false);
            if (end == lines.size()) goto syntax;
            i = end; continue;
        }
        if (s == "end") continue;
        if (s == "finish") return 0;
        std::cerr << "xc: line " << (i + 1) << ": unknown statement\n";
        return 1;
syntax:
        std::cerr << "xc: line " << (i + 1) << ": invalid syntax or value\n";
        return 1;
    }
    return 0;
}
}
int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: xc <program.xc>\n";
        return 2;
    }
    return run(argv[1]);
}
