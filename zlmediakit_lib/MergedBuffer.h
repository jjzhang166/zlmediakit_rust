#ifndef LIBORWELL_MERGED_BUFFER_H
#define LIBORWELL_MERGED_BUFFER_H

/*
    An easy way to convert from smoltcp::Buffer to toolkit::Buffer
*/
class MergedBuffer : public toolkit::Buffer
{
public:
    MergedBuffer(uint8_t* buffer, size_t size) : buffer_(buffer), size_(size){};
    virtual ~MergedBuffer(){
        delete[] buffer_;
    };
    //返回数据长度
    virtual char *data() const
    {
        return reinterpret_cast<char *>(this->buffer_);
    }
    virtual uint32_t size() const
    {
        //std::cout << "gonna convert " << smolBuffer->len << " to " << (uint32_t) smolBuffer->len << std::endl;
        return this->size_;
    }

private:
    uint8_t* buffer_;
    size_t size_;
};
#endif //LIBORWELL_MERGED_BUFFER_H