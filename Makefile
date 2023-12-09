#!/bin/bash

all:
	@echo "Compiling..."
	gcc ALGAV_Projet.c -o ALGAV_Projet
	@echo "Compiling done"
	@echo "Running..."
	./ALGAV_Projet
	@echo "Running done"

testCle:
	gcc cle.c testCle.c -o testCle
	./testCle

testTas:
	gcc cle.c tas.c testTas.c -o testTas
	./testTas

calculPerformanceTas:
	gcc cle.c tas.c calculPerformanceTas.c -o calculPerformanceTas
	./calculPerformanceTas

testFilesBinomiales:
	gcc filesBinomiales.c testFilesBinomiales.c -o testFilesBinomiales
	./testFilesBinomiales

testTasArbre:
	gcc testTasArbre.c liste.c cle.c tasArbre.c -o tasArbre
	./tasArbre
	
testAbr:
	gcc testAbr.c liste.c cle.c abr.c -o testAbr
	./testAbr

clean:
	rm -f *.o calculPerformanceTas tasArbre testCle testTas testFilesBinomiales 

