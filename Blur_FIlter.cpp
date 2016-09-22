#include "Blur_Filter.h"
#include "Color.h"
#include <string>
#include <cstdlib>
#include <iostream>

using namespace imaging;

Image * BlurFilter::apply( Image & src)
{
	std:: cout << "\nApplying Blur Filter...\n";
	Image *newImage=new Image(src);
	
	for (unsigned int i =0 ; i< src.getHeight();i++){
		for(unsigned int j=0 ; j<src.getWidth(); j++){
			
			int count = 0; 
			float red=0;
			float blue=0;
			float green = 0;
			Color c1;
			Color c2;
			for(int m = -1 ; m<=1;m++){
				for(int n = -1; n<=1 ; n++){
					if((i+m<src.getHeight())&&((int)i+m>=0)&&(j+n<src.getWidth())&&((int)j+n>=0)){
						
						count++;
						c1=src.getPixel((int)i+m,(int)j+n);
						red = red + (float)c1.x;
						green = green +(float)c1.y;
						blue= blue+(float)c1.z;
					}
				}
			}
			red=red/count;
			green=green/count;
			blue=blue/count;

			c2.x=red;
			c2.y=green;
			c2.z=blue;

			newImage->setPixel(i,j,c2);

			}
		}
	std:: cout << "The filter was succesfully applied\n"; 
	return newImage;


	}

