import pkg from "jsonwebtoken";
const { sign, verify } = pkg;

const JWT_SECRET =
  "rUaoBbDDkHW+NJyzToCVwcBxfmuKbjIHh402TqsZAbuvrLoR4BmDPGo+pE21it5P";

export type UserData = {
  username: string;
  img: string;
  mothers_maiden_name: string;
  city: string;
  birthday: string;
  iat: number;
};

async function authorizedUserData(
  auth_cookie: string | undefined
): Promise<UserData | null> {
  if (!auth_cookie) {
    return null;
  }

  try {
    const jwt_cookie = verify(auth_cookie.split(" ")[1]!, JWT_SECRET);
    return jwt_cookie as UserData;
  } catch (error) {
    console.log(auth_cookie, "\n", error);
    return null;
  }
}

async function createSession(user_data: UserData) {
  return sign(user_data, JWT_SECRET);
}

export { authorizedUserData as authorizeUser, createSession };
