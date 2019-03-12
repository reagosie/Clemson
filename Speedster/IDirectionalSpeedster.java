package cpsc2150.Speedster;

/**
        This interface tracks travel of an object and finds the total distance (sum of all distances traveled, ignoring direction),
        total time to travel, net distance (straight line distance from start to end point), average speed (total distance / total time), and net speed.
        Travel is tracked by considering points on a grid with x,y coordinates. Positive x means it moved east, negative x means it moved west, positive y means
        it moved north and negative y means it moved south. Travel starts at 0,0 and if the object moves
        2 miles south and one mile east, then it has moved to (1, -2).

        Initialization ensures: Starting point is 0,0 and netDistance =0, totalDistance = 0, totalTime = 0, avgSpeed = 0, netSpeed = 0, curXPos = 0, curYPos = 0

        Defines: netDistance: R (real number)
        totalDistance: R
        totalTime: R
        avgSpeed: R
        netSpeed: R
        curXPos: R
        curYPos: R

        constraints : netDistance = [Distance from 0,0 to curXPos, curYPos] AND
        totalDistance >= 0 AND
        totalTime >=0 AND
        (avgSpeed = totalDistance / totalTime OR avgSpeed = 0) AND
        (netSpeed = netDistance / totalTime OR netSpeed = 0)
        */

public interface IDirectionalSpeedster {
    /**
     *
     * @param xChange change in x position. Positive is East, negative is west
     * @param yChange change in y position. Positive is North, negative is south
     * @param time how long this travel took
     * @pre time > 0
     * @post curXPos = #curXPos + xChange and curYPos = #curYPos + yChange
     */
    void addTravel(double xChange, double yChange, double time);

    /**
     *
     * @return the total distance travelled
     * @post getTotalDistance = totalDistance
     */
    double getTotalDistance();

    /**
     *
     * @return the total time
     * @post getTotalTime = totalTime
     */
    double getTotalTime();

    /**
     *
     * @return the net distance
     * @post getNetDistance = netDistance
     */
    double getNetDistance();



    /**
     *
     * @return the average speed over all the travel
     * @post getAverageSpeed = averageSpeed
     */
    double getAverageSpeed();

    /**
     *
     * @return the net speed of the travel
     * @post getNetSpeed = netSpeed
     */
    double getNetSpeed();

    /**
     *
     * @return the current X position
     * @post  getCurXPos = curXPos
     */
    double getCurXPos();

    /**
     *
     * @return the current Y position
     * @post getCurYPos = curYPos
     */
    double getCurYPos();


}
