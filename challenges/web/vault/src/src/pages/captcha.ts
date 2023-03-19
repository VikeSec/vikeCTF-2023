import type { APIRoute } from "astro";
import pkg from "jsonwebtoken";
const { verify, sign } = pkg;
import cookie from "cookie";
import captcha from "svg-captcha";
import crypto from "crypto-js";

type token_body = { remaining_captchas: number; captcha: string };
type response_body = { svg: string; remaining_captchas: number };
const secret = process.env.JWT_SECRET || "asdfgthtgrfewdwrgthngbfrw";
const max_captchas = 50;

function newCaptcha(remaining_captchas: number): Response {
  let { data, text } = captcha.create({
    size: 4,
    noise: 3,
    color: true,
    ignoreChars: "I",
  });
  let text_enc = crypto.AES.encrypt(text, secret).toString();
  let new_token_body: token_body = {
    remaining_captchas: remaining_captchas,
    captcha: text_enc,
  };
  let token = sign(new_token_body, secret);
  let body: response_body = {
    svg: data,
    remaining_captchas: remaining_captchas,
  };
  return new Response(JSON.stringify(body), {
    headers: {
      "Set-Cookie": cookie.serialize("token", token),
      "Content-type": "application/json",
    },
  });
}

export const post: APIRoute = async ({ request }) => {
  const { solution } = (await request.json()) as { solution: string };
  let cookie_str = request.headers.get("Cookie") || "";
  let token = cookie.parse(cookie_str).token || "";
  let token_contents;

  try {
    token_contents = verify(token, secret) as token_body;
  } catch (err) {
    return newCaptcha(max_captchas);
  }

  if (!token_contents.captcha || !token_contents.remaining_captchas) {
    return newCaptcha(max_captchas);
  }

  const bytes = crypto.AES.decrypt(token_contents.captcha, secret);
  const correct_solution = bytes.toString(crypto.enc.Utf8);

  if (correct_solution === solution) {
    if (token_contents.remaining_captchas == 1) {
      token_contents.remaining_captchas = 0;
    }
    return newCaptcha(token_contents.remaining_captchas - 1);
  } else {
    return newCaptcha(max_captchas);
  }
};
