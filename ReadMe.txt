Emmanuel Colas
ReadMe.txt hw4compiler

This program is a mini pl\0 compiler I wrote for my System software class. This will take in your source code and translate it into tokens and put it into a token array. It will translate the text to tokenlist and print out a assembly code for our vm machine.

To run this code, you need to compile it in eustis using gcc hw4compiler.c command. After that you will enter the command ./a.out [filename].txt to run. 

If it runs to completion, the code will print out your source code, a message will print saying "No errors, program is syntactically correct", and print the assembly code to the terminal. It will also create a file called elf.txt for our virtual machine. If there is an error in the source code it will print out ERROR <"ERROR MESSAGE"> and stop. 
