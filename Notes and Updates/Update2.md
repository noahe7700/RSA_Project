## Update for RSA Project (3/29)

### Description of implementation at this point

Software:
- Added "RSA Encryption with AXI Stream Interface" Jupyter notebook for running and testing the AXI stream interface with the PL. The notebook uses some functions from the previous examples to validate the data on HW. To run the notebook with the overlay, download the overlay files and change the overlay location in the code.

HLS Vitis:
- The first attempt at implementing RSA on the PL is the file "RSA_Encryption.cpp" within the folder **HLS Source Code**. This code has been verified with the test bench and is able to perform the computations of generating the keys and performing the algorithm. However, the area and volume of LUTs exceeded the Zynqs capabilities. To resolve this, I implemented the "RSA_SIMPLE" version.
- In the folder **RSA_Simple** there is the next version of the RSA algorithm on PL called RSA_Simple.cpp. This version reduced the area by manipulating the data types and changing the loops, while eliminating the key generator functionality. As a result, static public and private keys that were verified on the software were used to help validate the encrypted results. 
- The RSA_Simple_Test.cpp is the testbench for the RSA_simple.cpp code, and uses the same parameters as the software implementation. The array of values for the charaters in the phrase "Hello, World!" are being passed to the AXI stream and encrypted, just like the notebook. The resulting values matched the notebooks, and confirmed that the encryption succeeded.  
- The HLS Project is located in the folder **Project Zip Files** with the name RSA_Simple.zip.

Vivado:
- In the folder **Overlays** are the overlay files (.hwh, .bit, .tcl) called "dma_axis_ip_example" after the .tcl used to generate the axi-stream block diagram.
- The Vivado project is located in the folder **Project Zip Files** with the name project_RSA2.zip.

Other:
- To quickly confirm this works (or worked if there are issues), please see the validation images in this folder. These images show the results are outputting accurate encrypted values that can be decrypted and read.
- The DMA channel can sometimes run into issues. If there is a problem while running the Jupyter notebook, try rerunning the previous functions before runKernel().
- For more information on the direction and steps done during this update, please see the Notes.md file with more specific updates.


### How to Run & Expected Outputs

To verify the HLS code, use the RSA_Simple.cpp as the source code and RSA_Simple_test.cpp as the test bench to validate the resulting values. In the notebook "RSA Encryption with AXI Stream Interface" run all the supporting code in the first section and observe the encrypted value results at the end to compare with the test. This notebook is able to encrypt and decrypt the message using the same keys as the hardware example. Furthermore, it calculated a private key to decrypt the RSA values which will be used to decrypt the output_buffer from the overlay.

To test the overlay and HW/SW interface, load the bitstream files onto the board (dma_axis_ip_example.hwh/bit/tcl). In the first part of the Overlay section in the notebook, change the overlay location to the proper address. Then run all the code below to see if the code works and returns the same values from the test bench. Furthermore, you can validate the implementation if the message "Hello, World!" is returned.


### Plan for Finishing

My short-term plan moving forward is to implement the decryption function on hardware and test it using the same static values. My belief is this will likely be a quick process to implement since it is very similar to the encryption. From there, depending on time, my next steps will be to add the key generator back into the function by optimizing the design to take up less area. If the key generator is too difficult to implement, I will then focus mainly on optimizing the design and testing it.
