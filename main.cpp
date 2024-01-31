#include <vector>
#include <cmath>
#include <iostream>
#include <random>
#include <experimental/random>
#include <fstream>
#include <memory>
#include <chrono>
#include "src/BarnesHut.h"
#include "src/CelestialBody.h"
#include "src/ConfigParser.h"

using std::vector;
using std::cout;
using std::endl;
using std::experimental::randint;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;


// Hardcoding the Galaxy by taking a fixed number of bodies and generating their random positions and masses
// Random number generation to use for the position of the bodies
double generateRandomDouble(double min, double max) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(min, max);
        return dis(gen);
    };

array<double,3> generateRandomPosition(double boundary){
    array<double,3> position = {0.0,0.0,0.0};
    for (int i = 0; i < 3; i++){
            position[i] = generateRandomDouble(-boundary, boundary);
        }   
    return position;
};

void calculateForce(vector<CelestialBody>& bodies){
    //Initializing the forces total sum being a nx3 matrix
    array<double,3> Fsum = {0.0,0.0,0.0};

    for (int i = 0; i < bodies.size(); i++){
        Fsum = {0.0,0.0,0.0};
        for (int j = 0; j < bodies.size(); j++){
            if (&bodies[i] != &bodies[j]){
                array<double,3> comp =  bodies[i].CalcCompF(bodies[j]);
                Fsum = Fsum + comp;
            }
        }
        bodies[i].setForce(Fsum);      
    }
}

void calculateAcceleration(vector<CelestialBody>& bodies){
    //Initializing the acceleration total sum being a nx3 matrix
    array<double,3> Asum = {0.0,0.0,0.0};

    for (int i = 0; i < bodies.size(); i++){
        Asum = {0.0,0.0,0.0};
        for (int j = 0; j < bodies.size(); j++){
            if (&bodies[i] != &bodies[j]){
                array<double,3> comp =  bodies[i].CalcCompA(bodies[j]);
                Asum = Asum + comp; 
            }
        }
        bodies[i].setAccel(Asum);
    }
}


//Generating the bodies using the CelestialBody class and giving them random masses and positions
vector<CelestialBody> generateBodies(int bodynumber, double boundary, int massMin, int massMax){
    vector<CelestialBody> bodies; 
    const int l_range = massMin;
    const int h_range = massMax; 
    for (int i = 0; i < bodynumber; i++){
        int mass = std::experimental::randint(l_range,h_range);
        array<double,3> position = generateRandomPosition(boundary); 
        CelestialBody body = CelestialBody(mass, position, {0.0,0.0,0.0}, {0.0,0.0,0.0}, {0.0,0.0,0.0});
        bodies.push_back(body);
    }
    return bodies;
}   

//Overloading the << operator to print the positions of the bodies
template < class T > inline std::ostream& operator << (std::ostream& os, const std::array<T,3>& a) {
    for (auto &elem: a){
        os << elem << ", ";
    }
    return os;
}

int main(){

    //Parsing the config file and initializing constants
    ConfigParser parser = ConfigParser("../config.ini");
    
    const double BOUNDARY =  std::stod(parser.config["boundary"]);
    const double TIMESTEP =  std::stod(parser.config["timestep"]);
    const int NO_OF_BODIES =  std::stoi(parser.config["bodynumber"]);
    const int MASS_MIN =  std::stoi(parser.config["mass_minimum"]);
    const int MASS_MAX =  std::stoi(parser.config["mass_maximum"]);
    const int DURATION =  std::stoi(parser.config["duration"]);
    const bool BRUTEFORCE =  std::stoi(parser.config["bruteforce"]);
    const bool THETA =  std::stoi(parser.config["theta"]);

    //Initializing the boundary for the TreeNode class
    TreeNode dummy;
    dummy.setBoundary(BOUNDARY);
    
    //timing components of the code
    auto t1 = high_resolution_clock::now();
        //Generating the bodies and initializing the Barnes-Hut algorithm
        vector<CelestialBody> bodies = generateBodies(NO_OF_BODIES, BOUNDARY, MASS_MIN, MASS_MAX);
    auto t2 = high_resolution_clock::now();

    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;
        cout << "time to generate bodies: " << ms_double.count() << "ms" << endl;

    BarnesHut tree;  


    //Calculating the force and acceleration for each body either brute force or using the algorithm
    if (BRUTEFORCE) { 
        t1 = high_resolution_clock::now();
            calculateForce(bodies);
        t2 = high_resolution_clock::now();

        /* Getting number of milliseconds as a double. */
        ms_double = t2 - t1;
            cout << "time to calculate initial force using brute force: " << ms_double.count() << "ms" << endl; 
    }
    else {
        t1 = high_resolution_clock::now();
            tree = BarnesHut(std::make_unique<CelestialBody>(bodies[0]), THETA);
            for (int i = 1; i < bodies.size(); i++) tree.insert(std::make_unique<CelestialBody>(bodies[i]));
        t2 = high_resolution_clock::now();

        /* Getting number of milliseconds as a double. */
        ms_double = t2 - t1;
            cout << "time to build tree: " << ms_double.count() << "ms" << endl;

        t1 = high_resolution_clock::now();
            for (int i = 0; i < bodies.size(); i++) bodies[i].setForce(tree.calculateForce(bodies[i], tree.root));
        t2 = high_resolution_clock::now();

        /* Getting number of milliseconds as a double. */
        ms_double = t2 - t1;
            cout << "time to calculate initial force using algorithm: " << ms_double.count() << "ms" << endl;
    }
    calculateAcceleration(bodies);

    // The way we can complete the calculations:
    // take an infinitesimally small time increment to integrate over, dt.
    // to calculate the velocity of each body. v = int[bounded](a* dt) + v_0 if there exists any previous velocity
    // to calculate the position of each body. p = int[bounded](v * dt) + p_0 = int[bounded](a * t * dt) + p_0


    std::ofstream filestream ("pos.csv"); //Filestream to write the positions of the bodies

t1 = high_resolution_clock::now();

t1 = high_resolution_clock::now();

    for (int t = 0; t < DURATION; t++){ //for every time step
        for (int i = 0; i < bodies.size(); i++){ //for every body
            array<double,3> Aint = bodies[i].getAccel();
            array<double,3> Vint = bodies[i].getVelo();
            array<double,3> Pint = bodies[i].getPosition();
            for (int j = 0; j < 3; j++){ //for every axis
                Vint[j] = Aint[j]*TIMESTEP + Vint[j]; // V should increase linearly
                Pint[j] = Vint[j]*TIMESTEP + Pint[j]; 
            }
            bodies[i].setVelo(Vint);
            bodies[i].setPosition(Pint);

            filestream << Pint;
        }
        filestream << '\n';

        //Calculating the force and acceleration for each body for every time step
        if (BRUTEFORCE) calculateForce(bodies);
        else for (int i = 0; i < bodies.size(); i++) bodies[i].setForce(tree.calculateForce(bodies[i], tree.root));
        calculateAcceleration(bodies);
    }
t2 = high_resolution_clock::now();

/* Getting number of milliseconds as a double. */
ms_double = t2 - t1;
    cout << "time to run simulation: " << ms_double.count() << "ms" << endl;
}
