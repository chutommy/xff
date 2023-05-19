#TODO complete

doc: doc src/*.h src/*/*.h
	doxygen Doxyfile

zip: prohlaseni.txt README.md Makefile Doxyfile assets src examples
	zip
