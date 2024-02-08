#include "utils.h"

double generateRandomDouble(double min, double max, std::mt19937& gen) {
    std::uniform_real_distribution<double> dis(min, max);
    return dis(gen);
}

int generateRandomInt(int min, int max, std::mt19937& gen) {
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}

array<double,3> generateRandomPosition(double& boundary, std::mt19937& gen){
    array<double,3> position = {0.0,0.0,0.0};
    position = {generateRandomDouble(-boundary, boundary, gen),generateRandomDouble(-boundary, boundary, gen),generateRandomDouble(-boundary, boundary, gen)};
    return position;
}

void calculateForce(vector<CelestialBody>& bodies){
    //Initializing the forces total sum being a nx3 matrix
    for (size_t i = 0; i < bodies.size(); i++){
        array<double,3> Fsum = {0.0,0.0,0.0};
        for (size_t j = 0; j < bodies.size(); j++){
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
    for (size_t i = 0; i < bodies.size(); i++){
        array<double,3> Asum = {0.0,0.0,0.0};
        for (size_t j = 0; j < bodies.size(); j++){
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
    std::ofstream output ("../output/mass.csv"); 
    std::random_device rd;
    std::mt19937 gen(rd());
    gen.seed(49);
    for (int i = 0; i < bodynumber; i++){
        int mass = generateRandomInt(l_range, h_range, gen);
        array<double,3> position = generateRandomPosition(boundary, gen); 
        CelestialBody body = CelestialBody(mass, position, {0.0,0.0,0.0}, {0.0,0.0,0.0}, {0.0,0.0,0.0});
        output << mass << ","; 
        bodies.push_back(body);
    }
    return bodies;
}   