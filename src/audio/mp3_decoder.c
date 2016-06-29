/*
#   love
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/


// This class currently does not work!

#include "mp3_decoder.h"

static struct {

} moduleData;


int audio_mp3_load(unsigned int bufferID, const char* filename) {
	 mpg123_handle *m = NULL;
    
	 int ret;
	 int channels = 0, encoding = 0;
	 size_t fill;
	 long rate;
	 unsigned char buffer[16*1024];
	 int err  = MPG123_OK;
	 

	 err = mpg123_init();
	 if (err != MPG123_OK) printf("%s \n", "Error: could not init mpg123");
	 
	 m = mpg123_new(NULL, &err);
	 if (m == NULL) printf("%s \n", "Error: Could not init mpg123's handle");	 
	 mpg123_open_feed(m);

	 err = mpg123_open(m,filename);
	 if (err != MPG123_OK) printf("%s %s \n", "Error: Could not load file named: ", filename);
		
	 mpg123_getformat(m, &rate, &channels, &encoding);	
	
    // Ensure that this output format will not change (it could, when we allow it). 
	 mpg123_format_none(m);
	 mpg123_format(m, rate, channels, encoding);
 	
	 /*
	 do {
   	 err = mpg123_read(m, buffer, sizeof(buffer), &err);
	 }while(err == MPG123_OK);
    */
	 
	 if (err != MPG123_DONE) printf("%s \n", "Error: could not finish decoding the file");

	 if (fill == MPG123_ERR) {
		 printf("%s \n","Error: could not decode", mpg123_strerror(m));
		 return 0; 
	 }
			
	 alBufferData(bufferID, AL_FORMAT_STEREO16,  buffer, sizeof(buffer), rate);		

	 mpg123_close(m);

	 mpg123_delete(m);	
	 mpg123_exit();
	 return 1;
}
