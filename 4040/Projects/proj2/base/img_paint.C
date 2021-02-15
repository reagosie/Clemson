//------------------------------------------------
//
//  img_paint
//
//
//-------------------------------------------------




#include <cmath>
#include <omp.h>
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
   glDrawPixels( image.nx(), image.ny(), GL_RGB, GL_FLOAT, image.raw() );
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
        image.biasUp();
        cout << "increase bias\n";
        break;
     case 'b':
        image.biasDown();
        cout << "decrease bias\n";
        break;
     case 'c':
	      image.compliment();
	      cout << "Compliment\n";
        break;
     case 'C':
        image.rmsContrast();
        cout << "rms contrast\n";
        break;
     /*case 'F':
        image.flip();
        cout << "flipped\n";
        break;
     case 'f':
        image.flop();
        cout << "flopped\n";
        break;*/
     case 'G':
        image.gammaUp();
        cout << "increase gamma\n";
        break;
     case 'g':
        image.gammaDown();
        cout << "decrease gamma\n";
        break;
     case 'H':
        image.histogramEQ();
        cout << "histogram equalization\n";
        break;
     case 'o':
        image.writeOpenEXR();
        cout << "write to OpenEXR file\n";
        break;
	   case 'q':
        image.quantize();
        cout << "quantize\n";
        break;
     case 'S':
        cout << "pixel analysis values...\n";
        image.pixelAnalysis();
        break;
	   case 'V':
        image.brightnessUp();
        cout << "increase brightness\n";
        break;
	   case 'v':
        image.brightnessDown();
        cout << "decrease brightness\n";
        break;
	   case 'w':
        image.grayscale();
        cout << "grayscale\n";
	      break;
   }
}

void PrintUsage()
{
   cout << "img_paint keyboard choices\n";
   cout << "B/b       Bias up/down\n";
   cout << "C         RMS Contrast\n";
   cout << "c         Compliment\n";
   //cout << "F/f       flip/flop image\n";
   cout << "G/g       Gamma up/down\n";
   cout << "H         Histogram Equalization\n";
   cout << "o         Write to OpenEXR file\n";
   cout << "q         Quantize\n";
   cout << "S         Print pixel analysis values (min, max, avg, std dev)\n";
   cout << "V/v       Brightness up/down\n";
   cout << "w         Grayscale\n";
}


int main(int argc, char** argv)
{
   lux::CmdLineFind clf( argc, argv );

   setNbCores(8);

   string imagename = clf.find("-image", "", "Image to drive color");

   clf.usage("-h");
   clf.printFinds();
   PrintUsage();

   image.load(imagename);


   // GLUT routines
   glutInit(&argc, argv);

   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
   glutInitWindowSize( image.nx(), image.ny() );

   // Open a window
   char title[] = "img_paint";
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
