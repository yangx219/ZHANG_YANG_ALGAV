#!/bin/bash

all:
	@echo "Compiling..."
	gcc ALGAV_Projet.c -o ALGAV_Projet.o 
	@echo "Compiling done"
	@echo "Running..."
	./ALGAV_Projet.o
	@echo "Running done"

testCle:
	gcc cle.c testCle.c -o testCle
	./testCle

testOperation:
	gcc cle.c tas.c testOperation.c -o testOperation
	./testOperation

calculPerformanceTas:
	gcc cle.c tas.c calculPerformanceTas.c -o calculPerformanceTas
	./calculPerformanceTas

testFilesBinomiales:
	gcc filesBinomiales.c -o testFilesBinomiales
	./testFilesBinomiales

testTasArbre:
	gcc testTasArbre.c liste.c cle.c tasArbre.c -o tasArbre
	./tasArbre

clean:
	rm -f *.o calculPerformanceTas tasArbre testCle testOperation testFilesBinomiales 

