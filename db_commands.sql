CREATE TABLE IF NOT EXISTS users
(
    id   INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS authors
(
    id   INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS genres
(
    id   INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS books
(
    id    INTEGER PRIMARY KEY AUTOINCREMENT,
    title TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS comments
(
    id      INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER,
    book_id INTEGER,
    content TEXT NOT NULL,
    FOREIGN KEY(user_id) REFERENCES users (id),
    FOREIGN KEY(book_id) REFERENCES books (id)
);

CREATE TABLE IF NOT EXISTS favourites
(
    user_id INTEGER NOT NULL,
    book_id INTEGER NOT NULL,
    FOREIGN KEY(user_id) REFERENCES users (id),
    FOREIGN KEY(book_id) REFERENCES books (id),
    PRIMARY KEY (user_id, book_id)
);

CREATE TABLE IF NOT EXISTS rentals
(
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id     INTEGER,
    book_id     INTEGER,
    rent_date   DATE DEFAULT CURRENT_DATE,
    return_date DATE,
    FOREIGN KEY(user_id) REFERENCES users (id),
    FOREIGN KEY(book_id) REFERENCES books (id)
);

CREATE TABLE IF NOT EXISTS authors_books_join_table
(
    authors_id INTEGER,
    book_id    INTEGER,
    FOREIGN KEY(authors_id) REFERENCES authors (id),
    FOREIGN KEY(book_id) REFERENCES books (id),
    PRIMARY KEY(authors_id, book_id)
);

CREATE TABLE IF NOT EXISTS genres_books_join_table
(
    genres_id INTEGER,
    book_id   INTEGER,
    FOREIGN KEY(genres_id) REFERENCES genres (id),
    FOREIGN KEY(book_id) REFERENCES books (id),
    PRIMARY KEY(genres_id, book_id)
);

