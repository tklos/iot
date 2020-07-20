#ifndef __meo2d20__
#define __meo2d20__


struct o2_measurement {
	int raw;
	float value;
};


class MEO2D20 {
	public:

	MEO2D20(int pin): pin(pin) {
	}

	int get_o2(struct o2_measurement &o2);


	private:

	int pin;
};


#endif
