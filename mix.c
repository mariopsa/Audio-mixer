#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>

#define BUFSIZE 512

/*Program to mix two mono or sterep soundfiles, instructions of use are explained in the mix.txt*/

int main (int argc, char *argv[]){

  SNDFILE *sfp1_in, *sfp2_in, *sfp_out;      //file pointers
  SF_INFO sfinfo1_in, sfinfo2_in, sfinfo_out;     //file information
  sf_count_t items, i, j;     //counting variables (samples/frames)
  float inbuf_1[BUFSIZE], inbuf_2[BUFSIZE], outbuf[BUFSIZE];    // buffers
  float pan_1, pan_2;    //pan variable
  float gain_1, gain_2;     //gain variable

            /*INPUT FILE 1 OPEN*/
  sfp1_in = sf_open(argv[1], SFM_READ, &sfinfo1_in);
  /*Debugging input file 1*/
  if (sfp1_in == NULL) {
    printf("could not open file %s", argv[1]);
    return -1;
  }
  printf("Input file 1 %s is open \n", argv[1]);
  
  /*Gain input 1 file with string to float function*/
  gain_1 = atof(argv[2]);
  
            /*INPUT FILE 2 OPEN*/
  sfp2_in = sf_open(argv[3], SFM_READ, &sfinfo2_in);
  /*Debugging input file 2*/
  if (sfp2_in == NULL) {
    printf("could not open file %s", argv[3]);
    return -1;
  }
  printf("Input file 2 %s is open \n", argv[3]);

  /*Gain input 2 file with string to float function*/
  gain_2 = atof(argv[4]);
  
  /*Check Sampling Rate and Format matching between two input files*/
  if (sfinfo1_in.samplerate == sfinfo2_in.samplerate){
    printf("Mathcing sampling rate success. \n");
  }
   else if (sfinfo1_in.format == sfinfo2_in.format){
    printf("Mathcing format success. \n");
    /*Error display if the SR or format doesnt match*/
   } else{
     printf("One the input file doesnt match with others! \n");
    return -1;
   }

  /*Channels set-up before open the output file*/
  if ((sfinfo1_in.channels == 1) || (sfinfo2_in.channels == 1)){
      sfinfo_out.samplerate = sfinfo1_in.samplerate;
      sfinfo_out.format = sfinfo1_in.format;
      sfinfo_out.channels = 2; //Change the output files to 2 channels
  }else {
      sfinfo_out.samplerate = sfinfo1_in.samplerate;
      sfinfo_out.format = sfinfo1_in.format;
      sfinfo_out.channels = 2;
  }

            /*OUTPUT FILE OPEN*/
  sfp_out = sf_open(argv[5], SFM_WRITE, &sfinfo_out);
  /*Debugging output file*/
  if (sfp_out == NULL) {
    printf("could not open file %s", argv[5]);
    return -1;
  }
  printf("Output file open \n");

                               /*MAIN MIX LOOPS*/
  
  /*FULL STEREO MIX*/
  if ( (sfinfo1_in.channels == 2) && (sfinfo2_in.channels == 2) ){
    if (argc == 6){ /*Checking for user input*/
      pan_1 = 0;
      pan_2 = 0; /*Random set value*/
	do{
	 items = sf_read_float (sfp1_in,inbuf_1,BUFSIZE);
	 items = sf_read_float (sfp2_in,inbuf_2,BUFSIZE);
       for(i=0; i<items; i++){
            outbuf[i] = (inbuf_1[i]*gain_1) + (inbuf_2[i]*gain_2);
	}
       sf_write_float(sfp_out, outbuf, items); /*Count in samples*/
       } while (items > 0);
    } else {
      printf("Invalid number of arguments! \n");
    }
    
  }

  /*FULL MONO MIX*/
  else if ( (sfinfo1_in.channels == 1) && (sfinfo2_in.channels == 1) ){
    if (argc == 8) {
      pan_1 = atof(argv[6]);
      pan_2 = atof(argv[7]);
	do{
	  items = sf_read_float (sfp1_in,inbuf_1,BUFSIZE/2); /*Half of the buffer size to count in frames*/
	  items = sf_read_float (sfp2_in,inbuf_2,BUFSIZE/2); /*Half of the buffer size to count in frames*/
	    for(i = j = 0; i < items; i++) {
	      outbuf[j++] = inbuf_1[i] * (1. - pan_1) * gain_1 + inbuf_2[i] * (1. - pan_2) * gain_2; 
	      outbuf[j++] = inbuf_1[i] * pan_1 * gain_1 + inbuf_2[i] * pan_2 * gain_2; 
	    }
       sf_writef_float(sfp_out, outbuf, items);
       } while (items > 0);
	
    } else{
      printf("Invalid number of arguments! \n");
    }
    
  }

  /*CHANNEL 1 MONO MIX*/
  else if ( (sfinfo1_in.channels == 1) && (sfinfo2_in.channels == 2) ){
    if (argc == 7){
      pan_1 = atof(argv[6]);
	do{
	  items = sf_read_float (sfp1_in,inbuf_1,BUFSIZE/2); /*Half of the buffer size to count in frames*/
	  items = sf_read_float (sfp2_in,inbuf_2,BUFSIZE);
	  for(i = j = 0; i < items; i++) {
		outbuf[j++] = inbuf_1[i] * (1. - pan_1) * gain_1 + inbuf_2[i] * gain_2; 
		outbuf[j++] = inbuf_1[i] * pan_1 * gain_1 + inbuf_2[i] * gain_2; 
	    }
       sf_write_float(sfp_out, outbuf, items);
       } while (items > 0);
	
    } else {
      printf("Invalid number of arguments! \n");
    }
    
  }
  
  /*CHANNEL 2 MONO MIX*/
  else if ( (sfinfo1_in.channels == 2) && (sfinfo2_in.channels == 1) ){
    if (argc == 7) {
      pan_1 = atof(argv[6]);
	do{
	  items = sf_read_float (sfp2_in,inbuf_2,BUFSIZE/2); /*Half of the buffer size first mono channel*/
	  items = sf_read_float (sfp1_in,inbuf_1,BUFSIZE);
	  for(i = j = 0; i < items; i++) {
		outbuf[j++] = inbuf_2[i] * (1. - pan_1) * gain_2 + inbuf_1[i] * gain_1; 
		outbuf[j++] = inbuf_2[i] * pan_1 * gain_2 + inbuf_1[i] * gain_1;
	    }
       sf_write_float(sfp_out, outbuf, items);
       } while (items > 0);
	
    } else{
      printf("Invalid number of arguments! \n");
    }
    
  }

  return 0;
}
