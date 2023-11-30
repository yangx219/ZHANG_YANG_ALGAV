#!/bin/bash

all:
	@echo "Compiling..."
	gcc ALGAV_Projet.c -o ALGAV_Projet.o 
	@echo "Compiling done"
	@echo "Running..."s
	./ALGAV_Projet.o
	@echo "Running done"

testCle:
	gcc cle.c testCle.c -o testCle
	./testCle

calculPerformanceTas:
	gcc cle.c tas.c calculPerformanceTas.c -o calculPerformanceTas
	./calculPerformanceTas

