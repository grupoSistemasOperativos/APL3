all: cliente servidor


######### ejecutables #########
cliente: cliente.o memoria.o utilitarios.o validaciones.o
	g++ -o bin/cliente obj/cliente.o obj/memoria.o obj/utilitarios.o obj/validaciones.o -lpthread -lrt

servidor: servidor.o memoria.o utilitarios.o validaciones.o
	g++ -o bin/servidor obj/servidor.o obj/memoria.o obj/utilitarios.o obj/validaciones.o -lpthread -lrt

###############################

####### archivos objeto #######
folder:
	mkdir -p obj/

cliente.o: folder src/cliente.cpp 
	g++ -c src/cliente.cpp -o obj/cliente.o

servidor.o: folder src/servidor.cpp
	g++ -c src/servidor.cpp -o obj/servidor.o

validaciones.o: folder src/validaciones.cpp
	g++ -c src/validaciones.cpp -o obj/validaciones.o

memoria.o: folder src/memoria.cpp 
	g++ -c src/memoria.cpp -o obj/memoria.o

utilitarios.o: folder src/utilitarios.cpp
	g++ -c src/utilitarios.cpp -o obj/utilitarios.o

###############################