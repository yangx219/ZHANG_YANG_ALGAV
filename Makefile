#!/bin/bash

all:
	@echo "Compiling..."
	gcc ALGAV_Projet.c -o ALGAV_Projet.o 
	@echo "Compiling done"
	@echo "Running..."s
	./ALGAV_Projet.o
	@echo "Running done"

