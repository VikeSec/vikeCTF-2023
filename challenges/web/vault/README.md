# The Vault of Asgard

**Author: [`Kyle Stang`](https://github.com/kylestang)**

**Category: `Web Medium`**

## Description

The flag used to fly high in the morning light, but the undead (bots) kept trying to steal it. Now we keep it in the vault of Asgard itself! No undead fiends dare pass the test at the gates.

## Organizers

Make sure that `users.db` is in the project folder and is not writable. Then set the `JWT_SECRET` environment variable to a long random value. Finally, run the site and give the competitors the URL.

## Solution

<details> 
  <summary><b>TLDR:</b> Spoiler Warning</summary>

> Ignore the CAPTCHA and send a basic `' OR 1=1;--` to the `/login` endpoint in a `POST` request (as detailed in the HTML form)

</details>

Going to the website, it looks like a fairly standard login page except that the captcha says "solves remaining: 50". When we solve the captcha, the count drops to 49. If we keep solving it, the count goes to 2, 1, -1. So that's unhelpful.

Ignoring the captcha, let's take a look at what the site does. The login page consists of a form which sends a post request to `/login` with the username and password field. Meanwhile, login button is disabled with the `disabled` tag.

If we hit the backend, we find it's vulnerable to SQL injection. After some testing, we can solve the challenge with the following steps:

1. **Ignore the captcha.**
2. Remove the `disabled` tag from the `Login` button.
3. Enter, for example, `a' OR 1=1;--` in the user field.
4. Enter anything in the password field.
5. Click `Login`.
6. We're redirected to `/flag` which shows us the flag.

## Flag

```
vikeCTF{0n1y_7h3_w0r7hy_m@y_p@55}
```
