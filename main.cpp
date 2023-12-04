#define DOCTEST_CONFIG_IMPLEMENT

#include <vector>
#include <cmath>
#include <iostream>
#include <random>
#include <experimental/random>
#include <fstream>
#include "doctest.h"
#include "BarnesHut.cpp"

using std::vector;
using std::cout;
using std::endl;
using std::experimental::randint;

// Hardcoding the Galaxy by taking a fixed number of bodies and generating their random positions and masses

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
}

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

int main(){

    int bodynumber =  2;
    int mass;
    vector<double> position(3);
    vector<CelestialBody> bodies;

    //generating the bodies using the CelestialBody class and giving them random masses and positions
    for (int i = 0; i < bodynumber; i++){
        mass = std::experimental::randint(500000,1000000);
        position = generateRandomPosition(); //check it out later
        CelestialBody body = CelestialBody(mass, position, vector<double>(3), vector<double>(3), vector<double>(3));
        bodies.push_back(body);
    }

    //visualization and printing
    cout << "The masses of the bodies: " << endl;
    for (int i = 0 ; i <bodies.size(); i++){
        cout << bodies[i].getMass() << " \t";
    }
    cout << endl;


    vector<vector<double>> Fsum = calculateForce(bodies);
    vector<vector<double>> Asum = calculateAcceleration(bodies);

            cout << "The Forces:" << endl ;
    for (int i = 0; i < bodies.size(); i++){
        for (int j = 0; j < 3; j++){
            cout << Fsum[i][j] << " \t";
        }
            cout << endl;
    }
    
    //     cout << endl;
    //     cout <<"The Acceleration:" << endl ;

    for (int i = 0; i < bodies.size(); i++){
        for (int j = 0; j < 3; j++){
            cout << Asum[i][j] << " \t";
        }
            cout << endl;
    }



    //     cout << endl;
    //     cout << "the positions: " << endl;
    // for (int i = 0; i < bodies.size(); i++){
    //     cout << "for body " << i << ": ";
    //     for (int j = 0; j < 3; j++){
    //         cout << bodies[i].getPosition().at(j)<< " ";
    //     }
    //         cout << endl;
    // }

    //     cout << endl;
    //     cout << "the position vectors: " ;;
    // for (int i = 0; i < bodies.size(); i++){
    //     for (int j = 0; j < bodies.size(); j++){
    //         if (&bodies[i] != &bodies[j]){
    //         cout << bodies[i].CalcR(bodies[j])<< " ";
    //         }
    //         cout << endl;
    //     }
    // }

    //     cout << "check " << Fsum[0][0] << endl;
    //     cout << "check " << Fsum[0][1] << endl;
    //     cout << "check " << Fsum[0][2] << endl;


        vector<double> pos1  = vector<double>{ 10.0, 10.0, 10.0 };
        vector<double> pos2  = vector<double>{ 100000.0, 100000.0, 100000.0 };
        vector<double> pos3  = vector<double>{ 900.0, -200.0, 30.0 };
        vector<double> pos4  = vector<double>{ -8490.0, -500.0, 30.0  };
        CelestialBody body1 = CelestialBody(500000, pos1, vector<double>(3), vector<double>(3), vector<double>(3));
        CelestialBody body2 = CelestialBody(500000, pos2, vector<double>(3), vector<double>(3), vector<double>(3));
        CelestialBody body3 = CelestialBody(555555, pos3, vector<double>(3), vector<double>(3), vector<double>(3));
        CelestialBody body4 = CelestialBody(4, pos4, vector<double>(3), vector<double>(3), vector<double>(3));

        // vector<double> origin  = vector<double>(3);
        // TreeNode root = TreeNode(&body1);
        // root.insertBody(&body2);

        BarnesHut tree = BarnesHut(&bodies[0]);
        for (int i = 1; i < bodynumber; i++) tree.insert(&bodies[i]);
        // tree.insert(&body2);
        // tree.insert(&body3);
        vector<double> force = tree.calculateForce(bodies[0], tree.root); 
        cout << force[0] << " " << force[1] << " " << force[2] << endl;
        cout << Fsum[0][0] << " " << Fsum[0][1] << " " << Fsum[0][2] << endl;


        // vector<double> brute_force = body1.CalcCompF(body2); 
        // cout << brute_force[0] << " " << brute_force[1] << " " << brute_force[2] << endl;



        //root.insertBody(&body3);
        //root.insertBody(&body4);
        //root.insertBody(&bodies[2]);
        //root.insertBody(&bodies[3]);
        cout << "Tree" << endl;
        //root.traverseTree(&root);
        //cout << root.centerOfMass[0] << " " << root.centerOfMass[1] << " " << root.centerOfMass[2] << endl;

        //Calculating speed and distance from acceleration
        //it is just taking the acceleration given and integrating over a specified time period, with additional intial velocity considered
        //for now we assume vo and so are 0.  
        // v = a*t + vo where t is the instant at which the acceleration is calculated.
        // s = a*t^2/2 + vo*t + s0 

        vector<double> time;
        const double timeStep = 1.0 / 100.0; // Calculate the time step size
        
        for (int i = 0; i < 1001; i++) {
            double currentTime = i * timeStep;
            time.push_back(currentTime);
        }

        cout << "\n";

        // for (int i = 0; i < 3; i++){
        // cout << bodies[i].setAccel(acomp[0], acomp[1], acomp[2]) << " ";
        // }
        // cout << "\n";

        // The way we can complete the calculations:
        // take an infinitesimally small time increment to integrate over, dt.
        // to calculate the velocity of each body. v = int[bounded](a* dt) + v_0 if there exists any previous velocity
        // to calculate the position of each body. p = int[bounded](v * dt) + p_0 = int[bounded](a * t * dt) + p_0

        // hence the pseudo code would be:
        // for all bodies {
        //      for each time step{s
            //      use setters to set the acceleration??
        // vector<vector<double>> Aint (bodies.size(), vector<double> (3));
        // vector<vector<double>> Vint (bodies.size(), vector<double> (3));
        // vector<vector<double>> Pint (bodies.size(), vector<double> (3));

        // std::ofstream pos ("pos.csv"); 

        // for (int t = 0; t < time.size(); t++){
        //     for (int i = 0; i < bodies.size(); i++){
        //         Aint[i] = Asum[i];
        //         bodies[i].setAccel(Asum[i]);
        //         for (int j = 0; j < 3; j++){
        //             Vint[i][j] = Aint[i][j]*timeStep + Vint[i][j];
        //             Pint[i][j] = 0.5*Aint[i][j]*timeStep*timeStep + Vint[i][j]*timeStep + Pint[i][j];
        //             vector<double> pos = bodies[i].getPosition();
        //             //bodies[i].setPosition(pos + Pint[i]);
        //             //deltaP..
        //             bodies[i].setPosition(pos + Pint[j]);
        //         }

        //         cout << "A " << Aint[i][0] << "\t" << Aint[i][1] << "\t" << Aint[i][2] << endl;
        //         cout << "V " << Vint[i][0] << "\t" << Vint[i][1] << "\t" << Vint[i][2] << endl;	
        //         cout << "P " << Pint[i][0] << "\t" << Pint[i][1] << "\t" << Pint[i][2] << endl;
        //     }
        //     cout << "\n";
        //     for (int i = 0; i < bodies.size(); i++){
        //         pos << Pint[i][0] << ", " << Pint[i][1] << ", " << Pint[i][2] << ", ";
        //     }
        //     cout << '\n';
        // }

        //visualization and printing
        
        
        //         for (int k = 0; k < 3; k++) {
        //             velo[i][k] += acceleration[k] * timeStep;
        //         }
        //     }
        // }


}
