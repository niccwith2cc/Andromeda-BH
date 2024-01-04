#include <array>

using std::array;

#ifndef CELESTIAL_BODY
#define CELESTIAL_BODY

template <typename T>
std::array<T,3> operator/(const std::array<T,3>& vec, const double& s) {
    std::array<T> res(vec.size());
    for(int i=0;i<vec.size();++i)
        res[i] = vec[i] / s;
    return res;
}

template <typename T>
std::array<T,3> operator+(const std::array<T,3>& vec1, const std::array<T,3>& vec2) {
    if(vec1.size() == vec2.size()){
        std::array<T> res(vec1.size());
        for(int i=0;i<vec1.size();++i)
            res[i] = vec1[i] + vec2[i];
        return res;
    }
    return array<double>(3);
    // add exception
}

template <typename T>
bool operator==(const std::array<T,3>& vec1, const std::array<T,3>& vec2) {
    if(vec1.size() != vec2.size()) return false;
    for(int i=0;i<vec1.size();++i)
        if (vec1[i] != vec2[i]) return false;
    return true;
}

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