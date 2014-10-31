import khmer

def test_consume(fn):
    ht = khmer.new_counting_hash(25, 1e9, 4)
    ht.consume_fasta(fn)

def test_consume_async(fn, np):
    ht = khmer.new_counting_hash(25, 1e9, 4)
    ht.start_async(np)
    ht.consume_fasta_async(fn)
