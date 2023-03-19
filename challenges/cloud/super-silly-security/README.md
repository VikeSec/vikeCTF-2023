# Super Silly Security

**Author: [`MNThomson`](https://github.com/MNThomson)**

**Category: `Cloud Medium`**

## Description

The vikings have founded a new venture, BaaS (Battleaxes-as-a-Service). Their promise of secure storage is enticing, but this is the cloud...is it actually secure?

> http://super-silly-security.vikesec.ca

## Hints

1. What cloud service provides simple storage of static sites
2. That error seems specific, what's `Authenticated AWS User group`?

## Organizers

Run `terraform apply` from this challenge root directory.

## Solution

<details> 
  <summary><b>TLDR:</b> Spoiler Warning</summary>

> The S3 bucket that the website is hosted on has full read from `AWS Authenticated Users`. So as long as your awscli is logged in to an AWS account, `aws s3 cp s3://super-silly-security.vikesec.ca/flag.png flag.png` downloads the flag.

</details>

1. Let's open up the dev tools

   - Taking a look in the inspect element tab to see if we can gather any information from there.
     - We see some HTML mixed with some JavaScript
     - The JavaScript seems related, and we are seeing that error message rendered
   - Moving to the networks tab
     - There is a request being made to `/flag.png` but is getting a `403 Forbidden`...there's our problem!
     - Digging into this request more we can see the headers contain `Server AmazonS3`, `x-amz-id-2: ...`, & `x-amz-request-id: ...`. So the flag (and other static content) **is being stored in [Amazon S3](https://aws.amazon.com/s3/)**

2. Alright, let's take a look at the error

   - Doing some Google searches, we can [find](https://www.trendmicro.com/cloudoneconformity/knowledge-base/aws/S3/s3-bucket-authenticated-users-read-access.html#) [some](https://docs.aws.amazon.com/AmazonS3/latest/userguide/acl-overview.html) [relevant](https://stackoverflow.com/questions/41092658/what-are-s3-default-grantees-any-authenticated-aws-user-and-aws-account-alia) [results](https://www.chrisfarris.com/s3-bucket-security/)
   - Looks like the S3 bucket has the inherently insecure `AWS Authenticated Users` ACL (Access Control List) with `READ` access.

3. AWS Exploitation

   - So to be an authenticated user, we need valid AWS credentials. Either we can [sign up for an AWS account](https://portal.aws.amazon.com/billing/signup) (where a credit card is required) or we can use the AWS credentials provided in another of the Cloud category challenges (shhh....)
   - Now that we have credentials, let's [install the `awscli`](https://docs.aws.amazon.com/cli/latest/userguide/getting-started-install.html) and [configure it](https://docs.aws.amazon.com/cli/latest/userguide/getting-started-quickstart.html)
   - Doing some research on S3, it appears we need a bucket name. The [docs mention](https://docs.aws.amazon.com/AmazonS3/latest/userguide/website-hosting-custom-domain-walkthrough.html) that the name of the bucket must be that of the URL. So our bucket name must be `super-silly-security.vikesec.ca`.
   - Some searching shows that [to download an s3 file](https://stackoverflow.com/questions/45711312/download-s3-bucket-files-on-users-local-using-aws-cli), we need to use the command `aws s3 cp s3://mybucket/test.txt test.txt`.
   - Inserting the values we know, we get `aws s3 cp s3://super-silly-security.vikesec.ca/flag.png flag.png`
   - BOOM, and we now have the flag image locally!

## Flag

```
vikeCTF{c@r3fu1_w!7h_@cc355_c0n7r01$}
```
