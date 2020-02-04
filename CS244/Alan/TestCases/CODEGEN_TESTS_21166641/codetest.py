#########################################################
# @file    codetest.py
# @brief   Test code generation for compiler project.
# @author  C Strydom (21166641@sun.ac.za)
# @date    2018-09-23
#########################################################

import os
import styletest

'''Performs a comprehensive test of the code generation component'''
def test_codegen():	
	if styletest.test_style():
		print "No critical style errors found in source files."
	print
	
	test("trial/trivial", "trivial", False)
	test("trial/helloworld", "helloworld", False)
	test("general/testwrite", "testwrite", False)
	test("general/testmathex", "testmathex", False)
	test("general/testboolex", "testboolex", False)
	test("general/testvars", "testvars", False)
	test("general/testif", "testif", False)
	test("general/testwhile", "testwhile", False)
	test("general/testread", "testread", True)
	test("general/testfuncs", "testfuncs", True)
	test("special/longsum", "longsum", False)
	test("special/quicksort", "quicksort", True)

	os.system('rm *.pyc')

'''Executes one particular test case on the compiler'''
def test(path, class_name, inp):
	passed = False
	os.system('../bin/simplc %s.alan' % path)
	if inp:	
		os.system('java %s > output.temp < %s.in' % (class_name, path))
	else:
		os.system('java %s > output.temp' % class_name)
	os.system('diff %s.out output.temp | wc -l > wc.temp' % path)
	f = open("wc.temp", "r")
	if int(f.read(1)) == 0:
		passed = True
	f.close()
	if passed:
		print class_name, "PASSED!"
	else:
		print class_name, "FAILED!"
	print "Valgrind produced the following output:"
	os.system("valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ../bin/simplc %s.simpl" % path)
	print
	os.system('rm *.temp')
	os.system('rm *.class')

if __name__ == "__main__":
	test_codegen()
