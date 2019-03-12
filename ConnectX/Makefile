default: src/cpsc2150/ConnectX/GameBoard.java src/cpsc2150/ConnectX/GameBoardMem.java src/cpsc2150/ConnectX/Connect4Game.java src/cpsc2150/ConnectX/IGameBoard.java
	javac src/cpsc2150/ConnectX/GameBoard.java src/cpsc2150/ConnectX/Connect4Game.java src/cpsc2150/ConnectX/IGameBoard.java test/cpsc2150/ConnectX/TestIGameBoard.java

run: src/cpsc2150/ConnectX/Connect4Game.class
	java src.cpsc2150.ConnectX.Connect4Game

test: test/cpsc2150/ConnectX/TestIGameBoard.java
	javac test.cpsc2150.ConnectX.TestIGameBoard.java

runtest: test/cpsc2150/ConnectX/TestIGameBoard.class
	java test.cpsc2150.ConnectX.TestIGameBoard

clean:
	rm -f *.class
