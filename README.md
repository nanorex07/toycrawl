# Web Crawler written in C


### Frontier Implementation Overview

The crawler frontier is implemented as a disk-backed persistent queue designed to store URLs that are discovered during crawling but not yet processed. The design prioritizes sequential disk access, crash recovery, and simplicity, making it suitable for large crawls where the frontier may grow beyond available memory.

The frontier is implemented using two files stored inside a frontier directory:
	•	frontier.log — an append-only log containing serialized frontier entries
	•	frontier.offset — a small file storing the current read offset (the next item to dequeue)

Each frontier entry contains the following fields:
	•	depth (1 byte) — crawl depth used for BFS traversal
	•	url_len (2 bytes) — length of the URL string
	•	url_hash (8 bytes) — 64-bit hash of the normalized URL
	•	url (variable length) — the actual URL string

The first three fields form a fixed-length header of 11 bytes, followed by the URL bytes.

Records in frontier.log are written in the following format:
[depth:1][url_len:2][url_hash:8][url_bytes:url_len]

All numeric values are written in little-endian format to keep the binary layout predictable.

-----
