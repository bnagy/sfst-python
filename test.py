# First, run 'python setup.py --install user'

import os, sfst
t = sfst.Transducer('ext/latmor.a')
print("Internal SFST Version %s\n" % t.version())
for word in "gallia est omnis divisa".split():
	print("%s ->\n" % word)
	try:
		r,w = os.pipe()
		t.analyze_string(word, w)
		print(os.fdopen(r).read())
	except sfst.error as err:
		print("[SFST exception - %s]\n" % err)
print("Testing generate")
try:
	r,w = os.pipe()
	t.generate_string("laudare<V><pres><ind><active><sg><1>", w)
	print(os.fdopen(r).read())
except sfst.error as err:
	print("[SFST exception - %s]\n" % err)	
