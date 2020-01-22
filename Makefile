all : run clean
run : build
	@ echo "Running.."
	@ ./exe/main
build : 
	@ echo "Buiding.."
	@ mkdir -p exe
	@ gcc 2018CS10389_sh.c -o ./exe/2018CS10389_sh
	@ gcc main.c -o ./exe/main
clean : 
	@ echo "Cleaning.."
	@ rm -rf exe
	@ echo "Finshied.."