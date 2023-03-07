#include <algorithm>
#include <sstream>
#include "parser.hpp"
#include "util.hpp"
#include "xString.hpp"

using std::make_unique;
using std::getline, std::hex, std::ifstream;
using std::string, std::stringstream, std::vector;


short Parser::parseShort(const string& text) const {
    if (!std::ranges::all_of(text, isdigit))
        util::syntaxError(*this);

    return static_cast<short>(stoi(text));
}
Parser::Parser(const string& filePath) {
    file = make_unique<ifstream>();
    file->open(filePath);
}
bool Parser::readNextLine() {
    while (getline(*file, line)) {
        //Count empty lines too
        lineNumber++;

        words = xString::split(line, '=', MAX_WORDS);
        //Skip empty lines
        if (words.empty())
            continue;

        //Check syntax
        if (util::contains(paramKeys, getKey())) {
            if (getWordsNumber() != MAX_WORDS)
                util::syntaxError(*this);
        }
        else if (!util::contains(singleKeys, getKey()))
            util::syntaxError(*this);

        return true;
    }
    //End of file
    return false;
}

std::string Parser::getString(const std::string& otherwise) {
    if (getWordsNumber() == 1)
        return otherwise;

    return words.at(1);
}
Pair<string> Parser::getStrings() {
    const vector<string> values = xString::split(getString(), ' ', 2);
    if (values.size() < 2)
        util::syntaxError(*this);

    return {
        values.at(0),
        values.at(1)
    };
}
short Parser::getShort(const short otherwise) {
    if (getWordsNumber() == 1)
        return otherwise;

    return parseShort(getString());
}
Pair<short> Parser::getShorts() {
    const vector<string> values = xString::split(getString(), ' ', 2);
    if (values.size() < 2)
        util::syntaxError(*this);

    return {
        parseShort(values.at(0)),
        parseShort(values.at(1))
    };
}
short Parser::getHexColor(const short otherwise) {
    if (getWordsNumber() == 1)
        return otherwise;

    stringstream s;
    s << getString();
    short out;
    s >> hex >> out;
    return out;
}
