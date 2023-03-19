use std::io;

const PATTERN: &[u8] = "vikeCTF".as_bytes();

fn main() {
    let alphabet: String = ('a'..='z').collect();
    println!("input the text that you'd like decrypted:");

    let mut ciphertext = String::new();
    io::stdin().read_line(&mut ciphertext).unwrap();

    let mut solver = Solver {
        plaintext: String::with_capacity(ciphertext.len()),
        ciphertext: &ciphertext,
        alphabet,
    };

    println!("cracking...");
    for a in 0..26 {
        for b in 0..26 {
            for c in 0..26 {
                for d in 0..26 {
                    for e in 0..26 {
                        let key = [a, b, c, d, e];
                        let solution = solver.decrypt(key);
                        match solution {
                            None => continue,
                            Some(plaintext) => {
                                let key: Vec<_> = key.iter().map(|x| (26 - x) % 26).collect();
                                println!("found the solution with key {:?}: {}", key, plaintext);
                                return;
                            }
                        }
                    }
                }
            }
        }
        println!("{:.2}% of keys tried", (((a as f64) + 1.0) / 26.0) * 100.0)
    }
    println!("no solution found");
}

struct Solver<'a> {
    plaintext: String,
    ciphertext: &'a str,
    alphabet: String,
}

impl Solver<'_> {
    fn decrypt(&mut self, key: [u32; 5]) -> Option<String> {
        self.plaintext.truncate(0);
        let chars = self.ciphertext.char_indices();
        for (i, c) in chars.clone().take(key.len()) {
            self.iter(key, i, c);
        }

        if self.plaintext.as_bytes()[0..key.len()] != PATTERN[0..key.len()] {
            return None;
        }

        for (i, c) in chars.skip(key.len()) {
            self.iter(key, i, c);
        }

        Some(self.plaintext.clone())
    }

    fn iter(&mut self, key: [u32; 5], i: usize, c: char) {
        if !c.is_ascii_alphabetic() {
            self.plaintext.push(c);
            return;
        }

        let offset = c.to_ascii_lowercase() as u32 - 'a' as u32;
        let rotation = key[i % key.len()];

        let result = self
            .alphabet
            .chars()
            .nth((offset + rotation) as usize % self.alphabet.len())
            .unwrap();
        if c.is_lowercase() {
            self.plaintext.push(result);
        } else {
            self.plaintext.push(result.to_ascii_uppercase())
        }
    }
}
