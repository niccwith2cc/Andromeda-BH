#include <vector>
#include <cmath>
#include <iostream>
#include <random>
#include <experimental/random>
#include "CelestialBody.cpp"
#include "TreeNode.cpp"
using std::vector;
using std::cout;

// Hardcoding the Galaxy by taking a fixed number of bodies and generating their random positions and masses

double generateRandomDouble(double min, double max) {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<double> dis(min, max);
        return dis(gen);
    };

vector<double> generateRandomPosition(){
    vector<double> position(3);
    for (int i = 0; i < 3; i++){
            position[i] = generateRandomDouble(-1000000.0,1000000.0);
        }   
    return position;
}

int main(){

    int bodynumber =  2;
    int mass;
    vector<double> position(3);
    vector<CelestialBody> bodies;

    //generating the bodies using the CelestialBody class and giving them random masses and positions
    for (int i = 0; i < bodynumber; i++){
        mass = experimental::randint(500000,1000000);
        position = generateRandomPosition(); //check it out later
        CelestialBody body = CelestialBody(mass, position, vector<double>(3), vector<double>(3), vector<double>(3));
        bodies.push_back(body);
    }

    //Initializing the forces and acceleration total sum being a nx3 matrix
    vector<vector<double>> Fsum (bodies.size(), vector<double> (3,0));
    vector<vector<double>> Asum (bodies.size(), vector<double> (3,0));

    //for loop to calculate
    for (int i = 0; i < bodies.size(); i++){
        for (int j = 0; j < bodies.size(); j++){
            if (&bodies[i] != &bodies[j]){
                vector<double> comp =  bodies[i].CalcCompF(bodies[j]);
                vector<double> acomp =  bodies[i].CalcCompA(bodies[j]);
                Fsum[i][0] += comp[0]; //the x components
                Fsum[i][1] += comp[1]; //the y components
                Fsum[i][2] += comp[2]; //the z components

                Asum[i][0] += acomp[0]; //the x components
                Asum[i][1] += acomp[1]; //the y components
                Asum[i][2] += acomp[2]; //the z components
            }
        }     
    }

    //visualization and printing
    cout << "The masses of the bodies: " << endl;
    for (int i = 0 ; i <bodies.size(); i++){
        cout << bodies[i].getMass() << " \t";
    }
        cout << endl;
        cout << endl;
        cout << "The Forces:" << endl ;
    for (int i = 0; i < bodies.size(); i++){
        for (int j = 0; j < 3; j++){
            cout << Fsum[i][j] << " \t";
        }
            cout << endl;
    }
    
        cout << endl;
        cout <<"The Acceleration:" << endl ;

    for (int i = 0; i < bodies.size(); i++){
        for (int j = 0; j < 3; j++){
            cout << Asum[i][j] << " \t";
        }
            cout << endl;
    }

        cout << endl;
        cout << "the positions: " << endl;
    for (int i = 0; i < bodies.size(); i++){
        cout << "for body " << i << ": ";
        for (int j = 0; j < 3; j++){
            cout << bodies[i].getPosition().at(j)<< " ";
        }
            cout << endl;
    }

        cout << endl;
        cout << "the position vectors: " ;;
    for (int i = 0; i < bodies.size(); i++){
        for (int j = 0; j < bodies.size(); j++){
            if (&bodies[i] != &bodies[j]){
            cout << bodies[i].CalcR(bodies[j])<< " ";
            }
            cout << endl;
        }
    }

        //Calculating speed and distance from acceleration
        //it is just taking the acceleration given and integrating over a specified time period, with additional intial velocity considered
        //for now we assume vo and so are 0.  
        // v = a*t + vo where t is the instant at which the acceleration is calculated.
        // s = a*t^2/2 + vo*t + s0 

        vector<double> time;
        const double timeStep = 10.0 / 100.0; // Calculate the time step size
        
        for (int i = 0; i < 101; i++) {
            double currentTime = i * timeStep;
            time.push_back(currentTime);
        }

        cout << "get accel" << "\n";

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
        //      for each time step{
            //      use setters to set the acceleration??
        vector<vector<vector<double>>> Aint (bodies.size(), vector<vector<double>> (3, vector<double> (time.size(),0.0)));
        vector<vector<vector<double>>> Vint (bodies.size(), vector<vector<double>> (3, vector<double> (time.size(),0.0)));
        vector<vector<vector<double>>> Pint (bodies.size(), vector<vector<double>> (3, vector<double> (time.size(),0.0)));

        
        for (int i = 0; i < bodies.size(); i++) {
            for (int k = 0; k < time.size(); k++) {
                Aint[i][0][k] = Asum[i][0];
                Vint[i][0][0] = Aint[i][0][0]*timeStep;
                Pint[i][0][0] = 0.5*Aint[i][0][0]*pow(timeStep,2);

                Aint[i][1][k] = Asum[i][1];
                Vint[i][1][0] = Aint[i][1][0]*timeStep;
                Pint[i][1][0] = 0.5*Aint[i][1][0]*pow(timeStep,2);

                Aint[i][2][k] = Asum[i][2];
                Vint[i][2][0] = Aint[i][2][0]*timeStep;
                Pint[i][2][0] = 0.5*Aint[i][2][0]*pow(timeStep,2);

                Vint[i][0][k+1] = Aint[i][0][k]*timeStep + Vint[i][0][k];
                // Vint[i][1][k] = Aint[i][1][k-1]*timeStep + Vint[i][1][k-1];
                // Vint[i][2][k] = Aint[i][2][k-1]*timeStep + Vint[i][2][k-1];

                Pint[i][0][k+1] = 0.5*Pint[i][0][k]*pow(timeStep,2) + Pint[i][0][k];
                // Pint[i][1][k] = 0.5*Pint[i][1][k-1]*pow(timeStep,2) + Pint[i][1][k-1];
                // Pint[i][2][k] = 0.5*Pint[i][2][k-1]*pow(timeStep,2) + Pint[i][2][k-1];

            }
        }

        cout << "trial" << '\n';
        for (int i = 0; i < bodies.size(); i++){
            for (int j = 0; j < 3; j++){
                for (int k = 0; k < time.size(); k++){
                    cout << Aint[i][j][k] << " ";
                }
                cout << '\n';
            }
            cout << '\n';
        }

        cout << "trial" << '\n';
        for (int i = 0; i < bodies.size(); i++){
            for (int j = 0; j < 3; j++){
                for (int k = 0; k < time.size(); k++){
                    cout << Vint[i][j][k] << " ";
                }
                cout << '\n';
            }
            cout << '\n';
        }

        cout << "trial" << '\n';
        for (int i = 0; i < bodies.size(); i++){
            for (int j = 0; j < 3; j++){
                for (int k = 0; k < time.size(); k++){
                    cout << Pint[i][j][k] << " ";
                }
                cout << '\n';
            }
            cout << '\n';
        }
        
        //         for (int k = 0; k < 3; k++) {
        //             velo[i][k] += acceleration[k] * timeStep;
        //         }
        //     }
        // }


}
