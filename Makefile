APP=lifconfig
BIN=bin
SRC=src
RM=rm -rf


all: $(BIN)/$(APP)

$(BIN):
	mkdir $(BIN)

$(BIN)/$(APP): $(BIN)
	$(CC) $(SRC)/lifconfig.c -o $(BIN)/lifconfig

clean:
	$(RM) $(BIN)
