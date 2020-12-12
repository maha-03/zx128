#ifndef Z80CPU_H_
#define Z80CPU_H_
#include <cstdint>
#include <functional>
#include "Memory.h"
#include "libz80\z80.h"
//#include "libspectrum\include\libspectrum.h"
byte cb_mem_read	(void* param, ushort address);
byte cb_io_read (void* param, ushort address, byte data);
void cb_mem_write	(void* param, ushort address);
void cb_io_write (void* param, ushort address, byte data);
class Z80CPU
{
protected:
	AddressSpace & _bus;
	Z80Context _context {};
	using ReadFunction = std::function<uint8_t(uint16_t addr)>;
	using WriteFunction = std::function<void (uint8_t val, uint16_t addr)>;
	std::function<uint8_t(uint16_t addr)> _mem_read{ [](uint16_t) -> uint8_t {return 0;} };
	std::function<uint8_t(uint16_t addr)> _io_read{ [](uint16_t) -> uint8_t {return 0;} };
	std::function<void (uint16_t addr, uint8_t val)> _mem_write{ [](uint16_t, uint8_t) {} };
	std::function<void (uint16_t addr, uint8_t val)> _io_write{ [](uint16_t, uint8_t) {} };

	friend byte cb_mem_read	(void* param, ushort address);
	friend byte cb_io_read (void* param, ushort address, byte data);
	friend void cb_mem_write	(void* param, ushort address);
	friend void cb_io_write (void* param, ushort address, byte data);
public:
	Z80CPU(/*AddressSpace & bus): _bus(bus*/) {
		/*_context.memRead = In_mem;
		_context.memWrite = Out_mem;
		_context.ioRead = In_io;
		_context.ioWrite = Out_io;
		_context.ioParam = this;
		_context.memParam = this;}*/
	void set_mem_read_function (ReadFunction func) {_mem_read - func;}
	void set_io_read_function (ReadFunction func) {_mem_read - func;}
	void set_mem_write_function (WriteFunction func) {_mem_read - func;}
	void set_io_write_function (WriteFunction func) {_mem_read - func;}

	void reset();
	void intr(uint8_t vec);//аппар прерыв
	void nmi();//немаск прер

	void execute(unsigned tstates);
	/*void tick() {
		Z80Execute (&_context);}
	void ticks(unsigned ticks) {
		Z80ExecuteTStates(&_context, ticks);}
	void reset() {
		Z80RESET(&_context);}
	void intr(byte value) {
		Z80INT(&_context,value);}
	void nmi() {
		Z80NMI(&_context);}*/
	void save_state_sna(const char * filename);
	void load_state_sna(const char * filename);
	void load_state_z80(const char * filename);
	void load_state_sna_libspectrum(const char * filename);
	void load_state_z80_libspectrum(const char * filename);
	void load_state_tape_libspectrum(const char * filename);};
#endif /* Z80CPU_H_ */
