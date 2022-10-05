//#pragma once
//namespace bdtn
//{
//	namespace _priv
//	{
//		namespace comp
//		{
//			template <const unsigned char _BitIndex>
//			inline unsigned char read_bit(unsigned char byte)
//			{
//				static_assert(_BitIndex <= 7, "_BitIndex can be defined from 0 to 8");
//				return (byte >> (7 - _BitIndex)) & 1;
//			}
//
//			template <const unsigned int _UCeilByteness>
//			inline void __fastcall uceil_str_to_bcd_bin(
//				unsigned char bcd_memory[_UCeilByteness],
//				const char* numerics_string
//			)
//			{
//				unsigned int index = _UCeilByteness - 1;
//				const char* numeric_string = numerics_string + index;
//				while (*numeric_string != '\0')
//				{
//					bcd_memory[index] = *numeric_string - '0';
//					numeric_string--;
//					index--;
//				}
//			}
//
//
//
//		}
//
//		class BdtCeilBase
//		{
//		protected:
//			mutable char* m_stringHex;
//			mutable char* m_stringDec;
//			mutable char* m_stringOct;
//			mutable char* m_stringBin;
//
//			mutable bool m_hexValueChanged;
//			mutable bool m_decValueChanged;
//			mutable bool m_octValueChanged;
//			mutable bool m_binValueChanged;
//
//		public:
//			BdtCeilBase() :
//				m_stringHex(nullptr),
//				m_stringDec(nullptr),
//				m_stringOct(nullptr),
//				m_stringBin(nullptr),
//				m_hexValueChanged(true),
//				m_decValueChanged(true),
//				m_octValueChanged(true),
//				m_binValueChanged(true)
//			{
//
//			}
//
//			const char* toStringHex() const
//			{
//				if (m_hexValueChanged)
//				{
//					this->createStringHex();
//					m_hexValueChanged = false;
//				}
//				return m_stringHex;
//			}
//
//			const char* toStringDec() const
//			{
//				if (m_decValueChanged)
//				{
//					this->createStringDec();
//					m_decValueChanged = false;
//				}
//				return m_stringDec;
//			}
//
//			const char* toStringOct()const
//			{
//				if (m_octValueChanged)
//				{
//					this->createStringOct();
//					m_octValueChanged = false;
//				}
//				return m_stringOct;
//			}
//
//			const char* toStringBin()const
//			{
//				if (m_binValueChanged)
//				{
//					this->createStringBin();
//					m_binValueChanged = false;
//				}
//				return m_stringBin;
//			}
//
//			virtual void createStringHex() const = 0;
//			virtual void createStringDec() const = 0;
//			virtual void createStringOct() const = 0;
//			virtual void createStringBin() const = 0;
//
//			virtual void operator = (const char* numerics_string) = 0;
//
//			void onAssignBase(const char* numerics_string)
//			{
//				m_hexValueChanged = true;
//				m_decValueChanged = true;
//				m_octValueChanged = true;
//				m_binValueChanged = true;
//			}
//		};
//		
//	}
//
//	template <const unsigned int _UCeilByteness, const unsigned int _UCeilChunkByteness>
//	class UCeil : public _priv::BdtCeilBase
//	{
//
//		static_assert(
//			_UCeilChunkByteness == 0 || 
//			_UCeilChunkByteness != sizeof(char) || 
//			_UCeilChunkByteness != sizeof(short) || 
//			_UCeilChunkByteness != sizeof(int) ||
//			_UCeilChunkByteness != sizeof(long) ||
//			_UCeilChunkByteness != sizeof(long long),
//			"_UCeilChunkByteness can only have values of basic types: 1, 2, 4, 8."
//			);
//
//	protected:
//		static inline constexpr const unsigned int m_UCeilByteness = _UCeilByteness;
//		static inline constexpr const unsigned int m_UCeilChunkByteness = _UCeilChunkByteness;
//
//	private:
//		unsigned char m_numberMemory[_UCeilByteness];
//
//	public:
//		UCeil() : 
//			m_numberMemory()
//		{
//			this->m_stringBin = new char[_UCeilByteness * 8 + 1];
//			this->m_stringBin[_UCeilByteness * 8] = '\0';
//		}
//
//		void operator = (const char* numerics_string)
//		{
//			this->onAssignBase(numerics_string);
//			_priv::comp::uceil_str_to_bcd_bin<m__UCeilByteness>(this->m_numberMemory, numerics_string);
//		}
//
//		// Унаследовано через BdtCeilBase
//		virtual void createStringHex() const override
//		{
//			this->m_stringHex;
//		}
//
//		virtual void createStringDec() const override
//		{
//			this->m_stringDec;
//		}
//
//		virtual void createStringOct() const override
//		{
//			this->m_stringOct;
//		}
//
//		virtual void createStringBin() const override
//		{
//			for (unsigned int i = 0U; i < _UCeilByteness; i++)
//			{
//				this->m_stringBin[i * 8 + 0] = _priv::bit_util::read_bit<0>(this->m_numberMemory[i]) + '0';
//				this->m_stringBin[i * 8 + 1] = _priv::bit_util::read_bit<1>(this->m_numberMemory[i]) + '0';
//				this->m_stringBin[i * 8 + 2] = _priv::bit_util::read_bit<2>(this->m_numberMemory[i]) + '0';
//				this->m_stringBin[i * 8 + 3] = _priv::bit_util::read_bit<3>(this->m_numberMemory[i]) + '0';
//				this->m_stringBin[i * 8 + 4] = _priv::bit_util::read_bit<4>(this->m_numberMemory[i]) + '0';
//				this->m_stringBin[i * 8 + 5] = _priv::bit_util::read_bit<5>(this->m_numberMemory[i]) + '0';
//				this->m_stringBin[i * 8 + 6] = _priv::bit_util::read_bit<6>(this->m_numberMemory[i]) + '0';
//				this->m_stringBin[i * 8 + 7] = _priv::bit_util::read_bit<7>(this->m_numberMemory[i]) + '0';
//			}
//		}
//	};
//}