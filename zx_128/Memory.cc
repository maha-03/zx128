//#include <iostream>
#include <fstream>
#include "Memory.h"
/*static const char *cDEFAULT_ROM_0 = "128-0.ROM";
static const char *cDEFAULT_ROM_1 = "128-1.ROM";*/
uint8_t RAM::read(uint16_t address){
	return _content[address & ((1<<_width)-1)];
}
void RAM::write(uint16_t address, uint8_t velue){
	_content[address & ((1<<_width) - 1)] = value;
}
ROM::ROM(unsigned width< const char *filename, unsigned int page): RAM(width)
{
	unsigned pagesize = 1 << width;
	unsigned offset = pagesize * page;
	std::ifstream file(filename, std::ios::binary);
	if (file.is_open()){
		fie.seekg(offset);
		file.read(reinterpret_cast<char*>(&_content[0]), pagesize);
}}
	/*romfile.open(filename, std::ios::in | std::ios::ate);
	_contents.resize(romfile.tellg());
	romfile.seekg(0);
	romfile.read(reinterpret_cast<char*>(&_contents[0]), _contents.size());
	_addr_mask = _contents.size() - 1;}*/
void AddressSpace::write(unsigned address, uint8_t value, bool io)
{
	if (io == true) _io.write(address, value);
	else _ram.write(address, value);}
uint8_t AddressSpace::read(unsigned address, bool io)
{
	if (io == true)
		return _io.read(address);
	else {
		if (address < 16384) return _rom.read(address);
		else return _ram.read(address);}}
void AddressSpace::write16(unsigned address, uint16_t value)
{
	write(address,     (value >> 0) & 0x00ff);
	write(address + 1, (value >> 8) & 0x00ff);}
uint16_t AddressSpace::read16(unsigned address)
{
	uint16_t result = 0;
	result |= read(address);
	result |= uint16_t(read(address)) << 8;
	return result;}
