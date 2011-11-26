#OPA=/home/mguffroy/opa/bin/opa
#OPA=/home/matthieu/opa_bin/bin/opa
OPA=opa
EXE=main.exe
FILES=$(shell find src -name '*.opa')

all: $(EXE)

$(EXE): $(FILES)
	$(OPA) $^ -o $(EXE)

run: all
	./$(EXE) --db-local db --port 8181

clean:
	rm $(EXE)
	rm _build -r
