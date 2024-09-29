#include <iostream>
#include <vector>
#include <stack>
#include <cstdint>
#include <array>

using namespace std;

//Using snippets of struct code from Mobagen like Point2D and World and Node

//Point structure to get and store grid indecies
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

//Node struct to hold wall data and if it is visited and color
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
  Node(bool south, bool west) {
    data = ((uint8_t)south << 2U) | ((uint8_t)west << 3U);
  }

private:
  uint8_t data;
  bool visited = false;
  Color color = BLACK;


public:
  //Only uses west and north walls for each node
  bool inline GetSouth() const { return data >> 2U & 1U; };
  bool inline GetWest() const { return data >> 3U & 1U; };

  // Only gest west and south walls for each node
  void inline SetSouth(bool x) { data = (data & ~(1 << 2)) | x << 2; };
  void inline SetWest(bool x) { data = (data & ~(1 << 3)) | x << 3; };

  bool isVisited() const { return visited; };
  void setIsVisited(bool visit) { visited = visit; };
  Color getColor() { return color; };
  void setColor(Color newColor) { color = newColor; };
};

//Maze struct built from Mobagen World struct for the maze object and recursive maze building
struct Maze {
  private:
  std::vector<Point2D> stack;
  std::vector<Node> visited;
  int visWidth;
  int visHeight;
  int randomNumberIndex = 0;
  inline std::vector<Node>& getBuffer() { return visited; }

  public:
    bool Step() { //Steps the current maze building process
      if (stack.empty() && visitedIsEmpty()) { //Checks if this is the first step call and adds the top left point to the stack
        stack.push_back({0,0});
      }

      if (!stack.empty()) { //While the stack is not empty mark current point/node as visited and add new node/point to the stack
        Point2D const point = stack.back(); //Gets the current point from the stack
        Node* node = GetVisited(point); //Gets the node from that point location in the maze
        std::vector<Point2D> neighbors = getVisitables(point); //Gets the neighbors of the current point/node

        if (!neighbors.empty()) { //If there are available neighbors to go to, add a random one to the stack
          node->setIsVisited(true); //Marks the current top node from the stack as visited
          node->setColor(Node::BLUE); //Sets it's color to visited color

          int r = getRandomNumber(); //Gets random number
          Point2D const nextPoint = neighbors[r%neighbors.size()]; //Uses random number modulus to choose next point
          Node* nextNode = GetVisited(nextPoint); //Gets next node from next point
          stack.push_back(nextPoint); //Adds next point to the stack

          Point2D const direction = nextPoint - point; //Finds the direction to break the walls to the next node

          //This system only uses the West and South walls for each node
          if (direction.y == -1) { //Checks if going upwards in the grid
            nextNode->SetSouth(false); //Breaks the next node's south wall
          }
          else if (direction.x == 1) { //Checks if going right in the grid
            nextNode->SetWest(false); //Breaks the next node's west wall
          }
          else if (direction.y == 1) { //Checks if going down in the grid
            node->SetSouth(false); //Breaks the current node's south wall
          }
          else if (direction.x == -1) { //Checks if going left in the grid
            node->SetWest(false); //Breaks the current node's west wall
          }
        } else { //If no neighbors then removes the last node added to the stack and marks it as finished with a green color
          Point2D point = stack.back();
          GetVisited(point)->setColor(Node::GREEN);
          stack.pop_back();
        }
      }

      return !stack.empty(); //Returns false when the stack is empty meaning all the nodes have been visited
    }

    int getRandomNumber() { //Function to get a "random" number from the random number array
      const std::array randomInts = {72, 99, 56, 34, 43, 62, 31, 4, 70, 22, 6, 65, 96, 71, 29, 9, 98, 41, 90, 7, 30, 3, 97, 49, 63, 88, 47, 82, 91, 54, 74, 2, 86, 14, 58, 35, 89, 11, 10, 60, 28, 21, 52, 50, 55, 69, 76, 94, 23, 66, 15, 57, 44, 18, 67, 5, 24, 33, 77, 53, 51, 59, 20, 42, 80, 61, 1, 0, 38, 64, 45, 92, 46, 79, 93, 95, 37, 40, 83, 13, 12, 78, 75, 73, 84, 81, 8, 32, 27, 19, 87, 85, 16, 25, 17, 68, 26, 39, 48, 36};

      if (randomNumberIndex == std::size(randomInts)) { //Loop index if reaches the end of the array
        randomNumberIndex = 0;
      }
      return randomInts[randomNumberIndex++]; //Returns the random number at the current index and increments index
    }

  void setRandomIndex(int index) { randomNumberIndex = index; }; //Sets the initial value of the random number index

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

  std::vector<Point2D> getVisitables(const Point2D& p) {// Gets visitable neighbors of a point
      std::vector<Point2D> visitables;

      //Checks top neighbor
      Node* checkNode = GetVisited({p.x, p.y - 1});
      if (p.y - 1 >= 0 && !checkNode->isVisited() && checkNode->getColor() != Node::GREEN) {
        visitables.emplace_back(p.x, p.y - 1);
      }

      //Checks right neighbor
      checkNode = GetVisited({p.x + 1, p.y});
      if (p.x + 1 < visWidth && !checkNode->isVisited() && checkNode->getColor() != Node::GREEN){
        visitables.emplace_back(p.x + 1, p.y);
      }

      //Checks bottom neighbor
      checkNode = GetVisited({p.x, p.y + 1});
      if (p.y + 1 < visHeight && !checkNode->isVisited() && checkNode->getColor() != Node::GREEN) {
        visitables.emplace_back(p.x, p.y + 1);
      }

      //Checks left neighbor
      checkNode = GetVisited({p.x - 1, p.y});
      if (p.x - 1 >= 0 && !checkNode->isVisited() && checkNode->getColor() != Node::GREEN) {
        visitables.emplace_back(p.x - 1, p.y);
      }

      return visitables;
    }

  //Print function to output maze grid
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
  //Initializes variables
  Maze maze;
  int width = 0, height = 0, index = 0;

  //Gets input
  std::cin >> width >> height >> index; //Gets inputs

  //Sets up maze from input
  maze.Resize(width, height);
  maze.setRandomIndex(index);

  //Executes until finished
  while(maze.Step()) {
    //maze.print();
  }

  //Prints out the result
  cout << endl;
  maze.print();
}


