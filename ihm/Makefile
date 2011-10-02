# For the moment, need an opa compilator modified for irc :)
# See mattgu74 fork of opalang
#OPA=/home/mguffroy/opa/bin/opa
OPA=/home/matthieu/opa_bin/bin/opa
EXE=main.exe

all: $(EXE)

$(EXE): src/grid.opa src/irc.opa src/main.opa
	$(OPA) $^ -o $(EXE)

run: all
	./$(EXE) --db-local db --port 8181
