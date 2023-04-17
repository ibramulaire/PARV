#ifndef DEF_VECTEUR
#define DEF_VECTEUR
#include<cmath>
struct Vecteur {
        double x;
        double y;
        double z;   

        double getNorme(int power)
        {
            return pow(sqrt(pow(x,2)+pow(y,2)+pow(z,2)),power);
           
        }

        Vecteur Normalise(int power)
        {
            double norme= pow(sqrt(pow(x,2)+pow(y,2)+pow(z,2)),power);
            return{x/norme,y/norme,z/norme};
        }

        double ProduitScalaire(Vecteur s2)
        {

            return (s2.x*x)+(s2.y*y)+(s2.z*z);
        }

        Vecteur ProduitVectoriele(Vecteur s2)
        {
            return {(y*s2.z)-(z*s2.y),(z*s2.x)-(x*s2.z),(x*s2.y)-(y*s2.x)};


        }

        Vecteur Moins(Vecteur s2)
        {
            return {x-s2.x,y-s2.y,z-s2.z};

        }

        Vecteur Plus(Vecteur s2)
        {
            return {x+s2.x,y+s2.y,z+s2.z};

        }

        Vecteur Foi(Vecteur s2)
        {
            return {x*s2.x,y*s2.y,z*s2.z};

        }
        
         Vecteur Foi(double k)
        {
            return {x*k,y*k,z*k};

        }
    }; 

#endif