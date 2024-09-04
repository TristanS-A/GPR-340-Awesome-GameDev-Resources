#include <iostream>
#include <iomanip>
#include <vector>
#include <utility>
#include <cmath>

using namespace std;

struct Vector2 {
  double x=0, y=0;
  Vector2() : x(0), y(0){};
  Vector2(double x, double y) : x(x), y(y){};
  Vector2(const Vector2& v) = default;

  // unary operations
  Vector2 operator-() const { return {-x, -y}; }
  Vector2 operator+() const { return {x, y}; }

  // binary operations
  Vector2 operator-(const Vector2& rhs) const { return {x - rhs.x, y - rhs.y}; }
  Vector2 operator+(const Vector2& rhs) const { return {x + rhs.x, y + rhs.y}; }
  Vector2 operator*(const double& rhs) const { return {x * rhs, y * rhs}; }
  friend Vector2 operator*(const double& lhs, const Vector2& rhs) { return {lhs * rhs.x, lhs * rhs.y}; }
  Vector2 operator/(const double& rhs) const { return {x / rhs, y / rhs}; }
  Vector2 operator/(const Vector2& rhs) const { return {x / rhs.x, y / rhs.y}; }
  bool operator!=(const Vector2& rhs) const { return (*this - rhs).sqrMagnitude() >= 1.0e-6; };
  bool operator==(const Vector2& rhs) const { return (*this - rhs).sqrMagnitude() < 1.0e-6; };

  // assignment operation
  Vector2& operator=(Vector2 const& rhs) = default;
  Vector2& operator=(Vector2&& rhs) = default;

  // compound assignment operations
  Vector2& operator+=(const Vector2& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }
  Vector2& operator-=(const Vector2& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }
  Vector2& operator*=(const double& rhs) {
    x *= rhs;
    y *= rhs;
    return *this;
  }
  Vector2& operator/=(const double& rhs) {
    x /= rhs;
    y /= rhs;
    return *this;
  }
  Vector2& operator*=(const Vector2& rhs) {
    x *= rhs.x;
    y *= rhs.y;
    return *this;
  }
  Vector2& operator/=(const Vector2& rhs) {
    x /= rhs.x;
    y /= rhs.y;
    return *this;
  }

  double sqrMagnitude() const { return x * x + y * y; }
  double getMagnitude() const { return sqrt(sqrMagnitude()); }
  static double getMagnitude(const Vector2& vector) { return vector.getMagnitude(); }

  static double Distance(const Vector2& a, const Vector2& b) { return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)); };
  double Distance(const Vector2& b) const { return sqrt((x - b.x) * (x - b.x) + (y - b.y) * (y - b.y)); };
  static double DistanceSquared(const Vector2& a, const Vector2& b) { return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y); };
  double DistanceSquared(const Vector2& b) const { return (x - b.x) * (x - b.x) + (y - b.y) * (y - b.y); };

  static Vector2 normalized(const Vector2& v) { return v.normalized(); };
  Vector2 normalized() const {
    auto magnitude = getMagnitude();

    // If the magnitude is not null
    if (magnitude > 0.)
      return Vector2(x, y) / magnitude;
    else
      return {x, y};
  };

  static const Vector2 zero;
};

const Vector2 Vector2::zero = {0, 0};

struct Boid {
  Boid(const Vector2& pos, const Vector2& vel): position(pos), velocity(vel){};
  Boid():position({0,0}), velocity({0,0}){};
  Vector2 position;
  Vector2 velocity;
};

struct Cohesion {
  double radius;
  double k;

  Cohesion() = default;

  Vector2 ComputeForce(const vector<Boid>& boids, int boidAgentIndex) {
    Vector2 centerOfMass = {0,0};
    int numberOfBoidsInRadius = 0;

    //Runs through the boids vector to search for boids within the radius
    for (int i = 0; i < boids.size(); ++i) {
        const double distenceToBoid = (boids[i].position - boids[boidAgentIndex].position).getMagnitude();
        if (distenceToBoid <= radius && i != boidAgentIndex) { //Checks if distance is under radius and excludes self

          //Adds position of mass and increments boids within the radius found
          centerOfMass += boids[i].position;
          numberOfBoidsInRadius++;
      }
    }

    //Calculates accurate center of mass
    centerOfMass /= numberOfBoidsInRadius;

    //Finds vector to center of mass from agent
    Vector2 agentDirection = centerOfMass - boids[boidAgentIndex].position;

    //make sure that center of mass is in radius
    if (agentDirection.getMagnitude() <= radius) {
      return (agentDirection / radius) * k;
    }

    return {0, 0};
  }
};

struct Alignment {
  double radius;
  double k;

  Alignment() = default;

  Vector2 ComputeForce(const vector<Boid>& boids, int boidAgentIndex) {
    Vector2 avarageVelocity = {0,0};
    int numberOfBoidsInRadius = 0;

    //Runs through the boids vector to search for boids within the radius (includes itself)
    for (int i = 0; i < boids.size(); ++i) {
      const double distenceToBoid = (boids[i].position - boids[boidAgentIndex].position).getMagnitude();
      if (distenceToBoid <= radius) {
        //Adds position of mass and increments boids within the radius found
        avarageVelocity += boids[i].velocity;
        numberOfBoidsInRadius += 1;
      }
    }

    //Calculates accurate average velocity
    avarageVelocity /= numberOfBoidsInRadius;
    avarageVelocity *= k;

    return avarageVelocity;
  }
};

struct Separation {
  double radius;
  double k;
  double maxForce;

  Separation() = default;

  Vector2 ComputeForce(const vector<Boid>& boids, int boidAgentIndex) {
    Vector2 seperationForce = {0,0};

    //Runs through the boids vector to search for boids within the radius
    for (int i = 0; i < boids.size(); ++i) {
      if (i != boidAgentIndex) { //Excludes the agent's self
        const double distenceToBoid = (boids[boidAgentIndex].position - boids[i].position).getMagnitude();
        if (0 < distenceToBoid && distenceToBoid <= radius) {
          Vector2 otherBoidToAgentVector = boids[boidAgentIndex].position - boids[i].position;

          seperationForce += otherBoidToAgentVector / otherBoidToAgentVector.sqrMagnitude(); //More efficient than otherBoidToAgentVector.normalized() / otherBoidToAgentVector.getMagnitude()
        }
      }
    }

    seperationForce *= k;

    //To satisfy separation tests you must use this type of clamping
    if (seperationForce.getMagnitude() > maxForce) {
      return seperationForce.normalized() * maxForce * k;
    }

    return seperationForce * k;

    //More accurate separation result
    seperationForce *= k;

    //Scales seperation for to max force times k if larger than maxForce
    if (seperationForce.getMagnitude() > maxForce) {
      return seperationForce.normalized() * maxForce;
    }

    return seperationForce;
  }
};

// feel free to edit this main function to meet your needs
int main() {
  // Variable declaration
  Separation separation{};
  Alignment alignment{};
  Cohesion cohesion{};
  int numberOfBoids;
  string line; // for reading until EOF
  vector<Boid> currentState, newState;
  // Input Reading
  cin >> cohesion.radius >> separation.radius >> separation.maxForce >> alignment.radius >> cohesion.k >> separation.k >> alignment.k >> numberOfBoids;
  for (int i = 0; i < numberOfBoids; i++) {
    Boid b;
    cin >> b.position.x >> b.position.y >> b.velocity.x >> b.velocity.y;
    //cout << "b.y: " << b.y << endl;
    currentState.push_back(b);
    newState.push_back(b);
  }
  cin.ignore(256, '\n');
  // Final input reading and processing
  // todo: edit this. probably my code will be different than yours.
  while (getline(cin, line)) { // game loop
    // Use double buffer! you should read from the current and store changes in the new state.
    currentState = newState;
    double deltaT = stod(line);
    // a vector of the sum of forces for each boid.
    vector<Vector2> allForces = vector<Vector2>(numberOfBoids, {0, 0});
    // Compute Forces
    for (int i = 0; i < numberOfBoids; i++)  // for every boid
    {
      for (int j = 0; j < numberOfBoids; j++)  // for every boid combination. Pre-processing loop.
      {
        // Process Cohesion Forces
        auto dist = (currentState[i].position-currentState[j].position).getMagnitude();
        if (i != j && dist <= cohesion.radius) {
          allForces[i] += cohesion.ComputeForce(currentState, i);
        }
        // Process Separation Forces
        if (i != j && dist <= separation.radius) {
          allForces[i] += separation.ComputeForce(currentState, i);
        }
        // Process Alignment Forces
        if (i != j && dist <= alignment.radius) {
          allForces[i] += alignment.ComputeForce(currentState, i);
        }
      }
    }

    // Tick Time and Output
    // todo: edit this. probably my code will be different than yours.
    cout << fixed << setprecision(3);  // set 3 decimal places precision for output
    for (int i = 0; i < numberOfBoids; i++) // for every boid
    {
      cout << "force " << allForces[i].x * deltaT << " " << newState[i].velocity.x << " " << newState[i].position.x + allForces[i].x * deltaT * deltaT << " time " << deltaT << endl;
      newState[i].velocity += allForces[i] * deltaT;
      newState[i].position += newState[i].velocity * deltaT; //Applies velocity to boid
      cout << newState[i].position.x << " " << newState[i].position.y << " "
           << newState[i].velocity.x << " " << newState[i].velocity.y << endl;
    }
  }

  return 0;
}
