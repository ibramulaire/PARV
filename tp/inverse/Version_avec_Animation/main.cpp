/********************************************************/
/*                     cube.cpp                                                 */
/********************************************************/
/*                Affiche a l'ecran un cube en 3D                      */
/********************************************************/

/* inclusion des fichiers d'en-tete freeglut */

#ifdef __APPLE__
#include <GLUT/glut.h> /* Pour Mac OS X */
#else
#include <GL/glut.h>   /* Pour les autres systemes */
#endif 
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <armadillo>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/io.hpp>
#include <chrono>
#include <map>
#include"Sommet.h"
#include"Vecteur.h"

using namespace glm;
using namespace std;


//****************************************
#define NB_BRAS  4



//****************************************

struct Os
{
  float longeur;
  float angle;
  Vecteur axe;
  
  mat4 rotation;
  mat4 translation;


  vec3 couleur;
  vec3 position;
  void maj(float lo,float angl)
  {
    longeur=lo;
    angle=angl;
    quat q0;
    q0 =angleAxis ((float)radians(angl), vec3(0.,0.,1.));
    rotation=mat4_cast(q0);
    translation = glm::translate(glm::mat4(1.0), glm::vec3(lo/2, 0.0f, 0.0f));

  }


  void majangle(float angl)
  {
  
    angle=angl;
    quat q0;
    q0 =angleAxis ((float)radians(angl), vec3(0.,0.,1.));
    rotation=mat4_cast(q0);
   


  }
 
  void affiche()
  {
    glPushMatrix();
    
    
          glColor3f(couleur.x,couleur.y,couleur.z);
          glMultMatrixf(&translation[0][0]);
       
           glScalef(longeur,.2,.2);
          glutSolidCube(1.);
      glPopMatrix();

  }
  
};

struct Bras
{


   std::map<int, Os> bones; 
   int nb=0;
   mat4 rotation;
   mat4 translation;
   
   void add(int num,Os o)
   {
    bones[num]=o;
    nb++;
   }

   double getlongMax()
   {
      double res=0;
      for(int i=0;i<nb;i++)
      res+= bones[i].longeur;

    return res;
   }



arma::mat ComputeJacobienne(vec3 erreur)
{
   
    vec3 ai(0,0,1);

  arma::mat res=arma::mat(3,nb);
  for(int i=0;i<nb;i++)
  {
    vec3 temp=erreur-bones[i].position;

    Vecteur v=Vecteur{ai.x,ai.y,ai.z}.ProduitVectoriele(Vecteur{temp.x,temp.y,temp.z});
    res(0,i)=v.x;
    res(1,i)=v.y;
    res(2,i)=v.z;

  }
  return res;
 



}
vec3 computeEndPoint()
   {
    mat4 rotationslocal=mat4(1.0);
    mat4 translationlocal=mat4(1.0);
    vec4 monpoint(0.,0.,0.,1.);
       bones[0].position=vec3(0,0,0);
      for(int i=0;i<nb;i++)
        {
      
          Os c=bones[i];
          if(i>0)
          {
          vec4 position(bones[i-1].longeur,0,0,1);
          vec4 res= rotationslocal*position;
          translationlocal*=glm::translate(mat4(1.0), vec3(res));
           bones[i].position=vec3( translationlocal*monpoint);
          }

          rotationslocal*=c.rotation;
          
        }
        vec4 position(bones[nb-1].longeur,0,0,1);
        vec4 res= rotationslocal*position;
        translationlocal*=glm::translate(mat4(1.0), vec3(res));
        return vec3( translationlocal*monpoint);
   }
void maj(arma::vec V)
{
 for(int i=0;i<nb;i++)
        {
      
          bones[i].majangle(bones[i].angle+V(i));

          
        }


}

void computePOSITION()
   {
    mat4 rotationslocal=mat4(1.0);
    mat4 translationlocal=mat4(1.0);
    vec4 monpoint(0.,0.,0.,1.);
    bones[0].position=vec3(0,0,0);
      for(int i=0;i<nb;i++)
        {
      
          Os c=bones[i];
          if(i>0)
          {
          vec4 position(bones[i-1].longeur,0,0,1);
          vec4 res= rotationslocal*position;
          translationlocal*=glm::translate(mat4(1.0), vec3(res));
          bones[i].position=vec3( translationlocal*monpoint);
          }

          rotationslocal*=c.rotation;
          
        }
     
   }


   void setDroit(double angle)
   {
     bones [0].majangle(angle);
    for(int i=1;i<nb;i++)
        {
      
          bones[i].majangle(0.);
        }
       


   }
};





char presse;
bool mouseLeftDown;
bool mouseRightDown;
bool mouseMiddleDown;
float mouseX, mouseY;
float cameraAngleX;
float cameraAngleY;
float cameraDistance=0.;

float ang0=0.;
float ang1=-90.;
float ang2=90.;
float ang3=0.;
 vec3 cible(0.,2.,0.);
/* Prototype des fonctions */
void affichage();
void clavier(unsigned char touche,int x,int y);
void reshape(int x,int y);
void idle();
void mouse(int,int,int ,int );
void mouseMotion(int ,int );

Bras monbras;

void Creatbras()
{

  quat q0,q1,q2,q3;
  float l0=1.;
  float l1=2.;
  float l2=1.;
  float l3=1.;
 
   
  q0 =angleAxis ((float)radians(ang0), vec3(0.,0.,1.));
  q1 =angleAxis ((float)radians(ang1), vec3(0.,0.,1.));
  q2 =angleAxis ((float)radians(ang2), vec3(0.,0.,1.));
  q3 =angleAxis ((float)radians(ang3), vec3(0.,0.,1.));

  mat4 t0 = glm::translate(glm::mat4(1.0), glm::vec3(l0/2, 0.0f, 0.0f));
  mat4 t1 = glm::translate(glm::mat4(1.0), glm::vec3(l1/2, 0.0f, 0.0f));
  mat4 t2 = glm::translate(glm::mat4(1.0), glm::vec3(l2/2, 0.0f, 0.0f));
  mat4 t3 = glm::translate(glm::mat4(1.0), glm::vec3(l3/2, 0.0f, 0.0f));
  std::map<int, Os> bones;
  bones[0]={l0,ang0,{0,0,1},mat4_cast(q0),t0,vec3(1.,0.,0.)};
  bones[1]={l1,ang1,{0,0,1},mat4_cast(q1),t1,vec3(0.,1.,0.)};

  monbras={bones,2,mat4(1.0f),mat4(1.0f)} ;vec3(1.,0.,0.);

  monbras.add(2,{l2,ang2,{0,0,1},mat4_cast(q2),t2,vec3(0.,0.,1.)});
  monbras.add(3,{l3,ang3,{0,0,1},mat4_cast(q3),t3,vec3(1.,1.,0.)});



}





void Affichebras()
{

mat4 rotations=mat4(1.0);
mat4 translation=mat4(1.0);
  for(int i=0;i<monbras.nb;i++)
    {   
      Os c=monbras.bones[i];
      glPushMatrix();
      if(i>0)
      {
        vec4 position(monbras.bones[i-1].longeur,0,0,1);
        vec4 res= rotations*position;
        translation*=glm::translate(mat4(1.0), vec3(res));  
        glMultMatrixf(&translation[0][0]);   
      }
      rotations*=c.rotation;
      glMultMatrixf(&rotations[0][0]);  
      c.affiche();
      glPopMatrix();
    }

}



void calcule2(int NumTimer)
{
 
  double eps=0.001;
  float longmax=monbras.getlongMax();
  vec3 end=monbras.computeEndPoint();
  
  Vecteur target={cible.x,cible.y,cible.z};
  double angle=degrees(acos((target.ProduitScalaire({1.,0.,0.}))/target.getNorme(1)));

    vec3 erreur=cible-end;
    vec3 ai(0,0,1);
    

    if(Vecteur{erreur.x,erreur.y,erreur.z}.getNorme(1)>eps)
    {
      arma::vec3 E={erreur.x,erreur.y,erreur.z};
      arma::mat J=monbras.ComputeJacobienne(end);
      arma::mat Jplus=arma::pinv(J);
      arma::vec V=Jplus*E;
      
    
      if(arma::max(V)>2)
         V*=(2/arma::max(V));
       
      monbras.maj(V);
      end=monbras.computeEndPoint();

      erreur=cible-end;
    }
     glutPostRedisplay();
     glutTimerFunc(20,calcule2,1 );

}


  
//}

void affichage()
{
  int i,j;
  /* effacement de l'image avec la couleur de fond */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glShadeModel(GL_SMOOTH);
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  glTranslatef(0.,0.,-5.);
  glTranslatef(0,0,cameraDistance);
	glRotatef(cameraAngleX,1.,0.,0.)	;
	glRotatef(cameraAngleY,0.,1.,0.);

        
Affichebras();
 glPushMatrix();
      glTranslatef(cible.x,cible.y,cible.z);
            glPushMatrix();
                  glColor3f(1,0,0);
               
                  glScalef(0.02,.02,.02);
                  glutSolidSphere(1,20,20);
            glPopMatrix();

            glPushMatrix();
                  glBegin(GL_LINES);
                glColor3f(1.0,0.0,0.0);
              glVertex3f(-0.2, 0,0.0);
              glVertex3f(0.2, 0,0.0);
            glEnd();
            //axe des y en vert
            glBegin(GL_LINES);
              glColor3f(0.0,1.0,0.0);
              glVertex3f(0, -0.2,0.0);
              glVertex3f(0, 0.2,0.0);
            glEnd();
          
           glPopMatrix();

glPopMatrix();
    //Repère
    //axe x en rouge
    glBegin(GL_LINES);
        glColor3f(1.0,0.0,0.0);
    	glVertex3f(0, 0,0.0);
    	glVertex3f(1, 0,0.0);
    glEnd();
    //axe des y en vert
    glBegin(GL_LINES);
    	glColor3f(0.0,1.0,0.0);
    	glVertex3f(0, 0,0.0);
    	glVertex3f(0, 1,0.0);
    glEnd();
    //axe des z en bleu
    glBegin(GL_LINES);
    	glColor3f(0.0,0.0,1.0);
    	glVertex3f(0, 0,0.0);
    	glVertex3f(0, 0,1.0);
    glEnd();

  glFlush();
  
  //On echange les buffers 
  glutSwapBuffers();
}

int main(int argc,char **argv)
{
  /* initialisation de glut et creation
     de la fenetre */
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(200,200);
  glutInitWindowSize(1500,1500);
  glutCreateWindow("cube");

  /* Initialisation d'OpenGL */
  glClearColor(0.0,0.0,0.0,0.0);
  glColor3f(1.0,1.0,1.0);
  glPointSize(10.0);
  glEnable(GL_DEPTH_TEST);

  
   Creatbras();


  /* enregistrement des fonctions de rappel */
  glutDisplayFunc(affichage);
  glutKeyboardFunc(clavier);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(mouseMotion);
  glutTimerFunc(20, calcule2, 1);

  glMatrixMode( GL_PROJECTION );
     glLoadIdentity();
   gluPerspective(60 ,1,.1,1000.0f);

  /* Entree dans la boucle principale glut */
  glutMainLoop();
  return 0;
}







void clavier(unsigned char touche,int x,int y)
{
  switch (touche)
    {
      case '&':
        cible.x-=0.1;
      
         glutPostRedisplay();
        break;
        case '1':
          cible.x+=0.1;
        glutPostRedisplay();
        break;

        case '"':
        cible.y-=0.1;
      
         glutPostRedisplay();
        break;
        case '3':
          cible.y+=0.1;
        glutPostRedisplay();
        break;
        





    case 'a':
    ang0-=2.;
    monbras.bones[0].majangle(ang0);
    glutPostRedisplay();
    break;
    case 'A':
    ang0+=2.;
    monbras.bones[0].majangle(ang0);
    glutPostRedisplay();
    break;


    case 'z':
    ang1-=2.;
    monbras.bones[1].majangle(ang1);
    glutPostRedisplay();
    break;
    case 'Z':
    ang1+=2.;
    monbras.bones[1].majangle(ang1);
    glutPostRedisplay();
    break;

    case 'e':
    ang2-=2.;
    monbras.bones[2].majangle(ang2);
    glutPostRedisplay();
    break;
    case 'E':
    ang2+=2.;
    monbras.bones[2].majangle(ang2);
    glutPostRedisplay();
    break;
  
  


  case 'p': /* affichage du carre plein */
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glutPostRedisplay();
      break;
    case 'f': /* affichage en mode fil de fer */
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glutPostRedisplay();
      break;
    case 's' : /* Affichage en mode sommets seuls */
      glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
      glutPostRedisplay();
      break;
    case 'd':
      glEnable(GL_DEPTH_TEST);
      glutPostRedisplay();
      break;
    case 'D':
      glDisable(GL_DEPTH_TEST);
      glutPostRedisplay();
      break;
    case 'q' : /*la touche 'q' permet de quitter le programme */
      exit(0);
    }
}

void reshape(int x,int y)
{
  if (x<y)
    glViewport(0,(y-x)/2,x,x);
  else 
    glViewport((x-y)/2,0,y,y);
}

void mouse(int button, int state, int x, int y)
{
    mouseX = x;
    mouseY = y;

    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseLeftDown = true;
        }
        else if(state == GLUT_UP)
            mouseLeftDown = false;
    }

    else if(button == GLUT_RIGHT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseRightDown = true;
        }
        else if(state == GLUT_UP)
            mouseRightDown = false;
    }

    else if(button == GLUT_MIDDLE_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseMiddleDown = true;
        }
        else if(state == GLUT_UP)
            mouseMiddleDown = false;
    }
}


void mouseMotion(int x, int y)
{
    if(mouseLeftDown)
    {
        cameraAngleY += (x - mouseX);
        cameraAngleX += (y - mouseY);
        mouseX = x;
        mouseY = y;
        
    }
    if(mouseRightDown)
    {
        cameraDistance += (y - mouseY) * 0.2f;
        mouseY = y;
    }

    glutPostRedisplay();
}