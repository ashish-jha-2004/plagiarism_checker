#include <iostream>
#include <vector>
#include <string>

struct coords_t {
    double x;
    double y;
    bool operator == (const coords_t& coords) const = default;
};

struct coords3d_t {
    double x;
    double y;
    double z;
    bool operator == (const coords3d_t& coords) const = default;
};

class ocean_t {
    std::string name;
    double area;
    double depth;
    coords_t coords;
    bool operator == (const ocean_t& ocean) const = default;
};

struct mountain_t {
    std::string name;
    double height;
    double area;
    coords_t coords;
    bool operator == (const mountain_t& mountain) const = default;
};

struct desert_t {
    std::string name;
    double area;
    coords_t coords;
    bool operator == (const desert_t& desert) const = default;
};

struct plain_t {
    std::string name;
    double area;
    coords_t coords;
    bool operator == (const plain_t& plain) const = default;
};

struct winds_t {
    std::string name;
    coords_t direction;
    double temperature;
    bool operator == (const winds_t& wind) const = default;
};

struct lamp_t {
    std::string name;
    double radius;
    double luminosity;
    coords3d_t coords;
    bool operator == (const lamp_t& lamp) const = default;
};

struct arda_t {
    std::string name;
    std::string description;
    std::vector<ocean_t> oceans;
    std::vector<mountain_t> mountains;
    std::vector<desert_t> deserts;
    std::vector<plain_t> plains;
    std::vector<winds_t> winds;
    std::vector<lamp_t> lamps;
    bool operator == (const arda_t& arda) const = default;
};

class winds_manager_t {
    arda_t* planet;
public:
    void add_wind(winds_t& wind) {
        this->planet->winds.push_back(wind);
    }

    void remove_wind(winds_t& wind) {
        this->planet->winds.erase(std::remove(planet->winds.begin(), planet->winds.end(), wind), planet->winds.end());
    }

    void print_winds() {
        for (auto& wind : this->planet->winds) {
            std::cout << wind.name << " " << wind.direction.x << " " << 
            wind.direction.y << " " << wind.temperature << std::endl;
        }
    }
};

struct star_t {
    std::string name;
    double radius;
    double luminosity;
    coords3d_t coords;
    bool operator == (const star_t& star) const = default;
};

struct ea_t {
    std::string name;
    std::string description;
    std::unique_ptr<arda_t> arda;
    std::vector<star_t> stars;
    bool operator == (const ea_t& ea) const = default;
};