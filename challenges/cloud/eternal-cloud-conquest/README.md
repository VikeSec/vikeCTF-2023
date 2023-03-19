# Eternal Cloud Conquest - Sponsored by [Mirai Security](https://miraisecurity.com/)

**Author: [`MNThomson`](https://github.com/MNThomson)**

**Category: `Cloud Hard`**

## Description

When we first launched Viking Axe Co™, we utilized a legacy cloud static website to store our battle-axes. At the time, we believed that this was a secure solution, and that our axes were safe from harm. Unfortunately, we were wrong. Our website was hacked, and our precious battle-axes were stolen. It was a devastating blow, and it made us realize the importance of cybersecurity in the modern age.

> http://eternal-cloud-conquest.vikesec.ca

## Organizers

## Solution

<details> 
  <summary><b>TLDR:</b> Spoiler Warning</summary>

> [SSRF](https://portswigger.net/web-security/ssrf) of [AWS EC2](https://aws.amazon.com/ec2/) [Instance Metadata API](https://docs.aws.amazon.com/AWSEC2/latest/UserGuide/instancedata-data-retrieval.html) through the [users profile photo URL](http://169.254.169.254/latest/meta-data/iam/security-credentials/vikeCTF-metadata-role)

</details>

### Reconnaissance

Navigating to the URL provided, we find Viking Axe Co's website. Exploring further we can find three blog posts: `About us`, `A disterous retrospective`, & `Customer Success`. Reading through them, the retrospective seems to contain some interesting information. From this blog post, we can gather the follow:

- This current website is running on [AWS EC2](https://aws.amazon.com/ec2/)

  "`... we turned to AWS for a solution. We built a new EC2 server`"

- The **_external network_** has access controls

  "`... configured the external network access to not allow anyone to access restricted resources`"

Hmm...the "external network" comment is making me think of an [SSRF](https://portswigger.net/web-security/ssrf), not sure yet though.

Also, some blog posts have previous user comments. Inspecting the HTML we can notice that the profile images are data URLs (e.g. `data:image/png;base64,iVBORw0KG...`). Weird, but these are probably just to reduce network requests on static pages.

### Exploration

Now to active recon. Along the nav bar there appears to be a profile image, and upon clicking it, we are taken to `/register` page asking for a `username` & `password`. Registering with any data seems to work (except for the annoying password complexity prompts), no email confirmation required!

Ok, so we now have a user, and it appears we can add some more info to our profile such as:

- `Mothers Maiden Name` (I wonder why a website would ever want that...)
- `Your Birthday` (uhh, ok)
- `Profile Image URL` (ooooooh, interesting)
- `The City you live in` (getting creepier by the minute)

So the `Profile Image URL` looks fascinating, let's try that. Setting it to any example PNG, it appears that we get our profile image in the navbar! Upon closer HTML inspection though, it's just linking to the image directly (so no SSRF for us `:(`, sad).

Alright, nothing there so let's do another lap of recon. Reading the about page doesn't seem to provide anything interesting. Now there is a search bar, but the search is 100% local (as seen by viewing the network tab while typing), so I doubt there is anything there.

Circling back to the blog posts though provides us now with the ability to create a new comment...hello there! When we input some random text and click `New Comment`, the page is reloaded, and we can see our comment rendered. Alongside the comment, is our user specified profile image. And viewing the HTML, it's also a data URL...meaning URL was just requested server side. We've got an SSRF!

### Exploitation

Alright, so we know we have an SSRF, let's exploit it! Looking for local open ports though nothing really sticks out as interesting. So that's extremely weird since gaining access to local services is usually the goal of an SSRF. Alright, what other information do we have?

Wait, we know this is running on EC2, is that an avenue of attack? Bingo, searching for "[`AWS EC2 SSRF`](https://www.google.com/search?q=AWS+EC2+ssrf)" [finds](https://hackingthe.cloud/aws/exploitation/ec2-metadata-ssrf/), [us](https://scalesec.com/blog/exploit-ssrf-to-gain-aws-credentials/), [some](https://book.hacktricks.xyz/pentesting-web/ssrf-server-side-request-forgery/cloud-ssrf), [gold](https://infosecwriteups.com/ssrf-leads-to-access-aws-metadata-21952c220aeb)!

So setting our profile URL to `http://169.254.169.254/` we can go back to the comment section and grab the base64 encoded PNG profile image as `data:image/png;base64,bGF0ZXN0Cg==`

```bash
$ echo -n "bGF0ZXN0Cg==" | base64 -d
latest
```

So lets grab the name of the credentials.

```bash
# http://169.254.169.254/latest/meta-data/iam/security-credentials/
$ echo -n "dmlrZUNURi1tZXRhZGF0YS1yb2xl" | base64 -d
vikeCTF-metadata-role
```

Now to grab the credentials themselves!

```bash
# http://169.254.169.254/latest/meta-data/iam/security-credentials/vikeCTF-metadata-role
$ echo -n "ewoJIkNvZGUiOiAiU3VjY2VzcyIsCgkiTGFzdFVwZGF0ZWQiOiAiMjAyMy0wMy0xN1QxNjowMDowMFoiLAoJIlR5cGUiOiAiQVdTLUhNQUMiLAoJIkFjY2Vzc0tleUlkIjogIjEyMzQ1Njc4OTAxIiwKCSJTZWNyZXRBY2Nlc3NLZXkiOiAidi8xMjM0NTY3ODkwMSIsCgkiVG9rZW4iOiAidmlrZUNURntkMG50X2YwcmczdF9AYjB1dF9uM3R3MHJrX0BjYzM1NX0iLAoJIkV4cGlyYXRpb24iOiAiMjAyMy0wMy0xOVQxNjowMDowMFoiCn0=" | base64 -d
{
  "Code": "Success",
  "LastUpdated": "2023-03-17T16:00:00Z",
  "Type": "AWS-HMAC",
  "AccessKeyId": "12345678901",
  "SecretAccessKey": "v/12345678901",
  "Token": "vikeCTF{d0nt_f0rg3t_@b0ut_n3tw0rk_@cc355}",
  "Expiration": "2023-03-19T16:00:00Z"
}
```

## Flag

```
vikeCTF{d0nt_f0rg3t_@b0ut_n3tw0rk_@cc355}
```
