from BitVector import BitVector
from time import time

MAKE = True

""" Used Constants """
AES_MODULUS = BitVector(bitstring='100011011')
TWO = BitVector(intVal=2, size=8)
KEY_LEN = 128
BYTES = KEY_LEN // 8
ROW = 4
COL = BYTES // ROW
ROUNDS = 10 + (KEY_LEN - 128) // 32
FILE_BASE = 'assets/'

""" Used Constant Tables """
SBox = list(map(lambda a: BitVector(intVal=a, size=8), [
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16,
]))

InvSBox = list(map(lambda a: BitVector(intVal=a, size=8), [
    0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D,
]))


def inverse(bv):
    return BitVector(intVal=0, size=8) if int(bv) == 0 else bv.gf_MI(AES_MODULUS, 8)


def build_s_box():
    offset = BitVector(intVal=0x63, size=8)
    for i in range(0, 0x100):
        b = inverse(BitVector(intVal=i, size=8))
        s = offset ^ b ^ (b << 1) ^ (b << 1) ^ (b << 1) ^ (b << 1)
        SBox[i] = s
    for i in range(0, 0x100):
        print(hex(int(SBox[i])), end=' ')
        if i % 0x10 == 0xF:
            print()


def build_inv_s_box():
    offset = BitVector(intVal=0x5, size=8)
    for i in range(0, 0x100):
        s = BitVector(intVal=i, size=8)
        b = offset ^ (s << 1) ^ (s << 2) ^ (s << 3)
        InvSBox[i] = inverse(b)
    # for i in range(0, 0x100):
    #     print(hex(int(InvSBox[i])), end=' ')
    #     if i % 0x10 == 0xF:
    #         print()


multiplication_table = [[
    BitVector(intVal=j, size=8).gf_multiply_modular(BitVector(intVal=i, size=8), AES_MODULUS, 8)
    for i in range(0x100)] for j in range(0x10)
]
left = [[(j + i) % COL for j in range(COL)] for i in range(ROW)]
right = [[(j - i + COL) % COL for j in range(COL)] for i in range(ROW)]


def multiply(bv1, bv2):
    return multiplication_table[int(bv1)][int(bv2)]


class GaloisAESMatrix:

    def __init__(self, shex=None, n=None, m=None, list_2d=None):
        if list_2d is None:
            assert None not in (shex, n, m)
            self.n, self.m = n, m
            self.ar = [[BitVector(size=0) for _ in range(m)] for _ in range(n)]
            idx = 0
            for j in range(m):
                for i in range(n):
                    self.ar[i][j] = BitVector(hexstring=shex[idx:idx + 2])
                    idx += 2
        else:
            assert len(list_2d) > 0
            self.n, self.m = len(list_2d), len(list_2d[0])
            self.ar = list_2d

    def as_line(self, sep=' '):
        return sep.join(sep.join([self.ar[i][j].get_bitvector_in_hex() for i in range(self.n)]) for j in range(self.m))

    def as_matrix(self):
        return '\n'.join(' '.join([bv.get_bitvector_in_hex() for bv in row]) for row in self.ar) + '\n'

    __str__ = as_matrix

    def __xor__(self, other):
        assert self.n == other.n and self.m == other.m
        return GaloisAESMatrix(
            list_2d=[[(self.ar[i][j] ^ other.ar[i][j]) for j in range(self.m)] for i in range(self.n)]
        )

    def __matmul__(self, other):
        assert self.m == other.n
        new_n, new_m, mid = self.n, other.m, self.m
        result = [[BitVector(intVal=0, size=8) for _ in range(new_m)] for _ in range(new_n)]
        for i in range(new_n):
            for j in range(new_m):
                for k in range(mid):
                    result[i][j] ^= multiply(self.ar[i][k], other.ar[k][j])
        return GaloisAESMatrix(list_2d=result)

    def substitute_all(self, substitute_list):
        return GaloisAESMatrix(
            list_2d=[[substitute_list[int(bv)] for bv in row] for row in self.ar]
        )

    def cylindrical_shift(self, index):
        return GaloisAESMatrix(
            list_2d=[[self.ar[i][index[i][j]] for j in range(self.m)] for i in range(self.n)]
        )

    def next_round(self, current_round_const, substitute_list):
        g = [
            substitute_list[int(self.ar[(i + 1) % self.n][-1])] ^ current_round_const[i]
            for i in range(self.n)
        ]

        result = [[BitVector(size=0) for _ in range(self.m)] for _ in range(self.n)]
        for i in range(self.n):
            result[i][0] = self.ar[i][0] ^ g[i]
        for j in range(1, self.m):
            for i in range(self.n):
                result[i][j] = result[i][j - 1] ^ self.ar[i][j]

        next_round_const = current_round_const.copy()
        next_round_const[0] = multiply(TWO, next_round_const[0])

        return GaloisAESMatrix(list_2d=result), next_round_const


Mixer = GaloisAESMatrix(list_2d=[
    [BitVector(hexstring="02"), BitVector(hexstring="03"), BitVector(hexstring="01"), BitVector(hexstring="01")],
    [BitVector(hexstring="01"), BitVector(hexstring="02"), BitVector(hexstring="03"), BitVector(hexstring="01")],
    [BitVector(hexstring="01"), BitVector(hexstring="01"), BitVector(hexstring="02"), BitVector(hexstring="03")],
    [BitVector(hexstring="03"), BitVector(hexstring="01"), BitVector(hexstring="01"), BitVector(hexstring="02")]
])

InvMixer = GaloisAESMatrix(list_2d=[
    [BitVector(hexstring="0E"), BitVector(hexstring="0B"), BitVector(hexstring="0D"), BitVector(hexstring="09")],
    [BitVector(hexstring="09"), BitVector(hexstring="0E"), BitVector(hexstring="0B"), BitVector(hexstring="0D")],
    [BitVector(hexstring="0D"), BitVector(hexstring="09"), BitVector(hexstring="0E"), BitVector(hexstring="0B")],
    [BitVector(hexstring="0B"), BitVector(hexstring="0D"), BitVector(hexstring="09"), BitVector(hexstring="0E")]
])


def text_to_hex(s):
    return s.encode('ascii').hex()


def hex_to_text(s):
    return bytes.fromhex(s).decode('ascii')
    # return ''.join(chr(int(s[i:i+2], 16)) for i in range(0, len(s), 2))


def get_round_keys(key_str):
    round_key = [GaloisAESMatrix(shex=text_to_hex(key_str), n=ROW, m=COL)]
    round_const = list(map(lambda a: BitVector(intVal=a, size=8), [1, 0, 0, 0]))
    print(round_key[0].as_line())
    for _ in range(ROUNDS):
        new_key, round_const = round_key[-1].next_round(round_const, SBox)
        round_key.append(new_key)
        print(new_key.as_line())
    return round_key


def encrypt_block(data, round_keys):
    matrix = GaloisAESMatrix(shex=data, n=ROW, m=COL)

    matrix = matrix ^ round_keys[0]

    for i in range(1, ROUNDS):
        matrix = matrix.substitute_all(SBox)
        matrix = matrix.cylindrical_shift(left)
        matrix = Mixer @ matrix
        matrix = matrix ^ round_keys[i]

    matrix = matrix.substitute_all(SBox)
    matrix = matrix.cylindrical_shift(left)
    matrix = matrix ^ round_keys[-1]

    return matrix.as_line('')


def decrypt_block(cipher, round_keys):
    matrix = GaloisAESMatrix(shex=cipher, n=ROW, m=COL)

    matrix = matrix ^ round_keys[-1]

    for i in range(ROUNDS - 1, 0, -1):
        matrix = matrix.cylindrical_shift(right)
        matrix = matrix.substitute_all(InvSBox)
        matrix = matrix ^ round_keys[i]
        matrix = InvMixer @ matrix

    matrix = matrix.cylindrical_shift(right)
    matrix = matrix.substitute_all(InvSBox)
    matrix = matrix ^ round_keys[0]

    return matrix.as_line('')


def progress(i):
    if i % 1000 == 0:
        print('|', end='')
    elif i % 10 == 0:
        print('.', end='')


def encrypt_whole(data, round_keys):
    cipher = ''
    block_size = 2 * BYTES
    extra = 0
    for i in range(0, len(data), block_size):
        block = data[i:i + block_size]
        if len(block) < block_size:
            extra = block_size - len(block)
            block = block.ljust(block_size, '0')
        cipher += encrypt_block(block, round_keys)
        progress(i)
    print()
    return cipher, extra


def decrypt_whole(cipher, extra, round_keys):
    data = ''
    block_size = 2 * BYTES
    for i in range(0, len(cipher), block_size):
        block = cipher[i:i + block_size]
        data += decrypt_block(block, round_keys)
        progress(i)
    print()
    return data[:-extra] if extra > 0 else data


def encrypt_text(msg, round_keys):
    return encrypt_whole(text_to_hex(msg), round_keys)


def decrypt_text(cipher, extra, round_keys):
    return hex_to_text(decrypt_whole(cipher, extra, round_keys))


def encrypt_file(filename, round_keys):
    try:
        with open(FILE_BASE+filename, 'rb') as f:
            content = f.read().hex()
        return encrypt_whole(content, round_keys)
    except FileNotFoundError:
        print('No such file: \'' + filename + '\'')
        return '', -1


def decrypt_file(cipher, extra, filename, round_keys):
    with open(FILE_BASE+filename, 'wb') as f:
        f.write(bytes.fromhex(decrypt_whole(cipher, extra, round_keys)))


def for_text(round_keys):
    msg_str = input('Enter text: ')

    start = time()
    cipher, extra = encrypt_text(msg_str, round_keys)
    end = time()
    print(cipher)
    print('encryption time: {:.3f}s'.format(end - start))

    start = time()
    msg_back = decrypt_text(cipher, extra, round_keys)
    end = time()
    print(msg_back)
    print('decryption time: {:.3f}s'.format(end - start))


def for_file(round_keys):
    from_file = input('encrypt from: ')

    start = time()
    cipher, extra = encrypt_file(from_file, round_keys)
    end = time()
    print('encryption time: {:.3f}s'.format(end - start))

    if extra == -1:
        return

    into_file = input('decrypt into: ')

    start = time()
    decrypt_file(cipher, extra, into_file, round_keys)
    end = time()
    print('decryption time: {:.3f}s'.format(end - start))


def main():
    if MAKE:
        build_s_box()
        build_inv_s_box()

    key_str = input('Enter key: ')
    if len(key_str) < BYTES:
        key_str = key_str.ljust(BYTES, '$')
    elif len(key_str) > BYTES:
        key_str = key_str[0:BYTES]

    start = time()
    round_keys = get_round_keys(key_str)
    end = time()
    print('Key Scheduling: {:.3f}s'.format(end - start))

    while True:
        option = input('Encrypt (1.Text, 2.File): ')
        if option == '1':
            for_text(round_keys)
        elif option == '2':
            for_file(round_keys)
        elif option == 'q':
            break


if __name__ == '__main__':
    main()
