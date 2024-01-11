#include <vector>
#include <cmath>
#include <iostream>
#include <random>
#include <experimental/random>
#include <fstream>
#include "BarnesHut.h"
#include "CelestialBody.h"

using std::vector;
using std::cout;
using std::endl;
using std::experimental::randint;

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

    constexpr int bodynumber =  6;
    
    int mass; //should mass be also constexpr? 
    array<double,3> position = {0.0,0.0,0.0};
    vector<CelestialBody> bodies = generateBodies(bodynumber);
    calculateForce(bodies); //brute force
    calculateAcceleration(bodies);

    BarnesHut tree = BarnesHut(&bodies[0]);
    for (int i = 1; i < bodies.size(); i++) tree.insert(&bodies[i]);
    array<array<double,3>,bodynumber> F = {0.0,0.0,0.0};
    for (int i = 0; i < bodies.size(); i++) F[i] = tree.calculateForce(bodies[i], tree.root);


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
        std::ofstream filestream1 ("vel.csv");
        std::ofstream filestream2 ("acc.csv");

        for (int i = 0; i < bodies.size(); i++){

            array<double,3> pos = bodies[i].getPosition();
            array<double,3> vel = bodies[i].getVelo();
            array<double,3> acc = bodies[i].getAccel();
            filestream << pos[0] << ", " << pos[1] << ", " << pos[2] << ", ";
            filestream1 << vel[0] << ", " << vel[1] << ", " << vel[2] << ", ";
            filestream2 << acc[0] << ", " << acc[1] << ", " << acc[2] << ", ";
        }
        filestream << '\n';
        filestream1 << '\n';
        filestream2 << '\n';

        array<double,3> pos = bodies[0].getPosition();
        for (int t = 0; t < time.size(); t++){ //for every time step
            for (int i = 0; i < bodies.size(); i++){ //for every body
                array<double,3> Aint = bodies[i].getAccel();
                array<double,3> Vint = bodies[i].getVelo();
                array<double,3> Pint = bodies[i].getPosition();
                for (int j = 0; j < 3; j++){ //for every axis
                    Vint[j] = Aint[j]*timeStep + Vint[j]; // V should increase linearly
                    Pint[j] = Vint[j]*timeStep + Pint[j]; // please work, se parakalo
                }
                bodies[i].setVelo(Vint);
                bodies[i].setPosition(Pint);

                filestream << Pint[0] << ", " << Pint[1] << ", " << Pint[2]<< ", ";
                filestream1 << Vint[0] << ", " << Vint[1] << ", " << Vint[2] << ", ";
                filestream2 << Aint[0] << ", " << Aint[1] << ", " << Aint[2] << ", ";
            }
            calculateForce(bodies);
            calculateAcceleration(bodies);
            filestream << '\n';
            filestream1 << '\n';
            filestream2 << '\n';
        }
}
