all : run clean
run : build
	@ echo "Running.."
	@ ./exe/main
build : 
	@ echo "Buiding.."
	@ mkdir -p exe
	@ gcc shell.c -o ./exe/shell
	@ gcc main.c -o ./exe/main
clean : 
	@ echo "Cleaning.."
	@ rm -rf exe
	@ echo "Finshied.."
