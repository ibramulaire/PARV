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


using namespace glm;
using namespace std;

//****************************************



mat4 rotationInterpolMatrice,rotationInterpolQuaternion;

quat q0 ,q1; // déclaration de quaternions

float t=0.0;

//****************************************



char presse;
int anglex,angley,x,y,xold,yold;

/* Prototype des fonctions */
void affichage();
void clavier(unsigned char touche,int x,int y);
void reshape(int x,int y);
void idle();
void mouse(int bouton,int etat,int x,int y);
void mousemotion(int x,int y);

void anim( int NumTimer) ;


void anim( int NumTimer)
{
  q0 =angleAxis ((float)radians(45.), vec3(1.,0.,0.));
q1=angleAxis((float)radians(90.), vec3(0.,1.,0.));

mat4 myMatriceRotation1 = mat4_cast(q0);

mat4 myMatriceRotation2 = mat4_cast(q1);
  
    using namespace std::chrono;
    static int i=0;
    static time_point<system_clock> refTime = system_clock::now()  ;

    time_point<system_clock> currentTime = system_clock::now(); // This and "end"'s type is std::chrono::time_point

    duration<double> deltaTime = currentTime - refTime;

    int delatTemps = duration_cast<milliseconds>( deltaTime).count() ;
   float t=((float)(delatTemps%1000)/1000);
    rotationInterpolQuaternion=mat4_cast( mix(q0,q1,t));
    rotationInterpolMatrice=((1-t)*myMatriceRotation1)+t*myMatriceRotation2;

    glutPostRedisplay();
    glutTimerFunc(100,anim,1 );

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

  /* enregistrement des fonctions de rappel */
  glutDisplayFunc(affichage);
  glutKeyboardFunc(clavier);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(mousemotion);
  glutTimerFunc(200, anim, 1);

  glMatrixMode( GL_PROJECTION );
     glLoadIdentity();
   gluPerspective(60 ,1,.1,30.);

  /* Entree dans la boucle principale glut */
  glutMainLoop();
  return 0;
}



void bras()
{


// a animer pas interpolation de quaternions
    glPushMatrix();
    glMultMatrixf(&rotationInterpolQuaternion[0][0]);
        glColor3f(1,0,0);
        glScalef(2,.2,.2);
        glTranslatef(.5,0.,0.);
        glutSolidCube(1.);
    glPopMatrix();

    // a animer par interpolation de matrice


glPushMatrix();
    
    glMultMatrixf(&rotationInterpolMatrice[0][0]);
        glColor3f(1,1,0);
        glScalef(2,.2,.2);
        glTranslatef(.5,0.,0.);
        glutSolidCube(1.);
    glPopMatrix();


    

   
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

        bras();

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

void clavier(unsigned char touche,int x,int y)
{
  switch (touche)
    {
    


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
