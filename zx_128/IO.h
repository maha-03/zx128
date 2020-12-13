#ifndef IO_H_
#define IO_H_
#include <cstdint>
#include <memory>
#include <vector>

#include "AudioDriver.h"
class Device
{
public:
	using Ptr = std::shared_ptr<Device>;
	virtual ~Device() = default;

	virtual void write(uint16_t address, uint8_t value) = 0;
	virtual uint8_t read(uint16_t address) = 0;
};
struct IOLocation{
	uint32_t mask;
	uint32_t address;
	Device::Ptr device;

	IOLocation(uint32_t m; uint32_t a; Device::Ptr d):
		mask(m), addreass(a), device(d){}
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
class PortFE: public Device//клавиатура, бипер, бордюр и т.д.
{
protected:
	unsigned _border { 0 };
	unsigned _ear { 0 };
	unsigned _mic { 0 };//реализ порта магнитоф
	unsigned _beeper { 0 };
	uint8_t _key_atrix[8] {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
public:

	unsigned border()const {return _border & 0x07;}
	void keydown(unsigned row, unsigned col);
	void keyup(unsigned row, unsigned col);
	virtual uint8_t read(uint16_t address) override;
	virtual void write(uint16_t address, uint8_t value) override;
};
class IO
{
protected:
	/*//uint8_t _port_fe { 0x00 };
	uint8_t _ear { 0x00 };

//uint8_t _key_matrix[8] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t _key_matrix[8] { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };*/
	std::vector<IOLocation> _mem_io;
	std::vector<IOLocation> _io_io;
public:
	std::shared_ptr<Device> ram_pages[8];
	std::shared_ptr<Device> rom_pages[2];
	std::shared_ptr<Port7FFD> port_7ffd;
	std::shared_ptr<PortFE> port_fe;
public:
	IO();
	uint32_t translate_mem_addr(uint16_t address);
	void mem_write(uint16_t address, uint8_t value);
	uint8_t mem_read(uint16_t address);
	void io_write(uint16_t address, uint8_t value);
	uint8_t io_read(uint16_t address);
	//uint8_t border() const { return _port_fe & 0x07; }

	void reset(){
		port_7ffd->reset();
	}
	void keydown(unsigned row, unsigned col);
	void keyup(unsigned row, unsigned col);
	void set_ear(uint8_t ear) { _ear = ear & 1; }};

#endif /* IO_H_ */
