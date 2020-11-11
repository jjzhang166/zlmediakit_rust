#ifndef LIBORWELL_OPENVPN_ZL_SOCKET_UTILS_H
#define LIBORWELL_OPENVPN_ZL_SOCKET_UTILS_H
//#include "SmolTCP.h"
namespace liborwell
{
    static struct sockaddr_in cipaddress_to_ip(smoltcp::CIpAddress &ip, int *len)
    {
        struct sockaddr_in sin;
        if (ip.isIpv4 == 1)
        {
            sin.sin_family = AF_INET;
            std::string ipString = std::to_string(ip.ipv4Address.address[0]) + "." +
                                   std::to_string(ip.ipv4Address.address[1]) + "." +
                                   std::to_string(ip.ipv4Address.address[2]) + "." +
                                   std::to_string(ip.ipv4Address.address[3]);
            ipString = "192.168.1.43";
            sin.sin_addr.s_addr = inet_addr(ipString.c_str());
            sin.sin_port = 0;
            *len = sizeof(sin);
        }
        else
        {
            sin.sin_family = AF_INET;
            std::string ipString = std::to_string(ip.ipv6Address.address[0]) + ":" +
                                   std::to_string(ip.ipv6Address.address[1]) + ":" +
                                   std::to_string(ip.ipv6Address.address[2]) + ":" +
                                   std::to_string(ip.ipv6Address.address[3]) + ":" +
                                   std::to_string(ip.ipv6Address.address[4]) + ":" +
                                   std::to_string(ip.ipv6Address.address[5]) + ":" +
                                   std::to_string(ip.ipv6Address.address[6]) + ":" +
                                   std::to_string(ip.ipv6Address.address[7]);

            sin.sin_addr.s_addr = inet_addr(ipString.c_str());
            sin.sin_port = 0;
        }
        return sin;
    }
} // namespace liborwell
#endif //LIBORWELL_OPENVPN_ZL_SOCKET_UTILS_H