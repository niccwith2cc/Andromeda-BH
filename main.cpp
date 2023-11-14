#include <vector>
#include <cmath>
#include <iostream>
#include <random>
#include "CelestialBody.cpp"
using namespace std;

double generateRandomDouble(double min, double max) {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<double> dis(min, max);
        return dis(gen);
    };

int generateRandomInt(int min, int max) {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<int> dis(min, max);
        return dis(gen);
    };

int main(){

    //rand position could be of any value R^3 [-1,000,000:1,000,000]
    //rand mass of Z [1*10^10:1*10^30]

    int n = generateRandomInt(2,10);

    for (int i=0; i < n; i++){
        
    }
    double mass = generateRandomInt(1e10,1e15);
    // vector<double> position1 = {0.0, 2.0, 0.0};
    // vector<double> accel1 = {0.0, 0.0, 0.0};
    // vector<double> force1 = {0.0, 0.0, 0.0};
    // vector<double> velo1 = {0.0, 0.0, 0.0};

    // vector<double> position2 = {0.0, 0.0, 0.0};
    // vector<double> accel2 = {0.0, 0.0, 0.0};
    // vector<double> force2 = {0.0, 0.0, 0.0};
    // vector<double> velo2 = {0.0, 0.0, 0.0};

    // vector<double> position3 = {2.0, 0.0, 0.0};
    // vector<double> accel3 = {0.0, 0.0, 0.0};
    // vector<double> force3 = {0.0, 0.0, 0.0};
    // vector<double> velo3 = {0.0, 0.0, 0.0};

    // CelestialBody body1 = CelestialBody(10000, position1, accel1, velo1, force1);
    // CelestialBody body2 = CelestialBody(20000, position2, accel2, velo2, force2);
    // CelestialBody body3 = CelestialBody(50000, position3, accel3, velo3, force3);
    // vector<CelestialBody> bodies = {body1, body2, body3};

    // cout << body1.CalcForce(body2) << endl;
    // cout << body2.CalcForce(body3) << endl; 
    // cout << body3.CalcForce(body1) << endl;
    // cout << "the components of F:" << endl;
    
    // for (int i=0 ; i < 3 ; i++){
    //     cout << body1.CalcComp(body2)[i] << "\t";
    // }
    
    // cout << endl;

    // cout << "the components of F:" << endl;
    
    // for (int i=0 ; i < 3 ; i++){
    //     cout << body2.CalcComp(body3)[i] << "\t";
    // }
    // cout << endl;

    // cout << "the components of F:" << endl;
    
    // for (int i=0 ; i < 3 ; i++){
    //     cout << body3.CalcComp(body1)[i] << "\t";
    // }
    // cout << endl;

    vector<vector<double>> Fsum (bodies.size(), vector<double> (3,0));

    for (int i = 0; i < bodies.size(); i++){
        for (int j = 0; j < bodies.size(); j++){
            if (&bodies[i] != &bodies[j]){
                vector<double> comp =  bodies[i].CalcComp(bodies[j]);
                Fsum[i][0] += comp[0];
                Fsum[i][1] += comp[1];
                Fsum[i][2] += comp[2];
                cout << "the components of Fx" << i << ": " << Fsum[i][0] << '\t' << endl;
                cout << "the componenets of Fy" << i << ": " << Fsum[i][1] << '\t' << endl ;
                cout << "the componenets of Fz" << i << ": " << Fsum[i][2] << '\t' << endl;
                
            }
        }
    }

}
