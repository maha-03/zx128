#ifndef IO_H_
#define IO_H_

#include <cstdint>
#include "AudioDriver.h"

class IO
{
protected:
	uint8_t _port_fe { 0x00 };
	uint8_t _ear { 0x00 };

//uint8_t _key_matrix[8] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t _key_matrix[8] { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

	AudioDriver * _adrv;

public:
	IO(AudioDriver * drv): _adrv(drv) {}

	void write(unsigned address, uint8_t value);
	uint8_t read(unsigned address) const;

	uint8_t border() const { return _port_fe & 0x07; }

	void keydown(unsigned row, unsigned col);
	void keyup(unsigned row, unsigned col);
	void set_ear(uint8_t ear) { _ear = ear & 1; }
};
class Device
{
public:
	using Ptr = std::shared_ptr<Device>;
	virtual ~Device() = default;

	virtual void write(uint16_t address, uint8_t value) = 0;
	virtual uint8_t read(uint16_t address) = 0;
};
class Port7FFD: public Device
{
protected:
	uint8_t _value;
public:
	virtual uint8_t read(uint16_t adress) override { return _value; }
	virtual void write(uint16_t address, uint8_t value) override {
		if (((_value >> 5) & 1)==0)
			_value = value;
	}
	unsigned ram_page() const { return _value & 0x07; }
	unsigned vid_page() const { return (_value >> 3) & 1; }
	unsigned rom_page() const { return (_value >> 4) & 1; }

	void reset() { _value = 0;}
};
class PortFE: public Device
{

};
#endif /* IO_H_ */
