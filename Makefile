default: cpsc2150/ConnectX/GameBoard.java cpsc2150/ConnectX/Connect4Game.java cpsc2150/ConnectX.IGameBoard.java
        javac cpsc2150/ConnectX/GameBoard.java cpsc2150/ConnectX/Connect4Game.java cpsc2150/ConnectX.IGameBoard.java

run: cpsc2150/ConnectX/Connect4Game.class
    java cpsc2150.ConnectX.Connect4Game

clean:
    rm -f *.class