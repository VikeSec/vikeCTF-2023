import type { APIRoute } from "astro";
import pkg from "jsonwebtoken";
const { verify, sign } = pkg;
import cookie from "cookie";
import Database from "better-sqlite3";
import querystring from "querystring";

type login_token = { logged_in: boolean };
const secret = process.env.JWT_SECRET || "asdfgthtgrfewdwrgthngbfrw";
const db = Database("users.db");

export function loggedIn(token: string) {
  let token_contents;

  try {
    token_contents = verify(token, secret) as login_token;
  } catch (err) {
    return false;
  }

  return token_contents.logged_in;
}

export const post: APIRoute = async ({ request }) => {
  let body = await request.text();
  const { user, pass } = querystring.parse(body) as {
    user: string;
    pass: string;
  };
  try {
    let statement =
      "SELECT * FROM Users WHERE user='" + user + "' AND pass='" + pass + "'";
    // This package gets mad when there's a semicolon in the query and I'm too
    // tired to search for a 3rd sqlite package (why are they all so bad)
    statement = statement.replace(";", "");
    const row = db.prepare(statement).get();
    if (row == undefined) {
      throw new Error("Custom");
    }
    let token_body: login_token = { logged_in: true };
    let token = sign(token_body, secret);
    return new Response(null, {
      status: 303,
      headers: {
        Location: "/flag",
        "Set-Cookie": cookie.serialize("token", token),
      },
    });
  } catch (err) {
    return new Response(null, {
      status: 303,
      headers: { Location: "/" },
    });
  }
};
