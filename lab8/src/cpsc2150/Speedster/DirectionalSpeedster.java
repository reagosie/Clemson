/*  Reagan Leonard
    CPSC 2151
    Lab 8
    10/26/18
 */
package cpsc2150.Speedster;

public class DirectionalSpeedster implements IDirectionalSpeedster {
    private double totalTime;
    private double totalDist;
    private double netDist;
    private double curXPos;
    private double curYPos;
    private double avgSpeed;
    private double netSpeed;

    public void DirectionalSpeedster(){
        totalTime = 0;
        totalDist = 0;
        curXPos = 0;
        curYPos = 0;
        avgSpeed = 0;
        netSpeed = 0;
    }

    public void addTravel(double xChange, double yChange, double time){
        curXPos += xChange;
        curYPos += yChange;
        totalTime += time;
        totalDist += Math.sqrt(Math.pow(xChange, 2) + Math.pow(yChange, 2));
//        if(xChange < 0 || yChange < 0){
//            netDist -= totalDist;
//        }
        netDist = Math.sqrt(Math.pow(curXPos, 2) + Math.pow(curYPos, 2));
        avgSpeed = totalDist/totalTime;
        netSpeed = netDist/totalTime;
    }

    public double getTotalDistance(){
        return totalDist;
    }

    public double getTotalTime(){
        return totalTime;
    }

    public double getNetDistance(){
        return netDist;
    }

    public double getAverageSpeed(){
        return totalDist/totalTime;
    }

    public double getNetSpeed(){
        return netDist/totalTime;
    }

    public double getCurXPos(){
        return curXPos;
    }

    public double getCurYPos(){
        return curYPos;
    }
}
