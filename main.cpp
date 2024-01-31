#include <vector>
#include <cmath>
#include <iostream>
#include <random>
#include <experimental/random>
#include <fstream>
#include <memory>
#include <chrono>
#include "BarnesHut.h"
#include "CelestialBody.h"

using std::vector;
using std::cout;
using std::endl;
using std::experimental::randint;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

const double BOUNDARY = 1000000.0;
// Hardcoding the Galaxy by taking a fixed number of bodies and generating their random positions and masses
// Random number generation to use for the position of the bodies
double generateRandomDouble(double min, double max) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(min, max);
        return dis(gen);
    };

array<double,3> generateRandomPosition(){
    array<double,3> position = {0.0,0.0,0.0};
    for (int i = 0; i < 3; i++){
            position[i] = generateRandomDouble(-BOUNDARY, BOUNDARY);
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
vector<CelestialBody> generateBodies(int bodynumber){
    vector<CelestialBody> bodies; 
    constexpr int l_range = 500000;
    constexpr int h_range = 1000000; 
    for (int i = 0; i < bodynumber; i++){
        int mass = std::experimental::randint(l_range,h_range);
        array<double,3> position = generateRandomPosition(); //check it out later
        CelestialBody body = CelestialBody(mass, position, {0.0,0.0,0.0}, {0.0,0.0,0.0}, {0.0,0.0,0.0});
        bodies.push_back(body);
    }
    return bodies;
}   
    
template < class T > inline std::ostream& operator << (std::ostream& os, const std::array<T,3>& a) {
    for (auto &elem: a){
        os << elem << " ";
    }
    return os;
}

int main(){

    constexpr int bodynumber =  500;
    
    array<double,3> position = {0.0,0.0,0.0};

    auto t1 = high_resolution_clock::now();
        vector<CelestialBody> bodies = generateBodies(bodynumber);
    auto t2 = high_resolution_clock::now();
    // calculateForce(bodies); //brute force
    // calculateAcceleration(bodies);

    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;
        cout << "time taken to generate all bodies: "<< ms_double.count() << " ms" << endl;

    auto t3 = high_resolution_clock::now();
        BarnesHut tree = BarnesHut(std::make_unique<CelestialBody>(bodies[0])); //change this later
        for (int i = 1; i < bodies.size(); i++) tree.insert(std::make_unique<CelestialBody>(bodies[i]));
    auto t4 = high_resolution_clock::now();

    duration<double, std::milli> ms_double2 = t4 - t3;
        cout << "time taken to build the tree: "<< ms_double2.count() << " ms" << endl;

    auto t5 = high_resolution_clock::now();
        for (int i = 0; i < bodies.size(); i++)  bodies[i].setForce(tree.calculateForce(bodies[i], tree.root));
    auto t6 = high_resolution_clock::now();

    duration<double, std::milli> ms_double3 = t6 - t5;
        cout << "time taken to calculate the force: "<< ms_double3.count() << " ms" << endl;


    vector<double> time;
    const double timeStep = 1.0 / 100.0; // Calculate the time step size    
        for (int i = 0; i < 10000; i++) {
            double currentTime = i * timeStep;
            time.push_back(currentTime);
        }

        // The way we can complete the calculations:
        // take an infinitesimally small time increment to integrate over, dt.
        // to calculate the velocity of each body. v = int[bounded](a* dt) + v_0 if there exists any previous velocity
        // to calculate the position of each body. p = int[bounded](v * dt) + p_0 = int[bounded](a * t * dt) + p_0


        std::ofstream filestream ("pos.csv"); 

    auto t7 = high_resolution_clock::now();
        for (int i = 0; i < bodies.size(); i++){

            array<double,3> pos = bodies[i].getPosition();
            array<double,3> vel = bodies[i].getVelo();
            array<double,3> acc = bodies[i].getAccel();
            filestream << pos[0] << ", " << pos[1] << ", " << pos[2] << ", ";
        }
        filestream << '\n';

        array<double,3> pos = bodies[0].getPosition();
        for (int t = 0; t < time.size(); t++){ //for every time step
            for (int i = 0; i < bodies.size(); i++){ //for every body
                array<double,3> Aint = bodies[i].getAccel();
                array<double,3> Vint = bodies[i].getVelo();
                array<double,3> Pint = bodies[i].getPosition();
                for (int j = 0; j < 3; j++){ //for every axis
                    Vint[j] = Aint[j]*timeStep + Vint[j]; // V should increase linearly
                    Pint[j] = Vint[j]*timeStep + Pint[j]; 
                }
                bodies[i].setVelo(Vint);
                bodies[i].setPosition(Pint);

                filestream << Pint[0] << ", " << Pint[1] << ", " << Pint[2]<< ", ";
            }
            //calculateForce(bodies);
            for (int i = 0; i < bodies.size(); i++)  bodies[i].setForce(tree.calculateForce(bodies[i], tree.root));
            calculateAcceleration(bodies);
            filestream << '\n';
        }
    auto t8 = high_resolution_clock::now();

    duration<double, std::milli> ms_double4 = t8 - t7;
        cout << "time taken to calculate the algorithm: "<< ms_double4.count() << " ms" << endl;
}
