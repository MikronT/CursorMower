#pragma once
#include <map>
#include <string>
#include <vector>
#include "commandLine.hpp"


struct Point {
    short x = 1,
          y = 1;

    [[nodiscard]] auto getCoord() const;
};

struct Line {
    short color = CommandLine::COLOR_DEFAULT;
    std::string text;
};

struct Drawable {
    Point at;
    std::vector<Line> lines;
};


class IDrawable {
public:
    IDrawable() = default;
    IDrawable(const IDrawable&) = default;
    IDrawable(IDrawable&&) noexcept = default;
    IDrawable& operator=(const IDrawable&) = default;
    IDrawable& operator=(IDrawable&&) noexcept = default;
    virtual ~IDrawable() = default;

    [[nodiscard]] virtual bool shouldApplyMargins() const { return true; }
    virtual std::vector<Drawable> draw() = 0;
};

class Container final : public IDrawable {
    short margin = 0;
    std::vector<std::shared_ptr<IDrawable>> elements;
public:
    Container() = default;
    Container(const Container& that) = default;
    Container(Container&& that) noexcept = default;
    Container& operator=(const Container& that) = default;
    Container& operator=(Container&& that) noexcept = default;
    ~Container() override = default;


    Container& setMargin(const int value) {
        margin = static_cast<short>(value);
        return *this;
    }
    Container& put(const std::shared_ptr<IDrawable>& element) {
        elements.push_back(element);
        return *this;
    }

    [[nodiscard]] short getMargin() const { return margin; }

    std::vector<Drawable> draw() override;
};


class Cursor : Point {
public:
    Cursor& toX(const int value) {
        x = static_cast<short>(value);
        return *this;
    }
    Cursor& toY(const int value) {
        y = static_cast<short>(value);
        return *this;
    }
    Cursor& down(const int steps = 1) {
        y = static_cast<short>(y + steps);
        return *this;
    }
    Cursor& left(const int steps = 1) {
        x = static_cast<short>(x - steps);
        return *this;
    }
    Cursor& right(const int steps = 1) {
        x = static_cast<short>(x + steps);
        return *this;
    }
    Cursor& up(const int steps = 1) {
        y = static_cast<short>(y - steps);
        return *this;
    }

    [[nodiscard]] Point dropPoint() const { return Point(x, y); }
};

class Window {
    CommandLine cmd;
    short width = 0,
          height = 0;
    std::map<int, std::string> colormap;
    bool dimsUpdated = false,
         colormapUpdated = false;
public:
    explicit Window(const CommandLine& cmd) :
        cmd(cmd) {
        const auto& dims = cmd.getScreenDims();
        setWidth(dims->X);
        setHeight(dims->Y);
    }
    Window(const Window& that) = default;
    Window(Window&& that) noexcept = default;
    Window& operator=(const Window& that) = default;
    Window& operator=(Window&& that) noexcept = default;
    ~Window() = default;


    Window& setWidth(const int value) {
        width = static_cast<short>(value);
        dimsUpdated = true;
        return *this;
    }
    Window& setHeight(const int value) {
        height = static_cast<short>(value);
        dimsUpdated = true;
        return *this;
    }
    Window& remapColor(const std::string& key, const std::string& value);

    [[nodiscard]] short getWidth() const { return width; }
    [[nodiscard]] short getHeight() const { return height; }

    void show(Container& container);
};


class Paragraph final : public IDrawable {
    Point at;
    Line current;
    std::vector<Line> lines;
public:
    Paragraph() = default;
    explicit Paragraph(const Point& at) :
        at(at) {}
    explicit Paragraph(const Cursor& at) :
        at(at.dropPoint()) {}
    Paragraph(const Paragraph& that) = default;
    Paragraph(Paragraph&& that) noexcept = default;
    Paragraph& operator=(const Paragraph& that) = default;
    Paragraph& operator=(Paragraph&& that) noexcept = default;
    ~Paragraph() override = default;


    Paragraph& setPenColor(const short value) {
        current.color = value;
        return *this;
    }
    Paragraph& writeLine(const std::string& text) {
        current.text = text;
        lines.emplace_back(current);
        return *this;
    }

    std::vector<Drawable> draw() override;
};

class Rect final : public IDrawable {
    Point at;
    short width = 0, height = 0, margin = 0;
    bool fillScreen = false;
    short color = CommandLine::COLOR_DEFAULT;
public:
    Rect() = default;
    explicit Rect(const Window& window, const Container& container) :
        at(Point(1, 1)),
        width(window.getWidth()),
        height(window.getHeight()),
        margin(container.getMargin()),
        fillScreen(true) {}
    explicit Rect(const Point& at, const Point& to) :
        at(at),
        width(to.x),
        height(to.y) {}
    explicit Rect(const Cursor& at, const Cursor& to) :
        at(at.dropPoint()) {
        const auto [w, h] = to.dropPoint();
        width = static_cast<short>(w - this->at.x);
        height = static_cast<short>(h - this->at.y);
    }
    Rect(const Rect& that) = default;
    Rect(Rect&& that) noexcept = default;
    Rect& operator=(const Rect& that) = default;
    Rect& operator=(Rect&& that) noexcept = default;
    ~Rect() override = default;


    Rect& setFillColor(const short value) {
        color = value;
        return *this;
    }

    [[nodiscard]] bool shouldApplyMargins() const override { return !fillScreen; }
    std::vector<Drawable> draw() override;
};
