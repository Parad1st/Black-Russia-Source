#pragma once

#include <stdint.h>
#include "str_obfuscator_no_template.hpp"

// 2 = 1
#define MAX_SERVERS	2

class CSetServer
{
public:
	class CServerInstanceEncrypted
	{
	public:
		constexpr CServerInstanceEncrypted(const char* str, size_t len, int sum, int port, int isVoice) :
			_buffer{}, _decrypted{ false }, _key{ static_cast<const char>(len % 255) }, _length{ len }, _sum{ sum }, _port{ port }, _isVoice{ isVoice }
		{
			detail::encryptor::encrypt(_buffer, str, _length, _key);
		}
#ifdef _WIN32
		__declspec(noinline)
#elif __GNUC__
		__attribute__((noinline))
#endif
			const char* decrypt() const {
			if (_decrypted) {
				return _buffer;
			}

			for (size_t i = 0; i < _length; i++) {
				_buffer[i] ^= _key;
			}

			_decrypted = true;

			return _buffer;
		}

		char getKey() const {
			return _key;
		}

		const char* getBuffer() const {
			return _buffer;
		}

		int getSum() const
		{
			return _sum;
		}
		int isVoice() const
		{
			return _isVoice;
		}
		int getPort() const
		{
			return _port;
		}

	private:
		mutable char _buffer[31];
		mutable bool _decrypted;
		const char _key;
		size_t _length;
		int _sum;
		int _isVoice;
		int _port;
	};

	static constexpr auto create(const char* str, int sum, size_t len, int port, int isVoice) {
		return CServerInstanceEncrypted(str, len, sum, port, isVoice);
	}
};

#define NUM_TO_STR_IP(a1, a2, a3, a4) \
	#a1 "." #a2 "." #a3 "." #a4, a1 + a2 + a3 + a4

extern const CSetServer::CServerInstanceEncrypted g_sEncryptedAddresses[MAX_SERVERS];
extern const char* g_szServerNames[MAX_SERVERS];