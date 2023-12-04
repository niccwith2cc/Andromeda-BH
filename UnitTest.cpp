#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "BarnesHut.cpp"

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

TEST_CASE("testing the calculateForce function") {
    vector<CelestialBody> bodies;

    bodies.push_back(CelestialBody(50000, vector<double>{1000.0, 1000.0, 1000.0}, vector<double>(3), vector<double>(3), vector<double>(3)));
    bodies.push_back(CelestialBody(80000, vector<double>{-5000.0, -3000.0, 6000.0}, vector<double>(3), vector<double>(3), vector<double>(3)));

    vector<vector<double>> forces = calculateForce(bodies);

    BarnesHut tree = BarnesHut(&bodies[0], 0.0);
    tree.insert(&bodies[1]);


    for (int i = 0; i < bodies.size(); i++){
        CHECK(forces[i] == tree.calculateForce(bodies[i], tree.root));
    }
}