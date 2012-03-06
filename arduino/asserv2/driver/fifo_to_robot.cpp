#include "fifo_to_robot.h"

#include "Arduino.h"
#include "parameters.h"
#include "tools.h"



void FifoObj_to_robot(FifoObj * obj, FifoObj * nextObj)
{
	char err_msg[50];
	
	if (obj == NULL)
	{
		sprintf(err_msg, "Error -- %s (%s:%d) -- obj=NULL", __FUNCTION__, __FILE__, __LINE__);
		Serial.println(err_msg);
	}
	else
	{
		switch (obj->get_t())
		{
			case CMD_GOTO:
			{
				int final_speed=0;
				if (nextObj != NULL and (nextObj->get_t() == CMD_GOTO or nextObj->get_t() == CMD_GOTOR))
					final_speed=nextObj->get_data(2);
				
				Serial.println("coucou");
				Serial.println(obj->get_data(0));
				Serial.println(obj->get_data(1));
				robot.go_to(
					(long int)obj->get_data(0) * ENC_MM_TO_TICKS,
					(long int)obj->get_data(1) * ENC_MM_TO_TICKS,
					convert_speed(obj->get_data(2)),
					convert_speed(final_speed)
				);
				break;
			}
			
			case CMD_GOTOR:
			{
				int final_speed=0;
				if (nextObj != NULL and (nextObj->get_t() == CMD_GOTO or nextObj->get_t() == CMD_GOTOR))
					final_speed=nextObj->get_data(2);
				
				double co = cos(robot.get_a());
				double si = sin(robot.get_a());

				long int dx = (long int)(((double)(obj->get_data(0))*co - (double)(obj->get_data(1))*si) * ENC_MM_TO_TICKS);
				long int dy = (long int)(((double)(obj->get_data(0))*si + (double)(obj->get_data(1))*co) * ENC_MM_TO_TICKS);
				
				robot.go_to(
					robot.get_x() + dx,
					robot.get_y() + dy,
					convert_speed(obj->get_data(2)),
					convert_speed(final_speed)
				);
				break;
			}
			
			case CMD_TURN:
			{
				double angle = moduloPI(((double)obj->get_data(0)) * DEG_TO_RAD);
				robot.turn(
					angle,
					convert_speed(obj->get_data(1))
				);
				break;
			}
			
			case CMD_TURNR:
			{
				double angle = moduloPI(((double)obj->get_data(0)) * DEG_TO_RAD + robot.get_a());
				robot.turn(
					angle,
					convert_speed(obj->get_data(1))
				);
				break;
			}

			case CMD_PWM:
			{
				robot.set_pwm_right_left(obj->get_data(0));
				break;
			}
			
			case CMD_UNDEFINED:
			{
				sprintf(err_msg, "Error -- %s (%s:%d) -- Undefined type", __FUNCTION__, __FILE__, __LINE__);
				Serial.println(err_msg);
				break;
			}
			
			default:
			{
				sprintf(err_msg, "Error -- %s (%s:%d) -- Unknown type : (%d)", __FUNCTION__, __FILE__, __LINE__, obj->get_t());
				Serial.println(err_msg);
				break;
			}
		}
	}
}

