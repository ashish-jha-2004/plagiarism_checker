#if __has_include("design_ea.cpp")
#include "design_ea.cpp"
#elif __has_include("ainur/design_ea.cpp")
#include "ainur/design_ea.cpp"
#else
#error "Cannot find design_ea.cpp"
#endif


/*
class star_manager_t {
    ea_t* ea;
public:
    void print_stars() {
        for (auto& star : this->ea->stars) {
            std::cout << star.name << " " << star.radius << " " << star.luminosity << " " <<
            star.coords.x << " " << star.coords.y << " " << star.coords.z << std::endl;
        }
    }

    void add_star(const std::string& name, double radius, double luminosity, 
            double x, double y, double z) {
        this->ea->stars.push_back({name, radius, luminosity, {x, y, z}});
    }

    void enlighten_star(const star_t& star) {
        for (auto& lamp : this->ea->arda->lamps) {
            double distance = sqrt(pow(lamp.coords.x - star.coords.x, 2) + 
            pow(lamp.coords.y - star.coords.y, 2) + pow(lamp.coords.z - star.coords.z, 2));
            if (distance <= lamp.radius) {
                star.luminosity += lamp.luminosity;
            }
        }
    }

    void remove_star(const star_t& star) {
        this->ea->stars.erase(std::remove(ea->stars.begin(), ea->stars.end(), star), ea->stars.end());
    }
};

class lamp_manager_t {
    arda_t* arda;
public:
    void add_lamp(const std::string& name, double radius, double luminosity, 
            double x, double y, double z) {
        this->arda->lamps.push_back({name, radius, luminosity, {x, y, z}});
    }

    void remove_lamp(const lamp_t& lamp) {
        this->arda->lamps.erase(std::remove(arda->lamps.begin(), arda->lamps.end(), lamp), 
        arda->lamps.end());
    }

    void print_lamps() {
        for (auto& lamp : this->arda->lamps) {
            std::cout << lamp.name << " " << lamp.radius << " " <<  lamp.luminosity << " " <<
            lamp.coords.x << " " << lamp.coords.y << " " << lamp.coords.z << std::endl;
        }
    }
};
*/

void add_star(ea_t* ea, std::string name, double radius, double luminosity, 
        double x, double y, double z) {
    ea->stars.push_back({name, radius, luminosity, {x, y, z}});
}

void add_lamp(arda_t* arda, std::string name, double radius, double luminosity, 
        double x, double y, double z) {
    arda->lamps.push_back({name, radius, luminosity, {x, y, z}});
}

void add_lamp(ea_t* ea, std::string name, double radius, double luminosity, 
        double x, double y, double z) {
    ea->arda->lamps.push_back({name, radius, luminosity, {x, y, z}});
}

void remove_lamp(arda_t* arda, lamp_t& lamp) {
    arda->lamps.erase(std::remove(arda->lamps.begin(), arda->lamps.end(), lamp), arda->lamps.end());
}

void remove_lamp(ea_t* ea, lamp_t& lamp) {
    remove_lamp(ea->arda.get(), lamp);
}

void remove_star(ea_t* ea, star_t& star) {
    ea->stars.erase(std::remove(ea->stars.begin(), ea->stars.end(), star), ea->stars.end());
}

void show_off(ea_t* ea) {
    for (auto& star : ea->stars) {
        std::cout << star.name << " " << star.radius << " " << star.luminosity << " " <<
        star.coords.x << " " << star.coords.y << " " << star.coords.z << std::endl;
    }
    for (auto& lamp : ea->arda->lamps) {
        std::cout << lamp.name << " " << lamp.radius << " " <<  lamp.luminosity << " " <<
        lamp.coords.x << " " << lamp.coords.y << " " << lamp.coords.z << std::endl;
    }
}

void create_plus_constellation(ea_t* ea) {
    add_star(ea, "Aldebaran", 0.5, 0.5, 0.5, 0.5, 0.5);
    add_star(ea, "Betelgeuse", 0.5, 0.5, 1.0, 0.5, 0.5);
    add_star(ea, "Rigel", 0.5, 0.5, 0.5, 1.0, 0.5);
    add_star(ea, "Antaras", 0.5, 0.5, 0.0, 0.5, 0.5);
    add_star(ea, "Canopus", 0.5, 0.5, 0.5, 0.0, 0.5);
}