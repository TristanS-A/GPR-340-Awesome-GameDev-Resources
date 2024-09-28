#include <iostream>
#include <vector>
#include <stack>
#include <cstdint>
#include <array>

using namespace std;

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
  Point2D operator-(const Point2D& rhs) const { return {x - rhs.x, y - rhs.y}; };
  Point2D& operator-=(const Point2D& rhs);
  std::string to_string();
};

struct Node {
public:
  enum Color {
    BLACK,
    BLUE,
    GREEN
  };
  Node() {
    data = 1 | (1 << 1U) | (1 << 2U) | (1 << 3U);
  };
  Node(bool north, bool east, bool south, bool west) {
    data = ((uint8_t)north) | ((uint8_t)east << 1U) | ((uint8_t)south << 2U) | ((uint8_t)west << 3U);
  }

private:
  uint8_t data;
  bool visited = false;
  Color color = BLACK;


public:
  // todo: can you improve this?
  bool inline GetNorth() const { return data & 1U; };
  bool inline GetEast() const { return data >> 1U & 1U; };
  bool inline GetSouth() const { return data >> 2U & 1U; };
  bool inline GetWest() const { return data >> 3U & 1U; };

  // todo set
  // todo: can you improve this?
  void inline SetNorth(bool x) { data = (data & ~(1 << 0)) | x << 0; };
  void inline SetEast(bool x) { data = (data & ~(1 << 1)) | x << 1; };
  void inline SetSouth(bool x) { data = (data & ~(1 << 2)) | x << 2; };
  void inline SetWest(bool x) { data = (data & ~(1 << 3)) | x << 3; };

  bool isVisited() const { return visited; };
  void setIsVisited(bool visit) { visited = visit; };
  Color getColor() { return color; };
  void setColor(Color newColor) { color = newColor; };
};
struct Maze {
  private:
  std::vector<Point2D> stack;
  std::vector<Node> visited;
  int visWidth;
  int visHeight;
  inline std::vector<Node>& getBuffer() { return visited; }

  public:
    bool Step() {
      if (stack.empty() && visitedIsEmpty()) {
        stack.push_back({0,0});
      }

      if (!stack.empty()) {
        Point2D const point = stack.back();
        Node* node = GetVisited(point);
        std::vector<Point2D> neighbors = getVisitables(point);

        cout << neighbors.size() << '\n';
        if (!neighbors.empty()) {
          node->setIsVisited(true);
          node->setColor(Node::BLUE);

          int r = getRandomNumber();
          Point2D const nextPoint = neighbors[r%neighbors.size()];
          Node* nextNode = GetVisited(nextPoint);
          stack.push_back(nextPoint);

          cout << r << endl;
          Point2D const direction = nextPoint - point;

          if (direction.y == -1) {
            nextNode->SetSouth(false);
          }
          else if (direction.x == 1) {
            nextNode->SetWest(false);
          }
          else if (direction.y == 1) {
            node->SetSouth(false);
          }
          else if (direction.x == -1) {
            node->SetWest(false);
          }

        } else {
          Point2D point = stack.back();
          GetVisited(point)->setColor(Node::GREEN);
          stack.pop_back();
        }
      }

      return !stack.empty();
    }

    int getRandomNumber() {
      static int randomIndex = 0;
      const std::array randomInts = {72, 99, 56, 34, 43, 62, 31, 4, 70, 22, 6, 65, 96, 71, 29, 9, 98, 41, 90, 7, 30, 3, 97, 49, 63, 88, 47, 82, 91, 54, 74, 2, 86, 14, 58, 35, 89, 11, 10, 60, 28, 21, 52, 50, 55, 69, 76, 94, 23, 66, 15, 57, 44, 18, 67, 5, 24, 33, 77, 53, 51, 59, 20, 42, 80, 61, 1, 0, 38, 64, 45, 92, 46, 79, 93, 95, 37, 40, 83, 13, 12, 78, 75, 73, 84, 81, 8, 32, 27, 19, 87, 85, 16, 25, 17, 68, 26, 39, 48, 36};

      if (randomIndex == std::size(randomInts)) {
        randomIndex = 0;
      }
      return randomInts[randomIndex++];
    }

  inline const int& getVisitedWidth() const { return visWidth; }
  inline const int& getVisitedHeight() const { return visHeight; }
  void Resize(int newWidth, int newHeight) {
      visWidth = newWidth;
      visHeight = newHeight;
      visited.clear();
      visited.resize(visWidth * visHeight);
    }

  Node* GetVisited(Point2D point) {
      if (point.x < 0) point.x += visWidth;
      if (point.x >= visWidth) point.x %= visWidth;
      if (point.y < 0) point.y += visHeight;
      if (point.y >= visHeight) point.y %= visHeight;
      auto index = point.y * visWidth + point.x;
      auto sideSquared = visWidth * visHeight;
      if (index >= sideSquared) index %= sideSquared;
      return &visited[index];
    }

  bool visitedIsEmpty() {
      int count = 0;
      for (Node n : visited)  {
        count += static_cast<int>(n.isVisited());
      }

      return count == 0;
    }

  std::vector<Point2D> getVisitables(const Point2D& p) {
      std::vector<Point2D> visitables;

      Node* checkNode = GetVisited({p.x, p.y - 1});
      if (p.y - 1 >= 0 && !checkNode->isVisited() && checkNode->getColor() != Node::GREEN) {
        visitables.emplace_back(p.x, p.y - 1);
      }

      checkNode = GetVisited({p.x + 1, p.y});
      if (p.x + 1 < visWidth && !checkNode->isVisited() && checkNode->getColor() != Node::GREEN){
        visitables.emplace_back(p.x + 1, p.y);
      }

      checkNode = GetVisited({p.x, p.y + 1});
      if (p.y + 1 < visHeight && !checkNode->isVisited() && checkNode->getColor() != Node::GREEN) {
        visitables.emplace_back(p.x, p.y + 1);
      }

      checkNode = GetVisited({p.x - 1, p.y});
      if (p.x - 1 >= 0 && !checkNode->isVisited() && checkNode->getColor() != Node::GREEN) {
        visitables.emplace_back(p.x - 1, p.y);
      }

      return visitables;
    }

  //Print function to output world grid
  void print() {
      for (int i = 0; i < visWidth; i++) {
        cout << " _";
      }

      cout << "  " << std::endl;

      for (int i = 0; i < visHeight; i++) {
        for (int j = 0; j < visWidth; j++) {
          Node* node = GetVisited({j, i});
          if (node->GetWest()) {
            cout << "|";
          }
          else {
            cout << " ";
          }
          if (node->GetSouth()) {
            cout << "_";
          }
          else {
            cout << " ";
          }
        }
        std::cout << "| ";
        std::cout << std::endl;
      }
    }
};

int main()
{
  Maze maze;
  int flippedWidth = 0, flippedHeight = 0, turns = 0;

  std::cin >> flippedWidth >> flippedHeight >> turns; //Gets inputs

  maze.Resize(flippedWidth, flippedHeight); //Resizes world grid

  while(maze.Step()) {
    //maze.print();
  }

  maze.print();
}


