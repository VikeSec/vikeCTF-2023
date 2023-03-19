variable "domain_name" {
  default = "vikesec.ca"
  type    = string
}

variable "subdomain_name" {
  default = "super-silly-security"
  type    = string

}

terraform {
  required_providers {
    aws = {
      version = ">= 4.10.0"
      source  = "hashicorp/aws"
    }
    cloudflare = {
      source  = "cloudflare/cloudflare"
      version = "~> 3.0"
    }

  }
}

provider "aws" {
  region = "us-west-2"
}

provider "cloudflare" {
  api_token = yamldecode(file("~/.config/tokens/cloudflare.yml"))["api-token"]
}

data "aws_canonical_user_id" "current" {}

resource "aws_s3_bucket" "s3_bucket" {
  bucket = format("%s.%s", var.subdomain_name, var.domain_name)
}

resource "aws_s3_bucket_acl" "s3-acl" {
  bucket = aws_s3_bucket.s3_bucket.id

  access_control_policy {
    grant {
      grantee {
        type = "Group"
        uri  = "http://acs.amazonaws.com/groups/global/AuthenticatedUsers"
      }
      permission = "READ"
    }

    owner {
      id = data.aws_canonical_user_id.current.id
    }
  }
}

resource "aws_s3_bucket_policy" "public_read" {
  bucket = aws_s3_bucket.s3_bucket.id
  policy = jsonencode({
    Version = "2012-10-17"
    Statement = [
      {
        Sid       = "PublicReadGetObject"
        Effect    = "Allow"
        Principal = "*"
        Action    = "s3:GetObject"
        Resource = [
          aws_s3_bucket.s3_bucket.arn,
          "${aws_s3_bucket.s3_bucket.arn}/*.html",
          "${aws_s3_bucket.s3_bucket.arn}/*.css",

        ]
      },
    ]
  })
}


resource "aws_s3_bucket_website_configuration" "url" {
  bucket = aws_s3_bucket.s3_bucket.id

  index_document {
    suffix = "index.html"
  }
}


### S3 FILES
resource "aws_s3_object" "flag" {
  bucket       = aws_s3_bucket.s3_bucket.bucket
  key          = "flag.png"
  source       = "src/flag.png"
  etag         = filemd5("src/flag.png")
  content_type = "image/png"
  acl          = "authenticated-read"
}


resource "aws_s3_object" "index" {
  bucket       = aws_s3_bucket.s3_bucket.bucket
  key          = "index.html"
  source       = "src/index.html"
  etag         = filemd5("src/index.html")
  content_type = "text/html"
}

resource "aws_s3_object" "style" {
  bucket       = aws_s3_bucket.s3_bucket.bucket
  key          = "style.css"
  source       = "src/style.css"
  etag         = filemd5("src/style.css")
  content_type = "text/css"
}



### CLOUDFLARE
data "cloudflare_zone" "vikesecca" {
  name = var.domain_name
}

resource "cloudflare_record" "cname" {
  zone_id = data.cloudflare_zone.vikesecca.id
  name    = var.subdomain_name
  value   = aws_s3_bucket_website_configuration.url.website_endpoint
  type    = "CNAME"
  ttl     = 1
  proxied = false
}
