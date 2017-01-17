#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <fcntl.h>

using namespace std;
using namespace cv;

Mat frame;

#define BORNE 1  //Définit la borne >0, si on la dépasse on sort de la fractale 
#define NBI 200 //détermine le nombre d'itérations maximal
#define HEIGHT 600// hauteur de l'écran 
#define WIDTH 600 //largeur de l'écran



std::complex<long double> julia(0.28545,0.01354);// représente le nombre complexe de julia


void calculeJulia(){ // Thread principal
  long double x,y,PX,PY,norm;
  int i,couleur;
  for( x=0;x<WIDTH;x++){
    for( y=0;y<HEIGHT/20;y++){//1ere colonne
      PX=((x-((long double)WIDTH/2))/((long double)WIDTH/2)); //pour ramener l'écran entre les valeurs -1 et 1
      PY=((y-((long double)HEIGHT/2))/((long double)HEIGHT/2));
      std::complex<long double> pixel(PY,PX);//représente le pixel sur l'écran d'affichage 
      i=0;
      norm = std::norm(pixel);
      while(i<NBI && norm<BORNE){// pour rester dans la fractale
	pixel=(pixel*pixel)+julia;//Calcul de la fractale 
	norm = std::norm(pixel);//rester dans la fractale 
	i++;
      }
      if(i==NBI && norm<BORNE){ //
	frame.at<Vec3b>(x,y)[2]=255;
	frame.at<Vec3b>(x,y)[1]=0;
	frame.at<Vec3b>(x,y)[0]=0;
      }
      else{
	couleur=(int)((long double)i/(long double)NBI*255);
	frame.at<Vec3b>(x,y)[1]=couleur/6;
	frame.at<Vec3b>(x,y)[0]=couleur/6;
	frame.at<Vec3b>(x,y)[2]=couleur;
      }
    }
  }
}

void* calcule (void* p){
  int id = *((int*) p);
  long double x,y,PX,PY,norm;
  int i,couleur;
  for( x=0;x<WIDTH;x++){
    for( y= id*(HEIGHT/10) ;y < (id+1)*(HEIGHT/10);y++){
      PX=((x-((long double)WIDTH/2))/((long double)WIDTH/2));
      PY=((y-((long double)HEIGHT/2))/((long double)HEIGHT/2)); //pour garder l'écran entre -1 et 1

      std::complex<long double> pixel(PY,PX);
      i=0;
      norm = std::norm(pixel);
      while(i<NBI && norm<BORNE){ //verifie qu'on est dans la fractale
	pixel=(pixel*pixel)+julia;
	norm = std::norm(pixel);//vérifie qu'on ne dépasse pas la borne 
	i++;
      }
      if(i==NBI && norm<BORNE){ //couleur à l'intérieur de la fractale (BGR)
	frame.at<Vec3b>(x,y)[0]=0;
	frame.at<Vec3b>(x,y)[1]=128;
	frame.at<Vec3b>(x,y)[2]=128;
      }
      else{
	couleur=(int)((long double)i/(long double)NBI*255);// couleur à l'exterieur de la fractale
	frame.at<Vec3b>(x,y)[0]=couleur/6;
	frame.at<Vec3b>(x,y)[1]=couleur/3;
	frame.at<Vec3b>(x,y)[2]=couleur;
      }
    }
  }
}



int main( int argc, char** argv ){
  pthread_t ID[8];
  int tab[8]={1,2,3,4,5,6,7,8};

  frame=cv::Mat(WIDTH, HEIGHT, CV_8UC3);

  for(int it=0;it<8;it++)
    pthread_create(&ID[it], NULL,calcule,(void*) &tab[it]);// on crée un thread et on lui passe en paramètre son num

  calculeJulia();

  for(int it=1;it<8;it++)
    pthread_join(ID[it], NULL);//attend la fin du thread

  while( true ) {
    imshow("fractal Julia", frame );
    if(cvWaitKey(30) > 0){
      break;
    }
  }

  return 0;
}
