
Hello user, the mix.c program instructions are explained below.

The program is used to mix two input files mono or stereo independently, the code will check if the command line arguments are valid for input files. Below, I show you the steps for its use:


	- First compile the program with the Libsound API library (need to be pre-installed):

		$ cc -o mix mix.c -lsndfile


	- Second execute the program with this suggested structure:

		./mix  "FileInput1.wav"  GainFile1  "FileInput2.wav"  GainFile2  "FileOutput.wav"  PanValueFile1  PanValueFile2


		For example, we would say that we have some mono and stereo files in the same folder of our mix.c is executing such as: Piano_st.wav, Pad_st.wav, Guitar_mn.wav, Flute_mn.wav

		Some of the valid inputs for the program will be:

			$ ./mix Guitar_mn.wav 0.5 Flute_mn.wav 1 mixfile.wav 0.4 0.6         (Mono files with two independent pan values for each file)
			
			$ ./mix Pad_st.wav 1.5 Piano_st.wav 0.5 mixfile.wav                 (Stereo files without any necessary pan value)

			$ ./mix Guitar_mn.wav 0.5 Piano_st.wav 1.5 mixfile.wav 0.5           (Mono file 1 input, so you only need one pan value for the final stereo output)

			$ ./mix Pad_st.wav 0.5 Flute_mn.wav 1 mixfile.wav 0.5                (Mono file 1 input, so you only need one pan value for the final stereo output)


		NOTE: Pan values are fixed like this:

			left[n] = input[n] * pan;   // Values p = 0 full right channel
                        right[n] = input[n] * (1.0 - pan);  //Value p = 1 full left channel


	- Lastly, if any of this arguments missing the program will return with an error message such as: Invalid number of arguments! Also the program is made to accept matching formats and sampling rate values, an error message will be printed if any of this confirmation failed. In addition to, the code accept any major format.




Thanks, for using.
Mario
