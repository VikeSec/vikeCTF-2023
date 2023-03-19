# Berserker's Brain Teaser

**Author: [`Malcolm Seyd`](https://github.com/malcolmseyd)**

**Category: `Crypto Medium`**

## Description

Welcome to the "Berserker's Brain Teaser" challenge! The Vikings were known for their fearless and frenzied warriors called berserkers, and this challenge will require you to channel your inner berserker to solve a cryptogram. Can you break the code and decipher the message to claim victory?

> `zexqSNE{cVaLuM_xRxBuRs_vE_mTtAe_ToOiN_oEiK}`

## Organizers

Users should receive a copy of `encrypt.py`, as well as a key generated with the setup instructions below.

Encrypt a flag:

```console
$ python3 src/encrypt.py
text to encrypt:
vikeCTF{ilovepizza}
key: 7 10 25 26 3 22 3
ciphertext:
csjeFPI{skoyaspjya}
```

Any letters are allowed in the input, but only upper and lower case letters will be encrypted.

## Solution

This challenge uses the Vigenere cipher to encrypt the data with a fixed-length key. Since the key isn't longer than the string "vikeCTF", we can brute-force the ciphertext until the string starts with "vikeCTF" and recover the entire flag.

Decrypt a flag:

```console
$ python3 solution/decrypt.py
text to decrypt:
csjeFPI{skoyaspjya}
key: 7 10 25 26 3 22 3
plaintext:
vikeCTF{ilovepizza}
```

## Flag

```
vikeCTF{gIoVaN_bElLaSo_iS_sUpEr_DuPeR_cOoL}
```
