# Smoke on the Horizon

**Author: [`Kyle Stang`](https://github.com/kylestang)**

**Category: `Misc Medium`**

## Description

By Odin's beard, we've spotted some smoke signals between the longship and the village. It's rather cloudy today, so you'll need good eyes. Go find the messages and figure out what they're saying! Quickly!

## Organizers

Give competitors `smoke.pcap`.

Run `make` to compile the `encrypt` and `decrypt` binaries. Use `encrypt` to encrypt the flag with the password. Create a PCAP that contains the password, `decrypt`, and `flag.enc`.

## Solution

Open `smoke.pcap` with Wireshark. There's a HTTPS/TCP stream taking up most of the file. You can use various methods to search the PCAP for the required data. For example, we may notice that most of the noise is using ipv6. If we try the filter `!ipv6`, we get only a few dozen packets.

Searching the PCAP, there are 3 items of interest:

- an email (SMTP)
- an ftp connection (ftp-data)
- a HTTP connection (HTTP)\

First, find the password `p!ll@ge_🔥_p1und3r` in the email body. Then find `flag.enc` in the ftp transfer. Finally, find `decrypt` in the HTTP connection.

Right-click on the data section and click `Extract bytes` for both the `flag.enc` and `decrypt`. Once we have all 3, run `./decrypt flag.enc p!ll@ge_🔥_p1und3r`. The flag will be written to `flag.enc.dec`.

## Flag

```
vikeCTF{C@pt7ur3d_my_p@ckets?_Wh4t5_7h3_r@ns0m?}
```
