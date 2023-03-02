# RSA-Project
RSA Algorithm implementation for EECE4632.

## Overview:
My project for EECE4632 is implementing an RSA encryption algorithm on the Zynq board. The final result will feed values into an Overlay that computes the decryption and encryption of data on the FPGA. 


## Repository info:
There are currently 3 main folders:
 - HLS Source Code holds the cpp files used in HLS and any .tcl, .hwh, or .bit files used for the overlay.
 - Jupyter Notebooks holds all the notebooks where the algorithm is tested on the Zynq board.
 - Python Code holds all the python code I developed offline (w/o Zynq) to help develop the project. Note: most of these files are just rougher versions of the notebooks in the Jupyter folder.
## Updates:

### 3/2 Update 1
  
**Jupyter Notebook/Software Updates**

  The first python code, "Simple RSA PS", was created to implement an RSA encryption algorithm using small values and simple computation. This code was adapted from the example provided at https://www.geeksforgeeks.org/rsa-algorithm-cryptography/. When testing, as the prime numbers or inputted data got larger, the encryption failed. I first fixed this by implementing a built-in method of the "pow(a,b,c)" that calculates the modulo after the value a ^ b. This resolved the issue by preventing the returned value of "pow(a,b)" from being a float, and not being an accurate value to compute the modulo. Regardless, I found that after a certain point, the integer value maxes out and returns the same maximum value. From this code, I became very familar with the RSA algorithm and how it is performed to help develop the next version. Some of the functions implemented in this code were also adapted to the other versions.

  The next version, "String RSA Encryption with Prime generation", works by converting a string into bytes to be encrypted/decrypted. For developing this, I utilized ChatGPT to provide similar examples, and created my own adaptation. Some code is fully designed by me, while others area are adapted from sources, with comments to cite. In developing this, I found that large prime numbers enabled a larger size such as a string to be encrypted. This was because the prime numbers were generated using the function "random.getrandbits(NBITS)" where 2 * NBITS is the size of the encryption. However, for this to work, the random generator needs to confirm if it is prime, or generate another value. This resulted in a major delay that was noticable on the PS at around 8 seconds. As a result, I tried to adapt the code to use provided prime numbers, but the resulting decryption was limited to a 64 bit int. I attempted to use numpy to resolve the issue and tried converting the integers to bytes and adding MSB spacers, however nothing resolved the issue. I then decided to reassess the approach, and considered using chars instead of a string so the value could be limited. 
  
  In this version, "RSA Encryption with Chars", I found that I could generate an array of integers for each char in a string by iterating through it and using ord(char). For decrypting, the integer array is converted back to a char list using char(int), and is finally returned as a formed string using ''.join.(charList). By using char, this would help limit the value of the int while still returning a string. Lastly, it is able to have prime values given in the function, allowing it to run much faster than the previous attempt. Most of the code was recycled from the last notebook (#2), with the char addition found using examples from geek for geeks, stack overflow, and chatGPT. See the comments/markdown in this notebook for more.

  The last notebook is "RSA HLS_Stream Example" which implements the "RSA Encryption with Chars" method to stream an array of values into the PL to be encrypted. This notebook is not yet functional until the HLS Source code is working properly. While it cannot be tested, it shows how I plan to implement the RSA algorithm using the method from the "RSA Encryption with Chars" notebook. 
  
**HLS Source File/Hardware Updates**

  From the software examples it seemed that the char example was most promising for implementing on the PL because I can stream arrays like HW4. Using this design, I would use the PS to convert the string into an array of char and integers and then pass the values to the overlay. The encryption will be returned and stored in an array of ints. Then, this array will be passed back to the PL for decryption where they return to their original int values and are converted back to char and joined together on the PS.
  
  Using examples of HLS from past homework and examples of RSA encryption in C++, I was able to develop the source file "RSA_Encryption.cpp". The majority of the code is functions that replicate the calculations performed on software such as the inverse modulo or exponential modulo. Furthermore, to simplify things for this version, the public and private keys are generated in the source code with static prime number values. Lastly, there are two functions that perform the encryption and decryption using the keys and data. The main function was used to run and test the code outside.
  
  After the main function is the stream function that uses axi-stream to read/write data with the PS. This function was developed from the tutorials in HW4 to read the input_buffer array and return values to an output_buffer. Unfortunately, this part of the code has not been tested, and is my next step in developing my project.
  
**Upcoming Tasks**

  The next steps for my project is to finish the HLS Source file with the axi-stream functionality and implement it. This entails developing the RSA HLS_Stream Example notebook and RSA_Encryption.cpp file further so that I can test the overlay. The steps to do this would be the same as HW4 - develop the source code, export RTL, create block diagram, generate bitstream, and code Jupyter notebook to interact with overlay. Since most of the code is written for both the notebook and source code, finishing this part of the project won't take very long. Once this is operational, I can begin testing for optimal ways to handoff the data to the PL. 
  
  Lastly, I will work on optimizing the hardware. Optimization will still occur while developing the source code since it will save time on regenerating the bitstream, but testing different solutions will likely come after. This is because I want to ensure everything is working before moving onto the optimization. While optimizing, I will perform similar changes to the directives as done in HW6, and will produce a pareto graph with the results. My final version will use the best optimization based on both the needs of the implementation (faster/smaller) and the results of the pareto. 
