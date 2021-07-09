struct sockaddr {
    unsigned short sa_family; // address family, AF_xxx
    char sa_data[14];         // 14 bytes of protocol address
};

// Internet address (a structure for historical reasons)
struct in_addr {
    unsigned long s_addr; // that's a 32-bit long, or 4 bytes
};

struct sockaddr_in {
    short int sin_family;        // Address family
    unsigned short int sin_port; // Port number
    in_addr sin_addr;            // Internet address
    unsigned char sin_zero[8];   // Same size as struct sockaddr
};
