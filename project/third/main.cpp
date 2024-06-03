#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <memory>
#include <queue>
#include <unordered_set>


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
    bool moved = false;

    Car(const int& size, int x, const int y, const char& direction)
        : size(size), x(x), y(y), direction(direction) {}

    const void printCar() {
        cout << "size:" << size << " x:" << x << " y:" << y << " dir:" << direction << endl;
    }

    const bool onTheEdge(const int &H, const int &W){
        return (direction == 'h' && x <= 0) || (direction == 'h' && x + size >= W) || (direction == 'v' && y + size >= H) || (direction == 'v' && y <= 0);
    }
};

class Node {
public:
    vector<Car> cars;
    vector<string> map;
    stack<string> moves;
    shared_ptr<Car> specialCar;

    Node(vector<Car>& cars, vector<string>& map, stack<string>& moves, shared_ptr<Car>& specialCar)
        : cars(cars), map(map), moves(moves), specialCar(specialCar) {}

    Node(vector<Car>& cars, vector<string>& map) : cars(cars), map(map) {
        for (auto& car : cars) {
            if (car.special) {
                specialCar = make_shared<Car>(car);
                break;
            }
        }
    }
};

struct NodeHasher {
    size_t operator()(const Node &node) const {
        size_t hash_ = 0;
        for (const auto &car : node.cars) {
            hash_ ^= hash<int>()(car.x) ^ hash<int>()(car.y) ^ hash<int>()(car.size) ^ hash<char>()(car.direction);
        }
        return hash_;
    }
};

struct NodePtrHasher {
    size_t operator()(const shared_ptr<Node>& node_ptr) const {
        NodeHasher hasher;
        return hasher(*node_ptr);
    }
};


const vector<string> loadMap(const int& H, const int& W) {
    vector<string> map;
    map.reserve(H);
    string line;
    for(int i = 0; i < H; i++) {
        cin >> line;
        map.emplace_back(move(line));
    }
    return map;
}

const vector<Car> findCars(const vector<string> &map) {
    vector<Car> cars;
    cars.reserve(64);
    Car special_car(0, -1, -1, 'h');
    for (int y = 0; y < (int)map.size(); y++) {
        for (int x = 0; x < (int)map[y].size(); x++) {
            if (map[y][x] == 'o') {
                if (special_car.x == -1) {
                    special_car.x = x;
                    special_car.y = y;
                    special_car.special = true;
                    if(map[y][x + 1] == 'o') {
                        special_car.direction = 'h';
                    } else {
                        special_car.direction = 'v';
                    }
                }
                special_car.size++;
                
            } else if (map[y][x] == 'a') {
                int size = 1;
                while (x + size < (int)map[y].size() && map[y][x + size] == HORIZONTAL_SYMBOLS[size]) {
                    size++;
                }
                cars.emplace_back(size, x, y, 'h');
                
            }else if (map[y][x] == 'x') {
                int size = 1;
                while (y + size < (int)map.size() && map[y + size][x] == VERTICAL_SYMBOLS[size]) {
                    size++;
                }
                cars.emplace_back(size, x, y, 'v');
            }
        }
    }
    cars.emplace_back(move(special_car));
    return cars;
}

void modifyMap(vector<string> &map, const vector<Car> &cars) {
    for(int y = 1; y < (int)map.size() - 1; y++) {
        for(int x = 1; x < (int)map[y].size() - 1; x++) {
            if (map[y][x] != '#') map[y][x] = '.';
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
}


const bool canMoveNSteps(const int &carNumber, const shared_ptr<Node>& state, const int& n, const char& way) {
    Car currentCar = state->cars[carNumber];

    if(currentCar.moved) return false;

    if(currentCar.direction == 'v' && (way == 'L' || way == 'R'))
        return false;
    if(currentCar.direction == 'h' && (way == 'U' || way == 'D'))
        return false;

    if (way == 'L'){
        if (currentCar.x - n < 0){
            return false;
        }
        for (int i = 1; i <= n; i++){
            if (state->map[currentCar.y][currentCar.x - i] != '.'){
                return false;
            }
        }

    }

    else if (way == 'R'){
        if (currentCar.x + n >= (int)state->map[0].size()){
            return false;
        }
        for (int i = 1; i <= n; i++){
            if (state->map[currentCar.y][currentCar.x + i + currentCar.size - 1] != '.'){
                return false;
            }
        }

    }

    else if (way == 'U'){
        if (currentCar.y - n < 0){
            return false;
        }
        for (int i = 1; i <= n; i++){
            if (state->map[currentCar.y - i][currentCar.x] != '.'){
                return false;
            }
        }

    }

    else if (way == 'D'){
        if (currentCar.y + n >= (int)state->map.size()){
            return false;
        }
        for (int i = 1; i <= n; i++){
            if (state->map[currentCar.y + i + currentCar.size - 1][currentCar.x] != '.'){
                return false;
            }
        }

    }

    return true;
}

const shared_ptr<Node> move(const shared_ptr<Node>& prevState, const int &carNumber, int& n, const char &way) {
    vector<Car> cars = prevState->cars;
    vector<string> prevMap = prevState->map;
    stack<string> moves = prevState->moves;
    Car currentCar = prevState->cars[carNumber];

    int xBeforeMove = currentCar.x;
    int yBeforeMove = currentCar.y;
    shared_ptr<Car> moved_special = nullptr;

    if (canMoveNSteps(carNumber, prevState, n, way)){
        currentCar.x += (way == 'R') ? n : (way == 'L') ? -n : 0;
        currentCar.y += (way == 'D') ? n : (way == 'U') ? -n : 0;

        if (currentCar.special && currentCar.onTheEdge((int)prevState->map.size(), (int)prevState->map[0].size())){
            n+=1;
        }
        moves.push(to_string(xBeforeMove) + ' ' + to_string(yBeforeMove) + ' ' + way + ' ' + to_string(n));
        currentCar.moved = true;
        if (currentCar.special) {
            moved_special = make_shared<Car>(currentCar);
        }
    } else {
        return nullptr;
    }

    cars[carNumber] = currentCar;
    modifyMap(prevMap, cars);
    
    if (currentCar.special) {
        return make_shared<Node>(cars, prevMap, moves, moved_special);
    }
    return make_shared<Node>(cars, prevMap, moves, prevState->specialCar);
}

const int getLongestMove(const char& direction, const shared_ptr<Node>& current, const int& i){
    int cnt = 1;
    int start;
    switch (direction)
            {
            case 'U':
                start = current->cars[i].y - 1;
                while (start > 0 && current->map[start - 1][current->cars[i].x] == '.'){
                    start--;
                    cnt++;
                }
                return cnt;
            case 'D':
                start = current->cars[i].y + 1;
                while (start < (int)current->map.size() - 1 && current->map[start + 1][current->cars[i].x] == '.'){
                    start++;
                    cnt++;
                }
                return cnt;
            case 'L':
                start = current->cars[i].x - 1;
                while (start > 0 && current->map[current->cars[i].y][start - 1] == '.'){
                    start--;
                    cnt++;
                }
                return cnt;
            case 'R':
                start = current->cars[i].x + 1;
                while (start < (int)current->map[0].size() - 1 && current->map[current->cars[i].y][start + 1] == '.'){
                    start++;
                    cnt++;
                }
                return cnt;
            default:
                return -1;
            }
}


const shared_ptr<Node> search(const shared_ptr<Node>& root, const int &maxMoves) {
    vector<shared_ptr<Node>> q;
    q.emplace_back(move(root));
    unordered_set<shared_ptr<Node>, NodePtrHasher> visited;

    while (!q.empty()) {
        int randomIndex = rand() % q.size();
        shared_ptr<Node> current = q[randomIndex];
        q.erase(q.begin() + randomIndex);

        // remove random 20 % of the nodes
        

        if (visited.find(current) != visited.end()) {
            continue;
        }
        visited.insert(current);

        if (current->specialCar->onTheEdge((int)current->map.size(), (int)current->map[0].size())) {
            return current;
        }

        for (int i = 0; i < (int)current->cars.size(); i++) {
            if(current->cars[i].moved) continue;
            for (char direction : {'U', 'D', 'L', 'R'}) {
                int longestMove = getLongestMove(direction, current, i);
                for (int n = 1; n <= longestMove; n++) {
                    shared_ptr<Node> newNode = move(current, i, n, direction);
                    if (newNode != NULL && (int)newNode->moves.size() <= maxMoves && visited.find(newNode) == visited.end()){
                        if (q.size() > 1000 && rand() % 3 == 0) {
                            continue;
                        }
                        q.emplace_back(move(newNode));
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
    shared_ptr<Node> root = make_shared<Node>(cars, map);

    shared_ptr<Node> solution = search(root, N);

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
    else {
        cout << "No solution" << endl;
    }

    return 0;
}
