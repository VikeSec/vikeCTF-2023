# Floating Viking Head

**Author: [`Malcolm Seyd`](https://github.com/malcolmseyd)**

**Category: `Reverse Easy`**

## Description

Greetings, mere mortal! Are you ready to embark on a journey to unveil the flag and gain the wisdom of the floating Viking head? Harness the power of the oracle's gaze and use his words to guide you towards victory.

## Organizers

The binary has been committed for convenience, but if you'd like to build your own, here are some instructions.

Dependencies: `gcc` and `make`

To build, run

```console
$ make
```

## Solution

<details> 
  <summary><b>TLDR:</b> Spoiler Warning</summary>

> Use the "From Hex" and "XOR" operations to [solve it with CyberChef](<https://gchq.github.io/CyberChef/#recipe=From_Hex('Auto')XOR(%7B'option':'Hex','string':'0x5d'%7D,'Standard',false)&input=MmIgMzQgMzYgMzggMWUgMDkgMWIgMjYgMzMgNmQgMDIgNjggNmEgMGYgNmMgMzMgNjQgNjggMDIgMWIgNmQgMmYgMDIgMDQgNmQgMjggMjA>).

</details>

Check what kind of file that we have:

```console
$ file
challenge: ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=0ba368fba8b0ecc90712a1df5083cc9bf5ef8abc, for GNU/Linux 4.4.0, not stripped
```

We have a Linux executable! Let's open it up in Ghidra and look at the code. Here is the relevant part of `main`:

```c
iVar1 = 0x1c;
fgets(local_38,0x1c,stdin);
encrypt(local_38,iVar1);
iVar1 = memcmp(&DAT_00102008,local_38,0x1b);
if (iVar1 != 0) {
  fwrite("Failure, your input does not match the flag :(\n",1,0x2f,stderr);
}
else {
  fwrite("Success, your input matches the flag :)\n",1,0x28,stderr);
}
```

It's reads 0x1c bytes from stdin to `local_38`, passes it to `encrypt` with 0x1c as a second parameter, and compares the first 0x1b bytes of `local_38` to some literal bytes.

Let's look at `encrypt`.

```c
void encrypt(char *__block,int __edflag)
{
  ulong local_10;

  for (local_10 = 0; local_10 < 0x1b; local_10 = local_10 + 1) {
    __block[local_10] = __block[local_10] ^ 0x5d;
  }
  return;
}
```

It XOR's each element of the first parameter (which was `local_38`) with the literal `0x5d`.

XOR encryption is reversible by the mathematical property $a \oplus k \oplus k = a$. We have $k=0x5d$ and $ciphertext = input \oplus k = \And \text{DAT} \\_ 00102008$, so we can recover the flag by XOR-ing the first 0x1b bytes of `ciphertext` with `k`.

In Ghidra, we can recover these bytes by double-clicking `&DAT_00102008`, selecting the first line in the listing, pressing `b` for bytes, pressing `[` for create array, typing 0x1b and pressing enter, selecting our new piece of data, right-clicking and clicking Copy Special, and choosing whatever format we'd like.

One option is to choose "Python Byte String", paste it into a Python script, and write a solution to solve it. The code below does this:

```py
ciphertext=b'\x2b\x34\x36\x38\x1e\x09\x1b\x26\x33\x6d\x02\x68\x6a\x0f\x6c\x33\x64\x68\x02\x1b\x6d\x2f\x02\x04\x6d\x28\x20'
plaintext=""
for byte in ciphertext:
    plaintext+=chr(byte^0x5d)
print(plaintext)
```

If CyberChef is more your thing, we can copy as "Byte String" and use the "From Hex" and "XOR" operations to [solve it](<https://gchq.github.io/CyberChef/#recipe=From_Hex('Auto')XOR(%7B'option':'Hex','string':'0x5d'%7D,'Standard',false)&input=MmIgMzQgMzYgMzggMWUgMDkgMWIgMjYgMzMgNmQgMDIgNjggNmEgMGYgNmMgMzMgNjQgNjggMDIgMWIgNmQgMmYgMDIgMDQgNmQgMjggMjA>).

## Flag

```
vikeCTF{n0_57R1n95_F0r_Y0u}
```
