#ifndef LIBORWELL_ZL_VPN_SOCKET_H
#define LIBORWELL_ZL_VPN_SOCKET_H
#include "Network/Socket.h"
#include "MergedBuffer.h"

struct SocketInstance;
typedef uint8_t *(*AllocateFunction)(size_t);
//typedef uint8_t (*socket_receive)(SocketInstance *, uint8_t *, size_t *);
//typedef uint8_t (*socket_send)(SocketInstance *, uint8_t *, size_t);
//typedef uint8_t (*socket_connect)(SocketInstance *, uint8_t *address, uint16_t localPort, uint16_t port);
//typedef bool (*SocketMaySend)(SocketInstance *);

extern "C"
{
    uint8_t zlmedia_socket_receive(SocketInstance *socketInstance, uint8_t *data, size_t *size);
    uint8_t *cpp_allocate_buffer_zero_terminated(size_t size);
    uint8_t zlmedia_socket_receive_allocate(SocketInstance *socketInstance, uint8_t *data, size_t *size, AllocateFunction allocateFunction);
    uint8_t zlmedia_socket_send(SocketInstance *socketInstance, uint8_t *data, size_t size);
    uint8_t zlmedia_socket_connect(SocketInstance *socketInstance, const uint8_t *address, uint16_t localPort, uint16_t port);
    bool zlmedia_socket_may_send(SocketInstance *socketInstance);
}

//TODO: move these to .cpp only, no .h
using namespace toolkit;
//using namespace smoltcp;

class CustomSocket : public Socket
{
private:
    enum SocketType
    {
        TCP,
        UDP
    } _socketType;

public:
    using Ptr = std::shared_ptr<CustomSocket>;

    CustomSocket(const EventPoller::Ptr &poller,
                 SocketInstance *instance,
                 bool enableMutex = true);

    ~CustomSocket();

    void smolSocketPoller();

    //Socket *clone() override;
    //Socket *clone(const EventPoller::Ptr &poller) override;

    void connect(const string &url, uint16_t port, onErrCB con_cb, float timeout_sec = 5,
                 const string &local_ip = "0.0.0.0", uint16_t local_port = 0) override;
    bool listen(uint16_t port, const string &local_ip = "0.0.0.0", int backlog = 1024) override;
    bool bindUdpSock(uint16_t port, const string &local_ip = "0.0.0.0") override;
    void setOnRead(onReadCB cb) override;
    void setOnErr(onErrCB cb) override;
    void setOnAccept(onAcceptCB cb) override;
    void setOnFlush(onFlush cb) override;
    void setOnBeforeAccept(onCreateSocket cb) override;
    bool isConnected();

    //int send(const char *buf, int size = 0, struct sockaddr *addr = nullptr, socklen_t addr_len = 0, bool try_flush = true) override;
    //int send(string buf, struct sockaddr *addr = nullptr, socklen_t addr_len = 0, bool try_flush = true) override;
    int send(toolkit::Buffer::Ptr buf, struct sockaddr *addr = nullptr, socklen_t addr_len = 0, bool try_flush = true) override;

    bool emitErr(const SockException &err) override;
    void enableRecv(bool enabled) override;
    int rawFD() const override;

    string get_local_ip() override;
    uint16_t get_local_port() override;
    string get_peer_ip() override;
    uint16_t get_peer_port() override;
    string getIdentifier() const override;

    void setSendTimeOutSecond(uint32_t second) override;
    BufferRaw::Ptr obtainBuffer() override;
    bool isSocketBusy() const override;
    const EventPoller::Ptr &getPoller() const override;
    bool cloneFromListenSocket(const Socket &other) override;
    bool setSendPeerAddr(const struct sockaddr *dst_addr, socklen_t addr_len = 0) override;
    void setSendFlags(int flags) override;
    //void setReadBuffer(const BufferRaw::Ptr &readBuffer) override;
    void closeSock() override;
    int getSendBufferCount() override;
    uint64_t elapsedTimeAfterFlushed() override;

protected:
    bool shouldContinue()
    {
        return _shouldContinue.load();
    }
    std::atomic<bool> _shouldContinue;
    SocketInstance *socketInstance;
    //smoltcp::SmolSocket _smolSocket;
    uint16_t srcPort;
    //std::shared_ptr<LOpenVPNClient<Stack>> _lOpenVPNClient;
    EventPoller::Ptr _poller;
    MutexWrapper<recursive_mutex> _mutexEvent;
    MutexWrapper<recursive_mutex> _mutexSock;
    onReadCB _readCB;
    onErrCB _errCB;
    onAcceptCB _acceptCB;
    onFlush _flushCB;
    onCreateSocket _onCreateSocket;
    std::atomic<bool> _enableRecv;
    std::atomic<bool> _canSendSock;
    BufferRaw::Ptr _readBuffer;
    std::shared_ptr<MergedBuffer> currentMergedBuffer;

    //TODO: false?
    std::thread smolSocketPollerThread;
    bool _enableMutex = false;
    std::string _localIp;
    uint16_t _localPort;
    std::string _peerIp;
    uint16_t _peerPort;
};
#endif //LIBORWELL_ZL_VPN_SOCKET_H