#include <vector>
#include <cmath>
#include <iostream>
#include "CelestialBody.cpp"
using namespace std;

int main(){
    vector<double> position1 = {5.0, 1.0, -15.0};
    vector<double> accel1 = {0.0, 0.0, 0.0};
    vector<double> force1 = {0.0, 0.0, 0.0};
    vector<double> velo1 = {0.0, 0.0, 0.0};

    vector<double> position2 = {-4.0, 8.0, 3.0};
    vector<double> accel2 = {0.0, 0.0, 0.0};
    vector<double> force2 = {0.0, 0.0, 0.0};
    vector<double> velo2 = {0.0, 0.0, 0.0};

    CelestialBody body1 = CelestialBody(10000, position1, accel1, velo1, force1);
    CelestialBody body2 = CelestialBody(20000, position2, accel2, velo2, force2);

    cout << body1.CalcForce(body2) << endl;
    cout << "the components of F:" << endl;
    
    for (int i=0 ; i < 3 ; i++){
        cout << body1.CalcComp(body2)[i] << "\t";
    }
    
    cout << endl;
    }