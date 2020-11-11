#include "CustomSocket.h"
#include "Thread/WorkThreadPool.h"

//#include "utils.h"

#define LOCK_GUARD(mtx) lock_guard<decltype(mtx)> lck(mtx)

static void printBuffer(uint8_t *data, size_t len, std::string c)
{
    static bool debug = false;

    if (debug)
    {
        std::cout << "====================================" << c << std::endl;
        for (size_t i = 0; i < len; i++)
        {
            std::cout << data[i];
        }
        std::cout << std::endl;
        std::cout << "====================================" << c << std::endl;
    }
}

CustomSocket::CustomSocket(const EventPoller::Ptr &poller,
                           SocketInstance *socketInstance,
                           bool enableMutex) : _mutexEvent(enableMutex),
                                               _mutexSock(enableMutex),
                                               socketInstance(socketInstance)
{
    bool isUdp = false;
    _readBuffer = std::make_shared<BufferRaw>(isUdp ? 0xFFFF : 128 * 1024);
    _socketType = SocketType::TCP;
    if (!_poller)
    {
        _poller = EventPollerPool::Instance().getPoller();
    }

    _shouldContinue.store(true);
    smolSocketPollerThread = std::thread(&CustomSocket::smolSocketPoller, this);
}

CustomSocket::~CustomSocket()
{
}

void CustomSocket::smolSocketPoller()
{
    while (shouldContinue())
    {
        //We're using a special allocator for ZLMediaKit, which zero terminates the allocated buffer
        //auto buffer_ = _lOpenVPNClient->receiveWait(_smolSocket, &cpp_allocate_buffer_zero_terminated);
        uint8_t *data;
        size_t *size;
        uint8_t result = zlmedia_socket_receive_allocate(this->socketInstance, data, size, &cpp_allocate_buffer_zero_terminated);
        if (result == 0)
        {
            //auto buffer = buffer_.value().first;
            //TODO: What is this cIpAddress?? I think it'll be needed in the future
            //auto cIpAddress = buffer_.value().second;
            auto mergedBuffer = std::make_shared<MergedBuffer>(data, *size);
            auto &readCB = _readCB;

            _poller->async([&readCB, this, mergedBuffer]() mutable {
                int len;
                //struct sockaddr_in addr = cipaddress_to_ip(cIpAddress, &len);
                LOCK_GUARD(_mutexEvent);
                //std::cout << "gonna call readCB" << std::endl;
                //currentMergedBuffer = mergedBuffer;
                //std::cout << "currentMergedBuffer len " << currentMergedBuffer->size() << std::endl;
                readCB(mergedBuffer, nullptr, 0);
                //std::cout << "readCB called" << std::endl;
            },
                           false);
            //std::cout << "107" << std::endl;
        }
        else
        {
        }
    }
}
/*
Socket *CustomSocket::clone()
{
    std::cout << "------------CLONE HAPPENED!" << std::endl;
    //return new CustomSocket(_lOpenVPNClient, nullptr, _enableMutex);
    std::exit(1);
}

Socket *CustomSocket::clone(const EventPoller::Ptr &poller)
{
    std::cout << "------------CLONE HAPPENED!" << std::endl;
    std::exit(1);
    //return new CustomSocket(_lOpenVPNClient, poller, _enableMutex);
}
*/
//创建tcp客户端，url可以是ip或域名
void CustomSocket::connect(const string &url, uint16_t port, onErrCB con_cb_in, float timeout_sec,
                           const string &local_ip, uint16_t local_port)
{
    //temporary ip version type
    int type = 0;
    if (type == 0)
    {
        std::cout << "CustomSocket::connect to url " << url << " and port " << port << " with localPort " << local_port << std::endl;
        auto poller = _poller;
        WorkThreadPool::Instance().getExecutor()->async([url, port, local_ip, local_port, poller, this, &con_cb_in]() {
            //阻塞式dns解析放在后台线程执行
            bool r = zlmedia_socket_connect(this->socketInstance, reinterpret_cast<const uint8_t *>(url.c_str()), local_port, port);
            if (r)
            {
                //TODO: break after timeoutSec
                while (!isConnected())
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
                std::cout << "Connected!" << std::endl;
                _canSendSock.store(true);
                auto s = SockException(Err_success, "success");
                con_cb_in(s);
                /*
                case UV_ECONNREFUSED:
                    return SockException(Err_refused, uv_strerror(error),error);
                case UV_ETIMEDOUT:
                    return SockException(Err_timeout, uv_strerror(error),error);
                default:
                    return SockException(Err_other, uv_strerror(error),error);
                */
                //connectCB(SockException(Err_success, "success"));
                //con_cb_in
            }
            else
            {
                std::cout << "connection problem" << std::endl;
                auto s = SockException(Err_refused, "", 1);
                con_cb_in(s);
            }
        });
        //TODO: on connection, do _canSendSock = true
    }
}

//创建tcp监听
bool CustomSocket::listen(uint16_t port, const string &local_ip, int backlog)
{
    std::cout << "CustomSocket listen NOT implemented yet " << std::endl;
    std::exit(1);
}

bool CustomSocket::bindUdpSock(uint16_t port, const string &local_ip)
{
    std::cout << "bindUdpSock not implemented yet";
    //TODO: simply recreate the socket here but now in UDP mode.
    //This is how UDP sockets are created with CustomSocket!
    std::exit(1);
    _socketType = SocketType::UDP;
}

void CustomSocket::setOnRead(onReadCB cb)
{
    LOCK_GUARD(_mutexEvent);
    if (cb)
    {
        _readCB = cb;
    }
    else
    {
        _readCB = [](const toolkit::Buffer::Ptr &buf, struct sockaddr *, int) {
            WarnL << "Socket not set readCB";
        };
    }
}

void CustomSocket::setOnErr(onErrCB cb)
{
    LOCK_GUARD(_mutexEvent);
    if (cb)
    {
        _errCB = cb;
    }
    else
    {
        _errCB = [](const SockException &err) {
            WarnL << "Socket not set errCB";
        };
    }
}

void CustomSocket::setOnAccept(onAcceptCB cb)
{
    LOCK_GUARD(_mutexEvent);
    if (cb)
    {
        _acceptCB = cb;
    }
    else
    {
        _acceptCB = [](Socket::Ptr &sock) {
            WarnL << "Socket not set acceptCB";
        };
    }
}

void CustomSocket::setOnFlush(onFlush cb)
{
    LOCK_GUARD(_mutexEvent);
    if (cb)
    {
        _flushCB = cb;
    }
    else
    {
        _flushCB = []() { return true; };
    }
}

void CustomSocket::setOnBeforeAccept(onCreateSocket cb)
{
    LOCK_GUARD(_mutexEvent);
    if (cb)
    {
        _onCreateSocket = cb;
    }
    else
    {
        _onCreateSocket = [](const EventPoller::Ptr &poller) {
            return nullptr;
        };
    }
}

bool CustomSocket::isConnected()
{
    return zlmedia_socket_may_send(this->socketInstance);
}
/*
int CustomSocket::send(const char *buf, int size, struct sockaddr *addr, socklen_t addr_len, bool try_flush)
{
    if (size <= 0)
    {
        size = strlen(buf);
        if (!size)
        {
            return 0;
        }
    }
    BufferRaw::Ptr ptr = obtainBuffer();
    ptr->assign(buf, size);
    return send(ptr, addr, addr_len, try_flush);
}
*/
/*
int CustomSocket::send(string buf, struct sockaddr *addr, socklen_t addr_len, bool try_flush)
{
    return send(std::make_shared<BufferString>(buf), addr, addr_len, try_flush);
}
*/
/*
int CustomSocket::send(string &&buf, struct sockaddr *addr, socklen_t addr_len, bool try_flush)
{
    return send(std::make_shared<BufferString>(std::move(buf)), addr, addr_len, try_flush);
}
*/

int CustomSocket::send(toolkit::Buffer::Ptr buf, struct sockaddr *addr, socklen_t addr_len, bool try_flush)
{
    //printBuffer(reinterpret_cast<uint8_t *>(buf->data()), buf->size(), ">");
    if (_canSendSock.load() == false)
    //if (!isConnected())
    {
        std::cout << "CustomSocket::send ignoring the call, no connection yet" << std::endl;
        return 0;
    }
    else
    {
        //std::cout << "going to send" << std::endl;
    }
    if (addr != nullptr)
    {
        std::cout << "UDP not implemented yet" << std::endl;
        std::exit(1);
    }

    //_lOpenVPNClient->send_copy(_smolSocket, reinterpret_cast<const uint8_t *>(buf->data()), buf->size());
    zlmedia_socket_send(this->socketInstance, reinterpret_cast<uint8_t *>(buf->data()), buf->size());
    return buf->size();
}

bool CustomSocket::emitErr(const SockException &err)
{
    closeSock();

    weak_ptr<CustomSocket> weakSelf = std::dynamic_pointer_cast<CustomSocket>(shared_from_this());
    _poller->async([weakSelf, err]() {
        auto strongSelf = weakSelf.lock();
        if (!strongSelf)
        {
            return;
        }
        LOCK_GUARD(strongSelf->_mutexEvent);
        strongSelf->_errCB(err);
    });
    //TODO: what??
    return true;
}

void CustomSocket::enableRecv(bool enabled)
{
    if (_enableRecv == enabled)
    {
        return;
    }
    _enableRecv = enabled;
}

int CustomSocket::rawFD() const
{
    std::cout << "rawFD called" << std::endl;
    //TODO: return what?
    //if (connected.load()) {
    //    return 0;
    //} else {
    return -1;
    //}
}

string CustomSocket::get_local_ip()
{
    LOCK_GUARD(_mutexSock);
    std::cout << "get_local_ip called" << std::endl;
    return _localIp;
}

uint16_t CustomSocket::get_local_port()
{
    LOCK_GUARD(_mutexSock);
    std::cout << "get_local_port called" << std::endl;

    return _localPort;
}

string CustomSocket::get_peer_ip()
{
    LOCK_GUARD(_mutexSock);
    std::cout << "get_peer_ip called" << std::endl;
    return _peerIp;
}

uint16_t CustomSocket::get_peer_port()
{
    LOCK_GUARD(_mutexSock);
    std::cout << "get_peer_ip called" << std::endl;
    return _peerPort;
}

string CustomSocket::getIdentifier() const
{
    //TODO: enhance this
    return "openvpn";
}

void CustomSocket::setSendTimeOutSecond(uint32_t second)
{
    //_sendTimeOutMS = second * 1000;
    std::cout << "setSendTimeOutSecond called" << std::endl;
}

BufferRaw::Ptr CustomSocket::obtainBuffer()
{
    return std::make_shared<BufferRaw>();
}

bool CustomSocket::isSocketBusy() const
{
    std::cout << "isSocketBusy called" << std::endl;
    return !_canSendSock.load();
}

const EventPoller::Ptr &CustomSocket::getPoller() const
{
    return _poller;
}

bool CustomSocket::cloneFromListenSocket(const Socket &other)
{
    std::cout << "cloneFromListenSocket SHOULDN'T be called" << std::endl;
    std::exit(1);
}

bool CustomSocket::setSendPeerAddr(const struct sockaddr *dst_addr, socklen_t addr_len)
{
    if (_socketType != SocketType::UDP)
    {
        return false;
    }
    else
    {
        //
        std::cout << "UDP not enabled yet" << std::endl;
        std::exit(1);
    }
}

void CustomSocket::setSendFlags(int flags)
{
    //_sock_flags = flags;
    std::cout << "setSendFlags SHOULDN'T be called" << std::endl;
}

/*
void CustomSocket::setReadBuffer(const BufferRaw::Ptr &readBuffer)
{
    if (!readBuffer || readBuffer->getCapacity() < 2)
    {
        return;
    }
    weak_ptr<CustomSocket> weakSelf = std::dynamic_pointer_cast<CustomSocket>(shared_from_this());
    _poller->async([readBuffer, weakSelf]() {
        auto strongSelf = weakSelf.lock();
        if (strongSelf)
        {
            strongSelf->_readBuffer = std::move(readBuffer);
        }
    });
}
*/

void CustomSocket::closeSock()
{
    //TODO
    std::cout << "closeSock called" << std::endl;
}

int CustomSocket::getSendBufferCount()
{
    //TODO: what?
    std::cout << "getSendBufferCount called" << std::endl;

    return 0;
}

uint64_t CustomSocket::elapsedTimeAfterFlushed()
{
    std::cout << "elapsedTimeAfterFlushed called" << std::endl;

    return 0;
}