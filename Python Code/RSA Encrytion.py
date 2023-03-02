import math
import numpy as np

# def gcd(a, h):
#     temp = 0
#     while(1):
#         temp = a % h
#         if (temp == 0):
#             return h
#         a = h
#         h = temp

def gcd(a,b):
    if b > a:
        if b % a == 0:
            return a
        else:
            return gcd(b % a, a)
    else:
        return gcd(b, a % b)

def find_d(phi, e):
    k = 1
    mod0 = False
    while not mod0:
        if (k*phi+1) % e == 0:
            return (k*phi+1)// e
        k=k+1
def find_d2(phi, e):
    return pow(e, -1, phi)

def find_e(phi): 
    e = 3
    while True:
        if not gcd(e, phi) == 1:
            e=e+1
        else:
            if (e<phi):
                return e
            else:
                raise TypeError("Invalid public key")
            
def generate_key_pair(p,q):
    p = p
    q = q
    n = p * q
    phi = (p - 1) * (q - 1)
    e = find_e(phi)
    d = mod_inv(e, phi)
    return ((e, n), (d, n))
            
        
def mod_inv(a, m):
    if gcd(a, m) != 1:
        return None
    u1, u2, u3 = 1, 0, a
    v1, v2, v3 = 0, 1, m
    while v3 != 0:
        q = u3 // v3
        v1, v2, v3, u1, u2, u3 = (u1 - q * v1), (u2 - q * v2), (u3 - q * v3), v1, v2, v3
    return u1 % m

p = 7
q = 13
n = int(p*q)
phi = (p-1)*(q-1)

# while (e < phi):
 
#     # e must be co-prime to phi and
#     # smaller than phi.
#     if(gcd(e, phi) == 1):
#         break
#     else:
#         e = e+1
 
e = find_e(phi)

# Private key (d stands for decrypt)
# choosing d such that it satisfies
# d*e = 1 + k * totient
 

d = find_d2(phi, e)
 
# Message to be encrypted
msg = 12

 
print("Message data = ", msg)
 
# Encryption c = (msg ^ e) % n

def encrypt(message, public_key):
    e, n = public_key
    m = int.from_bytes(message, 'big')
    c = pow(m, e, n)
    return c.to_bytes((c.bit_length() + 7) // 8, 'big')

def decrypt(ciphertext, private_key):
    d, n = private_key
    c = int.from_bytes(ciphertext, 'big')
    m = pow(c, d, n)
    return m.to_bytes((m.bit_length() + 7) // 8, 'big')

#c = np.uint64(math.fmod(pow(msg, e), n))

c = pow(msg, e, n)
# c = c.to_bytes(c.bit_length() + 7 // 8, 'big')
public_key, private_key = generate_key_pair(p, q)
message = b"Hello"
enData = encrypt(message, public_key)
print("Encrypted data = ", c)
print("Encrypted value v2: ", enData)
# Decryption m = (c ^ d) % n

decrData = decrypt(enData, private_key)
print("decrypted Data v2: ", decrData.decode('utf-8'))
#temp2 = pow(c,d,n)
# c = int.from_bytes(enData, 'big')
m = pow(c,d,n)
# decrypyedData = m.to_bytes((m.bit_length()+7)//8, 'big')

print("Original Message Sent = ", m)
