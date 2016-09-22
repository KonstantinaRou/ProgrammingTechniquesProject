#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include "ppm_format.h"
#include "cpi_format.h"
#include "Grey_Filter.h"
#include "Color_Filter.h"
#include "Blur_Filter.h"
#include "Median_Filter.h"
#include "Diff_Filter.h"

#include "rle_format.h"
#include "Filter.h"
#include <sys/stat.h>

using namespace imaging ;


//createName : methodos dhmiourgias onomatos
//vriskei apo to palio onoma thn teleia "."
//kai apothikeyei se mia metavlhth oti yparxei prin thn teleia 
//epeita synthetei to neo onoma
std::string createName(std::string oldname){
	
	std:: string new_name ; 
	std::size_t pos = oldname.find('.');
	std:: string strStart = oldname.substr(0,pos);
	
	new_name.append(strStart);
	new_name.append(".filtered");
	new_name.append(".ppm"); //den mas noiazei ti exei edw
	

	return new_name;
}


 
int main(int argc,char * argv[]){

	
	Image *im = NULL;
	
	Filter *f;
	Color color;
	std:: string in_filename = argv[argc-1];
	std:: string out_filename ;
	
	
	if(isSuffix(in_filename,"ppm")){
		PPMImageReader *ppmReader= new PPMImageReader();
		im = ppmReader->read(in_filename);
		
	}

	else if(isSuffix(in_filename,"rle")){
		RLEImageReader *rleReader= new RLEImageReader();
		im=rleReader->read(in_filename);
	}
	
	Image *newImage = new Image(*im);
	int i=1;

	while(i < argc-1){
		if(strcmp(argv[i],"-f")==0){
			i++;
		}
		else if(strcmp(argv[i],"gray")==0){
			f = new GreyFilter();
			newImage = f->apply(*newImage);
			i++;
		}
		else if(strcmp(argv[i],"color")==0){
			if(atoi(argv[i+1])>255||atoi(argv[i+2])>255||atoi(argv[i+3])>255){
				std:: cout <<"The colors you selected are out of range.\n";
				break;
			}
			color.x=(unsigned int)atoi(argv[i+1]);
			color.y=(unsigned int)atoi(argv[i+2]);
			color.z=(unsigned int)atoi(argv[i+3]);
			f = new ColorFilter(color);
			newImage = f->apply(*newImage);

			i=i+4;
		}
		else if(strcmp(argv[i],"blur")==0){
			f = new BlurFilter();
			newImage = f->apply(*newImage);
			i++;
		}
		else if(strcmp(argv[i],"median")==0){
			 f = new MedianFilter();
			 newImage = f->apply(*newImage);
			 i++;
		}
		else if(strcmp(argv[i],"diff")==0){
			 f = new DiffFilter();
			 newImage = f->apply(*newImage);
			 i++;
		}
		else {
			std:: cout<< "You gave wrong type of arguments\n" ;
			break;
		}
	}

	out_filename = createName(in_filename);

	if(isSuffix(in_filename,"ppm")){
		PPMImageWriter *ppmWriter = new PPMImageWriter();
		ppmWriter->write(out_filename,*newImage);
		
	}

	else if(isSuffix(in_filename,"rle")){
		RLEImageWriter *rleWriter= new RLEImageWriter();
		rleWriter->write(out_filename,*newImage);
	}

	system("PAUSE");
	
	return 0;
}


	











