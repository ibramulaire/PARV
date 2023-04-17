#ifndef DEF_SOMMET
#define DEF_SOMMET

#include<cmath>
struct Sommet {
        double x;
        double y;
        double z;   

        double getNorme(int power)
        {
            return pow(sqrt(pow(x,2)+pow(y,2)+pow(z,2)),power);
           
        }

        Sommet Normalise(int power)
        {
            double norme= pow(sqrt(pow(x,2)+pow(y,2)+pow(z,2)),power);
            return{x/norme,y/norme,z/norme};
        }

        double ProduitScalaire(Sommet s2)
        {

            return (s2.x*x)+(s2.y*y)+(s2.z*z);
        }

        Sommet ProduitVectoriele(Sommet s2)
        {
            return {(y*s2.z)-(z*s2.y),(z*s2.x)-(x*s2.z),(x*s2.y)-(y*s2.x)};


        }

        Sommet Moins(Sommet s2)
        {
            return {x-s2.x,y-s2.y,z-s2.z};

        }

        Sommet Plus(Sommet s2)
        {
            return {x+s2.x,y+s2.y,z+s2.z};

        }

        Sommet Foi(Sommet s2)
        {
            return {x*s2.x,y*s2.y,z*s2.z};

        }
        
         Sommet Foi(double k)
        {
            return {x*k,y*k,z*k};

        }
    }; 
#endif