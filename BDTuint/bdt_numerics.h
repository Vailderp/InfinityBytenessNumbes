#pragma once
#include <iostream>
struct BDTN_CEIL_8421BCD_NUMBER_DESC
{
	unsigned int byteness;
	bool isNegative;
};

struct BDTN_CEIL_8421BCD_NUMBER
{
	BDTN_CEIL_8421BCD_NUMBER_DESC desc;
	unsigned char* memory;
	unsigned int chunks_count;
};

struct BDTN_CEIL_NUMBER_DESC
{
	unsigned int byteness;
};

struct BDTN_CEIL_NUMBER
{
	BDTN_CEIL_NUMBER_DESC desc;
	unsigned char* memory;
};

inline unsigned int __fastcall BdtnStringLength(const char* bcd_str)
{
	unsigned int length = 0;
	while (bcd_str[length] != '\0')
	{
		length++;
	}
	return length;
}

inline unsigned int __fastcall BdtnCalculate8421BCDByteness(const char* bcd_str)
{
	const unsigned int str_length = BdtnStringLength(bcd_str);
	if (str_length % 2 == 0)
	{
		return str_length / 2;
	}
	else
	{
		return (str_length + 1) / 2;
	}
}

inline void __fastcall BdtnRightShiftMemory(
	unsigned char* memory, 
	const unsigned int length
) {
	static constexpr unsigned char shr_mask = 0B00000001;
	unsigned char preBuffer = 0;
	unsigned char postBuffer = 0;
	for (unsigned int i = 0; i < length; ++i) 
	{
		postBuffer = (memory[i] & shr_mask) << 7;
		memory[i] = memory[i] >> 1;
		memory[i] |= preBuffer;
		preBuffer = postBuffer;
	}
}

inline void __fastcall BdtnLeftShiftMemory(
	unsigned char* memory,
	const unsigned int length
) 
{
	static constexpr unsigned char shl_mask = 0B10000000;
	unsigned char preBuffer = 0;
	unsigned char postBuffer = 0;
	for (unsigned int i = length - 1U; i < length; --i) 
	{
		postBuffer = (memory[i] & shl_mask) >> 7;
		memory[i] = memory[i] << 1;
		memory[i] |= preBuffer;
		preBuffer = postBuffer;
	}
}

inline void __fastcall BdtnCopyMemory(
	unsigned char* dst_memory,
	const unsigned char* src_memory,
	unsigned int length
)
{
	while (length--)
	{
		*dst_memory++ = *src_memory++;
	}
}

template <const unsigned char _BitIndex>
inline constexpr unsigned char __fastcall BdtnReadBit(
	unsigned char byte
)
{
	return (byte >> (7 - _BitIndex)) & 0B00000001;
}

inline unsigned char __fastcall BdtnReadBit(
	unsigned char byte,
	const unsigned char index
)
{
	const unsigned char shr_val = static_cast<unsigned char>(8U - 1U) - index;
	return (byte >> shr_val) & 0B00000001;
}

inline unsigned char __fastcall BdtnReadBit(
	unsigned char* memory,
	const unsigned int index
)
{
	return BdtnReadBit(memory[index / 8U], index % 8U);
}

inline void __fastcall BdtnCreateCeil8421BCDNumber(
	BDTN_CEIL_8421BCD_NUMBER** pp_bcd_number,
	BDTN_CEIL_8421BCD_NUMBER_DESC bcd_desc
)
{
	*pp_bcd_number = new BDTN_CEIL_8421BCD_NUMBER();
	(*pp_bcd_number)->desc = bcd_desc;
	(*pp_bcd_number)->chunks_count = (*pp_bcd_number)->desc.byteness * 2;
	(*pp_bcd_number)->memory = new unsigned char[(*pp_bcd_number)->desc.byteness]{};
}

inline void __fastcall BdtnCreateCeilNumber(
	BDTN_CEIL_NUMBER** pp_ceil_number,
	BDTN_CEIL_NUMBER_DESC ceil_desc
)
{
	*pp_ceil_number = new BDTN_CEIL_NUMBER();
	(*pp_ceil_number)->desc = ceil_desc;
	(*pp_ceil_number)->memory = new unsigned char[(*pp_ceil_number)->desc.byteness] {};
}

inline void __fastcall BdtnDestroyCeil8421BCDNumber(
	BDTN_CEIL_8421BCD_NUMBER* p_bcd_number
)
{
	delete[] p_bcd_number->memory;
	delete p_bcd_number;
}

inline void __fastcall BdtnDestroyCeilNumber(
	BDTN_CEIL_NUMBER* p_ceil_number
)
{
	delete[] p_ceil_number->memory;
	delete p_ceil_number;
}

inline void __fastcall BdtnCeilDecStrToCeil8421BCDNumber(
	BDTN_CEIL_8421BCD_NUMBER* p_bcd_number,
	const char* ceil_dec_str
)
{
	BDTN_CEIL_8421BCD_NUMBER_DESC& bcd_desc = p_bcd_number->desc;
	unsigned int index = bcd_desc.byteness - 1U;
	const char* numeric_string = nullptr;
	numeric_string = ceil_dec_str + bcd_desc.byteness * 2 - 1;
	if (*numeric_string == '\0')
	{
		numeric_string--;
	}
	while (*numeric_string != '\0')
	{
		unsigned char right_4bit = *numeric_string - '0';
		numeric_string--;
		if (*numeric_string != '\0')
		{
			unsigned char left_4bit = *numeric_string - '0';
			numeric_string--;
			left_4bit = left_4bit << static_cast<unsigned char>(8U / 2U);
			unsigned char all_8bit = left_4bit | right_4bit;
			p_bcd_number->memory[index] = all_8bit;
			index--;
		}
		else
		{
			p_bcd_number->memory[index] = right_4bit;
		}
	}
}

//REVERSE DOUBLE DABBLE ALGORITHM
inline void __fastcall BdtnCeil8421BCDNumberToCeilNumber(
	BDTN_CEIL_8421BCD_NUMBER* p_bcd_number,
	BDTN_CEIL_NUMBER* p_ceil_number
)
{
	unsigned char* bcd_memory = p_bcd_number->memory;
	unsigned char* bin_memory = p_ceil_number->memory;

	const unsigned int convert_memory_length = p_bcd_number->desc.byteness + p_ceil_number->desc.byteness;
	unsigned char* convert_memory = new unsigned char[convert_memory_length];
	BdtnCopyMemory(convert_memory, bcd_memory, p_bcd_number->desc.byteness);
	unsigned char* convert_memory_begin_bin = convert_memory + p_bcd_number->desc.byteness;

	const unsigned int bin_memory_bits_count = p_ceil_number->desc.byteness * 8;
	const unsigned int bcd_memory_bits_count = p_bcd_number->desc.byteness * 8;

	for (unsigned int bin_bit_idx = 0; bin_bit_idx < bin_memory_bits_count; bin_bit_idx++)
	{
		BdtnRightShiftMemory(convert_memory, convert_memory_length);
		if (bin_bit_idx != bin_memory_bits_count - 1)
		{
			for (unsigned int bcd_byte_idx = 0; bcd_byte_idx < p_bcd_number->desc.byteness; bcd_byte_idx++)
			{
				unsigned char& curr_bcd_byte = convert_memory[bcd_byte_idx];
				unsigned char bcd_left_chunk = curr_bcd_byte >> 4;
				unsigned char bcd_right_chunk = curr_bcd_byte & 0B00001111;
				if (bcd_left_chunk > 7)
				{
					bcd_left_chunk -= 3;
					curr_bcd_byte = bcd_left_chunk;
					curr_bcd_byte <<= 4;
					curr_bcd_byte += bcd_right_chunk;
				}
				if (bcd_right_chunk > 7)
				{
					curr_bcd_byte -= 3;
				}
			}
		}
	}

	BdtnCopyMemory(bin_memory, convert_memory_begin_bin, p_ceil_number->desc.byteness);
	delete[] convert_memory;
}

//DOUBLE DABBLE ALGORITHM
inline void __fastcall BdtnCeilNumberToCeil8421BCDNumber(
	BDTN_CEIL_NUMBER* p_ceil_number,
	BDTN_CEIL_8421BCD_NUMBER* p_bcd_number
)
{
	unsigned char* bin_memory = p_ceil_number->memory;
	unsigned char* bcd_memory = p_bcd_number->memory;

	const unsigned int convert_memory_length = p_bcd_number->desc.byteness + p_ceil_number->desc.byteness;
	unsigned char* convert_memory = new unsigned char[convert_memory_length] {};
	unsigned char* convert_memory_begin_bin = convert_memory + p_bcd_number->desc.byteness;
	BdtnCopyMemory(convert_memory_begin_bin, bin_memory, p_ceil_number->desc.byteness);

	const unsigned int bin_memory_bits_count = p_ceil_number->desc.byteness * 8;
	const unsigned int bcd_memory_bits_count = p_bcd_number->desc.byteness * 8;

	for (unsigned int bin_bit_idx = 0; bin_bit_idx < bin_memory_bits_count; bin_bit_idx++)
	{
		BdtnLeftShiftMemory(convert_memory, convert_memory_length);
		if (bin_bit_idx != bin_memory_bits_count - 1)
		{
			for (unsigned int bcd_byte_idx = 0; bcd_byte_idx < p_bcd_number->desc.byteness; bcd_byte_idx++)
			{
				unsigned char& curr_bcd_byte = convert_memory[bcd_byte_idx];
				unsigned char bcd_left_chunk = curr_bcd_byte >> 4;
				unsigned char bcd_right_chunk = curr_bcd_byte & 0B00001111;
				if (bcd_left_chunk > 4)
				{
					bcd_left_chunk += 3;
					curr_bcd_byte = bcd_left_chunk;
					curr_bcd_byte <<= 4;
					curr_bcd_byte += bcd_right_chunk;
				}
				if (bcd_right_chunk > 4)
				{
					curr_bcd_byte += 3;
				}
			}
		}
	}

	BdtnCopyMemory(bcd_memory, convert_memory, p_bcd_number->desc.byteness);
	delete[] convert_memory;
}

template<unsigned char _BitMask>
inline constexpr void  BdtnComputeBit(
	mutable bool& transbyted, 
	unsigned char* temp_memory,
	const BDTN_CEIL_NUMBER* p_src_ceil_first,
	const BDTN_CEIL_NUMBER* p_src_ceil_second,
	long long byte_idx
	)
{
	const unsigned char upper_bit = p_src_ceil_first->memory[byte_idx] & _BitMask;
	const unsigned char lower_bit = p_src_ceil_second->memory[byte_idx] & _BitMask;
	if (upper_bit == _BitMask && lower_bit == _BitMask)
	{
		if (transbyted == true)
		{
			temp_memory[byte_idx] += static_cast<unsigned char>(_BitMask);
		}
		else
		{
			temp_memory[byte_idx] += static_cast<unsigned char>(0B00000000);
		}
		transbyted = true;
	}
	else if ((upper_bit == 0B00000000 && lower_bit == _BitMask) || (upper_bit == _BitMask && lower_bit == 0B00000000))
	{
		if (transbyted == true)
		{
			temp_memory[byte_idx] += static_cast<unsigned char>(0B00000000);
		}
		else
		{
			temp_memory[byte_idx] += static_cast<unsigned char>(_BitMask);
		}
	}
	else if (upper_bit == 0B00000000 && lower_bit == 0B00000000)
	{
		if (transbyted == true)
		{
			temp_memory[byte_idx] += static_cast<unsigned char>(_BitMask);
			transbyted = false;
		}
		else
		{
			temp_memory[byte_idx] += static_cast<unsigned char>(0B00000000);
		}
	}
}

inline void __fastcall BdtnSumCeilNumber(
	BDTN_CEIL_NUMBER* p_dst_ceil,
	const BDTN_CEIL_NUMBER* p_src_ceil_first,
	const BDTN_CEIL_NUMBER* p_src_ceil_second
)
{
#ifdef _DEBUG
	if (
		p_dst_ceil->desc.byteness != p_src_ceil_first->desc.byteness || 
		p_dst_ceil->desc.byteness != p_src_ceil_second->desc.byteness ||
		p_src_ceil_first->desc.byteness != p_src_ceil_second->desc.byteness
		)
	{
		throw;
	}
#endif // DEBUG
	unsigned char* temp_memory = new unsigned char[p_dst_ceil->desc.byteness]{};
	bool transbyted = false;
	for (long long byte_idx = p_dst_ceil->desc.byteness - 1; byte_idx > -1; byte_idx--)
	{
		BdtnComputeBit<0B00000001>(transbyted, temp_memory, p_src_ceil_first, p_src_ceil_second, byte_idx);
		BdtnComputeBit<0B00000010>(transbyted, temp_memory, p_src_ceil_first, p_src_ceil_second, byte_idx);
		BdtnComputeBit<0B00000100>(transbyted, temp_memory, p_src_ceil_first, p_src_ceil_second, byte_idx);
		BdtnComputeBit<0B00001000>(transbyted, temp_memory, p_src_ceil_first, p_src_ceil_second, byte_idx);
		BdtnComputeBit<0B00010000>(transbyted, temp_memory, p_src_ceil_first, p_src_ceil_second, byte_idx);
		BdtnComputeBit<0B00100000>(transbyted, temp_memory, p_src_ceil_first, p_src_ceil_second, byte_idx);
		BdtnComputeBit<0B01000000>(transbyted, temp_memory, p_src_ceil_first, p_src_ceil_second, byte_idx);
		BdtnComputeBit<0B10000000>(transbyted, temp_memory, p_src_ceil_first, p_src_ceil_second, byte_idx);
	}

	BdtnCopyMemory(p_dst_ceil->memory, temp_memory, p_dst_ceil->desc.byteness);
	delete[] temp_memory;
}

inline char* __fastcall BdtnCeil8421BCDNumberToDecStr(const BDTN_CEIL_8421BCD_NUMBER* p_bcd_number)
{
	char* dec_str = new char[p_bcd_number->chunks_count + 1];
	dec_str[p_bcd_number->chunks_count] = '\0';
	for (unsigned int byte_idx = 0; byte_idx < p_bcd_number->desc.byteness; byte_idx++)
	{
		unsigned char curr_bcd_byte = p_bcd_number->memory[byte_idx];
		unsigned char bcd_left_chunk = curr_bcd_byte >> 4;
		unsigned char bcd_right_chunk = curr_bcd_byte & 0B00001111;
		const unsigned int chunk_idx = byte_idx * 2;
		dec_str[chunk_idx + 0] = bcd_left_chunk + '0';
		dec_str[chunk_idx + 1] = bcd_right_chunk + '0';
	}
	return dec_str;
}

inline char* __fastcall BdtnCeil8421BCDNumberToBinStr(const BDTN_CEIL_8421BCD_NUMBER* p_bcd_number)
{
	char* bin_str = new char[p_bcd_number->desc.byteness * 8 + 1] {};
	bin_str[p_bcd_number->desc.byteness * 8] = '\0';
	for (unsigned int byte_idx = 0; byte_idx < p_bcd_number->desc.byteness; byte_idx++)
	{
		unsigned char curr_bcd_byte = p_bcd_number->memory[byte_idx];
		const unsigned int chunk_idx = byte_idx * 8;
		bin_str[chunk_idx + 0] = BdtnReadBit<0>(curr_bcd_byte) + '0';
		bin_str[chunk_idx + 1] = BdtnReadBit<1>(curr_bcd_byte) + '0';
		bin_str[chunk_idx + 2] = BdtnReadBit<2>(curr_bcd_byte) + '0';
		bin_str[chunk_idx + 3] = BdtnReadBit<3>(curr_bcd_byte) + '0';
		bin_str[chunk_idx + 4] = BdtnReadBit<4>(curr_bcd_byte) + '0';
		bin_str[chunk_idx + 5] = BdtnReadBit<5>(curr_bcd_byte) + '0';
		bin_str[chunk_idx + 6] = BdtnReadBit<6>(curr_bcd_byte) + '0';
		bin_str[chunk_idx + 7] = BdtnReadBit<7>(curr_bcd_byte) + '0';
	}
	return bin_str;
}

inline char* __fastcall BdtnCeilNumberToBinStr(const BDTN_CEIL_NUMBER* p_ceil_number)
{
	char* bin_str = new char[p_ceil_number->desc.byteness * 8 + 1]{};
	bin_str[p_ceil_number->desc.byteness * 8] = '\0';
	for (unsigned int byte_idx = 0; byte_idx < p_ceil_number->desc.byteness; byte_idx++)
	{
		unsigned char curr_bcd_byte = p_ceil_number->memory[byte_idx];
		const unsigned int chunk_idx = byte_idx * 8;
		bin_str[chunk_idx + 0] = BdtnReadBit<0>(curr_bcd_byte) + '0';
		bin_str[chunk_idx + 1] = BdtnReadBit<1>(curr_bcd_byte) + '0';
		bin_str[chunk_idx + 2] = BdtnReadBit<2>(curr_bcd_byte) + '0';
		bin_str[chunk_idx + 3] = BdtnReadBit<3>(curr_bcd_byte) + '0';
		bin_str[chunk_idx + 4] = BdtnReadBit<4>(curr_bcd_byte) + '0';
		bin_str[chunk_idx + 5] = BdtnReadBit<5>(curr_bcd_byte) + '0';
		bin_str[chunk_idx + 6] = BdtnReadBit<6>(curr_bcd_byte) + '0';
		bin_str[chunk_idx + 7] = BdtnReadBit<7>(curr_bcd_byte) + '0';
	}
	return bin_str;
}