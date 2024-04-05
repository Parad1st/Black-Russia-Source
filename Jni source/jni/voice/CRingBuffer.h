#pragma once
#include <mutex>
template<typename T, size_t BufferSize>
class RingBuffer
{
	T _buffer[BufferSize] = { 0 };
	size_t _writeCursor = 0;
	size_t _readCursor = 0;
	std::mutex _accessMutex;
public:
	RingBuffer()
	{
		memset(&_buffer[0], 0, BufferSize * sizeof(T));
	}

	~RingBuffer()
	{
	}

	void Write(const T* buffer, size_t count)
	{
		//_accessMutex.lock();

		if ((_writeCursor + count) > (_readCursor + BufferSize))
		{
			//Buffer autoclear on overflow
			_writeCursor = 0;
			_readCursor = 0;

			if (count > BufferSize)
				count = BufferSize;
		}

		size_t _writeCursorDivided = _writeCursor % BufferSize;
		if ((_writeCursorDivided + count) > BufferSize)
		{
			size_t writeTail = BufferSize - _writeCursorDivided;
			if (writeTail != 0)
				memcpy(&_buffer[_writeCursorDivided], &buffer[0], writeTail * sizeof(T));

			size_t writeHead = count - writeTail;
			if (writeHead != 0)
				memcpy(&_buffer[0], &buffer[writeTail], writeHead * sizeof(T));
		}
		else
			memcpy(&_buffer[_writeCursorDivided], buffer, count * sizeof(T));
		//_accessMutex.unlock();
		_writeCursor += count;
	}

	size_t Read(T* buffer, size_t count)
	{
		//_accessMutex.lock();
		size_t _readCursorDivided = _readCursor % BufferSize;
		if ((_readCursor + count) > _writeCursor)
			count = _writeCursor - _readCursor;

		if ((_readCursorDivided + count) > BufferSize)
		{
			size_t readTail = BufferSize - _readCursorDivided;
			if (readTail != 0)
				memcpy(&buffer[0], &_buffer[_readCursorDivided], readTail * sizeof(T));

			size_t readHead = count - readTail;
			if (readHead != 0)
				memcpy(&buffer[readTail], &_buffer[0], readHead * sizeof(T));
		}
		else
			memcpy(buffer, &_buffer[_readCursorDivided], count * sizeof(T));

		_readCursor += count;
		//_accessMutex.unlock();
		return count;
	}

	size_t BytesToRead()
	{
		//_accessMutex.lock();
		size_t toRead = _writeCursor - _readCursor;
		//_accessMutex.unlock();
		return toRead;
	}

	bool IsHalfFull()
	{
		//_accessMutex.lock();
		bool halfFull = (_writeCursor - _readCursor) > (BufferSize / 2);
		//_accessMutex.unlock();
		return halfFull;
	}

	float GetFillLevel()
	{
		//_accessMutex.lock();
		float fillLevel = (_writeCursor - _readCursor) / (float)BufferSize;
		//_accessMutex.unlock();
		return fillLevel;
	}
};

