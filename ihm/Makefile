# For the moment, need an opa compilator modified for irc :)
# See mattgu74 fork of opalang
#OPA=/home/mguffroy/opa/bin/opa
OPA=opa
EXE=main.exe

all: $(EXE)

$(EXE): src/irc.opa src/main.opa
	$(OPA) $^ -o $(EXE)

run: all
	$(EXE) --db-local db --port 8181
