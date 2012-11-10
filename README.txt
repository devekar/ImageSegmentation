Vaibhav Devekar
vaibhav.devekar@gmail.com

Project: Implementation of Network Flow Algorithms in Image Segmentation


How to run:
Compile the cpp files. The filenames of the executables produced may need to be changed in main.py at lines 34/35.
Execute main.py

By default the Edmond-Karp Algorithm is used to segment image. 
To use Push_Relabel Algorithm, use the variable "algo" defined in main.py at lines 34/35
You can also change the image used by editing the path specified in main.py at line 8


Files:
1) main.py  : 	main execution file
2) graphcut.py : 	helper module to create graph from pixels based on bayesian classifier model
3) bayes.py : 	helper module to generate bayesian values
4) Graph_stl.h :  contains implementation of both Edmond-Karp and FIFO push-relabel methods
5) edmond_karp.cpp ,push_relabel.cpp : 	main execution file
6) flower.jpg, empire_low.jpg : Sample image files
7) Thesis.docx - Survey and project details