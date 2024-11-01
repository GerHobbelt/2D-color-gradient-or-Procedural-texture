/*



  =============================
  gcc d.c -Wall -lm

  ./a.out 

  


  ===================
c program creates nMax 







twist
X = x.cos(angle) + z.sin(angle)
Z = -x.sin(angle) + z.cos(angle)
https://blender.stackexchange.com/questions/78379/how-to-twist-a-procedural-material
----------------

noise

https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/procedural-texturing

https://docs.bentley.com/LiveContent/web/MicroStation%20Help-v13/en/GUID-AC1EEF76-9118-FE2E-32F6-399AF454E86C.html

https://github.com/cpetry/TextureGenerator-Online

http://www.upvector.com/?section=Tutorials&subsection=Intro%20to%20Procedural%20Textures

https://gamedevacademy.org/complete-guide-to-procedural-level-generation-in-unity-part-1/

https://github.com/simongeilfus/SimplexNoise

https://github.com/stegu/perlin-noise


https://github.com/sol-prog/Perlin_Noise

https://gitlab.com/drummyfish/ptdesigner/-/tree/master/



https://commons.wikimedia.org/wiki/File:XOR_texture.png


sharp
https://stackoverflow.com/questions/7295235/image-sharpening-methods

----------------

https://ux.uis.no/~tranden/brodatz.html
http://sipi.usc.edu/database/database.php?volume=textures#top
http://multibandtexture.recherche.usherbrooke.ca/normalized_brodatz_more.html


*/

#define _USE_MATH_DEFINES    // to make sure MSVC will load the M_PI define, among others

#include <stdio.h>
#include <string.h> // strncat
#include <stdlib.h> // malloc
#include <math.h> // log10
#include <complex.h> 		// complex numbers : https://stackoverflow.com/questions/6418807/how-to-work-with-complex-numbers-in-c


double TwoPi=2.0*M_PI;

// color = RGB =  in the order red, green, blue
// color component ( channel) = double number from 0.0 to 1.0 

double  DisplayAspectRatio  = 1.0; // https://en.wikipedia.org/wiki/Aspect_ratio_(image)

// size of virtual 2D array of pixels
// each pixel has a RGB color
int iWidth ; // 
int iHeight = 600; // 




// see SetZPlane






// complex plane 
double ZxMin = -1.0;	//-0.05;
double ZxMax = 1.0;	//0.75;
double ZyMin = -1.0;	//-0.1;
double ZyMax = 1.0;	//0.7;


double PixelWidth;	// =(ZxMax-ZxMin)/ixMax;
double PixelHeight;	// =(ZyMax-ZyMin)/iyMax;
double ratio;







// size of the dynamic 1D array 
unsigned char * data;
size_t ColorSize = 3; // RGB = number of color components = channels
size_t ArrayLength; // number of 1D array's elements = ENumber = iWidth*iHeight*ColorSize
size_t ElementSize; // size of array's element in bytes
size_t ArraySize; // size of array in bytes = ElementSize*ArrayLength 

// ppm P6 file
size_t HeaderSize ; // size of the P6 file  header in bytes
size_t FileSize; // = HeaderSize +ArraySize [bytes]



int nMax; // =  sizeof(titles)/sizeof(titles[0]); // 18; // see GiveColor function, it should be one more then max n in GiveColor
  

// add new title here and new function to GiveColor
// titles = "Rainbow Linas Magma GrayL GrayNL2 GrayNL3 GraySqrt Green NewLinas CoolWarm GrayGamma GrayNL3Wave2 GrayNL3Wave10 GraySqrtWave ColorLWave ColorLWaveInverted NL3Wave5NonInv Cubehelix"
const char *titles[] = {"max", "min", "product" , "maxabs", "minsqrt", "sqrtp", "powxy", "pow", "sin", "sinxy", 
			"checker", "star8", "star8i", "cabs", "cabsi", "cargm", "cturn", "conic", "himmelblau", "sinxpy", 
			"repeatingp", "sincosin", "sinx", "checkersin", "cartesian_grid", "polar_grid", "polar_grid2", "carg"
			};



// ------------------------ functions ----------------------------------------------------------------------




// gives position of 2D point (ix,iy) in 1D array  ; uses also global variables: iWidth , ColorSize
int Give_i ( int iX, int iY)
{
  return (iX + iY * iWidth) * ColorSize;  
}




 // from screen to world coordinate ; linear mapping
// uses global cons
double GiveZx ( int ix)
{
  return (ZxMin + ix * PixelWidth);
}

// uses globaal cons
double GiveZy (int iy) {
  return (ZyMax - iy * PixelHeight);
}				// reverse y axis


complex double GiveZ( int ix, int iy){
  double Zx = GiveZx(ix);
  double Zy = GiveZy(iy);
	
  return Zx + Zy*I;
	
	


}







// **********************************************************************************************************************

/**
 * Find maximum between two numbers.
 https://codeforwin.org/2016/02/c-program-to-find-maximum-and-minimum-using-functions.html
 */
double max(double n1, double n2)
{
    return (n1 > n2 ) ? n1 : n2;
}





//---------------------

double min(double n1, double n2)
{
    return (n1 < n2 ) ? n1 : n2;
}


double clip(double d){

	return (d> 1.0) ? 1.0 : d;
}


// clamp d to [0,1] range
double clamp(const double d){
	
	double r = d;
	if (r>1.0) {r = 1.0;}
	if (r<0.0) {r = 0.0;}
	return r;

}



// fractional part of real (double) number
double frac(double d){

	double fraction = d - ((long)d);
	return fraction;
}




/* 

output of carg is: 
-  argument also called phase angle) of complex number z, with a branch cut along the negative real axis.
- in radians from -pi to pi

modification translates range from [-pi, pi] to  [0 ,2*pi]

*/
double cargm(double complex z)
{
	double argument;
  	argument = carg(z); //   argument in radians from -pi to pi
  
  	if (argument<0.0) 
    		{argument = TwoPi + argument ; }//   argument in radians in [0 ,2*pi]
    	argument /= TwoPi  ; //   argument in radians from 0 to 1
    	argument *= 5.0; // repeating gradient
     
  	return argument ; // argument in radians from 0.0 to 2*Pi
}






double cturn(double complex z)
{
  double argument;
  
  argument = carg(z); //   argument in radians from -pi to pi
  
  if (argument<0.0) 
    argument = TwoPi + argument ; //   argument in radians from 0 to 2*pi
     
  return (argument/TwoPi) ; // argument in turns from 0.0 to 1.0
}




/*

https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/procedural-texturing
fraktal.republika.pl/grid.html


https://iquilezles.org/www/articles/checkerfiltering/checkerfiltering.htm

checkers, in mod form
*/
double checker( double x, double y){

	double m = 5.0; 
	// map input from [-1,1]x[-1,1] to [-m,m]x[-m,m]
	int ix = floor(m*x);
	int iy = floor(m*y);


	return abs(ix + iy) % 2;


}

// give 
double grid(const  double d, const double grid_width){

	
	double g = fabs(d);
	g = frac(g);
	if (g >  1.0-grid_width)
		{g = 1.0- g;}
		else { if (g > grid_width)
				{g = 1.0;}}
		
	return g;	
	


}



double Give_cartesian_grid( double x, double y){

	double grid_width = 0.05;
	// mapped input from [-1,1]x[-1,1] to [-m,m]x[-m,m]
	double m = 10.0; // 
	double gx = grid(m*x, grid_width);
	double gy = grid(m*y, grid_width);
	double g = (gx + gy)/2.0; 


	return g ;


}


double Give_polar_grid( double x, double y){

	double grid_width = 0.05;
	double m = 10.0; // 
	complex double z = x+ y*I;
	
	double r = m*cabs(z);
	// mapped input from [0,1] to [0,m]
	double t = m*cturn(z);
	
	double gr = grid(r, grid_width);
	
	// The radial grid lines have noticeable changes in the width 
	double gt = grid(t, grid_width);

	double g = ( gr + gt)/2.0; 


	return g ;


}

double Give_polar_grid2( double x, double y){

	double grid_width = 0.05;
	double m = 10.0; // 
	complex double z = x+ y*I;
	
	double r = m*cabs(z);
	// mapped input from [0,1] to [0,m]
	double t = m*cturn(z);
	
	double gr = grid(r, grid_width);
	
	// The radial grid lines have noticeable changes in the width 
	// arc length of the circle depends on r
	// grid_width = grid_width - grid_width*tan(r/m)
	double gt = grid(t, grid_width/r);

	double g = ( gr + gt)/2.0; 


	return g ;


}



// not works
double checkersin( const double x, const double y){
//domain runs from -p to p in the real part and 0 to 2*pi in the imaginary part. 

	complex double z = M_PI*x+ M_PI*(1.0+y)*I;
	z = sin(z);
	
	
	int ix = floor(cabs(z));
	int iy = floor(carg(z));


	return abs(ix + iy) % 2;


}




/* 
 r is the smooth potential and phi is the final angle
 code by xenodreambuie : "I call this texture pyramids. My code in Pascal for the Star8 texture is "
 https://fractalforums.org/programming/11/how-many-different-ways-are-there-to-show-such-set/3874/msg25389#msg25389
 
*/ 

double GiveStar8(double r, double phi){
	double fr;
	double fphi;
	double t;
	double g;
	
	fr = fabs(frac(r));
  	fphi = fabs(frac(phi));
  	if (fphi>fr) {
    		t= fr; 
    		fr= fphi; 
    		fphi=t;
    		}
  
	g = 1+1.5*fphi-2.5*fr;
	t = 1-2.5*fphi-fr;
	if (t> g) 
	  	{ g = t;}
	if (g<0)
  		{g=0;}
  	
	return g;
}





double conic(double complex z)
{
  double argument;
  
  argument = carg(z); //   argument in radians from -pi to pi
  
  argument = fabs(argument)/ M_PI;
      
  return argument; // argument in range from 0.0 to 1.0
}







// https://en.wikipedia.org/wiki/Himmelblau%27s_function
double GiveHimmelblau(double x, double y){
	// mapped input to [-m,m]x[-m,m]
	double m = 6.0;
	x *= m;
	y *= m;
	double a = x*x+y-11.0;
	double b = x+y*y-7.0;
	// mapped output to 
	 return (a*a + b*b)/200.0;

}




/* 1d wave */
double GiveLWaveInverted(const double position){
	
  int segments=4;
  double d = fabs(segments*position);
  /* if position > 1 then we have repetition of colors it maybe useful    */
  if (d >1.0)
    {	int p = (int)d;
      d = d-p; // fractional part 
      if (p % 2)
	{d  = 1.0- d;} // reverse gradient
    }
   return d; // in [0.1] range

}

double GiveSine(const double position){
	
	// the commensts by P Kovesi
	// my values are slightly changed to get similar visual effect
	double angle = position*2.0*M_PI; // input of the sin function is an angle in radians in range [0, 2*Pi]
	//double f = iWidth/4.0; // frequency: The wavelength of the sine wave is set at 8 pixels 
	double s = (1.0+ sin(angle))/2.0; // wave range from [-1,1] to [0,1]
  	
  	return fabs(s);
  	
	

}


double GiveCosine(const double position){
	
	// the comments by P Kovesi
	// my values are slightly changed to get similar visual effect
	double angle = position*2.0*M_PI; // input of the sin function is an angle in radians in range [0, 2*Pi]
	//double f = iWidth/4.0; // frequency: The wavelength of the sine wave is set at 8 pixels 
	double s = (1.0+cos(angle))/2.0; // wave range from [-1,1] to [0,1]
  	
  	return fabs(s);
  	
	

}





/*

RepeatingP


*/
double GiveRepeatingP(double x, double y){
	// mapped input to [-6,6]x[-6,6]
	double a = 10.0*x;
	double b = 60.0*y;
	a = GiveLWaveInverted(a);
	b =  GiveSine(b);
	// mapped output to 
	 return (a + b)/2.0;

}





double Give_sincosin(double x, double y){
	// mapped input from [-1,1]x[-1,1] to [-m,m]x[-m,m]
	double m = 4.0;
	double a = m*x;
	double b = m*y;
	a = GiveSine(a);
	b = GiveCosine(b);
	// mapped output from [0,2] to [0,1] range
	 return (a + b)/2.0;

}



double Give_sinx(const double x, const double y){
	// mapped input to [-6,6]x[-6,6]
	double a = x;
	//double b = y;
	a = GiveSine(a);
	//b = 1.0-GiveCosine(b);
	// mapped output to 
	 return a;

}






/* 
   remember to update : 
   *  add new title in titles array in the proper order
   * titles in plot.gp
   
   
   to add
   https://www.iquilezles.org/www/articles/distfunctions2d/distfunctions2d.htm
   
   
   2D Gray gradient  = 2D gray texture
   
*/

double GiveGray(const double x, const double y, const int n, double rgb[]){

	
	double d; 
	
	
  	switch(n){
	
  		case 0: {d =  max(fabs(x),fabs(y)); break;} // max
  	
  		case 1: {d =  1.0 - min(fabs(x),fabs(y)); ; break;} // min 
  		
  		case 2 : {d = clip(50.0*fabs(x*y)); break;} // product
  		
  		case 3 : {d = min( clip(50.0*fabs(x*y)),  max(fabs(x),fabs(y)) ); break;} // maxabs
  		
  		case 4 : {d =  min( sqrt(fabs(x)), sqrt(fabs(y)) ); break;} //minsqrt 
  		
  		case 5 : {d =  sqrt(fabs(x*y)); break;} //sqrtp 
  		
  		case 6 : {d =  clip(pow(fabs(x),fabs(y))); break;} //powxy 
  		
  		case 7 : {d =  min( clip(pow(fabs(x),fabs(y))), clip(pow(fabs(y),fabs(x))) ); break;} //pow 
  		
  		// http://www.upvector.com/?section=Tutorials&subsection=Intro%20to%20Procedural%20Textures
  		case 8 : {d =  (1.0 + sin(x* 2.0* M_PI*3.0 ))/2.0; break;} // sin
  		
  		case 9 : {d =  (1.0 + sin(x*y* 2.0* M_PI*3.0 ))/2.0; break;} // sinxy
  		
  		case 10 : {d =  checker(x,y); break;} // checker
  		
  		case 11 : {d =  GiveStar8(x,y); break;} // star8
  		
  		case 12 : {d =  1.0 - GiveStar8(x,y); break;} // star8i 
  		
  		case 13 : {d =  clamp(cabs(x+y*I));  break;} // cabs
  		
  		case 14 : {d =  clamp(1.0 - cabs(x+y*I)); break;} //  cabsi
  		
  		case 15 : {d =  cargm(x+y*I); break;} //  cargm = modified carg
  		
  		case 16 : {d =  cturn(x+y*I); break;} //  cturn
  		
  		case 17 : {d =  conic(x+y*I); break;} //  conic
  		
  		case 18 : {d =  GiveHimmelblau(x,y); break;} //  Himmelblau
  		
  		case 19 : {d =  (1.0 + sin(x+y* 2.0* M_PI ))/2.0; break;} // sinxpy
  
  		case 20 : {d = GiveRepeatingP(x, y); break;}
  			
  		case 21 : {d = Give_sincosin(x, y); break;}
  		
  		case 22 : {d = Give_sinx(x, y); break;}
  		
  		case 23 : {d = checkersin(x,y); break;}
  		
  		case 24 : {d = Give_cartesian_grid(x,y); break;}
  		
  		case 25 : {d = Give_polar_grid(x,y); break;}
  		
  		case 26 : {d = Give_polar_grid2(x,y); break;}
  		
  		case 27 : {d = carg(x+y*I); break;}  // carg ( not modified)
  
  
  		default:{}
  	}
	
	
  	return d;


}







int GiveGrayColor(const complex double z, const int n, double rgb[]){

	// rectangle cell [-1,1]x[-1,1] 
 
	
	//complex double center = 0 + 0*I;
	double x =  creal(z);
	double y =  cimag(z);
	
	double d = GiveGray(x, y, n, rgb); // compute shade of gray
	// save color to the output array rgb
	rgb[0] = d;
	rgb[1] = d;
	rgb[2] = d;
 
  
  	
  return 0;


}





int PlotPoint(unsigned char A[], int iX, int iY, int n){


	int i = Give_i(iX, iY);
	complex double z = GiveZ(iX, iY);
	double rgb[3];
	GiveGrayColor( z, n, rgb); // compute color rgb


	// save color to image array A
  	// conversion from [0,1] range to [0,255]
  	A[i]   = (unsigned char) (255*rgb[0]); /* Red*/
  	A[i+1] = (unsigned char) (255*rgb[1]); /* Green */
  	A[i+2] = (unsigned char) (255*rgb[2]);  /* Blue */
  
  	return 0;


}




/* 
 each pixel is computed "independently"
*/
int FillArray (unsigned char A[] , int n){

  int iX;
  int iXmax = iWidth-1;
  int iY;
  int iYmax = iHeight-1;
  
  //
  for(iX=0; iX<iXmax; ++iX)    
  	for(iY=0; iY<iYmax; ++iY){
      		PlotPoint(A, iX, iY, n); }
        		
         			
  


  
  return 0;
}





// --------------- save dynamic "A" array of unsigned char to the binary ppm file ( P6 ) --------------------------------
int SaveArray2PPM (unsigned char A[], size_t ASize,   int n)
{

  FILE *fp;
  const unsigned char MaxColorComponentValue = 255;	/* color component is coded from 0 to 255 ;  it is 8 bit color file */
  
  
  char name [100]=""; /* name of file */
  strncat(name, titles[n], strlen(titles[n])); /*  */
  char *filename = strcat(name, ".ppm");
  
  
  
  

  /* save image to the pgm file  */
  fp = fopen (filename, "wb");	/*create new file,give it a name and open it in binary mode  */
  if (fp == NULL) 
    {	printf("File open error"); 
      return 1;}
  else {
    fprintf (fp, "P6\n%u %u\n%u\n", iWidth, iHeight, MaxColorComponentValue);	/*write header to the file */
    fwrite (A, ASize, 1, fp);	// write dynamic A array to the binary file in one step  
 
    printf ("File %s saved. \n", filename);
    fclose (fp);
    return 0;}
  
}



// n = nummber of the gradient function
int MakeGradientImage(unsigned char A[],  int n){


  FillArray(A, n);
  SaveArray2PPM(A, ArraySize, n);
  return 0;

}



int setup(){


	nMax =  sizeof(titles)/sizeof(titles[0]); // 18; // see GiveColor function, it should be one more then max n in GiveColor
  

  iWidth = iHeight*DisplayAspectRatio ; 
  
  /* Pixel sizes */
  PixelWidth = (ZxMax - ZxMin) / (iWidth -1);	//  ixMax = (iWidth-1)  step between pixels in world coordinate 
  PixelHeight = (ZyMax - ZyMin) / (iHeight -1 );

  
  
  // 1D array
  ArrayLength = iWidth*iHeight*ColorSize;
  ElementSize = sizeof(unsigned char);
  ArraySize = ElementSize*ArrayLength ; 
  HeaderSize = 11 + (size_t) (log10(iHeight) +log10(iWidth)); 
	
  FileSize = HeaderSize + ArraySize; 
	
  /* create dynamic 1D array for RGB colors  */
  data = malloc (ArraySize);
  if (data == NULL ){
    printf ( "Could not allocate memory for the array\n");
    return 1;}

	
  return 0;
}


void info(){

  printf("ppm (P6) header size = %zu bytes\n", HeaderSize);
  printf("Array Size  = %zu bytes\n", ArraySize);
  printf("PPM file size  = %zu bytes\n", FileSize); // 1,1 MB = 1 080 015 bytes 

	
}



int end(){


  printf (" allways free memory (deallocate )  to avoid memory leaks \n"); // https://en.wikipedia.org/wiki/C_dynamic_memory_allocation
  free (data);
  info();
  return 0;

}



// ================================== main ============================================

int main (){

  int n;
  setup();
  //
  for (n = 0; n< nMax; ++n)
    MakeGradientImage(data, n);
	
  end();

  return 0;
}


















