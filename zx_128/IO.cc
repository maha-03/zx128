#include "IO.h"
void IO::io_write(uint16_t address, uint8_t value)
{
	for(auto && loc : _io_io){
		if ((address & 1) == 0) {
			_port_fe = value;

			int lev = (_port_fe >> 3) & 0x03;
			switch(lev) {
			case 0x00: _adrv->set_level(-16384-8192); break;
			case 0x01: _adrv->set_level(-16384); break;
			case 0x02: _adrv->set_level(16384); break;
			case 0x03: _adrv->set_level(16384+8192); break;
			default: ;
			}
		}}
}
void IO::mem_write(uint16_t address, uint8_t value){
	for (auto && loc : _mem_io){
		if ((address & loc.mask)==loc.address){
			loc.device->write(address & ~address, value);
			returm;
		}
	}
}
void IO::io_read(uint16_t address){
	for (auto && loc : _io_io){
		if ((address & loc.mask)==loc.address){
			loc.device->read(address & ~address, value);
			returm;
		}
	}
}

uint8_t IO::mem_read(uint16_t address)
{
	uint8_t tap= (address >> 6);
	uint8_t a = (address >> 8);
	uint8_t p = (address & 0xff);
	for(auto && loc : _mem_io){
		if (p == 0xfe) {
			switch (a) {
			case 0x7f: return (_ear << 6) | _key_matrix[0];
			case 0xbf: return (_ear << 6) | _key_matrix[1];
			case 0xdf: return (_ear << 6) | _key_matrix[2];
			case 0xef: return (_ear << 6) | _key_matrix[3];
			case 0xf7: return (_ear << 6) | _key_matrix[4];
			case 0xfb: return (_ear << 6) | _key_matrix[5];
			case 0xfd: return (_ear << 6) | _key_matrix[6];
			case 0xfe: return (_ear << 6) | _key_matrix[7];
			default: return 0;
			}
		}

	return 0;
}
}/*
void IO::keydown(unsigned row, unsigned col)
{
	_key_matrix[row] &= ~(1<<col);
}

void IO::keyup(unsigned row, unsigned col)
{
	_key_matrix[row] |= (1<<col);
}*/

uint8_t PortFE::read(uint16_t address) {
	uint8_t a = (address >>8);
	uint8_t result = 0xff;
	for (unsigned bit = 0; bit < 8; bit++){
		if (a & (1<<bit)) == 0
				result &= _key_matrix[bit];
	}
}

void PortFE::write(uint16_t address, uint8_t value) {
}

uint8_t IO::io_read(uint16_t address) {
}

void PortFE::keydown(unsigned row, unsigned col) {
}

void PortFE::keyup(unsigned row, unsigned col) {
}

uint8_t PortFE::read(uint16_t address) {
}

void PortFE::write(uint16_t address, uint8_t value) {
}
