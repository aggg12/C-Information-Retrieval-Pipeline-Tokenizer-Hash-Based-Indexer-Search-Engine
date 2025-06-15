I built a complete C++ IR pipeline split into three components:

- **Tokenizador**: a configurable tokenizer that segments text (files, directories, file-lists) into word tokens, handling special cases (URLs, e-mails, decimals, acronyms, hyphenated compounds) and optional lowercase/diacritic folding.  
- **IndexadorHash**: a hash-based indexer that, given a list of documents and a stopword list, constructs and stores an inverted index (term→posting lists with frequencies, positions, document metadata, collection statistics), supports re-indexing on file changes, question indexing, and on-disk serialization/recovery.  
- **Buscador**: a searcher that loads the saved index, accepts a query (or query batch), computes document scores under DFR and Okapi BM25 models, and returns the top N results via a priority queue, with options to print ranked lists or overlay hits in TREC format. 
