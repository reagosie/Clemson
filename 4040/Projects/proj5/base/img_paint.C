//------------------------------------------------
//
//  img_paint
//
//
//-------------------------------------------------




#include <cmath>
#include <omp.h>
//#include "fftimgproc.h"
#include "imgproc.h"
#include "CmdLineFind.h"
#include <vector>



#include <GL/gl.h>   // OpenGL itself.
#include <GL/glu.h>  // GLU support library.
#include <GL/glut.h> // GLUT support library.


#include <iostream>
#include <stack>


using namespace std;
using namespace img;

ImgProc image;
int num_input_frames;
char** input_frames_list;
int movie_frame;



void setNbCores( int nb )
{
   omp_set_num_threads( nb );
}

void cbMotion( int x, int y )
{
}

void cbMouse( int button, int state, int x, int y )
{
}

void cbDisplay( void )
{
  glClear(GL_COLOR_BUFFER_BIT );
  //check how many channels
  if(image.depth()==3){//if 3 channels, call glDrawPixels with GL_RGB
    glDrawPixels( image.nx(), image.ny(), GL_RGB, GL_FLOAT, image.raw() );
  }else if(image.depth()==4){//if 4 channels, call glDrawPixels with GL_RGBA
    glDrawPixels( image.nx(), image.ny(), GL_RGBA, GL_FLOAT, image.raw() );
  }else{return;}
  glutSwapBuffers();
}

void cbIdle()
{
   glutPostRedisplay();
}

void cbOnKeyboard( unsigned char key, int x, int y )
{
  switch (key)
  {
    case 'B':
       //image.biasUp();
       cout << "increase bias\n";
       break;
    case 'b':
       //image.biasDown();
       cout << "decrease bias\n";
       break;
    case 'c':
       //image.compliment();
       cout << "Compliment\n";
       break;
    case 'C':
       //image.rmsContrast();
       cout << "rms contrast\n";
       break;
    case 'G':
       //image.gammaUp();
       cout << "increase gamma\n";
       break;
    case 'g':
       //image.gammaDown();
       cout << "decrease gamma\n";
       break;
    case 'H':
       //image.histogramEQ();
       cout << "histogram equalization\n";
       break;
    case 'o':
       //image.writeOpenEXR();
       cout << "write to OpenEXR file\n";
       break;
    case 'q':
       //image.quantize();
       cout << "quantize\n";
       break;
    case 'S':
       cout << "pixel analysis values...\n";
       //image.pixelAnalysis();
       break;
    case 'V':
       //image.brightnessUp();
       cout << "increase brightness\n";
       break;
    case 'v':
       //image.brightnessDown();
       cout << "decrease brightness\n";
       break;
    case 'w':
       //image.grayscale();
       cout << "grayscale\n";
       break;
    case '!':
       //image.doIFS(image);
       cout << "IFS started\n";
       break;
    case 'e':
       //image.doCoherentEstimation();
       cout << "Coherent Estimation\n";
       break;
    case 'L':
       //image.doLinearWaveEstimate();
       cout << "Linear Wave Estimate\n";
       break;
  }
}

void PrintUsage()
{
  // cout << "img_paint keyboard choices\n";
  // cout << "B/b       Bias up/down\n";
  // cout << "C         RMS Contrast\n";
  // cout << "c         Compliment\n";
  // cout << "G/g       Gamma up/down\n";
  // cout << "H         Histogram Equalization\n";
  // cout << "o         Write to OpenEXR file\n";
  // cout << "q         Quantize\n";
  // cout << "S         Print pixel analysis values (min, max, avg, std dev)\n";
  // cout << "V/v       Brightness up/down\n";
  // cout << "w         Grayscale\n";
  // cout << "!         one pass of fractal flames\n";
  // cout << "e         Coherent Estimation\n";
  cout << "Performing Linear Wave Estimate...\n";
}


int main(int argc, char** argv)
{
   lux::CmdLineFind clf( argc, argv );
   setNbCores(0);
   num_input_frames = argc - 1;
   input_frames_list = argv + 1;
   double alpha = 3.33;
   movie_frame = 99;
   LinearWaveEstimate* lwe = nullptr;

   for(int i = 1; i < argc; i++){
     string imagename(argv[i]);
     std::cout << "Loading " << imagename << "..." << std::endl;
     ImgProc input_image;
     input_image.load(imagename);
     if(lwe == nullptr){lwe = new LinearWaveEstimate(input_image, alpha);}
     lwe->ingest(input_image);
   }

   img::extract_image(*lwe, movie_frame, image);
   //image.writeOpenEXR();
   //std::cout << "Result written to OpenEXR file\n";


   // GLUT routines
   glutInit(&argc, argv);

   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
   glutInitWindowSize( image.nx(), image.ny() );

   // Open a window
   char title[] = "img_paint_lwe";
   glutCreateWindow( title );

   glClearColor( 1,1,1,1 );

   glutDisplayFunc(&cbDisplay);
   glutIdleFunc(&cbIdle);
   glutKeyboardFunc(&cbOnKeyboard);
   glutMouseFunc( &cbMouse );
   glutMotionFunc( &cbMotion );

   glutMainLoop();
   return 1;
};
