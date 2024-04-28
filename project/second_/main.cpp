#include <iostream>
#include <vector>
#include <deque>
#include <unordered_set>
#include <chrono>
#include <algorithm>
#include <stack>
#include <memory>
#include <utility>
#include <set>
#include <queue>


using namespace std;

char HORIZONTAL_SYMBOLS[] = {'a', 'b', 'c', 'd'};
char VERTICAL_SYMBOLS[] = {'x', 'y', 'z', 'w'};

class Car {
public:
    int size;
    int x;
    int y;
    char direction;
    bool special = false;

    Car(int size, int x, int y, char direction)
        : size(size), x(x), y(y), direction(direction) {}

    void printCar() {
        cout << "size:" << size << " x:" << x << " y:" << y << " dir:" << direction << endl;
    }

    bool onTheEdge(const int &H, const int &W){
        return (direction == 'h' && x == 0) || (direction == 'h' && x + size >= W) || (direction == 'v' && y + size >= H) || (direction == 'v' && y == 0);
    }
};

class Node {
public:
    vector<Car> cars;
    shared_ptr<Node> parent;
    string operation;
    vector<string> map;
    stack<string> moves;


    Node(vector<Car>& cars, shared_ptr<Node> parent, string operation, vector<string>& map)
        : cars(cars), parent(parent), operation(operation), map(map) {}
};

vector<string> loadMap(int H, int W) {
    vector<string> map;
    string line;
    for(int i = 0; i < H; i++) {
        cin >> line;
        map.push_back(line);
    }
    return map;
}

vector<Car> findCars(vector<string> &map) {
    vector<Car> cars;
    Car special_car(0, -1, -1, 'h');
    for (int y = 0; y < (int)map.size(); y++) {
        for (int x = 0; x < (int)map[y].size(); x++) {
            if (map[y][x] == 'o') {
                if (special_car.x == -1) {
                    special_car.x = x;
                    special_car.y = y;
                    special_car.special = true;
                }
                special_car.size++;
                
            } else if (map[y][x] == 'a') {
                int size = 1;
                while (x + size < (int)map[y].size() && map[y][x + size] == HORIZONTAL_SYMBOLS[size]) {
                    size++;
                }
                cars.push_back(Car(size, x, y, 'h'));
                
            }else if (map[y][x] == 'x') {
                int size = 1;
                while (y + size < (int)map.size() && map[y + size][x] == VERTICAL_SYMBOLS[size]) {
                    size++;
                }
                cars.push_back(Car(size, x, y, 'v'));
            }
        }
    }
    cars.push_back(special_car);
    return cars;
}

vector<string> createMap(const vector<string> &previousMap, const vector<Car> &cars) {
    vector<string> map = previousMap;

    for(int y = 1; y < (int)map.size() - 1; y++) {
        for(int x = 1; x < (int)map[y].size() - 1; x++) {
            map[y][x] = '.';
        }
    }

    for(Car car : cars) {
        if (car.special && car.direction == 'h') {
            for(int i = 0; i < car.size; i++) {
                map[car.y][car.x + i] = 'o';
            }
        }
        else if (car.special && car.direction == 'v') {
            for(int i = 0; i < car.size; i++) {
                map[car.y + i][car.x] = 'o';
            }
        }
        else if (car.direction == 'h') {
            for(int i = 0; i < car.size; i++) {
                map[car.y][car.x + i] = HORIZONTAL_SYMBOLS[i];
            }
        } else {
            for(int i = 0; i < car.size; i++) {
                map[car.y + i][car.x] = VERTICAL_SYMBOLS[i];
            }
        }
    }
    return map;
}


bool canMoveNSteps(const int &carNumber, Node& state, const int n, const char way) {
    Car currentCar = state.cars[carNumber];

    if(currentCar.direction == 'v' && (way == 'L' || way == 'R'))
        return false;
    if(currentCar.direction == 'h' && (way == 'U' || way == 'D'))
        return false;

    if (way == 'L'){
        if (currentCar.x - n < 0){
            return false;
        }
        for (int i = 1; i <= n; i++){
            if (state.map[currentCar.y][currentCar.x - i] != '.'){
                return false;
            }
        }

    }

    else if (way == 'R'){
        if (currentCar.x + n >= (int)state.map[0].size()){
            return false;
        }
        for (int i = 1; i <= n; i++){
            if (state.map[currentCar.y][currentCar.x + i + currentCar.size - 1] != '.'){
                return false;
            }
        }

    }

    else if (way == 'U'){
        if (currentCar.y - n < 0){
            return false;
        }
        for (int i = 1; i <= n; i++){
            if (state.map[currentCar.y - i][currentCar.x] != '.'){
                return false;
            }
        }

    }

    else if (way == 'D'){
        if (currentCar.y + n >= (int)state.map.size()){
            return false;
        }
        for (int i = 1; i <= n; i++){
            if (state.map[currentCar.y + i + currentCar.size - 1][currentCar.x] != '.'){
                return false;
            }
        }

    }

    return true;
}

shared_ptr<Node> move(shared_ptr<Node> prevState, const int &carNumber, int n, const char &way) {
    vector<Car> cars = prevState->cars;
    vector<string> prevMap = prevState->map;
    stack<string> moves = prevState->moves;
    Car currentCar = cars[carNumber];

    int xBeforeMove = currentCar.x;
    int yBeforeMove = currentCar.y;

    if (canMoveNSteps(carNumber, *prevState, n, way)){
        currentCar.x += (way == 'R') ? n : (way == 'L') ? -n : 0;
        currentCar.y += (way == 'D') ? n : (way == 'U') ? -n : 0;

        if (currentCar.special && currentCar.onTheEdge((int)prevState->map.size(), (int)prevState->map[0].size())){
            n+=1;
            if (currentCar.x == 0 || currentCar.y == 0){
                n-=2;
            }
        }
        moves.push(to_string(xBeforeMove) + ' ' + to_string(yBeforeMove) + ' ' + way + ' ' + to_string(n));
    } else {
        return NULL;
    }

    cars[carNumber] = currentCar;
    vector<string> newMap = createMap(prevMap, cars);
    
    shared_ptr<Node> newState = make_shared<Node>(cars, prevState, "", newMap);
    newState->moves = moves;
    return newState;
}

shared_ptr<Node> search(shared_ptr<Node> root, const int &maxMoves, int& currentMoves) {
    queue<shared_ptr<Node>> q;
    q.push(root);
    set<vector<string>> visited;

    while (!q.empty()) {
        shared_ptr<Node> current = q.front();
        q.pop();

        if (visited.find(current->map) != visited.end()) {
            continue;
        }
        visited.insert(current->map);

        for (Car& car : current->cars) {
            if (car.special && car.onTheEdge((int)current->map.size(), (int)current->map[0].size())) {
                return current;
            }
        }

        for (int i = 0; i < (int)current->cars.size(); i++) {
            int longestMove = max(abs((int)current->map.size() - current->cars[i].y), abs((int)current->map[0].size() - current->cars[i].x));
            for (char direction : {'U', 'D', 'L', 'R'}) {
                for (int n = 1; n <= longestMove; n++) {
                    shared_ptr<Node> newNode = move(current, i, n, direction);
                    if (newNode != NULL) {
                        currentMoves++;
                        q.push(newNode);
                    }
                }
            }
        }
    }

    return nullptr;
}



int main() {
    int W, H, N;
    cin >> W >> H >> N;

    vector<string> map = loadMap(H, W);

    vector<Car> cars = findCars(map);
    shared_ptr<Node> root = make_shared<Node>(cars, nullptr, "", map);

    int currentMoves = 0;
    shared_ptr<Node> solution = search(root, N, currentMoves);

    stack<string> res;
    if (solution != NULL) {
        stack<string> moves = solution->moves;
        int numMoves = (int)moves.size();
        cout << numMoves << endl;

        while (!moves.empty()) {
            res.push(moves.top());
            moves.pop();
        }
        while (!res.empty()) {
            cout << res.top() << endl;
            res.pop();
        }
    }
    
    return 0;
}