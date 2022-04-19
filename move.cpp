#include <cmath>
#include "math.h"
#include "move.h"

namespace move{


    double distance;
    double time = 0;
    double acceleration_max = 30;
    double deceleration_max = 5;
    double angular_acceleration_max = 100;
    double angular_deceleration_max = 3;
    double angular_velocity_minimum = 10;

    
    bool starting = true;

    double current_distance = 0;
    double remaining_distance = 0;

    double velocity = 0;
    double velocity_max = 36;
    double velocity_minimum = 5;


    double angle_max = 195.5;


    double voltage = 0;
    double lastHeading = 0;
    double voltage_max = 12;

    double dT = 10; 

    void follow_spline(double x2, double y2, double x3, double y3, double x4, double y4){

        vex::timer timer = vex::timer();
        time = 0;

        Spline spline = Spline(0,0, x2, y2, x3, y3, x4, y4);
        
        current_distance = 0;



        distance = spline.getDistance();

        remaining_distance = distance;

        velocity = 0;

        voltage = 0;



        
 
        



        double dT = 10;

        while (true) {

            double acceleration_speed = fmax(acceleration_max * time, velocity_minimum);

            remaining_distance = distance - current_distance;
            double deceleration_speed = remaining_distance * deceleration_max;


            velocity = fmin(acceleration_speed, velocity_max);
            velocity = fmin(velocity, deceleration_speed);


            double distance_percentage = current_distance / distance;

            double heading = spline.getH(distance_percentage);

            double headingDiff = heading - lastHeading;

            lastHeading = heading;


            double leftFactor = 1.0;
            double rightFactor = 1.0;

            double adjustment = fabs(headingDiff * 45);

            if (headingDiff > 0) {
                leftFactor = 1.0 - adjustment;
            }

            else if (headingDiff < 0) {
                rightFactor = 1.0 - adjustment;
            }



            voltage = (velocity / velocity_max) * voltage_max;


            

            if(velocity == deceleration_speed){

              frontLeft.spin(fwd, voltage *leftFactor, voltageUnits::volt);
              backLeft.spin(fwd, voltage *leftFactor, voltageUnits::volt);
              frontRight.spin(fwd, voltage * 0.92 *rightFactor, voltageUnits::volt);
              backRight.spin(fwd, voltage * 0.92 *rightFactor, voltageUnits::volt);
            }

            else{
            frontLeft.spin(fwd, voltage *leftFactor, voltageUnits::volt);
            backLeft.spin(fwd, voltage *leftFactor, voltageUnits::volt);
            frontRight.spin(fwd, voltage *rightFactor, voltageUnits::volt);
            backRight.spin(fwd, voltage*rightFactor, voltageUnits::volt);

            }
          



            task::sleep(dT);

            double new_time = timer.time(sec);

            current_distance += (new_time - time) * velocity;
            time = new_time;


            if (current_distance >= remaining_distance && velocity <= velocity_minimum) {
                frontLeft.stop();
                frontRight.stop();
                backLeft.stop();
                backRight.stop();
               break;
            }

        }
    }


    
}