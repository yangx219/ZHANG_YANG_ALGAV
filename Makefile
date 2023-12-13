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

testTasTableau:
	gcc cle.c tasTableau.c testTas.c -o testTas
	./testTas

calculPerformanceTasTableau:
	gcc cle.c tasTableau.c liste.c tasArbre.c calculPerformanceTasTableau.c -o calculPerformanceTasTableau
	./calculPerformanceTasTableau

calculPerformanceTasArbreCons:
	gcc cle.c tasTableau.c liste.c tasArbre.c calculPerformanceTasArbreCons.c -o calculPerformanceTasArbreCons
	./calculPerformanceTasArbreCons

calculPerformanceTasArbreAjoutIt:
	gcc cle.c tasTableau.c liste.c tasArbre.c calculPerformanceTasArbreAjoutIt.c -o calculPerformanceTasArbreAjoutIt
	./calculPerformanceTasArbreAjoutIt

calculPerformanceTasArbreUnion:
	gcc cle.c tasTableau.c liste.c tasArbre.c calculPerformanceTasArbreUnion.c -o calculPerformanceTasArbreUnion
	./calculPerformanceTasArbreUnion

performanceFilesBinomiales:
	gcc cle.c tasTableau.c filesBinomiales.c calculPerformanceFB.c -o performanceFilesBinomiales
	./performanceFilesBinomiales

testFilesBinomiales:
	gcc cle.c tasTableau.c filesBinomiales.c testFilesBinomiales.c -o testFilesBinomiales
	./testFilesBinomiales

testTasArbre:
	gcc testTasArbre.c liste.c cle.c tasArbre.c -o tasArbre
	./tasArbre
	
testAbr:
	gcc testAbr.c liste.c cle.c abr.c -o testAbr
	./testAbr

clean:
	rm -f *.o calculPerformanceTasTableau calculPerformanceTasArbreCons calculPerformanceTasArbreAjoutIt calculPerformanceTasArbreUnion tasArbre testCle testTas testFilesBinomiales performanceFilesBinomiales testAbr

