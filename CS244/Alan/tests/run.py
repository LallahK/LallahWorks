import os

errors = False
cat = ['assign', 'body', 'call-expr', 'funcdef-vardef', 'if', 'input', 'leave',
 'output', 'simple', 'statements', 'term', 'while']

for c in cat:
  files = os.listdir(c)
  for test in files:
    sig = os.system('../bin/alanc %s/%s > output.temp 2>&1' % (c, test))
    if sig != 0:
      errors = True
      print('Error in %s %s' % (c, test))
os.system('rm output.temp')
if not errors:
  print('No errors found :)')
