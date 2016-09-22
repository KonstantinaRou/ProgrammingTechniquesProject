#include "cpi_format.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
using namespace imaging;
 

void CPIImageWriter::write(std::string filename,const Image & src){

	std::ofstream file;
	
	//anoigoume to arxeio pros eggrafi . ios::trunc = se periptvsh pou yparxei to 
	//arxeio diagrefei ta panta kai grafei apo panw tou
	//ios::binary = to anoigoyme se binary morfi
	//genika xrisimopoioume flags
	if (isSuffix(filename,"ppm"))
		replaceSuffix(filename,"cpi");

	file.open(filename, std::ios::binary | std::ios::out |std::ios::trunc); 

	if(!file.is_open()){ 
		std::cout << "Counld not open file \n"; // mhnyma lathoys 
		addLogEntry("Counld not open file \n");
	}


	if (file != NULL ){
		
		//xrisimopoioume thn methodo file pou vrisketai sto ofstream 
		//gia na prosdiorisoume emeis ta bytes
		file.write("CPI", 3);
		
		unsigned char one = 1;
		file.write((char*)&one, 1);
		
		unsigned short endian=258;
		file.write((char*)&endian, sizeof(endian));
		
		unsigned short w = src.getWidth();
		file.write((char*)&w,2);

		unsigned short h = src.getHeight();
		file.write((char*)&h,2);
		
		Image * myImage = new Image (src);
		myImage->convertToColorPlaneImage();
		for (int i =0 ; i<=3*h*w;i++){  
			file.write((char*)myImage->getRawDataPtr()+i,1);
		}
		addLogEntry("Succesfully wrote image data");
	}
	else{
		std::cout << "Cannot open file :";
		std:: cout << filename ;
		std:: cout << "\n It might be open from an other application\n";
		
	}
	file.close();
	
}


Image * CPIImageReader::read(std::string filename){
	std::ifstream file;
	file.open(filename, std::ios::binary | std::ios::in); // anoigoyme to arxeio se binary morfi
	
	if(!file.is_open()){ 
		std::cout << "Counld not open file \n"; // mhnyma lathoys 
		addLogEntry("Counld not open file \n");
		return NULL;
	}
	
	
	Component * data = (Component *) malloc(sizeof(Component));
	unsigned int W = 0;
	unsigned int H = 0;
	int count = 0 ;
	char * word  =(char*) malloc(sizeof(char));
	//diavazei 1 byte kai to vazei sto word
	
	while( count < 6) {  //oso yparxei string gia na diabasoyme 
		if(count == 0 ) {
			file.read(word,3);
			word[3] = '\0';
			if(strcmp(word,"CPI") != 0){
				std::cout << " Incorect format \n";
				addLogEntry("Incorect format \n");
				return NULL;
			}
			count++;
		}
		else if(count==1){
			unsigned short one;
			file.read((char*)word,1);
			one = *word;
			if(one != 1)
			{
				std::cout << " Wrong edition (a=only 1 ) \n";
				addLogEntry(" Wrong edition (a=only 1 ) \n");
				return NULL;
			}
			count++;
					}
		else if ( count == 2 ){
			unsigned char hi , lo ;
			file.read(reinterpret_cast<char*>(&hi), 1);
			file.read(reinterpret_cast<char*>(&lo), 1);

			unsigned int endian = (lo << 8) | hi;
			if(endian != 258){
				std::cout << endian <<" Incorrect Endiannes\n";
				addLogEntry(" Incorrect Endiannes\n");
				break;
			}
			count ++ ; 
		}

		else if ( count == 3 ){
			file.read((char *) &W,2);
			count ++ ;
		}
		
		else if ( count == 4 ){
			file.read((char *) &H,2);
			count ++ ;
		}
			
		else if(count == 5 ){
			data = (Component*)calloc(H*W*3,sizeof(Component));
			file.read((char*)data,H*W*3);
			break;
		}
		addLogEntry("Succesfully read image data\n");
	}

	file.close();
	bool interleaved=false;
	Image * theImage = new Image(W,H,data,interleaved);
	return theImage;
}


