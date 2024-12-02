#include "byte_stream.hh"

#include <algorithm>
#include <iterator>
#include <stdexcept>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity)
    : _buffer(), _capacity_size(capacity), _written_count(0), _read_count(0), _end_input(false), _error(false) {}

// Done in 24/11/22
size_t ByteStream::write(const string &data) {
    if (_end_input)
        return 0;

    // size of data being written
    size_t write_size = min(data.size(), remaining_capacity());
    _written_count += write_size;
    for (size_t i = 0; i < write_size; i++) {
        _buffer.push_back(data[i]);
    }

    return write_size;
}

// Done in 24/11/22
//! \param[in] len bytes next of the stream will be copied and then popped
//! \return a string
string ByteStream::read(const size_t len) {
    auto data = peek_output(len);
    pop_output(len);
    return data;
}

// Done in 24/11/22
//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    size_t pop_size = min(len, _buffer.size());
    return string(_buffer.begin(), _buffer.begin() + pop_size);
}

// Done in 24/11/22
//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) { 
    auto pop_size = min(len, buffer_size());
    _read_count += len;
    for (size_t i = 0; i < pop_size; i++)
        _buffer.pop_front();
}

// Done in 24/11/22
void ByteStream::end_input() { _end_input = true; }

// Done in 24/11/22
bool ByteStream::input_ended() const { return _end_input; }

size_t ByteStream::buffer_size() const { return _buffer.size(); }

bool ByteStream::buffer_empty() const { return _buffer.empty(); }

bool ByteStream::eof() const { return _end_input && buffer_empty(); }

size_t ByteStream::bytes_written() const { return _written_count; }

size_t ByteStream::bytes_read() const { return _read_count; }

size_t ByteStream::remaining_capacity() const { return _capacity_size - buffer_size(); }
