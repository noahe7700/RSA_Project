## Brief Update for RSA Project (3/2)

### Description of implementation at this point

Software:
- Worked on 3 different examples of RSA encryption using PS. In the **Jupyter Notebooks** folder, you can find all the software examples I implemented and tested.
- "Simple RSA PS" notebook was the first example and used an integer as its input. It was limited however by integer sizes and float conversions, and therefore could only encrypt small values.
- "String RSA Encryption PS with Prime generation" was the second attempt at implementing the RSA algorithm. In this example, a string was converted to its byte values and that was encrypted. However, in order for this to work, it needed large prime values. The example I found used random generation of an integer with N bits to produce the large integer - but this function took a very long time. I attempted to modify by passing prime numbers, but it would result in a failed encryption.
- The "RSA Encryption with char and int array" was the final implementation of the RSA algorithm on software. Like the string example, this too encrypted a string message, but differed by encrypting/decrypting each individual char in the string. The char is converted to an int, and each value is converted to produce an encrypted array. In reverse, the integers are decrypted, converted back to char, and joined together.
- In the end, "RSA Encryption with char and int array" was the best usage and would pair very well with AXI-Stream. Furthermore, the smaller integer sizes in the array will make it easier to optimize later on. 
- The "RSA HLS_Stream" notebook is an adaptation of the previous notebook with the added framework for implementing the overlay and PL. It is not functional as of now, but is being used to help formulate the approach of adding the interface. 

Hardware:
- In addition to the first run at the "RSA HLS_Stream", I began implementing the C++ code for PL. This can be found on the file "RSA_Encryption.cpp" within the folder **HLS Source Code**.
- The C++ code implemented is an early approach of replicating the functionality done in the RSA Encryption notebooks. Formulas available in python had to be recreated in this environment such as inverse modulo and exponential modulo. 
- Lastly, the final two functions are the AXI-Stream attempts that replicate the work done in HW4. This part of the code has not been tested yet and needs to work before the "RSA HLS_Stream" notebook can.

Other:
- In the **Python Code** folder there is all my rough drafts of developing the RSA code. These are the codes I ran on my computer before applying them in class.


### How to Run & Expected Outputs

The software notebooks can all be run on any device or board. As of now, there isn't any functional implementation of an overlay, so running the Jupyter Notebooks should be simple. 

The expected results of running the software code is to have the original value or string plugged into the encryption/decryption returned after running. For the first notebook, the value is the variable "msg" in the last code block, which is then computed and printed. If you increase the integer value too much without increasing the prime number values, you will find that the result is inaccurate. 

The second notebook using a string should have the plaintext value 'Hello World' returned when run. Notice that before the 'Hello World' in the output there will also be a very long non-sense string which is the encrypted text. You can test other string values by changing 'Hello World' in the line: message = b"Hello World" of the test example.

The third notebook using chars should have an output plaintext value of 'Hello, world!". Unlike the string example, you can see that the message is acually encoded in an array of numbers from the Cipher text. Additionally, in this example you can also change the prime numbers by changing the values of prime1 and prime2.

For the C++ program, there is a main() function that can be used to test the code. It had difficulties being tested outside of Vitis due to the libraries, but the debugger can show that the computations are functioning properly.


### Plan for Next Update (3/23)

My plan for the next update is to focus on developing the HLS implementation and AXI-stream. By 3/23, I much have the interface operational so that the AXI-Stream can be used and test values. There is a good chance that the resulting encryption or decryptions will be wrong, however it is crucial that the interface works so that the hardware can be tested. I also hope to have the encryption or decryption function operating with the stream and return valid results. Ideally I would have both the encryption and decryption functions working, but to help debug, I am splitting the functions apart to test individually. Therefore, my goal is to have the stream operational and at least one of the RSA functions working. 
