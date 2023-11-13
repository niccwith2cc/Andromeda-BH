#include <vector>
#include <cmath>

using namespace std;
const double  G = 6.6740105e-11;

class CelestialBody{
    private:
        double mass;
        vector<double> position = vector<double>(3);
        vector<double> accel = vector<double>(3);
        vector<double> velo = vector<double>(3);
        vector<double> force = vector<double>(3);

    public:

        CelestialBody(double new_mass, vector<double> new_position, vector<double> new_accel, vector<double> new_velo, vector<double> new_force){
            mass = new_mass;
            position = new_position;
            accel = new_accel;
            velo = new_velo;
            force = new_force;
        }

        void setMass(double new_mass){
            mass = new_mass;
        }

        double getMass(){
            return mass;
        }   

        void setForce(vector<double> new_force){
            force = new_force;
        }

        vector<double> getForce(){
            return force;
        }

        void setPosition(vector<double> new_position){
            position = new_position;
        }

        vector<double> getPosition(){
            return position;
        }

        double CalcR(CelestialBody body2){
            vector<double> position2 = body2.getPosition();
            return sqrt(pow(position2[0]-position[0],2)+pow(position2[1]-position[1],2)+pow(position2[2]-position[2],2));
        }

        //note that whenever the planets are in the same plane as each other, 2 components will be 0 and cause the force to go to inf

        // an alternative solution would be to take the absolute norm between the two planets and calculate the total force between them
        // once the total force is calculated, the components can be extracted for use to calculate the summation force for all the bodies acting
        // on the that body.

        // just in case {mass*body2.getMass()*G*(position2[0]-position[0])/pow((position2[0]-position[0]),3),mass*body2.getMass()*G*(position2[1]-position[1])/pow((position2[1]-position[1]),3),mass*body2.getMass()*G*(position2[2]-position[2])/pow((position2[2]-position[2]),3)};

        double CalcForce(CelestialBody body2){
            vector<double> position2 = body2.getPosition();
            double instantForce = G*mass*body2.getMass()/(pow(CalcR(body2),2));
            vector<double> componentForce = {(position2[0]-position[0])*instantForce/CalcR(body2),(position2[1]-position[1])*instantForce/CalcR(body2),(position2[2]-position[2])*instantForce/CalcR(body2)};
            return instantForce;
        }
};