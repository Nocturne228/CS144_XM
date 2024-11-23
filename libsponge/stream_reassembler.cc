#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity), _stream(capacity), 
                                                              _curr_idx(0), _eof_idx(std::numeric_limits<size_t>::max()), 
                                                              _unassembled_bytes_count(0) {}

// Done in 24/11/23
//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    auto start = max(index, _curr_idx);
    auto end = min(index + data.size(), min(_curr_idx + _capacity - _output.buffer_size(), _eof_idx));

    if (eof) _eof_idx = min(index + data.size(), _eof_idx);
    
    for (size_t i = start, j = start - index; i < end; ++i, ++j) {
        auto &t = _stream[i % _capacity];
        if (t.second == true) {
            if (t.first != data[j]) 
                throw runtime_error("StreamReassembler::push_substring: Inconsistent substrings!");
        } else {
            t = make_pair(data[j], true);
            ++_unassembled_bytes_count;
        }
    }

    string str;
    while (_curr_idx < _eof_idx && _stream[_curr_idx % _capacity].second == true ) {
        str.push_back(_stream[_curr_idx % _capacity].first);
        _stream[_curr_idx % _capacity] = {0, false};
        --_unassembled_bytes_count, ++_curr_idx;
    }

    _output.write(str);
    if (_curr_idx == _eof_idx) _output.end_input();

}

size_t StreamReassembler::unassembled_bytes() const { return _unassembled_bytes_count; }

bool StreamReassembler::empty() const { return unassembled_bytes() == 0; }
