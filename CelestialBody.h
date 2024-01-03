#include <array>

using std::array;

#ifndef CELESTIAL_BODY
#define CELESTIAL_BODY

class CelestialBody{
    private:
        int mass;
        array<double, 3> position;
        array<double, 3> accel; 
        array<double, 3> velo; 
        array<double, 3> force; 

    public:
        CelestialBody(int new_mass, array<double, 3> new_position, array<double, 3> new_accel, array<double, 3> new_velo, array<double, 3> new_force);
        
        void setMass(int new_mass);
        
        int getMass();
        
        void setForce(array<double, 3> new_force);
        
        array<double, 3> getForce();
        
        void setPosition(array<double, 3> new_position);
        
        array<double, 3> getPosition();
        
        void setAccel(array<double, 3> new_accel);
        
        array<double, 3> getAccel();
        
        double CalcR(CelestialBody body2);
        
        double CalcForce(CelestialBody body2);
        
        array<double, 3> CalcCompF(CelestialBody body2);
        
        array<double, 3> CalcCompA(CelestialBody const body2);
};

#endif