# Cipher Cracking

**Author: [`Joshua Machado`](https://github.com/JoshoTheMosho)**

**Category: `Crypto Easy`**

## Description

I've been sitting here for days, listening to the radio and writing down dots and dashes. I'm starting to feel like one of those old-timey codebreakers, you know? To be honest, I have no idea what it all means! Think you can help me out?

## Organizers

Provide the text file `1N73rC3P710N.txt`

## Solution

[CyberChef Decode](<https://gchq.github.io/CyberChef/#recipe=From_Morse_Code('Space','Line%20feed')Substitute('FT','01',false)From_Binary('Space',8)From_Base64('A-Za-z0-9%2B/%3D',true,false)Change_IP_format('Dotted%20Decimal','Hex')From_Hex('Auto')From_Base32('A-Z2-7%3D',true)ROT13(true,true,false,19)Reverse('Character')>)
|
[CyberChef Encode](<https://gchq.github.io/CyberChef/#recipe=ROT13(true,true,false,7)Reverse('Character')To_Base32('A-Z2-7%3D')To_Hex('Space',0)Change_IP_format('Hex','Dotted%20Decimal')To_Base64('A-Za-z0-9%2B/%3D')To_Binary('Space',8)Substitute('01','FT',false)To_Morse_Code('-/.','Space','Line%20feed')&input=dmlrZUNURntDMEQzXzhyMzRLM3I1XzYzN19DcjRDSzFONn0>)

Start off with `..-. etc`, Morse code

We then get `FTFFTFFT...`, substitute 0 for F, 1 for T

From `01001001...` we convert from binary

We see `...Rlz=`, ending with `=` and the length being a multiple of 4, indicating base64 encoding.
This gives us nonsense, and will most likely cause everyone issues unless they use the hint.

The hint indicates there's a cipher before the base64 decoding, and a letter relates to the cipher, its key and our CTF.
The letter V is significant as it's the first letter in our CTF name, vikeCTF, and is also the first letter in the cipher name, Vigenere Cipher. The key is also just vikeCTF.

From this we get `...Njg=`, which we can finally use base64 decoding on.

From `72.52.51...` we can convert this from dotted decimal IP format to hex. (Decimal or octal won't work, as we can't go further from decimal or octal)

From `483433464b4d4b` we just convert this from hex because of the last step, also all chars in here are between 1-9 and a-e.

From `H43FKMKSJ...` we might notice all chars are uppercase, an identifier of a base32 encoding.

From `}6U1RJ4yJ_736_5y3R43y8_3K0J{MAJlrpc` we can recognize it is similar to a flag format with the brackets, so we just reverse it.

From `cprlJAM{J0K3_8y34R3y5_637_Jy4JR1U6}` This kinda looks like a flag, so we assume some type of cipher was used. It turns out to be rot13, which it was rotated by 7, the length of 'vikeCTF'. So we rotate by 19 to decrypt it.

Alternatively, we may also notice the last 7 chars look like they could be the start of a flag, and rotate until it becomes recognizable, which it will after 19.

We then have our flag, Congrats!

## Flag

```
vikeCTF{C0D3_8r34K3r5_637_Cr4CK1N6}
```
