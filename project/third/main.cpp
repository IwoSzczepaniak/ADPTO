#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <memory>
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
    bool moved = false;

    Car(int size, int x, int y, char direction)
        : size(size), x(x), y(y), direction(direction) {}

    void printCar() {
        cout << "size:" << size << " x:" << x << " y:" << y << " dir:" << direction << endl;
    }

    bool onTheEdge(const int &H, const int &W){
        return (direction == 'h' && x <= 0) || (direction == 'h' && x + size >= W) || (direction == 'v' && y + size >= H) || (direction == 'v' && y <= 0);
    }
};

class Node {
public:
    vector<Car> cars;
    const shared_ptr<Node> parent;
    vector<string> map;
    stack<string> moves;
    shared_ptr<Car> specialCar;

    Node(vector<Car>& cars, const shared_ptr<Node>& parent, vector<string>& map, stack<string>& moves, shared_ptr<Car>& specialCar)
        : cars(cars), parent(parent), map(map), moves(moves), specialCar(specialCar) {}

    Node(vector<Car>& cars, vector<string>& map) : cars(cars), map(map) {
        // parent = nullptr;
        for (auto& car : cars) {
            if (car.special) {
                specialCar = make_shared<Car>(car);
                break;
            }
        }
    }
};

vector<string> loadMap(const int& H, const int& W) {
    vector<string> map;
    string line;
    for(int i = 0; i < H; i++) {
        cin >> line;
        map.push_back(line);
    }
    return map;
}

vector<Car> findCars(const vector<string> &map) {
    vector<Car> cars;
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
            if (previousMap[y][x] != '#') map[y][x] = '.';
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


bool canMoveNSteps(const int &carNumber, const shared_ptr<Node>& state, const int& n, const char& way) {
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

shared_ptr<Node> move(const shared_ptr<Node>& prevState, const int &carNumber, int n, const char &way) {
    vector<Car> cars = prevState->cars;
    vector<string> prevMap = prevState->map;
    stack<string> moves = prevState->moves;
    Car currentCar = cars[carNumber];

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
        return NULL;
    }

    cars[carNumber] = currentCar;
    vector<string> newMap = createMap(prevMap, cars);
    
    if (currentCar.special) {
        return make_shared<Node>(cars, prevState, newMap, moves, moved_special);
    }
    return make_shared<Node>(cars, prevState, newMap, moves, prevState->specialCar);
}


shared_ptr<Node> search(shared_ptr<Node> current, int maxMoves, int& currentMoves, vector<bool>& moved) {
    if (current->specialCar->onTheEdge((int)current->map.size(), (int)current->map[0].size())) {
        return current;
    }

    if (currentMoves >= maxMoves) {
        return nullptr;
    }

    for (int i = 0; i < (int)current->cars.size(); i++) {
        if (moved[i]) continue;
        int longestMove = max(abs((int)current->map.size() - current->cars[i].y), abs((int)current->map[0].size() - current->cars[i].x));
        
        for (char direction : {'U', 'D', 'L', 'R'}) {
            for (int n = 1; n <= longestMove; n++) {
                shared_ptr<Node> newNode = move(current, i, n, direction);
                
                if (newNode != nullptr) {
                    currentMoves++;
                    moved[i] = true;
                    shared_ptr<Node> result = search(newNode, maxMoves, currentMoves, moved);
                    
                    if (result != nullptr) {
                        return result;
                    }

                    moved[i] = false;
                    currentMoves--;
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

    int currentMoves = 0;
    vector<bool> moved(root->cars.size(), false);
    shared_ptr<Node> solution = search(root, N, currentMoves, moved);

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
