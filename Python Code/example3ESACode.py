import math
import random

# Generate prime numbers
def generate_primes(nbits):
    while True:
        p = random.getrandbits(nbits)
        if is_prime(p):
            return p

def is_prime(n):
    if n == 2:
        return True
    if n % 2 == 0 or n == 1:
        return False
    s = 0
    d = n - 1
    while d % 2 == 0:
        s += 1
        d //= 2
    for _ in range(10):
        a = random.randint(2, n - 2)
        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            continue
        for _ in range(s - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True

# Compute greatest common divisor
def gcd(a, b):
    while b != 0:
        a, b = b, a % b
    return a

# Compute modular inverse
def mod_inv(a, m):
    if gcd(a, m) != 1:
        return None
    u1, u2, u3 = 1, 0, a
    v1, v2, v3 = 0, 1, m
    while v3 != 0:
        q = u3 // v3
        v1, v2, v3, u1, u2, u3 = (u1 - q * v1), (u2 - q * v2), (u3 - q * v3), v1, v2, v3
    return u1 % m

# Generate RSA key pair
def generate_key_pair(nbits):
    p = generate_primes(nbits // 2)
    q = generate_primes(nbits // 2)
    n = p * q
    phi = (p - 1) * (q - 1)
    e = 65537
    d = mod_inv(e, phi)
    return ((e, n), (d, n))

# Encrypt data using public key
def encrypt(message, public_key):
    e, n = public_key
    m = int.from_bytes(message, 'big')
    c = pow(m, e, n)
    return c.to_bytes((c.bit_length() + 7) // 8, 'big')

# Decrypt data using private key
def decrypt(ciphertext, private_key):
    d, n = private_key
    c = int.from_bytes(ciphertext, 'big')
    m = pow(c, d, n)
    return m.to_bytes((m.bit_length() + 7) // 8, 'big')

# Example usage
message = b"Hello World"
public_key, private_key = generate_key_pair(1024)
ciphertext = encrypt(message, public_key)
plaintext = decrypt(ciphertext, private_key)
print("Ciphertext:", ciphertext)
print("Plaintext:", plaintext.decode('utf-8'))
