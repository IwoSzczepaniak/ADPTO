#include <string>

using namespace std;

enum class CarType {
    Horizontal,
    Vertical,
    Dean
};

class Car {
public:
    CarType type;
    int length;
    int row;
    int col;
    vector<Car> children;

    Car(CarType t, int len, int row, int col) : type(t), length(len), row(row), col(col) {}

    Car(){}

    ~Car() { 
        children.clear();
    }

    string getTypeString() const {
        switch (type) {
            case CarType::Horizontal:
                return "Horizontal";
            case CarType::Vertical:
                return "Vertical";
            case CarType::Dean:
                return "Dean";
            default:
                return "Unknown";
        }
    }

    bool operator<(const Car& other) const {
        if (row != other.row) {
            return row < other.row;
        }
        if (col != other.col) {
            return col < other.col;
        }
        return type < other.type;
    }
};
