#include <experimental/random>
#include <chrono>
#include "src/BarnesHut.h"
#include "src/CelestialBody.h"
#include "src/ConfigParser.h"
#include "src/utils.h"

using std::vector;
using std::cout;
using std::endl;
using std::experimental::randint;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;


int main(){

    std::string folderName = "../output"; 
 
    createDirectory(folderName);

    //logging files for checking the variables
    std::string filePath = "../output/"+getCurrentDateTime("now")+".txt";

    // Create a file stream for writing to the new file
    std::ofstream logFile(filePath);

    //Parsing the config file and initializing constants
    ConfigParser parser = ConfigParser("../config.ini");
    
    const double BOUNDARY =  std::stod(parser.config["boundary"]);
    const double THETA =  std::stod(parser.config["theta"]);
    const double TIMESTEP =  std::stod(parser.config["timestep"]);
    const int NO_OF_BODIES =  std::stoi(parser.config["bodynumber"]);
    const int MASS_MIN =  std::stoi(parser.config["mass_minimum"]);
    const int MASS_MAX =  std::stoi(parser.config["mass_maximum"]);
    const int DURATION =  std::stoi(parser.config["duration"]);
    const bool BRUTEFORCE =  std::stoi(parser.config["bruteforce"]);

    logFile << "bodies: " << NO_OF_BODIES << endl;
    logFile << "theta: " << THETA << endl;
    logFile << "bruteforce: " << BRUTEFORCE << endl;
    
    
    //timing components of the code
    auto t1 = high_resolution_clock::now();
        //Generating the bodies and initializing the Barnes-Hut algorithm
        vector<CelestialBody> bodies = generateBodies(NO_OF_BODIES, BOUNDARY, MASS_MIN, MASS_MAX);
    auto t2 = high_resolution_clock::now();

    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;
        logFile << "time to generate bodies: " << ms_double.count() << "ms" << endl;

    BarnesHut tree;  

    //Calculating the force and acceleration for each body either brute force or using the algorithm
    if (BRUTEFORCE) { 
        t1 = high_resolution_clock::now();
            calculateForce(bodies);
        t2 = high_resolution_clock::now();

        /* Getting number of milliseconds as a double. */
        ms_double = t2 - t1;
            logFile << "time to calculate initial force using brute force: " << ms_double.count() << "ms" << endl; 
    }
    else {
        t1 = high_resolution_clock::now();
            tree = BarnesHut(std::make_unique<CelestialBody>(bodies[0]), BOUNDARY, THETA);
            for (size_t i = 1; i < bodies.size(); i++) tree.insert(std::make_unique<CelestialBody>(bodies[i]));
        t2 = high_resolution_clock::now();

        // cout << bodies.size() << " bodies" << endl;
        // dummy.traverseTree(tree.root);

        /* Getting number of milliseconds as a double. */
        ms_double = t2 - t1;
            logFile << "time to build tree: " << ms_double.count() << "ms" << endl;

        t1 = high_resolution_clock::now();
            for (size_t i = 0; i < bodies.size(); i++) bodies[i].setForce(tree.calculateForce(bodies[i], tree.root));
        t2 = high_resolution_clock::now();

        /* Getting number of milliseconds as a double. */
        ms_double = t2 - t1;
            logFile << "time to calculate initial force using algorithm: " << ms_double.count() << "ms" << endl;
    }
    calculateAcceleration(bodies);

    // The way we can complete the calculations:
    // take an infinitesimally small time increment to integrate over, dt.
    // to calculate the velocity of each body. v = int[bounded](a* dt) + v_0 if there exists any previous velocity
    // to calculate the position of each body. p = int[bounded](v * dt) + p_0 = int[bounded](a * t * dt) + p_0


    std::ofstream filestream ("../output/positions.csv"); //Filestream to write the positions of the bodies

    t1 = high_resolution_clock::now();

        for (int t = 0; t < DURATION; t++){ //for every time step
            for (size_t i = 0; i < bodies.size(); i++){ //for every body
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
            else for (size_t i = 0; i < bodies.size(); i++) bodies[i].setForce(tree.calculateForce(bodies[i], tree.root));
            calculateAcceleration(bodies);
        }
    t2 = high_resolution_clock::now();

    /* Getting number of milliseconds as a double. */
    ms_double = t2 - t1;
        logFile << "time to run simulation: " << ms_double.count() << "ms" << endl;

}
