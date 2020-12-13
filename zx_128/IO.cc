#include "IO.h"
#include "Memory.h"
IO::IO(){
	port_7ffd = std::make_shared<Port7FFD>();
	port_fe = std::make_shared<PortFE>();

	for (unsigned i = 0; i < 8; i++)
		ram_pages[i] = std::make_shared<RAM>(14);
	rom_pages[0] = std::make_shared<ROM>(14, "128-0.ROM", 0);
	rom_pages[1] = std::make_shared<ROM>(14, "128-1.ROM", 1);

	_mem_io.push_back( IOLocation(0x1c000, 0x00000, ram_pages[0]) );
	_mem_io.push_back( IOLocation(0x1c000, 0x04000, ram_pages[1]) );
	_mem_io.push_back( IOLocation(0x1c000, 0x08000, ram_pages[2]) );
	_mem_io.push_back( IOLocation(0x1c000, 0x0c000, ram_pages[3]) );
	_mem_io.push_back( IOLocation(0x1c000, 0x10000, ram_pages[4]) );
	_mem_io.push_back( IOLocation(0x1c000, 0x14000, ram_pages[5]) );
	_mem_io.push_back( IOLocation(0x1c000, 0x18000, ram_pages[6]) );
	_mem_io.push_back( IOLocation(0x1c000, 0x1c000, ram_pages[7]) );
	//pzu
	_mem_io.push_back( IOLocation(0x1c000, 0x00000, rom_pages[0]) );
	_mem_io.push_back( IOLocation(0x1c000, 0x20000, rom_pages[1]) );

	_io_io.push_back( IOLocation(0xfffff, 0x07ffd, port_7ffd) );
	_io_io.push_back( IOLocation(0x00001, 0x07ffd, port_fe) );
}
void IO::mem_write(uint16_t address, uint8_t value){
	for (auto && loc : _mem_io){
		if ((address & loc.mask)==loc.address){
			loc.device->write(address, value);
			return;}}}
void IO::io_write(uint16_t address, uint8_t value)
{
	for(auto && loc : _io_io){
		if ((address & loc.mask) == loc.address){
			loc.device->write(address, value);
			return;}}}
uint8_t IO::io_read(uint16_t address){
	for (auto && loc : _io_io){
		if ((address & loc.mask)==loc.address){
			return loc.device->read(address);}}return 0;}
uint8_t IO::mem_read(uint16_t addres)
{
	for(auto && loc : _mem_io){
		if((address & loc.mask) == loc.address){
				return loc.device -> read(translate_mem_addr(address));}}return 0;}
uint8_t PortFE::read(uint16_t address) {
	uint8_t a = (address >>8);
	uint8_t result = 0xff;
	for (unsigned bit = 0; bit < 8; bit++){
		if ((a & (1<<bit)) == 0)
				result &= _key_matrix[bit];}
	result &=(_ear << 6) |0b10111111;
	return result;}
void PortFE::write(uint16_t address, uint8_t value) {
}
uint8_t IO::io_read(uint16_t address) {
}
uint32_t IO::translate_mem_addr(uint16_t address){//transl addressov
	uint32_t offset = address & 0x3fff;
	uint32_t real_address = offset;
	unsigned page = (address >> 14) & 0x03;
	if (page == 0){//str pzu
		if (port_7ffd->rom_page() == 0)
			real_address |= 0x20000;
		else real_address |= 0x24000;
	}else if (page ==1) {//str videopam
		if (port_7ffd ->vid_page()==0)
			real_address |= 0x14000;
		else real_address |= 0x1c000;
	}else if (page == 2){//obsh str, second bank
		real_address |= 0x08000;
	}else {
		real_address |= port_7ffd->ram_page()<< 14;
	}

	return real_address;
}

void PortFE::keydown(unsigned row, unsigned col) {
}

void PortFE::keyup(unsigned row, unsigned col) {
}
uint8_t PortFE::read(uint16_t address) {
	uint8_t a = (address >>8);
	uint8_t result = 0xff;
	for (unsigned bit = 0; bit < 8; bit++){
		if ((a & (1<<bit)) == 0)
				result &= _key_matrix[bit];
	}
	result &=(_ear << 6) |0b10111111;
	return result;
}

uint8_t PortFE::read(uint16_t address) {
}

void PortFE::write(uint16_t address, uint8_t value) {
	_border = value & 0x07;
	_mic = !!(value & 0x08);
	_beeper = !!(value & 0x10);
}
