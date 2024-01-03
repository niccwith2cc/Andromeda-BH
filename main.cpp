#include <vector>
#include <cmath>
#include <iostream>
#include <random>
#include <experimental/random>
#include <fstream>
#include "BarnesHut.cpp"

using std::vector;
using std::cout;
using std::endl;
using std::experimental::randint;

// Hardcoding the Galaxy by taking a fixed number of bodies and generating their random positions and masses

// Random number generation to use for the position of the bodies
double generateRandomDouble(double min, double max) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(min, max);
        return dis(gen);
    };

vector<double> generateRandomPosition(){
    vector<double> position(3);
    for (int i = 0; i < 3; i++){
            position[i] = generateRandomDouble(-BOUNDARY, BOUNDARY);
        }   
    return position;
};

vector<vector<double>> calculateForce(vector<CelestialBody> bodies){
    //Initializing the forces total sum being a nx3 matrix
    vector<vector<double>> Fsum (bodies.size(), vector<double> (3,0));

    for (int i = 0; i < bodies.size(); i++){
        for (int j = 0; j < bodies.size(); j++){
            if (&bodies[i] != &bodies[j]){
                vector<double> comp =  bodies[i].CalcCompF(bodies[j]);
                Fsum[i] = Fsum[i] + comp; 
            }
        }     
    }
    return Fsum;
}

vector<vector<double>> calculateAcceleration(vector<CelestialBody> bodies){
    //Initializing the acceleration total sum being a nx3 matrix
    vector<vector<double>> Asum (bodies.size(), vector<double> (3,0));

    for (int i = 0; i < bodies.size(); i++){
        for (int j = 0; j < bodies.size(); j++){
            if (&bodies[i] != &bodies[j]){
                vector<double> comp =  bodies[i].CalcCompA(bodies[j]);
                Asum[i] = Asum[i] + comp; 
            }
        }     
    }
    return Asum;
}


//Generating the bodies using the CelestialBody class and giving them random masses and positions
vector<CelestialBody> generateBodies(int bodynumber){
    vector<CelestialBody> bodies; 
    for (int i = 0; i < bodynumber; i++){
        int mass = std::experimental::randint(500000,1000000);
        vector<double> position = generateRandomPosition(); //check it out later
        CelestialBody body = CelestialBody(mass, position, vector<double>(3), vector<double>(3), vector<double>(3));
        bodies.push_back(body);
    }
    return bodies;
}   
    


int main(){

    int bodynumber =  6;
    int mass;
    vector<double> position(3);
    vector<CelestialBody> bodies = generateBodies(bodynumber);
    vector<vector<double>> Fsum = calculateForce(bodies); //brute force
    vector<vector<double>> Asum = calculateAcceleration(bodies);

    BarnesHut tree = BarnesHut(&bodies[0]);
    for (int i = 1; i < bodies.size(); i++) tree.insert(&bodies[i]);
    vector<vector<double>> F (bodies.size(), vector<double> (3,0));
    for (int i = 0; i < bodies.size(); i++) F[i] = tree.calculateForce(bodies[i], tree.root);


    vector<double> time;
    const double timeStep = 1.0 / 100.0; // Calculate the time step size    
        for (int i = 0; i < 1001; i++) {
            double currentTime = i * timeStep;
            time.push_back(currentTime);
        }

        // The way we can complete the calculations:
        // take an infinitesimally small time increment to integrate over, dt.
        // to calculate the velocity of each body. v = int[bounded](a* dt) + v_0 if there exists any previous velocity
        // to calculate the position of each body. p = int[bounded](v * dt) + p_0 = int[bounded](a * t * dt) + p_0

        vector<vector<double>> Aint (bodies.size(), vector<double> (3));
        vector<vector<double>> Vint (bodies.size(), vector<double> (3));
        vector<vector<double>> Pint (bodies.size(), vector<double> (3));

        std::ofstream filestream ("pos.csv"); 

        for (int i = 0; i < bodies.size(); i++){
            vector<double> pos = bodies[i].getPosition();
            filestream << pos[0] << ", " << pos[1] << ", " << pos[2] << ", ";
        }
        filestream << '\n';

        for (int t = 0; t < time.size(); t++){
            for (int i = 0; i < bodies.size(); i++){
                Aint[i] = Asum[i];
                bodies[i].setAccel(Asum[i]);
                for (int j = 0; j < 3; j++){
                    double Vprev = Vint[i][j];
                    Vint[i][j] = Aint[i][j]*timeStep + Vint[i][j];
                    Pint[i][j] = 0.5*(Vint[i][j]+Vprev)*timeStep + Pint[i][j];
                }
                vector<double> pos = bodies[i].getPosition();
                bodies[i].setPosition(pos + Pint[i]);
                
            }
            //cout << "\n";
            for (int i = 0; i < bodies.size(); i++){
                filestream << Pint[i][0] << ", " << Pint[i][1] << ", " << Pint[i][2] << ", ";
            }
            filestream << '\n';
        }
}
