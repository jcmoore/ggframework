#ifndef VAPIDJSON_FILEWRITESTREAM_H_
#define VAPIDJSON_FILEWRITESTREAM_H_

#include "vapidjson.h"
#include <cstdio>

namespace vapidjson {

//! Wrapper of C file stream for input using fread().
/*!
	\implements Stream
*/
class FileWriteStream {
public:
	typedef char Ch;	//!< Character type. Only support char.

	FileWriteStream(IOFILE* fp, char* buffer, size_t bufferSize) : fp_(fp), buffer_(buffer), bufferEnd_(buffer + bufferSize), current_(buffer_) { 
		VAPIDJSON_ASSERT(fp_ != 0);
	}

	void Put(char c) { 
		if (current_ >= bufferEnd_)
			Flush();

		*current_++ = c;
	}

	void PutN(char c, size_t n) {
		size_t avail = bufferEnd_ - current_;
		while (n > avail) {
			memset(current_, c, avail);
			current_ += avail;
			Flush();
			n -= avail;
			avail = bufferEnd_ - current_;
		}

		if (n > 0) {
			memset(current_, c, n);
			current_ += n;
		}
	}

	void Flush() {
		if (current_ != buffer_) {
			fwrite(buffer_, 1, current_ - buffer_, fp_);
			current_ = buffer_;
		}
	}

	// Not implemented
	char Peek() const { VAPIDJSON_ASSERT(false); }
	char Take() { VAPIDJSON_ASSERT(false); }
	size_t Tell() const { VAPIDJSON_ASSERT(false); }
	char* PutBegin() { VAPIDJSON_ASSERT(false); return 0; }
	size_t PutEnd(char*) { VAPIDJSON_ASSERT(false); return 0; }

private:
	IOFILE* fp_;
	char *buffer_;
	char *bufferEnd_;
	char *current_;
};

//! Implement specialized version of PutN() with memset() for better performance.
template<>
inline void PutN(FileWriteStream& stream, char c, size_t n) {
	stream.PutN(c, n);
}

} // namespace vapidjson

#endif // VAPIDJSON_FILESTREAM_H_
