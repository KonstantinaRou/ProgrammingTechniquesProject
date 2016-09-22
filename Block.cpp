#include "Block.h"
#include <iostream>
#include <string>

using namespace imaging ;

Block::Block (const size_t & block_size){
	size = block_size;
	data=(Component*)malloc(block_size);
}

Block::Block(const Block & src){
	*this=src;
}

Block:: ~Block (){
	free(data);
}

// Generates and returns a block of appropriate size according to a user-defined region on an NON-INTERLACED
// Image and copies the data from there. The channel variable decides which image channel will the block be copied from. The size 
// of the returned block depends on the size of the image and the position pos of the selected region in it.
// Returns nullptr if the estimated size is zero. This can happen for various reasons: a) the size argument was 0, b)
// there were no pixels left form the requested position to the right margin of the image, c) the y coordinate is 
// greater than image height-1, d) The requested block size cannot be allocated.
// If size > image.width - pos.x, then the block actual size becomes image.width - pos.x

Block * Block::copyFromImage(Image & src, Image::channel_t channel, const ivec2 & pos, const size_t & size){
	if(size==0)
		return NULL;
	if((unsigned int)pos.y>src.getHeight()-1)
		return NULL;
	if((unsigned int)pos.x>src.getWidth()-1)
		return NULL;

	size_t s;
	if((unsigned int)size>src.getWidth()-(unsigned int)pos.x)
		s=src.getWidth()-pos.x;
	else
		s=size;

	Block * temp = new Block(s);
	int red=(pos.y*src.getWidth()+pos.x);
	int green = red + src.getWidth()*src.getHeight();
	int blue = green +src.getWidth()*src.getHeight();

	if(channel==src.RED){
		for(unsigned int i=0 ;i<s ; i++ ){
			temp->data[i]=src.getRawDataPtr()[red+i];
		}
	}
	else if(channel==src.GREEN){
		for(unsigned int i=0 ;i<s ; i++ ){
			temp->data[i]=src.getRawDataPtr()[green+i];
		}
	}
	else if(channel==src.BLUE){
		for(unsigned int i=0 ;i<s ; i++ ){
			temp->data[i]=src.getRawDataPtr()[blue+i];
		}
	}
	return temp;
}

size_t Block:: getSize() const {
	return size;
}

Component* Block:: getDataPtr() const{
	return data;
}

void Block:: setData(const Component * src){
	for(unsigned int i=0 ; i<getSize();i++){
		data[i]=src[i];
	}
}

Block Block:: reverse() const {
	Block *temp = new Block(getSize());
	size_t s =getSize();
	for (unsigned int i=0 ; i<s; i++){
		temp[i]=this[s-i];
	}
	return *temp;
}

void Block::setErrorMargin(Component err){
	err=error_margin;
}

Component & Block:: operator[] (const size_t index) const {
	return data[index];
}

Component & Block:: operator() (const size_t & index) const{
	if (index>getSize())
		return data[getSize()];
	else
		return data[index];
}

bool Block:: operator==(const Block & rhs) const {
	if(rhs.getSize()!=getSize()){
		return false;
	}
	else {
		for (unsigned int i=0; i<getSize();i++){
			if(data[i]!=rhs[i])
				return false;
		}
	}
	return true;
}

bool Block:: operator!=(const Block& rhs) const{
	return !(*this==rhs);
}

Block & Block:: operator=(const Block & src){
	return *this;
}