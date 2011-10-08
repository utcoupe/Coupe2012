#OPA=/home/mguffroy/opa/bin/opa
#OPA=/home/matthieu/opa_bin/bin/opa
OPA=opa
EXE=main.exe

all: $(EXE)

$(EXE): src/config.opa src/grid.opa src/irc.opa src/boussole.opa src/main.opa
	$(OPA) $^ -o $(EXE)

run: all
	./$(EXE) --db-local db --port 8181
