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

struct volcano_t : mountain_t {
    double eruption_rate;
    double evilness;
    long num_orcs;
    long num_dragons;
    bool operator == (const volcano_t& volcano) const = default;
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
    std::vector<volcano_t> volcanoes;
    bool operator == (const arda_t& arda) const = default;
};

struct star_t {
    std::string name;
    double radius;
    double luminosity;
    coords3d_t coords;
    bool operator == (const star_t& star) const = default;
};

struct black_hole_t {
    std::string name;
    double mass;
    coords3d_t coords;
    bool operator == (const black_hole_t& black_hole) const = default;
};

struct ea_t {
    std::string name;
    std::string description;
    std::unique_ptr<arda_t> arda;
    std::vector<star_t> stars;
    std::vector<black_hole_t> black_holes;
    bool operator == (const ea_t& ea) const = default;
};

class destructor_t {
    ea_t* ea;
public:
    void add_black_hole(std::string name, double mass, double x, double y, double z) {
        this->ea->black_holes.push_back({name, mass, {x, y, z}});
    }

    void kill_star(star_t& star) {
        this->ea->stars.erase(std::remove(ea->stars.begin(), ea->stars.end(), star), ea->stars.end());
        this->ea->black_holes.push_back({star.name, star.luminosity, star.coords});
    }

    void add_base(std::string name, double height, double area, double x, double y,
            double eruption_rate, double evilness, long num_orcs, long num_dragons) {
        volcano_t new_volcano;
        new_volcano.name = name;
        new_volcano.height = height;
        new_volcano.area = area;
        new_volcano.coords = {x, y};
        new_volcano.eruption_rate = eruption_rate;
        new_volcano.evilness = evilness;
        new_volcano.num_orcs = num_orcs;
        new_volcano.num_dragons = num_dragons;
        this->ea->arda->volcanoes.push_back(new_volcano);
    }
};