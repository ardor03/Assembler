# SIC-XE Assembler
This assembler is based on the hypothetical SIC/XE machine architecture and is implemented in C++. All 4 instruction formats are available along with all valid addressing modes.

## Steps to compile and run the ASSEMBLER:
● Download the zip folder containing the assembler. </br>
● Extract the zip folder. </br>
● Open the terminal and navigate to the path of the extracted folder. </br>
● It is advisable to use a WSL or linux environment to run else depending on the version of the software, it may or may not run properly. </br>
● Run the command : g++ -std=c++11 pass2.cpp -o assembler.out .</br>
● A file named assembler.out will be created in the folder. </br>
● Move the file to the test_inputs folder. </br>
● Navigate to the text_inputs folder. </br>
● Run the command : ./assembler.out . </br>
● The program to be run should be present in the input.asm file. </br>
● Enter the name of the file as input.asm . </br>
● The output has been written to the respective files. </br>
● The errors if any will be listed in the file named error_input.asm . </br>
