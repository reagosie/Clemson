default: cpsc2150/Speedster/DirectionalSpeedster.java cpsc2150/Speedster/IDirectionalSpeedster.java cpsc2150/Speedster/DirectionalSpeedsterTest.java
	javac -cp .:/usr/share/java/junit4.jar cpsc2150/Speedster/DirectionalSpeedster.java cpsc2150/Speedster/IDirectionalSpeedster.java cpsc2150/Speedster/DirectionalSpeedsterTest.java

test: cpsc2150/Speedster/DirectionalSpeedster.class cpsc2150/Speedster/IDirectionalSpeedster.class cpsc2150/Speedster/DirectionalSpeedsterTest.class
	java -cp .:/usr/share/java/junit4.jar org.junit.runner.JUnitCore cpsc2150.Speedster.DirectionalSpeedsterTest

clean:
	rm -f *.class
