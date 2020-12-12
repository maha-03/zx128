#include <fstream>
#include <cstdint>
#include <functional>
#include <vector>
#include "Z80CPU.h"
#include <libspectrum.h>
#pragma pack(push, 1)

byte cb_mem_read(void* param, ushort address)
{
	auto cpu = reinterpret_cast<Z80CPU*>(param);
	return cpu -> _mem_read(address);
}
byte cb_io_read(void* param, ushort address)
{
	auto cpu = reinterpret_cast<Z80CPU*>(param);
	return cpu -> _io_read(address);
}				ReadFunction _io_read		{[] (uint16_t) -> uint8_t { return 0; }};
void cb_mem_write(void* param, ushort address, byte data)
{
	auto cpu = reinterpret_cast<Z80CPU*>(param);
	cpu -> _mem_write(address);
}
void cb_io_write(void* param, ushort address, byte data)
{
	auto cpu = reinterpret_cast<Z80CPU*>(param);
	cpu -> _io_write(address);
}
Z80CPU::Z80CPU(){
	_context.memParam = _context.ioParam = this;
}
void Z80CPU::reset() {Z80RESET (&_context);
}

void Z80CPU::intr(uint8_t vec) {Z80INT(&_context, vec);
}

void Z80CPU::nmi() {Z80NMIESERT(&_context);
}

void Z80CPU::execute(unsigned tstates) {Z80ExecuteTStates(&_context, tstates)
}
struct SNA_Header
{
	uint8_t I;
	uint16_t HL1;
	uint16_t DE1;
	uint16_t BC1;
	uint16_t AF1;
	uint16_t HL;
	uint16_t DE;
	uint16_t BC;
	uint16_t IY;
	uint16_t IX;
	uint8_t IFF2;
	uint8_t R;
	uint16_t AF;
	uint16_t SP;
	uint8_t IM;
	uint8_t FE;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Z80_Header_1
{
	uint8_t A, F;
	uint8_t C, B;
	uint8_t L, H;
	uint16_t PC;
	uint16_t SP;
	uint8_t I, R;
	uint8_t stuffs1;
	uint8_t E, D;
	uint8_t C1, B1;
	uint8_t E1, D1;
	uint8_t L1, H1;
	uint8_t A1, F1;
	uint8_t IYL, IYH;
	uint8_t IXL, IXH;
	uint8_t IFF1, IFF2;
	uint8_t stuffs2;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Z80_Header_2
{
//	uint16_t hdrlen;
	uint16_t PC;
	uint8_t hw;
	uint8_t ext0; // 7ffd
	uint8_t ext1; // If.1
	uint8_t ext2; // ayyyyy
	uint8_t ay_last_write;
	uint8_t ay_state[16];
	uint16_t tsc_lo;
	uint8_t tsc_hi;
	uint8_t zero; // используется эмулятором QL
	uint8_t whatever[4];
	uint8_t joy[10];
	uint8_t joy2[10];
	uint8_t fsck_those_bytes[4];
};
#pragma pack(pop)

