/*
 * Darkmoon Profile Picture Generator
 * To compile manually: type 'gcc -Wall --std=gnu18 -o gtf0 gtf0.c -lgd -lm'
 * NOTE: the 'libgd3' and 'libgd-dev' packages may need to be installed
 * Documentation:  https://libgd.github.io/manuals/2.3.0/files/preamble-txt.html
 * API:            https://libgd.github.io/manuals/2.3.0/index/Functions.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gd.h>
#include <math.h>
 
double rtod(const double rad) { return (rad*180/M_PI); }
double g(const double x, const int r) {
	if(x>0) {
		return (sqrt((x*x*r*r)/(1+(x*x))));
	}
	else {
		return (-sqrt((x*x*r*r)/(1+(x*x))));
	}
}
double f(const double x, const int r) {
	return (-sqrt((r*r)-(x*x)));
}
double L(const double x, const double s, const int r) {
	if(x<=0) {
		return (-s*(x-g(-s, r))+f(g(-s,r),r));
	}
	else {
		return (s*(x-g(s, r))+f(g(s,r),r));
	}
}

int main()
{
    //////////////////////////////////////////////////////////////////////////
    //
    // Declare variables
    //
    char       *outfile    = NULL;   // name of the output file
    char        project[]  = "pfp"; // "string" of project name
    FILE       *out        = NULL;   // output file pointer
    gdImagePtr  image;               // GD Image Construct
    int         wide       = 0;      // image width (pixels)
    int         high       = 0;      // image height (pixels)
    int         black      = 0;      // variable representing color value
    int         white      = 0;

    // Construct output file name (keep this as-is)
    outfile                = (char *) malloc (sizeof(char) * 73);
    sprintf (outfile, "%s.png", project);
 
    // image dimensions, in pixels (note: the first coordinate on the axis
    // is 0)

    wide                   = 1499;
    high                   = 1499;
 
    // Create new image of specified wide-ness and high-ness (this also will
    // initialize the gd library. Any gd functions called prior to this line
    // may result in error, since there was no image region in memory)

    image                  = gdImageCreate ((wide + 1), (high + 1));
 
    // mixing up our colors
    //                                             image  red   green blue

    black                  = gdImageColorAllocate (image, 0x00, 0x00, 0x00);
    white                  = gdImageColorAllocate (image, 0xFF, 0xFF, 0xE0);

    // Paint the background black
    gdImageFilledRectangle (image, 0, 0, wide, high, black);

	//Math variables
	unsigned int orx = (wide+1)/2; //origin of x/center
	unsigned int ory = 2*(high+1)/5; //origin of y/center
	unsigned int r = 450;
	unsigned int c2 = r*7/10;
	double slope = 2.5;

    // Main Picture:
    gdImageFill (image, 1, 1, black);

	// Main circle
    gdImageArc (image, 0+orx, 0+ory, r, r, 0, 360, white);
    gdImageFill (image, 0+orx, 0+ory, white);
	// Lines
	gdImageLine (image, (int)(-g(slope,r/2))+orx, (int)(-L(-g(slope, r/2), slope, r/2))+ory, orx, (int)(-L(0, slope, r/2))+ory, white);
	gdImageLine (image, orx, (int)(-L(0, slope, r/2))+ory, (int)(-g(-slope,r/2))+orx, (int)(-L(-g(slope, r/2), slope, r/2))+ory, white);
    gdImageFill (image, 0+orx, (int)(-L(0, slope, r/2))+ory-5, white);
	//Right Eye
    gdImageArc (image, orx+c2, ory, r, r, 0, 360, black);
    gdImageFill (image, orx+(r/2)-1, ory, black);
    //Left Eye
	gdImageArc (image, orx-c2, ory, r, r, 0, 360, black);
    gdImageFill (image, orx-(r/2)+1, ory, black);
    //Redraw Main circle
	gdImageArc (image, orx, ory, r, r, 0, 360, white);
	
	//Right Crescent
    gdImageArc  (image, orx+r, ory, r, r, 0, 360, white);
    gdImageFill (image, orx+r, ory, white);
    gdImageArc  (image, orx+r+(r/4), ory, r*3/4, r*3/4, 0, 360, black);
    gdImageFill (image, orx+r+(r/4), ory, black);
    
	//Left Crescent
    gdImageArc  (image, orx-r, ory, r, r, 0, 360, white);
    gdImageFill (image, orx-r, ory, white);
    gdImageArc  (image, orx-r-(r/4), ory, r*3/4, r*3/4, 0, 360, black);
    gdImageFill (image, orx-r-(r/4), ory, black);
    
	//Nostrils
    gdImageArc  (image, orx-r/8, r/2+ory , r/8, r/4, 0, 360, black);
    gdImageFill (image, orx-r/8, r/2+ory, black);
    gdImageArc  (image, orx+r/8, r/2+ory , r/8, r/4, 0, 360, black);
    gdImageFill (image, orx+r/8, r/2+ory, black);


    //////////////////////////////////////////////////////////////////////////
    //
    // Open the output file (indicated in 'outfile') for writing (keep as-is)
    //
    out           = fopen (outfile, "wb");
    if (out      == NULL)
    {
        fprintf (stderr, "Error opening '%s'\n", outfile);
        exit (1);
    }
 
    //////////////////////////////////////////////////////////////////////////
    //
    // Export, in PNG format, the image data in memory to our output file
    // (keep as-is)
    //
    gdImagePngEx (image, out, -1);
 
    //////////////////////////////////////////////////////////////////////////
    //
    // Close things up (keep as-is)
    //
    fclose (out);
    gdImageDestroy (image);
 
    return (0);
}
