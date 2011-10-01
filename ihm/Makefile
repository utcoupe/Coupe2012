OPA=/home/mguffroy/opa/bin/opa
EXE=main.exe

all: $(EXE)

main.exe: src/irc.opa src/main.opa
	$(OPA) $^ -o $(EXE)