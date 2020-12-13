#ifndef MEMORY_H_
#define MEMORY_H_
#include <vector>
#include <cstdint>
#include <random>
#include "IO.h"
class Memory
{
protected:
	std::vector<uint8_t> _contents;
public:
	virtual ~Memory() = default;
	virtual void write(unsigned addr, uint8_t value) = 0;
	virtual uint8_t read(unsigned addr) { return _contents[addr % _contents.size()]; }};
class RAM: public Device
{
protected:
	unsigned _width;
	std::vector<uint8_t> _content;
public:
	RAM(unsigned width): _width(width), _content(1 << width) {}
	virtual uint8_t read(uint16_t address) override;
	virtual void write(uint16_t address, uint8_t value) override;};
class ROM: public RAM
{
public:
	ROM(unsigned width, const char * filename, unsigned int page = 0);};
#endif /* MEMORY_H_ */
