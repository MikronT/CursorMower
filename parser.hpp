#pragma once
#include <fstream>
#include <memory>
#include <string>
#include <vector>


template <typename T> struct Pair {
    T first, second;
};


class Parser {
    inline constexpr static int MAX_WORDS = 2;

    std::unique_ptr<std::ifstream> file;
    std::vector<std::string> singleKeys, paramKeys;

    int lineNumber = 0;
    std::string line;
    std::vector<std::string> words;

    [[nodiscard]] short parseShort(const std::string& text) const;
public:
    explicit Parser(const std::string& filePath);
    Parser(const Parser& that) = delete;
    Parser(Parser&& that) noexcept = delete;
    Parser& operator=(const Parser& that) = delete;
    Parser& operator=(Parser&& that) noexcept = delete;
    ~Parser() { file->close(); }


    [[nodiscard]] bool isOpen() const { return file->is_open(); }
    void setSingleKeys(const std::vector<std::string>& values) { singleKeys = values; }
    void setParamKeys(const std::vector<std::string>& values) { paramKeys = values; }


    [[nodiscard]] bool readNextLine();
    [[nodiscard]] std::string getLine() const { return line; }
    [[nodiscard]] int getLineNumber() const { return lineNumber; }
    [[nodiscard]] size_t getWordsNumber() const { return words.size(); }

    [[nodiscard]] std::string getKey() { return words.at(0); }
    [[nodiscard]] std::string getString(const std::string& otherwise = std::string());
    [[nodiscard]] Pair<std::string> getStrings();
    [[nodiscard]] short getShort(short otherwise = -1);
    [[nodiscard]] Pair<short> getShorts();
    [[nodiscard]] short getHexColor(short otherwise);
};
