#include <vector>
#include <iostream>

struct Point2D {
public:
  Point2D(int x, int y) : x(x), y(y){};
  Point2D() = default;
  int x, y;
  bool operator==(const Point2D& rhs) const;
  bool operator!=(const Point2D& rhs) const;
  Point2D& operator=(const Point2D& rhs);
  Point2D operator+(const Point2D& rhs) const;
  Point2D& operator+=(const Point2D& rhs);
  Point2D operator-(const Point2D& rhs) const;
  Point2D& operator-=(const Point2D& rhs);
  std::string to_string();
};

struct World {
private:
  // double buffer approach to avoid memory reallocation
  std::vector<bool> buffer[2];
  int currentBufferId;
  int width;
  int height;
  inline std::vector<bool>& currentBuffer() { return buffer[currentBufferId % 2]; }
  inline std::vector<bool>& nextBuffer() { return buffer[(currentBufferId + 1) % 2]; }

public:
  inline const int& getWidth() const { return width; }
  inline const int& getHeight() const { return height; }
  void Resize(int newWidth, int newHeight) {
    currentBufferId = 0;
    width = newWidth;
    height = newHeight;
    buffer[0].clear();
    buffer[0].resize(width * height);
    buffer[1].clear();
    buffer[1].resize(width * height);
  }
  // to be called at the end of the frame
  void SwapBuffers() {
    currentBufferId = (currentBufferId + 1) % 2;
    for (int i = 0; i < buffer[currentBufferId].size(); i++) buffer[(currentBufferId + 1) % 2][i] = buffer[currentBufferId][i];
  }
  bool Get(Point2D point) {
    if (point.x < 0) point.x += width;
    if (point.x >= width) point.x %= width;
    if (point.y < 0) point.y += height;
    if (point.y >= height) point.y %= height;
    auto index = point.y * width + point.x;
    auto sideSquared = width * height;
    if (index >= sideSquared) index %= sideSquared;
    return buffer[currentBufferId % 2][index];
  }

  void SetNext(Point2D point, bool value) {
    if (point.x < 0) point.x += width;
    if (point.x >= width) point.x %= width;
    if (point.y < 0) point.y += height;
    if (point.y >= height) point.y %= height;
    auto index = point.y * width + point.x;
    auto sideSquared = width * height;
    if (index >= sideSquared) index %= sideSquared;
    buffer[(currentBufferId + 1) % 2][index] = value;
  }

  void SetCurrent(Point2D point, bool value) {
    if (point.x < 0) point.x += width;
    if (point.x >= width) point.x %= width;
    if (point.y < 0) point.y += height;
    if (point.y >= height) point.y %= height;
    auto index = point.y * width + point.x;
    auto sideSquared = width * height;
    if (index >= sideSquared) index %= sideSquared;
    buffer[currentBufferId % 2][index] = value;
  }

  void print() {
    for (int i = 0; i < width; i++) {
      for (int j = 0; j < height; j++) {
        if (Get({i, j})) {
          std::cout << "#";
        }
        else {
          std::cout << ".";
        }
      }
      std::cout << std::endl;
    }
  }
};


int main() {
  World world;
  bool playing = true;
  int flippedWidth = 0, flippedHeight = 0, turns = 0;
  char currState = ' ';

  while (playing) {
    std::cin >> flippedWidth >> flippedHeight >> turns;

    world.Resize(flippedHeight, flippedWidth);

    for (int i = 0; i < world.getWidth(); i++) {
      for (int j = 0; j < world.getHeight(); j++) {
        std::cin >> currState;
        if (currState == '.') {
          world.SetCurrent({i, j}, false);
        }
        else {
          world.SetCurrent({i, j}, true);
        }
      }
    }

    world.print();
    //playing = false;
  }

  return 0;
}
