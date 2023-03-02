import random
import math

def is_prime(num):
    if num < 2:
        return False
    for i in range(2, int(num ** 0.5) + 1):
        if num % i == 0:
            return False
    return True

def generate_keypair(prime1, prime2):
    if not (is_prime(prime1) and is_prime(prime2)):
        raise ValueError("Both numbers must be prime.")
    elif prime1 == prime2:
        raise ValueError("Both numbers cannot be equal.")

    n = prime1 * prime2
    phi = (prime1 - 1) * (prime2 - 1)

    # Choose a random public key e such that 1 < e < phi and gcd(e, phi) = 1.
    e = random.randrange(1, phi)
    gcd = math.gcd(e, phi)
    while gcd != 1:
        e = random.randrange(1, phi)
        gcd = math.gcd(e, phi)

    # Use the extended Euclidean algorithm to compute the private key d.
    d = pow(e, -1, phi)

    # Return the public and private keypairs
    return ((e, n), (d, n))

def encrypt(message, public_key):
    # Unpack the key into its components
    e, n = public_key

    # Convert each letter in the plaintext to numbers based on the character using a^b mod m
    cipher = [pow(ord(char), e, n) for char in message]

    # Return the array of bytes
    return cipher

def decrypt(cipher, private_key):
    # Unpack the key into its components
    d, n = private_key

    # Generate the plaintext based on the ciphertext and key using a^b mod m
    message = [chr(pow(char, d, n)) for char in cipher]

    # Return the array of bytes as a string
    return ''.join(message)

# import numpy as np

prime1 = 131
prime2 = 151
message = "Hello, world!"
public_key, private_key = generate_keypair(prime1, prime2)
ciphertext = encrypt(message, public_key)
plaintext = decrypt(ciphertext, private_key)
print("Public key:", public_key)
print("Private key:", private_key)
print("Ciphertext:", ciphertext)
print("Plaintext:", plaintext)
