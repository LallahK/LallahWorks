Description:

	Script and test cases for testing code generation component of compiler.

Author:

	C Strydom (21166641@sun.ac.za)

Editor:

	T de Bruin (21802580@sun.ac.za)

Instructions:

	Move the directory test into your alan directory.  It should be in the
	same directory as src and bin, but should not be in src or bin.  First
	compile your source code with "make alanc" and then execute the 
	test script in the test directory with "python codetest.py"

	A message will be displayed stating whether each test passed or failed.
	Output from Valgrind will also be displayed for each test.

Caution:

	The script assumes that the directories src, bin and test are in the same
	directory and that all source code is in src and all executables in bin.
	If this is not the case the script will fail to find your files.

	Do not open any test files with vim, gedit or any other editor.  These 
	editors may format the files in undesirable ways and cause the tests to 
	fail.  Use "cat <filename>" if you wish to see the contents of a file.

	Never place any files in the test directory.  Intermediate files are 
	created and removed during the testing process and so any files in the test
	directory could potentially be removed.

	The script used Valgrind so make sure it is installed and be aware that it
	may take a while to run.
