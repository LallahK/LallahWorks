import unittest
from sys import argv
from subprocess import call
from ctypes import cdll, c_char_p, POINTER, c_char

class Test(unittest.TestCase): 

    def setUpTest(self, name, var):
        lib = name[-12:-4]
        FILE = open("{}.c".format(lib), "w")
        if var is not None:
            call(['python3', 'phase_generator.py', name, '{"K":"' + var + '"}'], stdout = FILE)
        else:
            call(['python3', 'phase_generator.py', name], stdout = FILE)

        call(['gcc', '-fPIC', '-shared', '-o', '{}.so'.format(lib), '{}.c'.format(lib)])
        FILE.close()
        return cdll.LoadLibrary("./{}.so".format(lib))

    def tearDownTest(self, name):
        lib = name
        call(['rm', '{}.c'.format(lib), '{}.so'.format(lib)])

    def test_2_1(self):
        PHASE = self.setUpTest(name='phases/phase2/phase2_1.yml', var=None)

        self.assertEqual(PHASE.phase_2(c_char_p(b"4567"),c_char_p(b"6174")), 0)

        self.assertEqual(PHASE.phase_2(c_char_p(b"")), 1)

        self.tearDownTest(name='phase2_1')

    def test_2_2(self):
        PHASE = self.setUpTest(name='phases/phase2/phase2_2.yml', var=None)

        self.assertEqual(PHASE.phase_2(c_char_p(b""),c_char_p(b"1+2+3-4+5+6+78+9")), 0)

        self.assertEqual(PHASE.phase_2(c_char_p(b"")), 1)

        self.tearDownTest(name='phase2_2')

    def test_2_3(self):
        PHASE = self.setUpTest(name='phases/phase2/phase2_3.yml', var=None)

        self.assertEqual(PHASE.phase_2(c_char_p(b"10"),c_char_p(b"4")), 0)

        self.assertEqual(PHASE.phase_2(c_char_p(b"4")), 1)

        self.tearDownTest(name='phase2_3')

    def test_2_4(self):
        PHASE = self.setUpTest(name='phases/phase2/phase2_4.yml', var='{2,1,5,6,8,1,3,9,12,9,5,24,1,6,9,0,12,8,16,0}')

        self.assertEqual(PHASE.phase_2(c_char_p(b"4"),c_char_p(b"50")), 0)

        self.assertEqual(PHASE.phase_2(c_char_p(b"4")), 1)

        self.tearDownTest(name='phase2_4')

    def test_2_5(self):
        PHASE = self.setUpTest(name='phases/phase2/phase2_5.yml', var='{12,576,233,973,123,212,434,10,324,657,239,855}')

        self.assertEqual(PHASE.phase_2(c_char_p(b"4"),c_char_p(b"459")), 0)
        self.assertEqual(PHASE.phase_2(c_char_p(b"3"),c_char_p(b"2076")), 0)
        self.assertEqual(PHASE.phase_2(c_char_p(b"2"),c_char_p(b"1365")), 0)

        self.assertEqual(PHASE.phase_2(c_char_p(b"5"),c_char_p(b"225")), 1)
        self.assertEqual(PHASE.phase_2(c_char_p(b"6"),c_char_p(b"445")), 1)
        self.assertEqual(PHASE.phase_2(c_char_p(b"1"),c_char_p(b"4649")), 1)
        self.assertEqual(PHASE.phase_2(c_char_p(b"1")), 1)

        self.tearDownTest(name='phase2_5')

    def test_2_6(self):
        PHASE = self.setUpTest(name='phases/phase2/phase2_6.yml', var='587')

        self.assertEqual(PHASE.phase_2(c_char_p(b"27"),c_char_p(b"21")), 0)
        self.assertEqual(PHASE.phase_2(c_char_p(b"12"),c_char_p(b"48")), 0)
        self.assertEqual(PHASE.phase_2(c_char_p(b"7"),c_char_p(b"83")), 0)

        self.assertEqual(PHASE.phase_2(c_char_p(b"43"),c_char_p(b"14")), 1)
        self.assertEqual(PHASE.phase_2(c_char_p(b"18"),c_char_p(b"33")), 1)
        self.assertEqual(PHASE.phase_2(c_char_p(b"9"),c_char_p(b"66")), 1)
        self.assertEqual(PHASE.phase_2(c_char_p(b"9")), 1)

        self.tearDownTest(name='phase2_6')

    def test_3_1(self):
        PHASE = self.setUpTest(name='phases/phase3/phase3_1.yml', var = None)

        self.assertEqual(PHASE.phase_3(c_char_p(b"adenyl painch kiblah scorch"),c_char_p(b"an dypelaihin bcklasrhcc oh")), 0)
        self.assertEqual(PHASE.phase_3(c_char_p(b"oldies hazzan remaps sunray"),c_char_p(b"oi lehdsaznez marapsrsua ny")), 0)

        self.assertEqual(PHASE.phase_3(c_char_p(b""),c_char_p(b"an dypelaihin bcklasrhcc ohh")), 1)
        self.assertEqual(PHASE.phase_3(c_char_p(b""),c_char_p(b"an dypelaihin bcklasrhcc o")), 1)
        self.assertEqual(PHASE.phase_3(c_char_p(b""),c_char_p(b"adenyl painch kiblah scorch")), 1)
        self.assertEqual(PHASE.phase_3(c_char_p(b"")), 1)

        self.tearDownTest(name='phase3_1')

    def test_3_2(self):
        PHASE = self.setUpTest(name='phases/phase3/phase3_2.yml', var=None)

        self.assertEqual(PHASE.phase_3(c_char_p(b"accrue accuse acedia acetal abater"),c_char_p(b"ccrec aaaacbrec")), 0)
        self.assertEqual(PHASE.phase_3(c_char_p(b"oldies hazzan remaps sunray potato"),c_char_p(b"ldisa sossaoitl")), 0)

        # self.assertEqual(PHASE.phase_3(c_char_p(b""),c_char_p(b"an dypelaihin bcklasrhcc o")), 1)
        # self.assertEqual(PHASE.phase_3(c_char_p(b""),c_char_p(b"adenyl painch kiblah scorch")), 1)
        self.assertEqual(PHASE.phase_3(c_char_p(b"oldies hazzan remaps sunray potato")), 1)

        self.tearDownTest(name='phase3_2')

    def test_3_3(self):
        PHASE = self.setUpTest(name='phases/phase3/phase3_3.yml', var='{5, 4, 3}')

        self.assertEqual(PHASE.phase_3(c_char_p(b"adenyl painch kiblah scorch purple norths"),c_char_p(b"adenyinch lah sh pur nort")), 0)

        self.assertEqual(PHASE.phase_3(c_char_p(b"adenyl painch kiblah scorch purple norths")), 1)
        self.assertEqual(PHASE.phase_3(c_char_p(b"adenyl painch kiblah scorch purple norths"),c_char_p(b"adenyinch lah sh pur nortt")), 1)

        self.tearDownTest(name='phase3_3')

    def test_3_4(self):
        PHASE = self.setUpTest(name='phases/phase3/phase3_4.yml', var='109')

        self.assertEqual(PHASE.phase_3(c_char_p(b"accrue accuse acedia acetal"),c_char_p(b"acclratue acceucse acedia a")), 0)
        self.assertEqual(PHASE.phase_3(c_char_p(b"adenyl painch kiblah scorch"),c_char_p(b"adehncroyl cspai nch kiblah")), 0)
        self.assertEqual(PHASE.phase_3(c_char_p(b"oldies hazzan remaps sunray"),c_char_p(b"oldieys haarnusz spzaan mre")), 0)

        self.assertEqual(PHASE.phase_3(c_char_p(b"oldies hazzan remaps sunray")),1)

        self.tearDownTest(name='phase3_4')

    def test_3_5(self):
        PHASE = self.setUpTest(name='phases/phase3/phase3_5.yml', var='{4,3}')

        self.assertEqual(PHASE.phase_3(c_char_p(b"accrue accuse acedia acetal abater"),c_char_p(b"arcecsad elbtar ceciat ecuau eacaa")), 0)
        self.assertEqual(PHASE.phase_3(c_char_p(b"adenyl painch kiblah scorch purple"),c_char_p(b"reelackl ohupan ihiasr ldypn bhccp")), 0)
        
        self.assertEqual(PHASE.phase_3(c_char_p(b"accrue accuse acedia acetal abater")),1)

        self.tearDownTest(name='phase3_5')

    def test_4_2(self):
        PHASE = self.setUpTest(name='phases/phase4/phase4_2.yml', var='{23,56,37,12,94,17,66,32,12,66}')

        self.assertEqual(PHASE.phase_4(c_char_p(b"1"),c_char_p(b"0-3 1-8 2-5 4-7 6-8 7-9 7-8 8-7 0-5 0-5 :12")), 0)

        self.assertEqual(PHASE.phase_4(c_char_p(b"1")), 1)

        self.tearDownTest(name='phase4_2')

    def test_4_3(self):
        PHASE = self.setUpTest(name='phases/phase4/phase4_3.yml', var='{5,62,47,36,67,34,64,80,49,90,55,12,34,12,55,46}')

        self.assertEqual(PHASE.phase_4(c_char_p(b"222222222222"),c_char_p(b"nndnddndnddn")), 0)
        self.assertEqual(PHASE.phase_4(c_char_p(b"!P!P!P!P!P!P"),c_char_p(b"nnsnsddnnnsn")), 0)
        self.assertEqual(PHASE.phase_4(c_char_p(b"\r!&%\n0TT\0A]&"),c_char_p(b"nsdsssdnsnnn")), 0)
  
        self.assertEqual(PHASE.phase_4(c_char_p(b"222222222222"),c_char_p(b"nndssssssssn")), 1)
        self.assertEqual(PHASE.phase_4(c_char_p(b"\r!&%\n0TT\0A]&"),c_char_p(b"nsdsssdnnnnn")), 1)
        self.assertEqual(PHASE.phase_4(c_char_p(b"\r!&%\n0TT\0A]&")), 1)

        self.tearDownTest(name='phase4_3')
    
if __name__ == '__main__':
    unittest.main()
