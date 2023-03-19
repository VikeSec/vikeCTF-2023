# Docker Dad Jokes

**Author: [`Joshua Machado`](https://github.com/JoshoTheMosho)**

**Category: `Cloud Easy`**

## Description

Did vikings have dad jokes back in the day? Well, I know one thing for sure, these dad jokes are great!

## Organizers

Provide the file `dadJokes.tar.gz`

## Solution

Summary: A simple Docker image which makes a flag.txt, removes it, and also runs a dad joke python program.

Download file `docker load -i dadJokes.tar.gz`

![image](https://user-images.githubusercontent.com/48394084/226062187-976335c9-5207-47a8-9d42-0c6fe08308f3.png)

Viewing the image reveals 22 layers, in which you can see the flag being written to a flag.txt, and also the flag.txt file being removed.

Ideally players will explore the layers through docker or the file system, but it is also `strings`able.

The flag is in the layers, so you can pull it out.

## Flag

```
vikeCTF{D4D_H45_7H3_J0K35}
```
