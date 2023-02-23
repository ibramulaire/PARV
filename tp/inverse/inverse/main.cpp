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
//#include <armadillo>
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
    
    
          glColor3f(1,0,0);
          glMultMatrixf(&translation[0][0]);
         // glTranslatef(longeur/2,0,0);
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

};


char presse;
int anglex,angley,x,y,xold,yold;



float ang0=90.;
float ang1=90.;
float ang2=90.;

/* Prototype des fonctions */
void affichage();
void clavier(unsigned char touche,int x,int y);
void reshape(int x,int y);
void idle();
void mouse(int bouton,int etat,int x,int y);
void mousemotion(int x,int y);

Bras monbras;

void Creatbras()
{

  quat q0,q1,q2;
  float l0=1.;
  float l1=2.;
  float l2=1.;

 
   
  q0 =angleAxis ((float)radians(ang0), vec3(0.,0.,1.));
  q1 =angleAxis ((float)radians(ang1), vec3(0.,0.,1.));
  q2 =angleAxis ((float)radians(ang2), vec3(0.,0.,1.));

  mat4 t0 = glm::translate(glm::mat4(1.0), glm::vec3(l0/2, 0.0f, 0.0f));
  mat4 t1 = glm::translate(glm::mat4(1.0), glm::vec3(l1/2, 0.0f, 0.0f));
  mat4 t2 = glm::translate(glm::mat4(1.0), glm::vec3(l2/2, 0.0f, 0.0f));
  std::map<int, Os> bones;
  bones[0]={l0,ang0,{0,0,1},mat4_cast(q0),t0};
  bones[1]={l1,ang1,{0,0,1},mat4_cast(q1),t1};

  monbras={bones,2,mat4(1.0f),mat4(1.0f)} ;

  monbras.add(2,{l2,ang2,{0,0,1},mat4_cast(q2),t2});





 //q0 =angleAxis ((float)radians(45.), vec3(1.,0.,0.));
 //b.rotation=mat4_cast(q0);


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
    
      //c.posdb={res.x,res.y,res.z};
      translation*=glm::translate(mat4(1.0), vec3(res));
       
    
     glMultMatrixf(&translation[0][0]);  
    
      }

      rotations*=c.rotation;
      glMultMatrixf(&rotations[0][0]);  
      c.affiche();
      glPopMatrix();

    }



}

void affichage()
{
  int i,j;
  /* effacement de l'image avec la couleur de fond */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glShadeModel(GL_SMOOTH);
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  glTranslatef(0.,0.,-5.);
  glRotatef(angley,1.0,0.0,0.0);
  glRotatef(anglex,0.0,1.0,0.0);

        Affichebras();

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
  glutMotionFunc(mousemotion);
 // glutTimerFunc(200, anim, 1);

  glMatrixMode( GL_PROJECTION );
     glLoadIdentity();
   gluPerspective(60 ,1,.1,30.);

  /* Entree dans la boucle principale glut */
  glutMainLoop();
  return 0;
}







void clavier(unsigned char touche,int x,int y)
{
  switch (touche)
    {
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
  
  
  case '2':
//  orientation[1]+=.5;
  glutPostRedisplay();
  break;
//  case 'é':
//  orientation[1]+=.5;
//  glutPostRedisplay();
//  break;

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

void mouse(int button, int state,int x,int y)
{
  /* si on appuie sur le bouton gauche */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
  {
    presse = 1; /* le booleen presse passe a 1 (vrai) */
    xold = x; /* on sauvegarde la position de la souris */
    yold=y;
  }
  /* si on relache le bouton gauche */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
    presse=0; /* le booleen presse passe a 0 (faux) */
}

void mousemotion(int x,int y)
  {
    if (presse) /* si le bouton gauche est presse */
    {
      /* on modifie les angles de rotation de l'objet
	 en fonction de la position actuelle de la souris et de la derniere
	 position sauvegardee */
      anglex=anglex+(x-xold); 
      angley=angley+(y-yold);
      glutPostRedisplay(); /* on demande un rafraichissement de l'affichage */
    }
//    else
//    {
//        pointCible.x= ;
//        pointCible.y= ;
//    }

    xold=x; /* sauvegarde des valeurs courante de le position de la souris */
    yold=y;
  }
