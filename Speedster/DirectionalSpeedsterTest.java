package cpsc2150.Speedster;

import org.junit.Test;
import static org.junit.Assert.*;

public class DirectionalSpeedsterTest {
    private final double ERROR = .00000001;

    @Test
    public void constructorTest(){
        DirectionalSpeedster speedster = new DirectionalSpeedster();
        assertTrue(speedster.getTotalDistance() == 0 && speedster.getTotalTime() == 0 && speedster.getCurXPos() == 0 && speedster.getCurYPos() == 0);
    }

    //this is an average input test case, very simple
    @Test
    public void travel_x10_y10_t10(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(10, 10, 10);
        assertTrue(s.getCurXPos() == 10 && s.getCurYPos() == 10);
    }

    //this is an upper boundary test case
    @Test
    public void travel_x100000_y100000_t100000(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(100000, 100000, 100000);
        assertTrue(s.getCurXPos() == 100000 && s.getCurYPos() == 100000);
    }

    //this is an lower boundary test case
    @Test
    public void travel_xNeg10000_yNeg10000_t10000(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(-10000, -10000, 100);
        assertTrue(s.getCurXPos() == -10000 && s.getCurYPos() == -10000);
    }

    //this is a challenging test case to see if addTravel works going forwards and backwards back to back
    @Test
    public void travel_plusMinus_x5_y5_t20(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(10, 10, 10);
        s.addTravel(-5, -5, 10);
        assertTrue(s.getCurXPos() == 5 && s.getCurYPos() == 5);
    }

    //this is a challenging test case to see if addTravel can handle square roots
    @Test
    public void travel_sqrt_xsqrt50_ysqrt50_t10(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(Math.sqrt(50), Math.sqrt(50), 10);
        assertTrue(s.getCurXPos() == Math.sqrt(50) && s.getCurYPos() == Math.sqrt(50));
    }

    //this is a challenging test case to see if addTravel can add some positive travel and subtract so that
    //the total is ultimately negative
    @Test
    public void travel_plusMinusNeg_xNeg10_yNeg10_t30(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(5, 5, 10);
        s.addTravel(-15, -15, 20);
        assertTrue(s.getCurXPos() == -10 && s.getCurYPos() == -10);
    }

    //this is a challenging test case to see if addTravel can handle decimals
    @Test
    public void travel_decimal_x5_y5_t10(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(0.5, 0.5, 10);
        s.addTravel(1.2, 1.8, 5);
        assertTrue(s.getCurXPos() == 1.7 && s.getCurYPos() == 2.3);
    }

    //this is a challenging test case to see if addTravel can just add 0s
    @Test
    public void travel_x0_y0_t0(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(0, 0, 0);
        assertTrue(s.getCurXPos() == 0 && s.getCurYPos() == 0);
    }

    //this is a challenging test case to see if addTravel can just add positive travel to X and negative to Y
    @Test
    public void travel_xplus_yminus_t10(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(10, -15, 10);
        assertTrue(s.getCurXPos() == 10 && s.getCurYPos() == -15);
    }

    //this is a challenging test case to see if addTravel can just add negative travel to X and positive to Y
    @Test
    public void travel_xminus_yplus_t10(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(-7, 1, 43);
        assertTrue(s.getCurXPos() == -7 && s.getCurYPos() == 1);
    }

    //this is an average input test case
    @Test
    public void totaldistance_x10_y10_t10(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(10, 10, 10);
        assertTrue(s.getTotalDistance() == Math.sqrt(200));
    }

    //this is an upper boundary test case
    @Test
    public void totaldistance_x10000_y10000_10000(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(10000, 10000, 10000);
        assertTrue(s.getTotalDistance() == Math.sqrt(200000000));
    }

    //this is a lower boundary test case
    @Test
    public void totaldistance_xNeg10000_yNeg10000_t10000(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(-10000, -10000, 10000);
        assertTrue(s.getTotalDistance() == Math.sqrt(200000000));
    }

    //this is a test case for decimal numbers
    @Test
    public void totaldistance_decimal_x1_y2_t5(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(0.1, 0.1, 0.5);
        assertTrue((s.getTotalDistance() - Math.sqrt(0.02)) < ERROR);
    }

    //this is a test case for square roots
    @Test
    public void totaldistance_sqrt_x50_y50_t100(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(Math.sqrt(50), Math.sqrt(50), 100);
        assertTrue(s.getTotalDistance() == 10);
    }

    //this is an average input test case
    @Test
    public void netdistance_x10_y10_t10(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(10, 10, 10);
        assertTrue(s.getNetDistance() == Math.sqrt(200));
    }

    //this is a test case to test positive travel plus negative travel to get net distance
    @Test
    public void netdistance_x5_y5_t30(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(10, 10, 10);
        s.addTravel(-3, -3, 10);
        s.addTravel(-2, -2, 10);
        assertTrue(s.getNetDistance() == Math.sqrt(50));
    }

    //this is an upper boundary test case
    @Test
    public void netdistance_x10000_y10000_t10(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(10000, 10000, 5);
        s.addTravel(-9999, -9999, 5);
        assertTrue(s.getNetDistance() == Math.sqrt(2));
    }

    //this is a lower boundary test case
    @Test
    public void netdistance_xNeg10000_yNeg10000_t10(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(-10000, -10000, 5);
        s.addTravel(9999, 9999, 5);
        assertTrue(s.getNetDistance() == Math.sqrt(2));
    }

    //this is a test case for decimal numbers
    @Test
    public void netdistance_decimal_x1_y1_t10(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(0.5, 0.5, 0.5);
        s.addTravel(-0.4, -0.4, 0.5);
        assertTrue((s.getNetDistance() - Math.sqrt(0.02)) < ERROR);
    }

    //this is an average input test case
    @Test
    public void avgspeed_x10_y10_t10(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(10, 10, 10);
        assertTrue(s.getAverageSpeed() == Math.sqrt(200)/10);
    }

    //this is an upper boundary test case
    @Test
    public void avgspeed_x10000_y10000_t100(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(10000, 10000, 100);
        assertTrue(s.getAverageSpeed() == Math.sqrt(200000000)/100);
    }

    //this is a lower boundary test case
    @Test
    public void avgspeed_xNeg10000_yNeg10000_t100(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(-10000, -10000, 100);
        assertTrue(s.getAverageSpeed() == Math.sqrt(200000000)/100);
    }

    //this is a test case for decimal numbers
    @Test
    public void avgspeed_decimal_x1_y1_t5(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(0.1, 0.1, 0.5);
        assertTrue((s.getAverageSpeed() - Math.sqrt(0.02)/0.5) < ERROR);
    }

    //this is a test case for square roots
    @Test
    public void avgspeed_sqrt_x50_y50_t10(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(Math.sqrt(50), Math.sqrt(50), 10);
        assertTrue(s.getAverageSpeed() == 1);
    }

    //this is an average input test case
    @Test
    public void netspeed_x110_y110_t20(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(10, 10, 10);
        s.addTravel(100, 100, 10);
        assertTrue(s.getNetSpeed() == ((Math.sqrt(200) + Math.sqrt(20000))/10)/2);
    }

    //this is an upper boundary test case
    @Test
    public void netspeed_x10000_y10000_t100(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(10000, 10000, 50);
        s.addTravel(-9999, -9999, 50);
        assertTrue(s.getNetSpeed() == Math.sqrt(2)/100);
    }

    //this is a lower boundary test case
    @Test
    public void netspeed_xNeg10000_yNeg10000_t100(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(-10000, -10000, 50);
        s.addTravel(9999, 9999, 50);
        assertTrue(s.getNetSpeed() == Math.sqrt(2)/100);
    }

    //this is a test case for decimal numbers
    @Test
    public void netspeed_decimal_x1_y1_t5(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(0.1, 0.1, 0.5);
        assertTrue((s.getNetSpeed() - Math.sqrt(0.02)/0.5) < ERROR);
    }

    //this is a test case for square roots
    @Test
    public void netspeed_sqrt_x50_y50_t10(){
        DirectionalSpeedster s = new DirectionalSpeedster();
        s.addTravel(2*Math.sqrt(50), 2*Math.sqrt(50), 5);
        s.addTravel(-(Math.sqrt(50)), -(Math.sqrt(50)), 5);
        assertTrue(s.getNetSpeed() == 1);
    }
}
