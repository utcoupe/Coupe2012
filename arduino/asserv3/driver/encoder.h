#ifndef ENCODER_H_
#define ENCODER_H_


#include "parameters.h"

#include <inttypes.h>

void initEncoders();

/*The most common type of incremental encoder uses two output channels (A and B) to sense position.
 *  the two output channels of the quadrature encoder indicate both position and direction of rotation.
 *  If A leads B, for example, the disk is rotating in a clockwise direction.
 *  If B leads A, then the disk is rotating in a counter-clockwise direction.
 *  */

void valueChangeOnEncoderLeftPinA();
void valueChangeOnEncoderLeftPinB();
void valueChangeOnEncoderRightPinA();
void valueChangeOnEncoderRightPinB();

extern TICKS G_value_left_enc;
extern TICKS G_value_right_enc;


/*The most common type of incremental encoder uses two output channels (A and B) to sense position.
 *  the two output channels of the quadrature encoder indicate both position and direction of rotation.
 *  If A leads B, for example, the disk is rotating in a clockwise direction.
 *  If B leads A, then the disk is rotating in a counter-clockwise direction.
 *  */
/*
class Encoder
{
	public:
		Encoder();
		void init(int interruptA, int pinA, int interruptB, int pinB);
		int32_t getValue() const;
		void impulseA();
		void impulseB();

	private:
		int32_t _value;
		int _pin_a, _pin_b, _state_a, _state_b;
};
*/


#endif /* ENCODER_H_ */
